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
		{ 0, 15, 250, 100+10 },
		UserItem { enabled };

		/* Track List */
		{ 0, 120, 250, 505 },
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