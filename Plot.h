#pragma once

inline double coordinate_translation_x(int width, int X, double xmax, double xmin)
{
    return (xmax - xmin) * X / width + xmin;
}

inline double coordinate_translation_y(int height, int Y, double ymax, double ymin)
{
    return (ymax - ymin) * Y / height + ymin;
}

inline int back_coordinate_translation_x(int width, double x, double xmax, double xmin)
{
    return (int) ((x - xmin) * width / (xmax - xmin));
}

inline int back_coordinate_translation_y(int height, double y, double ymax, double ymin)
{
    return (int) (height - height * (y - ymin) / (ymax - ymin));
}

inline CString format_string(double number)
{
    CString string;
    string.Format(L"%.2lf", number);
    return string;
}

class Plot : public CStatic
{
	DECLARE_DYNAMIC(Plot)

public:
	int    grid_x;
	int    grid_y;
	double xmin;
	double xmax;
	double ymin;
	double ymax;
	int    n;
	double *points;
	double interval;
	double x0;
	Plot();
	virtual ~Plot();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


