// ModuleInfo.h: interface for the CModuleInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULEINFO_H__4896F6A8_86D5_4F58_935C_FEA38767D433__INCLUDED_)
#define AFX_MODULEINFO_H__4896F6A8_86D5_4F58_935C_FEA38767D433__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <Afxmt.h>
#include "Global.h"


class CModuleInfo : public CObject  
{
public:
	CModuleInfo();
	virtual ~CModuleInfo();

public:
	virtual BOOL OnIsExist(char * strIp, int nPort);
	virtual LONG OnDealInfo(LPPER_IO_OPERATION_DATA PerIoData, CBasicConfigInfo &ConfigInfo);
    virtual LONG OnDealInfo();

public:
	BOOL GetIsFirstWorkItem();
	BOOL GetStart();
	BOOL Realse();
	CCriticalSection m_lock;

private:
	CString OnGetXmlInfo(char* szPacket);

    BOOL ComparePackHead(char szBuf[4]);
	BOOL UnchainPackHead(char *pPacket);
	
private:
	int  m_nAllLength;
	int  m_nCurLength;
	int  m_nClientPort;


	char m_szMsgType;
	CString m_strLpPort;
	CString m_strLpChannel;
	CString m_strIOFlag;   
	int     msgid;  //标识符      ：用于区别监控，通常是0xff


	BOOL m_bIsStart,m_bIsFirstWorkItemFlag;
	CString m_strClientIp;

	CHAR * m_Buffer, *m_pTmp;
	CBasicConfigInfo BasicConfigInfo;

//	CMemoryState msOld, msNew, msDif;


};

#endif // !defined(AFX_MODULEINFO_H__4896F6A8_86D5_4F58_935C_FEA38767D433__INCLUDED_)
