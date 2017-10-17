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

#ifndef __INCLUDED_SRC_INTDISPLAY_H__
#define __INCLUDED_SRC_INTDISPLAY_H__

#include "lib/widget/widget.h"
#include "lib/widget/form.h"
#include "intimage.h"
#include "droid.h"

#define NUM_OBJECTSURFACES		(100)
#define NUM_TOPICSURFACES		(50)
#define NUM_STATSURFACES		(200)
#define NUM_SYSTEM0SURFACES		(100)
#define NUM_OBJECTBUFFERS		(NUM_OBJECTSURFACES*4)
#define NUM_STATBUFFERS			(NUM_STATSURFACES*4)
#define NUM_TOPICBUFFERS		(NUM_TOPICSURFACES*4)

#define NUM_SYSTEM0BUFFERS		(NUM_SYSTEM0SURFACES*8)


/* Power levels are divided by this for power bar display. The extra factor has
been included so that the levels appear the same for the power bar as for the
power values in the buttons */
#define POWERBAR_SCALE			(5 * WBAR_SCALE/STAT_PROGBARWIDTH)

#define BUTTONOBJ_ROTSPEED		90	// Speed to rotate objects rendered in
// buttons ( degrees per second )

//the two types of button used in the object display (bottom bar)
#define		TOPBUTTON			0
#define		BTMBUTTON			1


enum
{
	IMDTYPE_NONE,
	IMDTYPE_DROID,
	IMDTYPE_DROIDTEMPLATE,
	IMDTYPE_COMPONENT,
	IMDTYPE_STRUCTURE,
	IMDTYPE_RESEARCH,
	IMDTYPE_STRUCTURESTAT,
};

typedef struct
{
	char *Token;
	int16_t ID;
} TOKENID;

typedef struct
{
	char *Token;
	int16_t ID;
	int16_t IMD;
} RESEARCHICON;


typedef struct
{
	uint8_t *Buffer;		// Bitmap buffer.
	iSurface *Surface;	// Ivis surface definition.
} BUTTON_SURFACE;


#define RENDERBUTTON_INUSE(x)  ((x)->InUse=true)
#define RENDERBUTTON_NOTINUSE(x)  ((x)->InUse=false)

#define RENDERBUTTON_INITIALISED(x)  ((x)->Initialised=true)
#define RENDERBUTTON_NOTINITIALISED(x)  ((x)->Initialised=false)

#define IsBufferInitialised(x) ((x)->Initialised)
#define IsBufferInUse(x) ((x)->InUse)

typedef struct
{
	BOOL InUse;			// Is it in use.
	BOOL Initialised;	// Is it initialised.
	int32_t ImdRotation;	// Rotation if button is an IMD.
	uint32_t State;		// Copy of widget's state so we know if state has changed.
	void *Data;			// Any data we want to attach.
	void *Data2;		// Any data we want to attach.
	BUTTON_SURFACE *ButSurf;	// Surface to render the button into.
//	uint8 *Buffer;		// Bitmap buffer.
//	iSurface *Surface;	// Ivis surface definition.
} RENDERED_BUTTON;

extern RENDERED_BUTTON TopicBuffers[NUM_TOPICBUFFERS];
extern RENDERED_BUTTON ObjectBuffers[NUM_OBJECTBUFFERS];
extern RENDERED_BUTTON StatBuffers[NUM_STATBUFFERS];
extern RENDERED_BUTTON System0Buffers[NUM_SYSTEM0BUFFERS];

extern uint32_t ManuPower;		// Power required to manufacture the current item.
extern BASE_STATS *CurrentStatsTemplate;

// Set audio IDs for form opening/closing anims.
void SetFormAudioIDs(int OpenID, int CloseID);

// Initialise interface graphics.
void intInitialiseGraphics(void);

// Free up interface graphics.
void interfaceDeleteGraphics(void);

// Intialise button surfaces.
void InitialiseButtonData(void);

// Get a free RENDERED_BUTTON structure for an object window button.
int32_t GetObjectBuffer(void);

