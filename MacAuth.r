#include "Dialogs.r"

resource 'DLOG' (1028, purgeable) {
	{ 0, 0, 124, 340 },
	dBoxProc,
	visible,
	goAway,
	0,
	1028,
	"",
	alertPositionMainScreen
};

data 'dctb' (1028, purgeable) {
   $"0000 0000 0000 FFFF"  /*use default colors*/
};

resource 'DITL' (1028, purgeable) {
	{
		{ 90, 340-170, 90+20, 340-110 },
		Button { enabled, "Cancel" };

		{ 90, 340-10-80-4, 90+20, 340-10-4 },
		Button { enabled, "Continue" };
	}
};

resource 'ALRT' (1029, purgeable ) {
   { 0, 0, 110, 400 },
   1029,
   {
      OK, visible, silent,
      OK, visible, silent,
      OK, visible, silent,
      OK, visible, silent
   },
   alertPositionMainScreen
};

resource 'DITL' (1029, purgeable) {
	{
		{ 80, 300, 80+20, 380 },
		Button { enabled, "OK" };

		{8, 72, 70, 380},
		StaticText { disabled, "Please ensure you follow all of the steps your smartphone before clicking Continue." };
	}
};

resource 'ALRT' (1030, purgeable ) {
   { 0, 0, 110, 400 },
   1030,
   {
      OK, visible, silent,
      OK, visible, silent,
      OK, visible, silent,
      OK, visible, silent
   },
   alertPositionMainScreen
};

resource 'DITL' (1030, purgeable) {
	{
		{ 76, 300, 76+20, 380 },
		Button { enabled, "OK" };

		{8, 72, 70, 380},
		StaticText { disabled, "Sorry, something went wrong.\r^0" };
	}
};