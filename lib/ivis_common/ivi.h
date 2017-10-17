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
//*************************************************************************
//***	ivi.h iVi engine definitions. [Sam Kerbeck] ***//

#ifndef _ivi_
#define _ivi_

#include "piedef.h"

#define iV_DIVSHIFT 15
#define iV_DIVMULTP (1 << iV_DIVSHIFT)

extern void iV_Reset(void);
extern void iV_ShutDown(void);

#endif
