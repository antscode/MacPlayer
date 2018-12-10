#include <Dialogs.h>
#include <Lists.h>
#include <vector>
#include <MacWifi/MacWifiLib.h>
#include "Prefs.h"
#include "DarkListDef.h"
#include "DarkScrollbarDef.h"
#include "SpotifyClient.h"

const double mPollFrequencyMs = 5000;

enum UIState
{
	Init,
	Login,
	Player
};

enum
{
	kMenuApple = 128,
	kMenuFile,
	kMenuDevices
};

enum
{
	kItemAbout = 1,
	kItemQuit = 1
};

enum UIElements
{
	kPlayerDialog = 128,
	kPlayerMenubar = 128,
	kPlayerLoginButton = 2,
	kPlayerPlaylistsLabel = 1,
	kPlayerNavList = 2,
	kPlayerTrackLabel = 3,
	kPlayerArtistLabel = 4,
	kPlayerTrackList = 5,
	kPlayerPlayButton = 6,
	kPlayerPrevTrack = 7,
	kPlayerNextTrack = 8,
	kPlayerStartupDITL = 130,
	kPlayerLoginDITL = 128,
	kPlayerMainDITL = 129,
	kPlayerPlayIcon = 128,
	kPlayerPauseIcon = 136,
	kAboutDialog = 133
};

struct Device
{
	string name;
	string id;
	bool active;
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
	string albumId;
};

struct PlayerState
{
	bool isPlaying;
};

bool _run = true;
UIState _uiState = Login;
MacWifiLib _wifiLib;
Prefs _prefs;
SpotifyClient _spotifyClient(&_wifiLib, &_prefs);
UnsignedWide _lastPollTime;
PlayerState _playerState;
bool _viewNowPlaying = false;


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
void ShowAboutBox();
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
void SetTrackImage(JsonValue& track, Track& trackObj);
void PollPlayerState(); 
void DrawTrackImage();
void TogglePlayButtonIcon();
void ExitApp();
void ActivateDevice(int index);
void GetDevices();
void NoDevicesError();
void DrawTrackName();

vector<Device> _devices;
vector<Track> _tracks;
vector<Playlist> _playlists;
Track _currentTrack;
string _currentContext;
bool _activeDevice;

ListHandle _navList = 0, _trackList = 0;
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