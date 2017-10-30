
// FilterDlg.h : файл заголовка
//

#pragma once
#include "Plot.h"
#include "RawSignalDialog.h"


// диалоговое окно CFilterDlg
class CFilterDlg : public CDialogEx
{
// Создание
public:
	CFilterDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
	enum { IDD = IDD_FILTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
    volatile bool timer_set;
    Plot mr_plot_ctrl;
    Plot pr_plot_ctrl;
    afx_msg void OnBnClickedButton3();
    RawSignalDialog raw_signal_dialog;
    double frequency_step;
    void UpdateFRPlot(size_t points_to_draw, double ymax, double pymax);
    void UpdateIOPlot();
    volatile bool stop;
    afx_msg void OnBnClickedButton4();
    double initial_frequency_do;
    double current_frequency_do;
    double target_frequency_do;
    LRESULT OnUpdateDataMessage(WPARAM wpD, LPARAM lpD);
    BOOL debug_output_to_file;
    int update_rate;
};
