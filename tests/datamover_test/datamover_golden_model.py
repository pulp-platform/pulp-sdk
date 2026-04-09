# Copyright 2026 ETH Zurich and University of Bologna.
# Licensed under the Apache License, Version 2.0, see LICENSE for details.
# SPDX-License-Identifier: Apache-2.0
#
# Authors: Cyrill Durrer <cdurrer@iis.ee.ethz.ch>

import os
import math
import argparse
import numpy as np

def data_header_format(data, elements_per_line=16):
    lines = []
    for i in range(0, len(data), elements_per_line):
        line_elements = data[i:i + elements_per_line]
        formatted_elements = [f"0x{elem:02x}" for elem in line_elements]
        if i + elements_per_line < len(data):
            line = "  " + ", ".join(formatted_elements) + ","
        else:
            line = "  " + ", ".join(formatted_elements)
        lines.append(line)
    return lines

def write_data_header_file(output_dir, input_tensor, output_tensor, config_params, filename="data.h"):
    os.makedirs(output_dir, exist_ok=True)
    filepath = os.path.join(output_dir, filename)

    input_flat = np.asarray(input_tensor, dtype=np.uint8).reshape(-1).tolist()
    output_flat = np.asarray(output_tensor, dtype=np.uint8).reshape(-1).tolist()

    data_h_string = [
        "#pragma once",
        "",
        "#include <stdint.h>",
        "",
        "// Configuration Parameters",
        f"#define DATAMOVER_MODE {config_params['datamover_mode']}",
        f"#define TRANSP_MODE {config_params['transp_mode']}",
        f"#define CIM_MODE {config_params['cim_mode']}",
        f"#define ROW_TILE_SIZE {config_params['row_tile_size']}",
        f"#define SIZE_C {config_params['size_c']}",
        f"#define SIZE_M {config_params['size_m']}",
        f"#define SIZE_N {config_params['size_n']}",
        "",
        # "uint8_t golden_in [SIZE_C*SIZE_M*SIZE_N] = {",
        "PI_L1 uint8_t golden_in [SIZE_C*SIZE_M*SIZE_N] = {",       # PI_L1 only for GVSoC (siracusa)
    ]
    data_h_string.extend(data_header_format(input_flat))
    data_h_string.extend([
        "};",
        "",
        # "uint8_t golden_out [SIZE_C*SIZE_M*SIZE_N] = {",
        "PI_L1 uint8_t golden_out [SIZE_C*SIZE_M*SIZE_N] = {",      # PI_L1 only for GVSoC (siracusa)
    ])
    data_h_string.extend(data_header_format(output_flat))
    data_h_string.extend([
        "};",
        "",
    ])

    with open(filepath, "w", encoding="utf-8") as file:
        file.write("\n".join(data_h_string))

    return filepath

def cim_layout(tensor, row_tile_size, size_m, size_n):
    # Input tensor shape: (size_m, size_n)
    # Output shape: (1, size_m * size_n)
    # Transformation: columns are grouped into tiles of row_tile_size;
    # for each tile, all rows' slices are stored consecutively in the output.

    # size_m, size_n = tensor.shape
    n_tiles = size_n // row_tile_size
    leftover = size_n % row_tile_size

    # Reshape to (size_m, n_tiles, row_tile_size), transpose to (n_tiles, size_m, row_tile_size), flatten
    parts = [tensor[:, :n_tiles * row_tile_size].reshape(size_m, n_tiles, row_tile_size).transpose(1, 0, 2).reshape(-1)]
    if leftover > 0:
        parts.append(tensor[:, n_tiles * row_tile_size:].reshape(-1))
    return np.concatenate(parts).reshape(1, -1)

