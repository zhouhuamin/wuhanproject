// PlatformProxyServiceDlg.h : ͷ�ļ�
//

#pragma once
#include "public/BasicConfigInfo.h"


// CPlatformProxyServiceDlg �Ի���
class CPlatformProxyServiceDlg : public CDialog
{
// ����
public:
	CPlatformProxyServiceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PLATFORMPROXYSERVICE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
