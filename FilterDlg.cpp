// FilterDlg.cpp : файл реализации
//

#include "stdafx.h"
#include "Filter.h"
#include "FilterDlg.h"
#include "afxdialogex.h"
#include "RawSignalDialog.h"
#include "common.h"

#include "AFC.h"

#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#define WM_UPDATEDATA_MESSAGE (WM_USER+1000)
#endif

//#define MOCK_ADC


fr_data_t   mr_data,    pr_data;
volt_data_t input_data, output_data;





CFilterDlg::CFilterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFilterDlg::IDD, pParent)
	, timer_set(false)
    , raw_signal_dialog(this)
    , frequency_step(max_frequency / 1000)
    , stop(false)
    , initial_frequency_do(0)
    , current_frequency_do(0)
    , target_frequency_do(0)
    , debug_output_to_file(FALSE)
    , update_rate(1)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    mr_plot_ctrl.interval = frequency_step;
    mr_plot_ctrl.x0       = min_frequency;
    mr_plot_ctrl.xmin     = min_frequency;
    mr_plot_ctrl.xmax     = max_frequency;
    mr_plot_ctrl.ymin     = 0;
    mr_plot_ctrl.ymax     = 1;
    mr_plot_ctrl.grid_x   = 10;
    mr_plot_ctrl.grid_y   = 10;
    mr_plot_ctrl.n        = 0;
    mr_plot_ctrl.points   = mr_data;

    pr_plot_ctrl.interval = frequency_step;
    pr_plot_ctrl.x0       = min_frequency;
    pr_plot_ctrl.xmin     = min_frequency;
    pr_plot_ctrl.xmax     = max_frequency;
    pr_plot_ctrl.ymin     = 0;
    pr_plot_ctrl.ymax     = 0;
    pr_plot_ctrl.grid_x   = 10;
    pr_plot_ctrl.grid_y   = 10;
    pr_plot_ctrl.n        = 0;
    pr_plot_ctrl.points   = pr_data;

    raw_signal_dialog.input_plot_ctrl.x0       = 0;
    raw_signal_dialog.input_plot_ctrl.interval = 1. / GetMaxADCSamplerate();
    raw_signal_dialog.input_plot_ctrl.xmin     = 0;
    raw_signal_dialog.input_plot_ctrl.xmax     = sample_count / GetMaxADCSamplerate();
    raw_signal_dialog.input_plot_ctrl.ymin     = -10;
    raw_signal_dialog.input_plot_ctrl.ymax     = 10;
    raw_signal_dialog.input_plot_ctrl.grid_x   = 10;
    raw_signal_dialog.input_plot_ctrl.grid_y   = 10;
    raw_signal_dialog.input_plot_ctrl.n        = sample_count;
    raw_signal_dialog.input_plot_ctrl.points   = input_data;
    
    raw_signal_dialog.output_plot_ctrl.x0       = 0;
    raw_signal_dialog.output_plot_ctrl.interval = 1. / GetMaxADCSamplerate();
    raw_signal_dialog.output_plot_ctrl.xmin     = 0;
    raw_signal_dialog.output_plot_ctrl.xmax     = sample_count / GetMaxADCSamplerate();
    raw_signal_dialog.output_plot_ctrl.ymin     = -10;
    raw_signal_dialog.output_plot_ctrl.ymax     = 10;
    raw_signal_dialog.output_plot_ctrl.grid_x   = 10;
    raw_signal_dialog.output_plot_ctrl.grid_y   = 10;
    raw_signal_dialog.output_plot_ctrl.n        = sample_count;
    raw_signal_dialog.output_plot_ctrl.points   = output_data;
}

void CFilterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MR, mr_plot_ctrl);
    DDX_Control(pDX, IDC_PR, pr_plot_ctrl);
    DDX_Text(pDX, IDC_FS, frequency_step);
    DDX_Text(pDX, IDC_DO_IF, initial_frequency_do);
    DDX_Text(pDX, IDC_DO_CF, current_frequency_do);
    DDX_Text(pDX, IDC_DO_TF, target_frequency_do);
    DDX_Check(pDX, IDC_CHECK1, debug_output_to_file);
    DDX_Text(pDX, IDC_US, update_rate);
}

BEGIN_MESSAGE_MAP(CFilterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CFilterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CFilterDlg::OnBnClickedButton2)
//	ON_WM_TIMER()
    ON_BN_CLICKED(IDC_BUTTON3, &CFilterDlg::OnBnClickedButton3)
    ON_BN_CLICKED(IDC_BUTTON4, &CFilterDlg::OnBnClickedButton4)
    ON_MESSAGE(WM_UPDATEDATA_MESSAGE, &CFilterDlg::OnUpdateDataMessage)
END_MESSAGE_MAP()


// обработчики сообщений CFilterDlg

BOOL CFilterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна. Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

    raw_signal_dialog.Create(RawSignalDialog::IDD);
    initial_frequency_do = min_frequency;
    target_frequency_do = max_frequency;
    current_frequency_do = min_frequency;

    UpdateData(FALSE);
    UpdateData(TRUE);

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок. Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CFilterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CFilterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



UINT OnCollect(LPVOID wnd);
void CFilterDlg::OnBnClickedButton1()
{
    UpdateData(TRUE);
    stop = false;
    AfxBeginThread(&OnCollect, this);
}


void CFilterDlg::OnBnClickedButton2()
{
	HAFC session = OpenDevice();
	SetDDSFrequency(session, 1288490201685L);
	Start(session, START_DDS);
	Sleep(3000);
	Stop(session, STOP_DDS);
}


