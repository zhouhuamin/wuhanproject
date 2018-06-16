// PlatformProxyServiceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PlatformProxyService.h"
#include "PlatformProxyServiceDlg.h"
#include "CxLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern  "C"  _declspec(dllimport) int StartListening(const UINT& port, CBasicConfigInfo &ConfigInfo);
extern  "C"  _declspec(dllimport) int StopListening();
/////////////////////////////////////////////////////////////////////////////
// CAboutg dialog used for App About
extern VOID ServiceStop();

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


// CPlatformProxyServiceDlg 对话框




CPlatformProxyServiceDlg::CPlatformProxyServiceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPlatformProxyServiceDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	bFlag=FALSE;
}

void CPlatformProxyServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPlatformProxyServiceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_START, &CPlatformProxyServiceDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CPlatformProxyServiceDlg::OnBnClickedButtonStop)
END_MESSAGE_MAP()


// CPlatformProxyServiceDlg 消息处理程序

BOOL CPlatformProxyServiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ShowWindow(SW_HIDE);

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
	bFlag=FALSE;
	GetConfig((GetFileNamePath()+"SysConfig.ini"));
	OnBnClickedButtonStart();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CPlatformProxyServiceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CPlatformProxyServiceDlg::OnPaint()
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
HCURSOR CPlatformProxyServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPlatformProxyServiceDlg::OnBnClickedButtonStart()
{
	// TODO: 在此添加控件通知处理程序代码

	LOG("读取配置文件!");
	GetConfig((GetFileNamePath()+"SysConfig.ini"));
	if(bFlag==TRUE)
	{
		LOG("服务已经启动！");
		return ;
	}
	if(StartListening(m_ConfigPacket.nLocalPort, m_ConfigPacket) == -1)
	{
		LOG("监听失败!");	
	}
	else
	{
		bFlag=TRUE;
	}
}

void CPlatformProxyServiceDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码

	if(bFlag==FALSE)
		return ;
	if(StopListening()==0)
	{
		bFlag=FALSE;

		CString strTmp="停止";

		LOG("停止监控!");
	}
}

