cmake_minimum_required(VERSION 2.8)

add_application(MacPlayer
	MacPlayer.cpp
	resources.r
	Util.cpp
	Prefs.cpp
	Keys.cpp
	MacAuth.r
	base64.cpp
	DarkListDef.cpp
	DarkScrollbarDef.cpp
	gason.cpp

	CREATOR "MPLY"
)

target_link_libraries(MacPlayer MacWifiLib MacAuth jsoncpp)

set_target_properties(MacPlayer PROPERTIES COMPILE_OPTIONS -ffunction-sections)
set_target_properties(MacPlayer PROPERTIES LINK_FLAGS "-Wl,-gc-sections")
