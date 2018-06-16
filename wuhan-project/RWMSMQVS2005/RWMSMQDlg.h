// RWMSMQDlg.h : header file
//

#if !defined(AFX_RWMSMQDLG_H__89E8AD21_2324_4678_B144_7A35C94E5855__INCLUDED_)
#define AFX_RWMSMQDLG_H__89E8AD21_2324_4678_B144_7A35C94E5855__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "StructDef.h"
#include <vector>



#import "mqoa.dll"
using namespace MSMQ;
using std::vector;
/////////////////////////////////////////////////////////////////////////////
// CRWMSMQDlg dialog

class A
{
public:
    int i; unsigned int ui;
    long l;unsigned long ul;
    char szInt[MAX_PATH];
    CString strLong;
// public:
//     A() :
// 	  i(std::rand()),
// 		  ui(std::rand()),
// 		  l(std::rand()),
// 		  ul(std::rand())
// 	  {
// 		  std::stringstream ss;
// 		  ss << i;    ss >> szInt;
// 		  ss.clear();
// 		  ss << l;    ss >> strLong;
// 	  }
	A()
	{
		i = 1;
		ui = 2;
		l = 3;
		ul = 4;
		strcpy(szInt, "abcd");
		strLong = "1234";
	}
};



class CRWMSMQDlg : public CDialog
{
// Construction
public:
	CRWMSMQDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CRWMSMQDlg)
	enum { IDD = IDD_RWMSMQ_DIALOG };
	CButton	m_journal;
	CString	m_msgLabel;
	CString	m_ServerName;
	CString	m_msgData;
	int		m_nTimeSpan;
	int		m_nVaryRange;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRWMSMQDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

public:
	void ReleaseAll();
	void ParseFile(void);
	void GenerateFile(vector<structShipInfoTmp> &shipVect);

	// Generated message map functions
	//{{AFX_MSG(CRWMSMQDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnCreate();
	afx_msg void OnSend();
	afx_msg void OnRead();
	afx_msg void OnClear();
	afx_msg void OnExit();
	afx_msg void OnPrivate();
	afx_msg void OnPublic();
	afx_msg void OnJournal();
	afx_msg void OnButtonSendBytes();
	afx_msg void OnButtonReadFiel();
	afx_msg void OnButtonSimulateData();
	afx_msg void OnButtonReadMsmq();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RWMSMQDLG_H__89E8AD21_2324_4678_B144_7A35C94E5855__INCLUDED_)
