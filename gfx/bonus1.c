// convpng v7.2
#include <stdint.h>
#include "sprite.h"

// 8 bpp image
uint8_t bonus1_data[902] = {
 30,30,  // width,height
 0xEC,0xEC,0xE3,0x2F,0x5E,0x5E,0x2F,0x43,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
 0xBB,0xC8,0xF3,0x58,0x58,0xBA,0x58,0x2F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x54,0x4B,0x54,0x54,0x54,0x54,0x54,0x54,0x4B,0x33,0x00,0x00,0x00,0x00,0x00,
 0x7B,0x34,0xF3,0xE2,0xE2,0x98,0x98,0xA2,0x6A,0x00,0x00,0x00,0x20,0x6A,0xBA,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0xA2,0x19,0x00,0x00,0x00,0x00,
 0x6C,0x30,0xD3,0xE2,0x8E,0x8E,0xA7,0x98,0x6E,0x4B,0x47,0x09,0xBA,0x58,0x58,0x58,0x6E,0x6E,0x6E,0x6E,0x6E,0x6E,0x6E,0x6E,0x46,0x83,0x00,0x00,0x00,0x00,
 0x6C,0x94,0xF3,0xE2,0xC2,0xC2,0x95,0xA7,0x86,0xA2,0xA9,0x86,0xBA,0xA2,0x89,0x5E,0x06,0x54,0x54,0x54,0x4B,0x4B,0x4B,0x4B,0x2F,0x00,0x00,0x00,0x00,0x00,
 0xC4,0x70,0xE0,0x58,0xA2,0xA2,0x58,0x98,0xA7,0xDB,0xA7,0xBA,0x89,0x4B,0x54,0x6A,0x6A,0x5E,0x5E,0xAC,0x54,0x54,0x4B,0x09,0x27,0x4B,0x00,0x00,0x00,0x00,
 0xBD,0xBD,0xDC,0x0F,0x0F,0x0F,0x5B,0x58,0xDB,0x7D,0xDB,0x6E,0x6A,0x89,0x89,0x6A,0x6A,0x5E,0x5E,0xAC,0x54,0x4B,0x54,0x5B,0x4F,0x06,0x9A,0x00,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1C,0x58,0xA7,0xDB,0x58,0x0A,0x89,0xAC,0x54,0x06,0x54,0x54,0x54,0x54,0x4B,0x09,0x84,0x84,0x4A,0x5E,0x83,0x00,0x00,
 0xFE,0xFE,0x1F,0x1F,0x1F,0x1F,0x1F,0x00,0x14,0x86,0xA7,0x98,0x58,0x87,0x4B,0xDF,0x8F,0x8F,0x8F,0xDF,0xD3,0x54,0x4B,0x17,0x84,0x21,0xAC,0x2F,0x00,0x00,
 0x00,0x00,0x00,0x00,0xFE,0x00,0xFE,0x1F,0x00,0xCB,0x86,0xA7,0x98,0xA2,0x7A,0x25,0x25,0x25,0x25,0x25,0x25,0xDF,0x4B,0x4B,0x06,0x0E,0x06,0x5E,0x8B,0x00,
 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x4C,0x4C,0x00,0xBA,0x86,0xA7,0x86,0x6E,0x74,0x25,0x25,0x25,0x25,0x25,0x25,0xD3,0x54,0x4B,0x19,0x1C,0x2F,0x53,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBA,0x98,0xE2,0x86,0x6E,0x3E,0x25,0x25,0x25,0x25,0x25,0x8F,0x54,0x4B,0x06,0x18,0xAC,0x9A,0x43,
 0xF0,0xF0,0x48,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x2F,0xF8,0x7D,0xA7,0x86,0x41,0x52,0x25,0x25,0x25,0x25,0x25,0xD3,0x4B,0x10,0x33,0x54,0x2F,0x46,
 0xB6,0x78,0x85,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x8E,0x8E,0x7D,0xDB,0x58,0x40,0x8F,0x25,0x25,0x25,0x25,0xDF,0x4B,0x06,0x06,0x54,0x2F,0x46,
 0x6C,0x30,0xD3,0xE2,0x7D,0x7D,0x7D,0x7D,0x7D,0x7D,0x7D,0x8E,0xC9,0xC9,0x7D,0xA7,0x86,0x46,0x7A,0x25,0x25,0x25,0x25,0xDF,0x54,0x4B,0x4B,0x54,0x2F,0x46,
 0x6C,0x94,0xF3,0xE2,0x95,0x95,0xE6,0xE6,0xE6,0xE6,0xE6,0xE6,0xC9,0xC9,0x7D,0xA7,0x86,0x46,0x7A,0x25,0x25,0x25,0x25,0xDF,0x54,0x4B,0x4B,0x54,0x2F,0x46,
 0xC4,0x28,0xE0,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x8E,0x8E,0x7D,0xDB,0x58,0x40,0x8F,0x25,0x25,0x25,0x25,0xDF,0x4B,0x06,0x06,0x54,0x2F,0x87,
 0xBF,0xBF,0xB0,0x83,0x83,0x83,0x83,0x83,0x83,0x83,0x7C,0x5E,0xF8,0x7D,0xA7,0x86,0x41,0x52,0x25,0x25,0x25,0x25,0x25,0xD3,0x4B,0x10,0x33,0x54,0x2F,0x0A,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xBA,0x98,0xE2,0x86,0x6E,0x3E,0x25,0x25,0x25,0x25,0x25,0x8F,0x54,0x4B,0x06,0x27,0xAC,0x4B,0x83,
 0x01,0x01,0x1F,0x1F,0x1F,0x1F,0x1F,0x4C,0x1F,0x00,0xBA,0x86,0xA7,0x86,0x6E,0x74,0x25,0x25,0x25,0x25,0x25,0x25,0xD3,0x54,0x4B,0x19,0x1C,0x2F,0x53,0x00,
 0x00,0x00,0x00,0x00,0xFE,0x00,0xFE,0xFE,0x80,0xCB,0x86,0xA7,0x98,0xA2,0x7A,0x25,0x25,0x25,0x25,0x25,0x25,0xDF,0x4B,0x4B,0x06,0x0E,0x06,0x5E,0x8B,0x00,
 0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0xFE,0xCB,0x86,0xA7,0x98,0x58,0x87,0x4B,0xDF,0x8F,0x8F,0x8F,0xDF,0xD3,0x54,0x4B,0x17,0x84,0x21,0xAC,0x2F,0x00,0x00,
 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x27,0x58,0xA7,0xDB,0x58,0x0A,0x89,0xAC,0x54,0x06,0x54,0x54,0x54,0x54,0x4B,0x09,0x84,0x84,0x4A,0x5E,0x7C,0x00,0x00,
 0xDC,0xDC,0x20,0x0F,0x0F,0x0F,0x5B,0x58,0xDB,0x7D,0xDB,0x6E,0x6A,0x89,0x89,0x6A,0x6A,0x5E,0x5E,0xAC,0x54,0x4B,0x54,0x5B,0x4F,0x10,0x9A,0x00,0x00,0x00,
 0xB6,0xBB,0x85,0xA2,0xA2,0xA2,0x58,0x98,0xA7,0xDB,0xA7,0xBA,0x6A,0x4B,0x54,0x6A,0x6A,0x5E,0x5E,0xAC,0x54,0x54,0x4B,0x09,0x18,0x4B,0xFE,0x00,0x00,0x00,
 0x60,0x34,0xF3,0xE2,0xE2,0xE2,0xE2,0xA7,0x86,0xA2,0x31,0x86,0xBA,0xA2,0x89,0x5E,0x06,0x54,0x54,0x54,0x4B,0x4B,0x4B,0x4B,0x2F,0x4C,0x00,0x00,0x00,0x00,
 0x6C,0x30,0xD3,0xE2,0x7D,0x7D,0xDB,0x98,0x6E,0x4B,0x47,0x09,0xBA,0x58,0x58,0x58,0x6E,0x6E,0x6E,0x6E,0x6E,0x6E,0x6E,0x89,0x46,0x83,0x00,0x00,0x00,0x00,
 0x6C,0x50,0xF3,0xE2,0x95,0xDB,0x98,0xA2,0x6A,0x00,0x00,0x00,0x20,0x6A,0xBA,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0x58,0xA2,0x19,0x00,0x00,0x00,0x00,
 0x65,0x70,0xE0,0x58,0x58,0x58,0xA2,0x6A,0x1F,0x00,0x00,0x00,0x00,0x00,0x00,0x4B,0x54,0x54,0x54,0x54,0x54,0x54,0x54,0x4B,0x33,0x00,0x00,0x00,0x00,0x00,
 0xA8,0xA8,0xB2,0x9A,0x9A,0x9A,0x9A,0x35,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};