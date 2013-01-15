#include "lib/netplay/netplay.h"

#ifndef __INCLUDED_SRC_SPECTATE_H__
#define __INCLUDED_SRC_SPECTATE_H__

//Variables.
extern bool isSpectating;
extern bool blockDebug;
extern bool allowSpectating;

//Functions.

extern void sendSpecSignal();
extern void recvSpecSignal(NETQUEUE queue);
extern int specThread(void *);

#endif // __INCLUDED_SRC_SPECTATE_H__