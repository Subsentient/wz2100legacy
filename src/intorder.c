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

#include "lib/framework/frame.h"
#include "lib/ivis_common/bitimage.h"//bitmap routines

#include "hci.h"
#include "intdisplay.h"
#include "intorder.h"
#include "objects.h"
#include "order.h"
#include "scriptextern.h"


#define ORDER_X			23
#define ORDER_Y			45
#define ORDER_WIDTH		RET_FORMWIDTH
#define ORDER_HEIGHT	273
#define ORDER_BUTX		8
#define ORDER_BUTY		16
#define ORDER_BUTGAP	4
#define ORDER_BOTTOMY	318	+ E_H


#define MAX_SELECTED_DROIDS	1000    // Max size of selected droids list.

#define MAX_AVAILABLE_ORDERS 16	// Max available orders list.
#define MAX_DISPLAYABLE_ORDERS 11	// Max number of displayable orders.
#define MAX_ORDER_BUTS 5		// Max number of buttons for a given order.
#define NUM_ORDERS 12			// Number of orders in OrderButtons list.

#define IDORDER_ATTACK_RANGE				8010
#define IDORDER_REPAIR_LEVEL				8020
#define IDORDER_ATTACK_LEVEL				8030
#define IDORDER_PATROL						8040
#define IDORDER_HALT_TYPE					8050
#define IDORDER_RETURN						8060
#define IDORDER_RECYCLE						8070
#define IDORDER_ASSIGN_PRODUCTION			8080
#define IDORDER_ASSIGN_CYBORG_PRODUCTION	8090
#define IDORDER_FIRE_DESIGNATOR				8100
#define IDORDER_ASSIGN_VTOL_PRODUCTION		8110
#define IDORDER_CIRCLE						8120

//#define IDORDER_RETURN_TO_BASE		8050
//#define IDORDER_DESTRUCT				8060
//#define IDORDER_RETURN_TO_REPAIR		8080
//#define IDORDER_EMBARK				8100

typedef enum
{
	ORD_BTYPE_RADIO,			// Only one state allowed.
	ORD_BTYPE_BOOLEAN,			// Clicking on a button toggles it's state.
	ORD_BTYPE_BOOLEAN_DEPEND,	// Clicking on a button toggles it's state, button
	// is only enabled if previous button is down.
	ORD_BTYPE_BOOLEAN_COMBINE,	// Clicking on a button toggles it's state, all
	// the buttons states are OR'ed together to get the order state
} ORDBUTTONTYPE;

typedef enum
{
	ORDBUTCLASS_NORMAL,			// A normal button, one order type per line.
//	ORDBUTCLASS_NORMALMIXED,	// A normal button but multiple order types on one line.
	ORDBUTCLASS_FACTORY,		// A factory assignment button.
	ORDBUTCLASS_CYBORGFACTORY,	// A cyborg factory assignment button.
	ORDBUTCLASS_VTOLFACTORY, 	// A VTOL factory assignment button.
} ORDBUTTONCLASS;


/*
 NOTE:
	ORD_BTYPE_BOOLEAN_DEPEND only supports two buttons
	ie button 1 = enable destruct, button 2 = destruct
*/

typedef enum
{
	ORD_JUSTIFY_LEFT,			// Pretty self explanatory really.
	ORD_JUSTIFY_RIGHT,
	ORD_JUSTIFY_CENTER,
	ORD_JUSTIFY_COMBINE,		// allow the button to be put on the same line
	// as other orders with the same justify type
	ORD_NUM_JUSTIFY_TYPES,
} ORDBUTTONJUSTIFY;

// maximum number of orders on one line
#define ORD_MAX_COMBINE_BUTS	3

#define ORD_JUSTIFY_MASK	0x0f
#define ORD_JUSTIFY_NEWLINE	0x10	// Or with ORDBUTTONJUSTIFY enum type to specify start on new line.

typedef struct
{
	ORDBUTTONCLASS Class;					// The class of button.
	SECONDARY_ORDER Order;					// The droid order.
	uint32_t StateMask;						// It's state mask.
	ORDBUTTONTYPE ButType;					// The group type.
	ORDBUTTONJUSTIFY ButJustify;			// Button justification.
	uint32_t ButBaseID;						// Starting widget ID for buttons
	uint16_t NumButs;							// Number of buttons ( = number of states )
	uint16_t AcNumButs;						// Actual bumber of buttons enabled.
	uint16_t ButImageID[MAX_ORDER_BUTS];		// Image ID's for each button ( normal ).
	uint16_t ButGreyID[MAX_ORDER_BUTS];		// Image ID's for each button ( greyed ).
	uint16_t ButHilightID[MAX_ORDER_BUTS];		// Image ID's for each button ( hilight overlay ).
	uint16_t ButTips[MAX_ORDER_BUTS];			// Tip string id for each button.
	SECONDARY_STATE States[MAX_ORDER_BUTS];	// Order state relating to each button.
} ORDERBUTTONS;


typedef struct
{
	uint16_t OrderIndex;		// Index into ORDERBUTTONS array of available orders.
	uint16_t RefCount;			// Number of times this order is referenced.
} AVORDER;


enum
{
	STR_DORD_RANGE1,
	STR_DORD_RANGE2,
	STR_DORD_RANGE3,
	STR_DORD_REPAIR1,
	STR_DORD_REPAIR2,
	STR_DORD_REPAIR3,
	STR_DORD_FIRE1,
	STR_DORD_FIRE2,
	STR_DORD_FIRE3,
	STR_DORD_PATROL,
	STR_DORD_PURSUE,
	STR_DORD_GUARD,
	STR_DORD_HOLDPOS,
	STR_DORD_RETREPAIR,
	STR_DORD_RETBASE,
	STR_DORD_EMBARK,
	STR_DORD_ARMRECYCLE,
	STR_DORD_RECYCLE,
	STR_DORD_FACTORY,
	STR_DORD_CYBORG_FACTORY,
	STR_DORD_FIREDES,
	STR_DORD_VTOL_FACTORY,
	STR_DORD_CIRCLE,
};

// return translated text
static const char *getDORDDescription(int id)
{
	switch ( id )
	{
		case STR_DORD_RANGE1         :
			return _("Short Range");
		case STR_DORD_RANGE2         :
			return _("Long Range");
		case STR_DORD_RANGE3         :
			return _("Optimum Range");
		case STR_DORD_REPAIR1        :
			return _("Retreat at Medium Damage");
		case STR_DORD_REPAIR2        :
			return _("Retreat at Heavy Damage");
		case STR_DORD_REPAIR3        :
			return _("Do or Die!");
		case STR_DORD_FIRE1          :
			return _("Fire-At-Will");
		case STR_DORD_FIRE2          :
			return _("Return Fire");
		case STR_DORD_FIRE3          :
			return _("Hold Fire");
		case STR_DORD_PATROL         :
			return _("Patrol");
		case STR_DORD_PURSUE         :
			return _("Pursue");
		case STR_DORD_GUARD          :
			return _("Guard Position");
		case STR_DORD_HOLDPOS        :
			return _("Hold Position");
		case STR_DORD_RETREPAIR      :
			return _("Return For Repair");
		case STR_DORD_RETBASE        :
			return _("Return To HQ");
		case STR_DORD_EMBARK         :
			return _("Go to Transport");
		case STR_DORD_ARMRECYCLE     :
			return _("Return for Recycling");
		case STR_DORD_RECYCLE        :
			return _("Recycle");
		case STR_DORD_FACTORY        :
			return _("Assign Factory Production");
		case STR_DORD_CYBORG_FACTORY :
			return _("Assign Cyborg Factory Production");
		case STR_DORD_FIREDES        :
			return _("Assign Fire Support");
		case STR_DORD_VTOL_FACTORY   :
			return _("Assign VTOL Factory Production");
		case STR_DORD_CIRCLE         :
			return _("Circle");

		default :
			return "";  // make compiler shut up
	}
};

