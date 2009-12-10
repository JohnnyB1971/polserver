//////////////////////////////////////////////////////////////////////
//
// crypt/nocrypt.cpp
//
// Author:    Beosil <beosil@swileys.com>
// Date:      18. Apr. 2000
//
// Converted: Myrathi <tjh@myrathi.co.uk>
// Date:      03. Jan. 2003
//
// Versions:  no encryption (ignition)
//
// Copyright (C) 1999-2000 Bruno 'Beosil' Heidelberger
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
//
// History:
// - 03. Jan. 2003 : Updated for use alongside POL's multi-encryption code.
// - 18. Apr. 2000 : Keys updated for client 2.0.0
// - 27. Jan. 2000 : Keys updated for client 1.26.4
// - 18. Jan. 2000 : Keys updated for client 1.26.3
// - 23. Nov. 1999 : Keys updated for client 1.26.2 and some compatibility fixes
// - 21. Sep. 1999 : Full redesign to fix the "21036 bytes"-bug
// -  9. Sep. 1999 : Keys updated for client 1.26.1
// -  2. Sep. 1999 : Keys and boxes updated for client 1.26.0b and minor bugfixes
// - 13. Aug. 1999 : First release, working with client 1.26.0(a)
//
//////////////////////////////////////////////////////////////////////

#include <memory.h>
#include "nocrypt.h"

// Seed Table

