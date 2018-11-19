#include <string> 
#include <Dialogs.h>

#ifndef _UTIL_
#define _UTIL_

using namespace std;

class Util
{
	public:
		static char* PtoCStr(unsigned char *s);
		static std::string PtoStr(unsigned char *s);
		static unsigned char* CtoPStr(char *s);
		static unsigned char* StrToPStr(string str);
		static void Debug(string msg);
		static void FrameDefaultButton(DialogPtr dialog, short itemNo, bool active);
		static bool IsControlHilited(DialogPtr dialog, short itemNo);
		static string UrlEncode(string &value);
		static double MicrosecondToDouble(register const UnsignedWide *epochPtr);
};

#endif // _UTIL_
