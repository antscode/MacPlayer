#include <vector>
#include <MacAuth/MacAuth.h>
#include <gason/gason.hpp>
#include "SpotifyClient.h"
#include "Keys.h"
#include "base64.h"

using namespace gason;

SpotifyClient::SpotifyClient(MacWifiLib* wifiLib, Prefs* prefs)
{
	_wifiLib = wifiLib;
	_prefs = prefs;

	AccessToken = string(_prefs->Data.AccessToken);
	RefreshToken = string(_prefs->Data.RefreshToken);
}

void SpotifyClient::Get(string uri, function<void(MacWifiResponse)> onComplete)
{
	Request(uri, "", onComplete, bind(&SpotifyClient::DoGet, this) , false);
}

void SpotifyClient::Post(string uri, string content, function<void(MacWifiResponse)> onComplete)
{
	Request(uri, content, onComplete, bind(&SpotifyClient::DoPost, this), false);
}

void SpotifyClient::Put(string uri, string content, function<void(MacWifiResponse)> onComplete)
{
	Request(uri, content, onComplete, bind(&SpotifyClient::DoPut, this), false);
}

void SpotifyClient::Request(
	string uri, 
	string content, 
	function<void(MacWifiResponse)> onComplete, 
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
	_wifiLib->Get(_uri, _onComplete);
}

void SpotifyClient::DoPost()
{
	_wifiLib->Post(_uri, _content, _onComplete);
}

void SpotifyClient::DoPut()
{
	_wifiLib->Put(_uri, _content, _onComplete);
}

void SpotifyClient::HandleResponse(MacWifiResponse response)
{
	if (response.Success)
	{
		if (response.StatusCode == 200)
		{
			// All good, trigger callback
			_onComplete(response);
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
}

void SpotifyClient::Login(function<void(LoginResponse)> onComplete)
{
	MacAuth macAuth(_wifiLib);
	AuthRequest authRequest;

	authRequest.ClientId = Keys::ClientId;
	authRequest.Provider = "spotify";

	authRequest.Params.insert(pair<string, string>("client_id", Keys::SpotifyClientId));
	authRequest.Params.insert(pair<string, string>("response_type", "code"));
	authRequest.Params.insert(pair<string, string>("scope", MacWifiLib::Encode("user-read-playback-state user-read-recently-played")));

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
				}
				else
				{
					// TODO
				}

				onComplete(loginResponse);
			});
	}
	else
	{
		// TODO
		//onComplete(response);
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
				// TODO
			}
		}
		else
		{
			// TODO
		}
	});
}