static unsigned char seed_table[2][CRYPT_GAMESEED_COUNT][2][CRYPT_GAMESEED_LENGTH] =
{
	0x9E, 0xEC, 0x5B, 0x3C, 0x8F, 0xA8, 0x8C, 0x55,
	0xB6, 0x21, 0x71, 0x98, 0xA4, 0x47, 0x22, 0x58, 
	0xF8, 0xC4, 0xD8, 0x72, 0x54, 0xFC, 0xF9, 0xDE,
	0x2D, 0x53, 0xDB, 0x32, 0x03, 0x10, 0x5A, 0x18, 
	0x89, 0x9F, 0x5C, 0x53, 0x06, 0x7F, 0x44, 0x38,
	0x32, 0xCE, 0xAC, 0xDB, 0x91, 0x44, 0x4E, 0x1E, 
	0x29, 0x78, 0x5A, 0xF0, 0xAB, 0x00, 0x7F, 0x91,
	0xE6, 0xB6, 0xD2, 0xE7, 0xA0, 0x05, 0xC2, 0xF2, 
	0x8D, 0x46, 0xA9, 0xBB, 0x52, 0x1B, 0x41, 0xDF,
	0xF0, 0x4A, 0xC9, 0x14, 0x27, 0xA9, 0x6B, 0x4A, 
	0x91, 0x4B, 0x8A, 0x80, 0xF5, 0xCF, 0xBB, 0x3C,
	0xBC, 0xF4, 0xC9, 0xD5, 0x42, 0x7A, 0xFA, 0xB7, 
	0xD5, 0x8C, 0x01, 0xC0, 0xFD, 0x1E, 0xAA, 0x57,
	0xC1, 0x20, 0x7A, 0x38, 0x2C, 0xB7, 0xCD, 0x14, 
	0x55, 0x9F, 0xD1, 0x5B, 0xFB, 0x70, 0xC0, 0x77,
	0xA4, 0x15, 0xB3, 0x9F, 0x6B, 0xBB, 0x10, 0x5A, 
	0x80, 0x9D, 0x16, 0x54, 0x6B, 0x7C, 0x5F, 0xAD,
	0x35, 0xCB, 0x92, 0x24, 0x08, 0x11, 0xD9, 0x61, 
	0x24, 0xA7, 0x75, 0xBF, 0x4D, 0x7E, 0x70, 0x0C,
	0x90, 0xCF, 0x9C, 0x04, 0xAC, 0x53, 0x89, 0xEF, 
	0x99, 0x22, 0xF6, 0x89, 0x10, 0xE6, 0x72, 0x23,
	0x0A, 0x5C, 0xA5, 0xFF, 0x9C, 0x78, 0xDA, 0x7F, 
	0xDF, 0xFF, 0xBB, 0x11, 0x6B, 0x75, 0xF0, 0x29,
	0xA5, 0x86, 0xD0, 0x53, 0x77, 0xE7, 0xB1, 0x0D, 
	0x4C, 0x06, 0xDA, 0x55, 0x4E, 0x50, 0x1B, 0x7A,
	0x1C, 0x90, 0xCE, 0x64, 0xD6, 0x17, 0x52, 0xFB, 
	0x00, 0x26, 0x75, 0x25, 0xCD, 0x95, 0x15, 0x0F,
	0x13, 0xD8, 0xAB, 0x30, 0xF1, 0xC5, 0xC5, 0xFA, 
	0x0C, 0x8E, 0x86, 0x1E, 0x3F, 0xCB, 0x8B, 0xD1,
	0xEC, 0xCE, 0xA9, 0x96, 0x91, 0x11, 0xB4, 0x97, 
	0x1E, 0x65, 0x5F, 0xA4, 0x55, 0xEB, 0xEC, 0xCF,
	0x19, 0xD9, 0x9F, 0xE0, 0x5E, 0x57, 0x45, 0x73, 
	0x0E, 0x2D, 0x18, 0xE1, 0x55, 0x05, 0x04, 0xBF,
	0x5E, 0x81, 0x1F, 0xDD, 0xFF, 0x5C, 0xC3, 0xF4, 
	0xF2, 0x06, 0x56, 0x54, 0x4D, 0xFB, 0x96, 0x54,
	0x33, 0x97, 0x07, 0x43, 0x4F, 0x39, 0xC4, 0xA8, 
	0x5E, 0x02, 0x37, 0x17, 0x7B, 0x64, 0xE6, 0xA2,
	0x2E, 0x24, 0x13, 0x07, 0xFE, 0xA1, 0x88, 0xB7, 
	0x60, 0xDD, 0x4C, 0xE0, 0xA1, 0xDC, 0xBA, 0x6C,
	0x81, 0x5C, 0x3F, 0x93, 0x7A, 0x1F, 0x2A, 0x1C, 
	0xAE, 0x5C, 0xBE, 0x9D, 0x84, 0x6F, 0xCB, 0x51,
	0x4D, 0x13, 0xC6, 0x81, 0x28, 0xC3, 0x03, 0x34, 
	0xB0, 0x5D, 0xCB, 0x8D, 0x69, 0x1C, 0xDE, 0x29,
	0x31, 0xF1, 0x22, 0xC3, 0x1C, 0x82, 0x8A, 0x57, 
	0x08, 0x32, 0x8B, 0xA2, 0x1E, 0x12, 0xC9, 0xB9,
	0xCD, 0xA8, 0xE6, 0x1C, 0x59, 0xAC, 0x0C, 0xF6, 
	0xA5, 0x3B, 0xE4, 0x64, 0x2F, 0x45, 0x33, 0xA2,
	0x4A, 0xDA, 0x39, 0xE2, 0x0E, 0x94, 0xF2, 0xAA, 
	0xB0, 0x82, 0xB7, 0x33, 0xD2, 0x6F, 0xC0, 0x00,
	0xD7, 0x8D, 0x1F, 0x8E, 0x79, 0x85, 0x3E, 0x2A, 

	0xD2, 0xB7, 0xF6, 0x9C, 0xCF, 0x06, 0xE8, 0xC1,
	0xAE, 0xEB, 0x7F, 0xE9, 0x87, 0x28, 0x1C, 0x9B, 
	0xE8, 0x8C, 0x2A, 0x97, 0xD1, 0xD2, 0xA6, 0x76,
	0xAD, 0x23, 0x69, 0xA0, 0xEF, 0x1F, 0x8C, 0xBA, 
	0x24, 0x62, 0x40, 0x0B, 0x21, 0xC6, 0x07, 0x89,
	0xBA, 0x60, 0x9E, 0x26, 0x98, 0x18, 0xAF, 0x01, 
	0xDF, 0x2B, 0x56, 0xC9, 0xB3, 0x72, 0x35, 0x8D,
	0x1D, 0x4F, 0x61, 0xAF, 0x53, 0x12, 0x6E, 0x49, 
	0x1C, 0x87, 0x6C, 0xB1, 0xD4, 0x1B, 0xA2, 0xB2,
	0xD4, 0xA1, 0x2C, 0xE2, 0x2F, 0xE9, 0xA4, 0x62, 
	0x17, 0x83, 0x1C, 0x68, 0xB3, 0xD6, 0x65, 0x2D,
	0x81, 0x5B, 0x4D, 0x9B, 0x15, 0x6F, 0x0B, 0xDF, 
	0xCE, 0x91, 0xB9, 0x8A, 0x61, 0x20, 0xB1, 0xF9,
	0xCA, 0x0A, 0xC4, 0x76, 0x5B, 0x4B, 0xAB, 0x16, 
	0x5B, 0xD2, 0x4A, 0xFD, 0x44, 0xB7, 0xDF, 0x1F,
	0x8B, 0x6F, 0xAB, 0x0C, 0xAB, 0x3D, 0x0C, 0x7A, 
	0x35, 0x6C, 0xBD, 0xFF, 0x62, 0x53, 0x77, 0x44,
	0xF2, 0x44, 0x5F, 0x8C, 0x59, 0x25, 0x5F, 0x6B, 
	0xB5, 0x27, 0x0D, 0xD2, 0x23, 0xBE, 0x40, 0xB3,
	0x3E, 0x8B, 0x92, 0xB1, 0x78, 0x57, 0xCB, 0xB0, 
	0xB3, 0xB4, 0xB6, 0xD5, 0xB6, 0xA7, 0x66, 0x6E,
	0xFB, 0xA7, 0x32, 0x93, 0xEE, 0x79, 0x61, 0x45, 
	0x49, 0xD7, 0x93, 0x34, 0x90, 0x1A, 0xAD, 0x2C,
	0x84, 0x3E, 0xE9, 0x0B, 0x2C, 0xC6, 0xB3, 0xB1, 
	0x82, 0xFB, 0x86, 0xEC, 0xA8, 0x76, 0x55, 0x98,
	0x7E, 0xE3, 0xA2, 0x47, 0xB6, 0x72, 0x05, 0x61, 
	0x0B, 0xA5, 0x72, 0x17, 0xCB, 0x18, 0xAE, 0x03,
	0x8C, 0x61, 0x32, 0xD9, 0x2B, 0x42, 0xEF, 0xF2, 
	0x3F, 0x0A, 0x06, 0x82, 0x09, 0xC9, 0x76, 0xF2,
	0x3D, 0x54, 0x50, 0xFD, 0x25, 0xA2, 0x2F, 0x2E, 
	0xF1, 0x34, 0x64, 0x94, 0xDC, 0x90, 0x58, 0x5D,
	0x1E, 0x6F, 0xB4, 0xEF, 0x73, 0xE8, 0xB0, 0xED, 
	0xC0, 0xD2, 0xE1, 0x42, 0xEC, 0x04, 0x69, 0xA8,
	0x27, 0x9C, 0x7C, 0x79, 0x87, 0x9A, 0xB2, 0x48, 
	0x50, 0x73, 0xEC, 0x1E, 0x4D, 0xD0, 0x80, 0x51,
	0x46, 0x21, 0xC9, 0xF8, 0x93, 0xCC, 0xE8, 0x41, 
	0x70, 0xC9, 0xE4, 0x78, 0x8F, 0x6B, 0x2C, 0x27,
	0x4C, 0x7E, 0x2C, 0x5A, 0x15, 0x69, 0x64, 0xDD, 
	0x00, 0xC7, 0x09, 0xCD, 0xF6, 0x2D, 0x2D, 0x31,
	0x6F, 0x01, 0x01, 0x3E, 0xCD, 0x60, 0x16, 0xB4, 
	0xE7, 0xE8, 0x76, 0xC4, 0x50, 0x4F, 0x08, 0x5B,
	0x62, 0x28, 0x24, 0x42, 0x7D, 0x9A, 0x19, 0x26, 
	0x2F, 0xD4, 0x67, 0xB9, 0x24, 0x0C, 0xBB, 0x14,
	0x7D, 0x19, 0xC8, 0x73, 0x79, 0xA7, 0x70, 0xCF, 
	0x2D, 0x53, 0xDC, 0x91, 0x83, 0xF2, 0x0C, 0x12,
	0x3B, 0xAF, 0x1B, 0x6B, 0x02, 0x99, 0x8B, 0x61, 
	0xE3, 0x2C, 0xA2, 0x54, 0xCD, 0x51, 0xAF, 0xE5,
	0x18, 0x58, 0x11, 0x7F, 0xF0, 0x50, 0x9C, 0x15, 
	0x6E, 0x26, 0x01, 0xE9, 0xDB, 0x50, 0x13, 0xEA,
	0x22, 0x59, 0x30, 0x3B, 0xE4, 0x5F, 0x43, 0x1E
};

