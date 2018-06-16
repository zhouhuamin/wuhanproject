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
	pPacket       = NULL;   //包内容

	strDataUser   = "";     //用户名
	strDataPaw    = "";     //密码
	strDataServer = "";     //服务器
    strMonitorServerIp = "";     
	strMTFlatRoofIp = "";     //平台IP

    nFlatRoofPort = 0;      //平台端口
	nMTFlatRoofPort = 0;      //平台端口
	nLocalPort    = 0;      //本地端口
	nProxyPort    = 0;      //代理端口
	nMonitorPort  = 0;      //监控端端口
	nThreadNumber = 0;      //线程数
	nSendTimeOut  = 0;      //发送超时设定
	nRecvTimeOut  = 0;      //接受超时设定
	
	strLpPort     = "";     //场站名称
	strChannel    = "";     //通道名称
	strIOFlag     = "";     //进出口标识
	strFilePath   = "";	    //图片文件路径
	strConnIP     = "";     //连接IP

	nAllLength    = 0;      //包总长
	szMsg         = 0;	    //消息类型
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