// Define the order button groups.
ORDERBUTTONS OrderButtons[NUM_ORDERS] =
{
	{
		ORDBUTCLASS_NORMAL,
		DSO_ATTACK_RANGE,
		DSS_ARANGE_MASK,
		ORD_BTYPE_RADIO,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_ATTACK_RANGE,
		3, 0,
		{IMAGE_ORD_RANGE3UP,	IMAGE_ORD_RANGE1UP,	IMAGE_ORD_RANGE2UP},
		{IMAGE_ORD_RANGE3UP,	IMAGE_ORD_RANGE1UP,	IMAGE_ORD_RANGE2UP},
		{IMAGE_DES_HILIGHT,		IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT},
		{STR_DORD_RANGE3,	STR_DORD_RANGE1,	STR_DORD_RANGE2},
		{DSS_ARANGE_DEFAULT,	DSS_ARANGE_SHORT,	DSS_ARANGE_LONG}
	},
	{
		ORDBUTCLASS_NORMAL,
		DSO_REPAIR_LEVEL,
		DSS_REPLEV_MASK,
		ORD_BTYPE_RADIO,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_REPAIR_LEVEL,
		3, 0,
		{IMAGE_ORD_REPAIR3UP,	IMAGE_ORD_REPAIR2UP,	IMAGE_ORD_REPAIR1UP},
		{IMAGE_ORD_REPAIR3UP,	IMAGE_ORD_REPAIR2UP,	IMAGE_ORD_REPAIR1UP},
		{IMAGE_DES_HILIGHT,		IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT},
		{STR_DORD_REPAIR3,	STR_DORD_REPAIR2,	STR_DORD_REPAIR1},
		{DSS_REPLEV_NEVER,	DSS_REPLEV_HIGH,	DSS_REPLEV_LOW}
	},
	{
		ORDBUTCLASS_NORMAL,
		DSO_ATTACK_LEVEL,
		DSS_ALEV_MASK,
		ORD_BTYPE_RADIO,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_ATTACK_LEVEL,
		3, 0,
		{IMAGE_ORD_FATWILLUP,	IMAGE_ORD_RETFIREUP,	IMAGE_ORD_HOLDFIREUP},
		{IMAGE_ORD_FATWILLUP,	IMAGE_ORD_RETFIREUP,	IMAGE_ORD_HOLDFIREUP},
		{IMAGE_DES_HILIGHT,		IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT},
		{STR_DORD_FIRE1,	STR_DORD_FIRE2,	STR_DORD_FIRE3},
		{DSS_ALEV_ALWAYS,	DSS_ALEV_ATTACKED,	DSS_ALEV_NEVER}
	},
	{
		ORDBUTCLASS_NORMAL,
		DSO_FIRE_DESIGNATOR,
		DSS_FIREDES_MASK,
		ORD_BTYPE_BOOLEAN,
		ORD_JUSTIFY_COMBINE,
		IDORDER_FIRE_DESIGNATOR,
		1, 0,
		{IMAGE_ORD_FIREDES_UP,	0,	0},
		{IMAGE_ORD_FIREDES_UP,	0,	0},
		{IMAGE_DES_HILIGHT,	0,	0},
		{STR_DORD_FIREDES,	0,	0},
		{DSS_FIREDES_SET,	0,	0}
	},

	{
		ORDBUTCLASS_NORMAL,
		DSO_PATROL,
		DSS_PATROL_MASK,
		ORD_BTYPE_BOOLEAN,
		ORD_JUSTIFY_COMBINE,
		IDORDER_PATROL,
		1, 0,
		{IMAGE_ORD_PATROLUP,	0,	0},
		{IMAGE_ORD_PATROLUP,	0,	0},
		{IMAGE_DES_HILIGHT,	0,	0},
		{STR_DORD_PATROL,	0,	0},
		{DSS_PATROL_SET,	0,	0}
	},

	{
		ORDBUTCLASS_NORMAL,
		DSO_CIRCLE,
		DSS_CIRCLE_MASK,
		ORD_BTYPE_BOOLEAN,
		ORD_JUSTIFY_COMBINE,
		IDORDER_CIRCLE,
		1, 0,
		{IMAGE_ORD_CIRCLEUP,	0,	0},
		{IMAGE_ORD_CIRCLEUP,	0,	0},
		{IMAGE_DES_HILIGHT,	0,	0},
		{STR_DORD_CIRCLE,	0,	0},
		{DSS_CIRCLE_SET,	0,	0}
	},

	{
		ORDBUTCLASS_NORMAL,
		DSO_HALTTYPE,
		DSS_HALT_MASK,
		ORD_BTYPE_RADIO,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_HALT_TYPE,
		3, 0,
		{IMAGE_ORD_PURSUEUP,	IMAGE_ORD_GUARDUP,	IMAGE_ORD_HALTUP},
		{IMAGE_ORD_PURSUEUP,	IMAGE_ORD_GUARDUP,	IMAGE_ORD_HALTUP},
		{IMAGE_DES_HILIGHT,		IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT},
		{STR_DORD_PURSUE,	STR_DORD_GUARD,	STR_DORD_HOLDPOS},
		{DSS_HALT_PURSUE,	DSS_HALT_GUARD,	DSS_HALT_HOLD}
	},
	{
		ORDBUTCLASS_NORMAL,
		DSO_RETURN_TO_LOC,
		DSS_RTL_MASK,
		ORD_BTYPE_RADIO,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_RETURN,
		3, 0,

		{IMAGE_ORD_RTRUP,	IMAGE_ORD_GOTOHQUP,	IMAGE_ORD_EMBARKUP},
		{IMAGE_ORD_RTRUP,	IMAGE_ORD_GOTOHQUP,	IMAGE_ORD_EMBARKUP},

		{IMAGE_DES_HILIGHT,		IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT},
		{STR_DORD_RETREPAIR,	STR_DORD_RETBASE,	STR_DORD_EMBARK},
		{DSS_RTL_REPAIR,	DSS_RTL_BASE,	DSS_RTL_TRANSPORT},
	},
	/*	{
			ORDBUTCLASS_NORMAL,
			DSO_HOLD,
			DSS_HOLD_MASK,
			ORD_BTYPE_BOOLEAN,
			ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
			IDORDER_HOLD,
			1,0,
			{IMAGE_ORD_HALTUP,	0,	0},
			{IMAGE_ORD_HALTUP,	0,	0},
			{IMAGE_DES_HILIGHT,	0,	0},
			{STR_DORD_HOLDPOS,	0,	0},
			{DSS_HOLD_SET,	0,	0}
		},
	//	{
	//		ORDBUTCLASS_NORMALMIXED,
	//		DSO_RETURN_TO_BASE, DSO_EMBARK,		 DSO_RETURN_TO_REPAIR,
	//		DSS_RTB_MASK,		DSS_EMBARK_MASK, DSS_RTR_MASK,
	//		ORD_BTYPE_BOOLEAN,
	//		ORD_JUSTIFY_CENTER,
	//		IDORDER_RETURN_TO_BASE,IDORDER_EMBARK,IDORDER_RETURN_TO_REPAIR,
	//		3,0,
	//		{IMAGE_ORD_GOTOHQUP,	IMAGE_ORD_GOTOHQUP,	IMAGE_ORD_GOTOHQUP},
	//		{IMAGE_ORD_GOTOHQUP,	IMAGE_ORD_GOTOHQUP,	IMAGE_ORD_GOTOHQUP},
	//		{IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT},
	//		{STR_DORD_RETBASE,	STR_DORD_RETBASE,	STR_DORD_RETREPAIR},
	//		{DSS_RTB_SET,	DSS_EMBARK_SET,	DSS_RTR_SET}
	//	},
		{
			ORDBUTCLASS_NORMAL,
			DSO_RETURN_TO_BASE,
			DSS_RTB_MASK,
			ORD_BTYPE_BOOLEAN,
			ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
			IDORDER_RETURN_TO_BASE,
			1,0,
			{IMAGE_ORD_GOTOHQUP,	0,	0},
			{IMAGE_ORD_GOTOHQUP,	0,	0},
			{IMAGE_DES_HILIGHT,	0,	0},
			{STR_DORD_RETBASE,	0,	0},
			{DSS_RTB_SET,	0,	0}
		},
		{
			ORDBUTCLASS_NORMAL,
			DSO_EMBARK,
			DSS_EMBARK_MASK,
			ORD_BTYPE_BOOLEAN,
			ORD_JUSTIFY_CENTER,
			IDORDER_EMBARK,
			1,0,
			{IMAGE_ORD_GOTOHQUP,	0,	0},
			{IMAGE_ORD_GOTOHQUP,	0,	0},
			{IMAGE_DES_HILIGHT,	0,	0},
			{STR_DORD_EMBARK,	0,	0},
			{DSS_EMBARK_SET,	0,	0}
		},
		{
			ORDBUTCLASS_NORMAL,
			DSO_RETURN_TO_REPAIR,
			DSS_RTR_MASK,
			ORD_BTYPE_BOOLEAN,
			ORD_JUSTIFY_CENTER,
			IDORDER_RETURN_TO_REPAIR,
			1,0,
			{IMAGE_ORD_GOTOHQUP,	0,	0},
			{IMAGE_ORD_GOTOHQUP,	0,	0},
			{IMAGE_DES_HILIGHT,	0,	0},
			{STR_DORD_RETREPAIR,	0,	0},
			{DSS_RTR_SET,	0,	0}
		},
	//	{
	//		ORDBUTCLASS_NORMAL,
	//		DSO_DESTRUCT,
	//		DSS_DESTRUCT_MASK,
	//		ORD_BTYPE_BOOLEAN_DEPEND,
	//		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
	//		IDORDER_DESTRUCT,
	//		2,0,
	//		{IMAGE_ORD_DESTRUCT1UP,	IMAGE_ORD_DESTRUCT2UP,	0},
	//		{IMAGE_ORD_DESTRUCT1UP,	IMAGE_ORD_DESTRUCT2GREY,	0},
	//		{IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT,	0},
	//		{STR_DORD_ARMDESTRUCT,	STR_DORD_DESTRUCT,	0},
	//		{DSS_DESTRUCT_SET,	DSS_DESTRUCT_SET,	0}
	//	},*/
	{
		ORDBUTCLASS_NORMAL,
		DSO_RECYCLE,
		DSS_RECYCLE_MASK,
		ORD_BTYPE_BOOLEAN_DEPEND,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_RECYCLE,
		2, 0,
		{IMAGE_ORD_DESTRUCT1UP,	IMAGE_ORD_DESTRUCT2UP,	0},
		{IMAGE_ORD_DESTRUCT1UP,	IMAGE_ORD_DESTRUCT2GREY,	0},
		{IMAGE_DES_HILIGHT,	IMAGE_DES_HILIGHT,	0},
		{STR_DORD_ARMRECYCLE,	STR_DORD_RECYCLE,	0},
		{DSS_RECYCLE_SET,	DSS_RECYCLE_SET,	0}
	},
	{
		ORDBUTCLASS_FACTORY,
		DSO_ASSIGN_PRODUCTION,
		DSS_ASSPROD_MASK,
		ORD_BTYPE_BOOLEAN_COMBINE,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_ASSIGN_PRODUCTION,
		5, 0,
		{IMAGE_ORD_FAC1UP,		IMAGE_ORD_FAC2UP,	IMAGE_ORD_FAC3UP,	IMAGE_ORD_FAC4UP,	IMAGE_ORD_FAC5UP	},
		{IMAGE_ORD_FAC1UP,		IMAGE_ORD_FAC2UP,	IMAGE_ORD_FAC3UP,	IMAGE_ORD_FAC4UP,	IMAGE_ORD_FAC5UP	},
		{IMAGE_ORD_FACHILITE,	IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE	},
		{STR_DORD_FACTORY,	STR_DORD_FACTORY,	STR_DORD_FACTORY,	STR_DORD_FACTORY,	STR_DORD_FACTORY},
		{
			0x01 << DSS_ASSPROD_SHIFT, 0x02 << DSS_ASSPROD_SHIFT, 0x04 << DSS_ASSPROD_SHIFT,
			0x08 << DSS_ASSPROD_SHIFT, 0x10 << DSS_ASSPROD_SHIFT
		}
	},
	{
		ORDBUTCLASS_CYBORGFACTORY,
		DSO_ASSIGN_CYBORG_PRODUCTION,
		DSS_ASSPROD_MASK,
		ORD_BTYPE_BOOLEAN_COMBINE,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_ASSIGN_CYBORG_PRODUCTION,
		5, 0,
		{IMAGE_ORD_FAC1UP,		IMAGE_ORD_FAC2UP,	IMAGE_ORD_FAC3UP,	IMAGE_ORD_FAC4UP,	IMAGE_ORD_FAC5UP	},
		{IMAGE_ORD_FAC1UP,		IMAGE_ORD_FAC2UP,	IMAGE_ORD_FAC3UP,	IMAGE_ORD_FAC4UP,	IMAGE_ORD_FAC5UP	},
		{IMAGE_ORD_FACHILITE,	IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE	},
		{STR_DORD_CYBORG_FACTORY,	STR_DORD_CYBORG_FACTORY,	STR_DORD_CYBORG_FACTORY,	STR_DORD_CYBORG_FACTORY,	STR_DORD_CYBORG_FACTORY},
		{
			0x01 << DSS_ASSPROD_CYBORG_SHIFT, 0x02 << DSS_ASSPROD_CYBORG_SHIFT, 0x04 << DSS_ASSPROD_CYBORG_SHIFT,
			0x08 << DSS_ASSPROD_CYBORG_SHIFT, 0x10 << DSS_ASSPROD_CYBORG_SHIFT
		}
	},
	{
		ORDBUTCLASS_VTOLFACTORY,
		DSO_ASSIGN_VTOL_PRODUCTION,
		DSS_ASSPROD_MASK,
		ORD_BTYPE_BOOLEAN_COMBINE,
		ORD_JUSTIFY_CENTER | ORD_JUSTIFY_NEWLINE,
		IDORDER_ASSIGN_VTOL_PRODUCTION,
		5, 0,
		{IMAGE_ORD_FAC1UP,		IMAGE_ORD_FAC2UP,	IMAGE_ORD_FAC3UP,	IMAGE_ORD_FAC4UP,	IMAGE_ORD_FAC5UP	},
		{IMAGE_ORD_FAC1UP,		IMAGE_ORD_FAC2UP,	IMAGE_ORD_FAC3UP,	IMAGE_ORD_FAC4UP,	IMAGE_ORD_FAC5UP	},
		{IMAGE_ORD_FACHILITE,	IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE, IMAGE_ORD_FACHILITE	},
		{STR_DORD_VTOL_FACTORY,	STR_DORD_VTOL_FACTORY,	STR_DORD_VTOL_FACTORY,	STR_DORD_VTOL_FACTORY,	STR_DORD_VTOL_FACTORY},
		{
			0x01 << DSS_ASSPROD_VTOL_SHIFT, 0x02 << DSS_ASSPROD_VTOL_SHIFT, 0x04 << DSS_ASSPROD_VTOL_SHIFT,
			0x08 << DSS_ASSPROD_VTOL_SHIFT, 0x10 << DSS_ASSPROD_VTOL_SHIFT
		}
	},
};


