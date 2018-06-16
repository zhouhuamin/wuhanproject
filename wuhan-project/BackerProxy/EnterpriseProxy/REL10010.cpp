#include "StdAfx.h"
#include "REL10010.h"
#include "tinyxml/tinyxml.h"
#include "AdoOper/RADatabase.h"
#include "msmq/MSMQApiWrapper.h"
#include "CxLog.h"
#include "MessageType.h"
#include "MSMQSender.h"

#include <string>
#include <vector>
#include <boost/shared_array.hpp>

using namespace std;

CREL10010::CREL10010(void) : m_messageHead(), m_billInfo(), m_respondInfo(), m_chOpFlag('F'), \
m_chSEND_FLAG('1'), ENTRY_ID(""),TRAF_NAME(""),VOYAGE_NO(""),BILL_NO(""),AGENT_NAME(""),PACK_NO(""),GROSS_WT("")
{
}

CREL10010::~CREL10010(void)
{
}

void CREL10010::InitConfigInfo(structConfigInfo *pConfig)
{
	m_config.strQueueIP		= pConfig->strQueueIP;
	m_config.strQueueName	= pConfig->strQueueName;
	m_config.strQueueLabel	= pConfig->strQueueLabel;

	m_config.strWebQueueIP		= pConfig->strWebQueueIP;
	m_config.strWebQueueName	= pConfig->strWebQueueName;
	m_config.strWebQueueLabel	= pConfig->strWebQueueLabel;

	m_config.strFrontQueueIP		= pConfig->strFrontQueueIP;
	m_config.strFrontQueueName		= pConfig->strFrontQueueName;
	m_config.strFrontQueueLabel		= pConfig->strFrontQueueLabel;

	m_config.strBackerQueueIP		= pConfig->strBackerQueueIP;
	m_config.strBackerQueueName		= pConfig->strBackerQueueName;
	m_config.strBackerQueueLabel	= pConfig->strBackerQueueLabel;

	m_config.strSqlServerDataUser	= pConfig->strSqlServerDataUser;
	m_config.strSqlServerDataPwd	= pConfig->strSqlServerDataPwd;
	m_config.strSqlServerDataName	= pConfig->strSqlServerDataName;
	m_config.strSqlServerDataServer	= pConfig->strSqlServerDataServer;
	m_config.strFilePath			= pConfig->strFilePath;
	m_config.strErrorFilePath		= pConfig->strErrorFilePath;

}

void CREL10010::SaveFileName(const std::string &strFileName)
{
	m_strFileName = strFileName;
	//m_strFileName = "C:\\";

	//if (!strFileName.IsEmpty())
	//{
	//	m_strFileName += (LPCTSTR)strFileName;
	//	m_strFileName += ".xml";
	//}
	//else
	//{
	//	CString strNowTime = "";
	//	strNowTime = COleDateTime::GetCurrentTime().Format("%Y-%m-%d_%H:%M:%S_");
	//	m_strFileName += (LPCTSTR)strNowTime;
	//	m_strFileName += "ContaNoUnloader.xml";
	//}
}
//
//int CREL10010::SaveLocal(BYTE *pMessage, int iLen)
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

