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
/*
 * Gateway.c
 *
 * Routing gateway code.
 *
 */

#include "lib/framework/frame.h"

#include "lib/framework/listmacs.h"
#include "map.h"
#include "wrappers.h"

#include "gateway.h"

// the list of gateways on the current map
GATEWAY		*psGateways;

/******************************************************************************************************/
/*                   Gateway data access functions                                                    */

// get the size of the map
static int32_t gwMapWidth(void)
{
	return (int32_t)mapWidth;
}

static int32_t gwMapHeight(void)
{
	return (int32_t)mapHeight;
}

// set the gateway flag on a tile
static void gwSetGatewayFlag(int32_t x, int32_t y)
{
	mapTile((uint32_t)x, (uint32_t)y)->tileInfoBits |= BITS_GATEWAY;
}

// clear the gateway flag on a tile
static void gwClearGatewayFlag(int32_t x, int32_t y)
{
	mapTile((uint32_t)x, (uint32_t)y)->tileInfoBits &= ~BITS_GATEWAY;
}


/******************************************************************************************************/
/*                   Gateway functions                                                                */

// Initialise the gateway system
BOOL gwInitialise(void)
{
	ASSERT( psGateways == NULL, "gwInitialise: gateway list has not been reset" );

	psGateways = NULL;

	return true;
}


// Shutdown the gateway system
void gwShutDown(void)
{
	GATEWAY *psNext;

	while (psGateways != NULL)
	{
		psNext = psGateways->psNext;
		gwFreeGateway(psGateways);
		psGateways = psNext;
	}
}


// Add a gateway to the system
BOOL gwNewGateway(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	GATEWAY		*psNew;
	int32_t		pos, temp;

	if ((x1 < 0) || (x1 >= gwMapWidth())  ||
			(y1 < 0) || (y1 >= gwMapHeight()) ||
			(x2 < 0) || (x2 >= gwMapWidth())  ||
			(y2 < 0) || (y2 >= gwMapHeight()) ||
			((x1 != x2) && (y1 != y2)))
	{
		ASSERT( false, "gwNewGateway: invalid coordinates" );
		return false;
	}

	psNew = (GATEWAY *)malloc(sizeof(GATEWAY));
	if (!psNew)
	{
		debug( LOG_FATAL, "gwNewGateway: out of memory" );
		abort();
		return false;
	}

	// make sure the first coordinate is always the smallest
	if (x2 < x1)
	{
		// y is the same, swap x
		temp = x2;
		x2 = x1;
		x1 = temp;
	}
	else if (y2 < y1)
	{
		// x is the same, swap y
		temp = y2;
		y2 = y1;
		y1 = temp;
	}

	// initialise the gateway
	psNew->x1 = (uint8_t)x1;
	psNew->y1 = (uint8_t)y1;
	psNew->x2 = (uint8_t)x2;
	psNew->y2 = (uint8_t)y2;
	psNew->flags = 0;

	// add the gateway to the list
	psNew->psNext = psGateways;
	psGateways = psNew;

	// set the map flags
	if (x1 == x2)
	{
		// vertical gateway
		for(pos = y1; pos <= y2; pos++)
		{
			gwSetGatewayFlag(x1, pos);
		}
	}
	else
	{
		// horizontal gateway
		for(pos = x1; pos <= x2; pos++)
		{
			gwSetGatewayFlag(pos, y1);
		}
	}

	return true;
}


// Return the number of gateways.
uint32_t gwNumGateways(void)
{
	GATEWAY		*psCurr;
	uint32_t NumGateways = 0;

	for(psCurr = psGateways; psCurr; psCurr = psCurr->psNext)
	{
		NumGateways++;
	}

	return NumGateways;
}


GATEWAY *gwGetGateways(void)
{
	return psGateways;
}


// Release a gateway
void gwFreeGateway(GATEWAY *psDel)
{
	int32_t	pos;

	LIST_REMOVE(psGateways, psDel, GATEWAY);

	if (psMapTiles) // this lines fixes the bug where we were closing the gateways after freeing the map
	{
		// clear the map flags
		if (psDel->x1 == psDel->x2)
		{
			// vertical gateway
			for(pos = psDel->y1; pos <= psDel->y2; pos++)
			{
				gwClearGatewayFlag(psDel->x1, pos);
			}
		}
		else
		{
			// horizontal gateway
			for(pos = psDel->x1; pos <= psDel->x2; pos++)
			{
				gwClearGatewayFlag(pos, psDel->y1);
			}
		}

	}

	free(psDel);
}


// load a gateway list
BOOL gwLoadGateways(char *pFileBuffer, uint32_t fileSize)
{
	int32_t	numGW, x1, y1, x2, y2;
	char	*pPos;

	// get the number of gateways
	pPos = pFileBuffer;
	sscanf((char *)pPos, "%d", &numGW);
	for (; *pPos != '\n' && pPos < (pFileBuffer + fileSize); pPos += 1)
		;
	pPos += 1;

	while ((pPos < (pFileBuffer + fileSize)) && (numGW > 0))
	{
		sscanf((char *)pPos, "%d %d %d %d", &x1, &y1, &x2, &y2);

		if (!gwNewGateway(x1, y1, x2, y2))
		{
			return false;
		}

		for (; *pPos != '\n' && pPos < (pFileBuffer + fileSize); pPos += 1)
			;
		pPos += 1;
		numGW -= 1;
	}

	return true;
}
