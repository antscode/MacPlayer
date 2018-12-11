cmake_minimum_required(VERSION 2.8)

# See: https://gcc.gnu.org/onlinedocs/gcc/Optimize-Options.html
SET(CMAKE_CXX_FLAGS "-Ofast")

add_application(MacPlayer
	MacPlayer.cpp
	resources.r
	Util.cpp
	Prefs.cpp
	Keys.cpp
	MacAuth.r
	base64.cpp
	CDEF/DarkListDef.cpp
	CDEF/DarkScrollbarDef.cpp
	CDEF/DarkSliderDef.cpp
	CDEF/colorCDEF.cpp
	CDEF/grayCDEF.cpp
	CDEF/miscCDEF.cpp
	CDEF/qdCDEF.cpp
	SpotifyClient.cpp

	CREATOR "MPLY"
)

target_link_libraries(MacPlayer MacWifiLib MacAuth gason)

set_target_properties(MacPlayer PROPERTIES COMPILE_OPTIONS -ffunction-sections)
set_target_properties(MacPlayer PROPERTIES LINK_FLAGS "-Wl,-gc-sections")
