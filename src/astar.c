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
/** @file
 *  A* based path finding
 */

#include <assert.h>

#include "lib/framework/frame.h"

#include "objects.h"
#include "map.h"
#include "raycast.h"
#include "fpath.h"

#include "astar.h"

static int32_t	astarOuter, astarRemove;

/** Keeps track of the amount of iterations done in the inner loop of our A*
 *  implementation.
 *
 *  @ingroup pathfinding
 */
int astarInner = 0;

/** Counter to implement lazy deletion from nodeArray.
 *
 *  @see fpathTableReset
 */
static int resetIterationCount = 0;

/** The structure to store a node of the route in node table
 *
 *  @ingroup pathfinding
 */
typedef struct _fp_node
{
	int     x, y;           // map coords
	int16_t	dist, est;	// distance so far and estimate to end
	int16_t	type;		// open or closed node
	struct _fp_node *psOpen;
	struct _fp_node	*psRoute;	// Previous point in the route
	struct _fp_node *psNext;
	int iteration;
} FP_NODE;

// types of node
#define NT_OPEN		1
#define NT_CLOSED	2

/** List of open nodes
 */
static FP_NODE *psOpen;

/** A map's maximum width & height
 */
#define MAX_MAP_SIZE (UINT8_MAX + 1)

/** Table of closed nodes
 */
static FP_NODE *nodeArray[MAX_MAP_SIZE][MAX_MAP_SIZE] = { { NULL }, { NULL } };

#define NUM_DIR		8

// Convert a direction into an offset
// dir 0 => x = 0, y = -1
static const Vector2i aDirOffset[NUM_DIR] =
{
	{ 0, 1},
	{ -1, 1},
	{ -1, 0},
	{ -1, -1},
	{ 0, -1},
	{ 1, -1},
	{ 1, 0},
	{ 1, 1},
};

// reset the astar counters
void astarResetCounters(void)
{
	astarInner = 0;
	astarOuter = 0;
	astarRemove = 0;
}

/** Add a node to the node table
 *
 *  @param psNode to add to the table
 */
static void fpathAddNode(FP_NODE *psNode)
{
	const int x = psNode->x;
	const int y = psNode->y;

	ASSERT(x < ARRAY_SIZE(nodeArray) && y < ARRAY_SIZE(nodeArray[x]), "X (%d) or Y %d) coordinate for path finding node is out of range!", x, y);

	// Lets not leak memory
	if (nodeArray[x][y])
	{
		free(nodeArray[x][y]);
	}

	nodeArray[x][y] = psNode;

	// Assign this node to the current iteration (this node will only remain
	// valid for as long as it's `iteration` member has the same value as
	// resetIterationCount.
	psNode->iteration = resetIterationCount;
}

/** See if a node is in the table
 *  Check whether there is a node for the given coordinates in the table
 *
 *  @param x,y the coordinates to check for
 *  @return a pointer to the node if one could be found, or NULL otherwise.
 */
static FP_NODE *fpathGetNode(int x, int y)
{
	FP_NODE *psFound;

	if (x < 0 || y < 0 || x >= ARRAY_SIZE(nodeArray) || y >= ARRAY_SIZE(nodeArray[x]))
	{
		return NULL;
	}

	psFound = nodeArray[x][y];
	if (psFound
			&& psFound->iteration == resetIterationCount)
	{
		return psFound;
	}

	return NULL;
}

/** Reset the node table
 *
 *  @NOTE The actual implementation does a lazy reset, because resetting the
 *        entire node table is expensive.
 */
static void fpathTableReset(void)
{
	// Reset node table, simulate this by incrementing the iteration
	// counter, which will invalidate all nodes currently in the table. See
	// the implementation of fpathGetNode().
	++resetIterationCount;

	// Check to prevent overflows of resetIterationCount
	if (resetIterationCount < INT_MAX - 1)
	{
		ASSERT(resetIterationCount > 0, "Integer overflow occurred!");

		return;
	}

	// If we're about to overflow resetIterationCount, reset the entire
	// table for real (not lazy for once in a while) and start counting
	// at zero (0) again.
	fpathHardTableReset();
}

void fpathHardTableReset()
{
	int x, y;

	for (x = 0; x < ARRAY_SIZE(nodeArray); ++x)
	{
		for (y = 0; y < ARRAY_SIZE(nodeArray[x]); ++y)
		{
			if (nodeArray[x][y])
			{
				free(nodeArray[x][y]);
				nodeArray[x][y] = NULL;
			}
		}
	}

	resetIterationCount = 0;
}

/** Compare two nodes
 */
static inline int32_t fpathCompare(FP_NODE *psFirst, FP_NODE *psSecond)
{
	int32_t	first, second;

	first = psFirst->dist + psFirst->est;
	second = psSecond->dist + psSecond->est;
	if (first < second)
	{
		return -1;
	}
	else if (first > second)
	{
		return 1;
	}

	// equal totals, give preference to node closer to target
	if (psFirst->est < psSecond->est)
	{
		return -1;
	}
	else if (psFirst->est > psSecond->est)
	{
		return 1;
	}

	// exactly equal
	return 0;
}

