#include <string.h>
#include <Folders.h>
#include <Dialogs.h>
#include <json/json.h>
#include "Prefs.h"

Prefs::Prefs()
{
	long theSize = 0, start = 0;
	FSSpec theSpec;
	short theFile;
	OSErr err;

	char buf[8192];
	Json::Value root;

	if (GetPrefsSpec(&theSpec))
	{
		if (FSpOpenDF(&theSpec, fsRdPerm, &theFile) == noErr)
		{
			theSize = sizeof(buf);
			err = FSRead(theFile, &theSize, &buf);

			if (err == noErr || err == eofErr)
			{
				Json::Reader reader;
				bool parsingSuccessful = reader.parse(buf, root);
			}

			FSClose(theFile);
		}
	}

	Data = root;
}

bool Prefs::Save()
{
	FSSpec fsSpec;
	short file;
	OSErr err;

	Json::StreamWriterBuilder wbuilder;
	std::string outputConfig = Json::writeString(wbuilder, Data);
	const char* prefs = outputConfig.c_str();
	long size = strlen(prefs);

	if (GetPrefsSpec(&fsSpec)) {
		err = FSpOpenDF(&fsSpec, fsRdWrPerm, &file);
		if (err != noErr) {
			if ((err = FSpCreate(&fsSpec, 'MPLY', 'pref', 0)) == noErr)
				err = FSpOpenDF(&fsSpec, fsRdWrPerm, &file);
		}
		if (err == noErr) {
			if (SetEOF(file, 0L) == noErr) {
				if (FSWrite(file, &size, prefs) == noErr) {
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
	Str255 prefsName = "\pMacinspot Preferences";
	short vRefNum;
	long dirID;

	if (FindFolder(kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &vRefNum, &dirID) == noErr) {
		FSMakeFSSpec(vRefNum, dirID, prefsName, theSpec);
		return true;
	}

	return false;
}