static uint16_t NumSelectedDroids;
static DROID *SelectedDroids[MAX_SELECTED_DROIDS];
static STRUCTURE *psSelectedFactory;
static uint16_t NumAvailableOrders;
static AVORDER AvailableOrders[MAX_AVAILABLE_ORDERS];


BOOL OrderUp = false;

// update the order interface only if it is already open.
// NOTE: Unused! BOOL intUpdateOrder(DROID *psDroid)
BOOL intUpdateOrder(DROID *psDroid)
{
	if(widgGetFromID(psWScreen, IDORDER_FORM) != NULL && OrderUp)
	{
		widgDelete(psWScreen, IDORDER_CLOSE);
		//changed to a BASE_OBJECT to accomodate the factories - AB 21/04/99
		//intAddOrder(psDroid);
		intAddOrder((BASE_OBJECT *)psDroid);
	}

	return true;
}

// Build a list of currently selected droids.
// Returns true if droids were selected.
//
static BOOL BuildSelectedDroidList(void)
{
	DROID *psDroid;

	for(psDroid = apsDroidLists[selectedPlayer]; psDroid; psDroid = psDroid->psNext)
	{
		if(psDroid->selected)
		{
			if(NumSelectedDroids < MAX_SELECTED_DROIDS)
			{
				SelectedDroids[NumSelectedDroids] = psDroid;
				NumSelectedDroids++;
			}
		}
	}

	if(NumSelectedDroids)
	{
		return true;
	}

	return false;
}

