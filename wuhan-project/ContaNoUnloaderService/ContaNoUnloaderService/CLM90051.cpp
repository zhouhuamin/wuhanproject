#include "StdAfx.h"
#include "CLM90051.h"
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

CCLM90051::CCLM90051(void) : m_messageHead(), m_clm90051(), m_chOpFlag('F'), \
m_chSEND_FLAG('1'), m_strNote("")
{
}

CCLM90051::~CCLM90051(void)
{
}

void CCLM90051::InitConfigInfo(structConfigInfo *pConfig)
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

void CCLM90051::SaveFileName(const std::string &strFileName)
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

void CCLM90051::ParseMessage()
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

				//// 赋值
				//memcpy(m_messageHead.MESSAGE_ID, MESSAGE_ID.c_str(), sizeof(m_messageHead.MESSAGE_ID) - 1);
				//memcpy(m_messageHead.FUNCTION_CODE, FUNCTION_CODE.c_str(), sizeof(m_messageHead.FUNCTION_CODE) - 1);
				//memcpy(m_messageHead.MESSAGE_TYPE, MESSAGE_TYPE.c_str(), sizeof(m_messageHead.MESSAGE_TYPE) - 1);
				//memcpy(m_messageHead.SENDER, SENDER.c_str(), sizeof(m_messageHead.SENDER) - 1);
				//memcpy(m_messageHead.RECEIVE, RECEIVE.c_str(), sizeof(m_messageHead.RECEIVE) - 1);
				//memcpy(m_messageHead.MESSAGE_CREATE_TIME, MESSAGE_CREATE_TIME.c_str(), sizeof(m_messageHead.MESSAGE_CREATE_TIME) - 1);
				//memcpy(m_messageHead.VERSION, strVERSION.c_str(), sizeof(m_messageHead.VERSION) - 1);

				// 赋值
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
				if ((pNode = xmlListElement->FirstChild("PORT_INFO")) != NULL)
				{
					xmlSubElement = pNode->ToElement() ;

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("PORT_ID");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.PORT_ID = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("PORT_CODE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.PORT_CODE = pNodeTmp->ToElement()->GetText();

					std::string PORT_NAME = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("PORT_NAME");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						PORT_NAME = pNodeTmp->ToElement()->GetText();

					char szNote[256] = {0};
					char *pTmp = szNote;
					memcpy(szNote, PORT_NAME.c_str(), PORT_NAME.size() < 256 ? PORT_NAME.size() : 255);
					UTF8ToGBK(pTmp);
					szNote[255] = '\0';
					m_clm90051.PORT_NAME = szNote;
					
					std::string PLACE_NAME = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("PLACE_NAME");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						PLACE_NAME = pNodeTmp->ToElement()->GetText();

					memset(szNote, 0x00, sizeof(szNote));
					pTmp = szNote;
					memcpy(szNote, PLACE_NAME.c_str(), PLACE_NAME.size() < 256 ? PLACE_NAME.size() : 255);
					UTF8ToGBK(pTmp);
					szNote[255] = '\0';
					m_clm90051.PLACE_NAME = szNote;					
					
					
					std::string CORPOR = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CORPOR");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						CORPOR = pNodeTmp->ToElement()->GetText();
						
					memset(szNote, 0x00, sizeof(szNote));
					pTmp = szNote;
					memcpy(szNote, CORPOR.c_str(), CORPOR.size() < 256 ? CORPOR.size() : 255);
					UTF8ToGBK(pTmp);
					szNote[255] = '\0';
					m_clm90051.CORPOR = szNote;						
					
					

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CORPOR_CONTACT");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.CORPOR_CONTACT = pNodeTmp->ToElement()->GetText();
						
					std::string REG_AUTH = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("REG_AUTH");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						REG_AUTH = pNodeTmp->ToElement()->GetText();
						
					memset(szNote, 0x00, sizeof(szNote));
					pTmp = szNote;
					memcpy(szNote, REG_AUTH.c_str(), REG_AUTH.size() < 256 ? REG_AUTH.size() : 255);
					UTF8ToGBK(pTmp);
					szNote[255] = '\0';
					m_clm90051.REG_AUTH = szNote;							
						
						


					std::string IS_AVAIL = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("IS_AVAIL");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						IS_AVAIL = pNodeTmp->ToElement()->GetText();

					if (IS_AVAIL != "")
					{
						m_clm90051.IS_AVAIL = IS_AVAIL[0];
					}


					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("AVAIL_TIME");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.AVAIL_TIME = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("GATEWAY_IP");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.GATEWAY_IP = pNodeTmp->ToElement()->GetText();

					std::string GATEWAY_PORT = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("GATEWAY_PORT");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						GATEWAY_PORT = pNodeTmp->ToElement()->GetText();
						
					m_clm90051.GATEWAY_PORT = atoi(GATEWAY_PORT.c_str());
					
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CONTACT_CODE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.CONTACT_CODE = pNodeTmp->ToElement()->GetText();		

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CUSTOMS_CODE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.CUSTOMS_CODE = pNodeTmp->ToElement()->GetText();		

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("PASS_WORD");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.PASS_WORD = pNodeTmp->ToElement()->GetText();	

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CONTENT");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90051.CONTENT = pNodeTmp->ToElement()->GetText();						
				}
			}
		}

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

