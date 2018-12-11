/*
*
*     Author  - Alexander S. Colwell,
*               Copyright (C) 1988, 1989
*               Modified by Anthony Super, 2018
*/

#include <QuickDraw.h>
#include <ToolUtils.h>
#include <ControlDefinitions.h>
#include "DarkScrollbarDef.h"

extern "C"
{
	typedef struct {          /* Dual Control          */
		short        hasColor;    /* Has color monitor flag*/
		Rect         thumb;       /* Thumb rectangle area  */
		Rect         cThumb;      /* Current thmb rect area*/
		PolyHandle   up;          /* Up button handle      */
		PolyHandle   down;        /* Down button handle    */
		short        bLen;        /* Button length         */
		short        hBLen;       /* Half button length    */
		short        sLen;        /* Scroll bar length     */
		double       tFactor;     /* Thumb factor          */
		short        hFactor;     /* Horizontal factor     */
		short        vFactor;     /* Vertical factor       */
		ControlHandle cHdl;       /* Ctrl hdl to reference */
		long         blackPat[2]; /* Black pattern         */
		long         ltGrayPat[2];/* Light gray pattern    */
	} DS;
	typedef DS      *DSPTR;
	typedef DSPTR   *DSHDL;

	typedef struct {         /* Thumb Info            */
		Rect         limitRect;   /* Limit rect scrolling  */
		Rect         slopRect;    /* Slop rect scrolling   */
		short        axis;        /* Drag axis control     */
	} THUMB;
	typedef THUMB   *THUMBPTR;
	/* Misc definitions      */
#define NIL     (0L)         /* NIL pointer           */
#define abs(a)  (a<0?-a:a)   /* Absolute macro func   */
#define min(a,b) (a<b?a:b)   /* Minumim macro function*/
#define max(a,b) (a<b?b:a)   /* Maximum macro function*/

/* Trap definitions      */
#define SysEnvironsTrap 0xa090/* System Enviroment    */
#define UnknownTrap     0xa89f/* Unknown instruction  */

/* System 7.0 CDEF msgs  */
#define calcCntlRgn  10      /* Calculate ctnl region */
#define calcThumbRgn 11      /* Calculate “thumb” rgn */
/* Control’s macro procs */
#define DSFrameHUpBtn()                                \
        DSDrawHBtn(dPtr,dPtr->up,FALSE)
#define DSFrameLDownBtn()                              \
        DSDrawLBtn(dPtr,dPtr->down,FALSE)
#define DSFillHUpBtn()                                 \
        DSDrawHBtn(dPtr,dPtr->up,TRUE)
#define DSFillLDownBtn()                               \
        DSDrawLBtn(dPtr,dPtr->down,TRUE)

#define DSFrameBody(a)                                 \
        DSDrawObject(dPtr->cHdl,(ProcPtr)&DSGetForeGround,       \
                     (ProcPtr)&DSSetForeGround,(ProcPtr)&DSFrameRect,      \
                     blackColor,a,0)
#define DSClearBody(a)                                 \
        DSDrawObject(dPtr->cHdl,(ProcPtr)&DSGetBackGround,       \
                     (ProcPtr)&DSSetBackGround,(ProcPtr)&DSEraseRect,      \
                     blackColor,a,0)
#define DSFillBody(a)                                  \
        DSDrawObject(dPtr->cHdl,(ProcPtr)&DSGetBackGround,       \
        (ProcPtr)&DSSetBackGround,(ProcPtr)&DSEraseRect,blackColor,a,       \
        0)
#define DSFrameThumb(a)                                \
        DSDrawObject(dPtr->cHdl,(ProcPtr)&DSGetForeGround,       \
                     (ProcPtr)&DSSetForeGround,(ProcPtr)&DSFrameRect,      \
                     whiteColor,a,0)
#define DSFillThumb(a)                                 \
        DSDrawObject(dPtr->cHdl,(ProcPtr)&DSGetBackGround,       \
                     (ProcPtr)&DSSetBackGround,(ProcPtr)&DSFillRect,      \
                     whiteColor,a,(long)dPtr->blackPat)
#define DSFrameArrow(a)                                \
        DSDrawObject(dPtr->cHdl,(ProcPtr)&DSGetForeGround,       \
                     (ProcPtr)&DSSetForeGround,(ProcPtr)&DSFramePoly,      \
                     whiteColor,(long)a,0)
#define DSClearArrow(a)                                \
        DSDrawObject(dPtr->cHdl,(ProcPtr)&DSGetBackGround,       \
                     (ProcPtr)&DSSetBackGround,(ProcPtr)&DSErasePoly,      \
                     cBodyColor,(long)a,0)
#define DSFillArrow(a)                                 \
        DSDrawObject(dPtr->cHdl,(ProcPtr)&DSGetForeGround,       \
                     (ProcPtr)&DSSetForeGround,(ProcPtr)&DSFillPoly,       \
                     blackColor,(long)a,(long)dPtr->blackPat)
/* Define forward refs   */
	void DSInitControl(DSPTR dPtr, ControlPtr cPtr);
	void DSCalcThumb(DSPTR dPtr, ControlPtr cPtr);
	void DSDrawThumb(DSPTR dPtr, ControlHandle cHdl);
	void DSPositionThumb(DSPTR dPtr, ControlHandle cHdl, Point pt);
	void DSSetupThumb(DSPTR dPtr, THUMBPTR thumbPtr);
	void DSDrawHBtn(DSPTR dPtr, PolyHandle btn, short fill);
	void DSDrawLBtn(DSPTR dPtr, PolyHandle btn, short fill);
	void DSFrameButtons(DSPTR dPtr);
	long DSTestControl(DSPTR dPtr, Point pt);
	void DSSetScrollRgn(DSPTR dPtr, Rect* scrollRgn);
	void DSGetBackGround(RGBColor *color);
	void DSGetForeGround(RGBColor *color);
	void DSSetBackGround(RGBColor *color);
	void DSSetForeGround(RGBColor *color);
	void DSEraseRect(Rect *rect);
	void DSFrameRect(Rect *rect);
	void DSFillRect(Rect *rect, Pattern *pattern);
	void DSErasePoly(PolyHandle poly);
	void DSFramePoly(PolyHandle poly);
	void DSFillPoly(PolyHandle poly, Pattern *pattern);
	void DSDrawObject(ControlHandle cHdl, ProcPtr getGround, ProcPtr setGround, ProcPtr object, short color, long arg1, long arg2);


	pascal long DarkScrollbarDef(short varCode, ControlHandle theControl, short message, long param)
	{
		ControlPtr   cPtr;         /* Working control ptr   */
		DSHDL        dHdl;         /* Working dual ctrl hdl */
		DSPTR        dPtr = NIL;   /* Working dual ctrl ptr */
		Rect         wRect;        /* Working scroll area   */
		SysEnvRec    sysEnv;       /* Working system envt   */
		long         status = 0;   /* Working status flag   */

		HLock((Handle)theControl);         /* Lock down the ctrl hdl*/
		cPtr = *theControl;        /* Set control pointer   */
		if (message != initCntl) { /* Check if already init */
			if (cPtr->contrlData) {   /* Check if valid handle */
				HLock(cPtr->contrlData); /* Lock it down for work */
				dPtr = *((DSHDL)cPtr->contrlData);/* Set dual ptr */
			}
		}
		switch (message) {          /* Process specified msg */
		case drawCntl:            /* Draw the control      */
			if (dPtr) {              /* Check if it’s valid   */
				if (cPtr->contrlVis) {  /* Check if it’s visible */
					PenNormal();           /* Set to normal pen     */
					switch (LoWord(param)) {/* Switch on control part*/
					case 0:               /* Re-draw scroll bar    */
						DSInitControl(dPtr, cPtr);/* Re-init it        */
						//DSFrameBody((long)&cPtr->contrlRect);/* Init scroll */
						//DSFrameButtons(dPtr);/* Frame the buttons     */


						if (cPtr->contrlHilite != 255 &&
							cPtr->contrlMax != cPtr->contrlMin)
						{
							DSFrameHUpBtn();
							DSFrameLDownBtn();
						}

						DSDrawThumb(dPtr, theControl); break;
					case 129:             /* Value/Min/Max changed */
						DSDrawThumb(dPtr, theControl); 

						if (cPtr->contrlHilite != 255 &&
							cPtr->contrlMax != cPtr->contrlMin)
						{
							DSFrameHUpBtn();
							DSFrameLDownBtn();
						}
						
						
						break;
					case 0xff:            /* Control to be inactive*/
						DSSetScrollRgn(dPtr, &wRect);
						DSClearBody((long)&wRect); break;
					case kControlPageUpPart:        /* Hilite scroll bar     */
					case kControlPageDownPart:
						DSDrawThumb(dPtr, theControl); break;
					case kControlUpButtonPart:      /* Hilite high up button */
						if (cPtr->contrlHilite != 0 &&
							cPtr->contrlMin != cPtr->contrlMax)
						//	DSFrameHUpBtn();
						//else
							DSFillHUpBtn();

						break;
					case kControlDownButtonPart:    /* Hilite high down buttn*/
						if (cPtr->contrlHilite != 0 &&
							cPtr->contrlMin != cPtr->contrlMax)
						//	DSFrameLDownBtn();
						//else
							DSFillLDownBtn();
					}
				}
			}
			break;
		case testCntl:            /* Test the control      */
			if (dPtr)                /* Check if ptr valid    */
			{
				if (cPtr->contrlHilite != -1)/* Check if active  */
				{
					if (cPtr->contrlMin != cPtr->contrlMax)
					{
						Point point;
						point.v = HiWord(param);
						point.h = LoWord(param);

						if (MacPtInRect(point, &cPtr->contrlRect))
						{
							status = DSTestControl(dPtr, point);/* Find it */
						}
					}
				}
			}
			break;
		case calcCRgns:           /* Calculate ctrl region */
			if (dPtr) {              /* Check if pointer valid*/
				if (!(param & 0x80000000L))/* Check if whole ctrl*/
					RectRgn((RgnHandle)(param & 0x7fffffffL),
						&cPtr->contrlRect);
				else {                  /* Want thumb’s region   */
										/* Setup thumb’s area    */
					RectRgn((RgnHandle)(param & 0x7fffffffL),
						&dPtr->cThumb);
					/* Setup drag pattern    */
					//BlockMove(dPtr->ltGrayPat, DragPattern, 8L); // AS: Removed
				}
			}
			break;
		case calcCntlRgn:         /* Calculate cntl region */
			if (dPtr)                /* Check if ptr valid    */
				RectRgn((RgnHandle)(param), &cPtr->contrlRect);
			break;
		case calcThumbRgn:        /* Calculate “thumb” rgn */
			if (dPtr) {              /* Check if pointer valid*/
									 /* Setup thumb’s region  */
				RectRgn((RgnHandle)(param), &dPtr->cThumb);
				/* Setup drag pattern    */
				//BlockMove(dPtr->ltGrayPat, DragPattern, 8L); // AS: Removed
			}
			break;
		case initCntl:            /* Initialized control   */
								  /* Allocate data handle  */
			if (dHdl = (DSHDL)NewHandle(sizeof(DS))) {
				HLock((Handle)dHdl);            /* Lock it down for init */
				dPtr = *dHdl;           /* Set dual control ptr  */
				dPtr->cHdl = theControl;/* Save it for reference */
				dPtr->up = dPtr->down = NIL;/* Init poly handles */
				dPtr->blackPat[0] = 0xffffffffL;/* Init patterns */
				dPtr->blackPat[1] = 0xffffffffL;
				dPtr->ltGrayPat[0] = 0x88228822L;
				dPtr->ltGrayPat[1] = 0x88228822L;
				dPtr->hasColor = FALSE; /* Set to black & white  */
										/* Check SysEnvirons OK  */
				if ((long)NGetTrapAddress(SysEnvironsTrap, OSTrap) !=
					(long)NGetTrapAddress(UnknownTrap, ToolTrap)) {
					SysEnvirons(1, &sysEnv);/* Get system enviroment */
					dPtr->hasColor = sysEnv.hasColorQD;/* Save color*/
				}
				DSInitControl(dPtr, cPtr);/* Init control’s stuff */
				cPtr->contrlAction = (ControlActionUPP)(-1L);
				cPtr->contrlData = (Handle)dHdl;/* Save data hdl */
			}
			break;
		case dispCntl:            /* Dispose the control   */
			if (dPtr) {              /* Check if pointer valid*/
				if (dPtr->up)           /* Kill the polys        */
					KillPoly(dPtr->up);
				if (dPtr->down)
					KillPoly(dPtr->down);
				HUnlock(cPtr->contrlData);/* Unlock it now       */
				DisposeHandle(cPtr->contrlData);/* Return it      */
			}
			cPtr->contrlData = NIL;  /* Clear it              */
			break;
		case posCntl:             /* Re-position control   */
			if (dPtr)                /* Check if pointer valid*/
			{
				Point point;
				point.v = HiWord(param);
				point.h = LoWord(param);

				DSPositionThumb(dPtr, theControl, point);
			}
			break;
		case thumbCntl:           /* Calculate for dragging*/
			if (dPtr)                /* Check if pointer valid*/
			{
				DSSetupThumb(dPtr, (THUMBPTR)param);/* Setup for dragging   */
			}
			break;
		case dragCntl:            /* Drag the control      */
			status = 0;              /* Only drag thumb!      */
			break;
		case autoTrack:           /* Execute ctrl’s action */
			break;
		}
		if (cPtr->contrlData)      /* Check if valid pointer*/
			HUnlock(cPtr->contrlData);/* Unlock for memory mgr */
		HUnlock((Handle)theControl);       /* Unlock control handle */
		return(status);            /* Return status code    */
	}

	void DSInitControl(DSPTR dPtr, ControlPtr cPtr)
	{
		PolyHandle pHdl;          /* Working poly handle   */
		short      bLen;          /* Working button length */
		short      hBLen;         /* Working 1/2 button len*/
		short      hLen;          /* Working horizontal len*/
		short      vLen;          /* Working vertical len  */
		short      tmp1, tmp2;     /* Working tmp variables */
		Rect       cRect;         /* Working ctrl rect area*/

		cRect = cPtr->contrlRect; /* Set scroll bar area   */
		MacInsetRect(&cRect, 1, 1);    /* Shrink it by one-pixel*/
									/* Set horz & vert lens  */
		hLen = abs(cRect.right - cRect.left) + 1;
		vLen = abs(cRect.bottom - cRect.top) + 1;

		if (hLen > vLen) {        /* Check if horz longer  */
			dPtr->bLen = vLen;       /* Set button length     */
			dPtr->sLen = hLen + 1;   /* Set scroll bar length */
			dPtr->hFactor = 1;       /* Set horizontal factor */
			dPtr->vFactor = 0;       /* Set vertical factor   */
		}
		else {                    /* Nope, must be vertical*/
			dPtr->bLen = hLen;       /* Set button length     */
			dPtr->sLen = vLen + 1;   /* Set scroll bar length */
			dPtr->hFactor = 0;       /* Set horizontal factor */
			dPtr->vFactor = 1;       /* Set vertical factor   */
		}
		bLen = dPtr->bLen;        /* Set button length     */
		hBLen = dPtr->hBLen = bLen / 2;/* Set 1/2 buttn len*/
		dPtr->thumb.top = dPtr->thumb.left = 0;/* Init pt  */
		if (dPtr->hFactor) {      /* Check if horz position*/
			dPtr->thumb.bottom = bLen - 1;
			dPtr->thumb.right = hBLen + hBLen / 2;
		}
		else {                    /* Nope, vert position   */
			dPtr->thumb.bottom = hBLen + hBLen / 2;
			dPtr->thumb.right = bLen - 1;
		}
		tmp1 = hBLen - 1;         /* Set tmp calculations  */
		tmp2 = hBLen - 2;
		/* Create up poly handle */
		if (pHdl = OpenPoly()) {  /* Check if got poly hdl */
			if (dPtr->hFactor) {     /* Check horz position   */
				MoveTo(0, tmp1);
				MacLineTo(tmp2, tmp2 * 2 + 1);
				MacLineTo(tmp2, 1);
				MacLineTo(0, tmp1);
			}
			else {                   /* Nope, vert position   */
				MoveTo(1, tmp2);
				MacLineTo(tmp1, 0);
				MacLineTo(tmp2 * 2 + 1, tmp2);
				//MacLineTo(tmp1, 0);
			}
			ClosePoly();             /* Close the poly handle */
		}
		if (dPtr->up)             /* Check if old poly hdl */
			KillPoly(dPtr->up);      /* Release this poly hdl */
		dPtr->up = pHdl;          /* Set up poly handle    */
								  /* Create down poly hdl  */
		if (pHdl = OpenPoly()) {  /* Check if got poly hdl */
			if (dPtr->hFactor) {     /* Check horz position   */
				MoveTo(tmp2, tmp1);
				MacLineTo(0, tmp2 * 2 + 1);
				MacLineTo(0, 1);
				MacLineTo(tmp2, tmp1);
				OffsetPoly(pHdl, hBLen, 0);
			}
			else {                   /* Nope,  vert position  */
				//MoveTo(1, 0);
				MoveTo(tmp2 * 2 + 1, 0);
				MacLineTo(tmp1, tmp1-1);
				MacLineTo(1, 0);
				OffsetPoly(pHdl, 0, hBLen);
			}
			ClosePoly();             /* Close the poly handle */
		}
		if (dPtr->down)           /* Check if old poly hdl */
			KillPoly(dPtr->down);    /* Release this poly hdl */
		dPtr->down = pHdl;        /* Set down poly handle  */
	}

	void DSCalcThumb(DSPTR dPtr, ControlPtr cPtr)
	{
		double    tmp1, tmp2;      /* Working tmp registers */
		short     offset;         /* Working pixel offset  */
		Rect      wRect;          /* Working scroll area   */

								  /* Make sure this is min */
		cPtr->contrlMin = min(cPtr->contrlMin, cPtr->contrlMax);

		/* Make sure this is max */
		cPtr->contrlMax = max(cPtr->contrlMin, cPtr->contrlMax);

		/* Make it min<value<max */
		cPtr->contrlValue = min(cPtr->contrlMax,
			max(cPtr->contrlMin, cPtr->contrlValue));

		DSSetScrollRgn(dPtr, &wRect);/* Set scroll region   */
									 /* Init thumb factor calc*/
		tmp1 = max(abs(wRect.bottom - wRect.top) * dPtr->vFactor,
			abs(wRect.right - wRect.left) * dPtr->hFactor)
			- dPtr->bLen + dPtr->hBLen - 2;
		tmp2 = abs(cPtr->contrlMax - cPtr->contrlMin);

		dPtr->tFactor = tmp1 / tmp2;/* Set relative factor */
		tmp1 = cPtr->contrlValue - /*Set thumb pixel offset*/
			cPtr->contrlMin;
		tmp2 = tmp1 * dPtr->tFactor;
		offset = tmp2;
		dPtr->cThumb = dPtr->thumb;/* Set thumb rect area  */
		MacOffsetRect(&dPtr->cThumb, wRect.left + offset * dPtr->hFactor, wRect.top
			+ offset * dPtr->vFactor);
	}

	void DSDrawThumb(DSPTR dPtr, ControlHandle cHdl)
	{
		Rect         tRect;        /* Working thumb area    */
		ControlPtr   cPtr = *cHdl; /* Working control ptr   */

		DSSetScrollRgn(dPtr, &tRect);/* Set scroll region    */
		if (cPtr->contrlHilite != 255 &&/* Check if show it */
			cPtr->contrlMax != cPtr->contrlMin) {
			DSCalcThumb(dPtr, cPtr);   /* Re-calculate thumb    */
			ForeColor((long)(whiteColor));/* Make it’s b & w   */
			BackColor((long)(blackColor));
			DSFillBody((long)&tRect);       /* Fill in scroll bar    */
			tRect = dPtr->cThumb;     /* Set thumb rect area   */
			DSFrameThumb((long)&tRect);     /* Frame thumb           */
			MacInsetRect(&tRect, 1, 1);    /* Inset by one pixel    */
			DSFillThumb((long)&tRect);      /* Clear inside of thumb */
		}
		else                       /* Nope, don’t want it   */
			DSClearBody((long)&tRect);      /* Clr scroll region     */
	}

	void DSPositionThumb(DSPTR dPtr, ControlHandle cHdl, Point pt)
	{
		double       offset;       /* Working value offset  */

		if (dPtr->hFactor)         /* Check if horizontal   */
			offset = pt.h;            /* Use horizontal offset */
		else                       /* Nope, it’s vertical   */
			offset = pt.v;            /* Use vertical offset   */
		offset = offset / dPtr->tFactor;/* Reset ctrl value */
		(*cHdl)->contrlValue += offset;
		DSDrawThumb(dPtr, cHdl);    /* Draw da thumb, again! */
	}

	void DSSetupThumb(DSPTR dPtr, THUMBPTR thumbPtr)
	{
		Point   msePt;             /* Working mouse point   */

		msePt.h = thumbPtr->limitRect.left;/* Save mse down */
		msePt.v = thumbPtr->limitRect.top;
		DSSetScrollRgn(dPtr, &thumbPtr->limitRect);/*Set limt*/
		if (dPtr->hFactor) {       /* Check if horz scroll  */
								   /* Adjust it for mse     */
			thumbPtr->limitRect.left += msePt.h - dPtr->cThumb.left;
			thumbPtr->limitRect.right -= dPtr->cThumb.right - msePt.h - 1;
			/* Set slop rect area    */
			thumbPtr->slopRect.top = thumbPtr->limitRect.top - 16;
			thumbPtr->slopRect.bottom = thumbPtr->limitRect.bottom + 16;
			thumbPtr->slopRect.left = -32000;
			thumbPtr->slopRect.right = 32000;
			thumbPtr->axis = hAxisOnly;/* Set axis dragging dir*/
		}
		else {                     /* Nope,  vert scroll bar*/
								   /* Adjust it for mse     */
			thumbPtr->limitRect.top += msePt.v - dPtr->cThumb.top;
			thumbPtr->limitRect.bottom -= dPtr->cThumb.bottom - msePt.v - 1;
			/* Set slop rect area    */
			thumbPtr->slopRect.left = thumbPtr->limitRect.left - 16;
			thumbPtr->slopRect.right = thumbPtr->limitRect.right + 16;
			thumbPtr->slopRect.top = -32000;
			thumbPtr->slopRect.bottom = 32000;
			thumbPtr->axis = vAxisOnly;/* Set axis dragging dir*/
		}
	}

	void DSDrawHBtn(DSPTR dPtr, PolyHandle btn, short fill)
	{
		/* Offset where button   */
		OffsetPoly(btn, (*dPtr->cHdl)->contrlRect.left + 1,
			(*dPtr->cHdl)->contrlRect.top + 1);
		//if (fill)                  /* Check if filling arrow*/
		//	DSFillArrow(btn);         /* Fill in arrow button  */
		//else                       /* Nope, not clearing it */
		//	DSClearArrow(btn);        /* Clear the arrow button*/
		DSFrameArrow(btn);         /* Frame the arrow button*/
								   /* Restore the button    */
		OffsetPoly(btn, -(*dPtr->cHdl)->contrlRect.left - 1,
			-(*dPtr->cHdl)->contrlRect.top - 1);
	}

	void DSDrawLBtn(DSPTR dPtr, PolyHandle btn, short fill)
	{
		short    tmp1, tmp2;        /* Working tmp variables */
		short    offset;           /* Working offset        */

		offset = dPtr->sLen - dPtr->bLen - 1;/* Set offset  */
											 /* Offset where button   */
		OffsetPoly(btn, tmp1 = (*dPtr->cHdl)->contrlRect.left +
			dPtr->hFactor * offset + 1, tmp2 = (*dPtr->cHdl)->contrlRect.top
			+ dPtr->vFactor * offset + 1);
		//if (fill)                 /* Check if filling arrow */
		//	DSFillArrow(btn);        /* Fill in arrow button   */
		//else                      /* Nope, not clearing it  */
		//	DSClearArrow(btn);       /* Clear the arrow button */
		DSFrameArrow(btn);        /* Frame the arrow button */
		OffsetPoly(btn, -tmp1, -tmp2);/* Restore the button   */
	}

	void DSFrameButtons(DSPTR dPtr)
	{
		Rect       bRect;          /* Working box rect area */
		short      offset;         /* Working low button    */

		bRect.left = bRect.top = 0;/* Setup rect area       */
		bRect.right = bRect.bottom = dPtr->bLen + 1;
		/* Offset high button box*/
		MacOffsetRect(&bRect, (*dPtr->cHdl)->contrlRect.left,
			(*dPtr->cHdl)->contrlRect.top);
		DSClearBody((long)&bRect);       /* Clear the body part   */
		DSFrameBody((long)&bRect);       /* Frame high button box */
								   /* Set low button offset */
		offset = dPtr->sLen - dPtr->bLen - 1;
		MacOffsetRect(&bRect, dPtr->hFactor * offset,
			dPtr->vFactor * offset);
		DSClearBody((long)&bRect);       /* Clear the body part   */
		DSFrameBody((long)&bRect);       /* Frame low button box  */
	}

	long    DSTestControl(DSPTR dPtr, Point pt)
	{
		Rect       cRect;          /* Working control rect  */
		Rect       tRect;          /* Working temp rect area*/
		Rect       bRect;          /* Working button rect   */
		short      offset;         /* Working offset        */
		short      point;          /* Working pt in region  */
		long       status = 0;     /* Working status flag   */

		cRect = (*dPtr->cHdl)->contrlRect;/* Set ctrl rect  */
		bRect = dPtr->thumb;       /* Set button rect area  */
		if (dPtr->hFactor)         /* Check if horz button  */
			bRect.right = dPtr->hBLen;/* Adjust for horz button*/
		else
			bRect.bottom = dPtr->hBLen;/* Adjust for vert buttn*/
		tRect = bRect;             /* Set the temp rect area*/
		MacOffsetRect(&tRect, cRect.left, cRect.top);
		if (MacPtInRect(pt, &tRect)) { /* Check if high up buttn*/
			status = kControlUpButtonPart;      /* Set high up button    */
		}

		if (!status) {             /* Check if prev part    */
								   /* Offset where low down */
			tRect = bRect;
			offset = dPtr->sLen - dPtr->bLen;
				MacOffsetRect(&tRect, cRect.left + dPtr->hFactor*offset,
					cRect.top + dPtr->vFactor*offset);
			MacOffsetRect(&tRect, dPtr->hFactor * dPtr->hBLen,
				dPtr->vFactor * dPtr->hBLen);
			if (MacPtInRect(pt, &tRect)) {/* Check if the low down */
				status = kControlDownButtonPart;  /* Set low down button    */
			}

		}
		if (!status) {             /* Check if prev part    */
			if (MacPtInRect(pt, &dPtr->cThumb))/* Check if thumb   */
				status = kControlIndicatorPart;        /* Set thumb part        */
		}
		if (!status) {             /* Check if prev part    */
								   /* Set relative point    */
			point = pt.h * dPtr->hFactor + pt.v * dPtr->vFactor;
			/* Set relative offset   */
			offset = dPtr->cThumb.left * dPtr->hFactor +
				dPtr->cThumb.top * dPtr->vFactor;
			if (point > offset)       /* Check if in down page */
				status = kControlPageDownPart;     /* Set down page region  */
			else
				status = kControlPageUpPart;       /* Set up page region    */
		}
		return(status);            /* Return status part    */
	}

	void DSSetScrollRgn(DSPTR dPtr, Rect* scrollRgn)
	{
		*scrollRgn = (*dPtr->cHdl)->contrlRect;/* Set area */
		MacInsetRect(scrollRgn, 1, 1);/* Shrink it a bit        */
		if (dPtr->hFactor) {     /* Check if horizontal bar*/
			scrollRgn->left += dPtr->bLen;/* Adjust rect area */
			scrollRgn->right -= dPtr->bLen;
		}
		else {                   /* It’s a vert scroll bar */
			scrollRgn->top += dPtr->bLen;/* Adjust rect area  */
			scrollRgn->bottom -= dPtr->bLen;
		}
	}

	void DSGetBackGround(RGBColor *color)
	{
		GetBackColor(color);
	}

	void DSGetForeGround(RGBColor *color)
	{
		GetForeColor(color);
	}

	void DSSetBackGround(RGBColor *color)
	{
		RGBBackColor(color);
	}

	void DSSetForeGround(RGBColor *color)
	{
		RGBForeColor(color);
	}

	void DSEraseRect(Rect *rect)
	{
		EraseRect(rect);
	}

	void DSFrameRect(Rect *rect)
	{
		MacFrameRect(rect);
	}

	void DSFillRect(Rect *rect, Pattern *pattern)
	{
		MacFillRect(rect, pattern);
	}

	void DSErasePoly(PolyHandle poly)
	{
		ErasePoly(poly);
	}

	void DSFramePoly(PolyHandle poly)
	{
		FramePoly(poly);
	}
	void DSFillPoly(PolyHandle poly, Pattern *pattern)
	{
		FillPoly(poly, pattern);
	}

	void DSDrawObject(ControlHandle cHdl, ProcPtr getGround, ProcPtr setGround, ProcPtr object, short color, long arg1, long arg2)
	{
		AuxCtlHandle   acHdl = NIL;  /* Working aux color hdl */
		RGBColor     oldColor;     /* Working old color     */
		RGBColor     newColor;     /* Working new color     */
								   /* Check if has color    */
		if ((*((DSHDL)((*cHdl)->contrlData)))->hasColor) {
			GetAuxiliaryControlRecord(cHdl, &acHdl);   /* Get control color info*/
			if (acHdl) {              /* Check if really got it*/
				(*getGround)(&oldColor); /* Get back-ground color */
										 /* Get object’s color    */
				newColor = (*(*acHdl)->acCTable)->ctTable[color].rgb;
				(*setGround)(&newColor); /* Set control obj color */
			}
		}
		(*object)(arg1, arg2);      /* Draw the object       */
		if (acHdl)                 /* Check if have aux hdl */
			(*setGround)(&oldColor);  /* Restore back-grd color*/
	}
}