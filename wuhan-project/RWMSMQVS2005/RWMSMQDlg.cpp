// RWMSMQDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RWMSMQ.h"
#include "RWMSMQDlg.h"
#include <atlbase.h>

#include "ChineseCode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
// Global Variables

#define BUFSIZE 1024
#define TIMEID1  1


IMSMQQueuePtr		qDest;  // Represents Destination Queue
IMSMQQueueInfoPtr	qInfo;	  //Represents an open instance of the destination queue
IMSMQQueueInfoPtr	qRead;
IMSMQMessagePtr     qMsg;     //Represents the message

LPTSTR lpszSystemInfo;      // pointer to system information string 
DWORD cchBuff = BUFSIZE;    // size of computer 
TCHAR tchBuffer[BUFSIZE];   // buffer for string

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRWMSMQDlg dialog

CRWMSMQDlg::CRWMSMQDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRWMSMQDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRWMSMQDlg)
	m_msgLabel = _T("");
	m_ServerName = _T("");
	m_msgData = _T("");
	m_nTimeSpan = 0;
	m_nVaryRange = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRWMSMQDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRWMSMQDlg)
	DDX_Control(pDX, IDC_JOURNAL, m_journal);
	DDX_Text(pDX, IDC_MSGLABEL, m_msgLabel);
	DDV_MaxChars(pDX, m_msgLabel, 255);
	DDX_Text(pDX, IDC_MSMQSERVER, m_ServerName);
	DDV_MaxChars(pDX, m_ServerName, 255);
	DDX_Text(pDX, IDC_TEXT, m_msgData);
	DDX_Text(pDX, IDC_EDIT_TIME_SPAN, m_nTimeSpan);
	DDV_MinMaxInt(pDX, m_nTimeSpan, 1, 10000);
	DDX_Text(pDX, IDC_EDIT_VARY_RANGE, m_nVaryRange);
	DDV_MinMaxInt(pDX, m_nVaryRange, 1, 100);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CRWMSMQDlg, CDialog)
	//{{AFX_MSG_MAP(CRWMSMQDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_READ, OnRead)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_BN_CLICKED(IDC_PRIVATE, OnPrivate)
	ON_BN_CLICKED(IDC_PUBLIC, OnPublic)
	ON_BN_CLICKED(IDC_JOURNAL, OnJournal)
	ON_BN_CLICKED(IDC_BUTTON_SEND_BYTES, OnButtonSendBytes)
	ON_BN_CLICKED(IDC_BUTTON_READ_FIEL, OnButtonReadFiel)
	ON_BN_CLICKED(IDC_BUTTON_SIMULATE_DATA, OnButtonSimulateData)
	ON_BN_CLICKED(IDC_BUTTON_READ_MSMQ, OnButtonReadMsmq)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON2, &CRWMSMQDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CRWMSMQDlg::OnBnClickedButton3)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRWMSMQDlg message handlers