int CCLM90051::SaveToDB()
{
	LOG("Enter CCLM90051::SaveToDB");

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

		CString strSql = "";
		char szSqlBuffer[2048] = {0};

		if (m_messageHead.FUNCTION_CODE[0] == '2')
		{
			if (m_messageHead.MESSAGE_ID[0] != '\0')
			{
				char IS_AVAIL = ' ';
				if (m_clm90051.IS_AVAIL != '\0')
					IS_AVAIL = m_clm90051.IS_AVAIL;

				sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into P_PORT_CORP_INFO(PORT_ID,PORT_CODE,PORT_NAME,PLACE_NAME,CORPOR,\
															CORPOR_CONTACT,REG_AUTH, REG_TIME, AVAIL_TIME, IS_AVAIL, \
															GATEWAY_IP, GATEWAY_PORT,CONTACT_CODE,CUSTOMS_CODE,PASS_WORD,CONTENT) \
															values('%s', '%s', '%s', '%s', '%s', '%s', '%s', GetDate(),'%s', '%c', \
															'%s', %d, '%s', '%s', '%s','%s')", \
															m_clm90051.PORT_ID.c_str(),
															m_clm90051.PORT_CODE.c_str(),
															m_clm90051.PORT_NAME.c_str(),
															m_clm90051.PLACE_NAME.c_str(),
															m_clm90051.CORPOR.c_str(),
															m_clm90051.CORPOR_CONTACT.c_str(),
															m_clm90051.REG_AUTH.c_str(),
															m_clm90051.AVAIL_TIME.c_str(),
															IS_AVAIL,
															m_clm90051.GATEWAY_IP.c_str(),
															m_clm90051.GATEWAY_PORT,
															m_clm90051.CONTACT_CODE.c_str(),
															m_clm90051.CUSTOMS_CODE.c_str(),
															m_clm90051.PASS_WORD.c_str(),
															m_clm90051.CONTENT.c_str());

				strSql += szSqlBuffer;

				// 执行Sql语句
				nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
				if (nRet == S_OK)
					m_chOpFlag = 'T';
				else
				{
					m_chOpFlag = 'F';
					m_chSEND_FLAG = '2';
					return 0;
				}
				LOG1("insert into P_PORT_CORP_INFO, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);
			}
		}
		else if (m_messageHead.FUNCTION_CODE[0] == '5')
		{
			if (m_messageHead.MESSAGE_ID[0] != '\0')
			{
				char IS_AVAIL = ' ';
				if (m_clm90051.IS_AVAIL != '\0')
					IS_AVAIL = m_clm90051.IS_AVAIL;

				sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "update P_PORT_CORP_INFO set \
PORT_CODE = '%s',\
PORT_NAME = '%s',\
PLACE_NAME = '%s',\
CORPOR = '%s',\
CORPOR_CONTACT = '%s',\
REG_AUTH = '%s', \
AVAIL_TIME = '%s', \
IS_AVAIL = '%c', \
GATEWAY_IP = '%s', \
GATEWAY_PORT = %d, \
CONTACT_CODE = '%s',	\
CUSTOMS_CODE = '%s',	\
PASS_WORD = '%s',	\
CONTENT = '%s' where PORT_ID = '%s'", \
m_clm90051.PORT_CODE.c_str(),
m_clm90051.PORT_NAME.c_str(),
m_clm90051.PLACE_NAME.c_str(),
m_clm90051.CORPOR.c_str(),
m_clm90051.CORPOR_CONTACT.c_str(),
m_clm90051.REG_AUTH.c_str(),
m_clm90051.AVAIL_TIME.c_str(),
IS_AVAIL,
m_clm90051.GATEWAY_IP.c_str(),
m_clm90051.GATEWAY_PORT,
m_clm90051.CONTACT_CODE.c_str(),
m_clm90051.CUSTOMS_CODE.c_str(),
m_clm90051.PASS_WORD.c_str(),
m_clm90051.CONTENT.c_str(),
m_clm90051.PORT_ID.c_str());

				strSql += szSqlBuffer;

				// 执行Sql语句
				nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
				if (nRet == S_OK)
					m_chOpFlag = 'T';
				else
				{
					m_chOpFlag = 'F';
					m_chSEND_FLAG = '2';
					return 0;
				}
				LOG1("update P_PORT_CORP_INFO, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);
			}
		}
		else
		{
			m_chOpFlag = 'F';
			m_chSEND_FLAG = '2';
		}



	}
	catch(...)
	{
		return 0;
	}

	m_chSEND_FLAG = '2';
	return 1;
}

