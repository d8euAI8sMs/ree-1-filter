#include "stdafx.h"
#include "Plot.h"

IMPLEMENT_DYNAMIC(Plot, CStatic)

Plot::Plot()
    : n(0)
{
}

Plot::~Plot()
{
}

BEGIN_MESSAGE_MAP(Plot, CStatic)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

void Plot::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect bounds; GetClientRect(&bounds);

	int offset_x = 50;
	int offset_y = 20;

	CDC &dc = *CDC::FromHandle(lpDrawItemStruct->hDC);

	CPen pen(PS_SOLID, 1, RGB(0, 0, 0));

	CBrush brush(RGB(255, 255, 255));
	dc.SelectObject(&pen);
	dc.FillRect(&bounds, &brush);
	bounds.left += offset_x;
	bounds.bottom -= offset_y;
	dc.MoveTo(offset_x, 0);
	dc.LineTo(offset_x, bounds.Height() - 1);
	dc.LineTo(offset_x, bounds.Height() - 1);
	dc.LineTo(bounds.Width() - 1 + offset_x, bounds.Height() - 1);
	dc.LineTo(bounds.Width() - 1 + offset_x, 0);
	dc.LineTo(offset_x, 0);

	for (int i = 0; i < grid_x; i++)
	{
		double x = xmin + i*(xmax - xmin) / grid_x;
		int X = back_coordinate_translation_x(bounds.Width(), x, xmax, xmin);
		dc.MoveTo(X + offset_x, 0);
		dc.LineTo(X + offset_x, bounds.Height());
		CString string = format_string(x);
		CSize size = dc.GetTextExtent(string);
		CRect text_bounds(CPoint(X + offset_x - size.cx / 2, bounds.Height()), size);
		dc.DrawText(string, &text_bounds, 0);
	}

	for (int i = 0; i < grid_y; i++)
	{
		double y = ymin + i*(ymax - ymin) / grid_y;
		int Y = back_coordinate_translation_y(bounds.Height(), y, ymax, ymin);
		dc.MoveTo(offset_x, Y);
		dc.LineTo(bounds.Width() + offset_x, Y);
		CString string = format_string(y);
		CSize size = dc.GetTextExtent(string);
		CRect text_bounds(CPoint(0, Y - size.cy / 2), size);
		dc.DrawText(string, &text_bounds, 0);
	}

	for (int i = 0; i < n; i++)
	{
		int X = back_coordinate_translation_x(bounds.Width(), x0 + i*interval, xmax, xmin);
		int Y = back_coordinate_translation_y(bounds.Height(), points[i], ymax, ymin);
		if (i == 0)
		{
			dc.MoveTo(X + offset_x, Y);
		}
		else
		{
			dc.LineTo(X + offset_x, Y);
		}
	}
}


void Plot::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CStatic::OnDrawItem(nIDCtl, lpDrawItemStruct);
}