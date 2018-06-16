#include "StdAfx.h"
#include "CSM001.h"
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

CCSM001::CCSM001(void) : m_chOpFlag('F'), m_chSEND_FLAG('1'), m_strNote(""),m_csm001()
{
}

CCSM001::~CCSM001(void)
{
}

void CCSM001::InitConfigInfo(structConfigInfo *pConfig)
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

void CCSM001::SaveFileName(const std::string &strFileName)
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
//int CCSM001::SaveLocal(BYTE *pMessage, int iLen)
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

void CCSM001::ParseMessage()
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


		TiXmlHandle VesselNameEnHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselNameEn", 0).FirstChild();
		TiXmlHandle VesselNameCnHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselNameCn", 0).FirstChild();
		TiXmlHandle IMOHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("IMO", 0).FirstChild();
		TiXmlHandle CallSignHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CallSign", 0).FirstChild();
		TiXmlHandle NationalityHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("Nationality", 0).FirstChild();
		TiXmlHandle CertificateNoHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CertificateNo", 0).FirstChild();
		TiXmlHandle CertificateDateHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CertificateDate", 0).FirstChild();
		TiXmlHandle BuildDateHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("BuildDate", 0).FirstChild();
		TiXmlHandle HailingPortHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("HailingPort", 0).FirstChild();
		TiXmlHandle VesselSortHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselSort", 0).FirstChild();
		TiXmlHandle LineTypeHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("LineType", 0).FirstChild();
		TiXmlHandle CommunicationIDHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CommunicationID", 0).FirstChild();
		TiXmlHandle CommunicationTypeHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CommunicationType", 0).FirstChild();
		TiXmlHandle GrossTonnageHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("GrossTonnage", 0).FirstChild();
		TiXmlHandle NetTonnageHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("NetTonnage", 0).FirstChild();
		TiXmlHandle DeadWeightTonnageHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("DeadWeightTonnage", 0).FirstChild();
		TiXmlHandle VesselLengthHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselLength", 0).FirstChild();
		TiXmlHandle VesselBreadthHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselBreadth", 0).FirstChild();
		TiXmlHandle VesselDeapthHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselDeapth", 0).FirstChild();
		TiXmlHandle ControlTypeHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("ControlType", 0).FirstChild();
		TiXmlHandle CoCodeHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CoCode", 0).FirstChild();
		TiXmlHandle VesselCorpNameEnHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselCorpNameEn", 0).FirstChild();
		TiXmlHandle VesselCorpNameCnHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselCorpNameCn", 0).FirstChild();
		TiXmlHandle RecordDateTimeHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("RecordDateTime", 0).FirstChild();
		TiXmlHandle ClassLetterHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("ClassLetter", 0).FirstChild();
		TiXmlHandle ClassificationNoHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("ClassificationNo", 0).FirstChild();
		TiXmlHandle VesselColorHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("VesselColor", 0).FirstChild();
		TiXmlHandle FunnelColorHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("FunnelColor", 0).FirstChild();
		TiXmlHandle EmailHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("Email", 0).FirstChild();
		TiXmlHandle SatellitePhoneHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("SatellitePhone", 0).FirstChild();
		TiXmlHandle HorsePowerHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("HorsePower", 0).FirstChild();
		TiXmlHandle SpeedHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("Speed", 0).FirstChild();
		TiXmlHandle TPI_TPCHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("TPI_TPC", 0).FirstChild();
		TiXmlHandle DeepDraftHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("DeepDraft", 0).FirstChild();
		TiXmlHandle LightDraftHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("LightDraft", 0).FirstChild();
		TiXmlHandle CarbinWindowNumHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CarbinWindowNum", 0).FirstChild();
		TiXmlHandle CargoDearHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CargoDear", 0).FirstChild();
		TiXmlHandle BaleCapacityHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("BaleCapacity", 0).FirstChild();
		TiXmlHandle GrainCapacityHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("GrainCapacity", 0).FirstChild();
		TiXmlHandle CeilingTEUHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CeilingTEU", 0).FirstChild();
		TiXmlHandle DeckTEUHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("DeckTEU", 0).FirstChild();
		TiXmlHandle CrewLimitHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("CrewLimit", 0).FirstChild();
		TiXmlHandle PassengerLimitHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("PassengerLimit", 0).FirstChild();
		TiXmlHandle MemoHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("Archive", 0).ChildElement("Memo", 0).FirstChild();





		if (MessageIDHandle.Node() != NULL)	
			m_csm001.MessageHead.MessageID 			= MessageIDHandle.Text()->Value();
		if (FunctionCodeHandle.Node() != NULL)	
			m_csm001.MessageHead.FunctionCode 			= FunctionCodeHandle.Text()->Value();
		if (MessageTypeHandle.Node() != NULL)	
			m_csm001.MessageHead.MessageType 			= MessageTypeHandle.Text()->Value();
		if (SenderIDHandle.Node() != NULL)	
			m_csm001.MessageHead.SenderID 			= SenderIDHandle.Text()->Value();
		if (ReceiverIDHandle.Node() != NULL)	
			m_csm001.MessageHead.ReceiverID 			= ReceiverIDHandle.Text()->Value();
		if (SendTimeHandle.Node() != NULL)	
			m_csm001.MessageHead.SendTime 			= SendTimeHandle.Text()->Value();
		if (VersionHandle.Node() != NULL)	
			m_csm001.MessageHead.Version 			= VersionHandle.Text()->Value();

		if (VesselNameEnHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselNameEn 			= VesselNameEnHandle.Text()->Value();
		if (VesselNameCnHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselNameCn 			= VesselNameCnHandle.Text()->Value();
		if (IMOHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.IMO 					= IMOHandle.Text()->Value();
		if (CallSignHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CallSign 					= CallSignHandle.Text()->Value();

		if (NationalityHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.Nationality 					= NationalityHandle.Text()->Value();

		if (CertificateNoHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CertificateNo 					= CertificateNoHandle.Text()->Value();

		if (CertificateDateHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CertificateDate 					= CertificateDateHandle.Text()->Value();

		if (BuildDateHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.BuildDate 					= BuildDateHandle.Text()->Value();

		if (HailingPortHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.HailingPort 					= HailingPortHandle.Text()->Value();

		if (VesselSortHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselSort 					= VesselSortHandle.Text()->Value();

		if (LineTypeHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.LineType 					= LineTypeHandle.Text()->Value();

		if (CommunicationIDHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CommunicationID 					= CommunicationIDHandle.Text()->Value();

		if (CommunicationTypeHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CommunicationType 					= CommunicationTypeHandle.Text()->Value();

		if (GrossTonnageHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.GrossTonnage 					= GrossTonnageHandle.Text()->Value();

		if (NetTonnageHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.NetTonnage 					= NetTonnageHandle.Text()->Value();

		if (DeadWeightTonnageHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.DeadWeightTonnage 					= DeadWeightTonnageHandle.Text()->Value();

		if (VesselLengthHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselLength 					= VesselLengthHandle.Text()->Value();
		if (VesselBreadthHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselBreadth 					= VesselBreadthHandle.Text()->Value();
		if (VesselDeapthHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselDeapth 					= VesselDeapthHandle.Text()->Value();
		if (ControlTypeHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.ControlType 					= ControlTypeHandle.Text()->Value();
		if (CoCodeHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CoCode 					= CoCodeHandle.Text()->Value();
		if (VesselCorpNameEnHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselCorpNameEn 					= VesselCorpNameEnHandle.Text()->Value();
		if (VesselCorpNameCnHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselCorpNameCn 					= VesselCorpNameCnHandle.Text()->Value();
		if (RecordDateTimeHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.RecordDateTime 					= RecordDateTimeHandle.Text()->Value();
		if (ClassLetterHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.ClassLetter 					= ClassLetterHandle.Text()->Value();
		if (ClassificationNoHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.ClassificationNo 					= ClassificationNoHandle.Text()->Value();
		if (VesselColorHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.VesselColor 					= VesselColorHandle.Text()->Value();
		if (FunnelColorHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.FunnelColor 					= FunnelColorHandle.Text()->Value();
		if (EmailHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.Email 					= EmailHandle.Text()->Value();
		if (SatellitePhoneHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.SatellitePhone 					= SatellitePhoneHandle.Text()->Value();
		if (HorsePowerHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.HorsePower 					= HorsePowerHandle.Text()->Value();
		if (SpeedHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.Speed 					= SpeedHandle.Text()->Value();
		if (TPI_TPCHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.TPI_TPC 					= TPI_TPCHandle.Text()->Value();
		if (DeepDraftHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.DeepDraft 					= DeepDraftHandle.Text()->Value();
		if (LightDraftHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.LightDraft 					= LightDraftHandle.Text()->Value();
		if (CarbinWindowNumHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CarbinWindowNum 					= CarbinWindowNumHandle.Text()->Value();
		if (CargoDearHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CargoDear 					= CargoDearHandle.Text()->Value();
		if (BaleCapacityHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.BaleCapacity 					= BaleCapacityHandle.Text()->Value();
		if (GrainCapacityHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.GrainCapacity 					= GrainCapacityHandle.Text()->Value();
		if (CeilingTEUHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CeilingTEU 					= CeilingTEUHandle.Text()->Value();
		if (DeckTEUHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.DeckTEU 					= DeckTEUHandle.Text()->Value();
		if (CrewLimitHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.CrewLimit 					= CrewLimitHandle.Text()->Value();
		if (PassengerLimitHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.PassengerLimit 					= PassengerLimitHandle.Text()->Value();
		if (MemoHandle.Node() != NULL)	
			m_csm001.MessageBody.Declaration.Archive.Memo 					= MemoHandle.Text()->Value();
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

int CCSM001::SaveToDB()
{
	if (m_csm001.MessageHead.MessageID.empty())
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


		GUID guid = GetGUID();
		char buf[64] = {0};
		_snprintf_s(buf, sizeof(buf), sizeof(buf)
			, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
		, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
		, guid.Data4[6], guid.Data4[7]
		);

		CRADatabase database;
		HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		if (nRet == E_FAIL)
		{
			return 0;
		}

		if (!m_csm001.MessageHead.MessageID.empty())
		{
			double GrossTonnage = atof(m_csm001.MessageBody.Declaration.Archive.GrossTonnage.c_str());
			double NetTonnage	= atof(m_csm001.MessageBody.Declaration.Archive.NetTonnage.c_str());
			double DeadWeightTonnage = atof(m_csm001.MessageBody.Declaration.Archive.DeadWeightTonnage.c_str());
			double VesselLength		= atof(m_csm001.MessageBody.Declaration.Archive.VesselLength.c_str());
			double VesselBreadth	= atof(m_csm001.MessageBody.Declaration.Archive.VesselBreadth.c_str());
			double VesselDeapth		= atof(m_csm001.MessageBody.Declaration.Archive.VesselDeapth.c_str());
			CString strSql = "";
			char szSqlBuffer[2048] = {0};
			sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into P_VESSEL_INFO(GUID,SHIP_NAME_EN,IMO,CallSign,BuildDate,HailingPort, \
														VesselSort, LineType, CommunicationID, CommunicationType, GrossTonnage, \
														NetTonnage, DeadWeightTonnage, VesselLength, VesselBreadth, VesselDeapth,\
														RecordDateTime) \
														values('%s', '%s', '%s', '%s', '%s', '%s', \
														'%s', '%s', '%s', '%s', '%.4f', \
														'%.4f', '%.4f', '%.4f', '%.4f', '%.4f', \
														GetDate())", \
														buf, 
														m_csm001.MessageBody.Declaration.Archive.VesselNameEn.c_str(), 
														m_csm001.MessageBody.Declaration.Archive.IMO.c_str(), 
														m_csm001.MessageBody.Declaration.Archive.CallSign.c_str(),
														m_csm001.MessageBody.Declaration.Archive.BuildDate.c_str(),
														m_csm001.MessageBody.Declaration.Archive.HailingPort.c_str(),
														m_csm001.MessageBody.Declaration.Archive.VesselSort.c_str(),
														m_csm001.MessageBody.Declaration.Archive.LineType.c_str(),
														m_csm001.MessageBody.Declaration.Archive.CommunicationID.c_str(),
														m_csm001.MessageBody.Declaration.Archive.CommunicationType.c_str(),
														GrossTonnage,
														NetTonnage,
														DeadWeightTonnage,
														VesselLength,
														VesselBreadth,
														VesselDeapth,
														m_csm001.MessageBody.Declaration.Archive.RecordDateTime.c_str());

			strSql += szSqlBuffer;

			// 执行Sql语句
			nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
			if (nRet == E_FAIL)
			{
				m_chOpFlag = 'F';
				m_chSEND_FLAG = '2';
				return 0;
			}
			LOG1("insert into P_VESSEL_INFO, MESSAGE_ID=%s", m_csm001.MessageHead.MessageID.c_str());
		}




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

void CCSM001::SendResponse()
{
	if (m_csm001.MessageHead.MessageID.empty())
		return;

	CStringArray queueNameArray;
	CStringArray mqIPArray;
	SelectMQ(queueNameArray, mqIPArray);

	CString MESSAGE_CREATE_TIME = "";
	MESSAGE_CREATE_TIME =COleDateTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
	CString strResponse = "";

	std::string MESSAGE_TYPE = CMessageType::CSM301;
	std::string RecType		= "REC";
	std::string RecContent	= "";
	std::string PortCode	= "";

	strResponse.Format("<?xml version=\"1.0\" encoding=\"GB2312\"?>\n"
"<Message>\n"
"<MessageHead>\n"
"\t<MessageID>%s</MessageID>\n"
"\t<FunctionCode>%s</FunctionCode>\n"
"\t<MessageType>%s</MessageType>\n"
"\t<SenderID>%s</SenderID>\n"
"\t<ReceiverID>%s</ReceiverID>\n"
"\t<SendTime>%s</SendTime>\n"
"\t<Version>%s</Version>\n"
"</MessageHead>\n"
"<MessageBody>\n"
"\t<Declaration>\n"
"\t<REC>\n"
"\t<MessageID>%s</MessageID>\n"
"\t<IMO>%s</IMO>\n"
"\t<Voyage>%s</Voyage>\n"
"\t<RecType>%s</RecType>\n"
"\t<RecContent>%s</RecContent>\n"
"\t<PortCode>%s</PortCode>\n"
"\t<CustomsName>%s</CustomsName>\n"
"\t<SendTime>%s</SendTime>\n"
"\t</REC>\n"
"\t</Declaration>\n"
"</MessageBody>\n"
"</Message>\n", 
		m_csm001.MessageHead.MessageID.c_str(), 
		m_csm001.MessageHead.FunctionCode.c_str(),
		MESSAGE_TYPE.c_str(),
		m_csm001.MessageHead.ReceiverID.c_str(),
		m_csm001.MessageHead.SenderID.c_str(),
		MESSAGE_CREATE_TIME,
		m_csm001.MessageHead.Version.c_str(),
		m_csm001.MessageHead.MessageID.c_str(),
		m_csm001.MessageBody.Declaration.Archive.IMO.c_str(),
		m_csm001.MessageBody.Declaration.Archive.CallSign.c_str(),
		RecType.c_str(),
		RecContent.c_str(),
		PortCode.c_str(),
		m_csm001.MessageHead.ReceiverID.c_str(),
		MESSAGE_CREATE_TIME);

	// 发送MSMQ
	//CMSMQApiWrapper ob;
	//std::string szLabel = CMessageType::CSM001;
	CString strMessage = strResponse;
	//if ((!m_strMQIp.empty()) && (!m_strQueueName.empty()))
	//	int nRet = ob.SendStreamMessage(m_strMQIp.c_str(), m_strQueueName.c_str(), szLabel.c_str(), szLabel.c_str(), strMessage);

	//std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(m_strMQIp, m_strQueueName, szLabel, szLabel));
	//apBasic->SendMessage((LPCTSTR)strMessage, strMessage.GetLength());

	for (int i = 0, j = 0; i < queueNameArray.GetSize() && j < mqIPArray.GetSize(); ++i,++j)
	{
		if (i != j)
			break;
		// 发送MSMQ
		std::string strQueueName	= (LPCTSTR)queueNameArray[j];
		std::string strMQIp			= (LPCTSTR)mqIPArray[i];
		std::string szLabel			= CMessageType::CSM301;
		std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
		apBasic->SendMessage((LPCTSTR)strMessage, strMessage.GetLength());
	}




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
	//		szLabel += CMessageType::CSM001;
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
	//	std::string szLabel			= CMessageType::CSM001;
	//	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	//	apBasic->SendMessage(spXmlData.get(), iLen);
	//}

	// delete file
	if (m_strFileName != "")
	{
		if (GetFileAttributes(m_strFileName.c_str()) != -1)
			CFile::Remove(m_strFileName.c_str());
	}
}

int CCSM001::UpdateDB()
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

std::string CCSM001::GetFileName()
{
	return m_strFileName;
}

int CCSM001::SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	if (m_csm001.MessageHead.MessageID.empty())
		return 0;

	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='PROXY' and INOROUTFLAG='0'", m_csm001.MessageHead.ReceiverID.c_str());

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

int CCSM001::SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='WEB' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='WEB' and INOROUTFLAG='0'", m_csm001.MessageHead.ReceiverID.c_str());

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

GUID CCSM001::GetGUID(void)
{
	GUID guid;
	CoInitialize(NULL);
	::CoCreateGuid(&guid);
	CoUninitialize();
	return guid;
}