/** make a 50/50 random choice
 */
static BOOL fpathRandChoice(void)
{
	return ONEINTWO;
}

/** Add a node to the open list
 */
static void fpathOpenAdd(FP_NODE *psNode)
{
	psNode->psOpen = psOpen;
	psOpen = psNode;
}

/** Get the nearest entry in the open list
 */
static FP_NODE *fpathOpenGet(void)
{
	FP_NODE	*psNode, *psCurr, *psPrev, *psParent = NULL;
	int32_t	comp;

	if (psOpen == NULL)
	{
		return NULL;
	}

	// find the node with the lowest distance
	psPrev = NULL;
	psNode = psOpen;
	for(psCurr = psOpen; psCurr; psCurr = psCurr->psOpen)
	{
		comp = fpathCompare(psCurr, psNode);
		if (comp < 0 || (comp == 0 && fpathRandChoice()))
		{
			psParent = psPrev;
			psNode = psCurr;
		}
		psPrev = psCurr;
	}

	// remove the node from the list
	if (psNode == psOpen)
	{
		// node is at head of list
		psOpen = psOpen->psOpen;
	}
	else
	{
		psParent->psOpen = psNode->psOpen;
	}

	return psNode;
}

/** Estimate the distance to the target point
 */
static int32_t fpathEstimate(int32_t x, int32_t y, int32_t fx, int32_t fy)
{
	int32_t xdiff, ydiff;

	xdiff = x > fx ? x - fx : fx - x;
	ydiff = y > fy ? y - fy : fy - y;

	xdiff = xdiff * 10;
	ydiff = ydiff * 10;

	return xdiff > ydiff ? xdiff + ydiff / 2 : xdiff / 2 + ydiff;
}

/** Generate a new node
 */
static FP_NODE *fpathNewNode(int32_t x, int32_t y, int32_t dist, FP_NODE *psRoute)
{
	FP_NODE	*psNode = malloc(sizeof(FP_NODE));

	if (psNode == NULL)
	{
		debug(LOG_ERROR, "fpathNewNode: Out of memory");
		return NULL;
	}

	psNode->x = (int16_t)x;
	psNode->y = (int16_t)y;
	psNode->dist = (int16_t)dist;
	psNode->psRoute = psRoute;
	psNode->type = NT_OPEN;

	return psNode;
}

// Variables for the callback
static int32_t	finalX, finalY, vectorX, vectorY;
static BOOL		obstruction;

/** The visibility ray callback
 */
static bool fpathVisCallback(Vector3i pos, int dist, void *data)
{
	/* Has to be -1 to make sure that it doesn't match any enumerated
	 * constant from PROPULSION_TYPE.
	 */
	static const PROPULSION_TYPE prop = (PROPULSION_TYPE) - 1;

	// See if this point is past the final point (dot product)
	int vx = pos.x - finalX, vy = pos.y - finalY;

	if (vx * vectorX + vy * vectorY <= 0)
	{
		return false;
	}

	if (fpathBlockingTile(map_coord(pos.x), map_coord(pos.y), prop))
	{
		// found an obstruction
		obstruction = true;
		return false;
	}

	return true;
}

BOOL fpathTileLOS(int32_t x1, int32_t y1, int32_t x2, int32_t y2)
{
	// convert to world coords
	Vector3i p1 = { world_coord(x1) + TILE_UNITS / 2, world_coord(y1) + TILE_UNITS / 2, 0 };
	Vector3i p2 = { world_coord(x2) + TILE_UNITS / 2, world_coord(y2) + TILE_UNITS / 2, 0 };
	Vector3i dir = Vector3i_Sub(p2, p1);

	// Initialise the callback variables
	finalX = p2.x;
	finalY = p2.y;
	vectorX = -dir.x;
	vectorY = -dir.y;
	obstruction = false;

	rayCast(p1, dir, RAY_MAXLEN, fpathVisCallback, NULL);

	return !obstruction;
}