// Compression Table

static unsigned int bit_table[257][2] =
{
	0x02, 0x00, 	0x05, 0x1F, 	0x06, 0x22, 	0x07, 0x34, 	0x07, 0x75, 	0x06, 0x28, 	0x06, 0x3B, 	0x07, 0x32, 
	0x08, 0xE0, 	0x08, 0x62, 	0x07, 0x56, 	0x08, 0x79, 	0x09, 0x19D,	0x08, 0x97, 	0x06, 0x2A, 	0x07, 0x57, 
	0x08, 0x71, 	0x08, 0x5B, 	0x09, 0x1CC,	0x08, 0xA7, 	0x07, 0x25, 	0x07, 0x4F, 	0x08, 0x66, 	0x08, 0x7D, 
	0x09, 0x191,	0x09, 0x1CE, 	0x07, 0x3F, 	0x09, 0x90, 	0x08, 0x59, 	0x08, 0x7B, 	0x08, 0x91, 	0x08, 0xC6, 
	0x06, 0x2D, 	0x09, 0x186, 	0x08, 0x6F, 	0x09, 0x93, 	0x0A, 0x1CC,	0x08, 0x5A, 	0x0A, 0x1AE,	0x0A, 0x1C0, 
	0x09, 0x148,	0x09, 0x14A, 	0x09, 0x82, 	0x0A, 0x19F, 	0x09, 0x171,	0x09, 0x120, 	0x09, 0xE7, 	0x0A, 0x1F3, 
	0x09, 0x14B,	0x09, 0x100,	0x09, 0x190,	0x06, 0x13, 	0x09, 0x161,	0x09, 0x125,	0x09, 0x133,	0x09, 0x195, 
	0x09, 0x173,	0x09, 0x1CA,	0x09, 0x86, 	0x09, 0x1E9, 	0x09, 0xDB, 	0x09, 0x1EC,	0x09, 0x8B, 	0x09, 0x85, 
	0x05, 0x0A, 	0x08, 0x96, 	0x08, 0x9C, 	0x09, 0x1C3, 	0x09, 0x19C,	0x09, 0x8F, 	0x09, 0x18F,	0x09, 0x91, 
	0x09, 0x87, 	0x09, 0xC6, 	0x09, 0x177,	0x09, 0x89, 	0x09, 0xD6, 	0x09, 0x8C, 	0x09, 0x1EE,	0x09, 0x1EB, 
	0x09, 0x84, 	0x09, 0x164, 	0x09, 0x175,	0x09, 0x1CD, 	0x08, 0x5E, 	0x09, 0x88, 	0x09, 0x12B,	0x09, 0x172, 
	0x09, 0x10A,	0x09, 0x8D, 	0x09, 0x13A,	0x09, 0x11C, 	0x0A, 0x1E1,	0x0A, 0x1E0, 	0x09, 0x187,	0x0A, 0x1DC, 
	0x0A, 0x1DF,	0x07, 0x74, 	0x09, 0x19F,	0x08, 0x8D,		0x08, 0xE4, 	0x07, 0x79, 	0x09, 0xEA, 	0x09, 0xE1, 
	0x08, 0x40, 	0x07, 0x41, 	0x09, 0x10B,	0x09, 0xB0, 	0x08, 0x6A, 	0x08, 0xC1, 	0x07, 0x71, 	0x07, 0x78, 
	0x08, 0xB1, 	0x09, 0x14C, 	0x07, 0x43, 	0x08, 0x76, 	0x07, 0x66, 	0x07, 0x4D, 	0x09, 0x8A, 	0x06, 0x2F, 
	0x08, 0xC9,		0x09, 0xCE, 	0x09, 0x149,	0x09, 0x160, 	0x0A, 0x1BA, 	0x0A, 0x19E, 	0x0A, 0x39F, 	0x09, 0xE5, 
	0x09, 0x194, 	0x09, 0x184, 	0x09, 0x126, 	0x07, 0x30, 	0x08, 0x6C, 	0x09, 0x121, 	0x09, 0x1E8, 	0x0A, 0x1C1, 
	0x0A, 0x11D, 	0x0A, 0x163, 	0x0A, 0x385, 	0x0A, 0x3DB, 	0x0A, 0x17D, 	0x0A, 0x106, 	0x0A, 0x397, 	0x0A, 0x24E, 
	0x07, 0x2E, 	0x08, 0x98, 	0x0A, 0x33C, 	0x0A, 0x32E, 	0x0A, 0x1E9, 	0x09, 0xBF, 	0x0A, 0x3DF, 	0x0A, 0x1DD, 
	0x0A, 0x32D, 	0x0A, 0x2ED, 	0x0A, 0x30B, 	0x0A, 0x107, 	0x0A, 0x2E8, 	0x0A, 0x3DE, 	0x0A, 0x125, 	0x0A, 0x1E8, 
	0x09, 0xE9, 	0x0A, 0x1CD, 	0x0A, 0x1B5, 	0x09, 0x165, 	0x0A, 0x232, 	0x0A, 0x2E1, 	0x0B, 0x3AE, 	0x0B, 0x3C6, 
	0x0B, 0x3E2, 	0x0A, 0x205, 	0x0A, 0x29A, 	0x0A, 0x248, 	0x0A, 0x2CD, 	0x0A, 0x23B, 	0x0B, 0x3C5, 	0x0A, 0x251, 
	0x0A, 0x2E9, 	0x0A, 0x252, 	0x09, 0x1EA, 	0x0B, 0x3A0, 	0x0B, 0x391, 	0x0A, 0x23C, 	0x0B, 0x392, 	0x0B, 0x3D5, 
	0x0A, 0x233, 	0x0A, 0x2CC, 	0x0B, 0x390, 	0x0A, 0x1BB, 	0x0B, 0x3A1, 	0x0B, 0x3C4, 	0x0A, 0x211, 	0x0A, 0x203, 
	0x09, 0x12A, 	0x0A, 0x231, 	0x0B, 0x3E0, 	0x0A, 0x29B, 	0x0B, 0x3D7, 	0x0A, 0x202, 	0x0B, 0x3AD, 	0x0A, 0x213, 
	0x0A, 0x253, 	0x0A, 0x32C, 	0x0A, 0x23D, 	0x0A, 0x23F, 	0x0A, 0x32F, 	0x0A, 0x11C, 	0x0A, 0x384, 	0x0A, 0x31C, 
	0x0A, 0x17C, 	0x0A, 0x30A, 	0x0A, 0x2E0, 	0x0A, 0x276, 	0x0A, 0x250, 	0x0B, 0x3E3, 	0x0A, 0x396, 	0x0A, 0x18F, 
	0x0A, 0x204, 	0x0A, 0x206, 	0x0A, 0x230, 	0x0A, 0x265, 	0x0A, 0x212, 	0x0A, 0x23E, 	0x0B, 0x3AC, 	0x0B, 0x393, 
	0x0B, 0x3E1, 	0x0A, 0x1DE, 	0x0B, 0x3D6, 	0x0A, 0x31D, 	0x0B, 0x3E5, 	0x0B, 0x3E4, 	0x0A, 0x207, 	0x0B, 0x3C7, 
	0x0A, 0x277, 	0x0B, 0x3D4, 	0x08, 0xC0,		0x0A, 0x162, 	0x0A, 0x3DA, 	0x0A, 0x124, 	0x0A, 0x1B4, 	0x0A, 0x264, 
	0x0A, 0x33D, 	0x0A, 0x1D1, 	0x0A, 0x1AF, 	0x0A, 0x39E, 	0x0A, 0x24F, 	0x0B, 0x373, 	0x0A, 0x249, 	0x0B, 0x372, 
	0x09, 0x167, 	0x0A, 0x210, 	0x0A, 0x23A, 	0x0A, 0x1B8, 	0x0B, 0x3AF, 	0x0A, 0x18E, 	0x0A, 0x2EC, 	0x07, 0x62, 
	0x04, 0x0D
};

