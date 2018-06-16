#include "StdAfx.h"
#include "CLM10051.h"
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

CCLM10051::CCLM10051(void) : m_messageHead(), m_billInfo(), m_respondInfo(), m_chOpFlag('F'), \
m_chSEND_FLAG('1'), m_strNote(""),m_contaInfoVect(0)
{
}

CCLM10051::~CCLM10051(void)
{
}

void CCLM10051::InitConfigInfo(structConfigInfo *pConfig)
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

void CCLM10051::SaveFileName(const std::string &strFileName)
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

void CCLM10051::ParseMessage()
{
	try
	{
		if (m_strFileName.empty())
			return;

		string strFileName = m_strFileName;
		TiXmlDocument doc(strFileName.c_str());
		doc.LoadFile();

		TiXmlNode*    node = 0;
		TiXmlElement* itemElement = 0;
		TiXmlElement* xmlRootElement = NULL;
		TiXmlElement* xmlRootElement2 = NULL;
		TiXmlElement* xmlListElement = NULL;
		TiXmlElement* xmlSubElement = NULL;
		TiXmlElement* xmlSonElement = NULL;
		TiXmlNode * pNode = NULL;
		TiXmlNode *pNodeTmp = NULL;

		//pNode = doc.FirstChild("string");
		//xmlRootElement2 = pNode->ToElement();
		//if (xmlRootElement2 == NULL)
		//	return;
		//pNode = xmlRootElement2->FirstChild("MESSAGE");
		//if (pNode == NULL)
		//	return;
		//xmlRootElement = pNode->ToElement();

		pNode = doc.FirstChild("MESSAGE");
		if (pNode == NULL)
			return ;
		xmlRootElement = pNode->ToElement();
		if(xmlRootElement)
		{
			if ((pNode = xmlRootElement->FirstChild("MESSAGE_HEAD")) != NULL)
			{
				string MESSAGE_ID = "";
				string FUNCTION_CODE = "";
				string MESSAGE_TYPE = "";
				string SENDER = "";
				string RECEIVE = "";
				string MESSAGE_CREATE_TIME = "";
				string strVERSION = "";

				xmlSubElement = pNode->ToElement() ;
				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("MESSAGE_ID");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_ID = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("FUNCTION_CODE");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					FUNCTION_CODE = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("MESSAGE_TYPE");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_TYPE = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("SENDER");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					SENDER = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("RECEIVER");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					RECEIVE = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("MESSAGE_CREATE_TIME");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_CREATE_TIME = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("VERSION");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					strVERSION = pNodeTmp->ToElement()->GetText();

				//// ¸³Öµ
				//memcpy(m_messageHead.MESSAGE_ID, MESSAGE_ID.c_str(), sizeof(m_messageHead.MESSAGE_ID) - 1);
				//memcpy(m_messageHead.FUNCTION_CODE, FUNCTION_CODE.c_str(), sizeof(m_messageHead.FUNCTION_CODE) - 1);
				//memcpy(m_messageHead.MESSAGE_TYPE, MESSAGE_TYPE.c_str(), sizeof(m_messageHead.MESSAGE_TYPE) - 1);
				//memcpy(m_messageHead.SENDER, SENDER.c_str(), sizeof(m_messageHead.SENDER) - 1);
				//memcpy(m_messageHead.RECEIVE, RECEIVE.c_str(), sizeof(m_messageHead.RECEIVE) - 1);
				//memcpy(m_messageHead.MESSAGE_CREATE_TIME, MESSAGE_CREATE_TIME.c_str(), sizeof(m_messageHead.MESSAGE_CREATE_TIME) - 1);
				//memcpy(m_messageHead.VERSION, strVERSION.c_str(), sizeof(m_messageHead.VERSION) - 1);

				// ¸³Öµ
				memcpy(m_messageHead.MESSAGE_ID, MESSAGE_ID.c_str(), MESSAGE_ID.size());
				memcpy(m_messageHead.FUNCTION_CODE, FUNCTION_CODE.c_str(), FUNCTION_CODE.size());
				memcpy(m_messageHead.MESSAGE_TYPE, MESSAGE_TYPE.c_str(), MESSAGE_TYPE.size());
				memcpy(m_messageHead.SENDER, SENDER.c_str(), SENDER.size());
				memcpy(m_messageHead.RECEIVE, RECEIVE.c_str(), RECEIVE.size());
				memcpy(m_messageHead.MESSAGE_CREATE_TIME, MESSAGE_CREATE_TIME.c_str(), MESSAGE_CREATE_TIME.size());
				memcpy(m_messageHead.VERSION, strVERSION.c_str(), strVERSION.size());
			}

			if ((pNode = xmlRootElement->FirstChild("MESSAGE_LIST")) != NULL)
			{
				xmlListElement = pNode->ToElement();
				if ((pNode = xmlListElement->FirstChild("BILL_INFO")) != NULL)
				{
					string CUSTOMS_CODE = "";
					string I_E_FLAG = "";
					string SHIP_NAME_EN = "";
					string VOYAGE_NO = "";
					string BILL_NO   = "";
					string NOTE = "";

					xmlSubElement = pNode->ToElement() ;
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CUSTOMS_CODE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						CUSTOMS_CODE = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("I_E_FLAG");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						I_E_FLAG = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("SHIP_NAME_EN");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						SHIP_NAME_EN = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("VOYAGE_NO");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						VOYAGE_NO = pNodeTmp->ToElement()->GetText();

					//if (xmlSubElement != NULL)
					//	pNodeTmp = xmlSubElement->FirstChildElement("BILL_NO");
					//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					//	BILL_NO = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("NOTE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						NOTE = pNodeTmp->ToElement()->GetText();

					//// ¸³Öµ
					//memcpy(m_billInfo.CUSTOMS_CODE, CUSTOMS_CODE.c_str(), sizeof(m_billInfo.CUSTOMS_CODE) - 1);
					//memcpy(m_billInfo.I_E_FLAG, I_E_FLAG.c_str(), sizeof(m_billInfo.I_E_FLAG) - 1);
					//memcpy(m_billInfo.SHIP_NAME_EN, SHIP_NAME_EN.c_str(), sizeof(m_billInfo.SHIP_NAME_EN) - 1);
					//memcpy(m_billInfo.VOYAGE_NO, VOYAGE_NO.c_str(), sizeof(m_billInfo.VOYAGE_NO) - 1);
					//memcpy(m_billInfo.BILL_NO, BILL_NO.c_str(), sizeof(m_billInfo.BILL_NO) - 1);
					//memcpy(m_billInfo.NOTE, NOTE.c_str(), sizeof(m_billInfo.NOTE) - 1);

					// ¸³Öµ
					memcpy(m_billInfo.CUSTOMS_CODE, CUSTOMS_CODE.c_str(), CUSTOMS_CODE.size());
					memcpy(m_billInfo.I_E_FLAG, I_E_FLAG.c_str(), I_E_FLAG.size());
					memcpy(m_billInfo.SHIP_NAME_EN, SHIP_NAME_EN.c_str(), SHIP_NAME_EN.size());
					memcpy(m_billInfo.VOYAGE_NO, VOYAGE_NO.c_str(), VOYAGE_NO.size());
				}

			}

			if ((pNode = xmlRootElement->FirstChild("MESSAGE_LIST")) != NULL)
			{
				xmlListElement = pNode->ToElement();

				for (pNode = xmlListElement->FirstChild("CONTAINER_INFO"); pNode != NULL; pNode = pNode->NextSibling())
				{
					string CONTA_NO		= "";
					string CONTA_MODEL	= "";
					string CONTA_SIZE	= "";
					string NOTE			= "";

					xmlSubElement = pNode->ToElement() ;
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CONTA_NO");

					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						CONTA_NO = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CONTA_MODEL");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						CONTA_MODEL = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CONTA_SIZE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						CONTA_SIZE = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("NOTE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						NOTE = pNodeTmp->ToElement()->GetText();

					structCONTAINER_INFO_CLM10011 tmpContainerInfo = {0};

					//// ¸³Öµ
					//memcpy(tmpContainerInfo.CONTA_NO, CONTA_NO.c_str(), sizeof(tmpContainerInfo.CONTA_NO) - 1);
					//memcpy(tmpContainerInfo.CONTA_MODEL, CONTA_MODEL.c_str(), sizeof(tmpContainerInfo.CONTA_MODEL) - 1);
					//memcpy(tmpContainerInfo.CONTA_SIZE, CONTA_SIZE.c_str(), sizeof(tmpContainerInfo.CONTA_SIZE) - 1);
					//memcpy(tmpContainerInfo.NOTE, NOTE.c_str(), sizeof(tmpContainerInfo.NOTE) - 1);

					// ¸³Öµ
					memcpy(tmpContainerInfo.CONTA_NO, CONTA_NO.c_str(), CONTA_NO.size());
					memcpy(tmpContainerInfo.CONTA_MODEL, CONTA_MODEL.c_str(), CONTA_MODEL.size());
					memcpy(tmpContainerInfo.CONTA_SIZE, CONTA_SIZE.c_str(), CONTA_SIZE.size());


					m_contaInfoVect.push_back(tmpContainerInfo);
				}
			}
		}

		//// delete file
		//if (strFileName != "")
		//{
		//	CFile::Remove(strFileName.c_str());
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

int CCLM10051::SaveToDB()
{
	if (m_messageHead.MESSAGE_ID[0] == '\0')
		return 0;

	CRADatabase database;
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

		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return 0;
		}

		database.GetConn()->BeginTrans();
		if (m_messageHead.MESSAGE_ID[0] != '\0')
		{
			char I_E_FLAG = ' ';
			if (m_billInfo.I_E_FLAG[0] != '\0')
				I_E_FLAG = m_billInfo.I_E_FLAG[0];

			CString strSql = "";
			char szSqlBuffer[2048] = {0};
			sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_SHIP_DICTATE_BILL(MESSAGE_ID,CUSTOMS_CODE,I_E_FLAG,SHIP_NAME_EN,VOYAGE_NO,NOTE, \
														FUNCTION_CODE, MESSAGE_TYPE, SENDER, RECEIVER, CREATE_TIME, INPUT_TIME, SEND_FLAG, VERSION) \
														values('%s', '%s', '%c', '%s', '%s', '%s', \
														'%s', '%s', '%s', '%s', '%s', GetDate(), '%c', '%s')", \
														m_messageHead.MESSAGE_ID, m_billInfo.CUSTOMS_CODE, I_E_FLAG, m_billInfo.SHIP_NAME_EN, m_billInfo.VOYAGE_NO, m_billInfo.NOTE, \
														m_messageHead.FUNCTION_CODE, m_messageHead.MESSAGE_TYPE, m_messageHead.SENDER, m_messageHead.RECEIVE, m_messageHead.MESSAGE_CREATE_TIME, '1', m_messageHead.VERSION);

			strSql += szSqlBuffer;

			// Ö´ÐÐSqlÓï¾ä
			nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
			if (nRet == E_FAIL)
			{
				database.GetConn()->RollbackTrans();
				m_chOpFlag = 'F';
				m_chSEND_FLAG = '2';
				return 0;
			}
			//LOG1("insert into D_SHIP_DICTATE_BILL, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);
		}




		size_t nContaSize = m_contaInfoVect.size();
		for (size_t i = 0; i < nContaSize; ++i)
		{
			// single insert into table
			size_t iTmpIndex = i;
			CString strSql = "";
			char szSqlBuffer[1024] = {0};
			sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_SHIP_DICTATE_CONTA(MESSAGE_ID,CONTA_NO,CONTA_MODEL,CONTA_SIZE,SHIP_STATE, NOTE) \
														values('%s', '%s', '%s', '%s', '%c', '%s')", \
														m_messageHead.MESSAGE_ID, m_contaInfoVect[iTmpIndex].CONTA_NO, m_contaInfoVect[iTmpIndex].CONTA_MODEL, \
														m_contaInfoVect[iTmpIndex].CONTA_SIZE,  '1', m_contaInfoVect[iTmpIndex].NOTE);

			strSql += szSqlBuffer;

			// Ö´ÐÐSqlÓï¾ä
			nRet = database.RAExecuteNoRs((LPCTSTR)strSql);

			if (nRet == S_OK)
				m_chOpFlag = 'T';
			else
			{
				m_chOpFlag = 'F';
				m_chSEND_FLAG = '2';
				database.GetConn()->RollbackTrans();
				return 0;
			}
			//LOG2("insert into D_SHIP_DICTATE_CONTA, MESSAGE_ID=%s, CONTA_NO=%s", m_messageHead.MESSAGE_ID, m_contaInfoVect[iTmpIndex].CONTA_NO);
		}
		database.GetConn()->CommitTrans();
	}
	catch(...)
	{
		database.GetConn()->RollbackTrans();
		m_chOpFlag = 'F';
		m_chSEND_FLAG = '2';
		return 0;
	}

	m_chSEND_FLAG = '2';
	return 1;
}

