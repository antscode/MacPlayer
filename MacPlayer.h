#include <Dialogs.h>
#include <Lists.h>
#include <MacWifi/MacWifiLib.h>
#include <MacAuth/MacAuth.h>
#include "Prefs.h"
#include "DarkListDef.h"
#include "DarkScrollbarDef.h"

const short mAppleMenu = 128;

enum UIState
{
	Login,
	Player
};


bool _run = true;
UIState _uiState = Login;
MacWifiLib _wifiLib;
MacAuth _macAuth(&_wifiLib);
Prefs _prefs;
string _accessToken, _refreshToken;

int main();
void InitToolBox();
void ShowMainWindow();
void MenuInit();
void EventInit();
void EventLoop();
void InitPlayer(DialogPtr dialog);
void DoEvent(EventRecord *eventPtr);
void HandleMouseDown(EventRecord *eventPtr);
void HandleInContent(EventRecord *eventPtr);
void HandleLoginContent(short item);
void HandleUpdate(EventRecord *eventPtr);
void HandleActivate(EventRecord *eventPtr);
void HandleOSEvt(EventRecord *eventPtr);
void ModeLogin(DialogPtr dialog);
void ModePlayer(DialogPtr dialog);
void HandleMenuChoice(long menuChoice);
void HandleAppleChoice(short item);
void WaitCursor();
void HandlePlayerContent(short item);
void GetRecentTracks();
void SpotifyRequest(string uri, function<void(MacWifiResponse)> onComplete);
void SpotifyRequest(string uri, function<void(MacWifiResponse)> onComplete, bool refreshed);
void RefreshAccessToken(string uri, function<void(MacWifiResponse)> onComplete);
void InitCustomLDEF();

ListHandle _navList, _trackList;
ListHandle CreateList(
	WindowPtr windowPtr,
	Rect rect,
	int columnsInList,
	int ldef,
	short cellWidth,
	short cellHeight);
void PopulateNavList(ListHandle list);

void DoLogin();

pascal OSErr Quit(AppleEvent* appleEvent, AppleEvent* reply, long refCon);
pascal OSErr ProcessResponseEvent(AppleEvent* appleEvent, AppleEvent* reply, long refCon);