// GatherInfoTransact.h: interface for the CGatherInfoTransact class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GATHERINFOTRANSACT_H__9C784256_B092_4A9C_8A5A_E2435F0638E5__INCLUDED_)
#define AFX_GATHERINFOTRANSACT_H__9C784256_B092_4A9C_8A5A_E2435F0638E5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicTransact.h"
#include "SPStruct.h"

//采集数据
class CGatherInfoTransact : public CBasicTransact  
{
public:
	CGatherInfoTransact();
	virtual ~CGatherInfoTransact();

public:
	virtual char OnGetConfig()
	{
		//表示为采集数据传输
		return (char)_ChannelGatherInfo;
	}

    virtual BOOL OnDealData(CBasicConfigInfo &PackInfo);

private:
	BOOL OnParse(CBasicConfigInfo &PackInfo);
	BOOL OnSaveInfo(CBasicConfigInfo &PackInfo);
	BOOL OnGetRelInfo(CBasicConfigInfo &PackInfo, CStringArray &messageIDArray, CStringArray &contaArray);
	BOOL OnSendRelToClient(CBasicConfigInfo &PackInfo, CStringArray &messageIDArray, CStringArray &contaArray);
	BOOL OnUpdateDB(CBasicConfigInfo &PackInfo, CStringArray &messageIDArray, CStringArray &contaArray);
	BOOL OnGetRelInfo(CBasicConfigInfo &PackInfo, CStringArray &messageIDArray, CStringArray &contaArray, CString &strCheckResult, CString &strOpHint);
	BOOL OnUpdateRelInfo(CBasicConfigInfo &PackInfo);
private:
	_CHNL_GATHER_DATA	m_ChnlGatherData;
	_structCommandInfo	m_CommandInfo;

};

#endif // !defined(AFX_GATHERINFOTRANSACT_H__9C784256_B092_4A9C_8A5A_E2435F0638E5__INCLUDED_)
