// RawSignalDialog.cpp : implementation file
//

#include "stdafx.h"
#include "Filter.h"
#include "RawSignalDialog.h"
#include "afxdialogex.h"


// RawSignalDialog dialog

IMPLEMENT_DYNAMIC(RawSignalDialog, CDialog)

RawSignalDialog::RawSignalDialog(CWnd* pParent /*=NULL*/)
	: CDialog(RawSignalDialog::IDD, pParent)
{

	EnableAutomation();

}

RawSignalDialog::~RawSignalDialog()
{
}

void RawSignalDialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void RawSignalDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_I, input_plot_ctrl);
    DDX_Control(pDX, IDC_O, output_plot_ctrl);
}


BEGIN_MESSAGE_MAP(RawSignalDialog, CDialog)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(RawSignalDialog, CDialog)
END_DISPATCH_MAP()

// Note: we add support for IID_IRawSignalDialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {39FBEB14-4217-44A9-ABDB-2DD168790467}
static const IID IID_IRawSignalDialog =
{ 0x39FBEB14, 0x4217, 0x44A9, { 0xAB, 0xDB, 0x2D, 0xD1, 0x68, 0x79, 0x4, 0x67 } };

BEGIN_INTERFACE_MAP(RawSignalDialog, CDialog)
	INTERFACE_PART(RawSignalDialog, IID_IRawSignalDialog, Dispatch)
END_INTERFACE_MAP()


// RawSignalDialog message handlers
