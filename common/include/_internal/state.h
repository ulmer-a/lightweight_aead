#pragma once

#include <stdint.h>

typedef union {
  uint64_t x;
  uint32_t w[2];
  uint8_t  b[8];
} word_t;

typedef union
{
    uint64_t x;
    uint32_t w[2];
    uint8_t b[8];
} lane_t;

typedef struct {
    uint32_t l;
    uint32_t h;
} my_word_t;

typedef union {
    lane_t l[5];
    uint8_t b[5][8];
    uint32_t w32[5][2];
uint64_t x[5];
my_word_t   w[5];
} state_t;

typedef struct
{
    uint32_t e;
    uint32_t o;
} u32_2;
