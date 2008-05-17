// Murphy's Revenge// (c)1997 Q Software / Dave Peck, All Rights Reserved// This source code is confidential and proprietary, and may not be reproduced// or utilized by anyone without the express written permission of Q Software.// File: MRInterface.h// Auth: dave peck// Date: 7/2/97// Desc: wow!// ----------------------------------------------------------------// Includes// ----------------------------------------------------------------#include "MacIncludes.h"#include "MRGfxUtils.h"// ----------------------------------------------------------------// Constants// ----------------------------------------------------------------// values returned from the interface routine#define kErrorReturn		1#define kMapRoomReturn		2#define kQuitReturn			3// colors for extending the QSoft/MR banner graphics.#define kTopLeftRGBColor 	{39168, 39168, 52224}#define kTopRightRGBColor 	{0, 26112, 39168}#define kBotLeftRGBColor	{65535, 52224, 39168}#define kBotRightRGBColor	{65535, 39168, 0}// interface state constants#define kInterfaceSleepTime		0// interface states#define kInterfaceState			1#define kIntroState				2#define kCreditsState			3#define kHelpState				4#define kPrefsState				5// hiliteds#define kNoHilite				0#define kIntroHilite			1#define kCreditsHilite			2#define kHelpHilite				3#define kQuitHilite				4#define kStartHilite			5#define kPrefsHilite			6#define kMaxHilite				7		// make this previous + 1// special hilites#define kOliverSpecialHilite	-1#define kOliverPrefBox			0#define	kUpPrefBox				1#define kRightPrefBox			2#define kLeftPrefBox			3#define kDownPrefBox			4#define kSpecialPrefBox			5#define kPausePrefBox			6#define kSuicidePrefBox			7#define kMusicLArrowPrefBox		8#define kMusicRArrowPrefBox		9#define kMusicPictPrefBox		10#define kSoundLArrowPrefBox		11#define kSoundRArrowPrefBox		12#define kSoundPictPrefBox		13#define kCancelPrefBox			14#define kOkayPrefBox			15#define kInterlacePrefBox		16#define kSmallerPrefBox			17#define kCompatiblePrefBox		18#define kMaxPrefBox				19#define kPrefLoPict				1000#define kPrefHiPict				1001#define kPrefSilentPict			1002#define kPrefAudiblePict		1003#define kPrefPleasantPict		1004#define kPrefExcruciatingPict	1005#define kExcruciatingVolume		255#define kSilentVolume			0#define kAudibleVolume			85#define kPlesantVolume			171#define kPleasantVolume			kPlesantVolume// ----------------------------------------------------------------// Data Structures// ----------------------------------------------------------------typedef struct MRPrefCBDataStruct {	WindowPtr		mainWind;	GWorldPtr		off, hiliteOff;	Rect			offTrack[kMaxPrefBox]; 	Rect			onTrack[kMaxPrefBox];	Rect			offRect, onRect;	Rect			bubbleRect;	short			lastHilite;	Boolean			okayClicked;	MRBubbleData	bubbleData;};typedef struct MRPrefCBDataStruct MRPrefCBDataStruct;typedef MRPrefCBDataStruct *MRPrefCBData;		typedef struct MRIntfCBDataStruct {	// basic interface info	WindowPtr		mainWind;	int				currentState;		// pertaining to hiliting of interface parts	GWorldPtr		hiliteOff;	GWorldPtr		normalOff;	GWorldPtr		mouseOff;	int				lastHilite;	Rect			hiliteRects[kMaxHilite];	Point			adjustAmount;			// for key-presses	Boolean			lastWasKey;	int				avoidHilite;		// bubbly	MRBubbleData	bubbleData;};typedef struct MRIntfCBDataStruct MRIntfCBDataStruct;typedef MRIntfCBDataStruct *MRIntfCBData;// ----------------------------------------------------------------// Global Data// ----------------------------------------------------------------// ----------------------------------------------------------------// Function Prototypes// ----------------------------------------------------------------// ----------------------------------------------------------------// Functions// ----------------------------------------------------------------// returns either an error value,// or some constant dictating that the user entered the map room,// or quit, or yadda yadda.// (help, about, intro are all handled inside the intf package.)int RunInterface( WindowPtr wind );