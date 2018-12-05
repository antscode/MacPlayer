#include <Events.h>
#include <Resources.h>
#include <Dialogs.h>
#include <Quickdraw.h>
#include <Menus.h>
#include <ToolUtils.h>
#include <Timer.h>
#include <string.h>
#include <gason/gason.hpp>
#include "MacPlayer.h"
#include "Util.h"
#include "Keys.h"

DialogPtr _dialog;

using namespace gason;

int main()
{	
	InitToolBox();
	MenuInit();
	EventInit();
	InitCustomLDEF();
	ShowMainWindow();
	EventLoop();
}

void InitToolBox()
{
	InitGraf(&qd.thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(NULL);
	InitCursor();
}

void ShowMainWindow()
{
	_dialog = GetNewDialog(128, 0, (WindowPtr)-1);
	MacSetPort(_dialog);

	if (_spotifyClient.AccessToken.size() > 0)
	{
		InitPlayer(_dialog);
	}
	else
	{
		ModeLogin(_dialog);
	}
}

void EventInit()
{
	AEInstallEventHandler(
		kCoreEventClass,
		kAEQuitApplication,
		(AEEventHandlerUPP)Quit,
		0L,
		false);

	AEInstallEventHandler(
		kCoreEventClass,
		kAEAnswer,
		(AEEventHandlerUPP)ProcessResponseEvent,
		0L,
		false);
}

void MenuInit()
{
	Handle menuBar;
	MenuHandle menu;
	ControlHandle control;
	OSErr myErr;
	long feature;
	
	menuBar = GetNewMBar(128);
	SetMenuBar(menuBar);
	DisposeHandle(menuBar);

	AppendResMenu(GetMenuHandle(128), 'DRVR');

	MacDrawMenuBar();
}

void EventLoop()
{
	EventRecord event;

	while (_run)
	{
		if (WaitNextEvent(everyEvent, &event, 0, NULL))
		{
			DoEvent(&event);
		}
		else
		{ 
			PollPlayerState();
		}
	}
}

void DoEvent(EventRecord *eventPtr)
{
	char theChar;

	switch (eventPtr->what)
	{
		case keyDown:
		case autoKey:
			theChar = eventPtr->message & charCodeMask;
			if ((eventPtr->modifiers & cmdKey) != 0)
				HandleMenuChoice(MenuKey(theChar));
			break;

		case mouseDown:
			HandleMouseDown(eventPtr);
			break;

		case updateEvt:
			HandleUpdate(eventPtr);
			break;

		case activateEvt:
			HandleActivate(eventPtr);
			break;

		case osEvt:
			HandleOSEvt(eventPtr);
			break;

		case kHighLevelEvent:
			AEProcessAppleEvent(eventPtr);
			break;
	}
}

void HandleMouseDown(EventRecord *eventPtr)
{
	WindowPtr window;
	short int part;
	long menuChoice;

	part = FindWindow(eventPtr->where, &window);

	switch (part)
	{
		case inMenuBar:
			menuChoice = MenuSelect(eventPtr->where);
			HandleMenuChoice(menuChoice);
			break;

		case inContent:
			if (window == FrontWindow())
				HandleInContent(eventPtr);
			break;

		case inDrag:
			if (window == FrontWindow())
				DragWindow(window, eventPtr->where, &qd.screenBits.bounds);
			break;

		case inGoAway:
			if (TrackGoAway(window, eventPtr->where))
			{
				_spotifyClient.Pause(
					[](JsonValue& root)
					{
						_run = false;
					});
			}
			break;
	}
}

void HandleMenuChoice(long menuChoice)
{
	short menu;
	short item;

	if (menuChoice != 0)
	{
		menu = HiWord(menuChoice);
		item = LoWord(menuChoice);

		switch (menu)
		{
			case mAppleMenu:
				HandleAppleChoice(item);
				break;
		}

		HiliteMenu(0);
	}
}

void HandleAppleChoice(short item)
{
	MenuHandle appleMenu;
	Str255 accName;
	short accNumber;

	switch (item)
	{
		default:
			appleMenu = GetMenuHandle(mAppleMenu);
			break;
	}
}

void HandleInContent(EventRecord *eventPtr)
{
	WindowPtr windowPtr;
	short item;

	if (DialogSelect(eventPtr, &_dialog, &item))
	{
		switch (_uiState)
		{
			case Login:
				HandleLoginContent(item);
				break;

			case Player:
				HandlePlayerContent(item);
				break;
		}
	}
}

void HandleLoginContent(short item)
{
	DialogPtr dialog = _dialog;

	switch (item)
	{
		case 2:
			_spotifyClient.Login(
				[=](LoginResponse response)
				{
					if (response.Success)
					{
						InitPlayer(dialog);
					}
					else
					{
						// TODO
					}
				});
			break;
	}
}

void HandlePlayerContent(short item)
{
	Point pt;

	switch (item)
	{
		case 1:
		{
			// Nav List
			GetMouse(&pt);
			bool dblClick = LClick(pt, 0, _navList);
			Cell cell = LLastClick(_navList);

			Str255 cellDataPtr;
			short dataLen = sizeof(Str255);
			LGetCell(cellDataPtr, &dataLen, cell, _navList);

			short cellIndex = cell.v;

			if (cellIndex > 4) // FIX ME!
			{
				GetPlaylistTracks(_playlists[cellIndex - 4].uri, _playlists[cellIndex - 4].id);
			}
			else
			{
				GetRecentTracks();
			}
			break;
		}

		case 2:
		{
			// Track List
			GetMouse(&pt);
			bool dblClick = LClick(pt, 0, _trackList);
			Cell cell = LLastClick(_trackList);
			short cellIndex = cell.v;

			if (dblClick)
			{
				_currentTrack = _tracks[cellIndex];
				PlayTrack();
			}

			break;
		}
			
		case 3:
		{
			if (_playerState.isPlaying)
			{
				_spotifyClient.Pause(
					[=](JsonValue& root)
					{
						
					});
			}
			else
			{
				_spotifyClient.Play(
					[=](JsonValue& root)
					{

					});
			}

			break;
		}

		case 4:
		{
			_spotifyClient.PreviousTrack(
				[=](JsonValue& root)
				{
					UpdateCurrentTrack();
				});
			break;
		}

		case 5:
		{
			_spotifyClient.NextTrack(
				[=](JsonValue& root)
				{
					UpdateCurrentTrack();
				});
			break;
		}
		break;
	}
}

void PlayTrack()
{
	_spotifyClient.PlayTrack(
		_currentContext,
		_currentTrack.uri, 
		[=](JsonValue& root)
		{
			ViewNowPlaying();
		});
}

void UpdateCurrentTrack()
{
	_spotifyClient.GetPlayerState(
		[=](JsonValue& root)
		{
			JsonValue track = root("item");

			if (track.getTag() == JSON_OBJECT)
			{
				_currentTrack = GetTrackObject(track);

				ViewNowPlaying();
			}
			else
			{
				// TODO: nothing playing
			}
		});
}

void ViewNowPlaying()
{
	_viewNowPlaying = true;
	HideDialogItem(_dialog, 2);
	HideControl((**_trackList).vScroll);

	string image = _currentTrack.image;

	if (image != "")
	{
		_spotifyClient.GetImage(image, _currentTrack.albumId,
			[=](PicHandle picHandle)
			{
				Rect pictRect;
				MacSetRect(&pictRect, 127, 0, 377, 250);

				if (picHandle != nil)
				{
					ForeColor(whiteColor);
					PaintRect(&pictRect);
					DrawPicture(picHandle, &pictRect);
					DisposeHandle((Handle)picHandle);
				}
				else
				{
					// Error getting image, just draw black
					ForeColor(blackColor);
					PaintRect(&pictRect);
				}
			});
	}
}

void GetRecentTracks()
{
	_spotifyClient.GetRecentTracks(
		[=](JsonValue& root)
		{
			PopulateTrackList(root);
		});
}

void GetPlaylistTracks(const string& uri, const string& playlistId)
{
	_spotifyClient.GetPlaylistTracks(
		playlistId,
		[=](JsonValue& root)
		{
			_currentContext = uri;
			PopulateTrackList(root);
		});
}

void PopulateTrackList(JsonValue& root)
{
	JsonValue items = root("items");
	JsonValue track;
	JsonValue artists;
	Cell cell;
	char* trackName;
	char* artist;

	int rowNum = (**_trackList).dataBounds.bottom;

	_viewNowPlaying = false;
	ShowDialogItem(_dialog, 2);
	ShowControl((**_trackList).vScroll);

	LSetDrawingMode(false, _trackList);

	JsonIterator it = gason::begin(items);

	_tracks.clear();
	LDelRow(0, 0, _trackList);

	while (it.isValid())
	{
		JsonValue father = it->value;

		track = father("track");
		trackName = track("name").toString();
		artists = track("artists");
		artist = artists[0]("name").toString();

		char* pLabel = (char*)Util::CtoPStr(trackName);
		Str255 label;
		strncpy((char*)label, pLabel, 256);

		rowNum = LAddRow(1, rowNum, _trackList);
		SetPt(&cell, 0, rowNum);
		LSetCell(label, sizeof(Str255), cell, _trackList);

		char* pArtist = (char*)Util::CtoPStr(artist);
		Str255 artist255;
		strncpy((char*)artist255, pArtist, 256);

		SetPt(&cell, 1, rowNum);
		LSetCell(artist255, sizeof(Str255), cell, _trackList);

		_tracks.push_back(GetTrackObject(track));

		rowNum = rowNum + 1;
		it++;
	}

	LSetDrawingMode(true, _trackList);
	InvalRect(&(**_trackList).rView);

	if ((**_trackList).vScroll != NULL)
		InvalRect(&(**(**_trackList).vScroll).contrlRect);

	LUpdate(_dialog->visRgn, _trackList);
}

Track GetTrackObject(JsonValue& track)
{
	Track trackObj;
	trackObj.name = track("name").toString();
	trackObj.uri = track("uri").toString();
	SetTrackImage(track, trackObj);

	return trackObj;
}

void SetTrackImage(JsonValue& track, Track& trackObj)
{
	JsonValue album = track.child("album");

	if (album.getTag() == JSON_OBJECT)
	{
		JsonValue images = album.child("images");

		if (images.getTag() == JSON_ARRAY)
		{
			JsonValue image = images.at(0);

			if (image == JSON_OBJECT)
			{
				trackObj.albumId = string(album("id").toString());
				trackObj.image = string(image.child("url").toString());
			}
		}
	}
}

void WaitCursor()
{
	CursHandle theCursor = MacGetCursor(watchCursor);
	MacSetCursor(*theCursor);
}

void ModeInit(DialogPtr dialog)
{
	_uiState = Init;

	MacSetPort(dialog);
	ForeColor(whiteColor);

	ShortenDITL(dialog, CountDITL(dialog));
	Handle ditl = GetResource('DITL', 130);
	AppendDITL(dialog, ditl, overlayDITL);
	ReleaseResource(ditl);

	UpdateDialog(dialog, dialog->visRgn);
}

void ModeLogin(DialogPtr dialog)
{
	_uiState = Login;

	MacSetPort(dialog);

	ShortenDITL(dialog, CountDITL(dialog));
	Handle ditl = GetResource('DITL', 128);
	AppendDITL(dialog, ditl, overlayDITL);
	ReleaseResource(ditl);

	UpdateDialog(dialog, dialog->visRgn);
}

void ModePlayer(DialogPtr dialog)
{
	DialogItemType type;
	Handle itemH;
	Rect box;

	_uiState = Player;

	MacSetPort(dialog);

	ShortenDITL(dialog, CountDITL(dialog));
	Handle ditl = GetResource('DITL', 129);
	AppendDITL(dialog, ditl, overlayDITL);
	ReleaseResource(ditl);

	// Init nav list
	GetDialogItem(dialog, 1, &type, &itemH, &box);
	_navList = CreateList(dialog, box, 1, 128, 0, 0);
	PopulateNavList(_navList);

	// Init track list
	GetDialogItem(dialog, 2, &type, &itemH, &box);
	_trackList = CreateList(dialog, box, 2, 128, 0, 0);

	UpdateDialog(dialog, dialog->visRgn);
}

ListHandle CreateList(
	WindowPtr windowPtr,
	Rect rect,
	int columnsInList,
	int ldef,
	short cellWidth,
	short cellHeight)
{
	bool doDraw = true;
	bool noGrow = false;
	bool includeScrollBar = true;
	int scrollBarWidth = 15;
	Rect myDataBounds;
	Point myCellSize;

	MacSetRect(&myDataBounds, 0, 0, columnsInList, 0);
	SetPt(&myCellSize, cellWidth, cellHeight);
	rect.right = rect.right - scrollBarWidth;

	return
		LNew(&rect, &myDataBounds, myCellSize, ldef, windowPtr,
			doDraw, noGrow, !includeScrollBar,
			includeScrollBar);
}

void PopulateNavList(ListHandle list)
{
	array<string, 4> items = 
	{ 
		"Recently Played", 
		"Songs", 
		"Albums",
		"Artists"
	};

	int rowNum = (**list).dataBounds.bottom;
	Cell cell;

	for (const auto& item : items) 
	{
		rowNum = LAddRow(1, rowNum, list);
		SetPt(&cell, 0, rowNum);

		char* pLabel = (char*)Util::StrToPStr(item);
		Str255 label;
		strncpy((char*)label, pLabel, 256);

		LSetCell(label, sizeof(Str255), cell, list);
		rowNum = rowNum + 1;
	}

	for (const auto& playlist : _playlists)
	{
		rowNum = LAddRow(1, rowNum, list);
		SetPt(&cell, 0, rowNum);

		char* pLabel = (char*)Util::StrToPStr(playlist.name);
		Str255 label;
		strncpy((char*)label, pLabel, 256);

		LSetCell(label, sizeof(Str255), cell, list);
		rowNum = rowNum + 1;
	}
}

void InitPlayer(DialogPtr dialog)
{
	ModeInit(dialog);
	WaitCursor();
	Microseconds(&_lastPollTime);

	_currentTrack.uri = "";
	_playerState.isPlaying = false;

	// Get available devices
	_spotifyClient.GetDevices(
		[=](JsonValue& root)
		{	
			string deviceId, deviceName;
			bool active;

			MenuHandle deviceMenu = GetMenuHandle(130);
			int itemCount = CountMItems(deviceMenu);

			JsonValue devices = root("devices");

			JsonIterator it = gason::begin(devices);
			while (it.isValid())
			{
				JsonValue device = it->value;

				deviceId = device("id").toString();
				deviceName = device("name").toString();
				active = device("is_active").toBool();

				AppendMenu(deviceMenu, "\p ");
				itemCount++;
				SetMenuItemText(deviceMenu, itemCount, Util::StrToPStr(deviceName));

				if (active)
					SetItemMark(deviceMenu, itemCount, checkMark);

				it++;
			}

			MacDrawMenuBar();
			GetPlaylists(dialog);
		});
}

void GetPlaylists(DialogPtr dialog)
{
	_spotifyClient.GetPlaylists(
		[=](JsonValue& root)
		{
			JsonValue playlists = root("items");

			JsonIterator it = gason::begin(playlists);
			while (it.isValid())
			{
				JsonValue playlistNode = it->value;

				Playlist playlist;
				playlist.name = playlistNode("name").toString();
				playlist.id = playlistNode("id").toString();
				playlist.uri = playlistNode("uri").toString();

				_playlists.push_back(playlist);

				it++;
			}

			ModePlayer(dialog);
			InitCursor();
		});
}

void PollPlayerState()
{
	UnsignedWide now;
	Microseconds(&now);

	double diffMs = (Util::MicrosecondToDouble(&now) - Util::MicrosecondToDouble(&_lastPollTime)) / 1000;

	if (diffMs > mPollFrequencyMs)
	{
		_lastPollTime = now;
		_spotifyClient.GetPlayerState(
			[=](JsonValue& root)
			{
				bool isPlaying = root("is_playing").getTag() == JSON_TRUE;

				if (isPlaying != _playerState.isPlaying)
				{
					_playerState.isPlaying = isPlaying;

					int iconId = isPlaying ? 136 : 128;

					DialogItemType itemType;
					Handle tempHandle;
					Rect itemRect;
					Handle iconHandle = (Handle)GetCIcon(iconId);
					GetDialogItem(_dialog, 3, &itemType, &tempHandle, &itemRect);

					SetDialogItem(_dialog, 3, itemType, iconHandle, &itemRect);

					InvalRect(&itemRect);
					UpdateDialog(_dialog, _dialog->visRgn);
				}

				if (_viewNowPlaying)
				{
					if (root("item").getTag() == JSON_OBJECT)
					{
						JsonValue track = root("item");

						if (track("uri").getTag() == JSON_STRING)
						{
							string trackUri = track("uri").toString();

							if (trackUri != _currentTrack.uri)
							{
								_currentTrack = GetTrackObject(track);
								ViewNowPlaying();
							}
						}
					}
				}
			});
	}	
}

void HandleUpdate(EventRecord *eventPtr)
{
	WindowPtr windowPtr = (WindowPtr)eventPtr->message;

	if (windowPtr == FrontWindow())
	{
		BeginUpdate(windowPtr);
		UpdateDialog(windowPtr, windowPtr->visRgn);
		EndUpdate(windowPtr);
	}
}

void HandleActivate(EventRecord *eventPtr)
{
	WindowPtr windowPtr = (WindowPtr)eventPtr->message;

	/*if (windowPtr == _prefs.GetWindow())
	{
		bool becomingActive = (eventPtr->modifiers & activeFlag) == activeFlag;
		_prefs.Activate(becomingActive);
	}*/
}

void HandleOSEvt(EventRecord *eventPtr)
{
	switch ((eventPtr->message >> 24) & 0x000000FF)
	{
		case suspendResumeMessage:
			bool becomingActive = (eventPtr->message & resumeFlag) == 1;
			//_prefs.Activate(becomingActive);
			break;
	}
}

pascal OSErr Quit(AppleEvent* appleEvent, AppleEvent* reply, long refCon)
{
	_run = false;
}

pascal OSErr ProcessResponseEvent(AppleEvent* appleEvent, AppleEvent* reply, long refCon)
{
	_wifiLib.ProcessReply(appleEvent);
}

void InitCustomLDEF()
{
	// 10-byte code resource stub trick

	Handle h = GetResource('LDEF', 128);
	HLock(h);
	*(ListDefProcPtr*)(*h + 6) = &DarkListDef;

	Handle h2 = GetResource('CDEF', 1);
	HLock(h2);
	*(ControlDefProcPtr*)(*h2 + 6) = &DarkScrollbarDef;
}