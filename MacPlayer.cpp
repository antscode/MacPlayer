#include <Events.h>
#include <Resources.h>
#include <Dialogs.h>
#include <Quickdraw.h>
#include <Menus.h>
#include <ToolUtils.h>
#include <string.h>
#include <gason/gason.hpp>
#include "MacPlayer.h"
#include "Util.h"
#include "Keys.h"

DialogPtr _dialog;

using namespace gason;

string GetTrackImage(JsonValue& track);

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
				_run = false;
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
				GetRecentTracks();
			}
			break;

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
					ViewNowPlaying();
				}
			}
			break;
	}
}

void PlayTrack()
{
	// TODO
}

void ViewNowPlaying()
{
	HideDialogItem(_dialog, 2);
	HideControl((**_trackList).vScroll);

	string image = _currentTrack.image;

	if (image != "")
	{
		_wifiLib.Utf8ToMacRoman(false);
		_wifiLib.Get(
			"https://68k.io/image?ma_client_id=" + Keys::ClientId +
			"&source_url=" + image +
			"&dest_width=250&dest_height=250",
			[=](MacWifiResponse response)
		{
			if (response.Success)
			{
				vector<char> v(response.Content.begin(), response.Content.end());
				char* pict = &v[512]; // Skip 512-byte PICT1 header

				PicHandle imageHandle = (PicHandle)&pict;

				Rect pictRect;
				MacSetRect(&pictRect, 127, 0, 377, 250);

				DrawPicture(imageHandle, &pictRect);

				_wifiLib.Utf8ToMacRoman(true);
			}
		});
	}
}

void GetRecentTracks()
{
	_spotifyClient.Get(
		"https://api.spotify.com/v1/me/player/recently-played",
		[=](MacWifiResponse response)
	{
		if (response.Success)
		{
			JsonAllocator    allocator;
			JsonValue        root;
			
			string deviceId, deviceName;

			MenuHandle deviceMenu = GetMenuHandle(130);
			int itemCount = CountMItems(deviceMenu);

			JsonParseStatus status = jsonParse((char*)response.Content.c_str(), root, allocator);
			if (status == JSON_PARSE_OK)
			{
				JsonValue items = root("items");
				JsonValue track;
				JsonValue artists;
				Cell cell;
				char* trackName;
				char* artist;

				int rowNum = (**_trackList).dataBounds.bottom;

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

					Track trackObj;
					trackObj.image = GetTrackImage(track);

					_tracks.push_back(trackObj);

					rowNum = rowNum + 1;
					it++;
				}

				LSetDrawingMode(true, _trackList);
				InvalRect(&(**_trackList).rView);

				if((**_trackList).vScroll != NULL)
					InvalRect(&(**(**_trackList).vScroll).contrlRect);

				LUpdate(_dialog->visRgn, _trackList);
			}
		}
		else
		{
			// TODO
		}
	});
}

string GetTrackImage(JsonValue& track)
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
				return string(image.child("url").toString());
			}
		}
	}

	return "";
}

void WaitCursor()
{
	CursHandle theCursor = MacGetCursor(watchCursor);
	MacSetCursor(*theCursor);
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
}

void InitPlayer(DialogPtr dialog)
{
	WaitCursor();

	// Get available devices
	_spotifyClient.Get(
		"https://api.spotify.com/v1/me/player/devices",
		[=](MacWifiResponse response)
		{
			if (response.Success)
			{
				JsonAllocator allocator;
				JsonValue root;
				JsonParseStatus status = jsonParse((char*)response.Content.c_str(), root, allocator);
				string deviceId, deviceName;
				bool active;

				MenuHandle deviceMenu = GetMenuHandle(130);
				int itemCount = CountMItems(deviceMenu);

				if (status == JSON_PARSE_OK)
				{
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
					ModePlayer(dialog);
					InitCursor();
				}
			}
			else
			{
				// TODO
			}
		});
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