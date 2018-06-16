// TransactFactory.cpp: implementation of the CTransactFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataTransact.h"
#include "TransactFactory.h"

#include "GatherInfoTransact.h"
#include "CxLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTransactFactory::CTransactFactory()
{
	LOG("CTransactFactory init success\n");
}

CTransactFactory::~CTransactFactory()
{
	CBasicTransact* p=NULL;

	POSITION pos=m_ListTable.GetHeadPosition();
    while(pos != NULL)
	{
		delete m_ListTable.GetNext(pos);
	}
}

BOOL CTransactFactory::InitInfo()
{
	m_ListTable.AddHead(new CGatherInfoTransact());				//�ɼ�����
	//m_ListTable.AddHead(new CControlInfoTransact());			//��ʾΪƽ̨���ط���ָ��
	return TRUE;
}
//
BOOL CTransactFactory::InfoTransact(CBasicConfigInfo &PackInfo)
{
	try
	{
		CBasicTransact* p;
		if(PackInfo.szMsg==(char)_ChannelSndPicInfo)
			p=GetInfoType((char)_ChannelRetPicInfo);
		else
			p=GetInfoType(PackInfo.szMsg);
		if(p != NULL)
			p->OnDealData(PackInfo);
		else
		{
			LOG_ERR1("��ϢΪ0x%Xδ����",PackInfo.szMsg);
	
		//	CXMLLogfile::Instance()->Flush();
		}
	}
	catch(...)
	{
		LOG_ERR("CTransactFactory::InfoTransact����");
		return FALSE;
	}
	return TRUE;
}

CBasicTransact* CTransactFactory::GetInfoType(char szMsg)
{
	try
	{
		CBasicTransact* p=NULL;
		char type;
		
		POSITION pos=m_ListTable.GetHeadPosition();
		while(pos != NULL)
		{
			p=m_ListTable.GetNext(pos);
			type=p->OnGetConfig();
			if(type==szMsg)
				return p;
		}
	}
	catch(...)
	{
		LOG_ERR("CTransactFactory::GetInfoType����");
		return NULL;
	}

	return NULL;
}
