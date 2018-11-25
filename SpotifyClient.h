#include <string>
#include <MacWifi/MacWifiLib.h>
#include "Prefs.h"

#ifndef __SPOTIFY_CLIENT__
#define __SPOTIFY_CLIENT__

using namespace std;

struct LoginResponse
{
	bool Success;
	string ErrorMsg;
};

class SpotifyClient
{
	public:
		SpotifyClient(MacWifiLib* wifiLib, Prefs* prefs);
		void Login(function<void(LoginResponse)> onComplete);
		void Get(string uri, function<void(MacWifiResponse)> onComplete);
		void Post(string uri, string content, function<void(MacWifiResponse)> onComplete);
		void Put(string uri, string content, function<void(MacWifiResponse)> onComplete);
		string AccessToken, RefreshToken;

	private:
		MacWifiLib* _wifiLib;
		Prefs* _prefs;

		function<void(MacWifiResponse)> _onComplete;
		function<void()> _doRequest;
		string _uri, _content;
		bool _refreshed;

		void Request(
			string uri,
			string content,
			function<void(MacWifiResponse)> onComplete,
			function<void()> doRequest,
			bool refreshed);

		void DoGet();
		void DoPost();
		void DoPut();
		void HandleResponse(MacWifiResponse response);
		void RefreshAccessToken();
};

#endif

