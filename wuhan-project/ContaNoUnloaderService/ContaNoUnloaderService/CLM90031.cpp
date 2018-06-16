#include "StdAfx.h"
#include "CLM90031.h"
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

CCLM90031::CCLM90031(void) : m_messageHead(), m_clm90031(), m_chOpFlag('F'), \
m_chSEND_FLAG('1'), m_strNote("")
{
}

CCLM90031::~CCLM90031(void)
{
}

void CCLM90031::InitConfigInfo(structConfigInfo *pConfig)
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

void CCLM90031::SaveFileName(const std::string &strFileName)
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

void CCLM90031::ParseMessage()
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
		TiXmlElement* xmlCOMPANY_INFOElement	= NULL;
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
				if ((pNode = xmlListElement->FirstChild("COMPANY_INFO")) != NULL)
				{
					xmlSubElement = pNode->ToElement() ;

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CORP_CODE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.CORP_CODE = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CORP_ID");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.CORP_ID = pNodeTmp->ToElement()->GetText();

					std::string CORP_NAME = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CORP_NAME");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						CORP_NAME = pNodeTmp->ToElement()->GetText();

					char szNote[256] = {0};
					char *pTmp = szNote;
					memcpy(szNote, CORP_NAME.c_str(), CORP_NAME.size() < 256 ? CORP_NAME.size() : 255);
					UTF8ToGBK(pTmp);
					szNote[255] = '\0';
					m_clm90031.CORP_NAME = szNote;

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("LEGAL_REPRESENT");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.LEGAL_REPRESENT = pNodeTmp->ToElement()->GetText();


					std::string CORP_TYPE = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CORP_TYPE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						CORP_TYPE = pNodeTmp->ToElement()->GetText();

					if (CORP_TYPE != "")
					{
						m_clm90031.CORP_TYPE = CORP_TYPE[0];
					}


					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CORP_ADDR");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.CORP_ADDR = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("FAX_NUM");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.FAX_NUM = pNodeTmp->ToElement()->GetText();


					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("TEL_NUM");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.TEL_NUM = pNodeTmp->ToElement()->GetText();


					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("POST_CODE");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.POST_CODE = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("CONTACT");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.CONTACT = pNodeTmp->ToElement()->GetText();

					//if (xmlSubElement != NULL)
					//	pNodeTmp = xmlSubElement->FirstChildElement("CUSTOM_ATTACH");
					//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					//	m_clm90031.CUSTOM_ATTACH = pNodeTmp->ToElement()->GetText();

					std::string IS_AVAIL = "";
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("IS_AVAIL");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						IS_AVAIL = pNodeTmp->ToElement()->GetText();

					if (IS_AVAIL != "")
					{
						m_clm90031.IS_AVAIL = IS_AVAIL[0];
					}


					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("AVAIL_TIME");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.AVAIL_TIME = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("REG_AUTH");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_clm90031.REG_AUTH = pNodeTmp->ToElement()->GetText();


					if ((pNode = xmlRootElement->FirstChild("MESSAGE_LIST")) != NULL)
					{
						xmlListElement = pNode->ToElement();
						if ((pNode = xmlListElement->FirstChild("COMPANY_INFO")) != NULL)
						{
							string CONTA_NO		= "";
							string CONTA_MODEL	= "";
							string CONTA_SIZE	= "";
							string NOTE			= "";

							xmlCOMPANY_INFOElement = pNode->ToElement() ;
							if (xmlCOMPANY_INFOElement != NULL)
							{
								TiXmlNode *pPORT_IDNode = NULL;
								for (pPORT_IDNode = xmlCOMPANY_INFOElement->FirstChild("PORT_ID"); pPORT_IDNode != NULL; pPORT_IDNode = pPORT_IDNode->NextSibling("PORT_ID"))
								{
									std::string PORT_ID = "";
									xmlSubElement = pPORT_IDNode->ToElement() ;
									if (pPORT_IDNode != NULL && pPORT_IDNode->ToElement() != NULL && pPORT_IDNode->ToElement()->GetText() != 0)
									{
										PORT_ID = pPORT_IDNode->ToElement()->GetText();
										m_clm90031.PORT_ID.push_back(PORT_ID);
									}									
								}					
							}
						}
					}


					//if (xmlSubElement != NULL)
					//	pNodeTmp = xmlSubElement->FirstChildElement("COMPANY_ID");
					//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					//	m_clm90031.COMPANY_ID = pNodeTmp->ToElement()->GetText();

					//if (xmlSubElement != NULL)
					//	pNodeTmp = xmlSubElement->FirstChildElement("COMPANY_NAME");
					//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					//	m_clm90031.COMPANY_NAME = pNodeTmp->ToElement()->GetText();

					//if (xmlSubElement != NULL)
					//	pNodeTmp = xmlSubElement->FirstChildElement("COMPANY_ADDRESS");
					//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					//	m_clm90031.COMPANY_ADDRESS = pNodeTmp->ToElement()->GetText();

					//if (xmlSubElement != NULL)
					//	pNodeTmp = xmlSubElement->FirstChildElement("LEGAL");
					//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					//	m_clm90031.LEGAL = pNodeTmp->ToElement()->GetText();

					//if (xmlSubElement != NULL)
					//	pNodeTmp = xmlSubElement->FirstChildElement("LEGAL_PHONE");
					//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					//	m_clm90031.LEGAL_PHONE = pNodeTmp->ToElement()->GetText();

					//if (xmlSubElement != NULL)
					//	pNodeTmp = xmlSubElement->FirstChildElement("NOTE");
					//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					//	m_clm90031.NOTE = pNodeTmp->ToElement()->GetText();
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

int CCLM90031::SaveToDB()
{
	LOG("Enter CCLM90031::SaveToDB");

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

		CString strSql = "";
		char szSqlBuffer[2048] = {0};

		database.GetConn()->BeginTrans();
		if (m_messageHead.FUNCTION_CODE[0] == '2')
		{
			if (m_messageHead.MESSAGE_ID[0] != '\0')
			{
				char CORP_TYPE = ' ';
				if (m_clm90031.CORP_TYPE != '\0')
					CORP_TYPE = m_clm90031.CORP_TYPE;

				char IS_AVAIL = ' ';
				if (m_clm90031.IS_AVAIL != '\0')
					IS_AVAIL = m_clm90031.IS_AVAIL;

				//CString CORP_ID = "";
				//CString FAX_NUM = "";
				//CString POST_CODE = "";
				//CString CONTACT	= "";
				//CString CUSTOM_ATTACH = "";
				//CString AVAIL_TIME = "";

				sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into P_ENTERPRISE_INFO(CORP_CODE,CORP_ID,CORP_NAME,LEGAL_REPRESENT,CORP_TYPE,CORP_ADDR,FAX_NUM, \
															TEL_NUM, POST_CODE, CONTACT, AVAIL_TIME, REG_TIME,IS_AVAIL,REG_AUTH) \
															values('%s', '%s', '%s', '%s', '%c', '%s', '%s', \
															'%s', '%s', '%s', '%s',  GetDate(),'%c','%s')", \
															m_clm90031.CORP_CODE.c_str(),
															m_clm90031.CORP_ID.c_str(),
															m_clm90031.CORP_NAME.c_str(),
															m_clm90031.LEGAL_REPRESENT.c_str(),
															CORP_TYPE,
															m_clm90031.CORP_ADDR.c_str(),
															m_clm90031.FAX_NUM.c_str(),
															m_clm90031.TEL_NUM.c_str(),
															m_clm90031.POST_CODE.c_str(),
															m_clm90031.CONTACT.c_str(),
															m_clm90031.AVAIL_TIME.c_str(),
															IS_AVAIL,
															m_clm90031.REG_AUTH.c_str());



				//			sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into P_ENTERPRISE_INFO(CORP_CODE,CORP_ID,CORP_NAME,LEGAL_REPRESENT,CORP_TYPE,CORP_ADDR,FAX_NUM, \
				//														TEL_NUM, POST_CODE, CONTACT, CUSTOM_ATTACH, AVAIL_TIME, REG_TIME) \
				//values('%s', '%s', '%s', '%s', '%c', '%s', '%s', \
				//'%s', '%s', '%s', '%s', '%s',  GetDate())", \
				//m_clm90031.COMPANY_ID.c_str(),
				//CORP_ID,
				//m_clm90031.COMPANY_NAME.c_str(),
				//m_clm90031.LEGAL.c_str(),
				//CORP_TYPE,
				//m_clm90031.COMPANY_ADDRESS.c_str(),
				//FAX_NUM,
				//m_clm90031.LEGAL_PHONE.c_str(),
				//POST_CODE,
				//CONTACT,
				//CUSTOM_ATTACH,
				//AVAIL_TIME);

				strSql += szSqlBuffer;

				// 执行Sql语句
				nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
				if (nRet == S_OK)
					m_chOpFlag = 'T';
				else
				{
					database.GetConn()->RollbackTrans();
					m_chOpFlag = 'F';
					m_chSEND_FLAG = '2';
					return 0;
				}
				LOG1("insert into P_ENTERPRISE_INFO, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);


				for (size_t i = 0; i < m_clm90031.PORT_ID.size(); ++i)
				{
sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into P_ENTERPRISE_PORT_RELATION(CORP_ID,PORT_ID) values('%s', '%s')", m_clm90031.CORP_ID.c_str(),m_clm90031.PORT_ID[i].c_str());

					strSql = szSqlBuffer;

					// 执行Sql语句
					nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
					if (nRet == E_FAIL)
					{
						database.GetConn()->RollbackTrans();
						m_chOpFlag = 'F';
						m_chSEND_FLAG = '2';
						return 0;
					}
				}

				LOG1("insert into P_ENTERPRISE_INFO, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);


			}
		}
		else if (m_messageHead.FUNCTION_CODE[0] == '5')
		{
			if (m_messageHead.MESSAGE_ID[0] != '\0')
			{
				char CORP_TYPE = ' ';
				if (m_clm90031.CORP_TYPE != '\0')
					CORP_TYPE = m_clm90031.CORP_TYPE;

				char IS_AVAIL = ' ';
				if (m_clm90031.IS_AVAIL != '\0')
					IS_AVAIL = m_clm90031.IS_AVAIL;

				sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "update P_ENTERPRISE_INFO set \
CORP_CODE = '%s',\
CORP_NAME = '%s',\
LEGAL_REPRESENT = '%s',\
CORP_TYPE = '%c',\
CORP_ADDR = '%s',\
FAX_NUM = '%s', \
TEL_NUM = '%s', \
POST_CODE = '%s', \
CONTACT = '%s', \
AVAIL_TIME = '%s', \
IS_AVAIL = '%c',	\
REG_AUTH = '%s' where CORP_ID = '%s'", \
m_clm90031.CORP_CODE.c_str(),
m_clm90031.CORP_NAME.c_str(),
m_clm90031.LEGAL_REPRESENT.c_str(),
CORP_TYPE,
m_clm90031.CORP_ADDR.c_str(),
m_clm90031.FAX_NUM.c_str(),
m_clm90031.TEL_NUM.c_str(),
m_clm90031.POST_CODE.c_str(),
m_clm90031.CONTACT.c_str(),
m_clm90031.AVAIL_TIME.c_str(),
IS_AVAIL,
m_clm90031.REG_AUTH.c_str(),
m_clm90031.CORP_ID.c_str());

				strSql += szSqlBuffer;

				// 执行Sql语句
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
				LOG1("update P_ENTERPRISE_INFO, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);

				sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "delete from P_ENTERPRISE_PORT_RELATION where CORP_ID='%s'", m_clm90031.CORP_ID.c_str());

				strSql = szSqlBuffer;

				// 执行Sql语句
				nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
				if (nRet == E_FAIL)
				{
					database.GetConn()->RollbackTrans();
					m_chOpFlag = 'F';
					m_chSEND_FLAG = '2';
					return 0;
				}
				LOG1("delete P_ENTERPRISE_PORT_RELATION, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);

				for (size_t i = 0; i < m_clm90031.PORT_ID.size(); ++i)
				{
					sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into P_ENTERPRISE_PORT_RELATION(CORP_ID,PORT_ID) values('%s', '%s')", m_clm90031.CORP_ID.c_str(),m_clm90031.PORT_ID[i].c_str());

					strSql = szSqlBuffer;

					// 执行Sql语句
					nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
					if (nRet == E_FAIL)
					{
						database.GetConn()->RollbackTrans();
						m_chOpFlag = 'F';
						m_chSEND_FLAG = '2';
						return 0;
					}
				}

				LOG1("insert into P_ENTERPRISE_INFO, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);

			}
		}
		else
		{			
			m_chOpFlag = 'F';
			m_chSEND_FLAG = '2';
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

void CCLM90031::SendResponse()
{
	LOG1("Enter CCLM90031::SendResponse,m_messageHead.SENDER=%s", m_messageHead.SENDER);
	if (m_messageHead.MESSAGE_ID[0] == '\0')
		return;

	SelectMQ();

	CString MESSAGE_CREATE_TIME = "";
	MESSAGE_CREATE_TIME =COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	CString strResponse = "";

	std::string MESSAGE_TYPE = CMessageType::CLM90032;

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
	szLabel += CMessageType::CLM90032;
	//std::string szLabel = CMessageType::CLM90032;
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

int CCLM90031::UpdateDB()
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

std::string CCLM90031::GetFileName()
{
	return m_strFileName;
}

int CCLM90031::SelectMQ()
{
	if (m_messageHead.MESSAGE_ID[0] == '\0')
		return 0;

	try
	{
		CString strMQPath = "";
		std::string MQMessageType = CMessageType::CLM90032;
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

int CCLM90031::SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray)
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

void CCLM90031::UTF8ToGBK( char *&szOut )
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