void CREL10010::ParseMessage()
{
	try
	{	
		if (m_strFileName.empty())
			return;

		string strFileName = m_strFileName;
		TiXmlDocument doc(strFileName.c_str());
		doc.LoadFile();

		TiXmlHandle docHandle(&doc);
		TiXmlHandle AIR_ENTRY_LIST			= docHandle.FirstChildElement("AIR_ENTRY_LIST");
		TiXmlHandle ENTRY_IDHandle			= docHandle.FirstChildElement("AIR_ENTRY_LIST").ChildElement("ENTRY_ID", 0).FirstChild();
		TiXmlHandle TRAF_NAMEHandle			= docHandle.FirstChildElement("AIR_ENTRY_LIST").ChildElement("TRAF_NAME", 0).FirstChild();
		TiXmlHandle VOYAGE_NOHandle			= docHandle.FirstChildElement("AIR_ENTRY_LIST").ChildElement("VOYAGE_NO", 0).FirstChild();
		TiXmlHandle BILL_NOHandle			= docHandle.FirstChildElement("AIR_ENTRY_LIST").ChildElement("BILL_NO", 0).FirstChild();
		TiXmlHandle AGENT_NAMEHandle		= docHandle.FirstChildElement("AIR_ENTRY_LIST").ChildElement("AGENT_NAME", 0).FirstChild();
		TiXmlHandle PACK_NOHandle			= docHandle.FirstChildElement("AIR_ENTRY_LIST").ChildElement("PACK_NO", 0).FirstChild();
		TiXmlHandle GROSS_WTHandle			= docHandle.FirstChildElement("AIR_ENTRY_LIST").ChildElement("GROSS_WT", 0).FirstChild();

		if (ENTRY_IDHandle.Node() != NULL)	
			ENTRY_ID 			= ENTRY_IDHandle.Text()->Value();
		if (TRAF_NAMEHandle.Node() != NULL)	
			TRAF_NAME 			= TRAF_NAMEHandle.Text()->Value();
		if (VOYAGE_NOHandle.Node() != NULL)	
			VOYAGE_NO			= VOYAGE_NOHandle.Text()->Value();
		if (BILL_NOHandle.Node() != NULL)	
			BILL_NO 			= BILL_NOHandle.Text()->Value();
		if (AGENT_NAMEHandle.Node() != NULL)	
			AGENT_NAME 			= AGENT_NAMEHandle.Text()->Value();
		if (PACK_NOHandle.Node() != NULL)	
			PACK_NO 			= PACK_NOHandle.Text()->Value();
		if (GROSS_WTHandle.Node() != NULL)	
			GROSS_WT 			= GROSS_WTHandle.Text()->Value();
	}
	catch(...)
	{
		return ;
	}

	//CStdioFile file;
	//file.Open("F:\\test\\log.txt",CFile::modeRead|CFile::typeBinary);
	//int iLen = file.GetLength();
	//char* pData = new char[iLen+1];
	//memset(pData, 0x00, sizeof(char) * (iLen + 1));
	//file.Seek(0,CFile::begin);
	//file.Read(pData, iLen);
	//file.Close();	

	return ;
}

int CREL10010::SaveToDB()
{
	return 1;
	if (m_messageHead.MESSAGE_ID[0] == '\0')
		return 0;

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

		std::string ENTRY_ID = "";
		std::string TRAF_NAME = "";
		std::string VOYAGE_NO = "";
		std::string BILL_NO = "";
		std::string AGENT_NAME = "";
		std::string PACK_NO = "";
		std::string GROSS_WT = "";		
		
		
		if (m_messageHead.MESSAGE_ID[0] != '\0')
		{
			double dPACK_NO = atof(PACK_NO.c_str());
			double dGROSS_WT	= atof(GROSS_WT.c_str());
			CString strSql = "";
			char szSqlBuffer[1024] = {0};
			sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into H_AIR_ENTRY_LIST(ENTRY_ID,TRAF_NAME,VOYAGE_NO,BILL_NO,AGENT_NAME,PACK_NO,GROSS_WT) \
values('%s', '%s', '%s', '%s', '%s', '%.1f', '%.5f', GetDate())",
ENTRY_ID,
TRAF_NAME,
VOYAGE_NO,
BILL_NO,
AGENT_NAME,
dPACK_NO,
dGROSS_WT
);

			strSql += szSqlBuffer;

			// 执行Sql语句
			database.RAExecuteNoRs((LPCTSTR)strSql);
			LOG1("insert into H_AIR_ENTRY_LIST, ENTRY_ID=%s", ENTRY_ID);
		}
	}
	catch(...)
	{
		return 0;
	}

	m_chSEND_FLAG = '2';
	return 1;
}

