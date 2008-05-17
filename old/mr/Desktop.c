// =============================================================================================// Mars Rising// David Wareing 1997//// Desktop.c - saves the position of desktop icons and restores them when screen size changes.// =============================================================================================// Modified by David Wareing. Based on original code by Quinn./*	File:				SnapshotSample.c	Contains:		Code for saving and restoring desktop icons.	Written by:	Quinn "The Eskimo!"	Copyright:	� 1996 by Apple Computer, Inc., all rights reserved.	Change History (most recent first):	You may incorporate this sample code into your applications without	restriction, though the sample code has been provided "AS IS" and the	responsibility for its operation is 100% yours.  However, what you are	not permitted to do is to redistribute the source as "DSC Sample Code"	after having made changes. If you're going to re-distribute the source,	we require that you make it clear in the source that the code was	descended from Apple Sample Code, but that you've made changes.*//*	This sample, when run the first time, creates a 	snapshot file in the Preferences folder which contains a list 	of the names and locations fo the items on the 	desktop.		Running the sample again will read the snapshot	file and tell the Finder to position the items	at the locations read from the snapshot file.		Sample does not read/set location of Trash,	nor of mounted volumes.		Sample is provided as is... I didn't get much 	chance to test beyond the standard, "oh, this	works!" stage.		The original Pascal version was written by	Quinn "The Eskimo!" and released as a Freeware	utility.  This DTS sample was created by	Deborah Grits.  This version was tidied up	and released by Quinn "The Eskimo!".		Oh what a tangled web we weave...*/// Includes.#include <AppleEvents.h>#include <AEPackObject.h>#include <Errors.h>#include <Events.h>#include <Fonts.h>#include <GestaltEqu.h>#include <Memory.h>#include <Menus.h>#include <OSUtils.h>#include <QuickDraw.h>#include <Resources.h>#include <Script.h>#include <ToolUtils.h>#include <Windows.h>#include "Desktop.h"//#include "MRError.h"					// Application specific error handling.#ifndef I_HATE_CFM68K// The snapshot file is made up of records of SnapRecord type.// An empty record (ie with name == "") is used to indicate// the end of the file.typedef struct SnapRecord{	Str63 	name;	Point 	loc;};typedef struct SnapRecord SnapRecord;typedef SnapRecord *SnapRecordPtr;typedef SnapRecordPtr *SnapRecordHandle;// Private constants.const static AEDesc gNullDesc = {typeNull, nil};	// An empty AppleEvent Descriptor.// Private function prototypes.static OSErr RestoreSnapshot(FSSpec snapFss);static OSErr WriteFileRecord(SInt16 snapFileRef, Str63 itemName, Point itemPosition);static OSErr CreateSnapshotFile(FSSpec snapFss);// =============================================================================================// Public functions// =============================================================================================#pragma mark === Public Functions ===// Records the positions of all desktop file icons in a file in the Preferences folder.// This should be called just once just before we change the screen resolution.void Desktop_RecordIcons(void){		FSSpec		snapFss;	SInt32		gestaltResponse;	// First, a quick check to make sure we have Scriptable Finder.  If we don't we bail quickly.	if ((Gestalt(gestaltFinderAttr, &gestaltResponse) != noErr) || ((gestaltResponse & (1 << gestaltOSLCompliantFinder)) == 0)) {		return;			// Get out of here.	}		// Find the Preferences folder.	OSErr err = FindFolder(kOnSystemDisk, kPreferencesFolderType, FALSE, 			&(snapFss.vRefNum), &(snapFss.parID));	if (err == noErr) {		// Open the snapshot file in the Preferences folder.		// Should get this from a resource, of course.		(void) FSMakeFSSpec(snapFss.vRefNum, snapFss.parID, "\pDesktop Snap", &snapFss);		err = FSpCreate(&snapFss, 'FR�G', 'FR�G', 0);				// Create the snapshot file for the desktop		err = CreateSnapshotFile(snapFss);	}	/*	if (err != noErr) {		ErrorCheck(err, "Desktop_RecordIcons() - couldn't create a new snapshot file.", false);	}	*/}// Get the positions of the desktop icons from a file in the Preferences folder// and then reset all the icon positions. This should be called just once as the// application is exiting, after the screen resolution has been switched back to// its initial setting.void Desktop_RestoreIcons(void){	FSSpec		snapFss;	SInt32		gestaltResponse;	// First, a quick check to make sure we have Scriptable Finder.  If we don't we bail quickly.	if ((Gestalt(gestaltFinderAttr, &gestaltResponse) != noErr) || ((gestaltResponse & (1 << gestaltOSLCompliantFinder)) == 0)) {		return;				// Get outa here.	}		// Find the Preferences folder.	OSErr err = FindFolder(kOnSystemDisk, kPreferencesFolderType, FALSE, 			&(snapFss.vRefNum), &(snapFss.parID));	if (err == noErr) {		// Open the snapshot file in the Preferences folder.		// Should get this from a resource, of course.		(void) FSMakeFSSpec(snapFss.vRefNum, snapFss.parID, "\pDesktop Snap", &snapFss);		err = FSpCreate(&snapFss, 'FR�G', 'FR�G', 0);		if (err == dupFNErr) {							// If we already have a snapshot file,			err = RestoreSnapshot(snapFss);				// restore icon positions,		}	}		if (err != noErr) {		#ifdef DEBUG			DebugValues("Desktop_RestoreIcons() - couldn't restore icons.", err);		#endif	}}	// =============================================================================================// Private functions// =============================================================================================#pragma mark -#pragma mark === Private Functions ===static OSErr FindProcessByTypeAndCreator(OSType typeToFind, OSType creatorToFind, 	ProcessSerialNumber *processSN){	// Runs through the process list looking for the indicated application.		ProcessInfoRec 	infoRecToFill;			OSErr err = noErr;    processSN->lowLongOfPSN = kNoProcess;    processSN->highLongOfPSN = kNoProcess;	infoRecToFill.processInfoLength = sizeof(ProcessInfoRec);	infoRecToFill.processName = nil;	infoRecToFill.processAppSpec = nil;	      do {        err = GetNextProcess(processSN);        if (err == noErr) {            GetProcessInformation(processSN, &infoRecToFill);        }    } while ((infoRecToFill.processSignature != creatorToFind || infoRecToFill.processType != typeToFind) ||		err != noErr);        return (err);}// Restoring Snapshots// Sends an AppleEvent to the targetDesc (which should target the Finder)//  to set the position of the item whose name is fname to the position dest.static OSErr SetItemPosition(Str255 fname, Point dest, AEDesc *setDataEvent){	AEDesc		fileNameDesc = gNullDesc;	AEDesc		rootDesc = gNullDesc;	AEDesc		propertyDescriptor = gNullDesc;	AEDesc		objParentDesc = gNullDesc;	AEDesc		objDesc = gNullDesc;	AEDesc		reply = gNullDesc;	AEDesc		newData = gNullDesc;	DescType	procDescData = 'posn';	// Create fileNameDesc to hold the file name, ief "xxx"	OSErr err = AECreateDesc(typeChar, &fname[1], *fname, &fileNameDesc);		// Create objParentDesc to specify the parent object, ie item "xxxx" of application	if (err == noErr) {		err = CreateObjSpecifier(cItem, &rootDesc, formName, &fileNameDesc, FALSE, &objParentDesc);	}	// Create propertyDescriptor to hold the property specifier, ie position	if (err == noErr) {		err = AECreateDesc(typeType, &procDescData, sizeof(procDescData), &propertyDescriptor);	}		// Create objDesc to reference the data to set, ie position of item "xxx" of application	if (err == noErr) {		err = CreateObjSpecifier(cQDPoint, &objParentDesc, formPropertyID, &propertyDescriptor, FALSE, &objDesc);	}	// Create on descriptor to hold the new data, ie the new icon position.	if (err == noErr) {		err = AECreateDesc(typeQDPoint, (void*)&dest, sizeof(dest), &newData);	}	// Fill out the parameters, putting the objDesc into the direct object and the new data	//  into the data parameter.	if (err == noErr) {		err = AEPutParamDesc(setDataEvent, keyDirectObject, &objDesc);	}	if (err == noErr) {		err = AEPutParamDesc(setDataEvent, keyAEData, &newData);	}	// Send the event.	if (err == noErr) {		err = AESend(setDataEvent, &reply, kAENoReply, kAENormalPriority, kAEDefaultTimeout, nil, nil);	}	// Clean up all those messy descriptors.  Don't you just love AppleEvents (-:	AEDisposeDesc(&rootDesc);	AEDisposeDesc(&fileNameDesc);	AEDisposeDesc(&objParentDesc);	AEDisposeDesc(&propertyDescriptor);	AEDisposeDesc(&objDesc);	AEDisposeDesc(&newData);		return (err);}// Restore the snapshot file specified by snapFss.static OSErr RestoreSnapshot(FSSpec snapFss){		ProcessSerialNumber	finderPSN;	AEDesc				targetDesc = gNullDesc;	AEDesc				setDataEvent = gNullDesc;	SInt16				snapFileRef;	Boolean 			done;	SnapRecord			aSnapRecord;	SInt32				byteCount;	snapFileRef = 0;		// Find the Finder's ProcessSerialNumber and create the targetDesc for it.	OSErr err = FindProcessByTypeAndCreator('FNDR', 'MACS', &finderPSN);	if (err == noErr) {		err = AECreateDesc(typeProcessSerialNumber, (Ptr)&finderPSN, 			sizeof(finderPSN), &targetDesc);	}		// Create the AppleEvent.	if (err == noErr) {		err = AECreateAppleEvent(kAECoreSuite, kAESetData, &targetDesc,			kAutoGenerateReturnID, kAnyTransactionID, &setDataEvent);	}			// Open the file.	if (err == noErr) {		err = FSpOpenDF(&snapFss, fsRdPerm, &snapFileRef);	}		if (err == noErr) {				// Read the records in the file, repositioning the icons as we go.		done = FALSE;		do {			byteCount = sizeof(SnapRecord);			err = FSRead(snapFileRef, &byteCount, (void*) &aSnapRecord);						if (err == noErr) {				// We're done if we hit the empty string.				done = (aSnapRecord.name[0] == 0);								if (!done) {					// Reposition the icon.					err = SetItemPosition(aSnapRecord.name, aSnapRecord.loc, &setDataEvent);				}			}		} while (!done);	}		if (err == eofErr) {		err = noErr;	}	// clean up	if (snapFileRef != 0) {		(void) FSClose(snapFileRef);	}	AEDisposeDesc(&setDataEvent);	AEDisposeDesc(&targetDesc);			return (err);}// Creating Snapshots// Add a new record to the snapshot file.static OSErr WriteFileRecord(SInt16 snapFileRef, Str63 itemName, Point itemPosition){			SnapRecord	aSnapRecord;	SInt32		byteCount;		BlockMoveData(itemName, aSnapRecord.name, sizeof(aSnapRecord.name));	aSnapRecord.loc = itemPosition;		byteCount = sizeof(SnapRecord);	OSErr err = FSWrite(snapFileRef, &byteCount, (void*) &aSnapRecord);		return (err);}// Add the positions for the icons on the desktop to the snapshot file snapFss.static OSErr CreateSnapshotFile(FSSpec snapFss){			SInt16			snapFileRef;	SInt16			deskVRefNum;	SInt32			deskDirID;	CInfoPBRec		pb;	SInt16			fileIndex;	Point			folderOrigin;	Point			itemPosition;	Str63			itemName;	snapFileRef = 0;		// Find the desktop folder.	OSErr err = FindFolder(kOnSystemDisk, kDesktopFolderType, TRUE, &deskVRefNum, &deskDirID);		// Get the origin for the desktop folder.	pb.dirInfo.ioNamePtr = nil;	pb.dirInfo.ioDrDirID = deskDirID;	pb.dirInfo.ioVRefNum = deskVRefNum;	pb.dirInfo.ioFDirIndex = -1;						// get information about ioDirID	err = PBGetCatInfoSync(&pb);	if (err == noErr) {		folderOrigin.h = 0;		folderOrigin.v = 0;		folderOrigin.v = folderOrigin.v - 20;			// evil fudge factor!	}	// Open up the snapshot file.	if (err == noErr) {		err = FSpOpenDF(&snapFss, fsRdWrPerm, &snapFileRef);	}	// Loop through each file in the folder, adding it to the snapshot file.	if (err == noErr) {		fileIndex = 1;		do {			pb.dirInfo.ioNamePtr = (StringPtr) itemName;			pb.dirInfo.ioDrDirID = deskDirID;			pb.dirInfo.ioVRefNum = deskVRefNum;			pb.dirInfo.ioFDirIndex = fileIndex;			err = PBGetCatInfoSync(&pb);			if (err == noErr) {				itemPosition = pb.hFileInfo.ioFlFndrInfo.fdLocation;				SubPt(folderOrigin, &itemPosition);				err = WriteFileRecord(snapFileRef, itemName, itemPosition);			}			fileIndex += 1;		} while (err == noErr);				// Write the sentinel to the end of the file.		if (err == fnfErr) {			itemName[0] = 0;			itemPosition.h = 0;			itemPosition.v = 0;			err = WriteFileRecord(snapFileRef, itemName, itemPosition);		}	}			// Clean up.	if (snapFileRef != 0) {		(void) FSClose(snapFileRef);	}			return (err);}#endif I_HATE_CFM68K