BOOL CRWMSMQDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	qDest = NULL;
	CoInitialize(NULL);
	try{
		//qInfo.CreateInstance("MSMQ.MSMQQueueInfo");
		qInfo.CreateInstance("MSMQ.MSMQQueueInfo");
		qRead.CreateInstance("MSMQ.MSMQQueueInfo");
		qMsg.CreateInstance("MSMQ.MSMQMessage");
		
	    lpszSystemInfo = tchBuffer; 
		GetComputerName(lpszSystemInfo,&cchBuff);    //  Getting the ComputerName
   		m_ServerName=lpszSystemInfo;
		UpdateData(FALSE);	
	}
	catch (_com_error)
	{
		MessageBox("Failed in CreateInstance of MSMQQueueInfo","MSMQ QueueInfo",1);
		PostQuitMessage(1);
		ReleaseAll();
	}
	SetTimer(TIMEID1,5000,0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRWMSMQDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRWMSMQDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRWMSMQDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRWMSMQDlg::OnCreate() 
{
	UpdateData(TRUE);
	HRESULT ht;
	int n;
   	try
	{
	//
	// m_ServerName = .\Private$\aaa 创建专有队列
	//
	//
	//
	// m_ServerName = .\aaa 公用队列
	//
//	qInfo->PathName = m_ServerName.AllocSysString();			 // Setting the Queue Path
	CString fname;
	fname="DIRECT=OS:";
		fname+=m_ServerName;
	qInfo->PathName = m_ServerName.AllocSysString();
	qInfo->FormatName = fname.AllocSysString();
	qInfo->Label= m_msgLabel.AllocSysString();		 // Setting the Queue Label
	ht = qInfo->Create();

	qInfo->Journal=0;								 // Enabling the Journal Option
	qInfo->Update();
	qInfo->Refresh();
	}
	catch (_com_error e)
	{
		CString str;
		str.Format("%s",e.ErrorMessage());

		MessageBox("Creating a Queue Failed","Create Queue",1);
		//ReleaseAll();
	}
	UpdateData(FALSE);
	m_msgLabel="";

}

void CRWMSMQDlg::OnSend() 
{
	UpdateData(TRUE);
	CString fname;
	VARIANT vMessage;

	try
	{
	// Create a direct format name of the queue, and
    // set the FormatName property of the MSMQQueueInfo object.

	//	qInfo->FormatName	L"DIRECT=TCP:10.9.255.71\\Private$\\test"	

		fname="DIRECT=OS:";
		fname+=m_ServerName;
		qInfo->PathName=m_ServerName.AllocSysString();
		qInfo->FormatName = fname.AllocSysString();    

		// Open the queue.

		qDest = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
		qMsg->Label = m_msgLabel.AllocSysString();
	
		VariantInit(&vMessage);
		vMessage.vt=VT_BSTR;
		vMessage.bstrVal=m_msgData.AllocSysString();
		qMsg->put_Body(vMessage);
		VariantClear(&vMessage);
		qMsg->Send(qDest);
		qDest->Close();
	
    }
	catch(...)
	{
		MessageBox("Sending the Queue failed","Send Queue",1);
		//ReleaseAll();
	}
	//catch(_com_error)
	//{
	//	MessageBox("Sending the Queue failed","Send Queue",1);
	//	//ReleaseAll();
	//}
	
}

void CRWMSMQDlg::OnRead() 
{
	UpdateData(TRUE);
	_variant_t vtReceiveTimeout;
	_bstr_t label,body;
	CString fname;
	int status=m_journal.GetCheck();
	try
	{
		qRead->PathName=m_ServerName.AllocSysString();
		fname="DIRECT=OS:";
		fname+=m_ServerName;
		
		if(status==1) fname=fname+";Journal";  // If Journal Option is Checked
		
		qInfo->PathName=m_ServerName.AllocSysString();

		qInfo->Journal = 0;
		qInfo->Update();

		qRead->FormatName = fname.AllocSysString();
		qDest = qRead->Open(MQ_RECEIVE_ACCESS,MQ_DENY_NONE);
		qMsg = qDest->Receive(&vtMissing,&vtMissing,&vtMissing,&vtReceiveTimeout);
		label=qMsg->GetLabel();
		body=qMsg->GetBody().bstrVal;
		m_msgLabel=(LPSTR)label;
		m_msgData=(LPCSTR)body;
		UpdateData(FALSE);
		qDest->Close();
    }
	catch(_com_error)
	{
		MessageBox("Reading the Queue Failed","Read Queue",1);	
		//ReleaseAll();
	}
	
}

void CRWMSMQDlg::OnClear() 
{
	m_msgLabel="";
	m_msgData="";
	UpdateData(FALSE);
}

void CRWMSMQDlg::OnExit() 
{
		::PostQuitMessage(1);
	
}

void CRWMSMQDlg::OnPrivate() 
{

	m_ServerName=lpszSystemInfo;
	m_ServerName+="\\Private$\\";
	UpdateData(FALSE);
}

void CRWMSMQDlg::OnPublic() 
{
	m_ServerName=lpszSystemInfo;
	m_ServerName+="\\Public$\\";
	UpdateData(FALSE);
}

void CRWMSMQDlg::OnJournal() 
{
	// TODO: Add your control notification handler code here
	
}
void CRWMSMQDlg::ReleaseAll()
{
	 qInfo->Release();
	 qRead->Release();
	 qMsg->Release();
	 CoUninitialize();
}

void CRWMSMQDlg::OnButtonSendBytes() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString fname;
//	VARIANT vMessage;
	CString strFileName = "";
	CFileDialog dlg(TRUE, NULL, NULL, NULL, NULL, NULL);

	dlg.DoModal();	
	strFileName = dlg.GetPathName();

	if (strFileName == "")
	{
		return;
	}
	try
	{
		// Create a direct format name of the queue, and
		// set the FormatName property of the MSMQQueueInfo object.
		
		//	qInfo->FormatName	L"DIRECT=TCP:10.9.255.71\\Private$\\test"	
		
		fname="DIRECT=OS:";
		fname+=m_ServerName;
		qInfo->PathName=m_ServerName.AllocSysString();
		qInfo->FormatName = fname.AllocSysString();    
		
		// Open the queue.
		
		qDest = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
		qMsg->Label = m_msgLabel.AllocSysString();
		
// 		VariantInit(&vMessage);
// 		vMessage.vt=VT_BSTR;
// 		vMessage.bstrVal=m_msgData.AllocSysString();
// 		qMsg->put_Body(vMessage);
// 		VariantClear(&vMessage);
		// read file
		CFile file;
		//file.Open("C:\\test0005.zip",CFile::modeRead|CFile::typeBinary);
		//file.Open("F:\\test\\log.txt",CFile::modeRead|CFile::typeBinary);
		//file.Open("C:\\log.txt",CFile::modeRead|CFile::typeBinary);
		file.Open(strFileName,CFile::modeRead|CFile::typeBinary);



		int iLen = file.GetLength();
		char* pData = new char[iLen+1];
		file.Seek(0,CFile::begin);
		file.Read(pData, iLen);
		file.Close();
		//处理数据
		
		//delete [] pData;
// 		BSTR bstrSend = SysAllocStringByteLen(NULL, iLen);
// 		LPBYTE pv = reinterpret_cast<LPBYTE>(bstrSend);
// 		CopyMemory(pv, (void *)pData, iLen);
// 		A aSend;
// 		BSTR bstrSend = SysAllocStringByteLen(NULL, sizeof(aSend));
// 		LPBYTE pv = reinterpret_cast<BYTE*>(bstrSend);
// 		CopyMemory(pv, (void *)&aSend, sizeof(aSend));
		_variant_t varBody;
		HRESULT hr = 0;
		LPSAFEARRAY lpsa = SafeArrayCreateVector(VT_UI1, 0, iLen);
		LPBYTE pbData = NULL;
		if (lpsa)	
			hr = SafeArrayAccessData(lpsa, (void **)&pbData);
		if (SUCCEEDED(hr))		
		{		
			CopyMemory(pbData, (void *) pData, iLen);		
			varBody.vt = VT_ARRAY|VT_UI1;		
			varBody.parray = lpsa;		
		}		
		if (pbData)	
			SafeArrayUnaccessData(varBody.parray);
		




// 		CComBSTR bstrBody;
// 		bstrBody.AppendBSTR(bstrSend);
// 		CComVariant varBody;
//		varBody = pv;

//		qMsg->put_Body(varBody);
		qMsg->Body = varBody;
	// read end


		qMsg->Send(qDest);
		qDest->Close();
		
		delete [] pData;
    }
	catch(_com_error e)
	{
		CString str;
		str.Format("%s",e.ErrorMessage());
		
		MessageBox(str,"Create Queue",1);
		//MessageBox("Sending the Queue failed","Send Queue",1);
		//ReleaseAll();
	}	
}

void CRWMSMQDlg::OnButtonReadFiel() 
{
	// TODO: Add your control notification handler code here
	CStdioFile file;
	file.Open("C:\\test0005.out",CFile::modeRead|CFile::typeBinary);
	int iLen = file.GetLength();
	char* pData = new char[iLen+1];
	file.Seek(0,CFile::begin);
	file.Read(pData, iLen);
		file.Close();	
}