void CREL10010::SendResponse(const std::string &strPreFlag)
{
	// 海关平台返回的回执
	if (strPreFlag == "WEB")
	{
		boost::shared_array<char> spXmlData;
		int nXmlLen = 0;

		CStdioFile file;
		if (!file.Open(m_strFileName.c_str(), CFile::modeRead|CFile::typeBinary))
		{
			return;
		}
		DWORD iLen = (DWORD)file.GetLength();
		if (iLen <= 0)
		{
			file.Close();
			return;
		}

		spXmlData.reset(new char[iLen + 1]);
		file.Seek(0,CFile::begin);
		file.Read(spXmlData.get(), iLen);
		file.Close();	
		spXmlData[iLen] = '\0';


		// 发送MSMQ
		std::string strQueueName	= m_config.strWebQueueName;
		std::string strMQIp			= m_config.strWebQueueIP;
		std::string szLabel			= "WEB-";
		szLabel += "REL10010"; // m_messageHead.MESSAGE_TYPE; // CMessageType::CLM20022;
		std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
		apBasic->SendMessage(spXmlData.get(), iLen);

		// delete file
		if (m_strFileName != "")
		{
			if (GetFileAttributes(m_strFileName.c_str()) != -1)
				CFile::Remove(m_strFileName.c_str());
		}


		//CStringArray queueNameArray;
		//CStringArray mqIPArray;
		//SelectMQForWeb(queueNameArray, mqIPArray);

		//for (int i = 0, j = 0; i < queueNameArray.GetSize() && j < mqIPArray.GetSize(); ++i,++j)
		//{
		//	if (i != j)
		//		break;
		//	// 发送MSMQ
		//	std::string strQueueName	= (LPCTSTR)queueNameArray[j];
		//	std::string strMQIp			= (LPCTSTR)mqIPArray[i];
		//	std::string szLabel			= CMessageType::CLM20022;
		//	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
		//	apBasic->SendMessage(spXmlData.get(), iLen);
		//}

		//// delete file
		//if (m_strFileName != "")
		//{
		//	CFile::Remove(m_strFileName.c_str());
		//}

		return;
	}

	//CString strMESSAGE_ID	= "";
	//CString strOP_FLAG		= "";
	//CString strNOTE			= "";

	//if (m_respondInfo.OP_FLAG[0] == 'T')
	//{
	//	SelectResponseInfo(strMESSAGE_ID, strNOTE, strOP_FLAG);
	//	if (strMESSAGE_ID.IsEmpty())
	//	{
	//		InsertResponseInfo();
	//		try
	//		{
	//			// delete file
	//			if (m_strFileName != "")
	//			{
	//				CFile::Remove(m_strFileName.c_str());
	//			}
	//		}
	//		catch(CFileException *pEx)
	//		{
	//			LOG("cannot be removed\n");
	//			pEx->Delete();
	//		}
	//		return;
	//	}
	//}

	//DeleteResponseInfo();

	boost::shared_array<char> spXmlData;
	int nXmlLen = 0;

	CStdioFile file;
	if (!file.Open(m_strFileName.c_str(), CFile::modeRead|CFile::typeBinary))
	{
		return;
	}
	DWORD iLen = (DWORD)file.GetLength();
	if (iLen <= 0)
	{
		file.Close();
		return;
	}

	spXmlData.reset(new char[iLen + 1]);
	file.Seek(0,CFile::begin);
	file.Read(spXmlData.get(), iLen);
	file.Close();	
	spXmlData[iLen] = '\0';

	// 发送MSMQ
	std::string strQueueName	= m_config.strWebQueueName;
	std::string strMQIp			= m_config.strWebQueueIP;
	std::string szLabel			= ""; // "WEB-";
	szLabel += "REL10010"; 		// m_messageHead.MESSAGE_TYPE; // CMessageType::CLM20022;
	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	apBasic->SendMessage(spXmlData.get(), iLen);

	// delete file
	if (m_strFileName != "")
	{
		if (GetFileAttributes(m_strFileName.c_str()) != -1)
			CFile::Remove(m_strFileName.c_str());
	}


	//CStringArray queueNameArray;
	//CStringArray mqIPArray;
	//SelectMQ(queueNameArray, mqIPArray);

	//for (int i = 0, j = 0; i < queueNameArray.GetSize() && j < mqIPArray.GetSize(); ++i,++j)
	//{
	//	if (i != j)
	//		break;
	//	// 发送MSMQ
	//	std::string strQueueName	= (LPCTSTR)queueNameArray[j];
	//	std::string strMQIp			= (LPCTSTR)mqIPArray[i];
	//	std::string szLabel			= CMessageType::CLM20022;
	//	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	//	apBasic->SendMessage(spXmlData.get(), iLen);
	//}

	//// delete file
	//if (m_strFileName != "")
	//{
	//	CFile::Remove(m_strFileName.c_str());
	//}

}

