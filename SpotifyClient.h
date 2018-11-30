#include <string>
#include <MacWifi/MacWifiLib.h>
#include <gason/gason.hpp>
#include "Prefs.h"

#ifndef __SPOTIFY_CLIENT__
#define __SPOTIFY_CLIENT__

using namespace std;
using namespace std::placeholders;
using namespace gason;

struct LoginResponse
{
	bool Success;
	string ErrorMsg;
};

class SpotifyClient
{
	public:
		string AccessToken, RefreshToken;
		SpotifyClient(MacWifiLib* wifiLib, Prefs* prefs);
		void Login(function<void(LoginResponse)> onComplete);
		void GetRecentTracks(function<void(JsonValue&)> onComplete);
		void PlayTrack(const string& trackUri, function<void(JsonValue&)> onComplete);
		void NextTrack(function<void(JsonValue&)> onComplete);
		void PreviousTrack(function<void(JsonValue&)> onComplete);
		void GetDevices(function<void(JsonValue&)> onComplete);
		void GetPlaylists(function<void(JsonValue&)> onComplete);
		void GetPlaylistTracks(const string& playlistId, function<void(JsonValue&)> onComplete);
		void GetImage(const string& image, function<void(PicHandle)> onComplete);

	private:
		const string _permissions =
			"user-read-playback-state "
			"user-modify-playback-state "
			"user-read-recently-played "
			"playlist-read-private "
			"playlist-read-collaborative";

		MacWifiLib* _wifiLib;
		Prefs* _prefs;
		JsonValue _root;
		function<void(JsonValue&)> _onComplete;
		function<void()> _doRequest;
		string _uri, _content;
		bool _refreshed;

		void Get(string uri, function<void(JsonValue&)> onComplete);
		void Post(string uri, string content, function<void(JsonValue&)> onComplete);
		void Put(string uri, string content, function<void(JsonValue&)> onComplete);
		void Request(
			string uri,
			string content,
			function<void(JsonValue&)> onComplete,
			function<void()> doRequest,
			bool refreshed);

		void DoGet();
		void DoPost();
		void DoPut();
		void HandleResponse(MacWifiResponse& response);
		void RefreshAccessToken();
};

#endif