// Build a list of orders available for the selected group of droids.
//
static BOOL BuildDroidOrderList(void)
{
	uint16_t OrdIndex;
	uint16_t i, j;
	BOOL Found;
	BOOL Sorted;
	AVORDER Tmp;

	NumAvailableOrders = 0;

	for(j = 0; j < NumSelectedDroids; j++)
	{

		for(OrdIndex = 0; OrdIndex < NUM_ORDERS; OrdIndex++)
		{

			// Is the order available?
			if(secondarySupported(SelectedDroids[j], OrderButtons[OrdIndex].Order))
			{
				if(NumAvailableOrders < MAX_AVAILABLE_ORDERS)
				{
					// Have we already got this order?
					Found = false;
					for(i = 0; i < NumAvailableOrders; i++)
					{
						if(AvailableOrders[i].OrderIndex == OrdIndex)
						{
							// Yes! Then increment it's reference count.
							AvailableOrders[i].RefCount++;
							Found = true;
						}
					}

					if(!Found)
					{
						// Not already got it so add it to the list of available orders.
						AvailableOrders[NumAvailableOrders].OrderIndex = OrdIndex;
						AvailableOrders[NumAvailableOrders].RefCount = 1;
						NumAvailableOrders++;
					}
				}
			}
		}
	}

	if(NumAvailableOrders == 0)
	{
		return false;
	}

	if(NumAvailableOrders > 1)
	{
		// Sort by Order index, A bubble sort? I know but it's only
		// a small list so what the hell.
		do
		{
			Sorted = true;
			for(i = 0; i < NumAvailableOrders - 1; i++)
			{
				if(AvailableOrders[i].OrderIndex > AvailableOrders[i + 1].OrderIndex)
				{
					Tmp = AvailableOrders[i];
					AvailableOrders[i] = AvailableOrders[i + 1];
					AvailableOrders[i + 1] = Tmp;
					Sorted = false;
				}
			}
		}
		while(!Sorted);
	}

	return true;
}

// Build a list of orders available for the selected structure.
static BOOL BuildStructureOrderList(STRUCTURE *psStructure)
{
	//only valid for Factories (at the moment)
	if (!StructIsFactory(psStructure))
	{
		ASSERT( false, "BuildStructureOrderList: structure is not a factory" );
		return false;
	}

	//this can be hard-coded!
	AvailableOrders[0].OrderIndex = 0;//DSO_ATTACK_RANGE;
	AvailableOrders[0].RefCount = 1;
	AvailableOrders[1].OrderIndex = 1;//DSO_REPAIR_LEVEL;
	AvailableOrders[1].RefCount = 1;
	AvailableOrders[2].OrderIndex = 2;//DSO_ATTACK_LEVEL;
	AvailableOrders[2].RefCount = 1;
	AvailableOrders[3].OrderIndex = 5;//DSO_HALTTYPE;
	AvailableOrders[3].RefCount = 1;

	NumAvailableOrders = 4;

	return true;
}


// return the state for an order for all the units selected
// if there are multiple states then don't return a state
static int32_t GetSecondaryStates(SECONDARY_ORDER sec)
{
	int32_t	i;
	SECONDARY_STATE state, currState;
	BOOL	bFirst;

	state = 0;
	bFirst = true;
	//handle a factory being selected - AB 22/04/99
	if (psSelectedFactory)
	{
		if (getFactoryState(psSelectedFactory, sec, &currState))
		{
			state = currState;
		}
	}
	else //droids
	{
		for (i = 0; i < NumSelectedDroids; i++)
		{
			currState = secondaryGetState(SelectedDroids[i], sec);
			if (bFirst)
			{
				state = currState;
				bFirst = false;
			}
			else if (state != currState)
			{
				state = 0;
			}
		}
	}

	return state;
}


static uint32_t GetImageWidth(IMAGEFILE *ImageFile, uint32_t ImageID)
{
	return iV_GetImageWidth(ImageFile, (uint16_t)ImageID);
}

static uint32_t GetImageHeight(IMAGEFILE *ImageFile, uint32_t ImageID)
{
	return iV_GetImageHeight(ImageFile, (uint16_t)ImageID);
}