int CREL10010::UpdateDB()
{
	//if (m_messageHead.MESSAGE_ID[0] == '\0')
	//	return 0;
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

	//	if (m_messageHead.MESSAGE_ID[0] != '\0')
	//	{
	//		CString strSql = "";
	//		char szSqlBuffer[2048] = {0};
	//		sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "update D_RELEASE_DICTATE_BILL set SEND_TIME=GetDate(), SEND_FLAG='%c' where MESSAGE_ID='%s'",  m_chSEND_FLAG, m_messageHead.MESSAGE_ID);

	//		strSql += szSqlBuffer;

	//		// 执行Sql语句
	//		database.RAExecuteNoRs((LPCTSTR)strSql);
	//		LOG2("update D_SHIP_DICTATE_BILL, MESSAGE_ID=%s, SEND_FLAG='%c'", m_messageHead.MESSAGE_ID, m_chSEND_FLAG);
	//	}
	//}
	//catch(...)
	//{
	//	return 0;
	//}

	return 1;
}

std::string CREL10010::GetFileName()
{
	return m_strFileName;
}

int CREL10010::SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='%s' and INOROUTFLAG='1'", m_messageHead.SENDER, m_messageHead.RECEIVE);

		CString strConn = "";
		strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		//strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataPwd.c_str();
		//strConn += ";Persist Security Info=True;User ID=";
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataUser.c_str();
		//strConn += ";Initial Catalog="; // WHHGWL";
		//strConn += m_config.strSqlServerDataName.c_str();
		//strConn += ";Data Source=";
		////strConn += "127.0.0.1";
		//strConn += m_config.strSqlServerDataServer.c_str();

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
			val = oRs->Fields->Item[_variant_t("MQPATH")]->Value;
			if (val.vt != (VT_NULL))
			{
				strMQPath = (char*)_bstr_t(val);	
				mqPathArray.Add(strMQPath);
			}

			oRs->MoveNext();
		}
		oRs->Release();					
		oRs->Close();


		CString strQueueName	= "";
		CString strMQIp			= "";
		//CStringArray queueNameArray;
		//CStringArray mqIPArray;
		for (int i = 0; i < mqPathArray.GetSize(); ++i)
		{
			strMQPath = mqPathArray[i];
			if (strMQPath.IsEmpty())
				continue;

			// formatname:direct=tcp:192.168.32.30\private$\whhg_OutToIn
			int index = strMQPath.ReverseFind('\\');
			if (index > 0)
			{
				strQueueName = (LPCTSTR)strMQPath.Mid(index + 1);
				queueNameArray.Add(strQueueName);
			}

			index = strMQPath.ReverseFind(':');
			int right = strMQPath.Find('\\');
			if (index > 0 && index < right)
			{
				strMQIp = (LPCTSTR)strMQPath.Mid(index + 1, right - index - 1);
				mqIPArray.Add(strMQIp);
			}
		}
	}
	catch(...)
	{
		return 0;
	}
	return 1;
}

