// TestLogDlg.h : 头文件
//

#pragma once

#include <string>
#include <vector>


struct structMT2010_Declaration								// 报文体
{
	std::string RepresentativePersonName;						// 舱单传输人名称
	std::string ExitCustomsOfficeID;								// 运输工具离境地海关代码
	std::string AgentID;											// 运输工具代理企业代码
	std::string CarrierID;										// 承运人代码
	std::vector<std::string> BorderTransportMeans;	// 运输工具信息
	std::vector<std::string>	Consignment;				// 提运单信息
	std::string AdditionalInformationContent;					// 备注
};

struct structMT2010
{
	std::string		 Head;
	structMT2010_Declaration Declaration;
};

// CTestLogDlg 对话框
class CTestLogDlg : public CDialog
{
// 构造
public:
	CTestLogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TESTLOG_DIALOG };

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
	afx_msg void OnBnClickedButton1();


private:
	structMT2010 mt2010;

	_RecordsetPtr m_pRecordset;
	_ConnectionPtr m_pConnection;
};
