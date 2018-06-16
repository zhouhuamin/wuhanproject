// ModuleInfo.cpp: implementation of the CModuleInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModuleInfo.h"
#include "trancemsgdef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern  "C"  _declspec(dllimport) int InfoDispose(CBasicConfigInfo &PackInfo);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern BOOL FisrstWorkFlags[255];
CModuleInfo::CModuleInfo()
{
	m_nAllLength  = 0;
	m_nCurLength  = 0;
	m_nClientPort = 0;
	
	m_strClientIp = "";	
	m_Buffer      = NULL;
    m_bIsStart    = FALSE;	
	msgid=0xffffffff;
	m_bIsFirstWorkItemFlag=FALSE;
}

CModuleInfo::~CModuleInfo()
{
//	msNew.Checkpoint();                //->设置第二个内存检查点
//    msDif.Difference( msOld, msNew );  //->检查变化
//    msDif.DumpStatistics();            //->Dump变化

	try
	{
	   if(m_Buffer != NULL)
	   {
		  delete[] m_Buffer;
		  m_Buffer=NULL;
	   }

	  
//	   _ASSERTE(_CrtCheckMemory());

	}catch(...)
	{
	}
}

BOOL CModuleInfo::Realse()
{
	try
	{
       if(m_Buffer != NULL)
		  delete[] m_Buffer;

	   m_Buffer = NULL;
	}catch(...)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CModuleInfo::OnIsExist(char * strIp, int nPort)
{
	try
	{
	   if((strcmp(strIp,m_strClientIp) == 0) && (m_nClientPort == nPort))
	       return TRUE;

	}catch(...)
	{
		return FALSE;
	}

	return FALSE;
}

LONG CModuleInfo::OnDealInfo(LPPER_IO_OPERATION_DATA PerIoData, CBasicConfigInfo &ConfigInfo)
{
	int i = PerIoData->DataBuf.len;

	try
	{
		if(PerIoData->bIsStart)
		{
			m_strClientIp  = PerIoData->szClientIp;
			m_nClientPort  = PerIoData->nClientPort;
			PerIoData->bIsStart = FALSE;
			
			if(!UnchainPackHead(PerIoData->szBuffer))
				return FALSE;
			int j = m_nAllLength;
			
			if(m_Buffer != NULL)
			{
		          delete[] m_Buffer;
				  m_Buffer = NULL;
			}

			// 会不会同时收到两个完整的包呢？ 20101223-周华民
			// zhouhm 2011-1-8 add explain
			//if (j < i)
			//{
			//	m_Buffer= new CHAR[i+1];
			//	memset(m_Buffer, 0, i+1);
			//}
			//else
			{
				m_Buffer = new CHAR[j+1];
				memset(m_Buffer, 0, j+1);
			}
			m_pTmp = m_Buffer;			
			if (m_nAllLength >= i)
			{
				memcpy(m_Buffer, PerIoData->szBuffer, i);
				m_pTmp += i;
				m_nCurLength +=i;
			}
			else
			{
				memcpy(m_Buffer, PerIoData->szBuffer, m_nAllLength);
				m_pTmp += m_nAllLength;
				m_nCurLength = m_nAllLength;
			}
		}
		else
		{
			if (m_pTmp < m_Buffer + m_nAllLength && m_pTmp + i <= m_Buffer + m_nAllLength)
			{
				memcpy(m_pTmp, PerIoData->szBuffer, i);
				m_pTmp += i;			
				m_nCurLength +=i;
			}
			else
			{
				int iDiff = m_nAllLength - m_nCurLength;
				if (iDiff > 0 && iDiff <= i && m_pTmp + iDiff <= m_Buffer + m_nAllLength)
				{
					memcpy(m_pTmp, PerIoData->szBuffer, iDiff);
					m_pTmp += iDiff;			
					m_nCurLength += iDiff;
				}
				else if (iDiff > 0 && iDiff > i && m_pTmp + i <= m_Buffer + m_nAllLength)
				{
					memcpy(m_pTmp, PerIoData->szBuffer, i);
					m_pTmp += i;			
					m_nCurLength += i;
				}
			}

		}
		
		if((m_nAllLength <= m_nCurLength) && (m_nAllLength != 0))
		{
			// process heap 溢出  zhouhm 2011-1-8
			if (m_nAllLength < m_nCurLength)
			{
				m_nCurLength = m_nAllLength;
			}
			m_bIsStart    = TRUE;	

			BasicConfigInfo.hNotifyWnd    = ConfigInfo.hNotifyWnd;
			BasicConfigInfo.nFlatRoofPort = ConfigInfo.nFlatRoofPort;
			BasicConfigInfo.nMTFlatRoofPort = ConfigInfo.nMTFlatRoofPort;
			BasicConfigInfo.nMonitorPort  = ConfigInfo.nMonitorPort;
			BasicConfigInfo.nProxyPort    = ConfigInfo.nProxyPort;
			BasicConfigInfo.nRecvTimeOut  = ConfigInfo.nRecvTimeOut;
			BasicConfigInfo.nSendTimeOut  = ConfigInfo.nSendTimeOut;
			BasicConfigInfo.strMonitorServerIp = ConfigInfo.strMonitorServerIp;
			BasicConfigInfo.strMTFlatRoofIp = ConfigInfo.strMTFlatRoofIp;
			
			BasicConfigInfo.strDataPaw    = ConfigInfo.strDataPaw;
			BasicConfigInfo.strDataServer = ConfigInfo.strDataServer;
			BasicConfigInfo.strDataUser   = ConfigInfo.strDataUser;

			BasicConfigInfo.nChnlLoginMode=ConfigInfo.nChnlLoginMode;
			BasicConfigInfo.nUserLoginMode=ConfigInfo.nUserLoginMode;
			BasicConfigInfo.nESealFlag=ConfigInfo.nESealFlag;
			BasicConfigInfo.nESealTime=ConfigInfo.nESealTime;
			BasicConfigInfo.nESealInfoDownFlag=ConfigInfo.nESealInfoDownFlag;

			//BasicConfigInfo.pPacket=new CHAR[m_nAllLength];
			//memcpy(BasicConfigInfo.pPacket, m_Buffer,m_nAllLength);
			//delete m_Buffer;
			//m_Buffer=NULL;
			BasicConfigInfo.pPacket       = m_Buffer;
			BasicConfigInfo.nAllLength    = m_nAllLength;
			BasicConfigInfo.strChannel    = m_strLpChannel;
			BasicConfigInfo.strConnIP     = m_strClientIp;
			BasicConfigInfo.strLpPort     = m_strLpPort;
			BasicConfigInfo.strIOFlag     = m_strIOFlag;
			BasicConfigInfo.szMsg         = m_szMsgType;
			BasicConfigInfo.msgid         = msgid;
			BasicConfigInfo.strFilePath   =ConfigInfo.strFilePath;

			BasicConfigInfo.nIsNeedActiveTest=ConfigInfo.nIsNeedActiveTest;
			BasicConfigInfo.nCheckLoginOutTime=ConfigInfo.nCheckLoginOutTime;
			BasicConfigInfo.nLoginOutTime=ConfigInfo.nLoginOutTime;
			if(FisrstWorkFlags[BasicConfigInfo.szMsg]==TRUE)
				m_bIsFirstWorkItemFlag=TRUE;
			else
				m_bIsFirstWorkItemFlag=FALSE;
			BasicConfigInfo.nCheckContaFlag=ConfigInfo.nCheckContaFlag;
			BasicConfigInfo.lprarm=BasicConfigInfo.lprarm;
			BasicConfigInfo.m_iSaveRunDataPacket = ConfigInfo.m_iSaveRunDataPacket;
			BasicConfigInfo.m_iQueryLicenseID = ConfigInfo.m_iQueryLicenseID;
			
//			InfoDispose(BasicConfigInfo);
			//OnGetXmlInfo(m_Buffer);
/*		CFile file;
		file.Open("hcontrol.xml",CFile::modeCreate | CFile::modeWrite,NULL);
		file.WriteHuge(m_Buffer, m_nAllLength);
		file.Close();
*/			DWORD id = GetCurrentThreadId();
//			TRACE("^^^^^^^^^^^^^^线程%x处理结束^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n", id);
		}
	}catch(...)
	{
		return FALSE;
	}
	
	return TRUE;
}

LONG CModuleInfo::OnDealInfo()
{
//	msOld.Checkpoint();//->设置第一个内存检查点
//    msOld.DumpAllObjectsSince();

	try
	{	
		InfoDispose(BasicConfigInfo);
      //  TRACE("$$$$$$$$$$$$$$$工作线程处理....^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n");
		m_bIsStart    = FALSE;	
	}catch(...)
	{
		return FALSE;
	}
	
	return TRUE;
}

BOOL CModuleInfo::GetStart()
{
	return m_bIsStart;
}

BOOL CModuleInfo::UnchainPackHead(char *pPacket)
{
	//总长 = (4+4+10+10+1+1+ 4 + nXmlLen + nAllPicLength + 2 );
	try
	{
		int iLen=0;
		char szTmp[4];
		char *pszPacket;

		pszPacket = pPacket;
	    memcpy(szTmp, pszPacket, _PACKETHEAD_FINDFLAG_SIZE);
		iLen=_PACKETHEAD_FINDFLAG_SIZE;
		if (ComparePackHead(szTmp))
		{
			
		
			memcpy(szTmp, (pszPacket+iLen), _PACKETHEAD_LEN_SIZE);	
			iLen=iLen+_PACKETHEAD_LEN_SIZE;
			//总长
			int *p = (int *)szTmp;
			m_nAllLength = (int)*p;
			//消息类型
			m_szMsgType = pszPacket[iLen];
			iLen=iLen+_PACKETHEAD_MSGTYPE_SIZE;
			//场站号
			for(int i=iLen; i<iLen+_PACKETHEAD_AREARID_SIZE; i++)
				m_strLpPort += pszPacket[i];
			iLen=iLen+_PACKETHEAD_AREARID_SIZE;
	
			//通道号
			for(int i=iLen; i<iLen+_PACKETHEAD_CHANNEL_SIZE; i++)
				m_strLpChannel += pszPacket[i];
			iLen=iLen+_PACKETHEAD_CHANNEL_SIZE;
	
			//进出标志
			m_strIOFlag = pszPacket[iLen];
			iLen=iLen+_PACKETHEAD_IETYPE_SIZE;

			//标识符
			memcpy(&msgid,pszPacket+iLen,_PACKETHEAD_IDFLAG_SIZE);
			
			return TRUE;
		}
	}catch(...)
	{
		return FALSE;
	}
	
	return FALSE;
}

BOOL CModuleInfo::ComparePackHead(char szBuf[4])
{
	try
	{
		const char PackageRoofHead[4]={(char)0xE2,(char)0x5C,(char)0x4B,(char)0x89};
		const char PackageTail[2]={(char)0xFF,(char)0xFF};
		
		if ((szBuf[0] == PackageRoofHead[0]) && (szBuf[1] == PackageRoofHead[1]) &&
			(szBuf[2] == PackageRoofHead[2]) && (szBuf[3] == PackageRoofHead[3]))
		{
			return TRUE;
		}
		
	}catch(...)
	{
		return FALSE;
	}
	return FALSE;
}

CString CModuleInfo::OnGetXmlInfo(char* szPacket)
{
	try
	{
		//nAllLength = (4+4+1+10+10+1+4+ 4 + nXmlLen + nAllPicLength + 2 );
		CString strTmp = "";
		
		if (m_nAllLength > 38)
		{
			//XML流长度。
			char szLengthAdress[4];
			
			szPacket+=30;
			for(int i=0; i<4; i++)
			{
				szLengthAdress[i] = *szPacket;
				szPacket+=1;
			}
			int *pLen = (int *)szLengthAdress;
			int nlen  = (int)*pLen;
			
			if(nlen<4096)//lzq update 2048 -->3072 2007-07-22 主要因为IC卡的单证长度太长
			{
				char * p = new char[nlen+1];
				memcpy(p, szPacket, nlen);
				p[nlen] = '\0';
				strTmp = p;
				delete []p;
			}

/*			CFile file;
			file.Open("c:\\tmp.xml", CFile::modeCreate | CFile::modeWrite, NULL);
			file.WriteHuge(strTmp, strTmp.GetLength());
			file.Close();
*/
			return strTmp;
		}
	}catch(...)
	{
		return "";
	}
	
	return "";
}

BOOL CModuleInfo::GetIsFirstWorkItem()
{
	return m_bIsFirstWorkItemFlag;
}
