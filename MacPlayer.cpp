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
		if (menuItem <= _devices.size())
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
		_devices[index - 1].id,
		[=](JsonValue& root)
		{
			MenuHandle deviceMenu = GetMenuHandle(130);
			SetItemMark(deviceMenu, index, checkMark);
			_activeDevice = true;
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
				if (_activeDevice)
				{
					_currentTrack = _tracks[cellIndex];
					PlayTrack();
				}
				else
				{
					NoDevicesError();
				}
			}

			break;
		}
			
		case kPlayerPlayButton:
		{
			if (_activeDevice)
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
			}
			else
			{
				NoDevicesError();
			}
			break;
		}

		case kPlayerPrevTrack:
		{
			if (_activeDevice)
			{
				_spotifyClient.PreviousTrack(
					[=](JsonValue& root)
					{
						UpdateCurrentTrack();
					});
			}
			else
			{
				NoDevicesError();
			}
			break;
		}

		case kPlayerNextTrack:
		{
			if (_activeDevice)
			{
				_spotifyClient.NextTrack(
					[=](JsonValue& root)
					{
						UpdateCurrentTrack();
					});
			}
			else
			{
				NoDevicesError();
			}
			break;
		}
		break;
	}
}

void PlayTrack()
{
	_playerState.isPlaying = true;
	TogglePlayButtonIcon();
	DrawTrackName();

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
	// Pause polling to prevent conflicts
	_pausePoll = true;

	_spotifyClient.GetPlayerState(
		[=](JsonValue& root)
		{
			JsonValue track = root("item");

			if (track.getTag() == JSON_OBJECT)
			{
				_currentTrack = GetTrackObject(track);
				
				DrawTrackName();
				ViewNowPlaying();
				_pausePoll = false;
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
		if (!_spotifyClient.ImageIsCached(_currentTrack.albumId))
		{
			DrawPlaceholderImage();
		}

		_spotifyClient.GetImage(image, _currentTrack.albumId,
			[=](PicHandle picHandle)
			{
				DrawTrackImage();
			});
	}
}

void DrawPlaceholderImage()
{
	Rect pictRect;
	MacSetRect(&pictRect, 127, 0, 377, 250);

	PicHandle pic = GetPicture(kPlaceholderImage);
	DrawPicture(pic, &pictRect);
}

void TrackChanging()
{
	// Temporary state while we don't yet know the next track
	_currentTrack.name = "";
	_currentTrack.artist = "";
	DrawTrackName();

	if (_viewNowPlaying)
	{
		DrawPlaceholderImage();
	}
}

void DrawTrackImage()
{
	if (_spotifyClient.ActiveTrackImage != nil)
	{
		ForeColor(whiteColor);
		PaintRect(&_trackImageRect);
		DrawPicture(_spotifyClient.ActiveTrackImage, &_trackImageRect);
	}
	else
	{
		// Error getting image, just draw black
		ForeColor(blackColor);
		PaintRect(&_trackImageRect);
	}
}

void DrawTrackName()
{
	Rect trackRect;
	MacSetRect(&trackRect, 10, 262, 130, 290);
	EraseRect(&trackRect);

	short length;

	string name = _currentTrack.name.c_str();
	const char* cName = name.c_str();
	length = strlen(cName);
	TruncText(120, (Ptr)cName, &length, truncEnd);

	unsigned char* pName = Util::CtoPStr((char*)cName);
	pName[0] = length;

	MoveTo(10, 271);
	DrawString(pName);

	string artist = _currentTrack.artist.c_str();
	const char* cArtist = artist.c_str();
	length = strlen(cArtist);
	TruncText(120, (Ptr)cArtist, &length, truncEnd);

	unsigned char* pArtist = Util::CtoPStr((char*)cArtist);
	pArtist[0] = length;

	MoveTo(10, 283);
	DrawString(pArtist);
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
	
	EraseRect(&(**_trackList).rView);
	EraseRect(&_trackImageRect);

	LSetDrawingMode(false, _trackList);

	JsonIterator it = gason::begin(items);

	_tracks.clear();
	LDelRow(0, 0, _trackList);

	while (it.isValid())
	{
		JsonValue father = it->value;
		ListRow row;
		Track trackObj;
		
		track = father("track");
		artists = track("artists");

		trackObj = GetTrackObject(track);
		_tracks.push_back(trackObj);

		row.CellCount = 2;

		char* pTrackName = (char*)Util::StrToPStr(trackObj.name);
		strncpy((char*)row.Cells[0].Content, pTrackName, 256);

		row.Cells[0].WidthPercent = 0.5;

		char* pArtist = (char*)Util::StrToPStr(trackObj.artist);
		strncpy((char*)row.Cells[1].Content, pArtist, 256);
		row.Cells[1].WidthPercent = 0.5;

		rowNum = LAddRow(1, rowNum, _trackList);
		SetPt(&cell, 0, rowNum);
		LSetCell(&row, sizeof(ListRow), cell, _trackList);

		rowNum = rowNum + 1;
		it++;
	}

	LSetDrawingMode(true, _trackList);
	InvalRect(&(**_trackList).rView);

	if ((**_trackList).vScroll != NULL)
		InvalRect(&(**(**_trackList).vScroll).contrlRect);

	UpdateDialog(_dialog, _dialog->visRgn);
}

Track GetTrackObject(JsonValue& track)
{
	Track trackObj;
	trackObj.name = track("name").toString();
	trackObj.uri = track("uri").toString();

	JsonValue artists = track("artists");
	trackObj.artist = artists[0]("name").toString();

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

	MacSetRect(&_trackImageRect, 127, 0, 377, 250);

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
				deviceObj.active = device("is_active").getTag() == JSON_TRUE;

				_devices.push_back(deviceObj);

				AppendMenu(deviceMenu, "\p ");
				itemCount++;
				SetMenuItemText(deviceMenu, itemCount, Util::StrToPStr(deviceObj.name));

				if (deviceObj.active)
				{
					SetItemMark(deviceMenu, itemCount, checkMark);
					_activeDevice = true;
				}

				it++;
			}

			AppendMenu(deviceMenu, "\p ");
			itemCount++;
			SetMenuItemText(deviceMenu, itemCount, "\pRefresh...");

			MacDrawMenuBar();

			if (itemCount == 1)
			{
				NoDevicesError();
			}
		});
}

void NoDevicesError()
{
	_spotifyClient.HandleError("No Spotify Connect devices found.\rPlease enable a device then refresh via Devices menu.");
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

	if (diffMs > mPollFrequencyMs && !_pausePoll)
	{
		_lastPollTime = now;
		_spotifyClient.GetPlayerState(
			[=](JsonValue& root)
			{
				if (_pausePoll)
				{
					return;
				}

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
								DrawTrackName();
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
	if (_dialog == FrontWindow())
	{
		BeginUpdate(_dialog);

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

		DrawTrackName();
		UpdateDialog(_dialog, _dialog->visRgn);
		EndUpdate(_dialog);
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