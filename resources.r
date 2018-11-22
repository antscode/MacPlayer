#include "Finder.r"
#include "MacTypes.r"
#include "Processes.r"
#include "Dialogs.r"
#include "Menus.r"
#include "Controls.r"

type 'MPLY' as 'STR ';
resource 'MPLY' (0, purgeable) {
   "1.0, 2018 Anthony Super."
};

resource 'SIZE' (-1) {
	dontSaveScreen,
	acceptSuspendResumeEvents,
	enableOptionSwitch,
	canBackground,
	multiFinderAware,
	backgroundAndForeground,
	dontGetFrontClicks,
	ignoreChildDiedEvents,
	is32BitCompatible,
	isHighLevelEventAware,
	onlyLocalHLEvents,
	notStationeryAware,
	reserved,
	reserved,
	reserved,
	reserved,
	3000 * 1024,
	3000 * 1024
};

resource 'MBAR' (128, preload) {
	{ 128, 129, 130 };
};

resource 'MENU' (128, preload) {
	128, textMenuProc,
	0b1111111111111111111111111111101,
	enabled, apple,
	{
		"About MacPlayer\311",
			noicon, nokey, nomark, plain;
		"-",
			noicon, nokey, nomark, plain
	}
};

resource 'MENU' (129) {
    129, textMenuProc;
    allEnabled, enabled;
    "File";
    {
        "Quit", noIcon, noKey, noMark, plain;
    }
};

resource 'MENU' (130) {
    130, textMenuProc;
    allEnabled, enabled;
    "Device";
    {

    }
};

resource 'DLOG' (128, purgeable) {
	{ 0, 0, 296, 505 },
	noGrowDocProc,
	visible,
	goAway,
	0,
	128,
	"MacPlayer",
	centerMainScreen
};


data 'dctb' (128, purgeable) {
	$"FFFF 0000 0000 0000 0000 0000 0000 0000"
};

// 10-byte code resource stub trick
data 'LDEF' (128) {
    $"2F3A 0004 4E75 0000 0000"
}; 

/* Nasty hack to replace all scroll bars with our own definition - but it works! */
data 'CDEF' (1) {
    $"2F3A 0004 4E75 0000 0000"
}; 

resource 'DITL' (128, purgeable) {
	{ 
		/* Spotify Logo */
		{ 60, 183, 60+41, 183+138 },
		Picture { enabled, 136 };

		/* Login */
		{ 120, 217, 120+20, 217+70},
		Button { enabled, "Login" };
	}
};

resource 'DITL' (129, purgeable) {
	{ 
		/* Nav List */
		{ 10, 15, 200+10, 100+10 },
		UserItem { enabled };

		/* Track List */
		{ 10, 120, 200+10, 505 },
		UserItem { enabled };

		/* Play */
		{ 254, 237, 286, 269 },
		Icon { enabled, 128 };

		/* Previous Track */
		{ 262, 201, 278, 217 },
		Icon { enabled, 130 };

		/* Next Track */
		{ 262, 289, 278, 305 },
		Icon { enabled, 129 };

		/* Shuffle */
		{ 262, 155, 278, 171 },
		Icon { enabled, 131 };

		/* Repeat */
		{ 262, 335, 278, 351 },
		Icon { enabled, 132 };
	}
};

data 'cicn' (128, "play", purgeable) {
	$"0000 0000 8004 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0001 0001 0001 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFF0 0FFF FF8F F1FF FE7F FE7F FDFF"            /* .......è........ */
	$"FFBF FBFF FFDF F7FF FFEF EFFF FFF7 DFFF"            /* .ø.............. */
	$"FFFB DFFF FFFB BFFF FFFD BFFF FFFD BFF7"            /* ......ø...ø...ø. */
	$"FFFD 7FF1 FFFE 7FF0 7FFE 7FF0 1FFE 7FF0"            /* ................ */
	$"07FE 7FF0 03FE 7FF0 07FE 7FF0 1FFE 7FF0"            /* ................ */
	$"7FFE BFF1 FFFD BFF7 FFFD BFFF FFFD DFFF"            /* ..ø...ø...ø..... */
	$"FFFB DFFF FFFB EFFF FFF7 F7FF FFEF FBFF"            /* ................ */
	$"FFDF FDFF FFBF FE7F FE7F FF8F F1FF FFF0"            /* .....ø.....è.... */
	$"0FFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFF0 0FFF FF8F"            /* ...............è */
	$"F1FF FE7F FE7F FDFF FFBF FBFF FFDF F7FF"            /* .........ø...... */
	$"FFEF EFFF FFF7 DFFF FFFB DFFF FFFB BFFF"            /* ..............ø. */
	$"FFFD BFFF FFFD BFF7 FFFD 7FF1 FFFE 7FF0"            /* ..ø...ø......... */
	$"7FFE 7FF0 1FFE 7FF0 07FE 7FF0 03FE 7FF0"            /* ................ */
	$"07FE 7FF0 1FFE 7FF0 7FFE BFF1 FFFD BFF7"            /* ..........ø...ø. */
	$"FFFD BFFF FFFD DFFF FFFB DFFF FFFB EFFF"            /* ..ø............. */
	$"FFF7 F7FF FFEF FBFF FFDF FDFF FFBF FE7F"            /* .............ø.. */
	$"FE7F FF8F F1FF FFF0 0FFF"                           /* ...è...... */
};

