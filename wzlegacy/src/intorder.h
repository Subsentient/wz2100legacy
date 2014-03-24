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
 *  definitions for order interface functions.
 */

#ifndef __INCLUDED_SRC_INTORDER_H__
#define __INCLUDED_SRC_INTORDER_H__

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

#define IDORDER_FORM	8000
#define IDORDER_CLOSE	8001

    extern BOOL OrderUp;

    BOOL intUpdateOrder(DROID *psDroid);	// update already open order form
    BOOL intAddOrder(BASE_OBJECT *psObj);			// create and open order form
    void intRunOrder(void);
    void intProcessOrder(uint32_t id);
    void intRemoveOrder(void);
    void intRemoveOrderNoAnim(void);
    BOOL intRefreshOrder(void);

//new function added to bring up the RMB order form for Factories as well as droids
    void intAddFactoryOrder(STRUCTURE *psStructure);

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // __INCLUDED_SRC_INTORDER_H__
