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

#ifndef __INCLUDED_SRC_ADVVIS_H__
#define __INCLUDED_SRC_ADVVIS_H__

#include "lib/framework/types.h"
#include "basedef.h"

extern void	avInformOfChange(int32_t x, int32_t y);
extern void	avUpdateTiles( void );
extern uint32_t avGetObjLightLevel( BASE_OBJECT *psObj, uint32_t origLevel);
extern void	setRevealStatus( BOOL val );
extern BOOL	getRevealStatus( void );
extern void	preProcessVisibility( void );

#endif // __INCLUDED_SRC_ADVVIS_H__
