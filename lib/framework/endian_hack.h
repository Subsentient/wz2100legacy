/*This code copyrighted (2013) for the Warzone 2100 Legacy Project under the GPLv2.

Warzone 2100 Legacy is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

Warzone 2100 Legacy is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Warzone 2100 Legacy; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA*/
#ifndef ENDIAN_HACK_H
#define ENDIAN_HACK_H

/* Endianness hacks */
// TODO Use SDL_SwapXXXX instead

static inline void endian_uword(uint16_t *uword)
{
#ifdef __BIG_ENDIAN__
	uint8_t tmp, *ptr;

	ptr = (uint8_t *) uword;
	tmp = ptr[0];
	ptr[0] = ptr[1];
	ptr[1] = tmp;
#else
	// Prevent warnings
	(void)uword;
#endif
}

static inline void endian_sword(int16_t *sword)
{
#ifdef __BIG_ENDIAN__
	uint8_t tmp, *ptr;

	ptr = (uint8_t *) sword;
	tmp = ptr[0];
	ptr[0] = ptr[1];
	ptr[1] = tmp;
#else
	// Prevent warnings
	(void)sword;
#endif
}

static inline void endian_udword(uint32_t *udword)
{
#ifdef __BIG_ENDIAN__
	uint8_t tmp, *ptr;

	ptr = (uint8_t *) udword;
	tmp = ptr[0];
	ptr[0] = ptr[3];
	ptr[3] = tmp;
	tmp = ptr[1];
	ptr[1] = ptr[2];
	ptr[2] = tmp;
#else
	// Prevent warnings
	(void)udword;
#endif
}

static inline void endian_sdword(int32_t *sdword)
{
#ifdef __BIG_ENDIAN__
	uint8_t tmp, *ptr;

	ptr = (uint8_t *) sdword;
	tmp = ptr[0];
	ptr[0] = ptr[3];
	ptr[3] = tmp;
	tmp = ptr[1];
	ptr[1] = ptr[2];
	ptr[2] = tmp;
#else
	// Prevent warnings
	(void)sdword;
#endif
}

#endif // ENDIAN_HACK_H
