
// CaprecogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Caprecog.h"
#include "CaprecogDlg.h"
#include "afxdialogex.h"
#include "../inc/common.h"
#include <direct.h>

#include "../Valorant/Stats.h"
#include "../Valorant/CheckEvent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


DWORD WINAPI MainThread(LPVOID lpParameter);
BOOL Capture(LPCTSTR LpWindowName, int Width, int Height);
BOOL GetRowData(HDC hDC, HBITMAP hBitmap, char* szPath);
void SaveBitmap(LPCTSTR fileName, int nWidth, int nHeight);

PBYTE m_pImg=NULL;
CStats m_Stats;
CCheckEvent m_CheckEvent;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCaprecogDlg dialog



CCaprecogDlg::CCaprecogDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CAPRECOG_DIALOG, pParent)
	, m_fRunning(false)
	, m_nFramesPerSecond(20)
	, m_sWindowName(_T(""))
	, m_nFramesCorrection(1)
	, m_nFrameSave(20)
	, m_fSaveRect(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCaprecogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_START, m_BtnStart);
	DDX_Control(pDX, IDC_COMBO_RESOLUTION, m_ComResolution);
	DDX_Text(pDX, IDC_EDIT_WINDOWNAME, m_sWindowName);
	DDX_Text(pDX, IDC_EDIT_FRAMESPERSECOND, m_nFramesPerSecond);
	DDV_MinMaxUInt(pDX, m_nFramesPerSecond, 1, 30);
	DDX_Text(pDX, IDC_EDIT_CORRECTION, m_nFramesCorrection);
	DDV_MinMaxUInt(pDX, m_nFramesCorrection, 1, 5);
	DDX_Text(pDX, IDC_EDIT_SAVEFRAME, m_nFrameSave);
	DDX_Check(pDX, IDC_CHECK_SAVERECT, m_fSaveRect);
}

BEGIN_MESSAGE_MAP(CCaprecogDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CCaprecogDlg::OnBnClickedButtonStart)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CCaprecogDlg message handlers

BOOL CCaprecogDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_ComResolution.AddString("1920*1080");
	m_ComResolution.AddString("1024*768");
	m_ComResolution.AddString("1152*864");
	m_ComResolution.AddString("1280*720");
	m_ComResolution.AddString("1280*768");
	m_ComResolution.AddString("1366*768");

	char str[MAX_PATH];
	GetPrivateProfileString(_T("Set"), _T("WindowName"), _T("Valorant"), str, MAX_PATH, SETTING_INI); 
	m_sWindowName = "VALORANT  ";//str;
	GetPrivateProfileString(_T("Set"), _T("Resolution"), _T("1920*1080"), str, MAX_PATH, SETTING_INI); 
	m_ComResolution.SelectString(0, str);
	m_nFramesPerSecond = GetPrivateProfileInt(_T("Set"), _T("CaptureRate"), 20, SETTING_INI);
	m_nFramesCorrection = GetPrivateProfileInt(_T("Set"), _T("FramesForCorrection"), 1, SETTING_INI);
	m_nFrameSave = GetPrivateProfileInt(_T("Set"), _T("SavePerFrames"), 20, SETTING_INI);
	UpdateData(false);

	DWORD threadId;
	CreateThread(NULL, 0, MainThread, (void*)this, 0, &threadId);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCaprecogDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	char str[MAX_PATH];
	UpdateData();
	WritePrivateProfileString(_T("Set"), _T("WindowName"), m_sWindowName, SETTING_INI);
	m_ComResolution.GetWindowText(str, MAX_PATH);
	WritePrivateProfileString(_T("Set"), _T("Resolution"), str, SETTING_INI);
	_itoa_s(m_nFramesPerSecond, str, 10); WritePrivateProfileString(_T("Set"), _T("CaptureRate"), str, SETTING_INI);
	_itoa_s(m_nFramesCorrection, str, 10); WritePrivateProfileString(_T("Set"), _T("FramesForCorrection"), str, SETTING_INI);
	_itoa_s(m_nFrameSave, str, 10); WritePrivateProfileString(_T("Set"), _T("SavePerFrames"), str, SETTING_INI);

	CDialogEx::OnClose();
}

void CCaprecogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCaprecogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCaprecogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CCaprecogDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	if (!m_fRunning)
	{
		if (UpdateData())
		{
			char rs[MAX_PATH], *p;
			m_ComResolution.GetWindowText(rs, MAX_PATH);
			p = strchr(rs, '*'); *p = 0; p++;
			m_nWidth = atoi(rs); m_nHeight = atoi(p);

			m_BtnStart.SetWindowText(_T("Stop"));
			m_fRunning = true;
		}
	}
	else
	{
		m_BtnStart.SetWindowText(_T("Start"));
		m_fRunning = false;
	}
}


