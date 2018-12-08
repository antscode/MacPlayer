#include <Events.h>
#include <Resources.h>
#include <Dialogs.h>
#include <Quickdraw.h>
#include <Menus.h>
#include <ToolUtils.h>
#include <Devices.h>
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
	_dialog = GetNewDialog(kPlayerDialog, 0, (WindowPtr)-1);
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
	
	menuBar = GetNewMBar(kPlayerMenubar);
	SetMenuBar(menuBar);
	DisposeHandle(menuBar);

	AppendResMenu(GetMenuHandle(kPlayerMenubar), 'DRVR');

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
		else if(_uiState == Player)
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
				ExitApp();
			}
			break;
	}
}

void HandleMenuChoice(long menuChoice)
{
	Str255 str;
	WindowRef w;
	short menuID = menuChoice >> 16;
	short menuItem = menuChoice & 0xFFFF;
	if (menuID == kMenuApple)
	{
		if (menuItem == kItemAbout)
			ShowAboutBox();
		else
		{
			GetMenuItemText(MacGetMenu(128), menuItem, str);
			OpenDeskAcc(str);
		}
	}
	else if (menuID == kMenuFile)
	{
		switch (menuItem)
		{
			case kItemQuit:
				ExitApp();
				break;
		}
	}
	else if (menuID == kMenuDevices)
	{
		if (menuItem < _devices.size())
		{
			ActivateDevice(menuItem);
		}
		else
		{
			// Refresh menu item
			GetDevices();
		}
	}

	HiliteMenu(0);
}

void ActivateDevice(int index)
{
	_spotifyClient.ActivateDevice(
		_devices[index].id,
		[=](JsonValue& root)
		{
			MenuHandle deviceMenu = GetMenuHandle(130);
			SetItemMark(deviceMenu, index, checkMark);
		});
}

void ShowAboutBox()
{
	DialogRef aboutDialog = GetNewDialog(kAboutDialog, 0, (WindowPtr)-1);

	MacSetPort(aboutDialog);
	UpdateDialog(aboutDialog, aboutDialog->visRgn);

	while (!Button())
		;
	while (Button())
		;

	FlushEvents(everyEvent, 0);
	DisposeWindow(aboutDialog);
	MacSetPort(_dialog);
}

