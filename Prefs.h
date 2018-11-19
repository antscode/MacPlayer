#include <json/json.h>

#ifndef _PREFS_
#define _PREFS_

class Prefs
{
public:
	Prefs();
	Json::Value Data;
	bool Save();
private:
	bool GetPrefsSpec(FSSpec *theSpec);
};

#endif // _PREFS_ 