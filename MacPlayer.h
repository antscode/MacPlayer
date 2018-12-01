#include <Dialogs.h>
#include <Lists.h>
#include <vector>
#include <MacWifi/MacWifiLib.h>
#include "Prefs.h"
#include "DarkListDef.h"
#include "DarkScrollbarDef.h"
#include "SpotifyClient.h"

const short mAppleMenu = 128;

enum UIState
{
	Init,
	Login,
	Player
};

struct Playlist
{
	string name;
	string id;
	string uri;
};


struct Track
{
	string name;
	string artist;
	string uri;
	string image;
};


bool _run = true;
UIState _uiState = Login;
MacWifiLib _wifiLib;
Prefs _prefs;
SpotifyClient _spotifyClient(&_wifiLib, &_prefs);

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
void ModeInit(DialogPtr dialog);
void ModeLogin(DialogPtr dialog);
void ModePlayer(DialogPtr dialog);
void HandleMenuChoice(long menuChoice);
void HandleAppleChoice(short item);
void WaitCursor();
void HandlePlayerContent(short item);
void GetRecentTracks();
void GetPlaylists(DialogPtr dialog);
void GetPlaylistTracks(const string& uri, const string& playlistId);
void InitCustomLDEF();
void PlayTrack();
void ViewNowPlaying();
void UpdateCurrentTrack();
void PopulateTrackList(JsonValue& root);
Track GetTrackObject(JsonValue& track);

vector<Track> _tracks;
vector<Playlist> _playlists;
Track _currentTrack;
string _currentContext;

ListHandle _navList, _trackList;
ListHandle CreateList(
	WindowPtr windowPtr,
	Rect rect,
	int columnsInList,
	int ldef,
	short cellWidth,
	short cellHeight);
void PopulateNavList(ListHandle list);

pascal OSErr Quit(AppleEvent* appleEvent, AppleEvent* reply, long refCon);
pascal OSErr ProcessResponseEvent(AppleEvent* appleEvent, AppleEvent* reply, long refCon);