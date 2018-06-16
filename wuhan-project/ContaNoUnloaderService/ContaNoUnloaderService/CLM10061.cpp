#include "StdAfx.h"
#include "CLM10061.h"
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

CCLM10061::CCLM10061(void) : m_messageHead(), m_billInfo(), m_respondInfo(), m_chOpFlag('F'), \
m_chSEND_FLAG('1'), m_strNote(""),m_contaInfoVect(0)
{
}

CCLM10061::~CCLM10061(void)
{
}

void CCLM10061::InitConfigInfo(structConfigInfo *pConfig)
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

void CCLM10061::SaveFileName(const std::string &strFileName)
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

void CCLM10061::ParseMessage()
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

					char szNote[256] = {0};
					char *pTmp = szNote;
					memcpy(szNote, NOTE.c_str(), NOTE.size() < 256 ? NOTE.size() : 255);
					UTF8ToGBK(pTmp);
					szNote[255] = '\0';
					NOTE = szNote;

					// 赋值
					memcpy(m_billInfo.CUSTOMS_CODE, CUSTOMS_CODE.c_str(), CUSTOMS_CODE.size());
					memcpy(m_billInfo.I_E_FLAG, I_E_FLAG.c_str(), I_E_FLAG.size());
					memcpy(m_billInfo.SHIP_NAME_EN, SHIP_NAME_EN.c_str(), SHIP_NAME_EN.size());
					memcpy(m_billInfo.VOYAGE_NO, VOYAGE_NO.c_str(), VOYAGE_NO.size());
					//memcpy(m_billInfo.BILL_NO, BILL_NO.c_str(), sizeof(m_billInfo.BILL_NO) - 1);
					memcpy(m_billInfo.NOTE, NOTE.c_str(), NOTE.size());
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

					char szNote[256] = {0};
					char *pTmp = szNote;
					memcpy(szNote, NOTE.c_str(), NOTE.size() < 256 ? NOTE.size() : 255);
					UTF8ToGBK(pTmp);
					szNote[255] = '\0';
					NOTE = szNote;

					structCONTAINER_INFO_CLM10011 tmpContainerInfo = {0};

					// 赋值
					memcpy(tmpContainerInfo.CONTA_NO, CONTA_NO.c_str(), CONTA_NO.size());
					memcpy(tmpContainerInfo.CONTA_MODEL, CONTA_MODEL.c_str(), CONTA_MODEL.size());
					memcpy(tmpContainerInfo.CONTA_SIZE, CONTA_SIZE.c_str(), CONTA_SIZE.size());
					memcpy(tmpContainerInfo.NOTE, NOTE.c_str(), NOTE.size());


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

int CCLM10061::SaveToDB()
{
	LOG("Enter CCLM10061::SaveToDB");
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
	//		sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_RELEASE_DICTATE_BILL(MESSAGE_ID,CUSTOMS_CODE,I_E_FLAG,SHIP_NAME_EN,VOYAGE_NO,BILL_NO,NOTE, \
	//													FUNCTION_CODE, MESSAGE_TYPE, SENDER, RECEIVER, CREATE_TIME, INPUT_TIME, VERSION) \
	//													values('%s', '%s', '%c', '%s', '%s', '%s', '%s', \
	//													'%s', '%s', '%s', '%s', '%s', GetDate(), '%s')", \
	//													m_messageHead.MESSAGE_ID, m_billInfo.CUSTOMS_CODE, m_billInfo.I_E_FLAG[0], m_billInfo.SHIP_NAME_EN, m_billInfo.VOYAGE_NO, m_billInfo.BILL_NO, m_billInfo.NOTE, \
	//													m_messageHead.FUNCTION_CODE, m_messageHead.MESSAGE_TYPE, m_messageHead.SENDER, m_messageHead.RECEIVE, m_messageHead.MESSAGE_CREATE_TIME, m_messageHead.VERSION);

	//		strSql += szSqlBuffer;

	//		// 执行Sql语句
	//		database.RAExecuteNoRs((LPCTSTR)strSql);
	//		LOG1("insert into D_RELEASE_DICTATE_BILL, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);
	//	}




	//	size_t nContaSize = m_contaInfoVect.size();
	//	for (size_t i = 0; i < nContaSize; ++i)
	//	{
	//		// single insert into table
	//		size_t iTmpIndex = i;
	//		CString strSql = "";
	//		char szSqlBuffer[1024] = {0};
	//		sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_RELEASE_DICTATE_CONTA(MESSAGE_ID,CONTA_NO,CONTA_MODEL,CONTA_SIZE,NOTE) \
	//													values('%s', '%s', '%s', '%s', '%s')", \
	//													m_messageHead.MESSAGE_ID, m_contaInfoVect[iTmpIndex].CONTA_NO, m_contaInfoVect[iTmpIndex].CONTA_MODEL, \
	//													m_contaInfoVect[iTmpIndex].CONTA_SIZE, m_contaInfoVect[iTmpIndex].NOTE);

	//		strSql += szSqlBuffer;

	//		// 执行Sql语句
	//		HRESULT hRet = database.RAExecuteNoRs((LPCTSTR)strSql);
	//		if (hRet == S_OK)
	//			m_chOpFlag = 'T';
	//		else
	//			m_chOpFlag = 'F';
	//		LOG2("insert into D_RELEASE_DICTATE_CONTA, MESSAGE_ID=%s, CONTA_NO=%s", m_messageHead.MESSAGE_ID, m_contaInfoVect[iTmpIndex].CONTA_NO);
	//	}
	//}
	//catch(...)
	//{
	//	return 0;
	//}

	//m_chSEND_FLAG = '2';
	return 1;
}

void CCLM10061::SendResponse()
{
	//std::string strPreFlag = "";
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
	//	std::string szLabel			= CMessageType::CLM10061;
	//	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	//	apBasic->SendMessage(spXmlData.get(), iLen);
	//}

	//// delete file
	//if (m_strFileName != "")
	//{
	//	CFile::Remove(m_strFileName.c_str());
	//}

}

int CCLM10061::UpdateDB()
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

std::string CCLM10061::GetFileName()
{
	return m_strFileName;
}

int CCLM10061::SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='PROXY' and INOROUTFLAG='1'", m_messageHead.SENDER);

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

void CCLM10061::UTF8ToGBK( char *&szOut )
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