// Clear ( make unused ) all RENDERED_BUTTON structures for the object window.
void ClearObjectBuffers(void);

// Clear ( make unused ) all RENDERED_BUTTON structures for the topic window.
void ClearTopicBuffers(void);

// Clear ( make unused ) a RENDERED_BUTTON structure.
void ClearObjectButtonBuffer(int32_t BufferID);

// Clear ( make unused ) a RENDERED_BUTTON structure.
void ClearTopicButtonBuffer(int32_t BufferID);

void RefreshObjectButtons(void);
void RefreshSystem0Buttons(void);
void RefreshTopicButtons(void);
void RefreshStatsButtons(void);


// Get a free RENDERED_BUTTON structure for a stat window button.
int32_t GetStatBuffer(void);

// Clear ( make unused ) all RENDERED_BUTTON structures for the stat window.
void ClearStatBuffers(void);

/*these have been set up for the Transporter - the design screen DOESN'T use them*/
// Clear ( make unused ) *all* RENDERED_BUTTON structures.
void ClearSystem0Buffers(void);
// Clear ( make unused ) a RENDERED_BUTTON structure.
void ClearSystem0ButtonBuffer(int32_t BufferID);
// Get a free RENDERED_BUTTON structure.
int32_t GetSystem0Buffer(void);

// callback to update the command droid size label
void intUpdateCommandSize(WIDGET *psWidget, W_CONTEXT *psContext);

// callback to update the command droid experience
void intUpdateCommandExp(WIDGET *psWidget, W_CONTEXT *psContext);

// callback to update the command droid factories
void intUpdateCommandFact(WIDGET *psWidget, W_CONTEXT *psContext);

void intUpdateProgressBar(WIDGET *psWidget, W_CONTEXT *psContext);

void intUpdateOptionText(WIDGET *psWidget, W_CONTEXT *psContext);

void intUpdateQuantity(WIDGET *psWidget, W_CONTEXT *psContext);
//callback to display the factory number
extern void intAddFactoryInc(WIDGET *psWidget, W_CONTEXT *psContext);
//callback to display the production quantity number for a template
extern void intAddProdQuantity(WIDGET *psWidget, W_CONTEXT *psContext);

