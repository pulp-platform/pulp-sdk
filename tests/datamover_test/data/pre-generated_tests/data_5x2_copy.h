#pragma once

#include <stdint.h>

// Configuration Parameters
#define DATAMOVER_MODE 0
#define TRANSP_MODE 0
#define CIM_MODE 0
#define ROW_TILE_SIZE 64
#define SIZE_C 1
#define SIZE_M 5
#define SIZE_N 2

uint8_t golden_in [SIZE_M*SIZE_N] = {
  0x88, 0x81, 0x88, 0x8a, 0xaf, 0x36, 0x6f, 0x3d, 0x53, 0xfa
};

uint8_t golden_out [SIZE_M*SIZE_N] = {
  0x88, 0x81, 0x88, 0x8a, 0xaf, 0x36, 0x6f, 0x3d, 0x53, 0xfa
};
