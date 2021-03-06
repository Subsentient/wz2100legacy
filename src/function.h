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
 *  Definitions for the Structure Functions.
 */

#ifndef __INCLUDED_SRC_FUNCTION_H__
#define __INCLUDED_SRC_FUNCTION_H__

#include "objectdef.h"


//holder for all functions
extern FUNCTION **asFunctions;
extern uint32_t numFunctions;

extern BOOL loadFunctionStats(const char *pFunctionData, uint32_t bufferSize);

extern void productionUpgrade(FUNCTION *pFunction, uint8_t player);
extern void researchUpgrade(FUNCTION *pFunction, uint8_t player);
extern void powerUpgrade(FUNCTION *pFunction, uint8_t player);
extern void reArmUpgrade(FUNCTION *pFunction, uint8_t player);
extern void repairFacUpgrade(FUNCTION *pFunction, uint8_t player);
extern void weaponUpgrade(FUNCTION *pFunction, uint8_t player);
extern void structureUpgrade(FUNCTION *pFunction, uint8_t player);
extern void wallDefenceUpgrade(FUNCTION *pFunction, uint8_t player);
extern void structureBodyUpgrade(FUNCTION *pFunction, STRUCTURE *psBuilding);
extern void structureArmourUpgrade(FUNCTION *pFunction, STRUCTURE *psBuilding);
extern void structureResistanceUpgrade(FUNCTION *pFunction, STRUCTURE *psBuilding);
extern void structureProductionUpgrade(STRUCTURE *psBuilding);
extern void structureResearchUpgrade(STRUCTURE *psBuilding);
extern void structurePowerUpgrade(STRUCTURE *psBuilding);
extern void structureRepairUpgrade(STRUCTURE *psBuilding);
extern void structureSensorUpgrade(STRUCTURE *psBuilding);
extern void structureReArmUpgrade(STRUCTURE *psBuilding);
extern void structureECMUpgrade(STRUCTURE *psBuilding);
extern void sensorUpgrade(FUNCTION *pFunction, uint8_t player);
extern void repairUpgrade(FUNCTION *pFunction, uint8_t player);
extern void ecmUpgrade(FUNCTION *pFunction, uint8_t player);
extern void constructorUpgrade(FUNCTION *pFunction, uint8_t player);
extern void bodyUpgrade(FUNCTION *pFunction, uint8_t player);
extern void droidSensorUpgrade(DROID *psDroid);
extern void droidECMUpgrade(DROID *psDroid);
extern void droidBodyUpgrade(FUNCTION *pFunction, DROID *psDroid);
extern void upgradeTransporterDroids(DROID *psTransporter,
									 void(*pUpgradeFunction)(DROID *psDroid));

extern BOOL FunctionShutDown(void);

#endif // __INCLUDED_SRC_FUNCTION_H__
