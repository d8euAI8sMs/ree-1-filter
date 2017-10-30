#pragma once
#include "Plot.h"


// RawSignalDialog dialog

class RawSignalDialog : public CDialog
{
	DECLARE_DYNAMIC(RawSignalDialog)

public:
	RawSignalDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~RawSignalDialog();

	virtual void OnFinalRelease();

// Dialog Data
	enum { IDD = IDD_RSD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
    Plot input_plot_ctrl;
    Plot output_plot_ctrl;
};
