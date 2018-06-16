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
	int nMsgType;	//��������
	int nSendSize;	//���Ͱ��Ĵ�С
	int nFlag;		//���Ͱ��ı�־��0Ϊ��һ������2Ϊ�ڶ�����
	int ntimestamp;	//���Ͱ���ʱ��� ����Ǽ�������ͬ����ʱ����Ļ���������һ�����ķְ�
};
class CBasicConfigInfo : public CObject  
{
public:
	CBasicConfigInfo();
	virtual ~CBasicConfigInfo();

public:
	HWND hNotifyWnd;
	
public:	
	////���ݿ�������Ϣ
	CString strDataUser;    //�û���
	CString strDataPaw;     //����
	CString strDataServer;  //������
		
	////����������Ϣ
    CString strMonitorServerIp;  //���м������IP
    int nFlatRoofPort;      //ƽ̨�˿�
	int nLocalPort;         //���ض˿�
	int nProxyPort;         //����˿�
	int nMonitorPort;       //��ض˶˿�
	BOOL nCtrlSpeedFlag;    //�������Ʊ�ǣ�0������Ҫ��1��Ҫ

	int nChnlLoginMode;		//ͨ����֤��ʽ

	int nUserLoginMode;		//��ض���֤��ʽ  0 ������֤ 1 ��̨��֤  
	////������������Ϣ
	int  nThreadNumber;     //�߳���
	int  nSendTimeOut;      //���ͳ�ʱ�趨
	int  nRecvTimeOut;      //���ܳ�ʱ�趨

	int  nIsNeedActiveTest;  //�Ƿ���Ҫ�������
	int  nCheckLoginOutTime;  //��ʱ�������״̬��ʱ��
	int  nLoginOutTime;		 //�ж���ʱ����ʱ��
	
	////��վ������Ϣ
	CString strLpPort;      //��վ����
	CString strChannel;     //ͨ������
	CString strIOFlag;      //�����ڱ�ʶ
	CString strFilePath;	//ͼƬ�ļ�·��

public:
	int     nAllLength;     //���ܳ�
	char    szMsg;	        //��Ϣ����
	CString strConnIP;      //����IP

	char    *pPacket;       //������

	int     msgid;          //��ʶ
	
	int     nCheckContaFlag; //���ʶ�����־��0��Ϊ����飬1Ϊ���
	int		nESealFlag;		 //����ʩ����Ƿ��ڼ��м������ 0  �� 1 ��
	int		nESealInfoDownFlag;//����Ҫ���ع�����Ϣ  0 �� 1 ��
	int     nESealTime;		//����ʱ���� (��)	
	LPVOID    lprarm;
public:
	BYTE m_iSaveRunDataPacket;
public:
	BYTE m_iQueryLicenseID;
	CString strMTFlatRoofIp;  //��ͷƽ̨IP
	int nMTFlatRoofPort;      //��ͷƽ̨�˿�	
};




#endif // !defined(AFX_BASICCONFIGINFO_H__6B699BB8_9581_4C19_B188_80DC1F0761DE__INCLUDED_)