// Add the droid order screen.
// Returns true if the form was displayed ok.
//
//changed to a BASE_OBJECT to accomodate the factories - AB 21/04/99
//BOOL _intAddOrder(DROID *Droid)
BOOL intAddOrder(BASE_OBJECT *psObj)
{
	W_FORMINIT			sFormInit;
	W_BUTINIT			sButInit;
	BOOL Animate = true;
	SECONDARY_STATE State;
	uint16_t i, j; //,k;
	uint16_t OrdIndex;
	W_FORM *Form;
	uint16_t Height, NumDisplayedOrders;
	uint16_t NumButs;
	uint16_t NumJustifyButs, NumCombineButs, NumCombineBefore;
	BOOL  bLastCombine, bHidden;

	//added to accomodate the factories - AB 21/04/99
	DROID       *Droid;
	STRUCTURE   *psStructure;


	if(bInTutorial)
	{
		// No RMB orders in tutorial!!
		return(false);
	}

	// Is the form already up?
	if(widgGetFromID(psWScreen, IDORDER_FORM) != NULL)
	{
		intRemoveOrderNoAnim();
		Animate = false;
	}
	// Is the stats window up?
	if(widgGetFromID(psWScreen, IDSTAT_FORM) != NULL)
	{
		intRemoveStatsNoAnim();
		Animate = false;
	}

	if (psObj)
	{
		if (psObj->type == OBJ_DROID)
		{
			Droid = (DROID *)psObj;
			psStructure =  NULL;
		}
		else if (psObj->type == OBJ_STRUCTURE)
		{
			Droid = NULL;
			psStructure = (STRUCTURE *)psObj;
			psSelectedFactory = psStructure;
		}
		else
		{
			ASSERT( false, "_intAddOrder: Invalid object type" );
			Droid = NULL;
			psStructure =  NULL;
		}
	}
	else
	{
		Droid = NULL;
		psStructure =  NULL;
	}



	//	intResetScreen(true);
	setWidgetsStatus(true);

	NumAvailableOrders = 0;
	NumSelectedDroids = 0;

	// Selected droid is a command droid?
	if ((Droid != NULL) && (Droid->droidType == DROID_COMMAND))
	{
		// displaying for a command droid - ignore any other droids
		SelectedDroids[0] = Droid;
		NumSelectedDroids = 1;
	}
	//added to accomodate the factories - AB 21/04/99
	else if (psStructure != NULL)
	{
		if (!BuildStructureOrderList(psStructure))
		{
			return false;
		}
	}
	// Otherwise build a list of selected droids.
	else if(!BuildSelectedDroidList())
	{
		// If no droids selected then see if we were given a specific droid.
		if(Droid != NULL)
		{
			// and put it in the list.
			SelectedDroids[0] = Droid;
			NumSelectedDroids = 1;
		}
	}

	// Build a list of orders available for the list of selected droids. - if a factory has not been selected
	if (psStructure == NULL)
	{
		if(!BuildDroidOrderList())
		{
			// If no orders then return;
			return false;
		}
	}

	widgEndScreen(psWScreen);

	/* Create the basic form */

	memset(&sFormInit, 0, sizeof(W_FORMINIT));
	sFormInit.formID = 0;
	sFormInit.id = IDORDER_FORM;
	sFormInit.style = WFORM_PLAIN;
	sFormInit.x = ORDER_X;
	sFormInit.y = ORDER_Y;
	sFormInit.width = ORDER_WIDTH;
	sFormInit.height = 	ORDER_HEIGHT;
	// If the window was closed then do open animation.
	if(Animate)
	{
		sFormInit.pDisplay = intOpenPlainForm;
		sFormInit.disableChildren = true;
	}
	else
	{
		// otherwise just recreate it.
		sFormInit.pDisplay = intDisplayPlainForm;
	}
	if (!widgAddForm(psWScreen, &sFormInit))
	{
		return false;
	}



	// Add the close button.
	memset(&sButInit, 0, sizeof(W_BUTINIT));
	sButInit.formID = IDORDER_FORM;
	sButInit.id = IDORDER_CLOSE;
	sButInit.style = WBUT_PLAIN;
	sButInit.x = ORDER_WIDTH - CLOSE_WIDTH;
	sButInit.y = 0;
	sButInit.width = CLOSE_WIDTH;
	sButInit.height = CLOSE_HEIGHT;
	sButInit.pTip = _("Close");
	sButInit.FontID = font_regular;
	sButInit.pDisplay = intDisplayImageHilight;
	sButInit.UserData = PACKint32_t_TRI(0, IMAGE_CLOSEHILIGHT , IMAGE_CLOSE);
	if (!widgAddButton(psWScreen, &sButInit))
	{
		return false;
	}


	memset(&sButInit, 0, sizeof(W_BUTINIT));
	sButInit.formID = IDORDER_FORM;
	sButInit.id = IDORDER_CLOSE + 1;
	sButInit.style = WBUT_PLAIN;
	sButInit.pDisplay = intDisplayButtonHilight;
	sButInit.FontID = font_regular;
	sButInit.y = ORDER_BUTY;

	Height = 0;
	NumDisplayedOrders = 0;

	for(j = 0; ((j < NumAvailableOrders) && (NumDisplayedOrders < MAX_DISPLAYABLE_ORDERS)); j++)
	{
		OrdIndex = AvailableOrders[j].OrderIndex;

		// Get current order state.
		State = GetSecondaryStates(OrderButtons[OrdIndex].Order);

		// Get number of buttons.
		NumButs = OrderButtons[OrdIndex].NumButs;
		// Set actual number of buttons.
		OrderButtons[OrdIndex].AcNumButs = NumButs;

		// Handle special case for factory -> command droid assignment buttons.
		switch (OrderButtons[OrdIndex].Class)
		{
			case ORDBUTCLASS_FACTORY:
				NumButs = countAssignableFactories((uint8_t)selectedPlayer, FACTORY_FLAG);
				break;
			case ORDBUTCLASS_CYBORGFACTORY:
				NumButs = countAssignableFactories((uint8_t)selectedPlayer, CYBORG_FLAG);
				break;
			case ORDBUTCLASS_VTOLFACTORY:
				NumButs = countAssignableFactories((uint8_t)selectedPlayer, VTOL_FLAG);
				break;
			default:
				break;
		}

		sButInit.id = OrderButtons[OrdIndex].ButBaseID;

		NumJustifyButs = NumButs;
//		for(k=j; k<NumAvailableOrders; k++) {
//	   		uint16_t Index = AvailableOrders[j].OrderIndex;
//			if(OrderButtons[OrdIndex].ButJustify & ORD_JUSTIFY_NEWLINE) {
//				DBPRINTF(("NewLine %d \n",k);
//				break;
//			} else {
//				DBPRINTF(("SameLine %d \n",k);
//			}
//		}

		bLastCombine = false;

		switch (OrderButtons[OrdIndex].ButJustify & ORD_JUSTIFY_MASK)
		{
			case ORD_JUSTIFY_LEFT:
				sButInit.x = ORDER_BUTX;
				break;

			case ORD_JUSTIFY_RIGHT:
				sButInit.x = (int16_t)(sFormInit.width - ORDER_BUTX -
									   ( ((NumJustifyButs * GetImageWidth(IntImages, OrderButtons[OrdIndex].ButImageID[0])) +
										  ((NumJustifyButs - 1) * ORDER_BUTGAP ) ) ));
				break;

			case ORD_JUSTIFY_CENTER:
//				sButInit.x = (int16_t)((sFormInit.width / 2) -
//						( ((NumJustifyButs * GetImageWidth(IntImages,OrderButtons[OrdIndex].ButImageID[0])) +
//						((NumJustifyButs-1) * ORDER_BUTGAP ) ) / 2 ));
				sButInit.x = ((int16_t)((sFormInit.width ) -
										( ((NumJustifyButs * GetImageWidth(IntImages, OrderButtons[OrdIndex].ButImageID[0])) +
										   ((NumJustifyButs - 1) * ORDER_BUTGAP ) )))) / 2;
				break;

			case ORD_JUSTIFY_COMBINE:
				// see how many are on this line before the button
				NumCombineBefore = 0;
				for(i = 0; i < j; i++)
				{
					if ((OrderButtons[AvailableOrders[i].OrderIndex].ButJustify & ORD_JUSTIFY_MASK)
							== ORD_JUSTIFY_COMBINE)
					{
						NumCombineBefore += 1;
					}
				}
				NumCombineButs = (uint16_t)(NumCombineBefore + 1);

				// now see how many in total
				for(i = (uint16_t)(j + 1); i < NumAvailableOrders; i++)
				{
					if ((OrderButtons[AvailableOrders[i].OrderIndex].ButJustify & ORD_JUSTIFY_MASK)
							== ORD_JUSTIFY_COMBINE)
					{
						NumCombineButs += 1;
					}
				}

				// get position on line
				NumCombineButs = (uint16_t)(NumCombineButs - (NumCombineBefore - (NumCombineBefore % ORD_MAX_COMBINE_BUTS)));

				if (NumCombineButs >= ORD_MAX_COMBINE_BUTS)
				{
					// the buttons will fill the line
					sButInit.x = (int16_t)(ORDER_BUTX +
										   (GetImageWidth(IntImages, OrderButtons[OrdIndex].ButImageID[0]) + ORDER_BUTGAP ) * NumCombineBefore);
				}
				else
				{
					// center the buttons
					sButInit.x = (int16_t)((sFormInit.width / 2) -
										   ( ((NumCombineButs * GetImageWidth(IntImages, OrderButtons[OrdIndex].ButImageID[0])) +
											  ((NumCombineButs - 1) * ORDER_BUTGAP ) ) / 2 ));
					sButInit.x = (int16_t)(sButInit.x +
										   (GetImageWidth(IntImages, OrderButtons[OrdIndex].ButImageID[0]) + ORDER_BUTGAP ) * NumCombineBefore);
				}

				// see if need to start a new line of buttons
				if ((NumCombineBefore + 1) == (NumCombineButs % ORD_MAX_COMBINE_BUTS))
				{
					bLastCombine = true;
				}

				break;
		}

		for(i = 0; i < OrderButtons[OrdIndex].AcNumButs; i++)
		{
			sButInit.pTip = getDORDDescription(OrderButtons[OrdIndex].ButTips[i]);
			sButInit.width = (uint16_t)GetImageWidth(IntImages, OrderButtons[OrdIndex].ButImageID[i]);
			sButInit.height = (uint16_t)GetImageHeight(IntImages, OrderButtons[OrdIndex].ButImageID[i]);
			sButInit.UserData = PACKint32_t_TRI(
									OrderButtons[OrdIndex].ButGreyID[i],
									OrderButtons[OrdIndex].ButHilightID[i],
									OrderButtons[OrdIndex].ButImageID[i]
								);
			if(!widgAddButton(psWScreen, &sButInit))
			{
				return false;
			}

			// Set the default state for the button.
			switch(OrderButtons[OrdIndex].ButType)
			{
				case ORD_BTYPE_RADIO:
				case ORD_BTYPE_BOOLEAN:
					if((State & OrderButtons[OrdIndex].StateMask) == (uint32_t)OrderButtons[OrdIndex].States[i])
					{
						widgSetButtonState(psWScreen, sButInit.id, WBUT_CLICKLOCK);
					}
					else
					{
						widgSetButtonState(psWScreen, sButInit.id, 0);
					}
					break;

				case ORD_BTYPE_BOOLEAN_DEPEND:
					if((State & OrderButtons[OrdIndex].StateMask) == (uint32_t)OrderButtons[OrdIndex].States[i])
					{
						widgSetButtonState(psWScreen, sButInit.id, WBUT_CLICKLOCK);
					}
					else
					{
						if(i == 0)
						{
							widgSetButtonState(psWScreen, sButInit.id, 0);
						}
						else
						{
							widgSetButtonState(psWScreen, sButInit.id, WBUT_DISABLE);
						}
					}
					break;
				case ORD_BTYPE_BOOLEAN_COMBINE:
					if( State & (uint32_t)OrderButtons[OrdIndex].States[i] )
					{
						widgSetButtonState(psWScreen, sButInit.id, WBUT_CLICKLOCK);
					}
					break;
			}

			// may not add a button if the factory doesn't exist
			bHidden = false;
			switch (OrderButtons[OrdIndex].Class)
			{
				case ORDBUTCLASS_FACTORY:
					if (!checkFactoryExists(selectedPlayer, FACTORY_FLAG, i))
					{
						widgHide(psWScreen, sButInit.id);
						bHidden = true;
					}
					break;
				case ORDBUTCLASS_CYBORGFACTORY:
					if (!checkFactoryExists(selectedPlayer, CYBORG_FLAG, i))
					{
						widgHide(psWScreen, sButInit.id);
						bHidden = true;
					}
					break;
				case ORDBUTCLASS_VTOLFACTORY:
					if (!checkFactoryExists(selectedPlayer, VTOL_FLAG, i))
					{
						widgHide(psWScreen, sButInit.id);
						bHidden = true;
					}
					break;
				default:
					break;
			}

			if (!bHidden)
			{

				sButInit.x = (int16_t)(sButInit.x + sButInit.width + ORDER_BUTGAP);
			}
			sButInit.id++;
		}

		if (((OrderButtons[OrdIndex].ButJustify & ORD_JUSTIFY_MASK) != ORD_JUSTIFY_COMBINE) ||
				bLastCombine)
		{
			sButInit.y = (int16_t)(sButInit.y + sButInit.height + ORDER_BUTGAP);
			Height = (uint16_t)(Height + sButInit.height + ORDER_BUTGAP);
		}
		NumDisplayedOrders ++;
	}

	// Now we know how many orders there are we can resize the form accordingly.
	Form = (W_FORM *)widgGetFromID(psWScreen, IDORDER_FORM);
	Form->height = (uint16_t)(Height + CLOSE_HEIGHT + ORDER_BUTGAP);
	Form->y = (int16_t)(ORDER_BOTTOMY - Form->height);



	OrderUp = true;

	return true;
}


