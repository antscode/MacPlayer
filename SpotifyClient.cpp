#include <vector>
#include <MacAuth/MacAuth.h>
#include <Folders.h>
#include <Script.h>
#include "SpotifyClient.h"
#include "Keys.h"
#include "base64.h"
#include "Util.h"

SpotifyClient::SpotifyClient(MacWifiLib* wifiLib, Prefs* prefs)
{
	_wifiLib = wifiLib;
	_prefs = prefs;

	AccessToken = string(_prefs->Data.AccessToken);
	RefreshToken = string(_prefs->Data.RefreshToken);
	InitCache();
}

void SpotifyClient::GetRecentTracks(function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/me/player/recently-played?limit=10", 
		false,
		onComplete);
}

void SpotifyClient::PlayTrack(const string& contextUri, const string& trackUri, function<void(JsonValue&)> onComplete)
{
	Put(
		"https://api.spotify.com/v1/me/player/play", 
		false,
		"{ \"context_uri\": \"" + contextUri + "\", \"offset\": { \"uri\": \"" + trackUri + "\" } }",
		onComplete);
}

void SpotifyClient::NextTrack(function<void(JsonValue&)> onComplete)
{
	Post(
		"https://api.spotify.com/v1/me/player/next",
		false,
		"",
		onComplete);
}

void SpotifyClient::PreviousTrack(function<void(JsonValue&)> onComplete)
{
	Post(
		"https://api.spotify.com/v1/me/player/previous", 
		false,
		"",
		onComplete);
}

void SpotifyClient::GetDevices(function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/me/player/devices",
		false,
		onComplete);
}

void SpotifyClient::GetPlaylists(function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/me/playlists",
		false,
		onComplete);
}

void SpotifyClient::GetPlaylistTracks(const string& playlistId, function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/playlists/" + playlistId + "/tracks?"
		"limit=20&"
		"fields=items(track(name,uri,album(id,images(url)),artists(name)))",
		false,
		onComplete);
}

void SpotifyClient::GetImage(const string& image, const string& albumId, function<void(PicHandle)> onComplete)
{
	FSSpec imageSpec;
	OSErr err;
	short file;

	// Check cache for existing image
	// We use album ID because the GUID image ID is too long for the Mac filesystem
	unsigned char* filename = Util::StrToPStr(albumId);
	FSMakeFSSpec(_cacheVRefNum, _cacheDirId, filename, &imageSpec);
	err = FSpOpenDF(&imageSpec, fsRdPerm, &file);

	if (err == noErr) {
		// Cached image exists, use it
		long size;
		GetEOF(file, &size);

		Ptr buffer = NewPtr(size);
		err = FSRead(file, &size, buffer);

		if (err == noErr || err == eofErr)
		{
			char* pict = &buffer[512]; // Skip 512-byte PICT1 header
			PicHandle imageHandle = (PicHandle)&pict;
			onComplete(imageHandle);
			DisposeHandle((Handle)imageHandle);
		}
		else
		{
			// Something went wrong, return null
			onComplete(nil);
		}

		FSClose(file);
		DisposePtr(buffer);
	}
	else
	{
		// Download image
		_wifiLib->Utf8ToMacRoman(false);
		_wifiLib->Get(
			"https://68k.io/image?ma_client_id=" + Keys::ClientId +
			"&source_url=" + image +
			"&dest_width=250&dest_height=250",
			[=](MacWifiResponse& response)
			{
				_wifiLib->Utf8ToMacRoman(true);
				
				if (response.Success)
				{
					vector<char> v(response.Content.begin(), response.Content.end());

					// Save image to cache
					SaveImage(&imageSpec, v);

					char* pict = &v[512]; // Skip 512-byte PICT1 header
					PicHandle imageHandle = (PicHandle)&pict;
					onComplete(imageHandle);
					DisposeHandle((Handle)imageHandle);
				}
				else
				{
					// Something went wrong, return null
					onComplete(nil);
				}
			});
	}
}

void SpotifyClient::GetPlayerState(function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/me/player",
		false,
		onComplete);
}

void SpotifyClient::Play(function<void(JsonValue&)> onComplete)
{
	Put(
		"https://api.spotify.com/v1/me/player/play",
		false,
		"",
		onComplete);
}

void SpotifyClient::Pause(function<void(JsonValue&)> onComplete)
{
	Put(
		"https://api.spotify.com/v1/me/player/pause",
		false,
		"",
		onComplete);
}

