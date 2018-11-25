#ifndef _PREFS_
#define _PREFS_

struct PrefsData
{
	char AccessToken[255];
	char RefreshToken[255];
};

class Prefs
{
	public:
		Prefs();
		PrefsData Data;
		bool Save();
	private:
		bool GetPrefsSpec(FSSpec *theSpec);
};

#endif // _PREFS_ 