data 'cicn' (129, "next", purgeable) {
	$"0000 0000 8004 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0001 0001 0001 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FCFF FC0F FCFF"            /* ................ */
	$"FC0F FC0F FC0F FC0F FC0F FC00 FC0F FC00"            /* ................ */
	$"FC0F FC00 0C0F FC00 0C0F FC00 000F FC00"            /* ................ */
	$"000F FC00 000F FC00 000F FC00 0C0F FC00"            /* ................ */
	$"0C0F FC00 FC0F FC00 FC0F FC0F FC0F FC0F"            /* ................ */
	$"FC0F FCFF FC0F FCFF FC0F FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FCFF FC0F FCFF FC0F FC0F FC0F FC0F"            /* ................ */
	$"FC0F FC00 FC0F FC00 FC0F FC00 0C0F FC00"            /* ................ */
	$"0C0F FC00 000F FC00 000F FC00 000F FC00"            /* ................ */
	$"000F FC00 0C0F FC00 0C0F FC00 FC0F FC00"            /* ................ */
	$"FC0F FC0F FC0F FC0F FC0F FCFF FC0F FCFF"            /* ................ */
	$"FC0F FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF"                           /* .......... */
};

data 'cicn' (130, "prev", purgeable) {
	$"0000 0000 8004 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0001 0001 0001 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF F03F FF3F F03F"            /* ...........?.?.? */
	$"FF3F F03F F03F F03F F03F F03F 003F F03F"            /* .?.?.?.?.?.?.?.? */
	$"003F F030 003F F030 003F F000 003F F000"            /* .?.0.?.0.?...?.. */
	$"003F F000 003F F000 003F F030 003F F030"            /* .?...?...?.0.?.0 */
	$"003F F03F 003F F03F 003F F03F F03F F03F"            /* .?.?.?.?.?.?.?.? */
	$"F03F F03F FF3F F03F FF3F FFFF FFFF FFFF"            /* .?.?.?.?.?...... */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF F03F FF3F F03F FF3F F03F F03F F03F"            /* ...?.?.?.?.?.?.? */
	$"F03F F03F 003F F03F 003F F030 003F F030"            /* .?.?.?.?.?.0.?.0 */
	$"003F F000 003F F000 003F F000 003F F000"            /* .?...?...?...?.. */
	$"003F F030 003F F030 003F F03F 003F F03F"            /* .?.0.?.0.?.?.?.? */
	$"003F F03F F03F F03F F03F F03F FF3F F03F"            /* .?.?.?.?.?.?.?.? */
	$"FF3F FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* .?.............. */
	$"FFFF FFFF FFFF FFFF FFFF"                           /* .......... */
};

data 'cicn' (131, "shuffle", purgeable) {
	$"0000 0000 8004 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0001 0001 0001 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FF3F FFFF FF3F FFFF FF0F FFFF"            /* .....?...?...... */
	$"FF0F C00F F003 C00F F003 FFC3 C30F FFC3"            /* ..¿...¿....√√..√ */
	$"C30F FFFF 0F3F FFFF 0F3F FFFC 3FFF FFFC"            /* √....?...?..?... */
	$"3FFF FFFC 3FFF FFFC 3FFF FFF0 FF3F FFF0"            /* ?...?...?....?.. */
	$"FF3F FFC3 C30F FFC3 C30F C00F F003 C00F"            /* .?.√√..√√.¿...¿. */
	$"F003 FFFF FF0F FFFF FF0F FFFF FF3F FFFF"            /* .............?.. */
	$"FF3F FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* .?.............. */
	$"FFFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FF3F FFFF"            /* .............?.. */
	$"FF3F FFFF FF0F FFFF FF0F C00F F003 C00F"            /* .?........¿...¿. */
	$"F003 FFC3 C30F FFC3 C30F FFFF 0F3F FFFF"            /* ...√√..√√....?.. */
	$"0F3F FFFC 3FFF FFFC 3FFF FFFC 3FFF FFFC"            /* .?..?...?...?... */
	$"3FFF FFF0 FF3F FFF0 FF3F FFC3 C30F FFC3"            /* ?....?...?.√√..√ */
	$"C30F C00F F003 C00F F003 FFFF FF0F FFFF"            /* √.¿...¿......... */
	$"FF0F FFFF FF3F FFFF FF3F FFFF FFFF FFFF"            /* .....?...?...... */
	$"FFFF FFFF FFFF FFFF FFFF"                           /* .......... */
};

