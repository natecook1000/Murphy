// Murphy's Revenge// (c)1997 Q Software / Dave Peck, All Rights Reserved// This source code is confidential and proprietary, and may not be reproduced// or utilized by anyone without the express written permission of Q Software.// File: MRChore.h// Auth: Dave Peck// Date: 7/3/98// Desc: Header file for game-time background chore code// -----------------------------------------------------------------------------// Includes// -----------------------------------------------------------------------------#include "MRPlayCommon.h"// -----------------------------------------------------------------------------// Constants// -----------------------------------------------------------------------------// -----------------------------------------------------------------------------// Data Structures// -----------------------------------------------------------------------------// info for a laser chore:typedef struct LaserChoreInfo {	short		mode;				// current laser state		short		deltaV, deltaH;		// which way to go next	short		oldTile;};// info for a bug chore:typedef struct BugChoreInfo {	Boolean		isZapping;	Boolean		isWaiting;};typedef struct BugChoreInfo BugChoreInfo;// info about an explosion chore.typedef struct ExplChoreInfo {	Boolean		shouldIgnite;	short		explTileID;	short		igniteTileID;} ;typedef struct ExplChoreInfo ExplChoreInfo;// info about an exit chore:typedef struct ExitChoreInfo {	short		exitID;} ;typedef struct ExitChoreInfo ExitChoreInfo;// info for a zap choretypedef struct ZapChoreInfo {	short		origTile;	short		curTile;	short		lastTile;} ;typedef struct ZapChoreInfo ZapChoreInfo;typedef struct RDBlinkChoreInfo {	short		blinkCount;	short		curTile;} ;typedef struct RDBlinkChoreInfo RDBlinkChoreInfo;// a typical chore to be performedtypedef struct MRChoreStruct {	short				choreType;	LevelCBInfo			cb;		Point				loc;	long				lastTime;		Ptr					processProc;	Ptr					deallocProc;		// info specific to one kind of chore	union {		BugChoreInfo	 bug;		ExplChoreInfo	 expl;		ExitChoreInfo	 exit;		ZapChoreInfo	 zap;		RDBlinkChoreInfo rdBlink;		LaserChoreInfo	 laser;	} info;};typedef struct MRChoreStruct MRChoreStruct;typedef MRChoreStruct *MRChore;// a function type for processing a choretypedef Boolean (*MRChoreProcessPtr)( MRChore chore );typedef void (*MRChoreDeallocPtr) ( MRChore chore );// -----------------------------------------------------------------------------// Global Data// -----------------------------------------------------------------------------// -----------------------------------------------------------------------------// Function Prototypes// -----------------------------------------------------------------------------void AllocChore_Explosion( LevelCBInfo cb,						   Point loc,						   short explTileID,						   Boolean shouldIgnite,						   short igniteTileID );						   void AllocChore_QuarkExplosion( LevelCBInfo cb,						   		Point loc,						   		short explTileID,						   		Boolean shouldIgnite,						   		short igniteTileID );						   		void AllocChore_Exit( LevelCBInfo cb,					  short v, short h,					  short exitTileID );					  void AllocChore_Laser( LevelCBInfo cb,					   short v, short h,					   short dv, short dh );					   void AllocChore_Bug( LevelCBInfo cb,					 short h, short v );					 void AllocChore_Zap( LevelCBInfo cb,					 short h, short v,					 short origTile );void AllocChore_RedDiskBlink( LevelCBInfo cb,							  Point loc,							  short curTile );void DeallocChore( MRChore *theChore );					  							  // -----------------------------------------------------------------------------// Functions// -----------------------------------------------------------------------------