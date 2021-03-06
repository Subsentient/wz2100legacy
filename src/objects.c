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
 * Objects.c
 *
 * The object system.
 *
 */


#include "lib/framework/frame.h"
#include "objects.h"


/* Initialise the object system */
BOOL objInitialise(void)
{
	if (!objmemInitialise())
	{
		return false;
	}

	return true;
}


/* Shutdown the object system */
BOOL objShutdown(void)
{
	objmemShutdown();

	return true;
}


/*goes thru' the list passed in reversing the order so the first entry becomes
the last and the last entry becomes the first!*/
void reverseObjectList(BASE_OBJECT **ppsList)
{
	BASE_OBJECT     *psPrev, *psNext, *psCurrent, *psObjList;

	//initialise the pointers
	psObjList = *ppsList;
	psPrev = psNext = NULL;
	psCurrent = psObjList;

	while(psCurrent != NULL)
	{
		psNext = psCurrent->psNext;
		psCurrent->psNext = psPrev;
		psPrev = psCurrent;
		psCurrent = psNext;
	}
	//set the list passed in to point to the new top
	*ppsList = psPrev;
}

const char *objInfo(const BASE_OBJECT *psObj)
{
	static char	info[PATH_MAX];

	switch (psObj->type)
	{
		case OBJ_DROID:
		{
			const DROID *psDroid = (const DROID *)psObj;

			ssprintf(info, "%s", droidGetName(psDroid));
			break;
		}
		case OBJ_STRUCTURE:
		{
			const STRUCTURE *psStruct = (const STRUCTURE *)psObj;

			ssprintf(info, "%s", getName(psStruct->pStructureType->pName));
			break;
		}
		case OBJ_FEATURE:
		{
			const FEATURE *psFeat = (const FEATURE *)psObj;

			ssprintf(info, "%s", getName(psFeat->psStats->pName));
			break;
		}
		case OBJ_PROJECTILE:
			sstrcpy(info, "Projectile");	// TODO
			break;
		case OBJ_TARGET:
			sstrcpy(info, "Target");	// TODO
			break;
		default:
			sstrcpy(info, "Unknown object type");
			break;
	}
	return info;
}
