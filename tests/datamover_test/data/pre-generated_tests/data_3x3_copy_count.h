#pragma once

#include <stdint.h>

// Configuration Parameters
#define DATAMOVER_MODE 0
#define TRANSP_MODE 0
#define CIM_MODE 0
#define ROW_TILE_SIZE 64
#define SIZE_C 1
#define SIZE_M 3
#define SIZE_N 3

uint8_t golden_in [SIZE_M*SIZE_N] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};

uint8_t golden_out [SIZE_M*SIZE_N] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08
};
