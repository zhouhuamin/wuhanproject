#include "StdAfx.h"
#include "GUITransaction.h"
#include "tinyxml/tinyxml.h"
#include "AdoOper/RADatabase.h"
#include "msmq/MSMQApiWrapper.h"
#include "CxLog.h"
#include "MessageType.h"
#include "MessageParseStrategy.h"
#include "CLMMessageParseStrategy.h"
#include "CSMMessageParseStrategy.h"
#include "MTMessageParseStrategy.h"
#include "MSMQSender.h"

#include <string>

using namespace std;


CGUITransaction::CGUITransaction(void)// : m_strGUID("")
{
}

CGUITransaction::~CGUITransaction(void)
{
}

void CGUITransaction::InitConfigInfo(structConfigInfo *pConfig)
{
	m_config.strQueueIP		= pConfig->strQueueIP;
	m_config.strQueueName	= pConfig->strQueueName;
	m_config.strQueueLabel	= pConfig->strQueueLabel;

	m_config.strSqlServerDataUser	= pConfig->strSqlServerDataUser;
	m_config.strSqlServerDataPwd	= pConfig->strSqlServerDataPwd;
	m_config.strSqlServerDataName	= pConfig->strSqlServerDataName;
	m_config.strSqlServerDataServer	= pConfig->strSqlServerDataServer;
	m_config.strFilePath			= pConfig->strFilePath;
	m_config.strErrorFilePath		= pConfig->strErrorFilePath;
}

void CGUITransaction::SaveFileName(const std::string &strFileName)
{
	m_strFileName = strFileName;
	return;
}
//
//int CCLM10041::SaveLocal(BYTE *pMessage, int iLen)
//{
//	if (pMessage == NULL || iLen <= 1)
//	{
//		return 0;
//	}
//
//	CStdioFile  file;
//	file.Open(m_strFileName.c_str(),CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
//	file.Write(pMessage, iLen);
//	file.Close();
//	delete []pMessage;
//	pMessage = NULL;
//	//m_strFileName = "C:\\whcontainer.xml";
//
//	return 1;
//}

void CGUITransaction::ParseMessage()
{
	//if (m_strFileName.empty())
	//	return;

	//string strFileName = m_strFileName;
	//// delete file
	//if (strFileName != "")
	//{
	//	//CFile::Remove(strFileName.c_str());
	//	CString strTmpName = strFileName.c_str();
	//	int index = strTmpName.ReverseFind('\\');
	//	if (index > 0)
	//		strTmpName = strTmpName.Mid(index + 1);

	//	SYSTEMTIME time;
	//	GetLocalTime(&time);
	//	CString strDateTime;
	//	strDateTime.Format(".%04d%02d%02d%02d%02d%02d%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
	//	char tmpPath[MAX_PATH] = {0};
	//	strncpy_s(tmpPath, sizeof(tmpPath), m_config.strErrorFilePath.c_str(), MAX_PATH);
	//	strcat_s(tmpPath, sizeof(tmpPath), "\\");
	//	strcat_s(tmpPath, sizeof(tmpPath), (LPCTSTR)strTmpName);
	//	strcat_s(tmpPath, sizeof(tmpPath), (LPCTSTR)strDateTime);
	//	try
	//	{
	//		::DeleteFile(tmpPath);
	//		::MoveFile(m_strFileName.c_str(), tmpPath);
	//	}
	//	catch(...)
	//	{
	//		int ia = 3;
	//	}
	//}
	return ;
}

int CGUITransaction::SaveToDB()
{
	return 1;
}

void CGUITransaction::SendResponse()
{
	std::vector<std::string> guidVect;
	GetGUIDFromDB(guidVect);
	for (size_t i = 0; i < guidVect.size(); ++i)
	{
		std::string strMessageType = "";
		std::string strSender	   = "";
		std::string strRECEIVER	   = "";
		std::string strGUID		   = guidVect[i];
		boost::shared_array<char> spXmlData;
		int nXmlLen = 0;

		if (strGUID.empty())
			continue;

		GetXmlFromDB(strGUID, strMessageType, strSender, spXmlData, nXmlLen);
		SaveLocalFile(strGUID, spXmlData, nXmlLen);
		GetReceiverFromLocal(strMessageType, strRECEIVER);
		SelectMQ(strMessageType, strSender, strRECEIVER);

		// 发送MSMQ
		std::string szLabel = "WEB-";
		szLabel += strMessageType;
		std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(m_strMQIp, m_strQueueName, szLabel, szLabel));
		apBasic->SendMessage(spXmlData.get(), nXmlLen);

		UpdateDB(strGUID);

	}





	//CMSMQApiWrapper ob;
	//std::string szLabel = strMessageType;
	//if ((!m_strMQIp.empty()) && (!m_strQueueName.empty()))
	//	int nRet = ob.SendStreamMessage2(m_strMQIp.c_str(), m_strQueueName.c_str(), szLabel.c_str(), szLabel.c_str(), spXmlData.get(), nXmlLen);
	return;
}