int CREL10010::SelectResponseInfo(CString &strMESSAGE_ID, CString &strOP_FLAG, CString &strNOTE)
{
	if (m_respondInfo.MESSAGE_ID[0] == '\0')
		return 0;
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select top 1 MESSAGE_ID,OP_FLAG,OP_NOTE from WH_RESPONSE_INFO where MESSAGE_ID='%s'", m_respondInfo.MESSAGE_ID);

		CString strConn = "";
		strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		//strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataPwd.c_str();
		//strConn += ";Persist Security Info=True;User ID=";
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataUser.c_str();
		//strConn += ";Initial Catalog="; // WHHGWL";
		//strConn += m_config.strSqlServerDataName.c_str();
		//strConn += ";Data Source=";
		////strConn += "127.0.0.1";
		//strConn += m_config.strSqlServerDataServer.c_str();

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
			val = oRs->Fields->Item[_variant_t("MESSAGE_ID")]->Value;
			if (val.vt != (VT_NULL))
			{
				strMESSAGE_ID = (char*)_bstr_t(val);	
			}

			val = oRs->Fields->Item[_variant_t("OP_FLAG")]->Value;
			if (val.vt != (VT_NULL))
			{
				strOP_FLAG = (char*)_bstr_t(val);	
			}

			val = oRs->Fields->Item[_variant_t("OP_NOTE")]->Value;
			if (val.vt != (VT_NULL))
			{
				strNOTE = (char*)_bstr_t(val);	
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

int CREL10010::InsertResponseInfo()
{
	if (m_respondInfo.MESSAGE_ID[0] == '\0')
		return 0;

	try
	{

		CString strConn = "";
		strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		//strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataPwd.c_str();
		//strConn += ";Persist Security Info=True;User ID=";
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataUser.c_str();
		//strConn += ";Initial Catalog="; // CGS_DB";
		//strConn += m_config.strSqlServerDataName.c_str();
		//strConn += ";Data Source=";
		////strConn += "127.0.0.1";
		//strConn += m_config.strSqlServerDataServer.c_str();



		CRADatabase database;
		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return 0;
		}

		CString strSql = "";
		char szSqlBuffer[2048] = {0};
		sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into WH_RESPONSE_INFO(MESSAGE_ID,OP_FLAG,OP_NOTE) values('%s', '%c', '%s')", m_respondInfo.MESSAGE_ID, m_respondInfo.OP_FLAG[0], m_respondInfo.NOTE);

		strSql += szSqlBuffer;

		// 执行Sql语句
		database.RAExecuteNoRs((LPCTSTR)strSql);
		LOG1("insert into WH_RESPONSE_INFO, MESSAGE_ID=%s", m_respondInfo.MESSAGE_ID);
	}
	catch(...)
	{
		return 0;
	}

	return 1;
}

int CREL10010::DeleteResponseInfo()
{
	if (m_respondInfo.MESSAGE_ID[0] == '\0')
		return 0;

	try
	{

		CString strConn = "";
		strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		//strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataPwd.c_str();
		//strConn += ";Persist Security Info=True;User ID=";
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataUser.c_str();
		//strConn += ";Initial Catalog="; // CGS_DB";
		//strConn += m_config.strSqlServerDataName.c_str();
		//strConn += ";Data Source=";
		////strConn += "127.0.0.1";
		//strConn += m_config.strSqlServerDataServer.c_str();



		CRADatabase database;
		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return 0;
		}

		CString strSql = "";
		char szSqlBuffer[2048] = {0};
		sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "delete from WH_RESPONSE_INFO where MESSAGE_ID = '%s'", m_respondInfo.MESSAGE_ID);

		strSql += szSqlBuffer;

		// 执行Sql语句
		database.RAExecuteNoRs((LPCTSTR)strSql);
		LOG1("delete from WH_RESPONSE_INFO, MESSAGE_ID=%s", m_respondInfo.MESSAGE_ID);
	}
	catch(...)
	{
		return 0;
	}

	return 1;
}

int CREL10010::SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='WEB' and INOROUTFLAG='1'", m_messageHead.SENDER);

		CString strConn = "";
		strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		//strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataPwd.c_str();
		//strConn += ";Persist Security Info=True;User ID=";
		////strConn += "sa";
		//strConn += m_config.strSqlServerDataUser.c_str();
		//strConn += ";Initial Catalog="; // WHHGWL";
		//strConn += m_config.strSqlServerDataName.c_str();
		//strConn += ";Data Source=";
		////strConn += "127.0.0.1";
		//strConn += m_config.strSqlServerDataServer.c_str();

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
			val = oRs->Fields->Item[_variant_t("MQPATH")]->Value;
			if (val.vt != (VT_NULL))
			{
				strMQPath = (char*)_bstr_t(val);	
				mqPathArray.Add(strMQPath);
			}

			oRs->MoveNext();
		}
		oRs->Release();					
		oRs->Close();


		CString strQueueName	= "";
		CString strMQIp			= "";
		//CStringArray queueNameArray;
		//CStringArray mqIPArray;
		for (int i = 0; i < mqPathArray.GetSize(); ++i)
		{
			strMQPath = mqPathArray[i];
			if (strMQPath.IsEmpty())
				continue;

			// formatname:direct=tcp:192.168.32.30\private$\whhg_OutToIn
			int index = strMQPath.ReverseFind('\\');
			if (index > 0)
			{
				strQueueName = (LPCTSTR)strMQPath.Mid(index + 1);
				queueNameArray.Add(strQueueName);
			}

			index = strMQPath.ReverseFind(':');
			int right = strMQPath.Find('\\');
			if (index > 0 && index < right)
			{
				strMQIp = (LPCTSTR)strMQPath.Mid(index + 1, right - index - 1);
				mqIPArray.Add(strMQIp);
			}
		}
	}
	catch(...)
	{
		return 0;
	}
	return 1;
}	