data 'cicn' (132, "repeat", purgeable) {
	$"0000 0000 8004 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0001 0001 0001 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FCFF FFFF FCFF FFFF FC3F FFFF"            /* .............?.. */
	$"FC3F FC00 000F FC00 000F F0FF FC3F F0FF"            /* .?...........?.. */
	$"FC3F C3FF FCFF C3FF FCFF CFFF FFF3 CFFF"            /* .?√...√...œ...œ. */
	$"FFF3 CFFF FFF3 CFFF FFF3 FF3F FFC3 FF3F"            /* ..œ...œ....?.√.? */
	$"FFC3 FC3F FF0F FC3F FF0F F000 003F F000"            /* .√.?...?.....?.. */
	$"003F FC3F FFFF FC3F FFFF FF3F FFFF FF3F"            /* .?.?...?...?...? */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FCFF FFFF"            /* ................ */
	$"FCFF FFFF FC3F FFFF FC3F FC00 000F FC00"            /* .....?...?...... */
	$"000F F0FF FC3F F0FF FC3F C3FF FCFF C3FF"            /* .....?...?√...√. */
	$"FCFF CFFF FFF3 CFFF FFF3 CFFF FFF3 CFFF"            /* ..œ...œ...œ...œ. */
	$"FFF3 FF3F FFC3 FF3F FFC3 FC3F FF0F FC3F"            /* ...?.√.?.√.?...? */
	$"FF0F F000 003F F000 003F FC3F FFFF FC3F"            /* .....?...?.?...? */
	$"FFFF FF3F FFFF FF3F FFFF FFFF FFFF FFFF"            /* ...?...?........ */
	$"FFFF FFFF FFFF FFFF FFFF"                           /* .......... */
};

data 'cicn' (133, "vol", purgeable) {
	$"0000 0000 8004 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0001 0001 0001 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF 3FFF FFFF"            /* ............?... */
	$"3FFF FFF0 3FCF FFF0 3FCF F00F 33CF F00F"            /* ?...?œ..?œ..3œ.. */
	$"33CF F3FF 3CF3 F3FF 3CF3 F3FF 3CF3 F3FF"            /* 3œ..<...<...<... */
	$"3CF3 F3FF 3CF3 F3FF 3CF3 F3FF 3CF3 F3FF"            /* <...<...<...<... */
	$"3CF3 F00F 33CF F00F 33CF FFF0 3FCF FFF0"            /* <...3œ..3œ..?œ.. */
	$"3FCF FFFF 3FFF FFFF 3FFF FFFF FFFF FFFF"            /* ?œ..?...?....... */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF 3FFF FFFF 3FFF FFF0 3FCF FFF0"            /* ....?...?...?œ.. */
	$"3FCF F00F 33CF F00F 33CF F3FF 3CF3 F3FF"            /* ?œ..3œ..3œ..<... */
	$"3CF3 F3FF 3CF3 F3FF 3CF3 F3FF 3CF3 F3FF"            /* <...<...<...<... */
	$"3CF3 F3FF 3CF3 F3FF 3CF3 F00F 33CF F00F"            /* <...<...<...3œ.. */
	$"33CF FFF0 3FCF FFF0 3FCF FFFF 3FFF FFFF"            /* 3œ..?œ..?œ..?... */
	$"3FFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ?............... */
	$"FFFF FFFF FFFF FFFF FFFF"                           /* .......... */
};