// Do any housekeeping for the droid order screen.
// Any droids that die now get set to NULL - John.
// No droids being selected no longer removes the screen,
// this lets the screen work with command droids - John.
void intRunOrder(void)
{
	uint16_t i;
	uint32_t NumDead = 0;
	uint32_t NumSelected = 0;

	// Check to see if there all dead or unselected.
	for(i = 0; i < NumSelectedDroids; i++)
	{
		if (SelectedDroids[i])
		{
//			if(SelectedDroids[i]->selected) {
			NumSelected++;
//			}
			if(SelectedDroids[i]->died)
			{
				NumDead++;
				SelectedDroids[i] = NULL;
			}
		}
	}

	// If all dead then remove the screen.
	if(NumDead == NumSelectedDroids)
	{
		//might have a factory selected
		if (psSelectedFactory == NULL)
		{
			intRemoveOrder();
			return;
		}
	}

	// If droids no longer selected then remove screen.
	if(NumSelected == 0)
	{
		//might have a factory selected
		if (psSelectedFactory == NULL)
		{
			intRemoveOrder();
			return;
		}
	}
}


// Set the secondary order state for all currently selected droids. And Factory (if one selected)
// Returns true if successful.
//
static BOOL SetSecondaryState(SECONDARY_ORDER sec, SECONDARY_STATE State)
{
	uint16_t i;

	for(i = 0; i < NumSelectedDroids; i++)
	{
		if (SelectedDroids[i])
		{
			//Only set the state if it's not a transporter.
			if(SelectedDroids[i]->droidType != DROID_TRANSPORTER)
			{
				if(!secondarySetState(SelectedDroids[i], sec, State))
				{
					return false;
				}
			}
		}
	}
	//set the Factory settings
	if (psSelectedFactory)
	{
		if (!setFactoryState(psSelectedFactory, sec, State))
		{
			return false;
		}
	}

	return true;
}