def cim_layout_reverse(tensor, row_tile_size, size_m, size_n):
    # Input tensor shape: (1, size_m * size_n) in CIM layout
    # Output shape: (size_m, size_n) in row-major layout
    # size_m, size_n are the dimensions of the original row-major tensor

    n_tiles = size_n // row_tile_size
    leftover = size_n % row_tile_size
    flat = tensor.reshape(-1)

    # Reshape to (n_tiles, size_m, row_tile_size), transpose to (size_m, n_tiles, row_tile_size)
    complete = flat[:n_tiles * size_m * row_tile_size].reshape(n_tiles, size_m, row_tile_size).transpose(1, 0, 2)
    if leftover > 0:
        leftover_part = flat[n_tiles * size_m * row_tile_size:].reshape(size_m, leftover)
        return np.concatenate([complete.reshape(size_m, n_tiles * row_tile_size), leftover_part], axis=1)
    return complete.reshape(size_m, size_n)

def cim_layout_transpose(tensor, row_tile_size, size_m, size_n):
    # Input tensor in CIM-layout (size_m and size_n are the dimensions of the original row-major tensor)
    # Output tensor: transposed input tensor in CIM-layout
    row_major_tensor = cim_layout_reverse(tensor, row_tile_size, size_m, size_n)
    row_major_transposed = np.transpose(row_major_tensor)
    output_tensor = cim_layout(row_major_transposed, row_tile_size, size_n, size_m)
    return output_tensor