data 'cicn' (134, "tracklist", purgeable) {
	$"0000 0000 8004 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0001 0001 0001 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF F3FF FFFF F3FF FFFF F03F FFFF F03F"            /* ...........?...? */
	$"FFFF F003 C003 F003 C003 F03F FFFF F03F"            /* ....¿...¿..?...? */
	$"FFFF F3FF FFFF F3FF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF F000 0003 F000 0003 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF F000 0003 F000 0003 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF F3FF FFFF F3FF"            /* ................ */
	$"FFFF F03F FFFF F03F FFFF F003 C003 F003"            /* ...?...?....¿... */
	$"C003 F03F FFFF F03F FFFF F3FF FFFF F3FF"            /* ¿..?...?........ */
	$"FFFF FFFF FFFF FFFF FFFF F000 0003 F000"            /* ................ */
	$"0003 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF F000 0003 F000"            /* ................ */
	$"0003 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF"                           /* .......... */
};

data 'cicn' (135, "cast", purgeable) {
	$"0000 0000 8004 0000 0000 0020 0020 0000"            /* ....Ä...... . .. */
	$"0000 0000 0000 0048 0000 0048 0000 0000"            /* .......H...H.... */
	$"0001 0001 0001 0000 0000 0000 0000 0000"            /* ................ */
	$"0000 0000 0000 0004 0000 0000 0020 0020"            /* ............. .  */
	$"0000 0000 0004 0000 0000 0020 0020 0000"            /* ........... . .. */
	$"0000 FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FC03 C00F FC03 C00F F3FF 3FF3 F3FF"            /* ....¿...¿...?... */
	$"3FF3 F3FF 3CF3 F3FF 3CF3 F3FF 3FF3 F3FF"            /* ?...<...<...?... */
	$"3FF3 F3FF 3FF3 F3FF 3FF3 F3FF 3033 F3FF"            /* ?...?...?...03.. */
	$"3033 F3FF 3333 F3FF 3333 FC03 3033 FC03"            /* 03..33..33..03.. */
	$"3033 FFFF 3FF3 FFFF 3FF3 FFFF 3FF3 FFFF"            /* 03..?...?...?... */
	$"3FF3 FF03 C00F FF03 C00F FFFF FFFF FFFF"            /* ?...¿...¿....... */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FC03 C00F FC03"            /* ............¿... */
	$"C00F F3FF 3FF3 F3FF 3FF3 F3FF 3CF3 F3FF"            /* ¿...?...?...<... */
	$"3CF3 F3FF 3FF3 F3FF 3FF3 F3FF 3FF3 F3FF"            /* <...?...?...?... */
	$"3FF3 F3FF 3033 F3FF 3033 F3FF 3333 F3FF"            /* ?...03..03..33.. */
	$"3333 FC03 3033 FC03 3033 FFFF 3FF3 FFFF"            /* 33..03..03..?... */
	$"3FF3 FFFF 3FF3 FFFF 3FF3 FF03 C00F FF03"            /* ?...?...?...¿... */
	$"C00F FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ¿............... */
	$"FFFF FFFF FFFF FFFF FFFF"                           /* .......... */
};

