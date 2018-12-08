#include "Finder.r"
#include "MacTypes.r"
#include "Processes.r"
#include "Dialogs.r"
#include "Menus.r"
#include "Controls.r"

type 'MPLY' as 'STR ';
resource 'MPLY' (0, purgeable) {
   "MacPlayer 1.0, 2018 Anthony Super."
};

resource 'FREF' (128, purgeable) {
   'APPL', 0, ""
};

resource 'BNDL' (128, purgeable) {
   'MPLY', 0,
   {  
      'ICN#', {0, 128},
      'FREF', {0, 128}
   }
};

data 'ICN#' (128) {
	$"000F F000 007F FE00 01FF FF80 03FF FFC0"            /* ...........Ä...¿ */
	$"07FF FFE0 0FFF FFF0 1FFF FFF8 3FF8 7FFC"            /* ............?... */
	$"3FE0 1FFC 7FC0 0FFE 7FC0 01FE 7F80 00FE"            /* ?....¿...¿...Ä.. */
	$"FF82 007F FE03 007F F803 801F F803 C01F"            /* .Ç........Ä...¿. */
	$"F003 E00F F003 C00F F003 800F F003 000F"            /* ......¿...Ä..... */
	$"7802 001E 7800 001E 7E00 007E 3FFF FFFC"            /* x...x...~..~?... */
	$"3FFF FFFC 1FFF FFF8 0FFF FFF0 07FF FFE0"            /* ?............... */
	$"03FF FFC0 01FF FF80 007F FE00 000F F000"            /* ...¿...Ä........ */
	$"000F F000 007F FE00 01FF FF80 03FF FFC0"            /* ...........Ä...¿ */
	$"07FF FFE0 0FFF FFF0 1FFF FFF8 3FFF FFFC"            /* ............?... */
	$"3FFF FFFC 7FFF FFFE 7FFF FFFE 7FFF FFFE"            /* ?............... */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"FFFF FFFF FFFF FFFF FFFF FFFF FFFF FFFF"            /* ................ */
	$"7FFF FFFE 7FFF FFFE 7FFF FFFE 3FFF FFFC"            /* ............?... */
	$"3FFF FFFC 1FFF FFF8 0FFF FFF0 07FF FFE0"            /* ?............... */
	$"03FF FFC0 01FF FF80 007F FE00 000F F000"            /* ...¿...Ä........ */
};