// Process the droid order screen.
//
void intProcessOrder(uint32_t id)
{
	uint16_t i;
	uint16_t OrdIndex;
	uint32_t BaseID;
	uint32_t StateIndex;
	uint32_t CombineState;

	if(id == IDORDER_CLOSE)
	{
		intRemoveOrder();
		if (intMode == INT_ORDER)
		{
			intMode = INT_NORMAL;
		}
		else
		{
			/* Unlock the stats button */
			widgSetButtonState(psWScreen, objStatID, 0);
			intMode = INT_OBJECT;
		}
		return;
	}

	for(OrdIndex = 0; OrdIndex < NUM_ORDERS; OrdIndex++)
	{
		BaseID = OrderButtons[OrdIndex].ButBaseID;

		switch(OrderButtons[OrdIndex].ButType)
		{
			case ORD_BTYPE_RADIO:
				if( (id >= BaseID) && (id < BaseID + OrderButtons[OrdIndex].AcNumButs) )
				{
					StateIndex = id - BaseID;

					for(i = 0; i < OrderButtons[OrdIndex].AcNumButs; i++)
					{
						widgSetButtonState(psWScreen, BaseID + i, 0);
					}
					if (SetSecondaryState(OrderButtons[OrdIndex].Order,
										  OrderButtons[OrdIndex].States[StateIndex] & OrderButtons[OrdIndex].StateMask))
					{
						widgSetButtonState(psWScreen, id, WBUT_CLICKLOCK);
					}
				}
				break;

			case ORD_BTYPE_BOOLEAN:
				if( (id >= BaseID) && (id < BaseID + OrderButtons[OrdIndex].AcNumButs) )
				{
					StateIndex = id - BaseID;

					if(widgGetButtonState(psWScreen, id) & WBUT_CLICKLOCK)
					{
						widgSetButtonState(psWScreen, id, 0);
						SetSecondaryState(OrderButtons[OrdIndex].Order, 0);
					}
					else
					{
						widgSetButtonState(psWScreen, id, WBUT_CLICKLOCK);
						SetSecondaryState(OrderButtons[OrdIndex].Order,
										  OrderButtons[OrdIndex].States[StateIndex] & OrderButtons[OrdIndex].StateMask);
					}

				}
				break;

			case ORD_BTYPE_BOOLEAN_DEPEND:
				// Toggle the state of this button.
				if(id == BaseID)
				{
					if(widgGetButtonState(psWScreen, id) & WBUT_CLICKLOCK)
					{
						widgSetButtonState(psWScreen, id, 0);
						// Disable the dependant button.
						widgSetButtonState(psWScreen, id + 1, WBUT_DISABLE);
					}
					else
					{
						widgSetButtonState(psWScreen, id, WBUT_CLICKLOCK);
						// Enable the dependant button.
						widgSetButtonState(psWScreen, id + 1, 0);
					}
				}
				if( (id > BaseID) && (id < BaseID + OrderButtons[OrdIndex].AcNumButs) )
				{
					// If the previous button is down ( armed )..
					if(widgGetButtonState(psWScreen, id - 1) & WBUT_CLICKLOCK)
					{
						// Toggle the state of this button.
						if(widgGetButtonState(psWScreen, id) & WBUT_CLICKLOCK)
						{
							widgSetButtonState(psWScreen, id, 0);
							SetSecondaryState(OrderButtons[OrdIndex].Order, 0);
						}
						else
						{
							StateIndex = id - BaseID;

							widgSetButtonState(psWScreen, id, WBUT_CLICKLOCK);
							SetSecondaryState(OrderButtons[OrdIndex].Order,
											  OrderButtons[OrdIndex].States[StateIndex] & OrderButtons[OrdIndex].StateMask);
						}
					}
				}
				break;
			case ORD_BTYPE_BOOLEAN_COMBINE:
				if( (id >= BaseID) && (id < BaseID + OrderButtons[OrdIndex].AcNumButs) )
				{
					// Toggle the state of this button.
					if(widgGetButtonState(psWScreen, id) & WBUT_CLICKLOCK)
					{
						widgSetButtonState(psWScreen, id, 0);
					}
					else
					{
						widgSetButtonState(psWScreen, id, WBUT_CLICKLOCK);
					}

					// read the state of all the buttons to get the final state
					CombineState = 0;
					for (StateIndex = 0; StateIndex < OrderButtons[OrdIndex].AcNumButs; StateIndex++)
					{
						if ( widgGetButtonState(psWScreen, BaseID + StateIndex) & WBUT_CLICKLOCK )
						{
							CombineState |= OrderButtons[OrdIndex].States[StateIndex];
						}
					}

					// set the final state
					SetSecondaryState(OrderButtons[OrdIndex].Order,
									  CombineState & OrderButtons[OrdIndex].StateMask);
				}
				break;
		}
	}
}


// check whether the order list has changed
//works on factories now as well - AB 21/04/99
//static BOOL CheckDroidOrderList(void)
static BOOL CheckObjectOrderList(void)
{
	uint16_t OrdIndex;
	uint16_t i, j;
	BOOL Found;
	BOOL Sorted;
	AVORDER Tmp;
	uint16_t	NumNewOrders;
	AVORDER NewAvailableOrders[MAX_AVAILABLE_ORDERS];

	NumNewOrders = 0;

	//added for factories - AB 21/04/99
	if (psSelectedFactory != NULL)
	{
		//only valid for Factories (at the moment)
		if (!StructIsFactory(psSelectedFactory))
		{
			ASSERT( false, "CheckObjectOrderList: structure is not a factory" );
			return false;
		}

		//this can be hard-coded!
		NewAvailableOrders[0].OrderIndex = 0;//DSO_ATTACK_RANGE;
		NewAvailableOrders[0].RefCount = 1;
		NewAvailableOrders[1].OrderIndex = 1;//DSO_REPAIR_LEVEL;
		NewAvailableOrders[1].RefCount = 1;
		NewAvailableOrders[2].OrderIndex = 2;//DSO_ATTACK_LEVEL;
		NewAvailableOrders[2].RefCount = 1;
		NewAvailableOrders[3].OrderIndex = 5;//DSO_HALTTYPE;
		NewAvailableOrders[3].RefCount = 1;

		NumNewOrders = 4;

		if (NumNewOrders != NumAvailableOrders)
		{
			return false;
		}
	}
	else
	{
		for(j = 0; j < NumSelectedDroids; j++)
		{

			for(OrdIndex = 0; OrdIndex < NUM_ORDERS; OrdIndex++)
			{

				// Is the order available?
				if(secondarySupported(SelectedDroids[j], OrderButtons[OrdIndex].Order))
				{
					if(NumNewOrders < MAX_AVAILABLE_ORDERS)
					{
						// Have we already got this order?
						Found = false;
						for(i = 0; i < NumNewOrders; i++)
						{
							if(NewAvailableOrders[i].OrderIndex == OrdIndex)
							{
								// Yes! Then increment it's reference count.
								NewAvailableOrders[i].RefCount++;
								Found = true;
							}
						}

						if(!Found)
						{
							// Not already got it so add it to the list of available orders.
							NewAvailableOrders[NumNewOrders].OrderIndex = OrdIndex;
							NewAvailableOrders[NumNewOrders].RefCount = 1;
							NumNewOrders++;
						}
					}
				}
			}
		}

		if (NumNewOrders != NumAvailableOrders)
		{
			return false;
		}

		if(NumNewOrders > 1)
		{
			// Sort by Order index, A bubble sort? I know but it's only
			// a small list so what the hell.
			do
			{
				Sorted = true;
				for(i = 0; i < NumNewOrders - 1; i++)
				{
					if(NewAvailableOrders[i].OrderIndex > NewAvailableOrders[i + 1].OrderIndex)
					{
						Tmp = NewAvailableOrders[i];
						NewAvailableOrders[i] = NewAvailableOrders[i + 1];
						NewAvailableOrders[i + 1] = Tmp;
						Sorted = false;
					}
				}
			}
			while(!Sorted);
		}
	}

	// now check that all the orders are the same
	for (i = 0; i < NumNewOrders; i++)
	{
		if (NewAvailableOrders[i].OrderIndex != AvailableOrders[i].OrderIndex)
		{
			return false;
		}
	}

	return true;
}

