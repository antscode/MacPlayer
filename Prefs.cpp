#include <string.h>
#include <Folders.h>
#include <Dialogs.h>
#include <json/json.h>
#include "Prefs.h"

Prefs::Prefs()
{
	long theSize = 0, start = 0;
	PrefsData data;
	FSSpec theSpec;
	short theFile;
	OSErr err;

	if (GetPrefsSpec(&theSpec))
	{
		if (FSpOpenDF(&theSpec, fsRdPerm, &theFile) == noErr)
		{
			theSize = sizeof(PrefsData);
			err = FSRead(theFile, &theSize, &data);

			if (err == noErr || err == eofErr)
			{
				Data = data;
			}

			FSClose(theFile);
		}
	}
}

bool Prefs::Save()
{
	long size = sizeof(PrefsData);
	FSSpec fsSpec;
	short file;
	OSErr err;

	if (GetPrefsSpec(&fsSpec)) {
		err = FSpOpenDF(&fsSpec, fsRdWrPerm, &file);
		if (err != noErr) {
			if ((err = FSpCreate(&fsSpec, 'MPLY', 'pref', 0)) == noErr)
				err = FSpOpenDF(&fsSpec, fsRdWrPerm, &file);
		}
		if (err == noErr) {
			if (SetEOF(file, 0L) == noErr) {
				if (FSWrite(file, &size, &Data) == noErr) {
					FSClose(file);
					return true;
				}

				FSClose(file);
			}
		}
	}

	return false;
}

bool Prefs::GetPrefsSpec(FSSpec *theSpec)
{
	Str255 prefsName = "\pMacPlayer Preferences";
	short vRefNum;
	long dirID;

	if (FindFolder(kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &vRefNum, &dirID) == noErr) {
		FSMakeFSSpec(vRefNum, dirID, prefsName, theSpec);
		return true;
	}

	return false;
}