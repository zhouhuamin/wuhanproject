// BasicTransact.cpp: implementation of the CBasicTransact class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataTransact.h"
#include "BasicTransact.h"
#include "DataInterviewClass.h"
#include "CxLog.h"

#include <mmsystem.h>
#include <direct.h>
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBasicTransact::CBasicTransact()
{
	
	m_Hwnd=NULL;
}

CBasicTransact::~CBasicTransact()
{
	
}

BOOL CBasicTransact::OnSetConfig(HWND  hWndDlg)
{
	m_Hwnd = hWndDlg;
		
	return TRUE;
}


CString CBasicTransact::OnGetXmlInfo(CBasicConfigInfo &PackInfo)
{
	try
	{
		//nAllLength = (4+4+10+10+1+1+ 4 + nXmlLen + nAllPicLength + 2 );
		CString strTmp = "";
		
		PackInfo.pPacket;
		if (PackInfo.nAllLength > _PACKETHEAD_LEN+_PACKET_XMLLEN_SIZE)
		{
			//XML流长度。
			char szLengthAdress[4];
			
			for(int i=0; i<4; i++)
				szLengthAdress[i] = PackInfo.pPacket[i+_PACKETHEAD_LEN];
			int *pLen = (int *)szLengthAdress;
			int nlen  = (int)*pLen;
			
			if(nlen<4096)//lzq update 2048 -->3072 2007-07-22 主要因为IC卡的单证长度太长
			{
				char * p = new char[nlen+1];
				memcpy(p, &PackInfo.pPacket[_PACKETHEAD_LEN+_PACKET_XMLLEN_SIZE], nlen);
				p[nlen] = '\0';
				strTmp = p;
				delete [] p;
			}
			return strTmp;
		}
	}catch(...)
	{
		LOG_ERR("CBasicTransact::OnGetXmlInfo出错");
		return "";
	}

	return "";
}
BOOL CBasicTransact::OnDealData(CBasicConfigInfo &PackInfo)
{
	return TRUE;
}

// 返回一个空闲的SOCKET
int CBasicTransact::GetEmptySocket(SOCKET sockArray[WSA_MAXIMUM_WAIT_EVENTS])
{
	int j=WSA_MAXIMUM_WAIT_EVENTS;
	for(int i=0;i<WSA_MAXIMUM_WAIT_EVENTS;i++)
	{
		if(sockArray[i]==INVALID_SOCKET)
		{
			return i;
		}
	}

	return -1;
}

//连接
BOOL CBasicTransact::OnSetupConnect(SOCKET &clientSocket, CString strIP, int nPort, int nSendTimeout, int nRecvTimeout,int nReConnectCount) 
{
//	sockaddr_in addr;	
	sockaddr_in ServAddr;

/*	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = 0;*/
	int nError=0;

	try
	{
		closesocket(clientSocket);
		clientSocket=INVALID_SOCKET;
		clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
		if(clientSocket==INVALID_SOCKET)
		{
			Sleep(1);
			closesocket(clientSocket);
			clientSocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);	
			if(clientSocket==INVALID_SOCKET)
			{
//				CXMLLogfile::Instance()->WriteXmlLog("调试信息---连接","……连接:%s  失败（创建套接字失败），错误号:%d",strIP,GetLastError());
				return FALSE;
			}
			
		}
		int nTimeOut=1;
		setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&nTimeOut, sizeof(nTimeOut)); 

	
	/*	
		::setsockopt(
				clientSocket, 
				SOL_SOCKET,SO_RCVTIMEO,
				(char*)&nRecvTimeout,
				sizeof(nRecvTimeout));

		::setsockopt(
				clientSocket, 
				SOL_SOCKET,SO_SNDTIMEO,
				(char*)&nSendTimeout,
				sizeof(nSendTimeout));*/
		
		if (clientSocket != INVALID_SOCKET)	
		{ 
			ServAddr.sin_family = AF_INET;
			ServAddr.sin_addr.s_addr = inet_addr(strIP);
			ServAddr.sin_port = htons(nPort);
			if (connect(clientSocket,(LPSOCKADDR)&ServAddr,sizeof(ServAddr))
				== SOCKET_ERROR)
			{
				nError=WSAGetLastError();
				LOG_ERR3("…………连接:%s  失败，错误号:%d,重新连接次数%d\n",strIP,nError,nReConnectCount);

//				if(nError!=10048)
//					CXMLLogfile::Instance()->WriteXmlLog("调试信息---连接","…………连接:%s  失败，错误号:%d",strIP,nError);

				closesocket(clientSocket);
				clientSocket= INVALID_SOCKET;
				if(nError==10048&&nReConnectCount<100)
				{
					nReConnectCount++;
			//		Sleep(1);
					if(nReConnectCount>5)
						Sleep(1000);
					else
						Sleep(100);
					if(OnSetupConnect(clientSocket,strIP,nPort,nSendTimeout,nRecvTimeout,nReConnectCount))
						return TRUE;
				}
				if(nReConnectCount<1000&&nError==10038)
				{
					nReConnectCount++;
					Sleep(1);
					if(OnSetupConnect(clientSocket,strIP,nPort,nSendTimeout,nRecvTimeout,nReConnectCount))
						return TRUE;
				}
				return FALSE;
			}
			else
			{	
			//	BOOL bDontLinger = FALSE; 
				linger m_sLinger;
				m_sLinger.l_onoff = 1;  // (在closesocket()调用,但是还有数据没发送完毕的时候容许逗留)
				m_sLinger.l_linger = 0; // (容许逗留的时间为0秒)


				setsockopt(clientSocket,SOL_SOCKET,SO_DONTLINGER,(const char*)&m_sLinger,sizeof(linger));
		//		Sleep(5);
				return TRUE;
			}	
		}
	}catch(...)
	{
		LOG_ERR("CBasicTransact::OnSetupConnect出错");
		return FALSE;
	}
	return FALSE;
}

