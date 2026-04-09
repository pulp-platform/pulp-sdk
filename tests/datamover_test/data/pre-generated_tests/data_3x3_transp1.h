#pragma once

#include <stdint.h>

// Configuration Parameters
#define DATAMOVER_MODE 1
#define TRANSP_MODE 1
#define CIM_MODE 0
#define ROW_TILE_SIZE 64
#define SIZE_C 1
#define SIZE_M 3
#define SIZE_N 3

PI_L1 uint8_t golden_in [SIZE_M*SIZE_N] = {
  0x49, 0xd0, 0xe9, 0xee, 0xb5, 0xdc, 0x74, 0xe9, 0x7a
};

PI_L1 uint8_t golden_out [SIZE_M*SIZE_N] = {
  0x49, 0xee, 0x74, 0xd0, 0xb5, 0xe9, 0xe9, 0xdc, 0x7a
};