void CCLM10051::SendResponse()
{
	LOG3("Enter CCLM10051::SendResponse, MessageType:%s, Sender:%s, Receiver:%s", m_messageHead.MESSAGE_TYPE, m_messageHead.SENDER, m_messageHead.RECEIVE);
	if (m_messageHead.MESSAGE_ID[0] == '\0')
		return;

	SelectMQ();

	CString MESSAGE_CREATE_TIME = "";
	MESSAGE_CREATE_TIME =COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	CString strResponse = "";

	std::string MESSAGE_TYPE = CMessageType::CLM10052;

	strResponse.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\n"
"<MESSAGE>\n"
"<MESSAGE_HEAD>\n"
"\t<MESSAGE_ID>%s</MESSAGE_ID>\n"
"\t<FUNCTION_CODE>%s</FUNCTION_CODE>\n"
"\t<MESSAGE_TYPE>%s</MESSAGE_TYPE>\n"
"\t<SENDER>%s</SENDER>\n"
"\t<RECEIVER>%s</RECEIVER>\n"
"\t<MESSAGE_CREATE_TIME>%s</MESSAGE_CREATE_TIME>\n"
"\t<VERSION>%s</VERSION>\n"
"</MESSAGE_HEAD>\n"
"<MESSAGE_LIST>\n"
"<RESPOND_INFO>\n"
"\t<MESSAGE_ID>%s</MESSAGE_ID>\n"
"\t<OP_FLAG>%c</OP_FLAG>\n"
"\t<NOTE>%s</NOTE>\n"
"</RESPOND_INFO>\n"
"</MESSAGE_LIST>\n"
"</MESSAGE>\n", 
m_messageHead.MESSAGE_ID, m_messageHead.FUNCTION_CODE, MESSAGE_TYPE.c_str(), m_messageHead.RECEIVE, \
m_messageHead.SENDER, MESSAGE_CREATE_TIME, m_messageHead.VERSION, m_messageHead.MESSAGE_ID, m_chOpFlag, m_strNote.c_str());

	// ·¢ËÍMSMQ
	//CMSMQApiWrapper ob;
	std::string szLabel = CMessageType::CLM10052;
	CString strMessage = strResponse;
	//if ((!m_strMQIp.empty()) && (!m_strQueueName.empty()))
	//	int nRet = ob.SendStreamMessage(m_strMQIp.c_str(), m_strQueueName.c_str(), szLabel.c_str(), szLabel.c_str(), strMessage);

	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(m_strMQIp, m_strQueueName, szLabel, szLabel));
	apBasic->SendMessage((LPCTSTR)strMessage, strMessage.GetLength());

}

int CCLM10051::UpdateDB()
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

	//		// Ö´ÐÐSqlÓï¾ä
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

std::string CCLM10051::GetFileName()
{
	return m_strFileName;
}

int CCLM10051::SelectMQ()
{
	if (m_messageHead.MESSAGE_ID[0] == '\0')
		return 0;

	try
	{
		CString strMQPath = "";
		std::string MQMessageType = CMessageType::CLM10052;
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPath from SITE_MQPATH_CONFIG where MQMessageType='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='ALL' and InOrOutFlag='0'", m_messageHead.SENDER);

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