data 'ics#' (128) {
	$"07E0 1FF8 3FFC 7FFE 7C3E F81F F90F E187"            /* ....?...|>.....á */
	$"C1C3 C183 C103 6006 7FFE 3FFC 1FF8 07E0"            /* ¡√¡É¡.`...?..... */
	$"07E0 1FF8 3FFC 7FFE 7FFE FFFF FFFF FFFF"            /* ....?........... */
	$"FFFF FFFF FFFF 7FFE 7FFE 3FFC 1FF8 07E0"            /* ..........?..... */
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

resource 'MENU' (128) {
    128, textMenuProc;
    allEnabled, enabled;
    apple;
    {
        "About MacPlayer...", noIcon, noKey, noMark, plain;
        "-", noIcon, noKey, noMark, plain;
    }
};

resource 'MENU' (129) {
    129, textMenuProc;
    allEnabled, enabled;
    "File";
    {
        "Quit", noIcon, "Q", noMark, plain;
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
		/* Playlists Label */
		{1, 4, 15, 100},
		StaticText { disabled, "PLAYLISTS" };

		/* Nav List */
		{ 17, 5, 250, 113 },
		UserItem { enabled };

		/* Track Label */
		{1, 119, 15, 300},
		StaticText { disabled, "TITLE" };

		/* Artist Label */
		{1, 302, 15, 505},
		StaticText { disabled, "ARTIST" };

		/* Track List */
		{ 17, 120, 250, 505 },
		UserItem { enabled };

		/* Play */
		{ 258, 237, 290, 269 },
		Icon { enabled, 128 };

		/* Previous Track */
		{ 266, 201, 282, 217 },
		Icon { enabled, 130 };

		/* Next Track */
		{ 266, 289, 282, 305 },
		Icon { enabled, 129 };

		/* Shuffle */
		{ 266, 155, 282, 171 },
		Icon { enabled, 131 };

		/* Repeat */
		{ 266, 335, 282, 351 },
		Icon { enabled, 132 };
	}
};

resource 'DITL' (130, purgeable) {
	{ 
		/* MacPlayer Logo */
		{ 71, 90, 71+121, 90+325 },
		Picture { enabled, 132 };

		{212, 200, 212+12, 400},
		StaticText { disabled, "Getting ready..." };
	}
};

resource 'ALRT' (131, purgeable ) {
   { 0, 0, 110, 400 },
   131,
   {
      OK, visible, silent,
      OK, visible, silent,
      OK, visible, silent,
      OK, visible, silent
   },
   alertPositionMainScreen
};

resource 'DITL' (131, purgeable) {
	{
		{ 76, 300, 76+20, 380 },
		Button { enabled, "OK" };

		{8, 72, 70, 380},
		StaticText { disabled, "^0" };
	}
};

resource 'DLOG' (133, purgeable) {
	{0, 0, 190, 320},
	dBoxProc,
	visible,
	goAway,
	0,
	133,
	"About MacPlayer",
	centerMainScreen
};

resource 'DITL' (133, purgeable) {
	{ 
		{ 10, 10, 121, 310 },
		Picture { enabled, 133 };

		{ 131, 10, 181, 310},
		StaticText { disabled, "Anthony Super 2018.\rCredits & support info at: https://github.com/antscode/MacPlayer." };
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

data 'cicn' (136, "pause", purgeable) {
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
	$"FFFB DFFF FFFB BFFF FFFD BFC3 C3FD BFC3"            /* ......ø...ø√√.ø√ */
	$"C3FD 7FC3 C3FE 7FC3 C3FE 7FC3 C3FE 7FC3"            /* √..√√..√√..√√..√ */
	$"C3FE 7FC3 C3FE 7FC3 C3FE 7FC3 C3FE 7FC3"            /* √..√√..√√..√√..√ */
	$"C3FE BFC3 C3FD BFC3 C3FD BFFF FFFD DFFF"            /* √.ø√√.ø√√.ø..... */
	$"FFFB DFFF FFFB EFFF FFF7 F7FF FFEF FBFF"            /* ................ */
	$"FFDF FDFF FFBF FE7F FE7F FF8F F1FF FFF0"            /* .....ø.....è.... */
	$"0FFF 0000 0000 0000 0001 0000 FFFF FFFF"            /* ................ */
	$"FFFF 0001 0000 0000 0000 FFF0 0FFF FF8F"            /* ...............è */
	$"F1FF FE7F FE7F FDFF FFBF FBFF FFDF F7FF"            /* .........ø...... */
	$"FFEF EFFF FFF7 DFFF FFFB DFFF FFFB BFFF"            /* ..............ø. */
	$"FFFD BFC3 C3FD BFC3 C3FD 7FC3 C3FE 7FC3"            /* ..ø√√.ø√√..√√..√ */
	$"C3FE 7FC3 C3FE 7FC3 C3FE 7FC3 C3FE 7FC3"            /* √..√√..√√..√√..√ */
	$"C3FE 7FC3 C3FE 7FC3 C3FE BFC3 C3FD BFC3"            /* √..√√..√√.ø√√.ø√ */
	$"C3FD BFFF FFFD DFFF FFFB DFFF FFFB EFFF"            /* √.ø............. */
	$"FFF7 F7FF FFEF FBFF FFDF FDFF FFBF FE7F"            /* .............ø.. */
	$"FE7F FF8F F1FF FFF0 0FFF"                           /* ...è...... */
};

data 'PICT' (132, "macplayer-black", purgeable) {
	$"0B1D 0000 0000 0079 0145 1101 A101 F200"            /* .......y.E..°... */
	$"1638 4249 4D00 0000 0000 0000 7901 4547"            /* .8BIM.......y.EG */
	$"7289 7068 AF62 6A01 000A 0000 0000 0079"            /* râphØbj........y */
	$"0145 9800 2C00 0000 0000 7901 4500 0000"            /* .Eò.,.....y.E... */
	$"0000 7901 4500 0000 0000 7901 4500 000A"            /* ..y.E.....y.E... */
	$"F7FF 00C7 E4FF 00F8 FE00 0AF7 FF00 83E4"            /* ...«..........É. */
	$"FF00 F8FE 000A F7FF 0003 E4FF 00F8 FE00"            /* ................ */
	$"0BF8 FF01 FE03 E4FF 00F8 FE00 0BF8 FF01"            /* ................ */
	$"FC07 E4FF 00F8 FE00 0BF8 FF01 F80F E4FF"            /* ................ */
	$"00F8 FE00 0BF8 FF01 E00F E4FF 00F8 FE00"            /* ................ */
	$"0BF8 FF01 C01F E4FF 00F8 FE00 0BF8 FF01"            /* ....¿........... */
	$"803F E4FF 00F8 FE00 0BF8 FF01 003F E4FF"            /* Ä?...........?.. */
	$"00F8 FE00 0CF9 FF02 FE00 7FE4 FF00 F8FE"            /* ................ */
	$"000B F9FF 01FC 00E3 FF00 F8FE 000B F9FF"            /* ................ */
	$"01F8 20E3 FF00 F8FE 000E FCFF 04FB FFFF"            /* .. ............. */
	$"F041 E3FF 00F8 FE00 0EFC FF04 E1FF FFE0"            /* .A.............. */
	$"81E3 FF00 F8FE 000E FCFF 04E1 FFFF C183"            /* Å.............¡É */
	$"E3FF 00F8 FE00 0EFC FF04 C1FF FF83 07E3"            /* ..........¡..É.. */
	$"FF00 F8FE 000E FCFF 0481 FFFF 0707 E3FF"            /* .........Å...... */
	$"00F8 FE00 0EFC FF04 03FF FE0E 0FE3 FF00"            /* ................ */
	$"F8FE 000F FDFF 05FE 03FF FC1C 0FE3 FF00"            /* ................ */
	$"F8FE 000F FDFF 05FC 03FF F83C 1FE3 FF00"            /* ...........<.... */
	$"F8FE 000F FDFF 05F8 07FF F078 3FE3 FF00"            /* ...........x?... */
	$"F8FE 000F FDFF 05F8 07FF E1F8 3FE3 FF00"            /* ............?... */
	$"F8FE 000F FDFF 05F0 0FFF C3F0 7FE3 FF00"            /* ..........√..... */
	$"F8FE 000F FDFF 05E0 0FFF 87F0 7FE3 FF00"            /* ..........á..... */
	$"F8FE 000E FDFF 04C1 0FFF 0FE0 E2FF 00F8"            /* .......¡........ */
	$"FE00 0EFD FF04 831F FE1F C0E2 FF00 F8FE"            /* ......É...¿..... */
	$"000E FDFF 0486 1FFC 3FC1 E2FF 00F8 FE00"            /* .....Ü..?¡...... */
	$"0EFD FF04 063F F87F 83E2 FF00 F8FE 000F"            /* .....?..É....... */
	$"FEFF 05FE 0C3F F0FF 83E2 FF00 F8FE 000F"            /* .....?..É....... */
	$"FEFF 05FC 1C3F E1FF 07E2 FF00 F8FE 000F"            /* .....?.......... */
	$"FEFF 05F8 3C7F C3FF 07E2 FF00 F8FE 0012"            /* ....<.√......... */
	$"FEFF 08F8 387F 87FE 0FFF FF07 E5FF 00F8"            /* ....8.á......... */
	$"FE00 12FE FF08 F078 FF0F FE0F F9FC 07E5"            /* .......x........ */
	$"FF00 F8FE 0012 FEFF 08E0 F8FE 1FFC 1FC0"            /* ...............¿ */
	$"7803 E5FF 00F8 FE00 12FE FF08 C1F0 FC3F"            /* x...........¡..? */
	$"FC3F 0FF0 67E5 FF00 F8FE 0012 FEFF 08C3"            /* .?..g..........√ */
	$"F1FC 7FF8 3C1F E0E7 E5FF 00F8 FE00 12FE"            /* ....<........... */
	$"FF08 83F1 F8FF F878 7E41 CFE5 FF00 F8FE"            /* ..É....x~Aœ..... */
	$"0012 FEFF 0807 E1F1 FFF0 70FC 03CF E5FF"            /* ..........p..œ.. */
	$"00F8 FE00 13FF FF09 FE0F E3E3 FFF0 E1F8"            /* .......∆........ */
	$"079E E5FF 00F8 FE00 13FF FF09 FE1F E3C7"            /* .û.........∆...« */
	$"FFE0 C3F0 0F3E E5FF 00F8 FE00 18FF FF09"            /* ..√..>.........∆ */
	$"FC1F C78F FFE1 87E0 0E7D ECFF 01E0 07FC"            /* ..«è..á..}...... */
	$"FF00 F8FE 0019 FFFF 09F8 3FC7 9FFF C107"            /* ........∆.?«ü.¡. */
	$"C00D FBEC FFFF 0000 7FFD FF00 F8FE 001A"            /* ¿¬.............. */
	$"FFFF 09F8 7FC7 3FFF C20F 801B F7ED FF03"            /* ..∆..«?.¬.Ä..... */
	$"F800 001F FDFF 00F8 FE00 1AFF FF09 F0FF"            /* .............∆.. */
	$"8E3F FFC2 1F00 3FCF EDFF 03F0 0000 07FD"            /* é?.¬..?œ........ */
	$"FF00 F8FE 001A FFFF 09E0 FF8C 7FFF 843E"            /* ........∆..å..Ñ> */
	$"007F 8FED FF03 C000 0003 FDFF 00F8 FE00"            /* ..è...¿......... */
	$"19FF FF09 E1FF 88FF FF80 3C00 FF1F EDFF"            /* ...∆..à..Ä<..... */
	$"0080 FE00 FDFF 00F8 FE00 19FF FF09 C3FF"            /* .Ä...........∆√. */
	$"19FF FF08 7800 FE3F EDFF FD00 007F FEFF"            /* ....x..?........ */
	$"00F8 FE00 1BFF FF09 83FF 13FF FF00 7421"            /* .......∆É.....t! */
	$"FC7F EEFF 00FE FD00 003F FEFF 00F8 FE00"            /* .........?...... */
	$"1AFF FF08 87FF 27FF FF10 E401 F0ED FF00"            /* ....á.'......... */
	$"FCFD 0000 1FFE FF00 F8FE 001A FFFF 080F"            /* ................ */
	$"FF27 FFFE 00C8 03E1 EDFF 00F8 FD00 000F"            /* .'...».......... */
	$"FEFF 00F8 FE00 1A0A FFFE 1FFE 0FFF FE01"            /* ................ */
	$"9803 87ED FF00 F0FD 0000 0FFE FF00 F8FE"            /* ò.á............. */
	$"001A 0AFF FE1F FE5F FFFE 2130 000F EDFF"            /* ......._..!0.... */
	$"00F0 FD00 0007 FEFF 00F8 FE00 1A0A FFFC"            /* ................ */
	$"3FFE 1FFF FC00 7040 1FED FF00 E0FD 0004"            /* ?.....p@........ */
	$"040F FFFF F8FE 0019 0AFF F87F FE3F FFFC"            /* .............?.. */
	$"40F0 E07F EDFF 00E0 FC00 0303 FFFF F8FE"            /* @............... */
	$"0018 09FF F87F FE7F FFFC 41F1 FBEC FF00"            /* ..∆.......A..... */
	$"C0FC 0003 01FF FFF8 FE00 1507 FFF0 FFFC"            /* ¿............... */
	$"7FFF FC43 EAFF 00C0 FB00 FFFF 00F8 FE00"            /* ...C...¿........ */
	$"1406 FFE1 FFFC FFFF F8E9 FF00 80FB 0002"            /* ............Ä... */
	$"7FFF F8FE 0014 06FF E3FF FCFF FFF8 E9FF"            /* ................ */
	$"0080 FB00 027F FFF8 FE00 1906 FFC3 FFFD"            /* .Ä...........√.. */
	$"FFFF F8FD FF01 FE00 EFFF 0080 FB00 023F"            /* ...........Ä...? */
	$"FFF8 FE00 1906 FFC7 FFFD FFFF F9FD FF01"            /* .......«........ */
	$"FE00 EFFF 0080 FB00 023F FFF8 FE00 1601"            /* .....Ä...?...... */
	$"FF8F FDFF 00F9 FDFF 01FE 00EF FFFA 0002"            /* .è.............. */
	$"07FF F8FE 0016 01FF 0FFD FF06 F9F8 0000"            /* ................ */
	$"0FFE 00EF FFF9 0001 FFF8 FE00 1601 FF1F"            /* ................ */
	$"FDFF 06F9 F800 0001 FE00 EFFF F900 013F"            /* ...............? */
	$"F8FE 0016 01FE 3FFD FF01 FBF8 FE00 017E"            /* ......?........~ */
	$"00EF FFF9 0001 0FF8 FE00 1601 FE3F FDFF"            /* .............?.. */
	$"01FB F8FE 0001 3E00 EFFF F900 0107 F8FE"            /* ......>......... */
	$"0018 01FC 7FFD FF01 FBF8 FE00 011E 00F0"            /* ................ */
	$"FF00 C0F9 0001 03F8 FE00 1700 F8FC FF01"            /* ..¿............. */
	$"FBF8 FE00 010E 00F1 FF00 FEF8 0001 01F8"            /* ................ */
	$"FE00 1A00 F8FB FF00 F8FE 0001 0E00 F1FF"            /* ................ */
	$"00FC FE00 0101 80FC 0000 F8FE 001A 00F1"            /* ......Ä......... */
	$"FBFF 05F8 07FF C006 00F1 FF00 F0FE 0001"            /* ......¿......... */
	$"01C0 FC00 0078 FE00 1A00 F3FB FF05 F807"            /* .¿...x.......... */
	$"FFF0 0600 F1FF 00E0 FE00 0101 F0FC 0000"            /* ................ */
	$"38FE 001A 00E3 FBFF 05F8 07FF F002 00F1"            /* 8............... */
	$"FF00 C0FE 0001 01F8 FC00 0038 FE00 1A00"            /* ..¿........8.... */
	$"E7FB FF05 F807 FFF8 0200 F1FF 0080 FE00"            /* .............Ä.. */
	$"0101 FCFC 0000 18FE 0026 00CF FBFF 15F8"            /* .........&.œ.... */
	$"07FF F802 00FF C001 F803 FFF8 03F8 00FF"            /* ......¿......... */
	$"E01F C3FF FFFD 0001 01FF FC00 0018 FE00"            /* ..√............. */
	$"2700 CFFB FF15 F807 FFFC 0200 FF00 007C"            /* '.œ............| */
	$"01FF F807 C000 3FE0 1F03 FFFE FD00 0201"            /* ....¿.?......... */
	$"FF80 FD00 0008 FE00 2700 9FFB FF15 F807"            /* .Ä......'.ü..... */
	$"FFFC 0200 FE00 001C 01FF F807 8000 0FE0"            /* ............Ä... */
	$"1E03 FFFE FD00 0201 FFC0 FD00 0008 FE00"            /* .........¿...... */
	$"2700 BFFB FF15 F807 FFFC 0200 FC00 000E"            /* '.ø............. */
	$"01FF F807 0000 07E0 1803 FFFC FD00 0201"            /* ................ */
	$"FFF0 FD00 0008 FE00 2300 7FFB FF15 F807"            /* ........#....... */
	$"FFF8 0200 F800 0006 01FF F00E 0000 03E0"            /* ................ */
	$"1803 FFFC FD00 0201 FFF8 F900 2300 7FFB"            /* ............#... */
	$"FF15 F807 FFF8 0200 F000 0006 00FF F00C"            /* ................ */
	$"0000 03E0 0003 FFF8 FD00 0201 FFFE F900"            /* ................ */
	$"2300 7FFB FF15 F807 FFF8 0200 F007 F003"            /* #............... */
	$"00FF F008 01FC 01E0 0003 FFF8 FD00 0201"            /* ................ */
	$"FFFE F900 21FA FF15 F807 FFF0 0600 E00F"            /* ....!........... */
	$"F803 00FF E018 07FE 01E0 0007 FFF8 FD00"            /* ................ */
	$"0201 FFFC F900 21FA FF15 F807 FFE0 0600"            /* ......!......... */
	$"E01F FC03 007F E010 0FFF 00E0 00FF FFF0"            /* ................ */
	$"FD00 0201 FFF0 F900 21FA FF15 F807 FE00"            /* ........!....... */
	$"0600 E01F FC03 807F E010 0FFF 00E0 01FF"            /* ......Ä......... */
	$"FFF0 FD00 0201 FFE0 F900 21FA FF00 F8FE"            /* ..........!..... */
	$"0011 0E00 FFFF FC03 807F C020 1FFF 8060"            /* ........Ä.¿ ..Ä` */
	$"03FF FFF0 FD00 0201 FFC0 F900 20FA FF00"            /* .........¿.. ... */
	$"F8FE 0011 1E00 FFFF FC03 803F C020 1FFF"            /* ..........Ä?¿ .. */
	$"8060 07FF FFF0 FD00 0101 FFF8 0020 FAFF"            /* Ä`........... .. */
	$"00F8 FE00 113E 00FF FFFC 03C0 3FC0 201F"            /* .....>.....¿?¿ . */
	$"FF80 6007 FFFF F0FD 0001 01FE F800 20FA"            /* .Ä`........... . */
	$"FF00 F8FE 0011 7E00 FFF8 0003 C03F C060"            /* ......~.....¿?¿` */
	$"1FFF 8060 0FFF FFF0 FD00 0101 FCF8 0020"            /* ..Ä`...........  */
	$"FAFF 00F8 FE00 09FE 00FF 8000 03C0 1F80"            /* ......∆...Ä..¿.Ä */
	$"60FE 0004 600F FFFF F0FD 0001 01F0 F800"            /* `...`........... */
	$"20FA FF0D F800 0007 FE00 FC00 0003 E01F"            /*  ..¬............ */
	$"8040 FE00 0460 0FFF FFF0 FD00 0101 E0F8"            /* Ä@...`.......... */
	$"0020 FAFF 0DF8 0001 FFFE 00F8 0000 03E0"            /* . ..¬........... */
	$"1F80 C0FE 0004 600F FFFF F0FD 0001 0180"            /* .Ä¿...`........Ä */
	$"F800 1FFA FF0D F807 FFFF FE00 F000 0003"            /* .....¬.......... */
	$"F01F 00C0 FE00 0460 0FFF FFF0 FD00 0001"            /* ...¿...`........ */
	$"F700 1FFA FF0D F807 FFFF FE00 E000 FC03"            /* .....¬.......... */
	$"F00F 00C0 FE00 0460 0FFF FFF0 F600 0008"            /* ...¿...`........ */
	$"FE00 1FFA FF15 F807 FFFF FE00 C00F FC03"            /* ............¿... */
	$"F00F 01C0 1FFF FFE0 0FFF FFF8 F600 0008"            /* ...¿............ */
	$"FE00 1FFA FF15 F807 FFFF FE00 C01F FC03"            /* ............¿... */
	$"F80F 01E0 1FFF FFE0 0FFF FFF8 F600 0008"            /* ................ */
	$"FE00 1FFA FF15 F807 FFFF FE00 C03F FC03"            /* ............¿?.. */
	$"F806 01E0 1FFF FFE0 0FFF FFF8 F600 0018"            /* ................ */
	$"FE00 1FFA FF15 F807 FFFF FE00 C03F FC03"            /* ............¿?.. */
	$"F806 03E0 1FFF FFE0 0FFF FFFC F600 0018"            /* ................ */
	$"FE00 1FFA FF15 F807 FFFF FE00 803F FC03"            /* ............Ä?.. */
	$"FC06 03E0 1FFF FFE0 0FFF FFFC F600 0038"            /* ...............8 */
	$"FE00 1FFA FF15 F807 FFFF FE00 803F FC03"            /* ............Ä?.. */
	$"FC06 03E0 0FFF 0060 0FFF FFFE F600 0078"            /* .......`.......x */
	$"FE00 1FFA FF15 F807 FFFF FE00 803F F803"            /* ............Ä?.. */
	$"FC00 07F0 0FFF 00E0 0FFF FFFE F600 0078"            /* ...............x */
	$"FE00 1EFA FF12 F807 FFFF FE00 C03F F803"            /* ............¿?.. */
	$"FE00 07F0 07FE 00E0 0FFE FFF6 0000 F8FE"            /* ................ */
	$"0021 FAFF 12F8 07FF FFFE 00C0 1FF0 03FE"            /* .!.........¿.... */
	$"0007 F801 FC01 E00F FEFF 0080 F800 0101"            /* ...........Ä.... */
	$"F8FE 0021 FAFF 12F8 07FF FFFE 00C0 07C0"            /* ...!.........¿.¿ */
	$"03FE 000F F800 0001 E00F FEFF 00C0 F800"            /* .............¿.. */
	$"0103 F8FE 0021 FAFF 12F8 07FF FFFE 00E0"            /* .....!.......... */
	$"0000 03FF 000F FC00 0003 E00F FEFF 00E0"            /* ................ */
	$"F800 0107 F8FE 0021 FAFF 12F8 07FF FFFE"            /* .......!........ */
	$"00E0 0002 03FF 000F FE00 0007 E00F FEFF"            /* ................ */
	$"00F0 F800 011F F8FE 0021 FAFF 12F8 07FF"            /* .........!...... */
	$"FFFE 00F0 0002 03FF 001F FF00 000F E00F"            /* ................ */
	$"FEFF 00FC F800 013F F8FE 001F FAFF 12F8"            /* .......?........ */
	$"07FF FFFE 00F8 0006 01FF 801F FF80 001F"            /* ..........Ä..Ä.. */
	$"E00F FDFF F800 01FF F8FE 0022 FAFF 12F8"            /* ...........".... */
	$"07FF FFFE 00FE 001E 01FF 801F FFE0 007F"            /* ..........Ä..... */
	$"E00F FDFF 00C0 FA00 0207 FFF8 FE00 12F3"            /* .....¿.......... */
	$"FF00 C0FE FF04 C03F FFFE 07F0 FF00 F8FE"            /* ..¿...¿?........ */
	$"000B EFFF 01C0 3FED FF00 F8FE 000B EFFF"            /* .....¿?......... */
	$"0180 3FED FF00 F8FE 000B EFFF 0180 7FED"            /* .Ä?..........Ä.. */
	$"FF00 F8FE 000B EFFF 0100 7FED FF00 F8FE"            /* ................ */
	$"000B F0FF 01FE 00EC FF00 F8FE 000B F0FF"            /* ................ */
	$"01FC 00EC FF00 F8FE 000B F0FF 0100 01EC"            /* ................ */
	$"FF00 F8FE 000B F0FF 0100 01EC FF00 F8FE"            /* ................ */
	$"000B F0FF 0100 03EC FF00 F8FE 000B F0FF"            /* ................ */
	$"0100 07EC FF00 F8FE 000B F0FF 0100 0FEC"            /* ................ */
	$"FF00 F8FE 000B F0FF 0100 1FEC FF00 F8FE"            /* ................ */
	$"000A F0FF 00C1 EBFF 00F8 FE00 FF"                   /* .....¡....... */
};

data 'PICT' (133, "macplayer-white") {
	$"0902 0000 0000 001B 0048 0011 02FF 0C00"            /* ∆........H...... */
	$"FFFE 0000 012C 0000 012C 0000 0000 0000"            /* .....,...,...... */
	$"006F 012C 0000 0000 00A1 01F2 0016 3842"            /* .o.,.....°....8B */
	$"494D 0000 0000 0000 001B 0048 4772 8970"            /* IM.........HGrâp */
	$"68AF 626A 0001 000A 0000 0000 006F 012C"            /* hØbj.........o., */
	$"0098 0028 0000 0000 006F 012C 0000 0000"            /* .ò.(.....o.,.... */
	$"006F 012C 0000 0000 006F 012C 0000 06F8"            /* .o.,.....o.,.... */
	$"0000 0EE3 0006 F800 001F E300 06F8 0000"            /* ................ */
	$"3FE3 0006 F800 007E E300 06F8 0000 FEE3"            /* ?......~........ */
	$"0007 F900 0103 FCE3 0007 F900 0103 F8E3"            /* ................ */
	$"0007 F900 010F F8E3 0007 F900 011F F0E3"            /* ................ */
	$"0007 F900 013F E0E3 0007 F900 017F E0E3"            /* .....?.......... */
	$"0007 F900 01FF C0E3 000A FC00 0460 0001"            /* ......¿......`.. */
	$"E7C0 E300 0AFC 0004 F000 03CF 80E3 000A"            /* .¿.........œÄ... */
	$"FD00 0401 F000 079F E200 0AFD 0004 03F0"            /* .......ü........ */
	$"000F 1FE2 000A FD00 0403 E000 1E3E E200"            /* .............>.. */
	$"0AFD 0004 07E0 003C 3CE2 000A FD00 040F"            /* .......<<....... */
	$"C000 787C E200 0AFD 0004 1FC0 00F0 78E2"            /* ¿.x|.......¿..x. */
	$"000A FD00 043F C001 E0F8 E200 0AFD 0004"            /* .....?¿......... */
	$"7F80 03C1 F0E2 000A FD00 047B 8007 81E0"            /* .Ä.¡.......{Ä.Å. */
	$"E200 0AFD 0004 F780 0F03 E0E2 000B FE00"            /* .......Ä........ */
	$"0501 F700 1E03 C0E2 000B FE00 0503 EF00"            /* ......¿......... */
	$"3C07 C0E2 000B FE00 0507 CE00 7807 80E2"            /* <.¿.......Œ.x.Ä. */
	$"000B FE00 0507 8E00 F00F 80E2 000A FE00"            /* ......é...Ä..... */
	$"040F 1E01 E00F E100 0DFE 0007 1F1C 03C0"            /* ........¬......¿ */
	$"1F00 001C E400 0DFE 0007 3E1C 0780 3E00"            /* ......¬...>..Ä>. */
	$"007E E400 0DFE 0007 3C38 0F00 3C03 E1FE"            /* .~..¬...<8..<... */
	$"E400 0DFE 0007 7838 1E00 3C0F 03E6 E400"            /* ..¬...x8..<..... */
	$"0DFE 0007 F838 3C00 783C 07CE E400 0EFF"            /* ¬....8<.x<.Œ.... */
	$"0008 01F0 7038 0078 F03F 8CE4 000E FF00"            /* ....p8.x.?å..... */
	$"0801 E070 7000 F1E0 7F18 E400 0EFF 0008"            /* ...pp........... */
	$"03C0 70E0 00F3 E0FE 10E4 000F FF00 0907"            /* .¿p...........∆. */
	$"C0E1 C001 E3C1 FE20 80E5 0012 FF00 080F"            /* ¿.¿..¡. Ä....... */
	$"80E3 8001 E783 FC41 ED00 0038 F900 14FF"            /* Ä.Ä..É.A...8.... */
	$"0008 0F00 C700 03CF 07FC 82EE 0002 07FF"            /* ....«..œ..Ç..... */
	$"C0FA 0014 FF00 081E 01C6 0003 DE0F F804"            /* ¿........∆...... */
	$"EE00 023F FFF8 FA00 14FF 0008 3E01 CC00"            /* ...?........>.Ã. */
	$"07BC 1FF0 0CEE 0002 7FFF FCFA 0014 FF00"            /* .º.............. */
	$"083C 019C 0007 BC3F E018 EF00 0001 FEFF"            /* .<.ú..º?........ */
	$"FA00 16FF 0008 7803 B800 0778 7FC0 70EF"            /* ......x.∏..x.¿p. */
	$"0000 07FE FF00 80FB 0016 FF00 08F0 03B0"            /* ......Ä........∞ */
	$"000F 78FF C0E0 EF00 000F FEFF 00E0 FB00"            /* ..x.¿........... */
	$"16FF 0008 F003 6000 0EF1 7F81 C0EF 0000"            /* ......`....Å¿... */
	$"0FFE FF00 E0FB 0016 0A00 01E0 0340 001F"            /* .............@.. */
	$"F2FF 8780 EF00 001F FEFF 00F0 FB00 1509"            /* ..áÄ...........∆ */
	$"0003 C007 C000 1DE4 FF9E EE00 003F FEFF"            /* ..¿.¿....û...?.. */
	$"00F8 FB00 1509 0003 8007 8000 1DF8 FFFC"            /* .....∆..Ä.Ä..... */
	$"EE00 007F FEFF 00FC FB00 1509 0007 8007"            /* ...........∆..Ä. */
	$"0000 1FF8 F7F8 EE00 007F FEFF 00FC FB00"            /* ................ */
	$"1309 000F 0007 0000 3BE1 E3E0 EE00 FCFF"            /* .∆......;....... */
	$"00FC FC00 1208 000E 0006 0000 3BC0 C0ED"            /* ............;¿¿. */
	$"00FC FF00 FEFC 0011 0700 1E00 0C00 0039"            /* ...............9 */
	$"80ED 0000 01FB FFFC 0012 0600 1C00 0C00"            /* Ä............... */
	$"0030 EC00 0001 FBFF 0080 FD00 1206 0038"            /* .0.......Ä.....8 */
	$"0008 0000 70EC 0000 03FB FF00 80FD 0016"            /* ....p.......Ä... */
	$"0600 7800 0800 0060 FD00 007F F100 0003"            /* ..x....`........ */
	$"FBFF 00C0 FD00 1501 0070 FD00 0060 FD00"            /* ...¿.....p...`.. */
	$"007F F100 0003 FBFF 00C0 FD00 1601 00E0"            /* .........¿...... */
	$"FD00 0560 7FFF FC00 7FF1 0000 03FB FF00"            /* ...`............ */
	$"FCFD 0014 0100 E0FD 0005 607F FFFF 807F"            /* ..........`...Ä. */
	$"F100 0003 FAFF FD00 1601 01C0 FD00 0560"            /* ...........¿...` */
	$"7FFF FFE0 7FF1 0000 03FA FF00 C0FE 0016"            /* ............¿... */
	$"0103 80FD 0005 607F FFFF F07F F100 0003"            /* ..Ä...`......... */
	$"FAFF 00F0 FE00 1800 03FC 0005 207F FFFF"            /* ............ ... */
	$"F87F F100 037F FFFF DFFD FF00 F8FE 0018"            /* ................ */
	$"0007 FB00 047F FFFF FC7F F200 0003 FEFF"            /* ................ */
	$"00CF FDFF 00FC FE00 1800 06FB 0004 7F00"            /* .œ.............. */
	$"1FFC 7FF2 0000 0FFE FF00 C7FD FF00 FEFE"            /* ..........«..... */
	$"0016 000E FB00 047F 0007 FE7F F200 001F"            /* ................ */
	$"FEFF 00C1 FCFF FE00 1600 0CFB 0004 7F00"            /* ...¡............ */
	$"01FE 7FF2 0000 7FFE FF00 C0FC FFFE 0019"            /* ..........¿..... */
	$"0018 FB00 047F 0001 FE7F F200 007F FEFF"            /* ................ */
	$"01C0 7FFD FF02 8000 0022 0010 FB00 077F"            /* .¿....Ä.."...... */
	$"0001 FF7F 003F F8FC 0006 7FC0 0000 3C00"            /* .....?.....¿..<. */
	$"00FD FF01 C01F FDFF 0280 0000 2400 30FB"            /* ....¿....Ä..$.0. */
	$"0013 7F00 00FF 7F00 FFFE 0FF0 007F 83FF"            /* ..............É. */
	$"F00F F0FC 0001 FDFF 01C0 0FFD FF02 C000"            /* .........¿....¿. */
	$"0024 0060 FB00 137F 0000 FF7F 03FF FF8F"            /* .$.`...........è */
	$"F000 7F0F FFFC 0FF3 FC00 03FD FF01 C003"            /* ..............¿. */
	$"FDFF 02E0 0000 2400 60FB 0013 7F00 00FF"            /* ......$.`....... */
	$"7F07 FFFF CFF0 007F 1FFF FE0F F3FC 0007"            /* ....œ........... */
	$"FDFF 01C0 01FD FF02 E000 0024 0040 FB00"            /* ...¿.......$.@.. */
	$"137F 0000 FF7F 0FFF FFC7 F800 FE3F FFFF"            /* .........«...?.. */
	$"0FF7 FC00 07FD FF01 C000 FDFF 02E0 0000"            /* ........¿....... */
	$"2500 80FB 0013 7F00 01FE 7F0F F87F E7F8"            /* %.Ä............. */
	$"00FE 7FF3 FF0F FFFC 000F FDFF 02C0 003F"            /* .............¿.? */
	$"FEFF 02E0 0000 23FA 0013 7F00 01FE 7F1F"            /* ......#......... */
	$"E01F F7F8 00FE 7F80 7F8F FFFC 000F FDFF"            /* .......Ä.è...... */
	$"02C0 003F FEFF 02F0 0000 22FA 0013 7F00"            /* .¿.?......"..... */
	$"03FE 7F1F E00F F3F8 01FC FF00 3FCF FFC0"            /* ............?œ.¿ */
	$"000F FDFF 01C0 00FD FF02 F000 0022 FA00"            /* .....¿.......".. */
	$"137F 000F FE7F 1FC0 0FF3 FC01 FCFE 003F"            /* .......¿.......? */
	$"CFFF 0000 0FFD FF01 C001 FDFF 02F0 0000"            /* œ.......¿....... */
	$"22FA 0013 7FFF FFFC 7F1F C007 F3FC 01FD"            /* ".........¿..... */
	$"FE00 1FCF FC00 001F FDFF 01C0 03FD FF02"            /* ...œ.......¿.... */
	$"F000 0022 FA00 137F FFFF F87F 0000 07F1"            /* ..."............ */
	$"FC03 FDFE 001F EFF8 0000 1FFD FF01 C007"            /* ..............¿. */
	$"FDFF 02F0 0000 22FA 0013 7FFF FFF0 7F00"            /* ......"......... */
	$"0007 F1FE 03F9 FC00 1FEF F800 001F FDFF"            /* ................ */
	$"01C0 1FFD FF02 F000 0022 FA00 137F FFFF"            /* .¿......."...... */
	$"E07F 0001 FFF0 FE03 F9FC 001F EFF8 0000"            /* ................ */
	$"1FFD FF01 C03F FDFF 02F0 0000 21FA 000B"            /* ....¿?......!... */
	$"7FFF FFC0 7F00 FFFF F0FE 03F9 FEFF 04EF"            /* ...¿............ */
	$"F000 001F FDFF 00C0 FCFF 02F0 0000 21FA"            /* .......¿......!. */
	$"000B 7FFF FE00 7F03 FFFF F0FF 07F1 FEFF"            /* ................ */
	$"04EF F000 001F FDFF 00C1 FCFF 02F0 0000"            /* .........¡...... */
	$"21FA 0000 7FFE 0007 7F0F FFFF F07F 07F3"            /* !............... */
	$"FEFF 04EF F000 001F FDFF 00C7 FCFF 02E0"            /* ...........«.... */
	$"0000 21FA 0000 7FFE 0007 7F1F FFFF F07F"            /* ..!............. */
	$"07F3 FEFF 04EF F000 001F FDFF 00CF FCFF"            /* .............œ.. */
	$"02E0 0000 21FA 0000 7FFE 0007 7F1F F807"            /* ....!........... */
	$"F07F 8FE3 FEFF 04EF F000 000F FDFF 00DF"            /* ..è............. */
	$"FCFF 02E0 0000 1DFA 0000 7FFE 000F 7F3F"            /* ...............? */
	$"E007 F03F 8FE1 FC00 000F F000 000F F7FF"            /* ...?è........... */
	$"02C0 0000 1DFA 0000 7FFE 000F 7F3F 8007"            /* .¿...........?Ä. */
	$"F03F 8FE1 FC00 000F F000 000F F7FF 02C0"            /* .?è............¿ */
	$"0000 1DFA 0000 7FFE 00FF 7F0D 8007 F03F"            /* ...........¬Ä..? */
	$"9FC1 FC00 000F F000 0007 F7FF 0280 0000"            /* ü¡...........Ä.. */
	$"1DFA 0000 7FFE 00FF 7F0D 800F F01F DFC1"            /* .........¬Ä....¡ */
	$"FE00 000F F000 0007 F7FF 0280 0000 1BFA"            /* ...........Ä.... */
	$"0000 7FFE 00FF 7F0D 800F F01F DFC1 FE00"            /* .......¬Ä....¡.. */
	$"1FCF F000 0003 F7FF FE00 1DFA 0000 7FFE"            /* .œ.............. */
	$"00FF 7F0D 800F F00F DF80 FF00 1FCF F000"            /* ...¬Ä....Ä...œ.. */
	$"0003 F8FF 00FE FE00 1DFA 0000 7FFE 00FF"            /* ................ */
	$"7F0D 801F F00F FF80 FF80 3FCF F000 0001"            /* .¬Ä....Ä.Ä?œ.... */
	$"F8FF 00FE FE00 1CFA 0000 7FFE 000C 7F3F"            /* ...............? */
	$"C03F F00F FF80 7FC0 7F8F F0FE 00F8 FF00"            /* ¿?...Ä.¿.è...... */
	$"FCFE 001E FA00 007F FE00 0C7F 3FF0 FFF0"            /* ............?... */
	$"07FF 007F FFFF 8FF0 FE00 007F F9FF 00F8"            /* ......è......... */
	$"FE00 1EFA 0000 7FFE 000C 7F1F FFFF F007"            /* ................ */
	$"FF00 3FFF FF0F F0FE 0000 3FF9 FF00 E0FE"            /* ..?.......?..... */
	$"001E FA00 007F FE00 0C7F 1FFF F7F0 07FE"            /* ................ */
	$"001F FFFE 0FF0 FE00 000F F9FF 0080 FE00"            /* .............Ä.. */
	$"1CFA 0000 7FFE 000C 7F0F FFF7 F007 FE00"            /* ................ */
	$"0FFF FC0F F0FE 0000 07F9 FFFD 001C FA00"            /* ................ */
	$"007F FE00 0C7F 07FF C7F0 03FE 0003 FFF0"            /* ........«....... */
	$"0FF0 FD00 FAFF 00F8 FD00 0FF5 0009 01FF"            /* .............∆.. */
	$"0000 03FE 0000 FFC0 EF00 07F1 0001 01FC"            /* .......¿........ */
	$"EB00 07F1 0001 03FC EB00 07F1 0001 03FC"            /* ................ */
	$"EB00 07F1 0001 07F8 EB00 07F1 0001 0FF8"            /* ................ */
	$"EB00 07F1 0001 3FF0 EB00 08F2 0002 03FF"            /* ......?......... */
	$"F0EB 0008 F200 0203 FFE0 EB00 08F2 0002"            /* ................ */
	$"03FF C0EB 0008 F200 0203 FFC0 EB00 08F2"            /* ..¿........¿.... */
	$"0002 03FF 80EB 0007 F200 0103 FCEA 0000"            /* ....Ä........... */
	$"00FF"                                               /* .. */
};

data 'PICT' (136, "spotify", purgeable) {
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

data 'PICT' (137, "dot") {
	$"0060 0000 0000 0004 0004 1101 A101 F200"            /* .`..........°... */
	$"1638 4249 4D00 0000 0000 0000 0400 0447"            /* .8BIM..........G */
	$"7289 7068 AF62 6A01 000A 0000 0000 0004"            /* râphØbj......... */
	$"0004 9000 0400 0000 0000 0400 0400 0000"            /* ..ê............. */
	$"0000 0400 0400 0000 0000 0400 0400 0090"            /* ...............ê */
	$"0000 0000 0000 0000 0000 0090 0000 00FF"            /* ...........ê.... */
};