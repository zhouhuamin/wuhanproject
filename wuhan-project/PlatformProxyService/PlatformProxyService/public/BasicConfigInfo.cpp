// BasicConfigInfo.cpp: implementation of the CBasicConfigInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BasicConfigInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicConfigInfo::CBasicConfigInfo()
{
	hNotifyWnd    = NULL;
	pPacket       = NULL;   //������

	strDataUser   = "";     //�û���
	strDataPaw    = "";     //����
	strDataServer = "";     //������
    strMonitorServerIp = "";     
	strMTFlatRoofIp = "";     //ƽ̨IP

    nFlatRoofPort = 0;      //ƽ̨�˿�
	nMTFlatRoofPort = 0;      //ƽ̨�˿�
	nLocalPort    = 0;      //���ض˿�
	nProxyPort    = 0;      //����˿�
	nMonitorPort  = 0;      //��ض˶˿�
	nThreadNumber = 0;      //�߳���
	nSendTimeOut  = 0;      //���ͳ�ʱ�趨
	nRecvTimeOut  = 0;      //���ܳ�ʱ�趨
	
	strLpPort     = "";     //��վ����
	strChannel    = "";     //ͨ������
	strIOFlag     = "";     //�����ڱ�ʶ
	strFilePath   = "";	    //ͼƬ�ļ�·��
	strConnIP     = "";     //����IP

	nAllLength    = 0;      //���ܳ�
	szMsg         = 0;	    //��Ϣ����
	nCtrlSpeedFlag=FALSE;
	nCheckContaFlag=0;
	lprarm=NULL;
	nChnlLoginMode=0;
	m_iSaveRunDataPacket = 0;
	m_iQueryLicenseID = 0;
}

CBasicConfigInfo::~CBasicConfigInfo()
{
//    if(pPacket != NULL)
//		delete[] pPacket;
}
