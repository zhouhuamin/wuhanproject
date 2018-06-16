// BasicConfigInfo.h: interface for the CBasicConfigInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASICCONFIGINFO_H__6B699BB8_9581_4C19_B188_80DC1F0761DE__INCLUDED_)
#define AFX_BASICCONFIGINFO_H__6B699BB8_9581_4C19_B188_80DC1F0761DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "trancemsgdef.h"
struct _udpHead {
	int nMsgType;	//包特征码
	int nSendSize;	//发送包的大小
	int nFlag;		//发送包的标志，0为第一个包，2为第二个包
	int ntimestamp;	//发送包的时间戳 如果是几个包是同样的时间戳的话，表明是一个包的分包
};
class CBasicConfigInfo : public CObject  
{
public:
	CBasicConfigInfo();
	virtual ~CBasicConfigInfo();

public:
	HWND hNotifyWnd;
	
public:	
	////数据库配置信息
	CString strDataUser;    //用户名
	CString strDataPaw;     //密码
	CString strDataServer;  //服务器
		
	////网络配置信息
    CString strMonitorServerIp;  //集中监控中心IP
    int nFlatRoofPort;      //平台端口
	int nLocalPort;         //本地端口
	int nProxyPort;         //代理端口
	int nMonitorPort;       //监控端端口
	BOOL nCtrlSpeedFlag;    //流量控制标记，0，不需要，1需要

	int nChnlLoginMode;		//通道验证方式

	int nUserLoginMode;		//监控端验证方式  0 本地验证 1 后台验证  
	////服务器配置信息
	int  nThreadNumber;     //线程数
	int  nSendTimeOut;      //发送超时设定
	int  nRecvTimeOut;      //接受超时设定

	int  nIsNeedActiveTest;  //是否需要心跳检测
	int  nCheckLoginOutTime;  //定时检查连接状态的时间
	int  nLoginOutTime;		 //判定超时掉线时间
	
	////场站配置信息
	CString strLpPort;      //场站名称
	CString strChannel;     //通道名称
	CString strIOFlag;      //进出口标识
	CString strFilePath;	//图片文件路径

public:
	int     nAllLength;     //包总长
	char    szMsg;	        //消息类型
	CString strConnIP;      //连接IP

	char    *pPacket;       //包内容

	int     msgid;          //标识
	
	int     nCheckContaFlag; //箱号识别检查标志，0，为不检查，1为检查
	int		nESealFlag;		 //关锁施解封是否在集中监控中心 0  是 1 否
	int		nESealInfoDownFlag;//否需要下载关锁信息  0 否 1 是
	int     nESealTime;		//下载时间间隔 (秒)	
	LPVOID    lprarm;
public:
	BYTE m_iSaveRunDataPacket;
public:
	BYTE m_iQueryLicenseID;
	CString strMTFlatRoofIp;  //码头平台IP
	int nMTFlatRoofPort;      //码头平台端口	
};




#endif // !defined(AFX_BASICCONFIGINFO_H__6B699BB8_9581_4C19_B188_80DC1F0761DE__INCLUDED_)
