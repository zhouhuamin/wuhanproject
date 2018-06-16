// PlatformProxyServiceDlg.cpp : ʵ���ļ�
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

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CPlatformProxyServiceDlg �Ի���




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


// CPlatformProxyServiceDlg ��Ϣ�������

BOOL CPlatformProxyServiceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ShowWindow(SW_HIDE);

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	bFlag=FALSE;
	GetConfig((GetFileNamePath()+"SysConfig.ini"));
	OnBnClickedButtonStart();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CPlatformProxyServiceDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ��������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù����ʾ��
//
HCURSOR CPlatformProxyServiceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CPlatformProxyServiceDlg::OnBnClickedButtonStart()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	LOG("��ȡ�����ļ�!");
	GetConfig((GetFileNamePath()+"SysConfig.ini"));
	if(bFlag==TRUE)
	{
		LOG("�����Ѿ�������");
		return ;
	}
	if(StartListening(m_ConfigPacket.nLocalPort, m_ConfigPacket) == -1)
	{
		LOG("����ʧ��!");	
	}
	else
	{
		bFlag=TRUE;
	}
}

void CPlatformProxyServiceDlg::OnBnClickedButtonStop()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if(bFlag==FALSE)
		return ;
	if(StopListening()==0)
	{
		bFlag=FALSE;

		CString strTmp="ֹͣ";

		LOG("ֹͣ���!");
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

	//ƽ̨IP
	::GetPrivateProfileString("NET","MonitorServerIp","127.0.0.1",szTmp,355,strFileName);
	m_ConfigPacket.strMonitorServerIp = szTmp;

	//ƽ̨IP
	::GetPrivateProfileString("NET","MTFlatRoofIp","127.0.0.1",szTmp,355,strFileName);
	m_ConfigPacket.strMTFlatRoofIp = szTmp;

	//���ض˿�
	nPort=::GetPrivateProfileInt("NET","LocalPort",9002,strFileName);
	m_ConfigPacket.nLocalPort = nPort;
	//ƽ̨�˿�
	nPort=::GetPrivateProfileInt("NET","FlatRoofPort",9001,strFileName);
	m_ConfigPacket.nFlatRoofPort = nPort;

	nPort=::GetPrivateProfileInt("NET","MTFlatRoofPort",9001,strFileName);
	m_ConfigPacket.nMTFlatRoofPort = nPort;
	//����˿�
	nPort=::GetPrivateProfileInt("NET","ProxyPort",9010,strFileName);
	m_ConfigPacket.nProxyPort = nPort;
	//��ض˶˿�
	nPort=::GetPrivateProfileInt("NET","MonitorPort",9011,strFileName);
	m_ConfigPacket.nMonitorPort = nPort;

	//�������Ʊ��
	nPort=::GetPrivateProfileInt("NET","CtrlSpeedFlag",0,strFileName);
	if(nPort==0)
		m_ConfigPacket.nCtrlSpeedFlag = FALSE;
	else
		m_ConfigPacket.nCtrlSpeedFlag=TRUE;
	nPort=::GetPrivateProfileInt("NET","ChnlLoginMode",0,strFileName);
	m_ConfigPacket.nChnlLoginMode=nPort;

	nPort=::GetPrivateProfileInt("NET","UserLoginMode",0,strFileName);
	m_ConfigPacket.nUserLoginMode=nPort;



	//�߳���
	nPort=::GetPrivateProfileInt("THREADTOOL","ThreadNumber",30,strFileName);
	m_ConfigPacket.nThreadNumber = nPort;

	//��ʱʱ��
	nPort=::GetPrivateProfileInt("TIME","SendTime",1000,strFileName);
	m_ConfigPacket.nSendTimeOut=nPort;
	nPort=::GetPrivateProfileInt("TIME","RecvTime",1000,strFileName);
	m_ConfigPacket.nRecvTimeOut=nPort;

	//ͼƬ·��
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
	//��������
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
	// TODO: �ڴ����ר�ô����/����û���

	if(bFlag)
	{
		OnBnClickedButtonStop();
		Sleep(20);
	}

	return CDialog::DestroyWindow();
}