void CRWMSMQDlg::OnButtonSimulateData() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString fname;
	//	VARIANT vMessage;
	
	try
	{
		ParseFile();



		// Create a direct format name of the queue, and
		// set the FormatName property of the MSMQQueueInfo object.
		
		//	qInfo->FormatName	L"DIRECT=TCP:10.9.255.71\\Private$\\test"	
		
		fname="DIRECT=OS:";
		fname+=m_ServerName;
		qInfo->PathName=m_ServerName.AllocSysString();
		qInfo->FormatName = fname.AllocSysString();    
		
		// Open the queue.
		
		qDest = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
		qMsg->Label = m_msgLabel.AllocSysString();
		
		// 		VariantInit(&vMessage);
		// 		vMessage.vt=VT_BSTR;
		// 		vMessage.bstrVal=m_msgData.AllocSysString();
		// 		qMsg->put_Body(vMessage);
		// 		VariantClear(&vMessage);
		// read file
		CFile file;
		//file.Open("C:\\test0005.zip",CFile::modeRead|CFile::typeBinary);
		//file.Open("F:\\test\\log20120725.txt",CFile::modeRead|CFile::typeBinary);
		//file.Open("F:\\test\\fwrite20120802.out",CFile::modeRead|CFile::typeBinary);
		file.Open("C:\\fwrite20120802.out",CFile::modeRead|CFile::typeBinary);
		int iLen = file.GetLength();
		char* pData = new char[iLen+1];
		file.Seek(0,CFile::begin);
		file.Read(pData, iLen);
		file.Close();
		//处理数据
		
		//delete [] pData;
		// 		BSTR bstrSend = SysAllocStringByteLen(NULL, iLen);
		// 		LPBYTE pv = reinterpret_cast<LPBYTE>(bstrSend);
		// 		CopyMemory(pv, (void *)pData, iLen);
		// 		A aSend;
		// 		BSTR bstrSend = SysAllocStringByteLen(NULL, sizeof(aSend));
		// 		LPBYTE pv = reinterpret_cast<BYTE*>(bstrSend);
		// 		CopyMemory(pv, (void *)&aSend, sizeof(aSend));
		_variant_t varBody;
		HRESULT hr = 0;
		LPSAFEARRAY lpsa = SafeArrayCreateVector(VT_UI1, 0, iLen);
		LPBYTE pbData = NULL;
		if (lpsa)	
			hr = SafeArrayAccessData(lpsa, (void **)&pbData);
		if (SUCCEEDED(hr))		
		{		
			CopyMemory(pbData, (void *) pData, iLen);		
			varBody.vt = VT_ARRAY|VT_UI1;		
			varBody.parray = lpsa;		
		}		
		if (pbData)	
			SafeArrayUnaccessData(varBody.parray);
		
		
		
		
		
		// 		CComBSTR bstrBody;
		// 		bstrBody.AppendBSTR(bstrSend);
		// 		CComVariant varBody;
		//		varBody = pv;
		
		//		qMsg->put_Body(varBody);
		qMsg->Body = varBody;
		// read end
		
		
		qMsg->Send(qDest);
		qDest->Close();
		
		delete [] pData;
    }
	catch(_com_error)
	{
		MessageBox("Sending the Queue failed","Send Queue",1);
		//ReleaseAll();
	}		
}