DWORD WINAPI MainThread(LPVOID lpParameter)
{
	CCaprecogDlg *dlg = (CCaprecogDlg *)lpParameter;
	FILE *fp;
	char strTime[64], fn[MAX_PATH];
	SYSTEMTIME lt;
	int curtime = 0, beforetime = 0, nCnt = 0;

	_mkdir("Out");
	m_pImg = new BYTE[1920 * 1080 * 5 * 3];	

	while (true)
	{
		GetLocalTime(&lt);

		OutputDebugStringA("MainThread - 0 - 1");

		curtime = (((((lt.wYear * 12 + lt.wMonth) * 31 + lt.wDay) * 24 + lt.wHour) * 60 + lt.wMinute) * 60 + lt.wSecond) * 1000 + lt.wMilliseconds;	
		
		CString log;
		log.Format("MainThread - 0 - 1 - [%d]", curtime);
		OutputDebugStringA(log);

		if (curtime < beforetime + 1000 / (int)(dlg->m_nFramesPerSecond))
		{
			log.Format("MainThread - 0 - 1 - delay [%d]", (beforetime + 1000 / dlg->m_nFramesPerSecond) - curtime);
			OutputDebugStringA(log);
			Sleep((beforetime + 1000 / dlg->m_nFramesPerSecond) - curtime);
		}

		OutputDebugStringA("MainThread - 0 - 2");
		
		GetLocalTime(&lt);
		beforetime = (((((lt.wYear * 12 + lt.wMonth) * 31 + lt.wDay) * 24 + lt.wHour) * 60 + lt.wMinute) * 60 + lt.wSecond) * 1000 + lt.wMilliseconds;

		if (!dlg->m_fRunning)
			continue;

		dlg->m_nWidth = GetSystemMetrics(SM_CXSCREEN);
		dlg->m_nHeight = GetSystemMetrics(SM_CYSCREEN);

		OutputDebugStringA("MainThread - 1");

		if (Capture(dlg->m_sWindowName, dlg->m_nWidth, dlg->m_nHeight))
		{
			OutputDebugStringA("MainThread - 2");
			m_Stats.GetStats(m_pImg, dlg->m_nWidth, dlg->m_nHeight);
			m_CheckEvent.HIS_LEN = dlg->m_nFramesCorrection;
			m_CheckEvent.Check(&m_Stats);
			
			sprintf_s(strTime, sizeof(strTime), "\n\n%02d%02d_%02d%02d_%02d%03d",
				lt.wMonth, lt.wDay, lt.wHour, lt.wMinute, lt.wSecond, lt.wMilliseconds);
			OutputDebugStringA("MainThread - 3");
			if (!fopen_s(&fp, "Out\\_log.txt", "a"))
			{
				fputs(strTime, fp);
				fputs(m_CheckEvent.m_strEvent, fp);
				fputs(m_CheckEvent.m_strState, fp);
				fclose(fp);
			}
			OutputDebugStringA("MainThread - 4");
			nCnt++; 
			if (dlg->m_nFrameSave)nCnt %= dlg->m_nFrameSave;
			if (!nCnt)
			{
				if (dlg->m_fSaveRect)
				{
					OutputDebugStringA("MainThread - 5");
					//draw enemy rect
					for (int i = 0; i < m_Stats.m_nEnemy; i++)
					{
						int j, x, y;
						for (j = m_Stats.m_Enemy[i].left; j <= m_Stats.m_Enemy[i].right; j++)
						{
							x = j; y = m_Stats.m_Enemy[i].top;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = j; y = m_Stats.m_Enemy[i].top-1;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = j; y = m_Stats.m_Enemy[i].top+1;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = j; y = m_Stats.m_Enemy[i].bottom;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = j; y = m_Stats.m_Enemy[i].bottom-1;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = j; y = m_Stats.m_Enemy[i].bottom+1;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
						}
						for (j = m_Stats.m_Enemy[i].top; j <= m_Stats.m_Enemy[i].bottom; j++)
						{
							x = m_Stats.m_Enemy[i].left; y = j;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = m_Stats.m_Enemy[i].left-1; y = j;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = m_Stats.m_Enemy[i].left+1; y = j;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = m_Stats.m_Enemy[i].right; y = j;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = m_Stats.m_Enemy[i].right-1; y = j;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
							x = m_Stats.m_Enemy[i].right+1; y = j;
							if ((x >= 0) && (x < dlg->m_nWidth) && (y >= 0) && (y < dlg->m_nHeight)) { x = (y*dlg->m_nWidth + x) * 3; m_pImg[x] = 255; m_pImg[x + 1] = 0; m_pImg[x + 2] = 0; }
						}
					}
				}

				sprintf_s(fn, sizeof(fn), "Out\\Capture_%s.bmp", strTime + 2);
				SaveBitmap(fn, dlg->m_nWidth, dlg->m_nHeight);
				OutputDebugStringA("MainThread - 6");
			}

			OutputDebugStringA("MainThread - 7");
		}
	}
	return 0;
}