BOOL CPlatformProxyServiceDlg::GetConfig(CString strFileName)
{
	char szTmp[356];
	int	 nPort;

	m_ConfigPacket.hNotifyWnd = this->m_hWnd;;

	::GetPrivateProfileString("DATABASE","DataServer","127.0.0.1",szTmp,355,strFileName);
	m_ConfigPacket.strDataServer = szTmp;
	::GetPrivateProfileString("DATABASE","DataUser","sa",szTmp,355,strFileName);
	m_ConfigPacket.strDataUser = szTmp;
	::GetPrivateProfileString("DATABASE","DataPassword","sa",szTmp,355,strFileName);
	m_ConfigPacket.strDataPaw = szTmp;

	::GetPrivateProfileString("DATABASE","DataName","WHHGWL",szTmp,355,strFileName);
	m_strDataName = szTmp;

	//平台IP
	::GetPrivateProfileString("NET","MonitorServerIp","127.0.0.1",szTmp,355,strFileName);
	m_ConfigPacket.strMonitorServerIp = szTmp;

	//平台IP
	::GetPrivateProfileString("NET","MTFlatRoofIp","127.0.0.1",szTmp,355,strFileName);
	m_ConfigPacket.strMTFlatRoofIp = szTmp;

	//本地端口
	nPort=::GetPrivateProfileInt("NET","LocalPort",9002,strFileName);
	m_ConfigPacket.nLocalPort = nPort;
	//平台端口
	nPort=::GetPrivateProfileInt("NET","FlatRoofPort",9001,strFileName);
	m_ConfigPacket.nFlatRoofPort = nPort;

	nPort=::GetPrivateProfileInt("NET","MTFlatRoofPort",9001,strFileName);
	m_ConfigPacket.nMTFlatRoofPort = nPort;
	//代理端口
	nPort=::GetPrivateProfileInt("NET","ProxyPort",9010,strFileName);
	m_ConfigPacket.nProxyPort = nPort;
	//监控端端口
	nPort=::GetPrivateProfileInt("NET","MonitorPort",9011,strFileName);
	m_ConfigPacket.nMonitorPort = nPort;

	//流量控制标记
	nPort=::GetPrivateProfileInt("NET","CtrlSpeedFlag",0,strFileName);
	if(nPort==0)
		m_ConfigPacket.nCtrlSpeedFlag = FALSE;
	else
		m_ConfigPacket.nCtrlSpeedFlag=TRUE;
	nPort=::GetPrivateProfileInt("NET","ChnlLoginMode",0,strFileName);
	m_ConfigPacket.nChnlLoginMode=nPort;

	nPort=::GetPrivateProfileInt("NET","UserLoginMode",0,strFileName);
	m_ConfigPacket.nUserLoginMode=nPort;



	//线程数
	nPort=::GetPrivateProfileInt("THREADTOOL","ThreadNumber",30,strFileName);
	m_ConfigPacket.nThreadNumber = nPort;

	//超时时间
	nPort=::GetPrivateProfileInt("TIME","SendTime",1000,strFileName);
	m_ConfigPacket.nSendTimeOut=nPort;
	nPort=::GetPrivateProfileInt("TIME","RecvTime",1000,strFileName);
	m_ConfigPacket.nRecvTimeOut=nPort;

	//图片路径
	::GetPrivateProfileString("PICTURE","Pic_path","d:\\pic",szTmp,355,strFileName);
	m_ConfigPacket.strFilePath=szTmp;

	CreateDirectory(m_ConfigPacket.strFilePath,NULL);

	m_ConfigPacket.nIsNeedActiveTest=::GetPrivateProfileInt("ACTIVETEST","IsNeedActiveTest",1,strFileName);
	m_ConfigPacket.nCheckLoginOutTime=::GetPrivateProfileInt("ACTIVETEST","CheckLoginOutTime",80,strFileName);
	m_ConfigPacket.nLoginOutTime=::GetPrivateProfileInt("ACTIVETEST","LoginOutTime",80,strFileName);

	// zhouhm add-20110411
	nPort=::GetPrivateProfileInt("RUNLOG","SaveRunDataPacket",0,strFileName);
	m_ConfigPacket.m_iSaveRunDataPacket = (BYTE)nPort;
	nPort=::GetPrivateProfileInt("RUNLOG","QueryLicenseID",0,strFileName);
	m_ConfigPacket.m_iQueryLicenseID = (BYTE)nPort;	

	if(m_ConfigPacket.nIsNeedActiveTest>1)
		m_ConfigPacket.nIsNeedActiveTest=0;
	if(m_ConfigPacket.nCheckLoginOutTime<100)
		m_ConfigPacket.nCheckLoginOutTime=100;

	nPort=::GetPrivateProfileInt("PICTURE","IsCheckContaNo",0,strFileName);
	m_ConfigPacket.nCheckContaFlag=nPort;
	//关锁配置
	nPort=::GetPrivateProfileInt("ESEALCONFIG","ESealFlag",0,strFileName);
	m_ConfigPacket.nESealFlag=nPort;

	nPort=::GetPrivateProfileInt("ESEALCONFIG","ESealInfoDownFlag",0,strFileName);
	m_ConfigPacket.nESealInfoDownFlag=nPort;

	nPort=::GetPrivateProfileInt("ESEALCONFIG","ESealDownTime",0,strFileName);
	m_ConfigPacket.nESealTime=nPort;

	//	if(m_ConfigPacket.nLoginOutTime)
	//CreatODBCSource();

	return TRUE;
}

CString CPlatformProxyServiceDlg::GetFileNamePath()
{
	char szAppName[MAX_PATH];
	int  len;
	::GetModuleFileName(AfxGetInstanceHandle(), szAppName, sizeof(szAppName));
	len = strlen(szAppName);
	for(int i=len; i>0; i--)
	{
		if(szAppName[i] == '\\')
		{
			szAppName[i+1]='\0';
			break;
		}
	}
	CString strFileNamePath=szAppName;
	return strFileNamePath;
}


BOOL CPlatformProxyServiceDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	if(bFlag)
	{
		OnBnClickedButtonStop();
		Sleep(20);
	}

	return CDialog::DestroyWindow();
}
