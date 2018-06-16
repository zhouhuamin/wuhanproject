#include "StdAfx.h"
#include "MTXXXX.h"

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


CMTXXXX::CMTXXXX(void) : m_chOpFlag('F'), m_chSEND_FLAG('1'), m_strNote(""),m_mtResponse()
{
}

CMTXXXX::~CMTXXXX(void)
{
}

void CMTXXXX::InitConfigInfo(structConfigInfo *pConfig)
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

void CMTXXXX::SaveFileName(const std::string &strFileName)
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
//int CMTXXXX::SaveLocal(BYTE *pMessage, int iLen)
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

void CMTXXXX::ParseMessage()
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
		TiXmlElement* xmlRootElement		= NULL;
		TiXmlElement* xmlRootElement2		= NULL;
		TiXmlElement* xmlResponseElement 	= NULL;
		TiXmlElement* xmlSubElement			= NULL;
		TiXmlElement* xmlSonElement			= NULL;
		TiXmlElement *xmlConsignmentElement = NULL;


		TiXmlNode * pNode					= NULL;
		TiXmlNode *pNodeTmp					= NULL;
		TiXmlNode *pManifestNode			= NULL;
		TiXmlNode *pHeadNode				= NULL;
		TiXmlNode *pResponseNode			= NULL;
		TiXmlNode *pConsignmentNode			= NULL;

		pManifestNode = doc.FirstChild("Manifest");
		if (pManifestNode == NULL)
			return ;
		xmlRootElement = pManifestNode->ToElement();
		if(xmlRootElement)
		{
			if ((pHeadNode = xmlRootElement->FirstChild("Head")) != NULL)
			{
				string MESSAGE_ID = "";
				string FUNCTION_CODE = "";
				string MESSAGE_TYPE = "";
				string SENDER = "";
				string RECEIVE = "";
				string MESSAGE_CREATE_TIME = "";
				string strVERSION = "";

				xmlSubElement = pHeadNode->ToElement() ;
				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("MessageID");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_ID = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("FunctionCode");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					FUNCTION_CODE = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("MessageType");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_TYPE = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("SenderID");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					SENDER = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("ReceiverID");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					RECEIVE = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("SendTime");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					MESSAGE_CREATE_TIME = pNodeTmp->ToElement()->GetText();

				if (xmlSubElement != NULL)
					pNodeTmp = xmlSubElement->FirstChildElement("Version");
				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
					strVERSION = pNodeTmp->ToElement()->GetText();

				// 赋值
				m_mtResponse.Head.MessageID		= MESSAGE_ID;
				m_mtResponse.Head.FunctionCode	= FUNCTION_CODE;
				m_mtResponse.Head.MessageType	= MESSAGE_TYPE;
				m_mtResponse.Head.SenderID		= SENDER;
				m_mtResponse.Head.ReceiverID	= RECEIVE;
				m_mtResponse.Head.SendTime		= MESSAGE_CREATE_TIME;
				m_mtResponse.Head.Version		= strVERSION;
			}

			if ((pResponseNode = xmlRootElement->FirstChild("Response")) != NULL)
			{
				xmlResponseElement = pResponseNode->ToElement();

				if (xmlResponseElement != NULL && (pNode = xmlResponseElement->FirstChild("BorderTransportMeans")) != NULL)
				{
					xmlSubElement = pNode->ToElement() ;
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("JourneyID");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mtResponse.Declaration.BorderTransportMeans.JourneyID = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("ID");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mtResponse.Declaration.BorderTransportMeans.ID = pNodeTmp->ToElement()->GetText();
				}
			}

			if (xmlResponseElement != NULL)
			{
				for (pConsignmentNode = xmlResponseElement->FirstChild("Consignment"); pConsignmentNode != NULL; pConsignmentNode = pConsignmentNode->NextSibling("Consignment"))
				{
					structMTXXXX_Consignment consignment;
					xmlConsignmentElement = pConsignmentNode->ToElement();
					if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("TransportContractDocument")) != NULL)
					{
						TiXmlElement *xmlTransportContractDocumentElement = NULL;
						xmlTransportContractDocumentElement = xmlSubElement = pNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("ID");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							consignment.TransportContractDocument.ID = pNodeTmp->ToElement()->GetText();
					}

					if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("AssociatedTransportDocument")) != NULL)
					{
						xmlSubElement = pNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("ID");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							consignment.AssociatedTransportDocument.ID = pNodeTmp->ToElement()->GetText();
					}
					
					if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("ResponseType")) != NULL)
					{
						xmlSubElement = pNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("Code");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							consignment.ResponseType.Code = pNodeTmp->ToElement()->GetText();
							
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("Text");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							consignment.ResponseType.Text = pNodeTmp->ToElement()->GetText();							
					}					

					m_mtResponse.Declaration.Consignment.push_back(consignment);
				}


				TiXmlNode *pTransportEquipmentNode = NULL;
				TiXmlElement *xmlTransportEquipmentElement = NULL;
				for (pTransportEquipmentNode = xmlResponseElement->FirstChild("TransportEquipment"); pTransportEquipmentNode != NULL; pTransportEquipmentNode = pTransportEquipmentNode->NextSibling("TransportEquipment"))
				{
					structMTXXXX_TransportEquipment TransportEquipment;
					xmlTransportEquipmentElement = pTransportEquipmentNode->ToElement() ;
					if (xmlTransportEquipmentElement != NULL && (pNodeTmp = xmlTransportEquipmentElement->FirstChild("EquipmentIdentification")))
					{
						xmlSubElement = pNodeTmp->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("ID");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						{
							TransportEquipment.EquipmentIdentification.ID = pNodeTmp->ToElement()->GetText();
						}
					}
					
					if (xmlTransportEquipmentElement != NULL && (pNodeTmp = xmlTransportEquipmentElement->FirstChild("ResponseType")))
					{
						xmlSubElement = pNodeTmp->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("Code");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						{
							TransportEquipment.ResponseType.Code = pNodeTmp->ToElement()->GetText();
						}
						
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("Text");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						{
							TransportEquipment.ResponseType.Text = pNodeTmp->ToElement()->GetText();
						}						
					}					

					m_mtResponse.Declaration.TransportEquipment.push_back(TransportEquipment);
				}


				xmlResponseElement = pResponseNode->ToElement();
				if (xmlResponseElement != NULL && (pNode = xmlResponseElement->FirstChild("ResponseType")) != NULL)
				{
					xmlSubElement = pNode->ToElement() ;
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("Code");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mtResponse.Declaration.ResponseType.Code = pNodeTmp->ToElement()->GetText();
						
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("Text");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mtResponse.Declaration.ResponseType.Text = pNodeTmp->ToElement()->GetText();						
				}
			}
		}

		// delete file
		//if (strFileName != "")
		//{
		//	CFile::Remove(strFileName.c_str());
		//}

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

