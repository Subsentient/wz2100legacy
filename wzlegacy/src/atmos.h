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

#ifndef __INCLUDED_SRC_ATMOS_H__
#define __INCLUDED_SRC_ATMOS_H__

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

typedef struct _atmosParticle
{
	uint8_t		status;
	uint8_t		type;
	uint32_t		size;
	Vector3f	position;
	Vector3f	velocity;
	iIMDShape	*imd;
} ATPART;

typedef	enum
{
	WT_RAINING,
	WT_SNOWING,
	WT_NONE
} WT_CLASS;

void atmosInitSystem(void);
void atmosUpdateSystem(void);
void renderParticle(ATPART *psPart);
void atmosDrawParticles(void);
void atmosSetWeatherType(WT_CLASS type);
WT_CLASS atmosGetWeatherType(void);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __INCLUDED_SRC_ATMOS_H__
