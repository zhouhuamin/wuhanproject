// MSMQApiWrapper.cpp: implementation of the CMSMQApiWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "MSMQApiWrapper.h"
#include "../CxLog.h"
#include "../DealJob.h"
extern CDealJob g_job;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//void VerifyZip(HZIP hz, LPCTSTR lpszFile);

CMSMQApiWrapper::CMSMQApiWrapper()
{
	//Initialise OLE Support
	OleInitialize(NULL); 
}

CMSMQApiWrapper::~CMSMQApiWrapper()
{

}

//	Creating A Queue [Public/Private]
	
int CMSMQApiWrapper::CreateQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel,BOOL bPublic)
{
	try
	{
		IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
		CString szQueueName,szQueueLabel;
		szQueueLabel = pszQueueLabel;
		szQueueName = pszPathQueue;
		
		if(szQueueName.Find("\\") == -1)
		{
			//User Has not provided complete path so let
			//us do it for him but on the local machine
			//independent client
			if(bPublic)
				szQueueName.Format(".\\%s",pszPathQueue);
			else
				szQueueName.Format(".\\PRIVATE$\\%s",pszPathQueue);
		}		
		qinfo->PathName = szQueueName.AllocSysString();
		qinfo->Label = szQueueLabel.AllocSysString();
		try
		{
			qinfo->Create();
		}
		catch (_com_error comerr)
		{
			HRESULT hr = comerr.Error();
			if(hr == MQ_ERROR_QUEUE_EXISTS)
			{
				return MSMQ_QUEUEEXISTS;
			}
			else
			{
				throw comerr;
			}
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr)
	{
	    return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::CreatePublicQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel)
{
	return CreateQueue(pszPathQueue,pszQueueLabel,TRUE);
}

int CMSMQApiWrapper::CreatePrivateQueue(LPCTSTR pszPathQueue,LPCTSTR pszQueueLabel)
{
	return CreateQueue(pszPathQueue,pszQueueLabel,FALSE);
}

//	Deleting A Queue 

int CMSMQApiWrapper::DeletePublicQueue(LPCTSTR pszQueueLabel)
{
	IMSMQQueueInfoPtr qinfo = NULL;
	int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
	if(iResult == MSMQ_QUEUEEXISTS)
	{
		try
		{
			qinfo->Delete();
		}
		catch (_com_error comerr)
		{
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	else 
		return iResult;
	return MSMQ_FUNCTIONFAILURE;
}

//	Locating A Public Queue By Label

int CMSMQApiWrapper::LocatePublicQueue(LPCTSTR pszQueueLabel,IMSMQQueueInfoPtr &qinfo)
{
	try
	{
		IMSMQQueryPtr query("MSMQ.MSMQQuery");
		IMSMQQueueInfosPtr qinfos;

		_variant_t vtLabel;
		CString szQueueLabel = pszQueueLabel;
		
		vtLabel = szQueueLabel.AllocSysString();


		////////////////////////////////////////////////////////////////////
		// Run query to retrieve MSMQQueueInfos object.
		////////////////////////////////////////////////////////////////////
		qinfos = query->LookupQueue(&vtMissing, &vtMissing, &vtLabel);

		////////////////////////////////////////////////////////////////////
		// Retrieve first MSMQQueueInfo object and exit if no queues found.
		////////////////////////////////////////////////////////////////////

		qinfo = qinfos->Next();

		if (qinfos == NULL)
		{
		   return MSMQ_NOQUEUEEXISTS;
		}

		return MSMQ_QUEUEEXISTS;
	}
	catch (_com_error comerr)
	{
	    return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

//	Synchronously Purging A Queue

int CMSMQApiWrapper::SynchronousPurgePublicQueue(LPCTSTR pszQueueLabel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult; 

		_variant_t vtReceiveTimeout;
		vtReceiveTimeout = (long)1000;

		IMSMQQueuePtr qRec;
		IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
		////////////////////////////////////////////////////////////
		// Open queue to retrieve message.
		////////////////////////////////////////////////////////////
		qRec = qinfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);


		////////////////////////////////////////////////////////////
		// Retrieve messages from queue.
		////////////////////////////////////////////////////////////
		msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
		if (msgRec == NULL)
		{
			return MSMQ_NOMESSAGESINQUEUE;
		}

		while (msgRec != NULL)
		{
			//Receive The messages thereby removing it
			msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
		}
		////////////////////////////////////////////////////////////
		// Close queue.
		////////////////////////////////////////////////////////////
		qRec->Close();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

//	Retrieving and Setting Queue Properties

int CMSMQApiWrapper::RetrieveAuthenticationLevelOfQueue(LPCTSTR pszQueueLabel,int &iAuthenticationLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iAuthenticationLevel = qinfo->Authenticate;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetAuthenticationLevelOfQueue(LPCTSTR pszQueueLabel,int iAuthenticationLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->Authenticate = iAuthenticationLevel;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrievePriorityLevelOfQueue(LPCTSTR pszQueueLabel,int &iPriorityLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iPriorityLevel = qinfo->BasePriority;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetPriorityLevelOfQueue(LPCTSTR pszQueueLabel,int iPriorityLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->BasePriority = iPriorityLevel;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveFormatNameOfQueue(LPCTSTR pszQueueLabel,CString &szFormatName)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		BSTR FAR FormatName = qinfo->FormatName;
		szFormatName.SetSysString(&FormatName);
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveTransactionLevelOfQueue(LPCTSTR pszQueueLabel,int &iTransactionLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iTransactionLevel = qinfo->IsTransactional;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveReadLevelOfQueue(LPCTSTR pszQueueLabel,int &iReadLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iReadLevel = qinfo->IsWorldReadable;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveJournalingLevelOfQueue(LPCTSTR pszQueueLabel,int &iJournalingLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iJournalingLevel = qinfo->Journal;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetJournalingLevelOfQueue(LPCTSTR pszQueueLabel,int iJournalingLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->Journal = iJournalingLevel;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveMaximumSizeOfQueueJournal(LPCTSTR pszQueueLabel,long &lMaximumSize)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		lMaximumSize = qinfo->JournalQuota;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetMaximumSizeOfQueueJournal(LPCTSTR pszQueueLabel,long lMaximumSize)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->JournalQuota = lMaximumSize;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveLabelOfQueue(LPCTSTR pszQueuePath,CString &szLabel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo("MSMQ.MSMQQueueInfo");
		try
		{
			qinfo->PathName = pszQueuePath;
			qinfo->Create();
		}
		catch (_com_error comerr)
		{
			HRESULT hr = comerr.Error();
			if(hr == MQ_ERROR_QUEUE_EXISTS)
			{
				szLabel = (LPSTR)qinfo->Label;
				return MSMQ_FUNCTIONSUCCESS;
			}
		}
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetLabelOfQueue(LPCTSTR pszQueueLabel,CString szLabel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		COleVariant vtLabel;
		vtLabel.SetString(szLabel,VT_BSTR);
		qinfo->Label = ((LPVARIANT)vtLabel)->bstrVal;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrievePrivacyLevelOfQueue(LPCTSTR pszQueueLabel,int &iPrivacyLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		iPrivacyLevel = qinfo->PrivLevel;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetPrivacyLevelOfQueue(LPCTSTR pszQueueLabel,int iPrivacyLevel)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->PrivLevel = iPrivacyLevel;
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrieveMaximumSizeOfQueue(LPCTSTR pszQueueLabel,long &lMaximumSize)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		lMaximumSize = qinfo->Quota;//-1 indicates infinity
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SetMaximumSizeOfQueue(LPCTSTR pszQueueLabel,long lMaximumSize)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		qinfo->Quota = lMaximumSize;//-1 indicates infinity
		qinfo->Update();
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::RetrievePathNameOfQueue(LPCTSTR pszQueueLabel,CString &szPathName)
{
	try
	{
		IMSMQQueueInfoPtr qinfo = NULL;
		int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
		if(iResult != MSMQ_QUEUEEXISTS)
			return iResult;
		qinfo->Refresh();
		szPathName = (LPSTR)qinfo->PathName;
		return MSMQ_FUNCTIONSUCCESS;
	}
	catch (_com_error comerr) 
	{
		return MSMQ_FUNCTIONFAILURE;
	}
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SendStringMessage(LPCTSTR pszQueueLabel,CString szLabel,CString szMessage)
{
	IMSMQQueueInfoPtr qinfo = NULL;
	int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
	if(iResult == MSMQ_QUEUEEXISTS)
	{
		try
		{
			IMSMQQueuePtr qSend;
			IMSMQMessagePtr m("MSMQ.MSMQMessage");
			qSend = qinfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
			COleVariant vtMessage,vtLabel;
			vtMessage.SetString(szMessage,VT_BSTR);
			vtLabel.SetString(szLabel,VT_BSTR);
			m->Body = ((LPVARIANT)vtMessage)->bstrVal;
			m->Label = ((LPVARIANT)vtLabel)->bstrVal;
			m->Send(qSend);
      
		}
		catch (_com_error comerr)
		{
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	else 
		return iResult;
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::ReadStringMessage(LPCTSTR pszQueueLabel,CString &szLabel,CString &szMessage)
{
	CString m_ServerName = ".";
	DWORD cchBuff = 256;
	LPTSTR lpszSystemInfo = NULL;
	char tchBuffer[256] = {0};
	lpszSystemInfo = tchBuffer; 
	GetComputerName(lpszSystemInfo,&cchBuff);    //  Getting the ComputerName
	m_ServerName=lpszSystemInfo;
	m_ServerName += "\\Private$\\sanbao";
	CString fname;
	_variant_t vtReceiveTimeout;
	vtReceiveTimeout = (long)1000;
	IMSMQQueueInfoPtr qinfo = NULL;
		qinfo.CreateInstance("MSMQ.MSMQQueueInfo");

		qinfo->PathName = m_ServerName.AllocSysString();
//		qinfo->FormatName = fname.AllocSysString();
		qinfo->Label= szLabel.AllocSysString();   // Setting the Queue Label



//	qinfo->PathName=m_ServerName.AllocSysString();
	fname="DIRECT=OS:";
	fname+=m_ServerName;
	qinfo->FormatName = fname.AllocSysString();
	//int iResult = LocatePublicQueue(pszQueueLabel,qinfo);
	//if(iResult == MSMQ_QUEUEEXISTS)
	{

		IMSMQQueuePtr qRec;
		IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
    	try
		{
			qRec = qinfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);
			if (qRec == NULL)
			{
				return MSMQ_FUNCTIONFAILURE;
			}
			msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
			if (msgRec != NULL)
			{
				VARIANT vtValue;
				vtValue = msgRec->GetBody();
				if(vtValue.vt == VT_BSTR)
					szMessage = vtValue.bstrVal;
				szLabel = (LPSTR) msgRec->GetLabel();
			}
		}
		catch (_com_error comerr)
		{
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}
	//else 
	//	return MSMQ_FUNCTIONSUCCESS;
	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::ReadStreamMessage(CString strIP, LPCTSTR pszQueueName, CString strQueueLabel, CString &szLabel, BYTE **pMessage, int &iMessageLen)
{
	CString m_ServerName = ".";
	DWORD cchBuff = 256;
	LPTSTR lpszSystemInfo = NULL;
	char tchBuffer[256] = {0};
	lpszSystemInfo = tchBuffer; 
//	GetComputerName(lpszSystemInfo,&cchBuff);    //  Getting the ComputerName
//	m_ServerName = lpszSystemInfo;
	m_ServerName = strIP;
	m_ServerName += "\\Private$\\";
	m_ServerName += pszQueueName;

	CString fname;
	_variant_t vtReceiveTimeout;
//	vtReceiveTimeout = (long)1000;
	IMSMQQueueInfoPtr qinfo = NULL;
	qinfo.CreateInstance("MSMQ.MSMQQueueInfo");

	qinfo->PathName = m_ServerName.AllocSysString();
	//		qinfo->FormatName = fname.AllocSysString();
	qinfo->Label = strQueueLabel.AllocSysString();   // Setting the Queue Label



	//	qinfo->PathName=m_ServerName.AllocSysString();
	//fname="DIRECT=OS:";
	fname="DIRECT=TCP:";
	fname+=m_ServerName;
	qinfo->FormatName = fname.AllocSysString();
	{
		IMSMQQueuePtr qRec;
		IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
		// remote access 2012-08-02
		//qinfo->Refresh();
		qinfo->Journal = 0;
		//qinfo->Update();
		try
		{
			qRec = qinfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);
			msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
			if (msgRec != NULL)
			{
				// 				VARIANT vtValue;
				// 				vtValue = msgRec->GetBody();
				// 				if(vtValue.vt = VT_BSTR)
				// 					szMessage = vtValue.bstrVal;
				//UINT uiRead = SysStringByteLen(msgRec->Body.bstrVal);
				//LPBYTE pvRead = reinterpret_cast<LPBYTE>(msgRec->Body.bstrVal);
				//BYTE *pData = new BYTE[uiRead + 1];
				//memset(pData, 0x00, sizeof(BYTE) * uiRead);
				//CopyMemory((void *)pData, pvRead, uiRead);
				//pMessage = pData;

				_variant_t &vtValue = msgRec->GetBody();

				SAFEARRAY* psa = vtValue.parray;
				long lBound;				
				SafeArrayGetLBound(psa, 1, &lBound);				
				long lUp;				
				SafeArrayGetUBound(psa, 1, &lUp);				
				DWORD dwSize = lUp - lBound + 1;				
				if(dwSize <= 1)					
					return S_FALSE; 

				iMessageLen = dwSize;
				BYTE *pData = new BYTE[dwSize + 1];
				memset(pData, 0x00, sizeof(BYTE) * (dwSize + 1));	
				void * tp;				
				SafeArrayAccessData(psa, reinterpret_cast<void**>(&tp));				
				CopyMemory((LPVOID)pData, tp, dwSize);				
				SafeArrayUnaccessData(psa);				
				//CStdioFile  file;
				//file.Open("test0005.zip",CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
				//file.Write(pData, dwSize);
				//file.Close();
				*pMessage = pData;
				pData[dwSize] = '\0';

				//TCHAR *strSourceFile = "test0005.zip";
				//TCHAR *strDestFolder = "C:\\test0005.out";

				//#ifdef _UNICODE
				//				ZIPENTRYW ze;
				//#else
				//				ZIPENTRY ze; 
				//#endif
				//HZIP hz = OpenZip(strSourceFile, 0, ZIP_FILENAME);

				//if (hz)
				//{
				//	ZRESULT zr = GetZipItem(hz, -1, &ze); 

				//	if (zr == ZR_OK)
				//	{
				//		int numitems = ze.index;

				//		if (numitems == 1)
				//		{
				//			VerifyZip(hz, strDestFolder);
				//			CloseZip(hz);
				//		}
				//		else
				//		{
				//		}
				//	}
				//	else
				//	{
				//	}
				//}
				//else
				//{
				//}


				szLabel = (LPSTR) msgRec->GetLabel();
			}
			qRec->Close();
		}
		catch (_com_error comerr)
		{
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}

	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::ReadStreamMessage2(CString strFileNamePath, CString strIP, LPCTSTR pszQueueName, CString strQueueLabel, CString &szLabel, BYTE **pMessage, int &iMessageLen)
{
	CString m_ServerName = ".";
	DWORD cchBuff = 256;
	LPTSTR lpszSystemInfo = NULL;
	char tchBuffer[256] = {0};
	lpszSystemInfo = tchBuffer; 
	GetComputerName(lpszSystemInfo,&cchBuff);    //  Getting the ComputerName
	m_ServerName = lpszSystemInfo;
	// m_ServerName = strIP;
	m_ServerName += "\\Private$\\";
	m_ServerName += pszQueueName;

	CString fname;
	_variant_t vtReceiveTimeout;
	vtReceiveTimeout = (long)2000; //	vtReceiveTimeout = (long)1000;
	IMSMQQueueInfoPtr qinfo = NULL;
	qinfo.CreateInstance("MSMQ.MSMQQueueInfo");

	qinfo->PathName = m_ServerName.AllocSysString();
	//		qinfo->FormatName = fname.AllocSysString();
	qinfo->Label = strQueueLabel.AllocSysString();   // Setting the Queue Label



	//	qinfo->PathName=m_ServerName.AllocSysString();
	fname="DIRECT=OS:";
	//fname="DIRECT=TCP:";
	fname+=m_ServerName;
	qinfo->FormatName = fname.AllocSysString();
	{
		IMSMQQueuePtr qRec;
		IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
		// remote access 2012-08-02
		//qinfo->Refresh();
		qinfo->Journal = 0;
		//qinfo->Update();
		try
		{
			qRec = qinfo->Open(MQ_RECEIVE_ACCESS, MQ_DENY_NONE);
			msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
			if (msgRec == NULL)
			{
				qRec->Close();

				SysFreeString(qinfo->PathName);
				SysFreeString(qinfo->Label);
				SysFreeString(qinfo->FormatName);
				if (msgRec)
					msgRec.Release();

				if (qRec)
					qRec.Release();

				if (qinfo)
					qinfo.Release();

				return MSMQ_FUNCTIONSUCCESS;
			}


			while (msgRec != NULL)
			{
				// 				VARIANT vtValue;
				// 				vtValue = msgRec->GetBody();
				// 				if(vtValue.vt = VT_BSTR)
				// 					szMessage = vtValue.bstrVal;
				//UINT uiRead = SysStringByteLen(msgRec->Body.bstrVal);
				//LPBYTE pvRead = reinterpret_cast<LPBYTE>(msgRec->Body.bstrVal);
				//BYTE *pData = new BYTE[uiRead + 1];
				//memset(pData, 0x00, sizeof(BYTE) * uiRead);
				//CopyMemory((void *)pData, pvRead, uiRead);
				//pMessage = pData;

				_variant_t &vtValue = msgRec->GetBody();

				SAFEARRAY* psa = vtValue.parray;
				long lBound = 0;				
				SafeArrayGetLBound(psa, 1, &lBound);
				if (lBound < 0)
				{
					SysFreeString(qinfo->PathName);
					SysFreeString(qinfo->Label);
					SysFreeString(qinfo->FormatName);
					return S_FALSE; 
				}


				long lUp = 0;				
				SafeArrayGetUBound(psa, 1, &lUp);
				if (lUp < 0 || lUp - lBound < 0)
				{
					SysFreeString(qinfo->PathName);
					SysFreeString(qinfo->Label);
					SysFreeString(qinfo->FormatName);
					return S_FALSE; 
				}
				

				DWORD dwSize = lUp - lBound + 1;				
				if(dwSize <= 1)
				{
					SysFreeString(qinfo->PathName);
					SysFreeString(qinfo->Label);
					SysFreeString(qinfo->FormatName);
					return S_FALSE; 
				}

				iMessageLen = dwSize;
				BYTE *pData = new BYTE[dwSize + 1];
				memset(pData, 0x00, sizeof(BYTE) * (dwSize + 1));	
				void * tp;				
				SafeArrayAccessData(psa, reinterpret_cast<void**>(&tp));				
				CopyMemory((LPVOID)pData, tp, dwSize);				
				SafeArrayUnaccessData(psa);				
				//CStdioFile  file;
				//file.Open("test0005.zip",CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
				//file.Write(pData, dwSize);
				//file.Close();
				*pMessage = pData;
				pData[dwSize] = '\0';
				szLabel = (LPSTR) msgRec->GetLabel();

				CString strTmpFileName = g_job.GetNewFileName();
				CString strFileName = strFileNamePath;
				strFileName += "\\";
				strFileName += strTmpFileName;
				strFileName += ".xml";

				CStdioFile  file;
				file.Open((LPCTSTR)strFileName,CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
				file.Write(pData, dwSize);
				file.Close();

				delete []pData;
				pData = NULL;

				CString strTmpType = szLabel;

				//strTmpType = strTmpType.Left(strTmpType.ReverseFind('-'));
				//strTmpType.Trim();
				//strTmpType.MakeUpper();


				int index = strTmpType.Find('-');
				if (index == -1)
				{
					strTmpType.Trim();
					strTmpType.MakeUpper();
				}
				else
				{
					CString strPre = strTmpType.Left(3);
					if (strPre == "WEB")
					{
						strTmpType = strTmpType.Mid(index + 1);
						index = strTmpType.Find('-');
						if (index == -1)
						{
							strTmpType = szLabel;
							strTmpType.Trim();
							strTmpType.MakeUpper();
						}
						else
						{
							strTmpType = szLabel;
							strTmpType = strTmpType.Left(strTmpType.ReverseFind('-'));
							strTmpType.Trim();
							strTmpType.MakeUpper();
						}
					}
					else
					{
						strTmpType = strTmpType.Left(index);
					}

				}

				JobItem *pJob = new JobItem;
				pJob->m_strType = strTmpType;
				pJob->m_Data = strFileName;
				g_job.AddJob(pJob);

				msgRec = qRec->Receive(&vtMissing, &vtMissing, &vtMissing, &vtReceiveTimeout);
			}


			qRec->Close();

			SysFreeString(qinfo->PathName);
			SysFreeString(qinfo->Label);
			SysFreeString(qinfo->FormatName);
		}
		catch (_com_error comerr)
		{
			return MSMQ_FUNCTIONFAILURE;
		}

		if (msgRec)
			msgRec.Release();
		
		if (qRec)
			qRec.Release();
		
		if (qinfo)
			qinfo.Release();


		return MSMQ_FUNCTIONSUCCESS;
	}

	return MSMQ_FUNCTIONFAILURE;
}

///////////////////////////////////////////////////////////////////////////////
// VerifyZip
//void VerifyZip(HZIP hz, LPCTSTR lpszFile)
//{
//#ifdef _UNICODE
//	ZIPENTRYW ze;
//#else
//	ZIPENTRY ze; 
//#endif
//
//	memset(&ze, 0, sizeof(ze));
//	int index = -1;
//	ZRESULT zr = 0;
//
//	//zr = FindZipItem(hz, lpszFile, TRUE, &index, &ze);
//	//TRACE(_T("index=%d\n"), index);
//
//	//if (zr == ZR_OK)
//	//{
//
//	//}
//	//else
//	//{
//
//	//}
//
//	TCHAR targetname[MAX_PATH];
//	_tcscpy(targetname, lpszFile);
//	//_tcscat(targetname, lpszFile);
//
//	// delete target file if it exists
//	TCHAR szTargetFileName[MAX_PATH] = {0};
//	_tcscpy_s(szTargetFileName, MAX_PATH, "F:\\智能指挥系统\\SBAISDataService\\SBAISDataService\\");
//	_tcscat(szTargetFileName, lpszFile);
//	::DeleteFile(szTargetFileName);
//
//	zr = UnzipItem(hz, index, targetname, 0, ZIP_FILENAME);
//
//	if (zr == ZR_OK)
//	{
//
//	}
//
//	//BOOL bResult = FALSE;
//	//BOOL bRet = Compare(lpszFile, targetname, &bResult);
//
//	//if (bRet && bResult)
//	//{
//
//	//}
//	//else
//	//{
//
//	//}
//
//	// the target file is not deleted - you can inspect it after running test
//
//	//::DeleteFile(targetname);
//}

int CMSMQApiWrapper::SendStreamMessage(CString strIP, LPCTSTR pszQueueName, CString strQueueLabel, CString szLabel,CString szMessage)
{
	//LOG3("Enter CMSMQApiWrapper::SendStreamMessage:%s, %s, %s", strIP, pszQueueName, strQueueLabel);
	CString m_ServerName = ".";
	DWORD cchBuff = 256;
	LPTSTR lpszSystemInfo = NULL;
	char tchBuffer[256] = {0};
	lpszSystemInfo = tchBuffer; 

	m_ServerName = strIP;
	m_ServerName += "\\Private$\\";
	m_ServerName += pszQueueName;

	CString fname;
	_variant_t vtReceiveTimeout;
	IMSMQQueueInfoPtr qinfo = NULL;
	qinfo.CreateInstance("MSMQ.MSMQQueueInfo");

	qinfo->PathName = m_ServerName.AllocSysString();
	//		qinfo->FormatName = fname.AllocSysString();
	qinfo->Label = strQueueLabel.AllocSysString();   // Setting the Queue Label



	//	qinfo->PathName=m_ServerName.AllocSysString();
	//fname="DIRECT=OS:";
	fname="DIRECT=TCP:";
	fname+=m_ServerName;
	qinfo->FormatName = fname.AllocSysString();
	{
		IMSMQQueuePtr qSend;
		IMSMQMessagePtr m("MSMQ.MSMQMessage");
		//  IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
		// remote access 2012-08-02
		//qinfo->Refresh();
		qinfo->Journal = 0;
		//qinfo->Update();
		try
		{
			VARIANT varIsTransactional;
			VariantInit(&varIsTransactional);
			varIsTransactional.vt = VT_I2;
			varIsTransactional.iVal = MQ_SINGLE_MESSAGE;

			qSend = qinfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
			if (qSend == NULL)
			{
				// LOG("in ReadStreamMessage: open queue failed pReq = 0");
				SysFreeString(qinfo->PathName);
				SysFreeString(qinfo->Label);
				SysFreeString(qinfo->FormatName);
				return MSMQ_FUNCTIONFAILURE;
			}


			// qSend = qinfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
			COleVariant vtMessage,vtLabel;



			//vtMessage.SetString(szMessage,VT_BSTR);

			CByteArray byteArray;
			byteArray.SetSize(szMessage.GetLength());
			memcpy(byteArray.GetData(), (LPCTSTR)szMessage, szMessage.GetLength());
			vtMessage.operator =(byteArray);

			vtLabel.SetString(szLabel,VT_BSTR);

			//m->Body = ((LPVARIANT)vtMessage)->pbVal;
			m->put_Body((VARIANT)vtMessage);


			m->Label = ((LPVARIANT)vtLabel)->bstrVal;
			m->Send(qSend, &varIsTransactional);

			// LOG1("in ReadStreamMessage: msgReq=%X", msgRec);
			SysFreeString(qinfo->PathName);
			SysFreeString(qinfo->Label);
			SysFreeString(qinfo->FormatName);
			qSend->Close();
			VariantClear(&varIsTransactional);
		}
		catch (_com_error comerr)
		{
			LOG("in ReadStreamMessage: open queue failed");
			if (qinfo != NULL)
				SysFreeString(qinfo->PathName);
			if (qinfo != NULL)
				SysFreeString(qinfo->Label);
			if (qinfo != NULL)
				SysFreeString(qinfo->FormatName);
			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}

	return MSMQ_FUNCTIONFAILURE;
}

int CMSMQApiWrapper::SendStreamMessage2(CString strIP, LPCTSTR pszQueueName, CString strQueueLabel, CString szLabel,const char *pMessage, int nLen)
{
	//LOG3("Enter CMSMQApiWrapper::SendStreamMessage:%s, %s, %s", strIP, pszQueueName, strQueueLabel);
	CString m_ServerName = ".";
	DWORD cchBuff = 256;
	LPTSTR lpszSystemInfo = NULL;
	char tchBuffer[256] = {0};
	lpszSystemInfo = tchBuffer; 

	m_ServerName = strIP;
	m_ServerName += "\\Private$\\";
	m_ServerName += pszQueueName;

	CString fname;
	_variant_t vtReceiveTimeout;
	IMSMQQueueInfoPtr qinfo = NULL;
	qinfo.CreateInstance("MSMQ.MSMQQueueInfo");

	qinfo->PathName = m_ServerName.AllocSysString();
	//		qinfo->FormatName = fname.AllocSysString();
	qinfo->Label = strQueueLabel.AllocSysString();   // Setting the Queue Label



	//	qinfo->PathName=m_ServerName.AllocSysString();
	//fname="DIRECT=OS:";
	fname="DIRECT=TCP:";
	fname+=m_ServerName;
	qinfo->FormatName = fname.AllocSysString();
	{
		IMSMQQueuePtr qSend;
		IMSMQMessagePtr m("MSMQ.MSMQMessage");
		//  IMSMQMessagePtr msgRec("MSMQ.MSMQMessage");
		// remote access 2012-08-02
		//qinfo->Refresh();
		qinfo->Journal = 0;
		//qinfo->Update();
		try
		{
			VARIANT varIsTransactional;
			VariantInit(&varIsTransactional);
			varIsTransactional.vt = VT_I2;
			varIsTransactional.iVal = MQ_SINGLE_MESSAGE;

			qSend = qinfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
			if (qSend == NULL)
			{
				// LOG("in ReadStreamMessage: open queue failed pReq = 0");
				SysFreeString(qinfo->PathName);
				SysFreeString(qinfo->Label);
				SysFreeString(qinfo->FormatName);
				return MSMQ_FUNCTIONFAILURE;
			}


			// qSend = qinfo->Open(MQ_SEND_ACCESS, MQ_DENY_NONE);
			COleVariant vtMessage,vtLabel;



			//vtMessage.SetString(szMessage,VT_BSTR);

			CByteArray byteArray;
			byteArray.SetSize(nLen);
			memcpy(byteArray.GetData(), pMessage, nLen);
			vtMessage.operator =(byteArray);

			vtLabel.SetString(szLabel,VT_BSTR);

			//m->Body = ((LPVARIANT)vtMessage)->pbVal;
			m->put_Body((VARIANT)vtMessage);


			m->Label = ((LPVARIANT)vtLabel)->bstrVal;
			m->Send(qSend, &varIsTransactional);

			// LOG1("in ReadStreamMessage: msgReq=%X", msgRec);
			SysFreeString(qinfo->PathName);
			SysFreeString(qinfo->Label);
			SysFreeString(qinfo->FormatName);
			qSend->Close();
			VariantClear(&varIsTransactional);
		}
		catch (_com_error comerr)
		{
			CString strMsg = "";
			strMsg = (char*) comerr.Description();
			LOG1("in SendStreamMessage2: open queue failed:%s", strMsg);

			if (qinfo != NULL)
				SysFreeString(qinfo->PathName);
			if (qinfo != NULL)
				SysFreeString(qinfo->Label);
			if (qinfo != NULL)
				SysFreeString(qinfo->FormatName);


			return MSMQ_FUNCTIONFAILURE;
		}
		return MSMQ_FUNCTIONSUCCESS;
	}

	return MSMQ_FUNCTIONFAILURE;
}