BOOL CBasicTransact::OnBasicDealInfo(char *pData, CString strIp, int nPort, int nLength, CBasicConfigInfo &PackInfo)
{
	SOCKET newSocket;

	if (OnSetupConnect(newSocket, strIp, nPort, PackInfo.nSendTimeOut, PackInfo.nRecvTimeOut))
	{
		Sleep(5);
		int nLen = send(newSocket, pData, nLength, 0);
		if(nLen<0)
		{
			shutdown(newSocket, SD_BOTH);
			closesocket(newSocket);	
			Sleep(200);

			SOCKET newSocket1;
			
			if (OnSetupConnect(newSocket1, strIp, nPort, PackInfo.nSendTimeOut, PackInfo.nRecvTimeOut))
			{
				Sleep(5);
				nLen = send(newSocket1, pData, nLength, 0);
				if(nLen<0)
				{
					shutdown(newSocket1, SD_BOTH);
					closesocket(newSocket1);	
					return FALSE;
				}
				shutdown(newSocket1, SD_BOTH);
				closesocket(newSocket1);
				return TRUE;
				
			}
			else
				return FALSE;
		}
		shutdown(newSocket, SD_BOTH);
		closesocket(newSocket);
		
	    return TRUE;
	}

	return FALSE;
}

CString CBasicTransact::BuildXmlValue(CString strTable, CString strValue)
{
	CString strTmp = "";

	if (strTable.CompareNoCase("") != 0)
	{
		strTmp  = "<" + strTable;
		strTmp += ">" + strValue;
		strTmp += "</" + strTable;
		strTmp += ">";
	}

	return strTmp;

}

_bstr_t CBasicTransact::GetRecordsetInfo(CString strField, _RecordsetPtr RdsetPtr)
{
	long lDataSize;

	try
	{	
		lDataSize = RdsetPtr->GetFields()->GetItem((_variant_t)strField)->ActualSize;
		if(lDataSize >0) 
		{
			_bstr_t strTmp = (_bstr_t)RdsetPtr->Fields->GetItem((_variant_t)strField)->Value;
			return strTmp;
		}
	}catch(...)
	{
		//CXMLLogfile::Instance()->WriteXmlLog("异常",
		//	"CBasicTransact::GetRecordsetInfo出错");
		return "";
	}

	return "";
}


char * CBasicTransact::GetRecordsetBlobInfo(CString strField,_RecordsetPtr RdsetPtr,int *pLen)
{
	long lDataSize;
	char *pBuf;
		
	try
	{	
		_variant_t varBLOB;
		*pLen=lDataSize = RdsetPtr->GetFields()->GetItem((_variant_t)strField)->ActualSize;
		
		if(lDataSize >0) 
		{
			varBLOB = (_bstr_t)RdsetPtr->Fields->GetItem((_variant_t)strField)->GetChunk(lDataSize);
			if(varBLOB.vt==(VT_ARRAY | VT_UI1))
			{
				SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);
				SafeArrayUnaccessData(varBLOB.parray);
				return pBuf;
			}
			
		}
	}catch(...)
	{
		return "";
	}
	
	return "";	
}

void CBasicTransact::ToWriteFile(CString strFileName, BYTE *pbyBuff, int nLen)
{
	//time_t timenow;
	//time(&timenow);
	SYSTEMTIME   t; 
	GetLocalTime(&t);
	CString strDateTime;
	strDateTime.Format("%04d%02d%02d", t.wYear,t.wMonth, t.wDay);
	CString filename,sPath;
	GetModuleFileName(NULL,sPath.GetBufferSetLength (MAX_PATH+1),MAX_PATH);
	sPath.ReleaseBuffer();
	int nPos;
	nPos=sPath.ReverseFind('\\');
	sPath=sPath.Left(nPos);

	//	filename.Format("\\LoginLog\\regxml%d.xml",timenow);
	filename=sPath+"\\RunLog\\" + strDateTime + "\\" + strFileName;
	_mkdir(sPath+"\\RunLog");
	_mkdir(sPath+"\\RunLog\\" + strDateTime);

	CFile file;
	if(nLen<=0)
		return ;
	if(file.Open(filename, CFile::modeCreate | CFile::modeWrite, NULL))
	{
		file.Write(pbyBuff, nLen);
		file.Close();
	}
}