void SpotifyClient::Get(string uri, bool debug, function<void(JsonValue&)> onComplete)
{
	Request(uri, "", debug, onComplete, bind(&SpotifyClient::DoGet, this, _1) , false);
}

void SpotifyClient::Post(string uri, bool debug, string content, function<void(JsonValue&)> onComplete)
{
	Request(uri, content, debug, onComplete, bind(&SpotifyClient::DoPost, this, _1), false);
}

void SpotifyClient::Put(string uri, bool debug, string content, function<void(JsonValue&)> onComplete)
{
	Request(uri, content, debug, onComplete, bind(&SpotifyClient::DoPut, this, _1), false);
}

void SpotifyClient::Request(
	string uri, 
	string content, 
	bool debug,
	function<void(JsonValue&)> onComplete,
	function<void(int)> doRequest, 
	bool refreshed)
{
	SpotifyRequest request;

	request.Uri = uri;
	request.Content = content;
	request.OnComplete = onComplete;
	request.DoRequest = doRequest;
	request.Refreshed = false;
	request.Debug = debug;

	_requestId++;
	_requests.insert(pair<int, SpotifyRequest>(_requestId, request));
	_wifiLib->SetAuthorization("Bearer " + AccessToken);

	doRequest(_requestId);
}

void SpotifyClient::DoGet(int requestId)
{
	SpotifyRequest request = _requests[requestId];
	_wifiLib->Get(request.Uri, bind(&SpotifyClient::HandleResponse, this, _1), requestId);
}

void SpotifyClient::DoPost(int requestId)
{
	SpotifyRequest request = _requests[requestId];
	_wifiLib->Post(request.Uri, request.Content, bind(&SpotifyClient::HandleResponse, this, _1), requestId);
}

void SpotifyClient::DoPut(int requestId)
{
	SpotifyRequest request = _requests[requestId];
	_wifiLib->Put(request.Uri, request.Content, bind(&SpotifyClient::HandleResponse, this, _1), requestId);
}

void SpotifyClient::HandleResponse(MacWifiResponse& response)
{
	SpotifyRequest request = _requests[response.Id];

	if (response.Success)
	{
		if (request.Debug)
		{
			Util::Debug(response.Content + "\r\r");
		}

		if (response.StatusCode >= 200 &&
			response.StatusCode <= 299)
		{
			if (response.StatusCode != 204)
			{
				// All good, parse json
				JsonAllocator allocator; 
				JsonParseStatus status = jsonParse((char*)response.Content.c_str(), _root, allocator);

				if (status == JSON_PARSE_OK)
				{
					request.OnComplete(_root);
				}
				else
				{ 
					HandleError(request.Uri + ": Error parsing response.");
				}
			}
			else
			{
				// No content
				JsonValue nullValue = JsonValue();
				request.OnComplete(nullValue);
			}
		}
		else if (response.StatusCode == 401)
		{
			if (!request.Refreshed)
			{
				// Expired access token, so refresh it and try again
				RefreshAccessToken(response.Id);
				return;
			}
			else
			{
				// Authentication still failing after a refresh, give up
				HandleError(request.Uri + ": Authentication error.");
			}
		}
		else
		{
			HandleError(request.Uri + ": HTTP " + to_string(response.StatusCode) + " response.");
		}
	}
	else
	{
		HandleError(request.Uri + ": " + response.ErrorMsg);
	}

	_requests.erase(response.Id);
}

