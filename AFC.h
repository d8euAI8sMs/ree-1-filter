
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the AFCDLL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// AFCDLL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef AFCDLL_EXPORTS
#define AFCDLL_API __declspec(dllexport)
#else
#define AFCDLL_API __declspec(dllimport)
#endif

typedef unsigned long HAFC, FAR *PHAFC;

#define CTRL_ADC0START			1
#define ENABLE_CTRL_ADC0START	2
#define CTRL_ADC1START			4
#define ENABLE_CTRL_ADC1START	8
#define CTRL_DACSTART			16
#define ENABLE_CTRL_DACSTART	32
#define CTRL_DACCYCLIC			64
#define ENABLE_CTRL_DACCYCLIC	128
#define CTRL_ADC1GAIN			256
#define ENABLE_CTRL_ADC1GAIN	512
#define CTRL_DDSSTART			1024
#define ENABLE_CTRL_DDSSTART	2048

#define STAT_ISRAMRDREADY		1
#define STAT_ISADC0BUSY			2
#define STAT_ISADC1BUSY			4
#define STAT_ISDACBUSY			8
#define STAT_ISDCSTROBEIN		16
#define STAT_ISDCREADY			32

#define START_ADC0				(CTRL_ADC0START|ENABLE_CTRL_ADC0START)
#define START_ADC1				(CTRL_ADC1START|ENABLE_CTRL_ADC1START)
#define START_DAC				(CTRL_DACSTART|ENABLE_CTRL_DACSTART)
#define START_DDS				(CTRL_DDSSTART|ENABLE_CTRL_DDSSTART)

#define STOP_ADC0				(ENABLE_CTRL_ADC0START)
#define STOP_ADC1				(ENABLE_CTRL_ADC1START)
#define STOP_DAC				(ENABLE_CTRL_DACSTART)
#define STOP_DDS				(ENABLE_CTRL_DDSSTART)

#define GAIN_SET				(CTRL_ADC1GAIN|ENABLE_CTRL_ADC1GAIN)
#define GAIN_RESET				(ENABLE_CTRL_ADC1GAIN)

#define REG_ADC0STARTADDRESS			0x00	//000000
#define REG_DACSTARTADDRESS				0x04	//000100
#define REG_ADC0FINALADDRESS			0x08	//001000
#define REG_DACFINALADDRESS				0x0c	//001100
#define REG_ADC1STARTADDRESS			0x10	//010000
#define REG_ADC1FINALADDRESS			0x14	//010100
#define REG_RAMADDRESS					0x18	//011000
#define REG_RAMDATA						0x1c	//011100
#define REG_CONTROL						0x20	//100000
#define REG_STATUS						0x24	//100100
#define REG_ADC0SAMPLERATE				0x28	//101000
#define REG_ADC1SAMPLERATE				0x2c	//101100
#define REG_DACSAMPLERATE				0x30	//110000
#define REG_DDSFREQUENCY				0x34	//110100
#define REG_DDSPHASE					0x38	//111000
#define REG_DCDATA						0x3c	//111100

AFCDLL_API HAFC OpenDevice(void);
AFCDLL_API BOOL CloseDevice(HAFC hSessn);

AFCDLL_API BOOL WrPort(HAFC hSessn, unsigned char nPort, unsigned long dwData);
AFCDLL_API BOOL RdPort(HAFC hSessn, unsigned char nPort, unsigned long *pdwData);

AFCDLL_API BOOL GetError(HAFC hSessn, unsigned long *pdwCode);

AFCDLL_API BOOL WrMemWord(HAFC hSessn, unsigned short wAddr, unsigned short wData);
AFCDLL_API BOOL RdMemWord(HAFC hSessn, unsigned short wAddr, unsigned short *pwData);
AFCDLL_API BOOL WrMemBlock(HAFC hSessn, unsigned short wAddrStart, unsigned short wWordCount, unsigned short *pwData);
AFCDLL_API BOOL RdMemBlock(HAFC hSessn, unsigned short wAddrStart, unsigned short wWordCount, unsigned short *pwData);

AFCDLL_API BOOL SetADC0StartAddress(HAFC hSessn, unsigned short wAddrStart);
AFCDLL_API BOOL SetADC0FinalAddress(HAFC hSessn, unsigned short wAddrFinal);
AFCDLL_API BOOL SetADC1StartAddress(HAFC hSessn, unsigned short wAddrStart);
AFCDLL_API BOOL SetADC1FinalAddress(HAFC hSessn, unsigned short wAddrFinal);
AFCDLL_API BOOL SetDACStartAddress(HAFC hSessn, unsigned short wAddrStart);
AFCDLL_API BOOL SetDACFinalAddress(HAFC hSessn, unsigned short wAddrFinal);

AFCDLL_API BOOL SetADC0Samplerate(HAFC hSessn, unsigned short wSmplr);
AFCDLL_API BOOL SetADC1Samplerate(HAFC hSessn, unsigned short wSmplr);
AFCDLL_API BOOL SetDACSamplerate(HAFC hSessn, unsigned short wSmplr);

AFCDLL_API BOOL SetDDSFrequency(HAFC hSessn, unsigned long dwFreq);
AFCDLL_API BOOL SetDDSPhase(HAFC hSessn, unsigned short wPhase);

AFCDLL_API BOOL Start(HAFC hSessn, unsigned short wStart);
AFCDLL_API BOOL Stop(HAFC hSessn, unsigned short wStop);

AFCDLL_API BOOL SetGain(HAFC hSessn, unsigned char bGain);
/*
0 - 2.004955947
1 - 11.5221519
*/

AFCDLL_API BOOL IsADC0Ready(HAFC hSessn);
AFCDLL_API BOOL IsADC1Ready(HAFC hSessn);
AFCDLL_API BOOL IsDACReady(HAFC hSessn);

 /***********************************************************************\
|**                             Automatic                               **|
 \***********************************************************************/
AFCDLL_API BOOL SetAutoADC0Samplerate(HAFC hSessn, double dDesiredRate, double& dAcquiredRate, int up);
AFCDLL_API BOOL SetAutoADC1Samplerate(HAFC hSessn, double dDesiredRate, double& dAcquiredRate, int up);
AFCDLL_API BOOL SetAutoDACSamplerate(HAFC hSessn, double dDesiredRate, double& dAcquiredRate, int up);

AFCDLL_API BOOL SetAutoDDSFrequency(HAFC hSessn, double dDesiredFreq, double& dAcquiredFreq, int up);
 /***********************************************************************\
|**                                                                     **|
 \***********************************************************************/

AFCDLL_API double GetMaxADCSamplerate();
AFCDLL_API double GetMaxDACSamplerate();
