#include <Quickdraw.h>
#include <Lists.h>
#include <ToolUtils.h>
#include <Resources.h>
#include "DarkListDef.h"

extern "C"
{

	extern pascal long DarkScrollbarDef(short varCode, ControlHandle theControl, short message, long param);


	pascal void DarkListDef(short message, Boolean selected, Rect* cellRect, Cell cell, short dataOffset, short dataLen, ListHandle list)
	{
		switch (message)
		{
			case lDrawMsg:
			case lHiliteMsg:
				DrawCell(selected, cellRect, cell, dataOffset, dataLen, list);
				break;
			case lCloseMsg:
				Dispose(list);
				break;
		}
	}

	pascal void DrawCell(Boolean selected, Rect* cellRect, Cell cell, short dataOffset, short dataLen, ListHandle list)
	{
		const int cellWidth = 56;
		const int iconSize = 32;
		const int iconOffsetLeft = 12;
		const int iconOffsetTop = 5;

		// Snapshot drawing environment
		GrafPtr savedPort;
		RgnHandle savedClip;
		PenState savedPenState;

		GetPort(&savedPort); 
		MacSetPort((**list).port);
		savedClip = NewRgn();
		GetClip(savedClip);
		ClipRect(cellRect);
		GetPenState(&savedPenState);

		PenNormal();
		EraseRect(cellRect);

		Str255 label;
		LGetCell(&label, (short*)&dataLen, cell, list);
		
		// Draw the label
		TextFont(1);
		TextSize(9);

		MoveTo(cellRect->left, cellRect->top + 10);


		ForeColor(whiteColor);
		
		short textMode = srcCopy;

		if (selected)
		{
			//TextFace(bold);
			textMode = notSrcCopy;
		}

		TextMode(textMode);
		DrawString((ConstStr255Param)label);
		ForeColor(blackColor);
		// Restore graphics environment
		MacSetPort(savedPort);
		SetClip(savedClip);
		DisposeRgn(savedClip);
		SetPenState(&savedPenState);
		TextFont(0);
		TextSize(0);
		TextFace(normal);
	}

	pascal void Dispose(ListHandle list)
	{
		Cell cell;
		Str255 label;
		int dataLength;

		SetPt(&cell, 0, 0);
		if (MacPtInRect(cell, &(**list).dataBounds))
		{
			do
			{
				dataLength = sizeof(Str255);
				LGetCell(&label, (short*)&dataLength, cell, list);
				if (dataLength == sizeof(Str255))
				{
					delete &label;
				}
			} while (LNextCell(true, true, &cell, list));
		}
	}
}