int CMTXXXX::SaveToDB()
{
	if (m_mtResponse.Head.MessageID.empty())
		return 0;

	try
	{

		//CString strConn = "";
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



		//CRADatabase database;
		//HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		//if (nRet == E_FAIL)
		//{
		//	return 0;
		//}

		//if (m_messageHead.MESSAGE_ID[0] != '\0')
		//{
		//	CString strSql = "";
		//	char szSqlBuffer[2048] = {0};
		//	sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_SHIP_DICTATE_BILL(MESSAGE_ID,CUSTOMS_CODE,I_E_FLAG,SHIP_NAME_EN,VOYAGE_NO,NOTE, \
		//												FUNCTION_CODE, MESSAGE_TYPE, SENDER, RECEIVER, CREATE_TIME, INPUT_TIME, SEND_FLAG, VERSION) \
		//												values('%s', '%s', '%c', '%s', '%s', '%s', \
		//												'%s', '%s', '%s', '%s', '%s', GetDate(), '%c', '%s')", \
		//												m_messageHead.MESSAGE_ID, m_billInfo.CUSTOMS_CODE, m_billInfo.I_E_FLAG[0], m_billInfo.SHIP_NAME_EN, m_billInfo.VOYAGE_NO, m_billInfo.NOTE, \
		//												m_messageHead.FUNCTION_CODE, m_messageHead.MESSAGE_TYPE, m_messageHead.SENDER, m_messageHead.RECEIVE, m_messageHead.MESSAGE_CREATE_TIME, '1', m_messageHead.VERSION);

		//	strSql += szSqlBuffer;

		//	// 执行Sql语句
		//	database.RAExecuteNoRs((LPCTSTR)strSql);
		//	LOG1("insert into D_SHIP_DICTATE_BILL, MESSAGE_ID=%s", m_messageHead.MESSAGE_ID);
		//}




		//size_t nContaSize = m_contaInfoVect.size();
		//for (size_t i = 0; i < nContaSize; ++i)
		//{
		//	// single insert into table
		//	size_t iTmpIndex = i;
		//	CString strSql = "";
		//	char szSqlBuffer[1024] = {0};
		//	sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_SHIP_DICTATE_CONTA(MESSAGE_ID,CONTA_NO,CONTA_MODEL,CONTA_SIZE,SEAL_NO,SHIP_STATE, NOTE) \
		//												values('%s', '%s', '%s', '%s', '%s', '%c', '%s')", \
		//												m_messageHead.MESSAGE_ID, m_contaInfoVect[iTmpIndex].CONTA_NO, m_contaInfoVect[iTmpIndex].CONTA_MODEL, \
		//												m_contaInfoVect[iTmpIndex].CONTA_SIZE, m_contaInfoVect[iTmpIndex].SEAL_NO, '1', m_contaInfoVect[iTmpIndex].NOTE);

		//	strSql += szSqlBuffer;

		//	// 执行Sql语句
		//	HRESULT hRet = database.RAExecuteNoRs((LPCTSTR)strSql);

		//	if (hRet == S_OK)
		//		m_chOpFlag = 'T';
		//	else
		//		m_chOpFlag = 'F';
		//	LOG2("insert into D_SHIP_DICTATE_CONTA, MESSAGE_ID=%s, CONTA_NO=%s", m_messageHead.MESSAGE_ID, m_contaInfoVect[iTmpIndex].CONTA_NO);
		//}
	}
	catch(...)
	{
		return 0;
	}

	m_chSEND_FLAG = '2';
	return 1;
}