void CCLM90051::SendResponse()
{
	LOG1("Enter CCLM90051::SendResponse,m_messageHead.SENDER=%s", m_messageHead.SENDER);
	if (m_messageHead.MESSAGE_ID[0] == '\0')
		return;

	SelectMQ();

	CString MESSAGE_CREATE_TIME = "";
	MESSAGE_CREATE_TIME =COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	CString strResponse = "";

	std::string MESSAGE_TYPE = CMessageType::CLM90052;

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

	// 发送MSMQ
	//CMSMQApiWrapper ob;

	std::string szLabel = "WEB-";
	szLabel += CMessageType::CLM90052;
	//std::string szLabel = CMessageType::CLM90052;
	CString strMessage = strResponse;
	//if ((!m_strMQIp.empty()) && (!m_strQueueName.empty()))
	//	int nRet = ob.SendStreamMessage(m_strMQIp.c_str(), m_strQueueName.c_str(), szLabel.c_str(), szLabel.c_str(), strMessage);

	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(m_strMQIp, m_strQueueName, szLabel, szLabel));
	apBasic->SendMessage((LPCTSTR)strMessage, strMessage.GetLength());

	//std::string strPreFlag = "";
	//// WEB应用发送
	//if (strPreFlag == "WEB")
	//{
	//	boost::shared_array<char> spXmlData;
	//	int nXmlLen = 0;

	//	CStdioFile file;
	//	if (!file.Open(m_strFileName.c_str(), CFile::modeRead|CFile::typeBinary))
	//	{
	//		return;
	//	}
	//	DWORD iLen = (DWORD)file.GetLength();
	//	if (iLen <= 0)
	//	{
	//		file.Close();
	//		return;
	//	}

	//	spXmlData.reset(new char[iLen + 1]);
	//	file.Seek(0,CFile::begin);
	//	file.Read(spXmlData.get(), iLen);
	//	file.Close();	
	//	spXmlData[iLen] = '\0';


	//	CStringArray queueNameArray;
	//	CStringArray mqIPArray;
	//	SelectMQForWeb(queueNameArray, mqIPArray);

	//	for (int i = 0, j = 0; i < queueNameArray.GetSize() && j < mqIPArray.GetSize(); ++i,++j)
	//	{
	//		if (i != j)
	//			break;
	//		// 发送MSMQ
	//		std::string strQueueName	= (LPCTSTR)queueNameArray[j];
	//		std::string strMQIp			= (LPCTSTR)mqIPArray[i];
	//		std::string szLabel			= "WEB-";
	//		szLabel += CMessageType::CLM90031;
	//		std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	//		apBasic->SendMessage(spXmlData.get(), iLen);
	//	}

	//	// delete file
	//	if (m_strFileName != "")
	//	{
	//		CFile::Remove(m_strFileName.c_str());
	//	}

	//	return;
	//}

	//boost::shared_array<char> spXmlData;
	//int nXmlLen = 0;

	//CStdioFile file;
	//if (!file.Open(m_strFileName.c_str(), CFile::modeRead|CFile::typeBinary))
	//{
	//	return;
	//}
	//DWORD iLen = (DWORD)file.GetLength();
	//if (iLen <= 0)
	//{
	//	file.Close();
	//	return;
	//}

	//spXmlData.reset(new char[iLen + 1]);
	//file.Seek(0,CFile::begin);
	//file.Read(spXmlData.get(), iLen);
	//file.Close();	
	//spXmlData[iLen] = '\0';


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
	//	std::string szLabel			= CMessageType::CLM90031;
	//	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	//	apBasic->SendMessage(spXmlData.get(), iLen);
	//}

	//// delete file
	//if (m_strFileName != "")
	//{
	//	CFile::Remove(m_strFileName.c_str());
	//}

}

int CCLM90051::UpdateDB()
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

std::string CCLM90051::GetFileName()
{
	return m_strFileName;
}

int CCLM90051::SelectMQ()
{
	if (m_messageHead.MESSAGE_ID[0] == '\0')
		return 0;

	try
	{
		CString strMQPath = "";
		std::string MQMessageType = CMessageType::CLM90052;
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPath from SITE_MQPATH_CONFIG where MQMessageType='ALL' and CUSTOMS_CODE='ALL' and ENTERPRISE_CODE='ALL' and InOrOutFlag='0'");

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

int CCLM90051::SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='CUSTOM' and INOROUTFLAG='1'", m_messageHead.RECEIVE);

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

void CCLM90051::UTF8ToGBK( char *&szOut )
{
	unsigned short *wszGBK;
	char *szGBK;
	//长度
	int len = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szOut, -1, NULL, 0);
	wszGBK = new unsigned short[len+1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)szOut, -1, (LPWSTR)wszGBK, len);

	//长度
	len = WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, NULL, 0, NULL, NULL);
	szGBK = new char[len+1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, (LPWSTR)wszGBK, -1, szGBK, len, NULL, NULL);

	//szOut = szGBK; //这样得到的szOut不正确，因为此句意义是将szGBK的首地址赋给szOut，当delete []szGBK执行后szGBK的内

	//存空间将被释放，此时将得不到szOut的内容

	memset(szOut,'\0',strlen(szGBK)+1); //改将szGBK的内容赋给szOut ，这样即使szGBK被释放也能得到正确的值
	memcpy(szOut,szGBK,strlen(szGBK));


	delete []szGBK;
	delete []wszGBK;
}