int CGUITransaction::UpdateDB()
{
	////if (m_strMQIp.empty() || m_strQueueName.empty())
	////	return 0;

	//if (m_strGUID.empty())
	//	return 0;
	////if (m_messageHead.MESSAGE_ID[0] == '\0')
	////	return 0;
	//try
	//{

	//	CString strConn = "";
	//	strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
	//	//strConn += "sa";
	//	strConn += m_config.strSqlServerDataPwd.c_str();
	//	strConn += ";Persist Security Info=True;User ID=";
	//	//strConn += "sa";
	//	strConn += m_config.strSqlServerDataUser.c_str();
	//	strConn += ";Initial Catalog="; // CGS_DB";
	//	strConn += m_config.strSqlServerDataName.c_str();
	//	strConn += ";Data Source=";
	//	//strConn += "127.0.0.1";
	//	strConn += m_config.strSqlServerDataServer.c_str();

	//	CRADatabase database;
	//	HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
	//	if (nRet == E_FAIL)
	//	{
	//		return 0;
	//	}

	//	if (!m_strGUID.empty())
	//	{
	//		CString strSql = "";
	//		char szSqlBuffer[2048] = {0};
	//		sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "update R_APPLY_REPORT set DEL_FLAG='%c' where GUID='%s'",  '1', m_strGUID.c_str());

	//		strSql += szSqlBuffer;

	//		// 执行Sql语句
	//		database.RAExecuteNoRs((LPCTSTR)strSql);
	//		//LOG2("update D_SHIP_DICTATE_BILL, MESSAGE_ID=%s, SEND_FLAG='%c'", m_messageHead.MESSAGE_ID, m_chSEND_FLAG);
	//	}
	//}
	//catch(...)
	//{
	//	return 0;
	//}
	return 1;
}

std::string CGUITransaction::GetFileName()
{
	return m_strFileName;
}

int CGUITransaction::GetXmlFromDB(const std::string &strParaGUID, std::string &strMessageType,std::string &strSender,boost::shared_array<char> &spXmlData, int &nXmlLen)
{
	try
	{
		char szSql[512] = {0};
		sprintf_s(szSql, sizeof(szSql), "select top 1 GUID, MESSAGE_TYPE, SENDER, REPORT from R_APPLY_REPORT where GUID = '%s'", strParaGUID.c_str());
		CString strResult		= "";
		CString strConn			= "";
		CString strGUID			= "";
		CString MESSAGE_TYPE	= "";
		CString SENDER			= "";
		strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
		//strConn += "sa";
		strConn += m_config.strSqlServerDataPwd.c_str();
		strConn += ";Persist Security Info=True;User ID=";
		//strConn += "sa";
		strConn += m_config.strSqlServerDataUser.c_str();
		strConn += ";Initial Catalog="; // CGS_DB";
		strConn += m_config.strSqlServerDataName.c_str();
		strConn += ";Data Source=";
		//strConn += "127.0.0.1";
		strConn += m_config.strSqlServerDataServer.c_str();

		CRADatabase database;
		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return 0;
		}

		_RecordsetPtr oRs;
		oRs = database.RAExecuteRs(szSql);
		CComVariant val;

		if (!oRs->EndOfFile)
		{	
			val = oRs->Fields->Item[_variant_t("GUID")]->Value;
			if (val.vt != (VT_NULL))
			{
				strGUID = (char*)_bstr_t(val);	
				//m_strGUID = strGUID;
				

				CString strFileName = m_config.strFilePath.c_str();
				strFileName += "\\";
				strFileName += strGUID;
				strFileName += ".xml";
				m_strFileName = (LPCTSTR)strFileName;
			}

			val = oRs->Fields->Item[_variant_t("MESSAGE_TYPE")]->Value;
			if (val.vt != (VT_NULL))
			{
				MESSAGE_TYPE = (char*)_bstr_t(val);	
				strMessageType = (LPCTSTR)MESSAGE_TYPE;
			}

			val = oRs->Fields->Item[_variant_t("SENDER")]->Value;
			if (val.vt != (VT_NULL))
			{
				SENDER		= (char*)_bstr_t(val);	
				strSender	= (LPCTSTR)SENDER;
			}
			
			//*bBuffer=NULL;
			long   lDataLength   =   oRs->GetFields()->GetItem( "REPORT")-> ActualSize; 
			if(lDataLength   >   0) 
			{ 
				char *pBuf=NULL;
				spXmlData.reset(new char[lDataLength + 1]);
				_variant_t   varBLOB; 
				varBLOB  =   oRs-> GetFields()->GetItem( "REPORT")->GetChunk(lDataLength); 
				if(varBLOB.vt == (VT_ARRAY | VT_UI1)) ///判断数据类型是否正确 
				{ 
					SafeArrayAccessData(varBLOB.parray,(void **)&pBuf);
					memcpy(spXmlData.get(), pBuf, lDataLength);
					SafeArrayUnaccessData(varBLOB.parray); 
					spXmlData[lDataLength] = '\0';
					nXmlLen = (int)lDataLength;
				}
			}
		}
		oRs->Release();					
		oRs->Close();
	}
	catch(...)
	{
		return 0;
	}
	return 1;
}