void intDisplayPowerBar(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayStatusButton(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayObjectButton(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayStatsButton(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void AdjustTabFormSize(W_TABFORM *Form, uint32_t *x0, uint32_t *y0, uint32_t *x1, uint32_t *y1);

void intDisplayStatsForm(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intOpenPlainForm(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intClosePlainForm(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayPlainForm(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayImage(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayImageHilight(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayButtonHilight(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayButtonFlash(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayButtonPressed(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayReticuleButton(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayTab(WIDGET *psWidget, uint32_t TabType, uint32_t Position,
				   uint32_t Number, BOOL Selected, BOOL Hilight, uint32_t x, uint32_t y, uint32_t Width, uint32_t Height);
void intDisplaySlider(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void intDisplayNumber(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);
void intAddLoopQuantity(WIDGET *psWidget, W_CONTEXT *psContext);

void intDisplayEditBox(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

void OpenButtonRender(uint16_t XPos, uint16_t YPos, uint16_t Width, uint16_t Height);
void CloseButtonRender(void);

void ClearButton(BOOL Down, uint32_t Size, uint32_t buttonType);

void RenderToButton(IMAGEFILE *ImageFile, uint16_t ImageID, void *Object, uint32_t Player, RENDERED_BUTTON *Buffer,
					BOOL Down, uint32_t IMDType, uint32_t buttonType);

void CreateIMDButton(IMAGEFILE *ImageFile, uint16_t ImageID, void *Object, uint32_t Player, RENDERED_BUTTON *Buffer,
					 BOOL Down, uint32_t IMDType, uint32_t buttonType);

void CreateImageButton(IMAGEFILE *ImageFile, uint16_t ImageID, RENDERED_BUTTON *Buffer, BOOL Down, uint32_t buttonType);

void CreateBlankButton(RENDERED_BUTTON *Buffer, BOOL Down, uint32_t buttonType);

void RenderImageToButton(IMAGEFILE *ImageFile, uint16_t ImageID, RENDERED_BUTTON *Buffer, BOOL Down, uint32_t buttonType);
void RenderBlankToButton(RENDERED_BUTTON *Buffer, BOOL Down, uint32_t buttonType);


extern BOOL DroidIsRepairing(DROID *Droid);

BOOL DroidIsBuilding(DROID *Droid);
STRUCTURE *DroidGetBuildStructure(DROID *Droid);
BOOL DroidGoingToBuild(DROID *Droid);
BASE_STATS *DroidGetBuildStats(DROID *Droid);
iIMDShape *DroidGetIMD(DROID *Droid);
uint32_t DroidGetIMDIndex(DROID *Droid);
BOOL DroidIsDemolishing(DROID *Droid);

BOOL StructureIsManufacturing(STRUCTURE *Structure);
RESEARCH_FACILITY *StructureGetResearch(STRUCTURE *Structure);
BOOL StructureIsResearching(STRUCTURE *Structure);
FACTORY *StructureGetFactory(STRUCTURE *Structure);
iIMDShape *StructureGetIMD(STRUCTURE *Structure);

DROID_TEMPLATE *FactoryGetTemplate(FACTORY *Factory);

//iIMDShape *TemplateGetIMD(DROID_TEMPLATE *DroidTemp,uint32_t Player);
//uint32_t TemplateGetIMDIndex(DROID_TEMPLATE *Template,uint32_t Player);

//int32_t ResearchGetImage(RESEARCH_FACILITY *Research);

BOOL StatIsStructure(BASE_STATS *Stat);
iIMDShape *StatGetStructureIMD(BASE_STATS *Stat, uint32_t Player);
BOOL StatIsTemplate(BASE_STATS *Stat);
BOOL StatIsFeature(BASE_STATS *Stat);

int32_t StatIsComponent(BASE_STATS *Stat);
BOOL StatGetComponentIMD(BASE_STATS *Stat, int32_t compID, iIMDShape **CompIMD, iIMDShape **MountIMD);

BOOL StatIsResearch(BASE_STATS *Stat);
//void StatGetResearchImage(BASE_STATS *Stat,int32_t *Image,iIMDShape **Shape, BOOL drawTechIcon);
void StatGetResearchImage(BASE_STATS *psStat, int32_t *Image, iIMDShape **Shape,
						  BASE_STATS **ppGraphicData, BOOL drawTechIcon);

/* Draws a stats bar for the design screen */
extern void intDisplayStatsBar(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset,
							   PIELIGHT *pColours);
/* Draws a Template Power Bar for the Design Screen */
void intDisplayDesignPowerBar(WIDGET *psWidget, uint32_t xOffset,
							  uint32_t yOffset, PIELIGHT *pColours);

// Widget callback function to play an audio track.
extern void WidgetAudioCallback(int AudioID);

// Widget callback to display a contents button for the Transporter
extern void intDisplayTransportButton(WIDGET *psWidget, uint32_t xOffset,
									  uint32_t yOffset, PIELIGHT *pColours);
/*draws blips on radar to represent Proximity Display*/
extern void drawRadarBlips(int radarX, int radarY, float pixSizeH, float pixSizeV);

/*Displays the proximity messages blips over the world*/
extern void intDisplayProximityBlips(WIDGET *psWidget, uint32_t xOffset,
									 uint32_t yOffset, PIELIGHT *pColours);

extern void intUpdateQuantitySlider(WIDGET *psWidget, W_CONTEXT *psContext);



extern void intDisplayDPButton(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

extern void intDisplayTime(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);
extern void intDisplayNum(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

extern void intDisplayResSubGroup(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

extern void intDisplayMissionClock(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

extern void intDisplayAllyIcon(WIDGET *psWidget, uint32_t xOffset, uint32_t yOffset, PIELIGHT *pColours);

#endif // __INCLUDED_SRC_INTDISPLAY_H__