ASR_RETVAL fpathAStarRoute(MOVE_CONTROL *psMove, PATHJOB *psJob)
{
	FP_NODE		*psFound, *psCurr, *psNew, *psParent, *psNext;
	FP_NODE		*psNearest, *psRoute;
	int32_t		dir, x, y, currDist;
	ASR_RETVAL		retval = ASR_OK;
	const int       tileSX = map_coord(psJob->origX);
	const int       tileSY = map_coord(psJob->origY);
	const int       tileFX = map_coord(psJob->destX);
	const int       tileFY = map_coord(psJob->destY);

	fpathTableReset();

	// Add the start point to the open list
	psCurr = fpathNewNode(tileSX, tileSY, 0, NULL);
	if (!psCurr)
	{
		fpathTableReset();
		return ASR_FAILED;
	}
	// estimate the estimated distance/moves
	psCurr->est = (int16_t)fpathEstimate(psCurr->x, psCurr->y, tileFX, tileFY);
	psOpen = NULL;
	fpathOpenAdd(psCurr);
	fpathAddNode(psCurr);
	psRoute = NULL;
	psNearest = NULL;

	// search for a route
	while (psOpen != NULL)
	{
		psCurr = fpathOpenGet();

		if (psCurr->x == tileFX && psCurr->y == tileFY)
		{
			// reached the target
			psRoute = psCurr;
			break;
		}

		// note the nearest node to the target so far
		if (psNearest == NULL || psCurr->est < psNearest->est)
		{
			psNearest = psCurr;
		}

		astarOuter += 1;

		// loop through possible moves in 8 directions to find a valid move
		for(dir = 0; dir < NUM_DIR; dir += 1)
		{
			/* make non-orthogonal-adjacent moves' dist a bit longer/cost a bit more
			   5  6  7
			     \|/
			   4 -I- 0
			     /|\
			   3  2  1
			   odd:orthogonal-adjacent tiles even:non-orthogonal-adjacent tiles
			   odd ones get extra 4 units(1.414 times) of distance/costs
			*/
			if (dir % 2 == 0)
			{
				currDist = psCurr->dist + 10;
			}
			else
			{
				currDist = psCurr->dist + 14;
			}

			// Try a new location
			x = psCurr->x + aDirOffset[dir].x;
			y = psCurr->y + aDirOffset[dir].y;


			// See if the node has already been visited
			psFound = fpathGetNode(x, y);
			if (psFound && psFound->dist <= currDist)
			{
				// already visited node by a shorter route
				continue;
			}

			// If the tile hasn't been visited see if it is a blocking tile
			if (!psFound && fpathBaseBlockingTile(x, y, psJob->propulsion, psJob->owner, psJob->moveType))
			{
				// tile is blocked, skip it
				continue;
			}

			astarInner += 1;
			ASSERT(astarInner >= 0, "astarInner overflowed!");

			// Now insert the point into the appropriate list
			if (!psFound)
			{
				// Not in open or closed lists - add to the open list
				psNew = fpathNewNode(x, y, currDist, psCurr);
				if (psNew)
				{
					psNew->est = (int16_t)fpathEstimate(x, y, tileFX, tileFY);
					fpathOpenAdd(psNew);
					fpathAddNode(psNew);
				}
			}
			else if (psFound->type == NT_OPEN)
			{
				astarRemove += 1;

				// already in the open list but this is shorter
				psFound->dist = (int16_t)currDist;
				psFound->psRoute = psCurr;
			}
			else if (psFound->type == NT_CLOSED)
			{
				// already in the closed list but this is shorter
				psFound->type = NT_OPEN;
				psFound->dist = (int16_t)currDist;
				psFound->psRoute = psCurr;
				fpathOpenAdd(psFound);
			}
			else
			{
				ASSERT(!"the open and closed lists are fried/wrong", "fpathAStarRoute: the open and closed lists are f***ed");
			}
		}

		// add the current point to the closed nodes
//		fpathAddNode(psCurr);
		psCurr->type = NT_CLOSED;
	}

	// return the nearest route if no actual route was found
	if (!psRoute && psNearest)
	{
		psRoute = psNearest;
		retval = ASR_NEAREST;
	}

	if (psRoute)
	{
		int index, count = psMove->numPoints;

		// get the route in the correct order
		// If as I suspect this is to reverse the list, then it's my suspicion that
		// we could route from destination to source as opposed to source to
		// destination. We could then save the reversal. to risky to try now...Alex M
		//
		// The idea is impractical, because you can't guarentee that the target is
		// reachable. As I see it, this is the reason why psNearest got introduced.
		// -- Dennis L.
		psParent = NULL;
		for(psCurr = psRoute; psCurr; psCurr = psNext)
		{
			psNext = psCurr->psRoute;
			psCurr->psRoute = psParent;
			psParent = psCurr;
			count++;
		}
		ASSERT(count > 0, "Route has no nodes");
		if (count <= 0)
		{
			fpathHardTableReset();
			return ASR_FAILED;
		}
		psRoute = psParent;

		psCurr = psRoute;
		psMove->asPath = realloc(psMove->asPath, sizeof(*psMove->asPath) * count);
		ASSERT(psMove->asPath, "Out of memory");
		if (!psMove->asPath)
		{
			fpathHardTableReset();
			return ASR_FAILED;
		}
		index = psMove->numPoints;
		while (psCurr && index < count)
		{
			psMove->asPath[index].x = psCurr->x;
			psMove->asPath[index].y = psCurr->y;
			index++;
			ASSERT(psCurr->x < mapWidth && psCurr->y < mapHeight, "Bad route generated!");
			psCurr = psCurr->psRoute;
		}
		psMove->numPoints = MIN(255, index);
		psMove->DestinationX = world_coord(psMove->asPath[index - 1].x);
		psMove->DestinationY = world_coord(psMove->asPath[index - 1].y);
	}
	else
	{
		retval = ASR_FAILED;
	}

	fpathTableReset();
	return retval;
}