void ExitApp()
{
	CloseDialog(_dialog);

	if (_playerState.isPlaying)
	{
		_spotifyClient.Pause(
			[](JsonValue& root)
			{
				_run = false;
			});
	}
	else
	{
		_run = false;
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
		case kPlayerLoginButton:
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
		case kPlayerNavList:
		{
			// Nav List
			GetMouse(&pt);
			bool dblClick = LClick(pt, 0, _navList);
			Cell cell = LLastClick(_navList);

			short cellIndex = cell.v;

			if (cellIndex < _playlists.size())
			{
				GetPlaylistTracks(_playlists[cellIndex].uri, _playlists[cellIndex].id);
			}
			break;
		}

		case kPlayerTrackList:
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
			
		case kPlayerPlayButton:
		{
			if (_playerState.isPlaying)
			{
				_spotifyClient.Pause(
					[=](JsonValue& root)
					{ });
			}
			else
			{
				_spotifyClient.Play(
					[=](JsonValue& root)
					{ });
			}

			_playerState.isPlaying = !_playerState.isPlaying;
			TogglePlayButtonIcon();
			break;
		}

		case kPlayerPrevTrack:
		{
			_spotifyClient.PreviousTrack(
				[=](JsonValue& root)
				{
					UpdateCurrentTrack();
				});
			break;
		}

		case kPlayerNextTrack:
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
	_playerState.isPlaying = true;
	TogglePlayButtonIcon();

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
	HideDialogItem(_dialog, kPlayerTrackLabel);
	HideDialogItem(_dialog, kPlayerArtistLabel);
	HideDialogItem(_dialog, kPlayerTrackList);
	HideControl((**_trackList).vScroll);

	string image = _currentTrack.image;

	if (image != "")
	{
		_spotifyClient.GetImage(image, _currentTrack.albumId,
			[=](PicHandle picHandle)
			{
				DrawTrackImage();
			});
	}
}

void DrawTrackImage()
{
	Rect pictRect;
	MacSetRect(&pictRect, 127, 0, 377, 250);

	if (_spotifyClient.ActiveTrackImage != nil)
	{
		ForeColor(whiteColor);
		PaintRect(&pictRect);
		DrawPicture(_spotifyClient.ActiveTrackImage, &pictRect);
	}
	else
	{
		// Error getting image, just draw black
		ForeColor(blackColor);
		PaintRect(&pictRect);
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

	int rowNum = (**_trackList).dataBounds.bottom;

	_viewNowPlaying = false;

	ShowDialogItem(_dialog, kPlayerTrackLabel);
	ShowDialogItem(_dialog, kPlayerArtistLabel);
	ShowDialogItem(_dialog, kPlayerTrackList);
	ShowControl((**_trackList).vScroll);

	LSetDrawingMode(false, _trackList);

	JsonIterator it = gason::begin(items);

	_tracks.clear();
	LDelRow(0, 0, _trackList);

	while (it.isValid())
	{
		JsonValue father = it->value;
		ListRow row;
		
		track = father("track");
		artists = track("artists");

		row.CellCount = 2;

		char* pTrackName = (char*)Util::CtoPStr(track("name").toString());
		strncpy((char*)row.Cells[0].Content, pTrackName, 256);

		row.Cells[0].WidthPercent = 0.5;

		char* pArtist = (char*)Util::CtoPStr(artists[0]("name").toString());
		strncpy((char*)row.Cells[1].Content, pArtist, 256);
		row.Cells[1].WidthPercent = 0.5;

		rowNum = LAddRow(1, rowNum, _trackList);
		SetPt(&cell, 0, rowNum);
		LSetCell(&row, sizeof(ListRow), cell, _trackList);

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
	Handle ditl = GetResource('DITL', kPlayerStartupDITL);
	AppendDITL(dialog, ditl, overlayDITL);
	ReleaseResource(ditl);

	UpdateDialog(dialog, dialog->visRgn);
}

void ModeLogin(DialogPtr dialog)
{
	_uiState = Login;

	MacSetPort(dialog);

	ShortenDITL(dialog, CountDITL(dialog));
	Handle ditl = GetResource('DITL', kPlayerLoginDITL);
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
	Handle ditl = GetResource('DITL', kPlayerMainDITL);
	AppendDITL(dialog, ditl, overlayDITL);
	ReleaseResource(ditl);

	HideDialogItem(dialog, kPlayerTrackLabel);
	HideDialogItem(dialog, kPlayerArtistLabel);

	// Init nav list
	GetDialogItem(dialog, kPlayerNavList, &type, &itemH, &box);
	_navList = CreateList(dialog, box, 1, 128, 0, 0);
	PopulateNavList(_navList);

	// Init track list
	GetDialogItem(dialog, kPlayerTrackList, &type, &itemH, &box);
	_trackList = CreateList(dialog, box, 1, 128, 0, 0);

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
	LSetDrawingMode(false, list);

	int rowNum = (**list).dataBounds.bottom;
	Cell cell;
	ListRow row;

	for (const auto& playlist : _playlists)
	{
		row.CellCount = 1;

		rowNum = LAddRow(1, rowNum, list);
		SetPt(&cell, 0, rowNum);

		char* pLabel = (char*)Util::StrToPStr(playlist.name);
		strncpy((char*)row.Cells[0].Content, pLabel, 256);
		row.Cells[0].WidthPercent = 1;

		LSetCell(&row, sizeof(ListRow), cell, list);
		rowNum = rowNum + 1;
	}

	LSetDrawingMode(true, list);
}

void InitPlayer(DialogPtr dialog)
{
	ModeInit(dialog);
	WaitCursor();
	Microseconds(&_lastPollTime);

	_currentTrack.uri = "";
	_playerState.isPlaying = false;

	GetDevices();
	GetPlaylists(dialog);
}

void GetDevices()
{
	_spotifyClient.GetDevices(
		[=](JsonValue& root)
		{
			string deviceId, deviceName;
			bool active;

			MenuHandle deviceMenu = GetMenuHandle(130);
			int itemCount = CountMItems(deviceMenu);

			// Clear existing menu items
			while (itemCount > 0) {
				DeleteMenuItem(deviceMenu, 1);
				itemCount--;
			}

			JsonValue devices = root("devices");

			JsonIterator it = gason::begin(devices);
			while (it.isValid())
			{
				JsonValue device = it->value;

				Device deviceObj;
				deviceObj.name = device("name").toString();
				deviceObj.id = device("id").toString();
				deviceObj.active = device("is_active").toBool();

				_devices.push_back(deviceObj);

				AppendMenu(deviceMenu, "\p ");
				itemCount++;
				SetMenuItemText(deviceMenu, itemCount, Util::StrToPStr(deviceObj.name));

				if (deviceObj.active)
					SetItemMark(deviceMenu, itemCount, checkMark);

				it++;
			}

			AppendMenu(deviceMenu, "\p ");
			itemCount++;
			SetMenuItemText(deviceMenu, itemCount, "\pRefresh...");

			MacDrawMenuBar();

			if (itemCount == 0)
			{
				// No available devices
				_spotifyClient.HandleError("No Spotify Connect devices found.\rPlease enable a device then refresh via the Devices menu.");
			}
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
					TogglePlayButtonIcon();
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

void TogglePlayButtonIcon()
{
	DialogItemType itemType;
	Handle tempHandle;
	Rect itemRect;

	int iconId = _playerState.isPlaying ? kPlayerPauseIcon : kPlayerPlayIcon;
	Handle iconHandle = (Handle)GetCIcon(iconId);

	GetDialogItem(_dialog, kPlayerPlayButton, &itemType, &tempHandle, &itemRect);
	SetDialogItem(_dialog, kPlayerPlayButton, itemType, iconHandle, &itemRect);

	InvalRect(&itemRect);
	UpdateDialog(_dialog, _dialog->visRgn);
}

void HandleUpdate(EventRecord *eventPtr)
{
	WindowPtr windowPtr = (WindowPtr)eventPtr->message;

	if (windowPtr == FrontWindow())
	{
		BeginUpdate(windowPtr);

		if (_navList != 0)
		{
			LUpdate(_dialog->visRgn, _navList);
		}

		if (_viewNowPlaying)
		{
			DrawTrackImage();
		}
		else
		{
			if (_trackList != 0)
			{
				LUpdate(_dialog->visRgn, _trackList);
			}
		}

		UpdateDialog(windowPtr, windowPtr->visRgn);
		EndUpdate(windowPtr);
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