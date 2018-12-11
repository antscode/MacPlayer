#include <Quickdraw.h>
#include <Lists.h>
#include <ToolUtils.h>
#include <Resources.h>
#include <string.h>
#include "Util.h"
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
		short textMode;

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

		TextFont(1);
		TextSize(9);
		ForeColor(whiteColor);

		ListRow row;
		LGetCell(&row, (short*)&dataLen, cell, list);

		double offset = 0;

		for (int i = 0; i < row.CellCount; i++)
		{
			textMode = srcCopy;

			if (i == 0 && selected)
			{
				textMode = notSrcCopy;
			}

			TextMode(textMode);
			MoveTo(cellRect->left + offset, cellRect->top + 10);
			DrawString((ConstStr255Param)row.Cells[i].Content);

			offset += ((cellRect->right - cellRect->left) * row.Cells[i].WidthPercent);
		}

		// Restore graphics environment
		MacSetPort(savedPort);
		SetClip(savedClip);
		DisposeRgn(savedClip);
		SetPenState(&savedPenState); 
		TextMode(srcCopy);
	}

	pascal void Dispose(ListHandle list)
	{
		Cell cell;
		ListRow row;
		int dataLength;

		SetPt(&cell, 0, 0);
		if (MacPtInRect(cell, &(**list).dataBounds))
		{
			do
			{
				dataLength = sizeof(ListRow);
				LGetCell(&row, (short*)&dataLength, cell, list);
				if (dataLength == sizeof(ListRow))
				{
					delete &row;
				}
			} while (LNextCell(true, true, &cell, list));
		}
	}
}