void CRWMSMQDlg::ParseFile(void)
{
	CStdioFile file;
	//file.Open("F:\\test\\log.txt",CFile::modeRead|CFile::typeBinary);
	file.Open("C:\\log.txt",CFile::modeRead|CFile::typeBinary);
	int iLen = file.GetLength();
	char* pData = new char[iLen+1];
	memset(pData, 0x00, sizeof(char) * (iLen + 1));
	file.Seek(0,CFile::begin);
	file.Read(pData, iLen);
	file.Close();	

	char szFieldSplit[3] = {(char)6, (char)8, 0x00};
	char szRecordSplit[4] = {(char)13, (char)8, (char)6, 0x00};
	int id = 0;
	int iTmpRowLen = 0;
	int iRowLen = 0;	// per row len
	char *pTmp = pData;
	vector<structShipInfoTmp> shipVect;

	for (int i = 0; i < iLen; ++i)
	{
		if (i < iLen && i + 1 < iLen && i + 2 < iLen)
		{
			if (pData[i] == szRecordSplit[0] && pData[i + 1] == szRecordSplit[1] && pData[i + 2] == szRecordSplit[2])
			{
				pData[i] = '\0';
				iRowLen = iTmpRowLen;
				int index = 0;
				int j = 0;
				struct structShipInfoTmp tmpData;

				// szSHIP_MMSI
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szSHIP_MMSI, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						break;
					}
					++j;
				}

				// szSHIP_IMO
				j += 2;
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szSHIP_IMO, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						break;
					}
					++j;
				}

				//// szSHIP_NAME_EN
				//j += 2;
				//while (j < iRowLen && j + 1 < iRowLen)
				//{
				//	if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				//	{
				//		pTmp[j] = '\0';
				//		memcpy(tmpData.szSHIP_NAME_EN, pTmp + index, strlen(pTmp + index));
				//		index += strlen(pTmp + index);
				//		index += 2;
				//		break;
				//	}
				//	++j;
				//}

				// szSHIP_NAME_EN
				string strSHIP_NAME_EN="";
				j += 2;
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szSHIP_NAME_EN, pTmp + index, j - index);
						//index += strlen(pTmp + index);
						int iTmpLen = j - index;
						index += iTmpLen;
						index += 2;
						if (tmpData.szSHIP_NAME_EN[0] != '\0')
						{
							ChineseCode::UTF_8ToGB2312(strSHIP_NAME_EN, tmpData.szSHIP_NAME_EN, iTmpLen + 1);
							memcpy(tmpData.szSHIP_NAME_EN, strSHIP_NAME_EN.c_str(), strSHIP_NAME_EN.size());
							tmpData.szSHIP_NAME_EN[strSHIP_NAME_EN.size()] = '\0';
						}
						break;
					}
					++j;
				}


				// szSHIP_ID
				//string strSHIP_ID="";
				j += 2;
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szSHIP_ID, pTmp + index, j - index);
						//index += strlen(pTmp + index);
						int iTmpLen = j - index;
						index += iTmpLen;
						index += 2;
						//if (tmpData.szSHIP_ID[0] != '\0')
						//{
						//	ChineseCode::UTF_8ToGB2312(strSHIP_ID, tmpData.szSHIP_ID, iTmpLen + 1);
						//	memcpy(tmpData.szSHIP_ID, strSHIP_ID.c_str(), strSHIP_ID.size());
						//	tmpData.szSHIP_ID[strSHIP_ID.size()] = '\0';
						//}
						break;
					}
					++j;
				}

				//// szSHIP_NATIONALITY
				//string strSHIP_NATIONALITY="";
				//j += 2;
				//while (j < iRowLen && j + 1 < iRowLen)
				//{
				//	if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				//	{
				//		pTmp[j] = '\0';
				//		memcpy(tmpData.szSHIP_NATIONALITY, pTmp + index, j - index);
				//		//index += strlen(pTmp + index);
				//		int iTmpLen = j - index;
				//		index += iTmpLen;
				//		index += 2;
				//		if (tmpData.szSHIP_NATIONALITY[0] != '\0')
				//		{
				//			ChineseCode::UTF_8ToGB2312(strSHIP_NATIONALITY, tmpData.szSHIP_NATIONALITY, iTmpLen + 1);
				//			memcpy(tmpData.szSHIP_NATIONALITY, strSHIP_NATIONALITY.c_str(), strSHIP_NATIONALITY.size());
				//			tmpData.szSHIP_NATIONALITY[strSHIP_NATIONALITY.size()] = '\0';
				//		}

				//		break;
				//	}
				//	++j;
				//}


				// szSHIP_TYPE
				string strSHIP_TYPE="";
				j += 2;
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szSHIP_TYPE, pTmp + index, j - index);
						//index += strlen(pTmp + index);
						int iTmpLen = j - index;
						index += iTmpLen;
						index += 2;
						if (tmpData.szSHIP_TYPE[0] != '\0')
						{
							ChineseCode::UTF_8ToGB2312(strSHIP_TYPE, tmpData.szSHIP_TYPE, iTmpLen + 1);
							memcpy(tmpData.szSHIP_TYPE, strSHIP_TYPE.c_str(), strSHIP_TYPE.size());
							tmpData.szSHIP_TYPE[strSHIP_TYPE.size()] = '\0';
						}
						break;
					}
					++j;
				}

				// nSHIP_LENGTH
				j += 2;
				char szSHIP_LENGTH[10 + 1] = {0};
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(szSHIP_LENGTH, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						tmpData.nSHIP_LENGTH = atoi(szSHIP_LENGTH);

						//memcpy(&tmpData.nSHIP_LENGTH, pTmp + index, sizeof(tmpData.nSHIP_LENGTH));
						//index += 4;
						//index += 2;
						break;
					}
					++j;
				}


				// nSHIP_WIDTH
				j += 2;
				char szSHIP_WIDTH[10 + 1] = {0};
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(szSHIP_WIDTH, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						tmpData.nSHIP_WIDTH = atoi(szSHIP_WIDTH);

						//memcpy(&tmpData.nSHIP_WIDTH, pTmp + index, sizeof(tmpData.nSHIP_WIDTH));
						//index += 4;
						//index += 2;
						break;
					}
					++j;
				}




				// szSHIP_DRAW
				//string strSHIP_DRAW="";
				j += 2;
				char szSHIP_DRAW[10 + 1] = {0};
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(szSHIP_DRAW, pTmp + index, j - index);
						//index += strlen(pTmp + index);
						int iTmpLen = j - index;
						index += iTmpLen;
						index += 2;
						tmpData.szSHIP_DRAW = atof(szSHIP_DRAW);
						//ChineseCode::UTF_8ToGB2312(strSHIP_DRAW, tmpData.szSHIP_DRAW, iTmpLen + 1);
						//memcpy(tmpData.szSHIP_DRAW, strSHIP_DRAW.c_str(), strSHIP_DRAW.size());
						//tmpData.szSHIP_NATIONALITY[strSHIP_DRAW.size()] = '\0';
						break;
					}
					++j;
				}


				// dSHIP_LATITUDE
				j += 2;
				char szSHIP_LATITUDE[32 + 1] = {0};
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(szSHIP_LATITUDE, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						tmpData.dSHIP_LATITUDE = atof(szSHIP_LATITUDE);

						//memcpy(&tmpData.dSHIP_LATITUDE, pTmp + index, sizeof(tmpData.dSHIP_LATITUDE));
						//index += 8;
						//index += 2;
						break;
					}
					++j;
				}


				// dSHIP_LONGITUDE
				j += 2;
				char szSHIP_LONGITUDE[32 + 1] = {0};
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(szSHIP_LONGITUDE, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						tmpData.dSHIP_LONGITUDE = atof(szSHIP_LONGITUDE);

						//memcpy(&tmpData.dSHIP_LONGITUDE, pTmp + index, sizeof(tmpData.dSHIP_LONGITUDE));
						//index += 8;
						//index += 2;
						break;
					}
					++j;
				}


				// szSHIP_DIRECTION
				j += 2;
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szSHIP_DIRECTION, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						break;
					}
					++j;
				}

				// szSHIP_SIGN
				j += 2;
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szSHIP_SIGN, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						break;
					}
					++j;
				}



				// nSHIP_SPEED
				j += 2;
				char szSHIP_SPEED[10 + 1] = {0};
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(szSHIP_SPEED, pTmp + index, strlen(pTmp + index));
						index += strlen(pTmp + index);
						index += 2;
						tmpData.nSHIP_SPEED = atof(szSHIP_SPEED);

						//memcpy(&tmpData.nSHIP_SPEED, pTmp + index, sizeof(tmpData.nSHIP_SPEED));
						//index += 4;
						//index += 2;
						break;
					}
					++j;
				}

				// szSHIP_DESTINATION
				//string strSHIP_DESTINATION="";
				j += 2;
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szSHIP_DESTINATION, pTmp + index, j - index);
						//index += strlen(pTmp + index);
						int iTmpLen = j - index;
						index += iTmpLen;
						index += 2;
						//ChineseCode::UTF_8ToGB2312(strSHIP_DESTINATION, tmpData.szSHIP_DESTINATION, iTmpLen + 1);
						//memcpy(tmpData.szSHIP_DESTINATION, strSHIP_DESTINATION.c_str(), strSHIP_DESTINATION.size());
						//tmpData.szSHIP_DESTINATION[strSHIP_DESTINATION.size()] = '\0';
						break;
					}
					++j;
				}


				// szSHIP_DESTINATION
				string strTimeReserve="";
				j += 2;
				while (j < iRowLen && j + 1 < iRowLen)
				{
					if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
					{
						pTmp[j] = '\0';
						memcpy(tmpData.szTimeReserve, pTmp + index, j - index);
						//index += strlen(pTmp + index);
						int iTmpLen = j - index;
						index += iTmpLen;
						index += 2;
						if (tmpData.szTimeReserve[0] != '\0')
						{
							ChineseCode::UTF_8ToGB2312(strTimeReserve, tmpData.szTimeReserve, iTmpLen + 1);
							memcpy(tmpData.szTimeReserve, strTimeReserve.c_str(), strTimeReserve.size());
							tmpData.szTimeReserve[strTimeReserve.size()] = '\0';
						}

						break;
					}
					++j;
				}

				// szEXPECTED_DATE_ARRIVAL
				//string strEXPECTED_DATE_ARRIVAL="";
				j += 2;
				if (j < iRowLen && j + 1 < iRowLen)
				{
					//if (pData[j] == szFieldSplit[0] && pData[j + 1] == szFieldSplit[1])
					{
						//	pTmp[index] = '\0';
						memcpy(tmpData.szEXPECTED_DATE_ARRIVAL, pTmp + index, iRowLen - index);
						//	index += strlen(pTmp + index);
						int iTmpLen = iRowLen - index;
						index += iTmpLen;
						//	index += 2;
						//ChineseCode::UTF_8ToGB2312(strEXPECTED_DATE_ARRIVAL, tmpData.szEXPECTED_DATE_ARRIVAL, iTmpLen + 1);
						//memcpy(tmpData.szEXPECTED_DATE_ARRIVAL, strEXPECTED_DATE_ARRIVAL.c_str(), strEXPECTED_DATE_ARRIVAL.size());
						//tmpData.szEXPECTED_DATE_ARRIVAL[strEXPECTED_DATE_ARRIVAL.size()] = '\0';
						//	break;
					}
					// ++j;
				}
				shipVect.push_back(tmpData);
				pTmp += iTmpRowLen;
				pTmp += 3;
				iTmpRowLen = 0;
				i += 2;
			}
			else
			{
				++iTmpRowLen;
			}
		}
		else
		{
			iTmpRowLen += 2;
			iRowLen = iTmpRowLen;
			int index = 0;
			int j = 0;
			struct structShipInfoTmp tmpData;

			// szSHIP_MMSI
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szSHIP_MMSI, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					break;
				}
				++j;
			}

			// szSHIP_IMO
			j += 2;
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szSHIP_IMO, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					break;
				}
				++j;
			}

			// szSHIP_NAME_EN
			string strSHIP_NAME_EN="";
			j += 2;
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szSHIP_NAME_EN, pTmp + index, j - index);
					//index += strlen(pTmp + index);
					int iTmpLen = j - index;
					index += iTmpLen;
					index += 2;
					if (tmpData.szSHIP_NAME_EN[0] != '\0')
					{
						ChineseCode::UTF_8ToGB2312(strSHIP_NAME_EN, tmpData.szSHIP_NAME_EN, iTmpLen + 1);
						memcpy(tmpData.szSHIP_NAME_EN, strSHIP_NAME_EN.c_str(), strSHIP_NAME_EN.size());
						tmpData.szSHIP_NAME_EN[strSHIP_NAME_EN.size()] = '\0';
					}
					break;
				}
				++j;
			}

			// szSHIP_ID
			j += 2;
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szSHIP_ID, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					break;
				}
				++j;
			}

			//// szSHIP_NATIONALITY
			//j += 2;
			//while (j < iRowLen && j + 1 < iRowLen)
			//{
			//	if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
			//	{
			//		pTmp[j] = '\0';
			//		memcpy(tmpData.szSHIP_NATIONALITY, pTmp + index, strlen(pTmp + index));
			//		index += strlen(pTmp + index);
			//		index += 2;
			//		break;
			//	}
			//	++j;
			//}
			// szSHIP_NATIONALITY
			//string strSHIP_NATIONALITY="";
			//j += 2;
			//while (j < iRowLen && j + 1 < iRowLen)
			//{
			//	if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
			//	{
			//		pTmp[j] = '\0';
			//		memcpy(tmpData.szSHIP_NATIONALITY, pTmp + index, j - index);
			//		//index += strlen(pTmp + index);
			//		int iTmpLen = j - index;
			//		index += iTmpLen;
			//		index += 2;
			//		if (tmpData.szSHIP_NATIONALITY[0] != '\0')
			//		{
			//			ChineseCode::UTF_8ToGB2312(strSHIP_NATIONALITY, tmpData.szSHIP_NATIONALITY, iTmpLen + 1);
			//			memcpy(tmpData.szSHIP_NATIONALITY, strSHIP_NATIONALITY.c_str(), strSHIP_NATIONALITY.size());
			//			tmpData.szSHIP_NATIONALITY[strSHIP_NATIONALITY.size()] = '\0';
			//		}

			//		break;
			//	}
			//	++j;
			//}

			//// szSHIP_TYPE
			//j += 2;
			//while (j < iRowLen && j + 1 < iRowLen)
			//{
			//	if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
			//	{
			//		pTmp[j] = '\0';
			//		memcpy(tmpData.szSHIP_TYPE, pTmp + index, strlen(pTmp + index));
			//		index += strlen(pTmp + index);
			//		index += 2;
			//		break;
			//	}
			//	++j;
			//}

			// szSHIP_TYPE
			string strSHIP_TYPE="";
			j += 2;
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szSHIP_TYPE, pTmp + index, j - index);
					//index += strlen(pTmp + index);
					int iTmpLen = j - index;
					index += iTmpLen;
					index += 2;
					if (tmpData.szSHIP_TYPE[0] != '\0')
					{
						ChineseCode::UTF_8ToGB2312(strSHIP_TYPE, tmpData.szSHIP_TYPE, iTmpLen + 1);
						memcpy(tmpData.szSHIP_TYPE, strSHIP_TYPE.c_str(), strSHIP_TYPE.size());
						tmpData.szSHIP_TYPE[strSHIP_TYPE.size()] = '\0';
					}
					break;
				}
				++j;
			}


			// nSHIP_LENGTH
			j += 2;
			char szSHIP_LENGTH[10 + 1] = {0};
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(szSHIP_LENGTH, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					tmpData.nSHIP_LENGTH = atoi(szSHIP_LENGTH);

					//memcpy(&tmpData.nSHIP_LENGTH, pTmp + index, sizeof(tmpData.nSHIP_LENGTH));
					//index += 4;
					//index += 2;
					break;
				}
				++j;
			}


			// nSHIP_WIDTH
			j += 2;
			char szSHIP_WIDTH[10 + 1] = {0};
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(szSHIP_WIDTH, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					tmpData.nSHIP_WIDTH = atoi(szSHIP_WIDTH);

					//memcpy(&tmpData.nSHIP_WIDTH, pTmp + index, sizeof(tmpData.nSHIP_WIDTH));
					//index += 4;
					//index += 2;
					break;
				}
				++j;
			}

			//// szSHIP_DRAW
			//j += 2;
			//while (j < iRowLen && j + 1 < iRowLen)
			//{
			//	if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
			//	{
			//		pTmp[j] = '\0';
			//		memcpy(tmpData.szSHIP_DRAW, pTmp + index, strlen(pTmp + index));
			//		index += strlen(pTmp + index);
			//		index += 2;
			//		break;
			//	}
			//	++j;
			//}
			// szSHIP_DRAW
			//string strSHIP_DRAW="";
			j += 2;
			char szSHIP_DRAW[10 + 1] = {0};
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(szSHIP_DRAW, pTmp + index, j - index);
					//index += strlen(pTmp + index);
					int iTmpLen = j - index;
					index += iTmpLen;
					index += 2;
					tmpData.szSHIP_DRAW = atof(szSHIP_DRAW);
					//ChineseCode::UTF_8ToGB2312(strSHIP_DRAW, tmpData.szSHIP_DRAW, iTmpLen + 1);
					//memcpy(tmpData.szSHIP_DRAW, strSHIP_DRAW.c_str(), strSHIP_DRAW.size());
					//tmpData.szSHIP_NATIONALITY[strSHIP_DRAW.size()] = '\0';
					break;
				}
				++j;
			}



			// dSHIP_LATITUDE
			j += 2;
			char szSHIP_LATITUDE[32 + 1] = {0};
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(szSHIP_LATITUDE, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					tmpData.dSHIP_LATITUDE = atof(szSHIP_LATITUDE);

					//memcpy(&tmpData.dSHIP_LATITUDE, pTmp + index, sizeof(tmpData.dSHIP_LATITUDE));
					//index += 8;
					//index += 2;
					break;
				}
				++j;
			}


			// dSHIP_LONGITUDE
			j += 2;
			char szSHIP_LONGITUDE[32 + 1] = {0};
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(szSHIP_LONGITUDE, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					tmpData.dSHIP_LONGITUDE = atof(szSHIP_LONGITUDE);

					//memcpy(&tmpData.dSHIP_LONGITUDE, pTmp + index, sizeof(tmpData.dSHIP_LONGITUDE));
					//index += 8;
					//index += 2;
					break;
				}
				++j;
			}


			// szSHIP_DIRECTION
			j += 2;
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szSHIP_DIRECTION, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					break;
				}
				++j;
			}

			// szSHIP_SIGN
			j += 2;
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szSHIP_SIGN, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					break;
				}
				++j;
			}



			// nSHIP_SPEED
			j += 2;
			char szSHIP_SPEED[10 + 1] = {0};
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(szSHIP_SPEED, pTmp + index, strlen(pTmp + index));
					index += strlen(pTmp + index);
					index += 2;
					tmpData.nSHIP_SPEED = atof(szSHIP_SPEED);


					//memcpy(&tmpData.nSHIP_SPEED, pTmp + index, sizeof(tmpData.nSHIP_SPEED));
					//index += 4;
					//index += 2;
					break;
				}
				++j;
			}

			//// szSHIP_DESTINATION
			//j += 2;
			//while (j < iRowLen && j + 1 < iRowLen)
			//{
			//	if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
			//	{
			//		pTmp[j] = '\0';
			//		memcpy(tmpData.szSHIP_DESTINATION, pTmp + index, strlen(pTmp + index));
			//		index += strlen(pTmp + index);
			//		index += 2;
			//		break;
			//	}
			//	++j;
			//}

			// szSHIP_DESTINATION
			//string strSHIP_DESTINATION="";
			j += 2;
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szSHIP_DESTINATION, pTmp + index, j - index);
					//index += strlen(pTmp + index);
					int iTmpLen = j - index;
					index += iTmpLen;
					index += 2;
					//ChineseCode::UTF_8ToGB2312(strSHIP_DESTINATION, tmpData.szSHIP_DESTINATION, iTmpLen + 1);
					//memcpy(tmpData.szSHIP_DESTINATION, strSHIP_DESTINATION.c_str(), strSHIP_DESTINATION.size());
					//tmpData.szSHIP_DESTINATION[strSHIP_DESTINATION.size()] = '\0';
					break;
				}
				++j;
			}

			// szSHIP_DESTINATION
			string strTimeReserve="";
			j += 2;
			while (j < iRowLen && j + 1 < iRowLen)
			{
				if (pTmp[j] == szFieldSplit[0] && pTmp[j + 1] == szFieldSplit[1])
				{
					pTmp[j] = '\0';
					memcpy(tmpData.szTimeReserve, pTmp + index, j - index);
					//index += strlen(pTmp + index);
					int iTmpLen = j - index;
					index += iTmpLen;
					index += 2;
					if (tmpData.szTimeReserve[0] != '\0')
					{
						ChineseCode::UTF_8ToGB2312(strTimeReserve, tmpData.szTimeReserve, iTmpLen + 1);
						memcpy(tmpData.szTimeReserve, strTimeReserve.c_str(), strTimeReserve.size());
						tmpData.szTimeReserve[strTimeReserve.size()] = '\0';
					}
					break;
				}
				++j;
			}

			//// szEXPECTED_DATE_ARRIVAL
			////while (j < iRowLen && j + 1 < iRowLen)
			//{
			//	//if (pData[j] == szFieldSplit[0] && pData[j + 1] == szFieldSplit[1])
			//	{
			//		//	pTmp[index] = '\0';
			//		memcpy(tmpData.szEXPECTED_DATE_ARRIVAL, pTmp + index, strlen(pTmp + index));
			//		index += strlen(pTmp + index);
			//		//	index += 2;
			//		//	break;
			//	}
			//	// ++j;
			//}
			// szEXPECTED_DATE_ARRIVAL
			//string strEXPECTED_DATE_ARRIVAL="";
			j += 2;
			if (j < iRowLen && j + 1 < iRowLen)
			{
				//if (pData[j] == szFieldSplit[0] && pData[j + 1] == szFieldSplit[1])
				{
					//	pTmp[index] = '\0';
					memcpy(tmpData.szEXPECTED_DATE_ARRIVAL, pTmp + index, iRowLen - index);
					//	index += strlen(pTmp + index);
					int iTmpLen = iRowLen - index;
					index += iTmpLen;
					//	index += 2;
					//ChineseCode::UTF_8ToGB2312(strEXPECTED_DATE_ARRIVAL, tmpData.szEXPECTED_DATE_ARRIVAL, iTmpLen + 1);
					//memcpy(tmpData.szEXPECTED_DATE_ARRIVAL, strEXPECTED_DATE_ARRIVAL.c_str(), strEXPECTED_DATE_ARRIVAL.size());
					//tmpData.szEXPECTED_DATE_ARRIVAL[strEXPECTED_DATE_ARRIVAL.size()] = '\0';
					//	break;
				}
				// ++j;
			}


			shipVect.push_back(tmpData);
			//pTmp += iTmpRowLen;
			//pTmp += 3;
			//iTmpRowLen = 0;
			break;
		}
	}

	delete []pData;
	pData = NULL;

	GenerateFile(shipVect);
}

