// convpng v7.2
#include <stdint.h>
#include "sprite.h"

// 8 bpp image
uint8_t brique_data[902] = {
 30,30,  // width,height
 0x40,0x40,0x3F,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,
 0x40,0x40,0x3F,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x23,0x39,0x39,
 0x3F,0x3F,0x56,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x66,0x74,0x5F,0x23,
 0x39,0x5F,0x3E,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0x81,0xB6,0x23,
 0x23,0x56,0x52,0x81,0x78,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x78,0x78,0x22,0x22,0x22,0x22,0x81,0x52,0x56,
 0x23,0x56,0x3A,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x78,0x81,0x52,0x56,
 0x23,0x56,0x3A,0x78,0x78,0x78,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x78,0x78,0x22,0x22,0x22,0x22,0x22,0x22,0x78,0x22,0x22,0x78,0x81,0x52,0x56,
 0x23,0x56,0x3A,0x22,0x22,0x78,0x78,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x78,0x78,0x72,0x16,0x72,0x22,0x22,0x81,0x52,0x56,
 0x23,0x56,0x3A,0x22,0x72,0x78,0x72,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x78,0x78,0x22,0x22,0x22,0x78,0x78,0x16,0x16,0x16,0x16,0x72,0x78,0x81,0x52,0x56,
 0x23,0x56,0x2C,0x72,0x16,0x72,0x78,0x78,0x78,0x22,0x22,0x22,0x78,0x78,0x78,0x22,0x78,0x72,0x78,0x22,0x22,0x72,0x0D,0x16,0x16,0x16,0x72,0x81,0x52,0x56,
 0x23,0x56,0x3E,0x3A,0x72,0x16,0x72,0x22,0x22,0x78,0x72,0x78,0x22,0x22,0x78,0x72,0x16,0x16,0x16,0x02,0x16,0x16,0x16,0x16,0x16,0x16,0x72,0x81,0x52,0x56,
 0x23,0x56,0x3E,0x3A,0x0D,0x16,0x16,0x72,0x72,0x16,0x16,0x22,0x22,0x78,0x72,0x16,0x0D,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x72,0x16,0x78,0x81,0x52,0x56,
 0x23,0x56,0x3E,0x2C,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x72,0x78,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x78,0x52,0x56,
 0x23,0x56,0x55,0x88,0x3A,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x3A,0x3E,0x56,
 0x23,0x23,0x5A,0x55,0x2C,0x3A,0x75,0x3A,0x75,0x75,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x3A,0x3E,0x56,
 0x23,0x23,0x55,0x3E,0x3A,0x2C,0x75,0x2C,0x75,0x75,0x3A,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x2C,0x74,0x56,
 0x23,0x56,0x3E,0x3A,0x3A,0x2C,0x3A,0x75,0x55,0x55,0x88,0x3E,0x75,0x75,0x3A,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x16,0x2C,0x88,0x5A,0x23,
 0x23,0x56,0x2C,0x3A,0x75,0x75,0x3E,0x88,0x55,0x55,0x55,0x55,0x88,0x3E,0x75,0x75,0x75,0x3A,0x3A,0x75,0x16,0x16,0x16,0x16,0x75,0x3A,0x2C,0x55,0x5A,0x23,
 0x3F,0x56,0x3E,0x2C,0x75,0x88,0x88,0x88,0x55,0x55,0x55,0x55,0x88,0x3E,0x55,0x88,0x75,0x2C,0x3A,0x75,0x16,0x16,0x16,0x16,0x3A,0x75,0x2C,0x3E,0x5A,0x3F,
 0x3F,0x23,0x66,0x2C,0x3E,0x88,0x55,0x74,0x55,0x55,0x55,0x55,0x55,0x88,0x55,0x55,0x88,0x2C,0x75,0x75,0x3A,0x16,0x16,0x3A,0x3A,0x75,0x88,0x66,0x7A,0x3F,
 0x40,0x3F,0x5A,0x88,0x3E,0x3E,0x55,0x55,0x5A,0x5A,0x55,0x55,0x55,0x55,0x55,0x55,0x88,0x88,0x3E,0x2C,0x3A,0x3A,0x3A,0x3A,0x2C,0x3E,0x88,0x5A,0x39,0x40,
 0x40,0x3F,0x5A,0x55,0x55,0x55,0x55,0x5A,0x56,0x5A,0x5A,0x55,0x55,0x5A,0x5A,0x74,0x74,0x55,0x55,0x55,0x88,0x88,0x88,0x88,0x88,0x88,0x55,0x5F,0x3F,0x40,
 0x40,0x3F,0x5F,0x5A,0x5A,0x5A,0x5A,0x56,0x5F,0x5F,0x5A,0x5A,0x5A,0x56,0x5F,0x3F,0x23,0x5F,0x56,0x5A,0x5A,0x5A,0x56,0x23,0x5A,0x5A,0x5A,0x23,0x40,0x41,
 0x3F,0x3F,0x5F,0x5A,0x5A,0x5A,0x5A,0x5F,0x23,0x23,0x5F,0x5F,0x5A,0x5F,0x3F,0x3F,0x3F,0x23,0x23,0x23,0x5F,0x23,0x3F,0x3F,0x23,0x5A,0x5F,0x3F,0x40,0x41,
 0x3F,0x3F,0x5F,0x5A,0x5A,0x5A,0x5F,0x5F,0x23,0x23,0x23,0x23,0x5F,0x5F,0x23,0x3F,0x3F,0x3F,0x40,0x3F,0x23,0x23,0x3F,0x3F,0x23,0x23,0x23,0x3F,0x41,0x41,
 0x40,0x39,0x5F,0x5A,0x5A,0x56,0x23,0x5F,0x23,0x23,0x23,0x23,0x23,0x23,0x5F,0x5F,0x39,0x3F,0x40,0x40,0x3F,0x39,0x39,0x39,0x39,0x39,0x3F,0x39,0x40,0x41,
 0x3F,0x39,0x5F,0x5F,0x5F,0x5F,0x23,0x23,0x5F,0x5F,0x5F,0x23,0x23,0x5F,0x5A,0x5A,0x5F,0x3F,0x40,0x40,0x3F,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x39,0x40,
 0x40,0x40,0x39,0x5F,0x39,0x39,0x39,0x39,0x5F,0x5F,0x5F,0x5F,0x3F,0x3F,0x5F,0x23,0x39,0x3F,0x40,0x40,0x39,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x3F,0x39,0x41,
 0x41,0x41,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x41,0x40,0x40,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x40,0x40,0x41,0x41,0x41,0x41,0x61,
 0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x61,0x61,
};