static BOOL intRefreshOrderButtons(void)
{
	SECONDARY_STATE State;
	uint16_t i, j; //,k;
	uint16_t OrdIndex;
	uint16_t NumButs;
	uint32_t	id;

	for(j = 0; ((j < NumAvailableOrders) && (j < MAX_DISPLAYABLE_ORDERS)); j++)
	{
		OrdIndex = AvailableOrders[j].OrderIndex;

		// Get current order state.
		State = GetSecondaryStates(OrderButtons[OrdIndex].Order);

		// Get number of buttons.
		NumButs = OrderButtons[OrdIndex].NumButs;
		// Set actual number of buttons.
		OrderButtons[OrdIndex].AcNumButs = NumButs;

		// Handle special case for factory -> command droid assignment buttons.
		switch (OrderButtons[OrdIndex].Class)
		{
			case ORDBUTCLASS_FACTORY:
				NumButs = countAssignableFactories((uint8_t)selectedPlayer, FACTORY_FLAG);
				break;
			case ORDBUTCLASS_CYBORGFACTORY:
				NumButs = countAssignableFactories((uint8_t)selectedPlayer, CYBORG_FLAG);
				break;
			case ORDBUTCLASS_VTOLFACTORY:
				NumButs = countAssignableFactories((uint8_t)selectedPlayer, VTOL_FLAG);
				break;
			default:
				break;
		}

		id = OrderButtons[OrdIndex].ButBaseID;
		for(i = 0; i < OrderButtons[OrdIndex].AcNumButs; i++)
		{

			// Set the state for the button.
			switch(OrderButtons[OrdIndex].ButType)
			{
				case ORD_BTYPE_RADIO:
				case ORD_BTYPE_BOOLEAN:
					if((State & OrderButtons[OrdIndex].StateMask) == (uint32_t)OrderButtons[OrdIndex].States[i])
					{
						widgSetButtonState(psWScreen, id, WBUT_CLICKLOCK);
					}
					else
					{
						widgSetButtonState(psWScreen, id, 0);
					}
					break;

				case ORD_BTYPE_BOOLEAN_DEPEND:
					if((State & OrderButtons[OrdIndex].StateMask) == (uint32_t)OrderButtons[OrdIndex].States[i])
					{
						widgSetButtonState(psWScreen, id, WBUT_CLICKLOCK);
					}
					else
					{
						if(i == 0)
						{
							widgSetButtonState(psWScreen, id, 0);
						}
						else
						{
							widgSetButtonState(psWScreen, id, WBUT_DISABLE);
						}
					}
					break;
				case ORD_BTYPE_BOOLEAN_COMBINE:
					if( State & (uint32_t)OrderButtons[OrdIndex].States[i] )
					{
						widgSetButtonState(psWScreen, id, WBUT_CLICKLOCK);
					}
					break;
			}

			id ++;
		}
	}

	return true;
}


// Call to refresh the Order screen, ie when a droids boards it.
//
BOOL intRefreshOrder(void)
{
	// Is the Order screen up?
	if ((intMode == INT_ORDER) &&
			(widgGetFromID(psWScreen, IDORDER_FORM) != NULL))
	{
		BOOL Ret;

		NumSelectedDroids = 0;
		//check for factory selected first
		if (!psSelectedFactory)
		{
			if (!BuildSelectedDroidList())
			{
				// no units selected - quit
				intRemoveOrder();
				return true;
			}
		}

		// if the orders havn't changed, just reset the state
		//if (CheckDroidOrderList())
		if (CheckObjectOrderList())
		{
			Ret = intRefreshOrderButtons();
		}
		else
		{
			// Refresh it by re-adding it.
			Ret = intAddOrder(NULL);
			if(Ret == false)
			{
				intMode = INT_NORMAL;
			}
		}
		return Ret;
	}

	return true;
}


// Remove the droids order screen with animation.
//
void intRemoveOrder(void)
{

	W_TABFORM *Form;

	widgDelete(psWScreen, IDORDER_CLOSE);

	// Start the window close animation.
	Form = (W_TABFORM *)widgGetFromID(psWScreen, IDORDER_FORM);
	if(Form)
	{
		Form->display = intClosePlainForm;
		Form->pUserData = NULL; // Used to signal when the close anim has finished.
		Form->disableChildren = true;
		ClosingOrder = true;
		OrderUp = false;
		NumSelectedDroids = 0;
		psSelectedFactory = NULL;
	}

}


// Remove the droids order screen without animation.
//
void intRemoveOrderNoAnim(void)
{
	widgDelete(psWScreen, IDORDER_CLOSE);
	widgDelete(psWScreen, IDORDER_FORM);
	OrderUp = false;
	NumSelectedDroids = 0;
	psSelectedFactory = NULL;
}


//looks thru' the players' list of Structures to see if one is a factory and it is selected
// NOTE: Unused! BOOL factorySelected(void)
BOOL factorySelected(void);
BOOL factorySelected(void)
{
	STRUCTURE *psStruct;

	for (psStruct = apsStructLists[selectedPlayer]; psStruct; psStruct = psStruct->psNext)
	{
		if (psStruct->selected && StructIsFactory(psStruct))
		{
			//found one - set as one to use for the interface
			psSelectedFactory = psStruct;
			return true;
		}
	}
	//obviously never found a factory
	return false;
}


//new function added to bring up the RMB order form for Factories as well as droids
void intAddFactoryOrder(STRUCTURE *psStructure)
{
	if(!OrderUp)
	{
		intResetScreen(false);
		intAddOrder((BASE_OBJECT *)psStructure);
		intMode = INT_ORDER;
	}
	else
	{
		intAddOrder((BASE_OBJECT *)psStructure);
	}
}