void CRWMSMQDlg::GenerateFile(vector<structShipInfoTmp> &shipVect)
{
	static int nIterator = 1;
	FILE *stream = NULL;
	char szFieldSplit[3] = {(char)6, (char)8, 0x00};
	char szRecordSplit[4] = {(char)13, (char)8, (char)6, 0x00};
	// Open file :
	//	timer t;
	//if( fopen_s( &stream, "F:\\test\\fwrite20120802.out", "w+b" ) == 0 )
	if( (stream = fopen("C:\\fwrite20120802.out", "w+b" )) == 0 )
	{
		for (int i = 0; i < shipVect.size(); ++i)
		{
			int index = 0;
			char szBuffer[2048] = {0};
			char *pTmp = szBuffer;

			memcpy(pTmp + index, shipVect[i].szSHIP_MMSI, strlen(shipVect[i].szSHIP_MMSI));
			index += strlen(shipVect[i].szSHIP_MMSI);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			memcpy(pTmp + index, shipVect[i].szSHIP_IMO, strlen(shipVect[i].szSHIP_IMO));
			index += strlen(shipVect[i].szSHIP_IMO);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			//memcpy(pTmp + index, shipVect[i].szSHIP_NAME_EN, strlen(shipVect[i].szSHIP_NAME_EN));
			//index += strlen(shipVect[i].szSHIP_NAME_EN);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			memcpy(pTmp + index, shipVect[i].szSHIP_ID, strlen(shipVect[i].szSHIP_ID));
			index += strlen(shipVect[i].szSHIP_ID);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			//memcpy(pTmp + index, shipVect[i].szSHIP_NATIONALITY, strlen(shipVect[i].szSHIP_NATIONALITY));
			//index += strlen(shipVect[i].szSHIP_NATIONALITY);
			//memcpy(pTmp + index, szFieldSplit, 1);
			//index += 1;

			//memcpy(pTmp + index, shipVect[i].szSHIP_TYPE, strlen(shipVect[i].szSHIP_TYPE));
			//index += strlen(shipVect[i].szSHIP_TYPE);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			char szBuffer2[100] = {0};
			sprintf(szBuffer2, "%d", shipVect[i].nSHIP_LENGTH);
			memcpy(pTmp + index, szBuffer2, strlen(szBuffer2));
			index += strlen(szBuffer2);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;


			memset(szBuffer2, 0x00, 100);
			sprintf(szBuffer2, "%d", shipVect[i].nSHIP_WIDTH);
			memcpy(pTmp + index, szBuffer2, strlen(szBuffer2));
			index += strlen(szBuffer2);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			memset(szBuffer2, 0x00, 100);
			sprintf(szBuffer2, "%lf", shipVect[i].szSHIP_DRAW);
			memcpy(pTmp + index, szBuffer2, strlen(szBuffer2));
			index += strlen(szBuffer2);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			memset(szBuffer2, 0x00, 100);
			double dfTmp = shipVect[i].dSHIP_LATITUDE;
			dfTmp = dfTmp + (double)(nIterator + 1); 

			sprintf(szBuffer2, "%lf", dfTmp);
			memcpy(pTmp + index, szBuffer2, strlen(szBuffer2));
			index += strlen(szBuffer2);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;




			memset(szBuffer2, 0x00, 100);
			dfTmp = shipVect[i].dSHIP_LONGITUDE;
			dfTmp = dfTmp + (double)(nIterator + 1); 
			sprintf(szBuffer2, "%lf", dfTmp);
			memcpy(pTmp + index, szBuffer2, strlen(szBuffer2));
			index += strlen(szBuffer2);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			memcpy(pTmp + index, shipVect[i].szSHIP_DIRECTION, strlen(shipVect[i].szSHIP_DIRECTION));
			index += strlen(shipVect[i].szSHIP_DIRECTION);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			memcpy(pTmp + index, shipVect[i].szSHIP_SIGN, strlen(shipVect[i].szSHIP_SIGN));
			index += strlen(shipVect[i].szSHIP_SIGN);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;


			memset(szBuffer2, 0x00, 100);
			sprintf(szBuffer2, "%f", shipVect[i].nSHIP_SPEED);
			memcpy(pTmp + index, szBuffer2, strlen(szBuffer2));
			index += strlen(szBuffer2);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			memcpy(pTmp + index, shipVect[i].szSHIP_DESTINATION, strlen(shipVect[i].szSHIP_DESTINATION));
			index += strlen(shipVect[i].szSHIP_DESTINATION);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;

			//memcpy(pTmp + index, shipVect[i].szTimeReserve, strlen(shipVect[i].szTimeReserve));
			//index += strlen(shipVect[i].szTimeReserve);
			memcpy(pTmp + index, szFieldSplit, 2);
			index += 2;


			COleDateTime dateTest = COleDateTime::GetCurrentTime();
			int nYear2 = dateTest.GetYear();
			int nMonth2 = dateTest.GetMonth();
			int nDay2 = dateTest.GetDay();
			int nHour2 = dateTest.GetHour();
			int nMinute2 = dateTest.GetMinute();
			int nSecond2 = dateTest.GetSecond();
			char szTime02[100] = {0};	// shipVect[i].szEXPECTED_DATE_ARRIVAL
			sprintf(szTime02, "%d-%d-%dT%d:%d:%d", nYear2, nMonth2, nDay2, nHour2, nMinute2, nSecond2);

			memcpy(pTmp + index, szTime02, strlen(szTime02));
			index += strlen(szTime02);
			memcpy(pTmp + index, szRecordSplit, 3);
			index += 3;

			fwrite( szBuffer, sizeof(char), index, stream);
		}
		fclose( stream );

	}
	nIterator++;
}


