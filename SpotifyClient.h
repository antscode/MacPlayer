#include <string>
#include <map>
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
		PicHandle ActiveTrackImage;
		SpotifyClient(MacWifiLib* wifiLib, Prefs* prefs);
		void Login(function<void(LoginResponse)> onComplete);
		void GetRecentTracks(function<void(JsonValue&)> onComplete);
		void PlayTrack(const string& contextUri, const string& trackUri, function<void(JsonValue&)> onComplete);
		void NextTrack(function<void(JsonValue&)> onComplete);
		void PreviousTrack(function<void(JsonValue&)> onComplete);
		void GetDevices(function<void(JsonValue&)> onComplete);
		void GetPlaylists(function<void(JsonValue&)> onComplete);
		void GetPlaylistTracks(const string& playlistId, function<void(JsonValue&)> onComplete);
		void ActivateDevice(const string& deviceId, function<void(JsonValue&)> onComplete);
		void GetImage(const string& image, const string& albumId, function<void(PicHandle)> onComplete);
		void GetPlayerState(function<void(JsonValue&)> onComplete);
		void Play(function<void(JsonValue&)> onComplete);
		void Pause(function<void(JsonValue&)> onComplete);
		void HandleError(const string& errorMsg);

	private:
		class SpotifyRequest
		{
			public:
				string Uri;
				string Content;
				function<void(JsonValue&)> OnComplete;
				function<void(int)> DoRequest;
				bool Refreshed;
				bool Debug;
		};

		const string _permissions =
			"user-read-playback-state "
			"user-modify-playback-state "
			"user-read-recently-played "
			"playlist-read-private "
			"playlist-read-collaborative";

		MacWifiLib* _wifiLib;
		Prefs* _prefs;
		JsonValue _root;	
		short _cacheVRefNum;
		long _cacheDirId;
		map<int, SpotifyRequest> _requests;
		int _requestId = 0;
		char _imageBuffer[20000];
		char* _imagePtr;

		void Get(string uri, bool debug, function<void(JsonValue&)> onComplete);
		void Post(string uri, bool debug, string content, function<void(JsonValue&)> onComplete);
		void Put(string uri, bool debug, string content, function<void(JsonValue&)> onComplete);
		void Request(
			string uri,
			string content,
			bool debug,
			function<void(JsonValue&)> onComplete,
			function<void(int)> doRequest,
			bool refreshed);

		void DoGet(int requestId);
		void DoPost(int requestId);
		void DoPut(int requestId);
		void HandleResponse(MacWifiResponse& response);
		void RefreshAccessToken(int requestId);
		void InitCache();
		bool SaveImage(const FSSpec* imageSpec, long size);
};

#endif