void CMTXXXX::SendResponse(const std::string &strPreFlag)
{
	//if (m_messageHead.MESSAGE_ID[0] == '\0')
	//	return;

	//SelectMQ();

	//CString MESSAGE_CREATE_TIME = "";
	//MESSAGE_CREATE_TIME =COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	//CString strResponse = "";

	//std::string MESSAGE_TYPE = CMessageType::CLM10042;

	//strResponse.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\n"
	//	"<MESSAGE>\n"
	//	"<MESSAGE_HEAD>\n"
	//	"\t<MESSAGE_ID>%s</MESSAGE_ID>\n"
	//	"\t<FUNCTION_CODE>%s</FUNCTION_CODE>\n"
	//	"\t<MESSAGE_TYPE>%s</MESSAGE_TYPE>\n"
	//	"\t<SENDER>%s</SENDER>\n"
	//	"\t<RECEIVER>%s</RECEIVER>\n"
	//	"\t<MESSAGE_CREATE_TIME>%s</MESSAGE_CREATE_TIME>\n"
	//	"\t<VERSION>%s</VERSION>\n"
	//	"</MESSAGE_HEAD>\n"
	//	"<MESSAGE_LIST>\n"
	//	"<RESPOND_INFO>\n"
	//	"\t<MESSAGE_ID>%s</MESSAGE_ID>\n"
	//	"\t<OP_FLAG>%c</OP_FLAG>\n"
	//	"\t<NOTE>%s</NOTE>\n"
	//	"</RESPOND_INFO>\n"
	//	"</MESSAGE_LIST>\n"
	//	"</MESSAGE>\n", 
	//	m_messageHead.MESSAGE_ID, m_messageHead.FUNCTION_CODE, MESSAGE_TYPE.c_str(), m_messageHead.RECEIVE, \
	//	m_messageHead.SENDER, MESSAGE_CREATE_TIME, m_messageHead.VERSION, m_messageHead.MESSAGE_ID, m_chOpFlag, m_strNote.c_str());

	//// 发送MSMQ
	////CMSMQApiWrapper ob;
	//std::string szLabel = CMessageType::MT3101;
	//CString strMessage = strResponse;
	////if ((!m_strMQIp.empty()) && (!m_strQueueName.empty()))
	////	int nRet = ob.SendStreamMessage(m_strMQIp.c_str(), m_strQueueName.c_str(), szLabel.c_str(), szLabel.c_str(), strMessage);

	//std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(m_strMQIp, m_strQueueName, szLabel, szLabel));
	//apBasic->SendMessage((LPCTSTR)strMessage, strMessage.GetLength());

	// WEB应用发送
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


		CStringArray queueNameArray;
		CStringArray mqIPArray;
		SelectMQForWeb(queueNameArray, mqIPArray);

		for (int i = 0, j = 0; i < queueNameArray.GetSize() && j < mqIPArray.GetSize(); ++i,++j)
		{
			if (i != j)
				break;
			// 发送MSMQ
			std::string strQueueName	= (LPCTSTR)queueNameArray[j];
			std::string strMQIp			= (LPCTSTR)mqIPArray[i];
			std::string szLabel			= "WEB-";
			szLabel += CMessageType::MTXXXX;
			std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
			apBasic->SendMessage(spXmlData.get(), iLen);
		}

		// delete file
		if (m_strFileName != "")
		{
			CFile::Remove(m_strFileName.c_str());
		}

		return;
	}


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


	CStringArray queueNameArray;
	CStringArray mqIPArray;
	SelectMQ(queueNameArray, mqIPArray);

	for (int i = 0, j = 0; i < queueNameArray.GetSize() && j < mqIPArray.GetSize(); ++i,++j)
	{
		if (i != j)
			break;
		// 发送MSMQ
		std::string strQueueName	= (LPCTSTR)queueNameArray[j];
		std::string strMQIp			= (LPCTSTR)mqIPArray[i];
		std::string szLabel			= CMessageType::MTXXXX;
		std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
		apBasic->SendMessage(spXmlData.get(), iLen);
	}

	// delete file
	if (m_strFileName != "")
	{
		CFile::Remove(m_strFileName.c_str());
	}
}

int CMTXXXX::UpdateDB()
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
	//		sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "update D_SHIP_DICTATE_BILL set SEND_FLAG='%c' where MESSAGE_ID='%s'",  m_chSEND_FLAG, m_messageHead.MESSAGE_ID);

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

std::string CMTXXXX::GetFileName()
{
	return m_strFileName;
}

int CMTXXXX::SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	if (m_mtResponse.Head.MessageID.empty())
		return 0;

	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='WEB-CUSTOM' and INOROUTFLAG='1'", m_mtResponse.Head.ReceiverID.c_str());

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

int CMTXXXX::SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='CUSTOM' and INOROUTFLAG='1'", m_mtResponse.Head.ReceiverID.c_str());

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
