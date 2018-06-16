// PlatformProxyServiceDlg.h : 头文件
//

#pragma once
#include "public/BasicConfigInfo.h"


// CPlatformProxyServiceDlg 对话框
class CPlatformProxyServiceDlg : public CDialog
{
// 构造
public:
	CPlatformProxyServiceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PLATFORMPROXYSERVICE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();


private:
	BOOL GetConfig(CString strFileName);
	CString GetFileNamePath();

public:
	CBasicConfigInfo m_ConfigPacket;
	BOOL bFlag;
	CString				m_strDataName;

	virtual BOOL DestroyWindow();
};
