#include <vector>
#include <MacAuth/MacAuth.h>
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
}

void SpotifyClient::GetRecentTracks(function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/me/player/recently-played?limit=10", 
		onComplete);
}

void SpotifyClient::PlayTrack(const string& contextUri, const string& trackUri, function<void(JsonValue&)> onComplete)
{
	Put(
		"https://api.spotify.com/v1/me/player/play",
		"{ \"context_uri\": \"" + contextUri + "\", \"offset\": { \"uri\": \"" + trackUri + "\" } }",
		onComplete);
}

void SpotifyClient::NextTrack(function<void(JsonValue&)> onComplete)
{
	Post(
		"https://api.spotify.com/v1/me/player/next",
		"",
		onComplete);
}

void SpotifyClient::PreviousTrack(function<void(JsonValue&)> onComplete)
{
	Post(
		"https://api.spotify.com/v1/me/player/previous",
		"",
		onComplete);
}

void SpotifyClient::GetDevices(function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/me/player/devices",
		onComplete);
}

void SpotifyClient::GetPlaylists(function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/me/playlists",
		onComplete);
}

void SpotifyClient::GetPlaylistTracks(const string& playlistId, function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/playlists/" + playlistId + "/tracks?"
		"limit=20&"
		"fields=items(track(name,uri,album(images(url)),artists(name)))",
		onComplete);
}

void SpotifyClient::GetImage(const string& image, function<void(PicHandle)> onComplete)
{
	_wifiLib->Utf8ToMacRoman(false);
	_wifiLib->Get(
		"https://68k.io/image?ma_client_id=" + Keys::ClientId +
		"&source_url=" + image +
		"&dest_width=250&dest_height=250",
		[=](MacWifiResponse& response)
	{
		if (response.Success)
		{
			_wifiLib->Utf8ToMacRoman(true);
			vector<char> v(response.Content.begin(), response.Content.end());
			char* pict = &v[512]; // Skip 512-byte PICT1 header

			PicHandle imageHandle = (PicHandle)&pict;
			onComplete(imageHandle);
		}
		else
		{
			// Something went wrong, return null
			onComplete(nil);
		}
	});
}

void SpotifyClient::GetCurrentlyPlaying(function<void(JsonValue&)> onComplete)
{
	Get(
		"https://api.spotify.com/v1/me/player",
		onComplete);
}

void SpotifyClient::Pause(function<void(JsonValue&)> onComplete)
{
	Put(
		"https://api.spotify.com/v1/me/player/pause",
		"",
		onComplete);
}

void SpotifyClient::Get(string uri, function<void(JsonValue&)> onComplete)
{
	Request(uri, "", onComplete, bind(&SpotifyClient::DoGet, this) , false);
}

void SpotifyClient::Post(string uri, string content, function<void(JsonValue&)> onComplete)
{
	Request(uri, content, onComplete, bind(&SpotifyClient::DoPost, this), false);
}

void SpotifyClient::Put(string uri, string content, function<void(JsonValue&)> onComplete)
{
	Request(uri, content, onComplete, bind(&SpotifyClient::DoPut, this), false);
}

void SpotifyClient::Request(
	string uri, 
	string content, 
	function<void(JsonValue&)> onComplete,
	function<void()> doRequest, 
	bool refreshed)
{
	_uri = uri;
	_content = content;
	_onComplete = onComplete; 
	_doRequest = doRequest;
	_refreshed = refreshed;
	_wifiLib->SetAuthorization("Bearer " + AccessToken);

	doRequest();
}

void SpotifyClient::DoGet()
{
	_wifiLib->Get(_uri, bind(&SpotifyClient::HandleResponse, this, _1));
}

void SpotifyClient::DoPost()
{
	_wifiLib->Post(_uri, _content, bind(&SpotifyClient::HandleResponse, this, _1));
}

void SpotifyClient::DoPut()
{
	_wifiLib->Put(_uri, _content, bind(&SpotifyClient::HandleResponse, this, _1));
}

void SpotifyClient::HandleResponse(MacWifiResponse& response)
{
	if (response.Success)
	{
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
					_onComplete(_root);
				}
				else
				{
					HandleError(_uri + ": Error parsing response.");
				}
			}
			else
			{
				// No content
				JsonValue nullValue = JsonValue();
				_onComplete(nullValue);
			}
		}
		else if (response.StatusCode == 401)
		{
			if (!_refreshed)
			{
				// Expired access token, so refresh it and try again
				RefreshAccessToken();
			}
			else
			{
				// Authentication still failing after a refresh, give up
				HandleError(_uri + ": Authentication error.");
			}
		}
		else
		{
			HandleError(_uri + ": HTTP " + to_string(response.StatusCode) + " response.");
		}
	}
	else
	{
		HandleError(_uri + ": " + response.ErrorMsg);
	}
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

void SpotifyClient::RefreshAccessToken()
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
				_doRequest();
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