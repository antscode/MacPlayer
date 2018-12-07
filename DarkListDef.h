#include <Lists.h>
#include <string>
#include <vector>

using namespace std;

extern "C"
{
	struct ListCell
	{
		Str255 Content;
		double WidthPercent;
	};

	struct ListRow
	{
		int CellCount;
		ListCell Cells[5];
	};

	pascal void DarkListDef(short message, Boolean selected, Rect* cellRect, Cell cell, short dataOffset, short dataLen, ListHandle list);
	pascal void DrawCell(Boolean selected, Rect* cellRect, Cell cell, short dataOffset, short dataLen, ListHandle list);
	pascal void Dispose(ListHandle list);
}
