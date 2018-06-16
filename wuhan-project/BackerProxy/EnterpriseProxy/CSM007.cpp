#include "StdAfx.h"
#include "CSM007.h"
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

CCSM007::CCSM007(void) : m_chOpFlag('F'), m_chSEND_FLAG('1'), m_strNote(""),m_csm007()
{
}

CCSM007::~CCSM007(void)
{
}

void CCSM007::InitConfigInfo(structConfigInfo *pConfig)
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

void CCSM007::SaveFileName(const std::string &strFileName)
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
//int CCSM007::SaveLocal(BYTE *pMessage, int iLen)
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

void CCSM007::ParseMessage()
{
	try
	{
		if (m_strFileName.empty())
			return;

		string strFileName = m_strFileName;
		TiXmlDocument doc(strFileName.c_str());
		doc.LoadFile();

		TiXmlHandle docHandle(&doc);
		TiXmlHandle MessageHandle			= docHandle.FirstChildElement("Message");
		TiXmlHandle MessageIDHandle			= docHandle.FirstChildElement("Message").FirstChildElement("MessageHead").ChildElement("MessageID", 0).FirstChild();
		TiXmlHandle FunctionCodeHandle		= docHandle.FirstChildElement("Message").FirstChildElement("MessageHead").ChildElement("FunctionCode", 0).FirstChild();
		TiXmlHandle MessageTypeHandle		= docHandle.FirstChildElement("Message").FirstChildElement("MessageHead").ChildElement("MessageType", 0).FirstChild();
		TiXmlHandle SenderIDHandle			= docHandle.FirstChildElement("Message").FirstChildElement("MessageHead").ChildElement("SenderID", 0).FirstChild();
		TiXmlHandle ReceiverIDHandle		= docHandle.FirstChildElement("Message").FirstChildElement("MessageHead").ChildElement("ReceiverID", 0).FirstChild();
		TiXmlHandle SendTimeHandle			= docHandle.FirstChildElement("Message").FirstChildElement("MessageHead").ChildElement("SendTime", 0).FirstChild();
		TiXmlHandle VersionHandle			= docHandle.FirstChildElement("Message").FirstChildElement("MessageHead").ChildElement("Version", 0).FirstChild();


		TiXmlHandle ID = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("ID", 0).FirstChild();
		TiXmlHandle Name = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("Name", 0).FirstChild();
		TiXmlHandle JourneyID = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("JourneyID", 0).FirstChild();


		//TiXmlHandle JourneyID2 = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("JourneyID", 0).();
		TiXmlNode *pNode = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("JourneyID", 0).ToNode();
		std::string JourneyID2 = "";
		if (pNode != NULL)
		{
			pNode = pNode->NextSibling("JourneyID");
			if (pNode != NULL)
			{
				if (pNode->ToElement() != NULL && pNode->ToElement()->GetText() != NULL)
					JourneyID2 = pNode->ToElement()->GetText();
			}
		}


		TiXmlHandle ArrivalDateTime = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("Itinerary", 0).ChildElement("ArrivalDateTime", 0).FirstChild();
		TiXmlHandle DepartureDateTime = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("Itinerary", 0).ChildElement("DepartureDateTime", 0).FirstChild();


		TiXmlHandle StayID = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("StayID", 0).FirstChild();
		TiXmlHandle DepartureConveyanceFacilityID = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("DepartureConveyanceFacility", 0).ChildElement("ID", 0).FirstChild();
		TiXmlHandle ArrivalConveyanceFacilityID = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("BorderTransPortMeans", 0).ChildElement("ArrivalConveyanceFacility", 0).ChildElement("ID", 0).FirstChild();


		TiXmlHandle AgentID = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Agent", 0).ChildElement("ID", 0).FirstChild();
		TiXmlHandle ContactName = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Agent", 0).ChildElement("Contact", 0).ChildElement("Name", 0).FirstChild();
		TiXmlHandle IssueDate = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("IssueDate", 0).FirstChild();
		TiXmlHandle Content = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("AdditionalInformation", 0).ChildElement("Content", 0).FirstChild();


		if (MessageIDHandle.Node() != NULL)	
			m_csm007.MessageHead.MessageID 			= MessageIDHandle.Text()->Value();
		if (FunctionCodeHandle.Node() != NULL)	
			m_csm007.MessageHead.FunctionCode 			= FunctionCodeHandle.Text()->Value();
		if (MessageTypeHandle.Node() != NULL)	
			m_csm007.MessageHead.MessageType 			= MessageTypeHandle.Text()->Value();
		if (SenderIDHandle.Node() != NULL)	
			m_csm007.MessageHead.SenderID 			= SenderIDHandle.Text()->Value();
		if (ReceiverIDHandle.Node() != NULL)	
			m_csm007.MessageHead.ReceiverID 			= ReceiverIDHandle.Text()->Value();
		if (SendTimeHandle.Node() != NULL)	
			m_csm007.MessageHead.SendTime 			= SendTimeHandle.Text()->Value();
		if (VersionHandle.Node() != NULL)	
			m_csm007.MessageHead.Version 			= VersionHandle.Text()->Value();

		if (ID.Node() != NULL)	
			m_csm007.MessageBody.Declaration.BorderTransPortMeans.ID 			= ID.Text()->Value();
		if (Name.Node() != NULL)	
			m_csm007.MessageBody.Declaration.BorderTransPortMeans.Name 			= Name.Text()->Value();
		if (JourneyID.Node() != NULL)	
			m_csm007.MessageBody.Declaration.BorderTransPortMeans.JourneyID 					= JourneyID.Text()->Value();
		//if (JourneyID2.Node() != NULL)	
		m_csm007.MessageBody.Declaration.BorderTransPortMeans.JourneyID2 						= JourneyID2;

		if (ArrivalDateTime.Node() != NULL)	
			m_csm007.MessageBody.Declaration.BorderTransPortMeans.Itinerary.ArrivalDateTime 					= ArrivalDateTime.Text()->Value();

		if (DepartureDateTime.Node() != NULL)	
			m_csm007.MessageBody.Declaration.BorderTransPortMeans.Itinerary.DepartureDateTime 					= DepartureDateTime.Text()->Value();

		if (StayID.Node() != NULL)	
			m_csm007.MessageBody.Declaration.BorderTransPortMeans.StayID 					= StayID.Text()->Value();

		if (DepartureConveyanceFacilityID.Node() != NULL)	
			m_csm007.MessageBody.Declaration.BorderTransPortMeans.DepartureConveyanceFacility.ID 					= DepartureConveyanceFacilityID.Text()->Value();

		if (ArrivalConveyanceFacilityID.Node() != NULL)	
			m_csm007.MessageBody.Declaration.BorderTransPortMeans.ArrivalConveyanceFacility.ID					= ArrivalConveyanceFacilityID.Text()->Value();

		if (AgentID.Node() != NULL)	
			m_csm007.MessageBody.Declaration.Agent.ID 					= AgentID.Text()->Value();

		if (ContactName.Node() != NULL)	
			m_csm007.MessageBody.Declaration.Agent.Contact.Name					= ContactName.Text()->Value();

		if (IssueDate.Node() != NULL)	
			m_csm007.MessageBody.Declaration.IssueDate 					= IssueDate.Text()->Value();

		if (Content.Node() != NULL)	
			m_csm007.MessageBody.Declaration.AdditionalInformation.Content 					= Content.Text()->Value();

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

int CCSM007::SaveToDB()
{
	return 1;
	if (m_csm007.MessageHead.MessageID.empty())
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

void CCSM007::SendResponse(const std::string &strPreFlag)
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
	//std::string szLabel = CMessageType::CSM007;
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
		//	std::string szLabel			= "WEB-";
		//	szLabel += CMessageType::CSM007;
		//	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
		//	apBasic->SendMessage(spXmlData.get(), iLen);
		//}


		// 发送MSMQ
		std::string strQueueName	= m_config.strBackerQueueName;
		std::string strMQIp			= m_config.strBackerQueueIP;
		std::string szLabel			= "WEB-";
		szLabel += CMessageType::CSM007;
		std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
		apBasic->SendMessage(spXmlData.get(), iLen);

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
	//	std::string szLabel			= CMessageType::CSM007;
	//	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	//	apBasic->SendMessage(spXmlData.get(), iLen);
	//}


	// 发送MSMQ
	std::string strQueueName	= m_config.strBackerQueueName;
	std::string strMQIp			= m_config.strBackerQueueIP;
	std::string szLabel			= ""; // "WEB-";
	szLabel += CMessageType::CSM007;
	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	apBasic->SendMessage(spXmlData.get(), iLen);


	// delete file
	if (m_strFileName != "")
	{
		CFile::Remove(m_strFileName.c_str());
	}
}

int CCSM007::UpdateDB()
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

std::string CCSM007::GetFileName()
{
	return m_strFileName;
}

int CCSM007::SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	if (m_csm007.MessageHead.MessageID.empty())
		return 0;

	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='WEB-CUSTOM' and INOROUTFLAG='1'", m_csm007.MessageHead.ReceiverID.c_str());

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

int CCSM007::SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='CUSTOM' and INOROUTFLAG='1'", m_csm007.MessageHead.ReceiverID.c_str());

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