// Macro Definitions ( to avoid big-/little-endian problems )

#define N2L(C, LL) LL  = ((unsigned int)(*((C)++))) << 24, LL |= ((unsigned int)(*((C)++))) << 16, LL |= ((unsigned int)(*((C)++))) << 8, LL |= ((unsigned int)(*((C)++)))
#define L2N(LL, C) *((C)++) = (unsigned char)(((LL) >> 24) & 0xff), *((C)++) = (unsigned char)(((LL) >> 16) & 0xff), *((C)++) = (unsigned char)(((LL) >> 8) & 0xff), *((C)++) = (unsigned char)(((LL)) & 0xff)
#define ROUND(LL, R, S, P) LL ^= P; LL ^= ((S[(R >> 24)]  + S[0x0100 + ((R >> 16) & 0xff)]) ^ S[0x0200 + ((R >> 8) & 0xff)]) + S[0x0300 + ((R) & 0xff)]

// Constructor / Destructor

CCryptNoCrypt::CCryptNoCrypt()
{
}

CCryptNoCrypt::~CCryptNoCrypt()
{
}

// Member Functions

void CCryptNoCrypt::Decrypt(void *pvIn, void *pvOut, int len)
{
	memcpy(pvOut, pvIn, len);
}

void CCryptNoCrypt::Init(void *pvSeed, int type)
{
	unsigned char *pSeed = (unsigned char *)pvSeed;
	N2L(pSeed, m_loginSeed);

	m_loginKey[0] = (((~m_loginSeed) ^ 0x00001357) << 16) | ((m_loginSeed ^ 0xffffaaaa) & 0x0000ffff);
	m_loginKey[1] = ((m_loginSeed ^ 0x43210000) >> 16) | (((~m_loginSeed) ^ 0xabcdffff) & 0xffff0000);

	//if(!m_bTablesReady) InitTables();

	m_gameTable = CRYPT_GAMETABLE_START;
	memcpy(m_gameSeed, seed_table[0][m_gameTable][0], CRYPT_GAMESEED_LENGTH);
	m_gameStreamPos = 0;
	m_gameBlockPos = 0;

	m_type = type;
}
