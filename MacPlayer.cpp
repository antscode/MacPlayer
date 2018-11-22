#include <Events.h>
#include <Resources.h>
#include <Dialogs.h>
#include <Quickdraw.h>
#include <Menus.h>
#include <ToolUtils.h>
#include <string.h>
#include <json/json.h>
#include "MacPlayer.h"
#include "Util.h"
#include "Keys.h"
#include "base64.h"
#include "gason.hpp"

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

	if (_prefs.Data.isMember("access_token"))
	{
		_accessToken = _prefs.Data["access_token"].asString();
		_refreshToken = _prefs.Data["refresh_token"].asString();
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

	if (_uiState == Login)
	{
		_macAuth.HandleEvents(eventPtr);
	}

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
	switch (item)
	{
		case 2:
			DoLogin();
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
				// TODO: Play track!
			}
			break;
	}
}

void GetRecentTracks()
{
	SpotifyRequest(
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

				LSetDrawingMode(false, _trackList);

				JsonIterator it = gason::begin(items);

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

void DoLogin()
{
	AuthRequest authRequest;

	authRequest.ClientId = Keys::ClientId;
	authRequest.Provider = "spotify";

	authRequest.Params.insert(pair<string, string>("client_id", Keys::SpotifyClientId));
	authRequest.Params.insert(pair<string, string>("response_type", "code"));
	authRequest.Params.insert(pair<string, string>("scope", MacWifiLib::Encode("user-read-playback-state user-read-recently-played")));

	// For some reason, accessing a dialog global within a lamba does weird things
	// but capturing as a local variable works fine
	DialogPtr dialog = _dialog;

	_macAuth.Authenticate(authRequest, [=](AuthResponse response)
	{
		if (response.Success)
		{
			WaitCursor();

			// Get access & refresh tokens
			_wifiLib.Post(
				"https://accounts.spotify.com/api/token",
				"client_id=" + Keys::SpotifyClientId +
				"&client_secret=" + Keys::SpotifyClientSecret +
				"&grant_type=authorization_code&code=" + response.Code +
				"&redirect_uri=https://68k.io/login/callback",
				[=](MacWifiResponse response)
				{
					if (response.Success)
					{
						Json::Value root;
						Json::Reader reader;
						bool parseSuccess = reader.parse(response.Content.c_str(), root);

						if (parseSuccess)
						{
							_prefs.Data["access_token"] = root["access_token"].asString();
							_prefs.Data["refresh_token"] = root["refresh_token"].asString();
							_prefs.Save();

							_accessToken = _prefs.Data["access_token"].asString();
							_refreshToken = _prefs.Data["refresh_token"].asString();
							InitPlayer(dialog);
						}
					}
					else
					{
						// TODO
					}
				});
		}
		else
		{
			// TODO
		}
	});
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

void SpotifyRequest(string uri, function<void(MacWifiResponse)> onComplete)
{
	SpotifyRequest(uri, onComplete, false);
}

void SpotifyRequest(string uri, function<void(MacWifiResponse)> onComplete, bool refreshed)
{
	_wifiLib.SetAuthorization("Bearer " + _accessToken);
	_wifiLib.Get(
		uri,
		[=](MacWifiResponse response)
		{
			if (response.Success)
			{
				if (response.StatusCode == 200)
				{
					// All good, trigger callback
					onComplete(response);
				}
				else if (response.StatusCode == 401)
				{
					if (!refreshed)
					{
						// Expired access token, so refresh it and try again
						RefreshAccessToken(uri, onComplete);
					}
					else
					{
						// Authentication still failing after a refresh, give up
						// TODO
					}
				}
				else
				{
					// TODO: Something else went wrong
				}
			}
			else
			{
				// TODO: Comms error
			}
		});
}

void RefreshAccessToken(string uri, function<void(MacWifiResponse)> onComplete)
{
	const std::string authHeader = Keys::SpotifyClientId + ":" + Keys::SpotifyClientSecret;
	std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(authHeader.c_str()), authHeader.length());

	_wifiLib.SetAuthorization("Basic " + encoded);
	_wifiLib.Post(
		"https://accounts.spotify.com/api/token",
		"grant_type=refresh_token&refresh_token=" + _refreshToken,
		[=](MacWifiResponse response)
		{
			if (response.Success)
			{
				Json::Value root;
				Json::Reader reader;
				bool parseSuccess = reader.parse(response.Content.c_str(), root);

				if (parseSuccess)
				{
					_prefs.Data["access_token"] = root["access_token"].asString();
					_prefs.Save();

					_accessToken = _prefs.Data["access_token"].asString();

					SpotifyRequest(uri, onComplete, true);
				}
			}
			else
			{
				// TODO
			}	
		});
}

void InitPlayer(DialogPtr dialog)
{
	WaitCursor();

	// Get available devices
	SpotifyRequest(
		"https://api.spotify.com/v1/me/player/devices",
		[=](MacWifiResponse response)
		{
			if (response.Success)
			{
				Json::Value root;
				Json::Reader reader;
				string deviceId, deviceName;
				bool parseSuccess = reader.parse(response.Content.c_str(), root);

				MenuHandle deviceMenu = GetMenuHandle(130);
				int itemCount = CountMItems(deviceMenu);

				if (parseSuccess)
				{
					Json::Value devices = root["devices"];

					for (int i = 0; i < devices.size(); i++)
					{
						deviceId = devices[i]["id"].asString();
						deviceName = devices[i]["name"].asString();

						AppendMenu(deviceMenu, "\p ");
						itemCount++;
						SetMenuItemText(deviceMenu, itemCount, Util::StrToPStr(deviceName));
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