int CGUITransaction::SaveLocalFile(const std::string &strGUID, boost::shared_array<char> &spXmlData, int &nXmlLen)
{
	if (nXmlLen <= 1)
	{
		return 0;
	}

	CStdioFile  file;
	file.Open(m_strFileName.c_str(),CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
	file.Write(spXmlData.get(), nXmlLen);
	file.Close();
	return 1;
}

int CGUITransaction::SelectMQ(std::string &strMessageType, std::string &strSender, std::string &strRECEIVER)
{
	if (strRECEIVER.empty())
		return 0;

	try
	{
		CString strMQPath = "";
		std::string MQMessageType =strMessageType;
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPath from SITE_MQPATH_CONFIG where MQMessageType='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='ALL' and InOrOutFlag='0'", strRECEIVER.c_str());

		CString strConn = "";
		strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
		//strConn += "sa";
		strConn += m_config.strSqlServerDataPwd.c_str();
		strConn += ";Persist Security Info=True;User ID=";
		//strConn += "sa";
		strConn += m_config.strSqlServerDataUser.c_str();
		strConn += ";Initial Catalog="; // CGS_DB";
		strConn += m_config.strSqlServerDataName.c_str();
		strConn += ";Data Source=";
		//strConn += "127.0.0.1";
		strConn += m_config.strSqlServerDataServer.c_str();

		CRADatabase database;
		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return 0;
		}

		_RecordsetPtr oRs;
		oRs = database.RAExecuteRs(szSql);
		CComVariant val;

		if (!oRs->EndOfFile)
		{	
			val = oRs->Fields->Item[_variant_t("MQPath")]->Value;
			if (val.vt != (VT_NULL))
			{
				strMQPath = (char*)_bstr_t(val);		
			}

			//oRs->MoveNext();
		}
		oRs->Release();					
		oRs->Close();

		// formatname:direct=tcp:192.168.32.30\private$\whhg_OutToIn
		int index = strMQPath.ReverseFind('\\');
		if (index > 0)
			m_strQueueName = (LPCTSTR)strMQPath.Mid(index + 1);

		index = strMQPath.ReverseFind(':');
		int right = strMQPath.Find('\\');
		if (index > 0 && index < right)
			m_strMQIp = (LPCTSTR)strMQPath.Mid(index + 1, right - index - 1);


	}
	catch(...)
	{
		return 0;
	}
	return 1;
}

