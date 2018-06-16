// TestLogDlg.h : ͷ�ļ�
//

#pragma once

#include <string>
#include <vector>


struct structMT2010_Declaration								// ������
{
	std::string RepresentativePersonName;						// �յ�����������
	std::string ExitCustomsOfficeID;								// ���乤���뾳�غ��ش���
	std::string AgentID;											// ���乤�ߴ�����ҵ����
	std::string CarrierID;										// �����˴���
	std::vector<std::string> BorderTransportMeans;	// ���乤����Ϣ
	std::vector<std::string>	Consignment;				// ���˵���Ϣ
	std::string AdditionalInformationContent;					// ��ע
};

struct structMT2010
{
	std::string		 Head;
	structMT2010_Declaration Declaration;
};

// CTestLogDlg �Ի���
class CTestLogDlg : public CDialog
{
// ����
public:
	CTestLogDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TESTLOG_DIALOG };

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
	afx_msg void OnBnClickedButton1();


private:
	structMT2010 mt2010;

	_RecordsetPtr m_pRecordset;
	_ConnectionPtr m_pConnection;
};
