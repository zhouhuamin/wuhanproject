#include "StdAfx.h"
#include "CSM301.h"
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

CCSM301::CCSM301(void) : m_chOpFlag('F'), m_chSEND_FLAG('1'), m_strNote(""),m_csm301()
{
}

CCSM301::~CCSM301(void)
{
}

void CCSM301::InitConfigInfo(structConfigInfo *pConfig)
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

void CCSM301::SaveFileName(const std::string &strFileName)
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
//int CCSM301::SaveLocal(BYTE *pMessage, int iLen)
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

void CCSM301::ParseMessage()
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


		TiXmlHandle MessageIDHandle2 = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("REC", 0).ChildElement("MessageID", 0).FirstChild();
		TiXmlHandle IMOHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("REC", 0).ChildElement("IMO", 0).FirstChild();
		TiXmlHandle VoyageHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("REC", 0).ChildElement("Voyage", 0).FirstChild();
		TiXmlHandle RecTypeHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("REC", 0).ChildElement("RecType", 0).FirstChild();
		TiXmlHandle RecContentHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("REC", 0).ChildElement("RecContent", 0).FirstChild();
		TiXmlHandle PortCodeHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("REC", 0).ChildElement("PortCode", 0).FirstChild();
		TiXmlHandle CustomsNameHandle = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("REC", 0).ChildElement("CustomsName", 0).FirstChild();
		TiXmlHandle SendTimeHandle2 = docHandle.FirstChildElement("Message").FirstChildElement("MessageBody").ChildElement("Declaration", 0).ChildElement("REC", 0).ChildElement("SendTime", 0).FirstChild();


		if (MessageIDHandle.Node() != NULL)	
			m_csm301.MessageHead.MessageID 			= MessageIDHandle.Text()->Value();
		if (FunctionCodeHandle.Node() != NULL)	
			m_csm301.MessageHead.FunctionCode 			= FunctionCodeHandle.Text()->Value();
		if (MessageTypeHandle.Node() != NULL)	
			m_csm301.MessageHead.MessageType 			= MessageTypeHandle.Text()->Value();
		if (SenderIDHandle.Node() != NULL)	
			m_csm301.MessageHead.SenderID 			= SenderIDHandle.Text()->Value();
		if (ReceiverIDHandle.Node() != NULL)	
			m_csm301.MessageHead.ReceiverID 			= ReceiverIDHandle.Text()->Value();
		if (SendTimeHandle.Node() != NULL)	
			m_csm301.MessageHead.SendTime 			= SendTimeHandle.Text()->Value();
		if (VersionHandle.Node() != NULL)	
			m_csm301.MessageHead.Version 			= VersionHandle.Text()->Value();

		if (MessageIDHandle2.Node() != NULL)	
			m_csm301.MessageBody.Declaration.REC.MessageID 			= MessageIDHandle2.Text()->Value();
		if (IMOHandle.Node() != NULL)	
			m_csm301.MessageBody.Declaration.REC.IMO				= IMOHandle.Text()->Value();
		if (VoyageHandle.Node() != NULL)	
			m_csm301.MessageBody.Declaration.REC.Voyage				= VoyageHandle.Text()->Value();
		if (RecTypeHandle.Node() != NULL)	
			m_csm301.MessageBody.Declaration.REC.RecType 			= RecTypeHandle.Text()->Value();

		std::string RecContent = "";
		if (RecContentHandle.Node() != NULL)	
			RecContent 		= RecContentHandle.Text()->Value();
		if (PortCodeHandle.Node() != NULL)	
			m_csm301.MessageBody.Declaration.REC.PortCode 		= PortCodeHandle.Text()->Value();
		if (CustomsNameHandle.Node() != NULL)	
			m_csm301.MessageBody.Declaration.REC.CustomsName 		= CustomsNameHandle.Text()->Value();
		if (SendTimeHandle2.Node() != NULL)	
			m_csm301.MessageBody.Declaration.REC.SendTime 		= SendTimeHandle2.Text()->Value();



		char szNote[256] = {0};
		char *pTmp = szNote;
		memcpy(szNote, RecContent.c_str(), RecContent.size() < 256 ? RecContent.size() : 255);
		UTF8ToGBK(pTmp);
		szNote[255] = '\0';
		m_csm301.MessageBody.Declaration.REC.RecContent = szNote;


		//TiXmlNode*    node = 0;
		//TiXmlElement* itemElement = 0;
		//TiXmlElement* xmlRootElement		= NULL;
		//TiXmlElement* xmlRootElement2		= NULL;
		//TiXmlElement* xmlDeclarationElement = NULL;
		//TiXmlElement* xmlSubElement			= NULL;
		//TiXmlElement* xmlSonElement			= NULL;
		//TiXmlElement *xmlConsignmentElement = NULL;


		//TiXmlNode * pNode					= NULL;
		//TiXmlNode *pNodeTmp					= NULL;
		//TiXmlNode *pManifestNode			= NULL;
		//TiXmlNode *pHeadNode				= NULL;
		//TiXmlNode *pDeclarationNode			= NULL;
		//TiXmlNode *pConsignmentNode			= NULL;

		//pManifestNode = doc.FirstChild("Manifest");
		//if (pManifestNode == NULL)
		//	return ;
		//xmlRootElement = pManifestNode->ToElement();
		//if(xmlRootElement)
		//{
		//	if ((pHeadNode = xmlRootElement->FirstChild("Head")) != NULL)
		//	{
		//		string MESSAGE_ID = "";
		//		string FUNCTION_CODE = "";
		//		string MESSAGE_TYPE = "";
		//		string SENDER = "";
		//		string RECEIVE = "";
		//		string MESSAGE_CREATE_TIME = "";
		//		string strVERSION = "";

		//		xmlSubElement = pHeadNode->ToElement() ;
		//		if (xmlSubElement != NULL)
		//			pNodeTmp = xmlSubElement->FirstChildElement("MessageID");
		//		if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//			MESSAGE_ID = pNodeTmp->ToElement()->GetText();

		//		if (xmlSubElement != NULL)
		//			pNodeTmp = xmlSubElement->FirstChildElement("FunctionCode");
		//		if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//			FUNCTION_CODE = pNodeTmp->ToElement()->GetText();

		//		if (xmlSubElement != NULL)
		//			pNodeTmp = xmlSubElement->FirstChildElement("MessageType");
		//		if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//			MESSAGE_TYPE = pNodeTmp->ToElement()->GetText();

		//		if (xmlSubElement != NULL)
		//			pNodeTmp = xmlSubElement->FirstChildElement("SenderID");
		//		if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//			SENDER = pNodeTmp->ToElement()->GetText();

		//		if (xmlSubElement != NULL)
		//			pNodeTmp = xmlSubElement->FirstChildElement("ReceiverID");
		//		if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//			RECEIVE = pNodeTmp->ToElement()->GetText();

		//		if (xmlSubElement != NULL)
		//			pNodeTmp = xmlSubElement->FirstChildElement("SendTime");
		//		if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//			MESSAGE_CREATE_TIME = pNodeTmp->ToElement()->GetText();

		//		if (xmlSubElement != NULL)
		//			pNodeTmp = xmlSubElement->FirstChildElement("Version");
		//		if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//			strVERSION = pNodeTmp->ToElement()->GetText();

		//		// 赋值
		//		m_mt2010.Head.MessageID		= MESSAGE_ID;
		//		m_mt2010.Head.FunctionCode	= FUNCTION_CODE;
		//		m_mt2010.Head.MessageType	= MESSAGE_TYPE;
		//		m_mt2010.Head.SenderID		= SENDER;
		//		m_mt2010.Head.SendTime		= MESSAGE_CREATE_TIME;
		//		m_mt2010.Head.Version		= strVERSION;
		//	}

		//	if ((pDeclarationNode = xmlRootElement->FirstChild("Declaration")) != NULL)
		//	{
		//		xmlDeclarationElement = pDeclarationNode->ToElement();
		//		if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("RepresentativePerson")) != NULL)
		//		{
		//			xmlSubElement = pNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("Name");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.RepresentativePersonName = pNodeTmp->ToElement()->GetText();
		//		}

		//		if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("ExitCustomsOffice")) != NULL)
		//		{
		//			xmlSubElement = pNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.ExitCustomsOfficeID = pNodeTmp->ToElement()->GetText();
		//		}

		//		if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("Agent")) != NULL)
		//		{
		//			xmlSubElement = pNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.AgentID = pNodeTmp->ToElement()->GetText();
		//		}

		//		if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("Carrier")) != NULL)
		//		{
		//			xmlSubElement = pNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.CarrierID = pNodeTmp->ToElement()->GetText();
		//		}

		//		if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("BorderTransportMeans")) != NULL)
		//		{
		//			xmlSubElement = pNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("JourneyID");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.BorderTransportMeans.JourneyID = pNodeTmp->ToElement()->GetText();

		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("TypeCode");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.BorderTransportMeans.TypeCode = pNodeTmp->ToElement()->GetText();

		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.BorderTransportMeans.ID = pNodeTmp->ToElement()->GetText();

		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("Name");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.BorderTransportMeans.Name = pNodeTmp->ToElement()->GetText();

		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("FirstArrivalLocationID");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.BorderTransportMeans.FirstArrivalLocationID = pNodeTmp->ToElement()->GetText();

		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("ArrivalDateTime");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.BorderTransportMeans.ArrivalDateTime = pNodeTmp->ToElement()->GetText();

		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("DepartureDateTime");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.BorderTransportMeans.DepartureDateTime = pNodeTmp->ToElement()->GetText();
		//		}
		//	}

		//	if (xmlDeclarationElement != NULL)
		//	{
		//		for (pConsignmentNode = xmlDeclarationElement->FirstChild("Consignment"); pConsignmentNode != NULL; pConsignmentNode = pConsignmentNode->NextSibling("Consignment"))
		//		{
		//			structMT2010_Consignment consignment;
		//			xmlConsignmentElement = pConsignmentNode->ToElement();
		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("TransportContractDocument")) != NULL)
		//			{
		//				TiXmlElement *xmlTransportContractDocumentElement = NULL;
		//				xmlTransportContractDocumentElement = xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.TransportContractDocument.ID = pNodeTmp->ToElement()->GetText();


		//				if (xmlTransportContractDocumentElement != NULL)
		//				{
		//					for (pNodeTmp = xmlTransportContractDocumentElement->FirstChild("ChangeReasonCode"); pNodeTmp != NULL; pNodeTmp = pNodeTmp->NextSibling("ChangeReasonCode")) // pNodeTmp = pNodeTmp->NextSibling())
		//					{
		//						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//						{
		//							structMT2010_ChangeReasonCode resonCode;
		//							resonCode.ChangeReasonCode = pNodeTmp->ToElement()->GetText();
		//							consignment.TransportContractDocument.ChangeReasonCode.push_back(resonCode);
		//						}

		//						//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//						//{
		//						//	structMT2010_ChangeReasonCode resonCode;
		//						//	resonCode.ChangeReasonCode = pNodeTmp->ToElement()->GetText();
		//						//	consignment.TransportContractDocument.ChangeReasonCode.push_back(resonCode);
		//						//}
		//					}
		//				}

		//				//if (xmlSubElement != NULL)
		//				//	pNodeTmp = xmlSubElement->FirstChildElement("ChangeReasonCode");
		//				//if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				//{
		//				//	std::string ChangeReasonCode = pNodeTmp->ToElement()->GetText();
		//				//	consignment.TransportContractDocument.ChangeReasonCode.push_back(ChangeReasonCode);
		//				//}

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ConditionCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.TransportContractDocument.ConditionCode = pNodeTmp->ToElement()->GetText();

		//				if ((pNode = xmlSubElement->FirstChild("Consolidator")) != NULL)
		//				{
		//					xmlSubElement = pNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//						consignment.TransportContractDocument.ConsolidatorID = pNodeTmp->ToElement()->GetText();
		//				}
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("AssociatedTransportDocument")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.AssociatedTransportDocumentID = pNodeTmp->ToElement()->GetText();
		//			}

		//			pNodeTmp = NULL;
		//			xmlSubElement = pConsignmentNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("GrossVolumeMeasure");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				consignment.GrossVolumeMeasure = pNodeTmp->ToElement()->GetText();


		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("ValueAmount")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("CurrencyTypeCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.CurrencyTypeCode = pNodeTmp->ToElement()->GetText();
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("LoadingLocation")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.LoadingLocationID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("LoadingDate");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.LoadingLocationLoadingDate = pNodeTmp->ToElement()->GetText();
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("UnloadingLocation")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.UnloadingLocationID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ArrivalDate");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.UnloadingLocationArrivalDate = pNodeTmp->ToElement()->GetText();
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("GoodsReceiptPlace")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.GoodsReceiptPlaceID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("Name");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.GoodsReceiptPlaceName = pNodeTmp->ToElement()->GetText();
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("TranshipmentLocation")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.TranshipmentLocationID = pNodeTmp->ToElement()->GetText();
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("TransitDestination")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.TransitDestinationID = pNodeTmp->ToElement()->GetText();
		//			}

		//			pNodeTmp = NULL;
		//			xmlSubElement = pConsignmentNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("RoutingCountryCode");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				consignment.RoutingCountryCode = pNodeTmp->ToElement()->GetText();


		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("GoodsConsignedPlace")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.GoodsConsignedPlaceID = pNodeTmp->ToElement()->GetText();
		//			}

		//			pNodeTmp = NULL;
		//			xmlSubElement = pConsignmentNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("CustomsStatusCode");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				consignment.CustomsStatusCode = pNodeTmp->ToElement()->GetText();

		//			pNodeTmp = NULL;
		//			xmlSubElement = pConsignmentNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("TransportSplitIndicator");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				consignment.TransportSplitIndicator = pNodeTmp->ToElement()->GetText();

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("FreightPayment")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("MethodCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.FreightPaymentMethodCode = pNodeTmp->ToElement()->GetText();
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("ConsignmentPackaging")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("QuantityQuantity");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.ConsignmentPackagingQuantityQuantity = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("TypeCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.ConsignmentPackagingTypeCode = pNodeTmp->ToElement()->GetText();
		//			}

		//			pNodeTmp = NULL;
		//			xmlSubElement = pConsignmentNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("TotalGrossMassMeasure");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				consignment.TotalGrossMassMeasure = pNodeTmp->ToElement()->GetText();


		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("PreviousCustomsDocument")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.PreviousCustomsDocumentID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("TypeCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.PreviousCustomsDocumentTypeCode = pNodeTmp->ToElement()->GetText();
		//			}


		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("DeliveryDestination")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("Line");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.DeliveryDestination.Line = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("CityName");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.DeliveryDestination.CityName = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("CountrySubEntityID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.DeliveryDestination.CountrySubEntityID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("CountrySubEntityName");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.DeliveryDestination.CountrySubEntityName = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("PostcodeID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.DeliveryDestination.PostcodeID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("CountryCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.DeliveryDestination.CountryCode = pNodeTmp->ToElement()->GetText();
		//			}


		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("Handling")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("InstructionsCodeCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.HandlingInstructionsCodeCode = pNodeTmp->ToElement()->GetText();

		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("IntermediateCarrier")) != NULL)
		//			{
		//				xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.IntermediateCarrier.ID = pNodeTmp->ToElement()->GetText();

		//				TiXmlNode *pCommunicationNode = NULL;
		//				for (pCommunicationNode = xmlSubElement->FirstChild("Communication"); pCommunicationNode != NULL; pCommunicationNode = pCommunicationNode->NextSibling("Communication"))
		//				{
		//					structMT2010_Communication tmpCommunication;
		//					xmlSubElement = pCommunicationNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.ID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("TypeID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.TypeID = pNodeTmp->ToElement()->GetText();
		//					}
		//					consignment.IntermediateCarrier.Communication.push_back(tmpCommunication);
		//				}
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("Consignee")) != NULL)
		//			{
		//				TiXmlElement *pConsigneeElement = NULL;
		//				pConsigneeElement = xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.Consignee.ID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("Name");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.Consignee.Name = pNodeTmp->ToElement()->GetText();




		//				TiXmlNode *pAddressNode = NULL;
		//				if (xmlSubElement != NULL && (pAddressNode = xmlSubElement->FirstChild("Address")) != NULL)
		//				{
		//					xmlSubElement = pAddressNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("Line");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignee.Addresss.Line = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CityName");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignee.Addresss.CityName = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountrySubEntityID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignee.Addresss.CountrySubEntityID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountrySubEntityName");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignee.Addresss.CountrySubEntityName = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("PostcodeID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignee.Addresss.PostcodeID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountryCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignee.Addresss.CountryCode = pNodeTmp->ToElement()->GetText();
		//					}
		//				}

		//				TiXmlNode *pCommunicationNode = NULL;
		//				for (pCommunicationNode = pConsigneeElement->FirstChild("Communication"); pCommunicationNode != NULL; pCommunicationNode = pCommunicationNode->NextSibling("Communication"))
		//				{
		//					structMT2010_Communication tmpCommunication;
		//					xmlSubElement = pCommunicationNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.ID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("TypeID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.TypeID = pNodeTmp->ToElement()->GetText();
		//					}
		//					consignment.Consignee.Communication.push_back(tmpCommunication);
		//				}

		//				TiXmlNode *pContactNode = NULL;
		//				TiXmlElement *xmlContactElement = NULL;
		//				if (xmlSubElement != NULL && (pContactNode = pConsigneeElement->FirstChild("Contact")) != NULL)
		//				{
		//					xmlContactElement = xmlSubElement = pContactNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("Name");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignee.Contact.Name = pNodeTmp->ToElement()->GetText();
		//					}
		//					if (xmlContactElement != NULL)
		//					{
		//						TiXmlNode *pCommunicationNode = NULL;
		//						for (pCommunicationNode = xmlContactElement->FirstChild("Communication"); pCommunicationNode != NULL; pCommunicationNode = pCommunicationNode->NextSibling("Communication"))
		//						{
		//							structMT2010_Communication tmpCommunication;
		//							xmlSubElement = pCommunicationNode->ToElement() ;
		//							if (xmlSubElement != NULL)
		//								pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//							if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//							{
		//								tmpCommunication.ID = pNodeTmp->ToElement()->GetText();
		//							}

		//							if (xmlSubElement != NULL)
		//								pNodeTmp = xmlSubElement->FirstChildElement("TypeID");
		//							if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//							{
		//								tmpCommunication.TypeID = pNodeTmp->ToElement()->GetText();
		//							}
		//							consignment.Consignee.Contact.Communication.push_back(tmpCommunication);
		//						}
		//					}
		//				}
		//			}


		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("Consignor")) != NULL)
		//			{
		//				TiXmlElement *pConsignorElement = NULL;
		//				pConsignorElement = xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.Consignor.ID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("Name");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.Consignor.Name = pNodeTmp->ToElement()->GetText();


		//				TiXmlNode *pAddressNode = NULL;
		//				if (xmlSubElement != NULL && (pAddressNode = xmlSubElement->FirstChild("Address")) != NULL)
		//				{
		//					xmlSubElement = pAddressNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("Line");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignor.Addresss.Line = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CityName");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignor.Addresss.CityName = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountrySubEntityID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignor.Addresss.CountrySubEntityID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountrySubEntityName");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignor.Addresss.CountrySubEntityName = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("PostcodeID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignor.Addresss.PostcodeID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountryCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.Consignor.Addresss.CountryCode = pNodeTmp->ToElement()->GetText();
		//					}
		//				}

		//				TiXmlNode *pCommunicationNode = NULL;
		//				for (pCommunicationNode = pConsignorElement->FirstChild("Communication"); pCommunicationNode != NULL; pCommunicationNode = pCommunicationNode->NextSibling("Communication"))
		//				{
		//					structMT2010_Communication tmpCommunication;
		//					xmlSubElement = pCommunicationNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.ID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("TypeID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.TypeID = pNodeTmp->ToElement()->GetText();
		//					}
		//					consignment.Consignor.Communication.push_back(tmpCommunication);
		//				}
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("NotifyParty")) != NULL)
		//			{
		//				TiXmlElement *pConsigneeElement = NULL;
		//				pConsigneeElement = xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.NotifyParty.ID = pNodeTmp->ToElement()->GetText();

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("Name");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.NotifyParty.Name = pNodeTmp->ToElement()->GetText();


		//				TiXmlNode *pAddressNode = NULL;
		//				if (xmlSubElement != NULL && (pAddressNode = xmlSubElement->FirstChild("Address")) != NULL)
		//				{
		//					xmlSubElement = pAddressNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("Line");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.NotifyParty.Addresss.Line = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CityName");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.NotifyParty.Addresss.CityName = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountrySubEntityID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.NotifyParty.Addresss.CountrySubEntityID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountrySubEntityName");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.NotifyParty.Addresss.CountrySubEntityName = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("PostcodeID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.NotifyParty.Addresss.PostcodeID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CountryCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignment.NotifyParty.Addresss.CountryCode = pNodeTmp->ToElement()->GetText();
		//					}
		//				}

		//				TiXmlNode *pCommunicationNode = NULL;
		//				for (pCommunicationNode = pConsigneeElement->FirstChild("Communication"); pCommunicationNode != NULL; pCommunicationNode = pCommunicationNode->NextSibling("Communication"))
		//				{
		//					structMT2010_Communication tmpCommunication;
		//					xmlSubElement = pCommunicationNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.ID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("TypeID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.TypeID = pNodeTmp->ToElement()->GetText();
		//					}
		//					consignment.NotifyParty.Communication.push_back(tmpCommunication);
		//				}
		//			}

		//			if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("UNDGContact")) != NULL)
		//			{
		//				TiXmlElement *pConsigneeElement = NULL;
		//				pConsigneeElement = xmlSubElement = pNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("Name");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					consignment.UNDGContact.Name = pNodeTmp->ToElement()->GetText();

		//				TiXmlNode *pCommunicationNode = NULL;
		//				for (pCommunicationNode = pConsigneeElement->FirstChild("Communication"); pCommunicationNode != NULL; pCommunicationNode = pCommunicationNode->NextSibling("Communication"))
		//				{
		//					structMT2010_Communication tmpCommunication;
		//					xmlSubElement = pCommunicationNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.ID = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("TypeID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						tmpCommunication.TypeID = pNodeTmp->ToElement()->GetText();
		//					}
		//					consignment.UNDGContact.Communication.push_back(tmpCommunication);
		//				}
		//			}


		//			TiXmlNode *pTransportEquipmentNode = NULL;
		//			TiXmlElement *xmlTransportEquipmentElement = NULL;
		//			for (pTransportEquipmentNode = xmlConsignmentElement->FirstChild("TransportEquipment"); pTransportEquipmentNode != NULL; pTransportEquipmentNode = pTransportEquipmentNode->NextSibling("TransportEquipment"))
		//			{
		//				structMT2010_TransportEquipment TransportEquipment;
		//				xmlTransportEquipmentElement = pTransportEquipmentNode->ToElement() ;
		//				if (xmlTransportEquipmentElement != NULL && (pNodeTmp = xmlTransportEquipmentElement->FirstChild("EquipmentIdentification")))
		//				{
		//					xmlSubElement = pNodeTmp->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						TransportEquipment.EquipmentIdentificationID = pNodeTmp->ToElement()->GetText();
		//					}
		//				}

		//				xmlSubElement = pTransportEquipmentNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("CharacteristicCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				{
		//					TransportEquipment.CharacteristicCode = pNodeTmp->ToElement()->GetText();
		//				}

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("SupplierPartyTypeCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				{
		//					TransportEquipment.SupplierPartyTypeCode = pNodeTmp->ToElement()->GetText();
		//				}

		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("FullnessCode");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				{
		//					TransportEquipment.FullnessCode = pNodeTmp->ToElement()->GetText();
		//				}

		//				TiXmlNode *pSealIDNode = NULL;
		//				for (pSealIDNode = xmlTransportEquipmentElement->FirstChild("SealID"); pSealIDNode != NULL; pSealIDNode = pSealIDNode->NextSibling("SealID"))
		//				{
		//					structMT2010_SealID sealID;
		//					xmlSubElement = pSealIDNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("AgencyCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						sealID.AgencyCode = pNodeTmp->ToElement()->GetText();
		//					}
		//					TransportEquipment.SealID.push_back(sealID);
		//				}

		//				consignment.TransportEquipment.push_back(TransportEquipment);
		//			}


		//			TiXmlNode *pConsignmentItemNode = NULL;
		//			TiXmlElement *xmlConsignmentItemElement = NULL;
		//			for (pConsignmentItemNode = xmlConsignmentElement->FirstChild("ConsignmentItem"); pConsignmentItemNode != NULL; pConsignmentItemNode = pConsignmentItemNode->NextSibling("ConsignmentItem"))
		//			{
		//				structMT2010_ConsignmentItem consignmentItem;
		//				xmlSubElement = pConsignmentItemNode->ToElement() ;
		//				if (xmlSubElement != NULL)
		//					pNodeTmp = xmlSubElement->FirstChildElement("SequenceNumeric");
		//				if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				{
		//					consignmentItem.SequenceNumeric = pNodeTmp->ToElement()->GetText();
		//				}


		//				xmlConsignmentItemElement = pConsignmentItemNode->ToElement() ;
		//				if (xmlConsignmentItemElement != NULL && (pNodeTmp = xmlConsignmentItemElement->FirstChild("ConsignmentItemPackaging")))
		//				{
		//					xmlSubElement = pNodeTmp->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("QuantityQuantity");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.QuantityQuantity = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("TypeCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.TypeCode = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("MarksNumbers");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.MarksNumbers = pNodeTmp->ToElement()->GetText();
		//					}
		//				}

		//				if (xmlConsignmentItemElement != NULL && (pNodeTmp = xmlConsignmentItemElement->FirstChild("Commodity")))
		//				{
		//					xmlSubElement = pNodeTmp->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CargoDescription");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.CargoDescription = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("UNDGCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.UNDGCode = pNodeTmp->ToElement()->GetText();
		//					}

		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("TariffClassificationCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.TariffClassificationCode = pNodeTmp->ToElement()->GetText();
		//					}
		//				}

		//				if (xmlConsignmentItemElement != NULL && (pNodeTmp = xmlConsignmentItemElement->FirstChild("AdditionalInformation")))
		//				{
		//					xmlSubElement = pNodeTmp->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("Content");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.Content = pNodeTmp->ToElement()->GetText();
		//					}
		//				}


		//				if (xmlConsignmentItemElement != NULL && (pNodeTmp = xmlConsignmentItemElement->FirstChild("GoodsMeasure")))
		//				{
		//					xmlSubElement = pNodeTmp->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("GrossMassMeasure");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.GrossMassMeasure = pNodeTmp->ToElement()->GetText();
		//					}
		//				}


		//				TiXmlNode *pEquipmentIdentificationNode = NULL;
		//				for (pEquipmentIdentificationNode = xmlConsignmentItemElement->FirstChild("EquipmentIdentification"); pEquipmentIdentificationNode != NULL; pEquipmentIdentificationNode = pEquipmentIdentificationNode->NextSibling("EquipmentIdentification"))
		//				{
		//					structMT2010_EquipmentIdentification identification;
		//					xmlSubElement = pEquipmentIdentificationNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						identification.ID = pNodeTmp->ToElement()->GetText();
		//					}
		//					consignmentItem.EquipmentIdentification.push_back(identification);
		//				}

		//				if (xmlConsignmentItemElement != NULL && (pNodeTmp = xmlConsignmentItemElement->FirstChild("CustomsProcedure")))
		//				{
		//					xmlSubElement = pNodeTmp->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("CurrentCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.CurrentCode = pNodeTmp->ToElement()->GetText();
		//					}
		//				}

		//				TiXmlNode *pUCRNode = NULL;
		//				for (pUCRNode = xmlConsignmentItemElement->FirstChild("UCR"); pUCRNode != NULL; pUCRNode = pUCRNode->NextSibling("UCR"))
		//				{
		//					structMT2010_UCR ucr;
		//					xmlSubElement = pUCRNode->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("ID");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						ucr.ID = pNodeTmp->ToElement()->GetText();
		//					}
		//					consignmentItem.UCR.push_back(ucr);
		//				}

		//				if (xmlConsignmentItemElement != NULL && (pNodeTmp = xmlConsignmentItemElement->FirstChild("Origin")))
		//				{
		//					xmlSubElement = pNodeTmp->ToElement() ;
		//					if (xmlSubElement != NULL)
		//						pNodeTmp = xmlSubElement->FirstChildElement("OriginCountryCode");
		//					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//					{
		//						consignmentItem.OriginCountryCode = pNodeTmp->ToElement()->GetText();
		//					}
		//				}


		//				consignment.ConsignmentItem.push_back(consignmentItem);
		//			}
		//			m_mt2010.Declaration.Consignment.push_back(consignment);
		//		}
		//		xmlDeclarationElement = pDeclarationNode->ToElement();
		//		if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("AdditionalInformation")) != NULL)
		//		{
		//			xmlSubElement = pNode->ToElement() ;
		//			if (xmlSubElement != NULL)
		//				pNodeTmp = xmlSubElement->FirstChildElement("Content");
		//			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
		//				m_mt2010.Declaration.AdditionalInformationContent = pNodeTmp->ToElement()->GetText();
		//		}
		//	}
		//}

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

int CCSM301::SaveToDB()
{
	if (m_csm301.MessageHead.MessageID.empty())
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

		if (!m_csm301.MessageHead.MessageID.empty())
		{
			//char OP_FLAG = 'F';
			//CString NOTE = "";
			CString strSql = "";
			char szSqlBuffer[2048] = {0};
			sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_CUSTOMREPORT_RESPONSE(MESSAGE_ID,FUNCTION_CODE,MESSAGE_TYPE, SENDER, RECEIVER, CREATE_TIME, INPUT_TIME, OP_FLAG, NOTE, VERSION) \
														values('%s', '%s', '%s', '%s', '%s', '%s', GetDate(), '%s', '%s', '%s')", \
														m_csm301.MessageHead.MessageID.c_str(),
														m_csm301.MessageHead.FunctionCode.c_str(),
														m_csm301.MessageHead.MessageType.c_str(),
														m_csm301.MessageHead.SenderID.c_str(),
														m_csm301.MessageHead.ReceiverID.c_str(),
														m_csm301.MessageHead.SendTime.c_str(),
														//OP_FLAG,
														//NOTE,
m_csm301.MessageBody.Declaration.REC.RecType.c_str(),
m_csm301.MessageBody.Declaration.REC.RecContent.c_str(),
														m_csm301.MessageHead.Version.c_str());

			strSql += szSqlBuffer;

			// 执行Sql语句
			nRet = database.RAExecuteNoRs((LPCTSTR)strSql);
			if (nRet == E_FAIL)
			{
				m_chOpFlag = 'F';
				m_chSEND_FLAG = '2';
				return 0;
			}
			m_chOpFlag = 'T';
			LOG1("insert into D_CUSTOMREPORT_RESPONSE, MESSAGE_ID=%s", m_csm301.MessageHead.MessageID.c_str());
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
	SaveBLOB();
	return 1;
}

void CCSM301::SendResponse()
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
	//std::string szLabel = CMessageType::CSM301;
	//CString strMessage = strResponse;
	////if ((!m_strMQIp.empty()) && (!m_strQueueName.empty()))
	////	int nRet = ob.SendStreamMessage(m_strMQIp.c_str(), m_strQueueName.c_str(), szLabel.c_str(), szLabel.c_str(), strMessage);

	//std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(m_strMQIp, m_strQueueName, szLabel, szLabel));
	//apBasic->SendMessage((LPCTSTR)strMessage, strMessage.GetLength());

	//const std::string strPreFlag = "";
	//// 海关平台返回的回执
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
	//		std::string szLabel			= CMessageType::CSM301;
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

	//CString strMESSAGE_ID	= "";
	//CString strOP_FLAG		= "";
	//CString strNOTE			= "";

	//if (!m_csm301.MessageHead.MessageID.empty())
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
	//	std::string szLabel			= CMessageType::CSM301;
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

int CCSM301::UpdateDB()
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

std::string CCSM301::GetFileName()
{
	return m_strFileName;
}

int CCSM301::SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	if (m_csm301.MessageHead.MessageID.empty())
		return 0;
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='%s' and INOROUTFLAG='1'", m_csm301.MessageHead.SenderID.c_str(), m_csm301.MessageHead.ReceiverID.c_str());

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

int CCSM301::SelectResponseInfo(CString &strMESSAGE_ID, CString &strOP_FLAG, CString &strNOTE)
{
	if (m_csm301.MessageHead.MessageID.empty())
		return 0;
	//try
	//{
	//	CStringArray mqPathArray;
	//	CString strMQPath = "";
	//	char szSql[1024] = {0};
	//	sprintf_s(szSql, sizeof(szSql), "select top 1 MESSAGE_ID,OP_FLAG,OP_NOTE from WH_RESPONSE_INFO where MESSAGE_ID='%s'", m_csm301.MessageHead.MessageID.c_str());

	//	//CString strConn = "";
	//	//strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
	//	strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
	//	//strConn += "sa";
	//	strConn += m_config.strSqlServerDataPwd.c_str();
	//	strConn += ";Persist Security Info=True;User ID=";
	//	//strConn += "sa";
	//	strConn += m_config.strSqlServerDataUser.c_str();
	//	strConn += ";Initial Catalog="; // WHHGWL";
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

	//	_RecordsetPtr oRs;
	//	oRs = database.RAExecuteRs(szSql);
	//	CComVariant val;

	//	if (!oRs->EndOfFile)
	//	{	
	//		val = oRs->Fields->Item[_variant_t("MESSAGE_ID")]->Value;
	//		if (val.vt != (VT_NULL))
	//		{
	//			strMESSAGE_ID = (char*)_bstr_t(val);	
	//		}

	//		val = oRs->Fields->Item[_variant_t("OP_FLAG")]->Value;
	//		if (val.vt != (VT_NULL))
	//		{
	//			strOP_FLAG = (char*)_bstr_t(val);	
	//		}

	//		val = oRs->Fields->Item[_variant_t("OP_NOTE")]->Value;
	//		if (val.vt != (VT_NULL))
	//		{
	//			strNOTE = (char*)_bstr_t(val);	
	//		}
	//	}
	//	oRs->Release();					
	//	oRs->Close();
	//}
	//catch(...)
	//{
	//	return 0;
	//}
	return 1;
}

int CCSM301::InsertResponseInfo()
{
	if (m_csm301.MessageHead.MessageID.empty())
		return 0;

	//try
	//{

	//	CString strConn = "";
	//	strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
	//	//strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
	//	////strConn += "sa";
	//	//strConn += m_config.strSqlServerDataPwd.c_str();
	//	//strConn += ";Persist Security Info=True;User ID=";
	//	////strConn += "sa";
	//	//strConn += m_config.strSqlServerDataUser.c_str();
	//	//strConn += ";Initial Catalog="; // CGS_DB";
	//	//strConn += m_config.strSqlServerDataName.c_str();
	//	//strConn += ";Data Source=";
	//	////strConn += "127.0.0.1";
	//	//strConn += m_config.strSqlServerDataServer.c_str();

	//	char OP_FLAG = 'F';

	//	if (m_csm301.MessageBody.Declaration.REC.RecType == "ERR" || m_csm301.MessageBody.Declaration.REC.RecType == "REF")
	//	{
	//		OP_FLAG = 'F';
	//	}
	//	else
	//	{
	//		OP_FLAG = 'T';
	//	}



	//	CRADatabase database;
	//	HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
	//	if (nRet == E_FAIL)
	//	{
	//		return 0;
	//	}

	//	CString strSql = "";
	//	char szSqlBuffer[2048] = {0};
	//	sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into WH_RESPONSE_INFO(MESSAGE_ID,OP_FLAG,OP_NOTE) values('%s', '%c', '%s')", m_csm301.MessageHead.MessageID.c_str(), OP_FLAG, m_csm301.MessageBody.Declaration.REC.RecContent.c_str());

	//	strSql += szSqlBuffer;

	//	// 执行Sql语句
	//	database.RAExecuteNoRs((LPCTSTR)strSql);
	//	LOG1("insert into WH_RESPONSE_INFO, MESSAGE_ID=%s", m_csm301.MessageHead.MessageID.c_str());
	//}
	//catch(...)
	//{
	//	return 0;
	//}

	return 1;
}

int CCSM301::DeleteResponseInfo()
{
	if (m_csm301.MessageHead.MessageID.empty())
		return 0;

	//try
	//{

	//	CString strConn = "";
	//	strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
	//	//strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=CGS_DB;Data Source=SANBAO-ZHOUHM
	//	////strConn += "sa";
	//	//strConn += m_config.strSqlServerDataPwd.c_str();
	//	//strConn += ";Persist Security Info=True;User ID=";
	//	////strConn += "sa";
	//	//strConn += m_config.strSqlServerDataUser.c_str();
	//	//strConn += ";Initial Catalog="; // CGS_DB";
	//	//strConn += m_config.strSqlServerDataName.c_str();
	//	//strConn += ";Data Source=";
	//	////strConn += "127.0.0.1";
	//	//strConn += m_config.strSqlServerDataServer.c_str();



	//	CRADatabase database;
	//	HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
	//	if (nRet == E_FAIL)
	//	{
	//		return 0;
	//	}

	//	CString strSql = "";
	//	char szSqlBuffer[2048] = {0};
	//	sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "delete from WH_RESPONSE_INFO where MESSAGE_ID = '%s'", m_csm301.MessageHead.MessageID.c_str());

	//	strSql += szSqlBuffer;

	//	// 执行Sql语句
	//	database.RAExecuteNoRs((LPCTSTR)strSql);
	//	LOG1("delete from WH_RESPONSE_INFO, MESSAGE_ID=%s", m_csm301.MessageHead.MessageID.c_str());
	//}
	//catch(...)
	//{
	//	return 0;
	//}

	return 1;
}

int CCSM301::SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='WEB' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='%s' and INOROUTFLAG='1'", m_csm301.MessageHead.SenderID.c_str(), m_csm301.MessageHead.ReceiverID.c_str());

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

int CCSM301::SaveBLOB()
{
	try
	{
		boost::shared_array<char> spXmlData;
		int nXmlLen = 0;

		CStdioFile file;
		if (!file.Open(m_strFileName.c_str(), CFile::modeRead|CFile::typeBinary))
		{
			return 0;
		}
		DWORD iLen = (DWORD)file.GetLength();
		if (iLen <= 0)
		{
			file.Close();
			return 0;
		}

		spXmlData.reset(new char[iLen + 1]);
		file.Seek(0,CFile::begin);
		file.Read(spXmlData.get(), iLen);
		file.Close();	
		spXmlData[iLen] = '\0';


		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select * from D_CUSTOMREPORT_RESPONSE where MESSAGE_ID='%s'", m_csm301.MessageHead.MessageID.c_str());

		CString strConn = "";
		strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		//strConn += "sa";
		strConn += m_config.strSqlServerDataPwd.c_str();
		strConn += ";Persist Security Info=True;User ID=";
		//strConn += "sa";
		strConn += m_config.strSqlServerDataUser.c_str();
		strConn += ";Initial Catalog="; // WHHGWL";
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
		oRs = database.RAExecuteRsForUpdate(szSql);
		if (oRs == NULL)
		{
			return 0;
		}
		CComVariant val;

		if (!oRs->EndOfFile)
		{	
			//VARIANT varOP_FLAG;
			VARIANT varblob;
			SAFEARRAY *psa = NULL;
			SAFEARRAYBOUND rgsabound[1];
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = iLen;
			psa = SafeArrayCreate(VT_UI1,1,rgsabound);

			if (psa == NULL)
				return 0;

			BYTE *pBuffer = (BYTE*)spXmlData.get();
			for(long i=0; i<(long)iLen; i++)
			{
				SafeArrayPutElement(psa,&i,pBuffer++);
			}
			varblob.vt = VT_ARRAY|VT_UI1;
			varblob.parray = psa;

			//varOP_FLAG.vt = VT_I1;
			//varOP_FLAG.cVal = m_chOpFlag;

			//oRs->Update();

			try
			{
				//oRs->GetFields()->GetItem("OP_FLAG")->AppendChunk(varOP_FLAG);
				//CString strOpFlag;
				//strOpFlag.Format("%c", m_chOpFlag);
				//oRs->PutCollect("OP_FLAG",(_variant_t)strOpFlag); 
				oRs->GetFields()->GetItem("REPORT")->AppendChunk(varblob);
			}
			catch(_com_error &e)
			{
				CString strMsg = "";
				strMsg = (char*) e.Description();
				LOG1("%s", strMsg);
			}

			if (psa != NULL)
			{
				::SafeArrayDestroyData(psa);
				::SafeArrayDestroyDescriptor(psa);
			}
			oRs->Update();
			oRs.Detach();
		}
	}
	catch(_com_error &e)
	{
		CString strMsg = "";
		strMsg = (char*) e.Description();
		LOG1("%s", strMsg);
		return 0;
	}

	//oRs->Release();					
	//oRs->Close();
	return 1;
}

void CCSM301::UTF8ToGBK( char *&szOut )
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