void CGUITransaction::GetReceiverFromLocal(std::string &strMessageType, std::string &strRECEIVER)
{
	try
	{
		if (m_strFileName.empty())
			return;

		std::string strFileName(m_strFileName);

		if (strMessageType.find("CSM") != -1)
		{
			std::auto_ptr<CMessageParseStrategy> apBasic(new CCSMMessageParseStrategy());
			apBasic->ParseXml(strFileName);
			strRECEIVER = apBasic->GetReceiver();
		}
		else if (strMessageType.find("CLM") != -1)
		{
			std::auto_ptr<CMessageParseStrategy> apBasic(new CCLMMessageParseStrategy());
			apBasic->ParseXml(strFileName);
			strRECEIVER = apBasic->GetReceiver();
		}
		else if (strMessageType.find("MT") != -1)
		{
			std::auto_ptr<CMessageParseStrategy> apBasic(new CMTMessageParseStrategy());
			apBasic->ParseXml(strFileName);
			strRECEIVER = apBasic->GetReceiver();
		}
		else
		{
			strRECEIVER = "";
		}

		//TiXmlDocument doc(strFileName.c_str());
		//doc.LoadFile();

		//TiXmlNode*    node = 0;
		//TiXmlElement* itemElement = 0;
		//TiXmlElement* xmlRootElement = NULL;
		//TiXmlElement* xmlListElement = NULL;
		//TiXmlElement* xmlSubElement = NULL;
		//TiXmlElement* xmlSonElement = NULL;
		//TiXmlNode * pNode = NULL;
		//TiXmlNode *pNodeTmp = NULL;

		//pNode = doc.FirstChild("Message");
		//if (pNode == NULL)
		//	return ;
		//xmlRootElement = pNode->ToElement();
		//if(xmlRootElement)
		//{
		//	if ((pNode = xmlRootElement->FirstChild("MessageHead")) != NULL)
		//	{
		//		std::string RECEIVER = "";

		//		xmlSubElement = pNode->ToElement() ;
		//		if (xmlSubElement != NULL)
		//			pNodeTmp = xmlSubElement->FirstChildElement("ReceiverID");
		//		if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//			RECEIVER = pNodeTmp->ToElement()->GetText();
		//		strRECEIVER = RECEIVER;
		//	}
		//}

		// delete file
		if (strFileName != "")
		{
			if (GetFileAttributes(m_strFileName.c_str()) != -1)
				CFile::Remove(strFileName.c_str());
		}
	}
	catch(...)
	{
		return ;
	}
	return ;
}

int CGUITransaction::UpdateDB(const std::string &strGUID)
{
	//if (m_strMQIp.empty() || m_strQueueName.empty())
	//	return 0;

	if (strGUID.empty())
		return 0;
	//if (m_messageHead.MESSAGE_ID[0] == '\0')
	//	return 0;
	try
	{

		CString strConn = "";
		strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
		//strConn += "sa";
		strConn += m_config.strSqlServerDataPwd.c_str();
		strConn += ";Persist Security Info=True;User ID=";
		//strConn += "sa";
		strConn += m_config.strSqlServerDataUser.c_str();
		strConn += ";Initial Catalog="; // CGS_DB";
		strConn += m_config.strSqlServerDataName.c_str();
		strConn += ";Data Source=";
		//strConn += "127.0.0.1";
		strConn += m_config.strSqlServerDataServer.c_str();

		CRADatabase database;
		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return 0;
		}

		if (!strGUID.empty())
		{
			CString strSql = "";
			char szSqlBuffer[2048] = {0};
			sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "update R_APPLY_REPORT set DEL_FLAG='%c' where GUID='%s'",  '1', strGUID.c_str());

			strSql += szSqlBuffer;

			// 执行Sql语句
			database.RAExecuteNoRs((LPCTSTR)strSql);
			//LOG2("update D_SHIP_DICTATE_BILL, MESSAGE_ID=%s, SEND_FLAG='%c'", m_messageHead.MESSAGE_ID, m_chSEND_FLAG);
		}
	}
	catch(...)
	{
		return 0;
	}
	return 1;
}

int CGUITransaction::GetGUIDFromDB(std::vector<std::string> &guidVect)
{
	try
	{
		char szSql[512] = {0};
		sprintf_s(szSql, sizeof(szSql), "select top 5 GUID from R_APPLY_REPORT where DEL_FLAG = '0'");
		CString strResult		= "";
		CString strConn			= "";
		std::string strGUID			= "";
		CString MESSAGE_TYPE	= "";
		CString SENDER			= "";
		strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
		//strConn += "sa";
		strConn += m_config.strSqlServerDataPwd.c_str();
		strConn += ";Persist Security Info=True;User ID=";
		//strConn += "sa";
		strConn += m_config.strSqlServerDataUser.c_str();
		strConn += ";Initial Catalog="; // CGS_DB";
		strConn += m_config.strSqlServerDataName.c_str();
		strConn += ";Data Source=";
		//strConn += "127.0.0.1";
		strConn += m_config.strSqlServerDataServer.c_str();

		CRADatabase database;
		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return 0;
		}

		_RecordsetPtr oRs;
		oRs = database.RAExecuteRs(szSql);
		CComVariant val;

		while (!oRs->EndOfFile)
		{	
			val = oRs->Fields->Item[_variant_t("GUID")]->Value;
			if (val.vt != (VT_NULL))
			{
				strGUID = (char*)_bstr_t(val);	
				guidVect.push_back(strGUID);
			}

			oRs->MoveNext();
		}
		oRs->Release();					
		oRs->Close();
	}
	catch(...)
	{
		return 0;
	}
	return 1;
}
