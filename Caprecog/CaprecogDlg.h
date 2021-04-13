
// CaprecogDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CCaprecogDlg dialog
class CCaprecogDlg : public CDialogEx
{
// Construction
public:
	CCaprecogDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAPRECOG_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	int m_nWidth;
	int m_nHeight;

	CButton m_BtnStart;
	CComboBox m_ComResolution;
	BOOL m_fRunning;
	UINT m_nFramesPerSecond;
	CString m_sWindowName;
	UINT m_nFramesCorrection;
	UINT m_nFrameSave;
	BOOL m_fSaveRect;
	afx_msg void OnClose();

};
