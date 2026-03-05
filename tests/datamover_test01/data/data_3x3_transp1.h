#pragma once

#include <stdint.h>

// Configuration Parameters
#define BANDWIDTH 512
#define WORD_WIDTH 64
#define ELEM_WIDTH 8
#define MEMORY_SIZE 65536
#define MISALIGNED_ACCESSES 0
#define DATAMOVER_MODE 1
#define TRANSP_MODE 1
#define CIM_MODE 0
#define CIM_INNER_DIM 64
#define CIM_OUTER_DIM 64
#define SIZE_M 3
#define SIZE_N 3

PI_L1 uint8_t golden_in [SIZE_M*SIZE_N] = {
  0x49, 0xd0, 0xe9, 0xee, 0xb5, 0xdc, 0x74, 0xe9, 0x7a
};

PI_L1 uint8_t golden_out [SIZE_M*SIZE_N] = {
  0x49, 0xee, 0x74, 0xd0, 0xb5, 0xe9, 0xe9, 0xdc, 0x7a
};