void SaveBitmap(LPCTSTR fileName, int nWidth, int nHeight)
{
	BITMAPFILEHEADER    BmHdr;
	BmHdr.bfType = 0x4d42;   // 'BM' WINDOWS_BITMAP_SIGNATURE
	BmHdr.bfSize = ((3 * nWidth + 3)& ~3) * nHeight + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	BmHdr.bfReserved1 = BmHdr.bfReserved2 = 0;
	BmHdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

	BITMAPINFOHEADER bmHeader;
	ZeroMemory(&bmHeader, sizeof(BITMAPINFOHEADER));
	bmHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmHeader.biWidth = nWidth;
	bmHeader.biHeight = nHeight;
	bmHeader.biPlanes = 1;
	bmHeader.biBitCount = 24;

	FILE *fp;
	if (!fopen_s(&fp, fileName, "wb"))
	{
		fwrite(&BmHdr, sizeof(BITMAPFILEHEADER), 1, fp);
		fwrite(&bmHeader, sizeof(BITMAPINFOHEADER), 1, fp);
		for (int i = nHeight - 1; i >= 0; i--)
		{
			fwrite(m_pImg + i * 3 * nWidth, 1, 3 * nWidth, fp);
			for (int j = 0; j < ((3 * nWidth + 3)& ~3) - 3 * nWidth; j++)
			{
				char z = 0;
				fwrite(&z, 1, 1, fp);
			}
		}
		fclose(fp);
	}

}

BOOL Capture(LPCTSTR LpWindowName, int Width, int Height)
{
	BOOL rtn = false;
	static int g_nCnt = 0;

	HDC hDC;
	HWND hDeskTopwnd = ::GetDesktopWindow();
	hDC = ::GetDC(hDeskTopwnd);
	HDC hMemDC = CreateCompatibleDC(hDC);
	SIZE size;
	size.cx = GetSystemMetrics(SM_CXSCREEN);
	size.cy = GetSystemMetrics(SM_CYSCREEN);

	HWND hGamewnd = ::FindWindow(NULL, LpWindowName);

	OutputDebugStringA("Capture - 1");
	if (hGamewnd)
	{
		RECT rect;
		::GetWindowRect(hGamewnd, &rect);
		int nWidth, nHeight;
		nWidth = rect.right - rect.left;
		nHeight = rect.bottom - rect.top;
		OutputDebugStringA("Capture - 2");
		if (nWidth == Width && nHeight == Height)//resolution check
		{
			HBITMAP hBitmap = CreateCompatibleBitmap(hDC, nWidth, nHeight);
			if (hBitmap)
			{
				OutputDebugStringA("Capture - 3");
				HBITMAP hOld = (HBITMAP)SelectObject(hMemDC, hBitmap);
				::BitBlt(hMemDC, 0, 0, nWidth, nHeight, hDC, rect.left, rect.top, SRCCOPY);
				char szFileName[MAX_PATH];
				sprintf_s(szFileName, "Sample_%06d.bmp", g_nCnt);
				g_nCnt++;
				rtn=GetRowData(hMemDC, hBitmap, szFileName);
				::SelectObject(hMemDC, hOld);
				::DeleteObject(hBitmap);
			}
		}
		OutputDebugStringA("Capture - 4");
	}
	OutputDebugStringA("Capture - 5");

	::DeleteDC(hMemDC);
	::ReleaseDC(NULL, hDC);
	return rtn;
}

BOOL GetRowData(HDC hDC, HBITMAP hBitmap, char* szPath)
{
	BITMAPINFO BitInfo;
	ZeroMemory(&BitInfo, sizeof(BITMAPINFO));
	BitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitInfo.bmiHeader.biBitCount = 0;
	if (!::GetDIBits(hDC, hBitmap, 0, 0, NULL, &BitInfo, DIB_RGB_COLORS))
	{
		return FALSE;
	}
	
	BitInfo.bmiHeader.biCompression = BI_RGB;
	BYTE *pData = new BYTE[BitInfo.bmiHeader.biSizeImage + 5];
	if (!::GetDIBits(hDC, hBitmap, 0, BitInfo.bmiHeader.biHeight, pData, &BitInfo, DIB_RGB_COLORS))
	{
		delete []pData;
		return FALSE;
	}

	//from pData to m_pImg
	PBYTE p = m_pImg, pp;
	for (int j = BitInfo.bmiHeader.biHeight-1; j >=0 ; j--)
	{
		pp = pData + j*BitInfo.bmiHeader.biWidth * 4;
		for (int i = 0; i < BitInfo.bmiHeader.biWidth; i++)
		{
			*p++ = *pp++;//B
			*p++ = *pp++;//G
			*p++ = *pp++;//R
			pp++;//Alpha
		}
	}

	delete[]pData;
	return TRUE;
}

