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
#include "imd.h"
#include "ivisdef.h"
#include "tex.h"
#include "ivispatch.h"

//*************************************************************************
//*** free IMD shape memory
//*
//* pre		shape successfully allocated
//*
//* params	shape = pointer to IMD shape
//*
//******
void iV_IMDRelease(iIMDShape *s)
{
    unsigned int i;
    iIMDShape *d;

    if (s)
    {
        if (s->points)
        {
            free(s->points);
        }
        if (s->connectors)
        {
            free(s->connectors);
        }
        if (s->polys)
        {
            for (i = 0; i < s->npolys; i++)
            {
                if (s->polys[i].pindex)
                {
                    free(s->polys[i].pindex);
                }
                if (s->polys[i].texCoord)
                {
                    free(s->polys[i].texCoord);
                }
            }
            free(s->polys);
        }
        if (s->shadowEdgeList)
        {
            free(s->shadowEdgeList);
            s->shadowEdgeList = NULL;
        }
        d = s->next;
        free(s);
        iV_IMDRelease(d);
    }
}