//void CFilterDlg::OnTimer(UINT_PTR nIDEvent)
//{
//}


void CFilterDlg::OnBnClickedButton3()
{
    if (raw_signal_dialog.IsWindowVisible())
    {
        raw_signal_dialog.ShowWindow(SW_HIDE);
    } else
    {
        raw_signal_dialog.ShowWindow(SW_SHOW);
        raw_signal_dialog.UpdateWindow();
    }
}


void CFilterDlg::UpdateFRPlot(size_t points_to_draw, double ymax, double phase_ymax)
{
    mr_plot_ctrl.interval = pr_plot_ctrl.interval = frequency_step;

    mr_plot_ctrl.ymax = ymax;
    pr_plot_ctrl.ymax = phase_ymax;
    pr_plot_ctrl.ymin = -phase_ymax;

    mr_plot_ctrl.n = points_to_draw;
    pr_plot_ctrl.n = points_to_draw;

    mr_plot_ctrl.RedrawWindow();
    pr_plot_ctrl.RedrawWindow();
}

void CFilterDlg::UpdateIOPlot()
{
    if (raw_signal_dialog.IsWindowVisible())
    {
        raw_signal_dialog.input_plot_ctrl.RedrawWindow();
        raw_signal_dialog.output_plot_ctrl.RedrawWindow();
    }
}


UINT OnCollect(LPVOID wnd)
{
    CFilterDlg &self = *(reinterpret_cast<CFilterDlg*>(wnd));

    if (!self.timer_set) self.timer_set = true;
    else return 0;

    std::ofstream file;

    if (self.debug_output_to_file)
    {
        file.open("./debug_output.txt");
    }

    double max_mr = 0;
    double max_pr = 0;

    adc_data_t adc_data_1, adc_data_2;

#ifndef MOCK_ADC

    HAFC session = OpenDevice();

    SetADC0StartAddress(session, 0);
    SetADC0FinalAddress(session, sample_count);
    SetADC1StartAddress(session, sample_count);
    SetADC1FinalAddress(session, 2 * sample_count);

    SetADC0Samplerate(session, 0xFFFF);
    SetADC1Samplerate(session, 0xFFFF);

    SetDDSPhase(session, 0);

    Start(session, START_DDS);

#endif

    double frequency = 0.0; int frequency_index = 0;
    for (;
         frequency <= max_frequency && frequency_index < frequency_count && !self.stop;
         frequency += self.frequency_step, frequency_index++)
    {
#ifndef MOCK_ADC
        SetDDSFrequency(session, frequency2dds(frequency/1000000.));

        Start(session, START_ADC0 | START_ADC1);

        while (!(IsADC0Ready(session) && IsADC1Ready(session)))
        {
            Sleep(0);
        }

        Stop(session, STOP_ADC0 | STOP_ADC1);

        RdMemBlock(session, 0, sample_count, adc_data_1);
        RdMemBlock(session, sample_count, sample_count, adc_data_2);

        adc2volt(adc_data_1, input_data,  gain_no);
        adc2volt(adc_data_2, output_data, gain_base);
#endif
#ifdef MOCK_ADC
        for (size_t j = 0; j < sample_count; j++)
        {
            input_data[j] = 5 * sin(2 * pi * frequency * j / GetMaxADCSamplerate());
            output_data[j] = 10 * abs(cos(2 * pi * frequency / max_frequency)) * (frequency / max_frequency) * (frequency / max_frequency) * sin(2 * pi * frequency * j / GetMaxADCSamplerate() + 3.14 * sin(2 * pi * frequency / max_frequency));
        }
#endif

        if (self.debug_output_to_file)
        {
            file << frequency << std::endl << "{";
            for (size_t j = 0; j < sample_count; j++)
            {
                file << output_data[j] << (j != (sample_count-1) ? ", " : "");
            }
            file << "}" << std::endl << std::endl;
        }

        pair_t amp_and_ph_1 = fourier_at(input_data, frequency),
            amp_and_ph_2 = fourier_at(output_data, frequency);

        double k = amp_and_ph_2.first / amp_and_ph_1.first;
        double pfd = amp_and_ph_2.second - amp_and_ph_1.second;
        
		if (frequency_index != 0)
		{
			if (pr_data[frequency_index-1] - pfd >= 2*pi - 1e-1)
			{
				pfd += 2 * pi;
			}
			else if (pr_data[frequency_index-1] - pfd <= -2*pi + 1e-1)
			{
				pfd -= 2 * pi;
			}
		}

        if (k > 1000 || k < 0) k = 0;

        mr_data[frequency_index] = k;
        pr_data[frequency_index] = pfd;

        max_mr = max(max_mr, k);
		max_pr = max(max_pr, abs(pfd));

        self.current_frequency_do = frequency;

        if (frequency_index % self.update_rate == 0)
        {
            self.UpdateFRPlot(frequency_index + 1, max_mr, max_pr);
            self.UpdateIOPlot();
            self.SendMessage(WM_UPDATEDATA_MESSAGE);
        }
    }

    self.UpdateFRPlot(frequency_index, max_mr, max_pr);
    self.UpdateIOPlot();
    self.SendMessage(WM_UPDATEDATA_MESSAGE);

#ifndef MOCK_ADC

    Stop(session, STOP_DDS);

    CloseDevice(session);

#endif

    self.timer_set = false;
    self.stop = false;

    return 0;
}


void CFilterDlg::OnBnClickedButton4()
{
    stop = true;
}


LRESULT CFilterDlg::OnUpdateDataMessage(WPARAM wpD, LPARAM lpD)
{
    UpdateData(FALSE);
    return LRESULT();
}