data 'PICT' (136, "spotify") {
	$"031D 0000 0000 0029 008A 1101 A101 F200"            /* .......).ä..°... */
	$"1638 4249 4D00 0000 0000 0000 2900 8A47"            /* .8BIM.......).äG */
	$"7289 7068 AF62 6A01 000A 0000 0000 0029"            /* râphØbj........) */
	$"008A 9800 1400 0000 0000 2900 8A00 0000"            /* .äò.......).ä... */
	$"0000 2900 8A00 0000 0000 2900 8A00 000B"            /* ..).ä.....).ä... */
	$"FFFF 0100 3FF4 FF02 C000 000B 03FF F800"            /* ....?...¿....... */
	$"07F4 FF02 C000 000B 03FF E000 01F4 FF02"            /* ....¿........... */
	$"C000 000C 04FF 8000 007F F5FF 02C0 0000"            /* ¿.....Ä......¿.. */
	$"0C00 FFFE 0000 3FF5 FF02 C000 000C 00FE"            /* ......?...¿..... */
	$"FE00 001F F5FF 02C0 0000 0C00 FCFE 0000"            /* .......¿........ */
	$"0FF5 FF02 C000 000C 00F8 FE00 0007 F5FF"            /* ....¿........... */
	$"02C0 0000 0C00 F0FE 0000 03F5 FF02 C000"            /* .¿............¿. */
	$"000C 00E0 FE00 0001 F5FF 02C0 0000 1000"            /* ...........¿.... */
	$"E0FE 0000 01F9 FF06 E7E0 FFFF C000 0013"            /* ............¿... */
	$"07C0 7FFF E000 FFF0 0FFC FF06 C3C0 FFFF"            /* .¿..........√¿.. */
	$"C000 0014 07C1 FFFF FE00 FFC0 03FD FFFF"            /* ¿....¡.....¿.... */
	$"C305 80FF FFC0 0000 1400 81FE FF03 807F"            /* √.Ä..¿....Å...Ä. */
	$"C003 FDFF 07C3 E787 FFFF C000 0014 0781"            /* ¿....√.á..¿....Å */
	$"FC03 FFE0 7F87 C3FD FF07 C3FF 87FF FFC0"            /* .....á√...√.á..¿ */
	$"0000 15FF 8011 001F F07F 87F7 0E3F F81F"            /* ....Ä.....á..?.. */
	$"0042 003F FFC0 0000 1410 8000 0003 F07F"            /* .B.?.¿....Ä..... */
	$"87FF 0807 E007 0042 001F 86FE 0013 FD00"            /* á......B..Ü..... */
	$"0F70 3F83 FF00 03C0 0300 4200 1F86 8000"            /* .p?É...¿..B..ÜÄ. */
	$"0015 1300 3FFF 8000 3F80 7F00 0380 0100"            /* ....?.Ä.?Ä...Ä.. */
	$"4200 1F0E 4000 0015 1300 FFFF F000 3FC0"            /* B...@.........?¿ */
	$"0F03 8183 C1C3 C387 0F0E 8000 0015 1300"            /* ..ÅÉ¡√√á..Ä..... */
	$"FFFF FE00 3FE0 0707 C107 E0C3 C387 0F1F"            /* ....?...¡..√√á.. */
	$"C000 0015 1300 F001 FF00 3FF8 030F E10F"            /* ¿.........?..... */
	$"F0C3 C387 861F C000 0014 FE00 101F 803F"            /* .√√áÜ.¿.......Ä? */
	$"FF81 0FE1 0FF0 C3C3 8786 1FC0 0000 14FE"            /* .Å....√√áÜ.¿.... */
	$"0010 0780 3FFF E10F E10F F0C3 C387 C63F"            /* ...Ä?......√√á∆? */
	$"C000 0015 1380 00F8 0100 7FDF E10F E10F"            /* ¿....Ä.......... */
	$"F0C3 C387 C03F C000 0015 1380 3FFF 8000"            /* .√√á¿?¿....Ä?.Ä. */
	$"7F8F E107 C187 E1C3 C387 C03F C000 0015"            /* .è..¡á.√√á¿?¿... */
	$"1380 7FFF F000 7F83 C103 8183 C1C3 C387"            /* .Ä.....É¡.ÅÉ¡√√á */
	$"E07F C000 0015 1380 7803 FC00 7F80 0300"            /* ..¿....Äx....Ä.. */
	$"03C0 03C0 4387 E07F C000 0015 13C0 0000"            /* .¿.¿Cá..¿....¿.. */
	$"3E00 FFC0 0700 07E0 07C0 4387 F0FF C000"            /* >..¿.....¿Cá..¿. */
	$"0015 13C0 0000 0E00 FFF0 0F08 0FF8 1FE0"            /* ...¿............ */
	$"4387 F0FF C000 0012 00E0 FE00 0001 FEFF"            /* Cá..¿........... */
	$"000F FBFF 04F1 FFC0 0000 1200 E0FE 0000"            /* .......¿........ */
	$"01FE FF00 0FFB FF04 01FF C000 0012 00F0"            /* ..........¿..... */
	$"FE00 0003 FEFF 000F FBFF 0401 FFC0 0000"            /* .............¿.. */
	$"1300 F8FE 0000 07FE FF00 0FFC FF05 FE03"            /* ................ */
	$"FFC0 0000 0C00 FCFE 0000 0FF5 FF02 C000"            /* .¿............¿. */
	$"000C 00FE FE00 001F F5FF 02C0 0000 0C00"            /* ...........¿.... */
	$"FFFE 0000 3FF5 FF02 C000 000C 04FF 8000"            /* ....?...¿.....Ä. */
	$"007F F5FF 02C0 0000 0B03 FFE0 0001 F4FF"            /* .....¿.......... */
	$"02C0 0000 0B03 FFF8 0007 F4FF 02C0 0000"            /* .¿...........¿.. */
	$"0BFF FF01 003F F4FF 02C0 0000 FF"                   /* .....?...¿... */
};