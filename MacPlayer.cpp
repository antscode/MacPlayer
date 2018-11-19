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

DialogPtr _dialog;

int main()
{	
	InitToolBox();
	MenuInit();
	EventInit();
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
			// Nav List
			GetMouse(&pt);
			bool dblClick = LClick(pt, 0, _navList);
			Cell cell = LLastClick(_navList);
			short cellIndex = cell.v;
			GetRecentTracks();
			break;
	}
}

void GetRecentTracks()
{
	SpotifyRequest(
		"https://api.spotify.com/v1/me/player/recently-played",
		[=](MacWifiResponse response)
	{
		Util::Debug(response.Content);


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
				Json::Value items = root["items"];
				Json::Value track;
				string trackName;
				Cell cell;

				int rowNum = (**_trackList).dataBounds.bottom;

				for (int i = 0; i < items.size(); i++)
				{
					track = items[i]["track"];
					trackName = track["name"].asString();

					rowNum = LAddRow(1, rowNum, _trackList);
					SetPt(&cell, 0, rowNum);
					LSetCell(trackName.c_str(), trackName.size(), cell, _trackList);
					rowNum = rowNum + 1;
				}
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

	MacFillRect(&dialog->portRect, &qd.black);
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
	_navList = CreateList(dialog, box, 1, 0, 0, 0);
	PopulateNavList(_navList);

	// Init track list
	GetDialogItem(dialog, 2, &type, &itemH, &box);
	_trackList = CreateList(dialog, box, 1, 0, 0, 0);

	//MacFillRect(&dialog->portRect, &qd.black);
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
		LSetCell(item.c_str(), item.size(), cell, list);
		rowNum = rowNum + 1;
	}
}

void SpotifyRequest(string uri, function<void(MacWifiResponse)> onComplete)
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
					// Expired access token, so refresh it and try again
					RefreshAccessToken(uri, onComplete);
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

					SpotifyRequest(uri, onComplete);
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