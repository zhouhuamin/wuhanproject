// TestLogDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestLog.h"
#include "TestLogDlg.h"
#include "RADatabase.h"
#include "tinyxml.h"
#include <string>
#include <boost/lexical_cast.hpp>
//#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost;
//using namespace boost::gregorian;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CTestLogDlg 对话框




CTestLogDlg::CTestLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestLogDlg::IDD, pParent), mt2010()
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTestLogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CTestLogDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CTestLogDlg 消息处理程序

BOOL CTestLogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//if (!AfxOleInit())
	//{
	//	MessageBox("初始化数据库连接失败");
	//	exit(1);
	//	return FALSE;
	//}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestLogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestLogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
//
HCURSOR CTestLogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


enum CodePage
{
	GB2312=936,
	UTF_8=65001
};

template<CodePage TSrc,CodePage TDec>
bool ConvertString(LPCSTR src,LPSTR* dec)
{
	wchar_t *buff;
	int buffsize = ::MultiByteToWideChar(TSrc,0,src,-1,NULL,0);
	buff = (wchar_t*)malloc(sizeof(wchar_t)*(buffsize+1));
	::MultiByteToWideChar(TSrc,0,src,-1,buff,buffsize);

	int decsize = ::WideCharToMultiByte(TDec,0,buff,-1,0,0,0,0);
	*dec = (char*)malloc(sizeof(char)*(decsize+1));
	::WideCharToMultiByte(TDec,0,buff,-1,*dec,decsize,0,0);
	free(buff);
	return true;
}






void CTestLogDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码


	string str = "123";
	int b = lexical_cast<int>(str);
	{
		CString szLabel= "WEB-REL10010-XXX";
		CString strTmpType = szLabel;

		if (!strTmpType.IsEmpty())
		{
			//strTmpType = strTmpType.Left(strTmpType.ReverseFind('-'));
			//strTmpType.Trim();
			//strTmpType.MakeUpper();


			int index = strTmpType.Find('-');
			if (index == -1)
			{
				strTmpType.Trim();
				strTmpType.MakeUpper();
			}
			else
			{
				CString strPre = strTmpType.Left(3);
				if (strPre == "WEB")
				{
					strTmpType = strTmpType.Mid(index + 1);
					index = strTmpType.Find('-');
					if (index == -1)
					{
						strTmpType = szLabel;
						strTmpType.Trim();
						strTmpType.MakeUpper();
					}
					else
					{
						strTmpType = szLabel;
						strTmpType = strTmpType.Left(strTmpType.ReverseFind('-'));
						strTmpType.Trim();
						strTmpType.MakeUpper();
					}
				}
				else
				{
					strTmpType = strTmpType.Left(index);
				}

			}
		}
	}


	CString szLabel= "WEB-CSM001-XXX";
	CString strTmpType = szLabel;
	int index = strTmpType.Find('-');
	if (index == -1)
	{
		strTmpType.Trim();
		strTmpType.MakeUpper();
	}
	else
	{
		CString strPre = strTmpType.Left(3);
		if (strPre == "WEB")
		{
			strTmpType = strTmpType.Mid(index + 1);
			index = strTmpType.Find('-');
			if (index == -1)
			{
				strTmpType = szLabel;
				strTmpType.Trim();
				strTmpType.MakeUpper();
			}
			else
			{
				strTmpType = szLabel;
				strTmpType = strTmpType.Left(strTmpType.ReverseFind('-'));
				strTmpType.Trim();
				strTmpType.MakeUpper();
			}
		}
		else
		{
			strTmpType = strTmpType.Left(index);
		}

	}


	//date fmtdt1 = from_string("xxxx");
	//from_string

	char szTime01[100] = {0};
	int nYear1 = 0;
	int nMonth1 = 0;
	int nDay1 = 0;
	int nHour1 = 0;
	int nMinute1 = 0;
	int nSecond1 = 0;
	int nReserve1 = 0;

	CString strTime1;
	strTime1 = "201403121412+0800";
	strTime1.TrimLeft();
	strTime1.TrimRight();


	if (strTime1.IsEmpty())
	{
		memset(szTime01, 0x00, sizeof(szTime01));
	}
	else
	{
		memcpy(szTime01, (LPCTSTR)strTime1, strTime1.GetLength());
		sscanf_s(szTime01, "%04d%02d%02d%02d%02d%03d", &nYear1, &nMonth1, &nDay1, &nHour1,&nMinute1, &nReserve1); // C4996

		memset(szTime01, 0x00, sizeof(szTime01));
		sprintf_s(szTime01, sizeof(szTime01), "%04d-%02d-%02d %02d:%02d:%02d", nYear1, nMonth1, nDay1, nHour1, nMinute1, nSecond1);
	}





	CString strResponse = "";

	std::string MESSAGE_ID = "111";
	std::string FUNCTION_CODE = "2";
	std::string MESSAGE_TYPE = "CLM10042";
	std::string RECEIVE	= "4";
	std::string SENDER	= "5";
	std::string MESSAGE_CREATE_TIME = "2014-09-15 21:51:23.000";
	std::string strVERSION = "1.0";
	char m_chOpFlag = 'F';
	std::string m_strNote = "中国";

	strResponse.Format("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
		"<MESSAGE>\n"
		"<MESSAGE_HEAD>\n"
		"\t<MESSAGE_ID>%s</MESSAGE_ID>\n"
		"\t<FUNCTION_CODE>%s</FUNCTION_CODE>\n"
		"\t<MESSAGE_TYPE>%s</MESSAGE_TYPE>\n"
		"\t<SENDER>%s</SENDER>\n"
		"\t<RECEIVER>%s</RECEIVER>\n"
		"\t<MESSAGE_CREATE_TIME>%s</MESSAGE_CREATE_TIME>\n"
		"\t<VERSION>%s</VERSION>\n"
		"</MESSAGE_HEAD>\n"
		"<MESSAGE_LIST>\n"
		"<RESPOND_INFO>\n"
		"\t<MESSAGE_ID>%s</MESSAGE_ID>\n"
		"\t<OP_FLAG>%c</OP_FLAG>\n"
		"\t<NOTE>%s</NOTE>\n"
		"</RESPOND_INFO>\n"
		"</MESSAGE_LIST>\n"
		"</MESSAGE>\n", 
		MESSAGE_ID.c_str(), FUNCTION_CODE.c_str(), MESSAGE_TYPE.c_str(), RECEIVE.c_str(), \
		SENDER.c_str(), MESSAGE_CREATE_TIME.c_str(), strVERSION.c_str(), MESSAGE_ID.c_str(), m_chOpFlag, m_strNote.c_str());

	LPSTR outstr = NULL;
	ConvertString<GB2312,UTF_8>(strResponse, &outstr);
	std::string m_strFileName = "D:\\TestXml001.xml";
	//CStdioFile file;
	//if (!file.Open(m_strFileName.c_str(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
	//{
	//	return;
	//}
	//file.Write((LPCTSTR)strResponse, strResponse.GetLength());
	//file.Close();

	TiXmlDocument doc;
	doc.Parse(outstr,0, TIXML_ENCODING_UTF8);
	doc.SaveFile(m_strFileName.c_str());
	free(outstr);




	//CStdioFile file;
	//if (!file.Open(m_strFileName.c_str(), CFile::modeRead|CFile::typeBinary))
	//{
	//	return;
	//}

	//int i = 10;
	//int j = 1;
	//do 
	//{
	//	if (j == 1)
	//		continue;
	//} while (--i);



	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		std::string MQMessageType = "xxxx";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where INTOOUTFLAG=0");

		CString strConn = "";
		strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		//strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataPwd.c_str();
		//strConn += ";Persist Security Info=True;User ID=";
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataUser.c_str();
		//strConn += ";Initial Catalog="; // WHHGWL";
		//strConn += m_config.strSqlServerDataName.c_str();
		//strConn += ";Data Source=";
		////strConn += "127.0.0.1";
		//strConn += m_config.strSqlServerDataServer.c_str();

		CRADatabase database;
		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return ;
		}

		_RecordsetPtr oRs;
		oRs = database.RAExecuteRs(szSql);
		CComVariant val;

		while (!oRs->EndOfFile)
		{	
			val = oRs->Fields->Item[_variant_t("MQPATH")]->Value;
			if (val.vt != (VT_NULL))
			{
				strMQPath = (char*)_bstr_t(val);	
				mqPathArray.Add(strMQPath);
			}

			oRs->MoveNext();
		}
		oRs->Release();					
		oRs->Close();


		CString strQueueName	= "";
		CString strMQIp			= "";
		CStringArray queueNameArray;
		CStringArray mqIPArray;
		for (int i = 0; i < mqPathArray.GetSize(); ++i)
		{
			strMQPath = mqPathArray[i];
			if (strMQPath.IsEmpty())
				continue;

			// formatname:direct=tcp:192.168.32.30\private$\whhg_OutToIn
			int index = strMQPath.ReverseFind('\\');
			if (index > 0)
			{
				strQueueName = (LPCTSTR)strMQPath.Mid(index + 1);
				queueNameArray.Add(strQueueName);
			}

			index = strMQPath.ReverseFind(':');
			int right = strMQPath.Find('\\');
			if (index > 0 && index < right)
			{
				strMQIp = (LPCTSTR)strMQPath.Mid(index + 1, right - index - 1);
				mqIPArray.Add(strMQIp);
			}
		}
	}
	catch(...)
	{
		return ;
	}
	

	//std::string str = mt2010.Head;
	//int a = 10;

	//HRESULT hr;
	//try
	//{
	//	hr = m_pConnection.CreateInstance("ADODB.Connection");///创建Connection对象
	//	if(SUCCEEDED(hr))
	//	{
	//		hr = m_pConnection->Open("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;","","",adModeUnknown);///连接数据库 //上面一句中连接字串中的Provider是针对ACCESS2000环境的，对于ACCESS97， //需要改为：Provider=Microsoft.Jet.OLEDB.3.51;
	//	}
	//}
	//catch(_com_error &e)///捕捉异常
	//{
	//CString errormessage;
	//errormessage.Format("连接数据库失败!\r\n错误信息:%s",e.ErrorMessage());
	//AfxMessageBox(errormessage);///显示错误信息
	//}

	//try   
	//{   
	//	// 用SQL语句删除不需要的Item   
	//	CString strCmd;   
	//	strCmd.Format(    
	//		_T("delete * from demo where username = \"%s\" "),   
	//		lvi.pszText   
	//		);   

	//	_CommandPtr spCmd;   
	//	spCmd.CreateInstance( __uuidof(Command) );   
	//	spCmd->ActiveConnection = this->m_spConnection;   
	//	spCmd->CommandText = (LPCTSTR)strCmd;   

	//	spCmd->Execute( NULL, NULL, adCmdText );   

	//	// 删除成功则刷新   
	//	this->OnBnClickedButtonRefresh();   
	//}   
	//catch( _com_error& e )   
	//{   
	//	MessageBox( e.ErrorMessage(), _T("删除数据失败！") );   
	//}  
}