def unfold(tensor, patch_size):
    # Input tensor shape: (CHANNELS, HEIGHT, WIDTH)
    # Output tensor shape: (PATCH_SIZE, NUM_PATCHES, CHANNELS)
    channels, height, width = tensor.shape
    patch_sidelength = int(math.sqrt(patch_size))
    assert (height % patch_sidelength == 0) and (width % patch_sidelength == 0), "Height and Width must be divisible by patch sidelength"
    num_patches_h = height // patch_sidelength
    num_patches_w = width // patch_sidelength
    num_patches = num_patches_h * num_patches_w
    tensor_unfolded = np.zeros((patch_size, num_patches, channels), dtype=tensor.dtype)
    for p in range(patch_size):
        for h in range(num_patches_h):
            for w in range(num_patches_w):
                n = h * num_patches_w + w
                h_idx = h * patch_sidelength + (p // patch_sidelength)
                w_idx = w * patch_sidelength + (p % patch_sidelength)
                tensor_unfolded[p, n, :] = tensor[:, h_idx, w_idx]
    return tensor_unfolded

def fold(tensor, patch_size, num_channels, height, width):  # Parameters: output tensor dimensions (CHW)
    # Input tensor shape: (PATCH_SIZE, NUM_PATCHES, CHANNELS) -- calculated from output dimensions
    # Output tensor shape: (CHANNELS, HEIGHT, WIDTH) -- num_channels, height, width are the folded dimensions
    patch_sidelength = int(math.sqrt(patch_size))
    # size_n = (height * width) // patch_size
    assert (height % patch_sidelength == 0) and (width % patch_sidelength == 0), "Height and Width must be divisible by patch sidelength"
    num_patches_h = height // patch_sidelength
    num_patches_w = width // patch_sidelength
    tensor_folded = np.zeros((num_channels, height, width), dtype=tensor.dtype)
    for p in range(patch_size):
        for h in range(num_patches_h):
            for w in range(num_patches_w):
                n = h * num_patches_w + w
                h_idx = h * patch_sidelength + (p // patch_sidelength)
                w_idx = w * patch_sidelength + (p % patch_sidelength)
                tensor_folded[:, h_idx, w_idx] = tensor[p, n, :]
    return tensor_folded

def main():
    parser = argparse.ArgumentParser(description="Datamover golden model generator")
    parser.add_argument("--datamover-mode", type=int, default=0,       dest="datamover_mode",  help="0: copy, 1: transpose, 2: CIM layout, 3: CIM layout transpose, 4: unfold, 5: fold")
    parser.add_argument("--transp-mode",    type=int, default=1,       dest="transp_mode",     help="Transpose element width: 1, 2, or 4")
    parser.add_argument("--cim-mode",       type=int, default=0,       dest="cim_mode",        help="0: row-major -> CIM-layout, 1: CIM-layout -> row-major")
    parser.add_argument("--row-tile-size",  type=int, default=64,      dest="row_tile_size",   help="CIM row tile size (inner dimension)")
    parser.add_argument("--size-c",         type=int, default=1,       dest="size_c",          help="Tensor dimension C")
    parser.add_argument("--size-m",         type=int, default=1,       dest="size_m",          help="Tensor dimension M (rows)")
    parser.add_argument("--size-n",         type=int, default=1,       dest="size_n",          help="Tensor dimension N (columns)")
    parser.add_argument("--patch-size",     type=int, default=4,       dest="patch_size",      help="MobileViT patch size (unfold/fold)")
    parser.add_argument("--memory-size",    type=int, default=1048576,      dest="memory_size",     help="TCDM size in Bytes")
    parser.add_argument("--output_filename",type=str, default="data.h",      dest="output",          help="Output header filename")
    parser.add_argument("--count",          action="store_true",       dest="count",           help="Use counting stimuli instead of random (for debugging)")
    args = parser.parse_args()

    # Check configuration
    # required_elements = 3 * args.size_c * args.size_m * args.size_n
    # assert required_elements <= args.memory_size, (
    #     f"MEMORY_SIZE ({args.memory_size}) is too small: requires at least "
    #     f"3*SIZE_C*SIZE_M*SIZE_N = {required_elements} elements"
    # )

    # Create a tensor of size (SIZE_C, SIZE_M, SIZE_N) with random or counting values
    if not args.count:
        input_tensor = np.random.randint(0, 256, (args.size_c, args.size_m, args.size_n), dtype=np.uint8)
    else:
        input_tensor = np.arange(args.size_c * args.size_m * args.size_n, dtype=np.uint8).reshape((args.size_c, args.size_m, args.size_n))

    # Generate golden output tensor based on the selected datamover mode
    if args.datamover_mode==0:
        output_tensor = input_tensor.copy()
    elif args.datamover_mode==1:
        output_tensor = np.transpose(input_tensor, (0, 2, 1))
    elif args.datamover_mode==2:
        if args.cim_mode == 0:   # row-major -> CIM-layout
            output_tensor = cim_layout(input_tensor.reshape(args.size_m, args.size_n), args.row_tile_size, args.size_m, args.size_n)
        elif args.cim_mode == 1: # CIM-layout -> row-major (reverse of mode 0)
            output_tensor = cim_layout_reverse(input_tensor.reshape(args.size_m, args.size_n), args.row_tile_size, args.size_m, args.size_n)
        else:
            raise ValueError(f"Unsupported CIM_MODE: {args.cim_mode}")
    elif args.datamover_mode==3:
        input_tensor = input_tensor.reshape(args.size_m, args.size_n)
        output_tensor = cim_layout_transpose(input_tensor, args.row_tile_size, args.size_m, args.size_n)
    elif args.datamover_mode==4:
        output_tensor = unfold(input_tensor, args.patch_size)
    elif args.datamover_mode==5:
        # For fold mode, generate the input as an unfolded tensor by first unfolding a counting tensor
        # base_tensor = np.arange(SIZE_C * SIZE_M * SIZE_N, dtype=np.uint8).reshape((SIZE_C, SIZE_M, SIZE_N))
        unfolded_tensor = unfold(input_tensor, args.patch_size)
        input_tensor = unfolded_tensor.copy()  # Use the unfolded tensor as input for fold mode
        print("\nUnfolded Tensor (input for fold mode):")
        print(unfolded_tensor)
        output_tensor = fold(unfolded_tensor, args.patch_size, args.size_c, args.size_m, args.size_n)
    else:
        raise ValueError(f"Unsupported DATAMOVER_MODE: {args.datamover_mode}")

    print("Input Tensor:")
    print(input_tensor)
    print("\nOutput Tensor:")
    print(output_tensor)

    output_dir = os.path.join(os.path.dirname(__file__), "data")
    config_params = {
        "datamover_mode": args.datamover_mode,
        "transp_mode":    args.transp_mode,
        "cim_mode":       args.cim_mode,
        "row_tile_size":  args.row_tile_size,
        "size_c":   args.size_c,
        "size_m":   args.size_m,
        "size_n":   args.size_n,
    }
    header_file = write_data_header_file(output_dir, input_tensor, output_tensor, config_params, args.output)
    print(f"\nWrote golden header to: {header_file}")

if __name__ == "__main__":
    main()
