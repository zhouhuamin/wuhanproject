// BasicTransact.h: interface for the CBasicTransact class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICTRANSACT_H__8DACE64B_2595_4995_97B2_C43634C414B4__INCLUDED_)
#define AFX_BASICTRANSACT_H__8DACE64B_2595_4995_97B2_C43634C414B4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../CurrencyCommunicate/Global.h"
#define DEBUG
#undef  DEBUG
#include "mylock.h"

struct struMsgTran
{
	TCHAR strIp[100];
	TCHAR strMsg[1000];
};

class CBasicTransact : public CObject  
{
public:
	CBasicTransact();
	virtual ~CBasicTransact();

public:
	virtual char OnGetConfig()
	{
		return 0;
	}

    virtual BOOL OnDealData(CBasicConfigInfo &PackInfo);

	BOOL OnSetConfig(HWND hWndDlg);

	CString OnGetXmlInfo(CBasicConfigInfo &PackInfo);

	int GetEmptySocket(SOCKET sockArray[WSA_MAXIMUM_WAIT_EVENTS]);	
	BOOL OnSetupConnect(SOCKET &clientSocket, CString strIP, int nPort, int nSendTimeout, int nRecvTimeout,int nReConnectCount=0);  

	BOOL OnBasicDealInfo(char *pData, CString strIp, int nPort, int nLength, CBasicConfigInfo &PackInfo);
public:
	char * GetRecordsetBlobInfo(CString strField,_RecordsetPtr RdsetPtr,int *pLen);

	_bstr_t GetRecordsetInfo(CString strField, _RecordsetPtr RdsetPtr);
	CString BuildXmlValue(CString strTable, CString strValue);

    HWND  m_Hwnd;
	void ToWriteFile(CString strFileName, BYTE *pbyBuff, int nLen);
};

#endif // !defined(AFX_BASICTRANSACT_H__8DACE64B_2595_4995_97B2_C43634C414B4__INCLUDED_)