void SpotifyClient::Login(function<void(LoginResponse)> onComplete)
{
	MacAuth macAuth(_wifiLib);
	AuthRequest authRequest;

	authRequest.ClientId = Keys::ClientId;
	authRequest.Provider = "spotify";

	authRequest.Params.insert(pair<string, string>("client_id", Keys::SpotifyClientId));
	authRequest.Params.insert(pair<string, string>("response_type", "code"));
	authRequest.Params.insert(pair<string, string>("scope", MacWifiLib::Encode(_permissions)));

	AuthResponse authResponse = macAuth.Authenticate(authRequest);

	if (authResponse.Success)
	{
		// Get access & refresh tokens
		_wifiLib->Post(
			"https://accounts.spotify.com/api/token",
			"client_id=" + Keys::SpotifyClientId +
			"&client_secret=" + Keys::SpotifyClientSecret +
			"&grant_type=authorization_code&code=" + authResponse.Code +
			"&redirect_uri=https://68k.io/login/callback",
			[=](MacWifiResponse response)
			{
				LoginResponse loginResponse;

				loginResponse.Success = false;
				loginResponse.ErrorMsg = "";

				if (response.Success)
				{
					JsonAllocator allocator;
					JsonValue root;
					JsonParseStatus status = jsonParse((char*)response.Content.c_str(), root, allocator);

					if (status == JSON_PARSE_OK)
					{
						char* accessToken = root("access_token").toString();
						char* refreshToken = root("refresh_token").toString();

						AccessToken = accessToken;
						RefreshToken = refreshToken;

						strcpy(_prefs->Data.AccessToken, accessToken);
						strcpy(_prefs->Data.RefreshToken, refreshToken);
						_prefs->Save();
							
						loginResponse.Success = true;
					}
					else
					{
						HandleError("Login: Error parsing response.");
					}
				}
				else
				{
					HandleError("Login: " + response.ErrorMsg);
				}

				onComplete(loginResponse);
			});
	}
	else
	{
		LoginResponse loginResponse;
		loginResponse.Success = false;
		loginResponse.ErrorMsg = "";

		onComplete(loginResponse);
	}
}

void SpotifyClient::RefreshAccessToken(int requestId)
{
	const std::string authHeader = Keys::SpotifyClientId + ":" + Keys::SpotifyClientSecret;
	std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(authHeader.c_str()), authHeader.length());

	_wifiLib->SetAuthorization("Basic " + encoded);
	_wifiLib->Post(
		"https://accounts.spotify.com/api/token",
		"grant_type=refresh_token&refresh_token=" + RefreshToken,
		[=](MacWifiResponse response)
	{
		if (response.Success)
		{
			JsonAllocator allocator;
			JsonValue root;
			JsonParseStatus status = jsonParse((char*)response.Content.c_str(), root, allocator);

			if (status == JSON_PARSE_OK)
			{
				char* accessToken = root("access_token").toString();

				AccessToken = accessToken;

				strcpy(_prefs->Data.AccessToken, accessToken);
				_prefs->Save();

				_wifiLib->SetAuthorization("Bearer " + AccessToken);

				SpotifyRequest request = _requests[requestId];
				request.DoRequest(requestId);
			}
			else
			{
				HandleError("RefreshAccessToken: Error parsing response.");
			}
		}
		else
		{
			HandleError("RefreshAccessToken: " + response.ErrorMsg);
		}
	});
}

void SpotifyClient::HandleError(const string& errorMsg)
{
	ParamText(Util::StrToPStr(errorMsg), nil, nil, nil);
	StopAlert(131, nil);

	// Reset any wait cursor
	InitCursor();
}

void SpotifyClient::InitCache()
{
	FSSpec dirSpec;
	FInfo dirInfo;
	OSErr err;
	long dirID;

	// Create cache folder if it doesn't exist
	Str255 dirName = "\pMacPlayer Cache";

	if (FindFolder(kOnSystemDisk, kPreferencesFolderType, kCreateFolder, &_cacheVRefNum, &dirID) == noErr) {
		FSMakeFSSpec(_cacheVRefNum, dirID, dirName, &dirSpec);
		Util::GetDirectoryId(_cacheVRefNum, dirID, dirName, &_cacheDirId);

		if (_cacheDirId == 0)
		{
			err = FSpDirCreate(&dirSpec, smSystemScript, &_cacheDirId);

			if (err != noErr)
			{
				HandleError("Could not create cache directory due to error " + to_string(err));
			}
		}
	}
	else
	{
		HandleError("Could not find Preferences folder.");
	}
}

bool SpotifyClient::SaveImage(const FSSpec* imageSpec, const vector<char>& content)
{
	OSErr err;
	short file;

	if ((err = FSpCreate(imageSpec, 'MPLY', 'PICT', 0)) == noErr)
		err = FSpOpenDF(imageSpec, fsRdWrPerm, &file);

	if (err == noErr) 
	{
		if (SetEOF(file, 0L) == noErr) 
		{
			long size = content.size();
			const char* ca = &content[0];

			if (FSWrite(file, &size, ca) == noErr) {
				FSClose(file);
				return true;
			}

			FSClose(file);
		}
	}
	
	return false;
}