void CRWMSMQDlg::OnButtonReadMsmq() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	m_ServerName = "192.168.15.125\\private$\\sanbao";

	_variant_t vtReceiveTimeout;
	_bstr_t label,body;
	CString fname;
	int status=m_journal.GetCheck();
	try
	{
		qRead->PathName=m_ServerName.AllocSysString();
		fname="DIRECT=TCP:";
		fname+=m_ServerName;

		if(status==1) fname=fname+";Journal";  // If Journal Option is Checked

		qInfo->PathName=m_ServerName.AllocSysString();

		//qInfo->Journal = 0;
		//qInfo->Update();

		qRead->FormatName = fname.AllocSysString();
		qDest = qRead->Open(MQ_RECEIVE_ACCESS,MQ_DENY_NONE);
		qMsg = qDest->Receive(&vtMissing,&vtMissing,&vtMissing,&vtReceiveTimeout);
		label=qMsg->GetLabel();
		body=qMsg->GetBody().bstrVal;
		m_msgLabel=(LPSTR)label;
		m_msgData=(LPCSTR)body;
		UpdateData(FALSE);
		qDest->Close();
	}
	catch(_com_error)
	{
		DWORD dwError = GetLastError();
		MessageBox("Reading the Queue Failed","Read Queue",1);	
		//ReleaseAll();
	}	
}

void CRWMSMQDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case TIMEID1:
		{
			break;
		}

	default:
		break;
	}


	CDialog::OnTimer(nIDEvent);
}

void CRWMSMQDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	CString fname;
	VARIANT vMessage;

	try
	{
		// Create a direct format name of the queue, and
		// set the FormatName property of the MSMQQueueInfo object.

		//	qInfo->FormatName	L"DIRECT=TCP:10.9.255.71\\Private$\\test"	

		fname="DIRECT=OS:";
		fname+=m_ServerName;
		qInfo->PathName=m_ServerName.AllocSysString();
		//qInfo->FormatName = fname.AllocSysString();    
		qInfo->FormatName = "DIRECT=TCP:192.168.32.27\\Private$\\whhg_OutToIn"; 

		//qInfo->FormatName = "DIRECT=TCP:27.17.53.220\\Private$\\Test"; 
		// Open the queue.

		VARIANT varIsTransactional;
		VariantInit(&varIsTransactional);
		varIsTransactional.vt = VT_I2;
		varIsTransactional.iVal = MQ_SINGLE_MESSAGE;
		//varIsTransactional.vt = VT_BOOL;
		//varIsTransactional.boolVal = MQ_TRANSACTIONAL;

		//VariantInit(&varIsWorldReadable);
		//varIsWorldReadable.vt = VT_BOOL;
		//varIsWorldReadable.boolVal = FALSE;MQ_SINGLE_MESSAGE

		qDest = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);

		qMsg->Label = m_msgLabel.AllocSysString();

		VariantInit(&vMessage);
		vMessage.vt=VT_BSTR;
		vMessage.bstrVal=m_msgData.AllocSysString();
		qMsg->put_Body(vMessage);
		VariantClear(&vMessage);

	
		qMsg->Send(qDest, &varIsTransactional);
		qDest->Close();
		VariantClear(&varIsTransactional);

	}
	catch(_com_error &comerr)
	{
		//TRACE("%s", e.ErrorMessage());

		HRESULT hr = comerr.Error();

		TRACE("Error Code = 0x%X\nError Description = %s\n", hr, (char *)comerr.Description());
		MessageBox("Sending the Queue failed","Send Queue",1);
		//ReleaseAll();
	}

}

void CRWMSMQDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);
	CString fname;
	//	VARIANT vMessage;
	CString strFileName = "";
	CFileDialog dlg(TRUE, NULL, NULL, NULL, NULL, NULL);

	dlg.DoModal();	
	strFileName = dlg.GetPathName();

	if (strFileName == "")
	{
		return;
	}
	try
	{
		// Create a direct format name of the queue, and
		// set the FormatName property of the MSMQQueueInfo object.

		//	qInfo->FormatName	L"DIRECT=TCP:10.9.255.71\\Private$\\test"	

		fname="DIRECT=OS:";
		fname+=m_ServerName;
		qInfo->PathName=m_ServerName.AllocSysString();
		qInfo->FormatName = fname.AllocSysString();    

		// Open the queue.

		qDest = qInfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
		qMsg->Label = m_msgLabel.AllocSysString();

		// 		VariantInit(&vMessage);
		// 		vMessage.vt=VT_BSTR;
		// 		vMessage.bstrVal=m_msgData.AllocSysString();
		// 		qMsg->put_Body(vMessage);
		// 		VariantClear(&vMessage);
		// read file
		CFile file;
		//file.Open("C:\\test0005.zip",CFile::modeRead|CFile::typeBinary);
		//file.Open("F:\\test\\log.txt",CFile::modeRead|CFile::typeBinary);
		//file.Open("C:\\log.txt",CFile::modeRead|CFile::typeBinary);
		file.Open(strFileName,CFile::modeRead|CFile::typeBinary);



		int iLen = file.GetLength();
		char* pData = new char[iLen+1];
		file.Seek(0,CFile::begin);
		file.Read(pData, iLen);
		file.Close();
		//处理数据

		//delete [] pData;
		// 		BSTR bstrSend = SysAllocStringByteLen(NULL, iLen);
		// 		LPBYTE pv = reinterpret_cast<LPBYTE>(bstrSend);
		// 		CopyMemory(pv, (void *)pData, iLen);
		// 		A aSend;
		// 		BSTR bstrSend = SysAllocStringByteLen(NULL, sizeof(aSend));
		// 		LPBYTE pv = reinterpret_cast<BYTE*>(bstrSend);
		// 		CopyMemory(pv, (void *)&aSend, sizeof(aSend));
		_variant_t varBody;
		HRESULT hr = 0;
		LPSAFEARRAY lpsa = SafeArrayCreateVector(VT_UI1, 0, iLen);
		LPBYTE pbData = NULL;
		if (lpsa)	
			hr = SafeArrayAccessData(lpsa, (void **)&pbData);
		if (SUCCEEDED(hr))		
		{		
			CopyMemory(pbData, (void *) pData, iLen);		
			varBody.vt = VT_ARRAY|VT_UI1;		
			varBody.parray = lpsa;		
		}		
		if (pbData)	
			SafeArrayUnaccessData(varBody.parray);





		// 		CComBSTR bstrBody;
		// 		bstrBody.AppendBSTR(bstrSend);
		// 		CComVariant varBody;
		//		varBody = pv;

		//		qMsg->put_Body(varBody);
		qMsg->Body = varBody;
		// read end
		VARIANT varIsTransactional;
		VariantInit(&varIsTransactional);
		varIsTransactional.vt = VT_I2;
		varIsTransactional.iVal = MQ_SINGLE_MESSAGE;

		//qMsg->Send(qDest);
		//qDest->Close();
		qMsg->Send(qDest, &varIsTransactional);
		qDest->Close();
		VariantClear(&varIsTransactional);

		delete [] pData;
	}
	catch(_com_error e)
	{
		CString str;
		str.Format("%s",e.ErrorMessage());

		MessageBox(str,"Create Queue",1);
		//MessageBox("Sending the Queue failed","Send Queue",1);
		//ReleaseAll();
	}	

}
