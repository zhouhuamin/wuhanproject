#include "StdAfx.h"
#include "MT4101.h"

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

CMT4101::CMT4101(void) : m_chOpFlag('F'), m_chSEND_FLAG('1'), m_strNote(""),m_mt4101(),m_mtResponse(),m_nFlag(0)
{
}

CMT4101::~CMT4101(void)
{
}

void CMT4101::InitConfigInfo(structConfigInfo *pConfig)
{
	m_config.strQueueIP		= pConfig->strQueueIP;
	m_config.strQueueName	= pConfig->strQueueName;
	m_config.strQueueLabel	= pConfig->strQueueLabel;

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

void CMT4101::SaveFileName(const std::string &strFileName)
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
//int CMT4101::SaveLocal(BYTE *pMessage, int iLen)
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

void CMT4101::ParseMessage()
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
		TiXmlElement* xmlDeclarationElement = NULL;
		TiXmlElement* xmlSubElement			= NULL;
		TiXmlElement* xmlSonElement			= NULL;
		TiXmlElement *xmlConsignmentElement = NULL;


		TiXmlNode * pNode					= NULL;
		TiXmlNode *pNodeTmp					= NULL;
		TiXmlNode *pManifestNode			= NULL;
		TiXmlNode *pHeadNode				= NULL;
		TiXmlNode *pDeclarationNode			= NULL;
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
				m_mt4101.Head.MessageID		= MESSAGE_ID;
				m_mt4101.Head.FunctionCode	= FUNCTION_CODE;
				m_mt4101.Head.MessageType	= MESSAGE_TYPE;
				m_mt4101.Head.SenderID		= SENDER;
				m_mt4101.Head.ReceiverID	= RECEIVE;
				m_mt4101.Head.SendTime		= MESSAGE_CREATE_TIME;
				m_mt4101.Head.Version		= strVERSION;
			}

			if ((pDeclarationNode = xmlRootElement->FirstChild("Declaration")) != NULL)
			{
				xmlDeclarationElement = pDeclarationNode->ToElement();
				if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("Carrier")) != NULL)
				{
					xmlSubElement = pNode->ToElement() ;
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("ID");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mt4101.Declaration.Carrier.ID = pNodeTmp->ToElement()->GetText();
				}

				if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("BorderTransportMeans")) != NULL)
				{
					xmlSubElement = pNode->ToElement() ;
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("JourneyID");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mt4101.Declaration.BorderTransportMeans.JourneyID = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("TypeCode");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mt4101.Declaration.BorderTransportMeans.TypeCode = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("IEFlag");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mt4101.Declaration.BorderTransportMeans.IEFlag = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("ID");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mt4101.Declaration.BorderTransportMeans.ID = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("Name");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mt4101.Declaration.BorderTransportMeans.Name = pNodeTmp->ToElement()->GetText();
				}
			}

			if (xmlDeclarationElement != NULL)
			{
				for (pConsignmentNode = xmlDeclarationElement->FirstChild("Consignment"); pConsignmentNode != NULL; pConsignmentNode = pConsignmentNode->NextSibling("Consignment"))
				{
					structMT4101_Consignment consignment;
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

					if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("LoadingLocation")) != NULL)
					{
						xmlSubElement = pNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("ID");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							consignment.LoadingLocation.ID = pNodeTmp->ToElement()->GetText();

						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("LoadingDate");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							consignment.LoadingLocation.LoadingDate = pNodeTmp->ToElement()->GetText();	
					}

					if (xmlConsignmentElement != NULL && (pNode = xmlConsignmentElement->FirstChild("ConsignmentPackaging")) != NULL)
					{
						xmlSubElement = pNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("QuantityQuantity");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							consignment.ConsignmentPackaging.QuantityQuantity = pNodeTmp->ToElement()->GetText();

						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("TypeCode");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							consignment.ConsignmentPackaging.TypeCode = pNodeTmp->ToElement()->GetText();
					}

					pNodeTmp = NULL;
					xmlSubElement = pConsignmentNode->ToElement() ;
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("TotalGrossMassMeasure");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						consignment.TotalGrossMassMeasure = pNodeTmp->ToElement()->GetText();

					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("TransportSplitIndicator");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						consignment.TransportSplitIndicator = pNodeTmp->ToElement()->GetText();		

					TiXmlNode *pTransportEquipmentNode = NULL;
					TiXmlElement *xmlTransportEquipmentElement = NULL;
					for (pTransportEquipmentNode = xmlConsignmentElement->FirstChild("TransportEquipment"); pTransportEquipmentNode != NULL; pTransportEquipmentNode = pTransportEquipmentNode->NextSibling("TransportEquipment"))
					{
						structMT4101_TransportEquipment TransportEquipment;

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

						xmlSubElement = pTransportEquipmentNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("TypeFlag");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						{
							TransportEquipment.TypeFlag = pNodeTmp->ToElement()->GetText();
						}	

						xmlSubElement = pTransportEquipmentNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("OperationType");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						{
							TransportEquipment.OperationType = pNodeTmp->ToElement()->GetText();
						}	

						xmlSubElement = pTransportEquipmentNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("CharacteristicCode");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						{
							TransportEquipment.CharacteristicCode = pNodeTmp->ToElement()->GetText();
						}	

						xmlSubElement = pTransportEquipmentNode->ToElement() ;
						if (xmlSubElement != NULL)
							pNodeTmp = xmlSubElement->FirstChildElement("FullnessCode");
						if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						{
							TransportEquipment.FullnessCode = pNodeTmp->ToElement()->GetText();
						}	

						TiXmlNode *pSealIDNode = NULL;
						for (pSealIDNode = xmlTransportEquipmentElement->FirstChild("SealID"); pSealIDNode != NULL; pSealIDNode = pSealIDNode->NextSibling("SealID"))
						{
							structMT4101_SealID sealID;
							xmlSubElement = pSealIDNode->ToElement() ;
							if (pSealIDNode != NULL && pSealIDNode->ToElement() != NULL && pSealIDNode->ToElement()->GetText() != 0)
							{
								sealID.SealID = pSealIDNode->ToElement()->GetText();
							}

							if (xmlSubElement != NULL)
							{
								if (xmlSubElement->Attribute("AgencyCode") != NULL)
									sealID.AgencyCode = xmlSubElement->Attribute("AgencyCode");
								TransportEquipment.SealID.push_back(sealID);
							}
						}						

						consignment.TransportEquipment.push_back(TransportEquipment);
					}

					TiXmlNode *pConsignmentItemNode = NULL;
					TiXmlElement *xmlConsignmentItemElement = NULL;
					for (pConsignmentItemNode = xmlConsignmentElement->FirstChild("ConsignmentItem"); pConsignmentItemNode != NULL; pConsignmentItemNode = pConsignmentItemNode->NextSibling("ConsignmentItem"))
					{
						structMT4101_ConsignmentItem consignmentItem;
						xmlConsignmentItemElement = pConsignmentItemNode->ToElement() ;
						if (xmlConsignmentItemElement != NULL && (pNodeTmp = xmlConsignmentItemElement->FirstChild("Commodity")))
						{
							xmlSubElement = pNodeTmp->ToElement() ;
							if (xmlSubElement != NULL)
								pNodeTmp = xmlSubElement->FirstChildElement("CargoDescription");
							if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
							{
								consignmentItem.Commodity.CargoDescription = pNodeTmp->ToElement()->GetText();
							}
						}

						consignment.ConsignmentItem.push_back(consignmentItem);
					}

					m_mt4101.Declaration.Consignment.push_back(consignment);
				}

				xmlDeclarationElement = pDeclarationNode->ToElement();
				if (xmlDeclarationElement != NULL && (pNode = xmlDeclarationElement->FirstChild("AdditionalInformation")) != NULL)
				{
					xmlSubElement = pNode->ToElement() ;
					if (xmlSubElement != NULL)
						pNodeTmp = xmlSubElement->FirstChildElement("Content");
					if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
						m_mt4101.Declaration.AdditionalInformation.Content = pNodeTmp->ToElement()->GetText();
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
				m_nFlag = 1;
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

int CMT4101::SaveToDB()
{
	return 1;
	if (m_mtResponse.Head.MessageID.empty())
		return 0;

	if (!m_mtResponse.Declaration.BorderTransportMeans.JourneyID.empty())
	{
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

			if (!m_mtResponse.Head.MessageID.empty())
			{

				char szTime01[100] = {0};
				int nYear1 = 0;
				int nMonth1 = 0;
				int nDay1 = 0;
				int nHour1 = 0;
				int nMinute1 = 0;
				int nSecond1 = 0;
				int nReserve1 = 0;

				CString strTime1;
				strTime1 = m_mtResponse.Head.SendTime.c_str();
				strTime1.TrimLeft();
				strTime1.TrimRight();


				if (strTime1.IsEmpty())
				{
					memset(szTime01, 0x00, sizeof(szTime01));
				}
				else
				{
					memcpy(szTime01, m_mtResponse.Head.SendTime.c_str(), m_mtResponse.Head.SendTime.size());
					sscanf_s(szTime01, "%04d%02d%02d%02d%02d%02d%03d", &nYear1, &nMonth1, &nDay1, &nHour1, &nMinute1, &nSecond1, &nReserve1); // C4996

					memset(szTime01, 0x00, sizeof(szTime01));
					sprintf_s(szTime01, sizeof(szTime01), "%04d-%02d-%02d %02d:%02d:%02d", nYear1, nMonth1, nDay1, nHour1, nMinute1, nSecond1);
				}


				char OP_FLAG = 'F';
				CString NOTE = "";
				CString strSql = "";
				char szSqlBuffer[2048] = {0};
				sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_CUSTOMREPORT_RESPONSE(MESSAGE_ID,FUNCTION_CODE,MESSAGE_TYPE, SENDER, RECEIVER, CREATE_TIME, INPUT_TIME, OP_FLAG, NOTE, VERSION) \
values('%s', '%s', '%s', '%s', '%s', '%s', GetDate(), '%c', '%s', '%s')", \
m_mtResponse.Head.MessageID.c_str(),
m_mtResponse.Head.FunctionCode.c_str(),
m_mtResponse.Head.MessageType.c_str(),
m_mtResponse.Head.SenderID.c_str(),
m_mtResponse.Head.ReceiverID.c_str(),
szTime01, //m_mtResponse.Head.SendTime.c_str(),
OP_FLAG,
NOTE,
m_mtResponse.Head.Version.c_str());

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
				LOG1("insert into D_CUSTOMREPORT_RESPONSE, MESSAGE_ID=%s", m_mtResponse.Head.MessageID.c_str());
			}
		}
		catch(...)
		{
			return 0;
		}

		m_chSEND_FLAG = '2';
		SaveBLOB();

		return 1;
	}

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
		if (!m_mt4101.Head.MessageID.empty())
		{
			char TypeCode = ' ';
			char LINE_FLAG	= ' ';
			if (!m_mt4101.Declaration.BorderTransportMeans.TypeCode.empty())
				TypeCode = m_mt4101.Declaration.BorderTransportMeans.TypeCode[0];
			CString strSql = "";
			char szSqlBuffer[2048] = {0};

			char I_E_FLAG = ' ';
			if (!m_mt4101.Declaration.BorderTransportMeans.IEFlag.empty())
			{
				I_E_FLAG = m_mt4101.Declaration.BorderTransportMeans.IEFlag[0];
			}

			sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_LOADING_HEAD(MESSAGE_ID,TRANS_NAME,TRANS_CODE,JOURNEY_ID,TYPE_CODE,LINE_FLAG,CARRIER_ID,CONTENT,INPUT_TIME,I_E_FLAG) \
														values('%s', '%s', '%s', '%s', '%c', '%c', '%s', '%s', GetDate(), '%c')",
														m_mt4101.Head.MessageID.c_str(), 
														
														m_mt4101.Declaration.BorderTransportMeans.Name.c_str(), 
														m_mt4101.Declaration.BorderTransportMeans.ID.c_str(), 
														m_mt4101.Declaration.BorderTransportMeans.JourneyID.c_str(), 
														TypeCode,
														LINE_FLAG,
														m_mt4101.Declaration.Carrier.ID.c_str(),
														
														m_mt4101.Declaration.AdditionalInformation.Content.c_str(),
														I_E_FLAG
														);
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
			//LOG1("insert into D_REACH_HEAD, MESSAGE_ID=%s", m_mt3101.Head.MessageID.c_str());

			for (size_t i = 0; i < m_mt4101.Declaration.Consignment.size(); ++i)
			{
				double DISCHARGE_CODE = atof(m_mt4101.Declaration.Consignment[i].ConsignmentPackaging.QuantityQuantity.c_str());
				double TOTAL_GROSS_MAME	= atof(m_mt4101.Declaration.Consignment[i].TotalGrossMassMeasure.c_str());
				sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_LOADING_BILL(MESSAGE_ID,BILL_NO,LOADING_ID,LOADING_DATE,PACK_NO,TYPE_CODE,GROSS_WT,TRANS_FLAG) \
															values('%s', '%s', '%s', '%s', '%.5f', '%s', '%.5f', '%s')",
															m_mt4101.Head.MessageID.c_str(), 
															m_mt4101.Declaration.Consignment[i].TransportContractDocument.ID.c_str(), 
															m_mt4101.Declaration.Consignment[i].LoadingLocation.ID.c_str(), 
															m_mt4101.Declaration.Consignment[i].LoadingLocation.LoadingDate.c_str(),
															DISCHARGE_CODE,
															m_mt4101.Declaration.Consignment[i].ConsignmentPackaging.TypeCode.c_str(),
															TOTAL_GROSS_MAME,
															m_mt4101.Declaration.Consignment[i].TransportSplitIndicator.c_str());

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
			//LOG1("insert into D_REACH_BILL, MESSAGE_ID=%s", m_mt3101.Head.MessageID.c_str());



			for (size_t i = 0; i < m_mt4101.Declaration.Consignment.size(); ++i)
			{
				for (size_t j = 0; j < m_mt4101.Declaration.Consignment[i].ConsignmentItem.size(); ++j)
				{
					sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_LOADING_GOODS_DETAIL(MESSAGE_ID,BILL_NO,CARGO_DESCRIPTION) values('%s', '%s', '%s')",
																m_mt4101.Head.MessageID.c_str(), 
																m_mt4101.Declaration.Consignment[i].TransportContractDocument.ID.c_str(), 
																m_mt4101.Declaration.Consignment[i].ConsignmentItem[j].Commodity.CargoDescription.c_str());

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
			}
			//LOG1("insert into D_REACH_GOODS_DETAIL, MESSAGE_ID=%s", m_mt3101.Head.MessageID.c_str());

			for (size_t i = 0; i < m_mt4101.Declaration.Consignment.size(); ++i)
			{
				for (size_t j = 0; j < m_mt4101.Declaration.Consignment[i].TransportEquipment.size(); ++j)
				{
					std::string strSealID = "";
					std::string AGENCY_CODE = "";
					std::string strSEAL_TYPE	= "";
					std::vector<structMT4101_SealID> &sealID = m_mt4101.Declaration.Consignment[i].TransportEquipment[j].SealID;
					if (sealID.size() > 0)
					{
						strSealID = sealID[0].SealID;
						CString strTmp = strSealID.c_str();
						int index = strTmp.Find("/");
						if (index != -1)
						{
							strSealID = strTmp.Left(index);
							strSEAL_TYPE = (LPCTSTR)strTmp.Mid(index + 1);
						}
						else
						{
							strSEAL_TYPE = "";
						}


						AGENCY_CODE = sealID[0].AgencyCode;
					}

					char CHARACTERISTIC_CODE = ' ';
					if (!m_mt4101.Declaration.Consignment[i].TransportEquipment[j].CharacteristicCode.empty())
					{
						CHARACTERISTIC_CODE = m_mt4101.Declaration.Consignment[i].TransportEquipment[j].CharacteristicCode[0];
					}

					char FULLNESSCODE = ' ';
					if (!m_mt4101.Declaration.Consignment[i].TransportEquipment[j].FullnessCode.empty())
					{
						FULLNESSCODE = m_mt4101.Declaration.Consignment[i].TransportEquipment[j].FullnessCode[0];
					}

					char TYPE_FLAG = ' ';
					if (!m_mt4101.Declaration.Consignment[i].TransportEquipment[j].TypeFlag.empty())
					{
						TYPE_FLAG = m_mt4101.Declaration.Consignment[i].TransportEquipment[j].TypeFlag[0];
					}

					char OperationType = ' ';
					if (!m_mt4101.Declaration.Consignment[i].TransportEquipment[j].OperationType.empty())
					{
						OperationType = m_mt4101.Declaration.Consignment[i].TransportEquipment[j].OperationType[0];
					}
					sprintf_s(szSqlBuffer, sizeof(szSqlBuffer), "insert into D_LOADING_CONTA(MESSAGE_ID,BILL_NO,CONTA_NO,CHARACTERISTIC_CODE,FULLNESSCODE,SEAL_ID,SEAL_TYPE,AGENCY_CODE, TYPE_FLAG,OperationType) \
																values('%s', '%s', '%s', '%c', '%c', '%s', '%s', '%s', '%c', '%c')",
																m_mt4101.Head.MessageID.c_str(), 
																m_mt4101.Declaration.Consignment[i].TransportContractDocument.ID.c_str(), 
																m_mt4101.Declaration.Consignment[i].TransportEquipment[j].EquipmentIdentification.ID.c_str(), 
																CHARACTERISTIC_CODE, 
																FULLNESSCODE, 
																strSealID.c_str(),
																strSEAL_TYPE.c_str(),
																AGENCY_CODE.c_str(), 
																TYPE_FLAG,
																OperationType);

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
			}

			//LOG1("insert into D_REACH_CONTA, MESSAGE_ID=%s", m_mt3101.Head.MessageID.c_str());
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

void CMT4101::SendResponse()
{
	LOG3("Enter CMT4101::SendResponse, MessageType:%s, Sender:%s, Receiver:%s", m_mt4101.Head.MessageType.c_str(), m_mt4101.Head.SenderID.c_str(), m_mt4101.Head.ReceiverID.c_str());
	//// delete file
	//if (m_strFileName != "")
	//{
	//	if (GetFileAttributes(m_strFileName.c_str()) != -1)
	//		CFile::Remove(m_strFileName.c_str());
	//}

	//if (!m_mtResponse.Declaration.BorderTransportMeans.JourneyID.empty())
	//{
	//	// 不产生回执了
	//}

	//return;
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
	//std::string szLabel = CMessageType::MT4101;
	//CString strMessage = strResponse;
	////if ((!m_strMQIp.empty()) && (!m_strQueueName.empty()))
	////	int nRet = ob.SendStreamMessage(m_strMQIp.c_str(), m_strQueueName.c_str(), szLabel.c_str(), szLabel.c_str(), strMessage);

	//std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(m_strMQIp, m_strQueueName, szLabel, szLabel));
	//apBasic->SendMessage((LPCTSTR)strMessage, strMessage.GetLength());

	std::string strPreFlag = "";
	// WEB应用发送
	//if (strPreFlag == "WEB")
	if (!m_mtResponse.Declaration.BorderTransportMeans.JourneyID.empty())
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
			std::string szLabel			= ""; // "WEB-";
			szLabel += CMessageType::MT4101;
			szLabel += "-";
			szLabel += m_mt4101.Head.MessageID;
			std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
			apBasic->SendMessage(spXmlData.get(), iLen);
		}

		// delete file
		if (m_strFileName != "")
		{
			if (GetFileAttributes(m_strFileName.c_str()) != -1)
				CFile::Remove(m_strFileName.c_str());
		}

		return;
	}

	if (m_nFlag == 1)
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
			std::string szLabel			= ""; // "WEB-";
			szLabel += CMessageType::MT4101;
			szLabel += "-";
			szLabel += m_mt4101.Head.MessageID;
			std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
			apBasic->SendMessage(spXmlData.get(), iLen);
		}

		// delete file
		if (m_strFileName != "")
		{
			if (GetFileAttributes(m_strFileName.c_str()) != -1)
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
	//	std::string szLabel			= CMessageType::MT4101;
	//	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	//	apBasic->SendMessage(spXmlData.get(), iLen);
	//}

	// 发送MSMQ
	std::string strQueueName	= m_config.strBackerQueueName;
	std::string strMQIp			= m_config.strBackerQueueIP;
	std::string szLabel			= CMessageType::MT4101;
	std::auto_ptr<CMessageSender> apBasic(new CMSMQSender(strMQIp, strQueueName, szLabel, szLabel));
	apBasic->SendMessage(spXmlData.get(), iLen);

	// delete file
	if (m_strFileName != "")
	{
		if (GetFileAttributes(m_strFileName.c_str()) != -1)
			CFile::Remove(m_strFileName.c_str());
	}
}

int CMT4101::UpdateDB()
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

std::string CMT4101::GetFileName()
{
	return m_strFileName;
}

int CMT4101::SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	if (m_mt4101.Head.MessageID.empty())
		return 0;

	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='ALL' and INOROUTFLAG='0'", m_mt4101.Head.ReceiverID.c_str());

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

int CMT4101::SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray)
{
	try
	{
		CStringArray mqPathArray;
		CString strMQPath = "";
		char szSql[1024] = {0};
		//sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='WEB' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='WEB' and INOROUTFLAG='0'", m_mt4101.Head.ReceiverID.c_str());
		//sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where MQMESSAGETYPE='ALL' and CUSTOMS_CODE='%s' and ENTERPRISE_CODE='%s' and INOROUTFLAG=1", m_mt4101.Head.SenderID.c_str(), m_mt4101.Head.ReceiverID.c_str());
		sprintf_s(szSql, sizeof(szSql), "select MQPATH from WH_MQPATH_CONFIG where ENTERPRISE_CODE='%s' and INOROUTFLAG=1", m_mt4101.Head.ReceiverID.c_str());

		//CString strConn = "";
		//strConn = "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=C:\\Njsamples\\CGS_DB\\WHPROXY_DB.accdb;"; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		////strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
		//////strConn += "sa";
		////strConn += m_config.strSqlServerDataPwd.c_str();
		////strConn += ";Persist Security Info=True;User ID=";
		//////strConn += "sa";
		////strConn += m_config.strSqlServerDataUser.c_str();
		////strConn += ";Initial Catalog="; // WHHGWL";
		////strConn += m_config.strSqlServerDataName.c_str();
		////strConn += ";Data Source=";
		//////strConn += "127.0.0.1";
		////strConn += m_config.strSqlServerDataServer.c_str();

		//CRADatabase database;
		//HRESULT nRet = database.RAConnect(LPCTSTR(strConn));
		//if (nRet == E_FAIL)
		//{
		//	return 0;
		//}

		//_RecordsetPtr oRs;
		//oRs = database.RAExecuteRs(szSql);
		//CComVariant val;

		//while (!oRs->EndOfFile)
		//{	
		//	val = oRs->Fields->Item[_variant_t("MQPATH")]->Value;
		//	if (val.vt != (VT_NULL))
		//	{
		//		strMQPath = (char*)_bstr_t(val);	
		//		mqPathArray.Add(strMQPath);
		//	}

		//	oRs->MoveNext();
		//}
		//oRs->Release();					
		//oRs->Close();



		BOOL bRet = FALSE;
		CDbSQLite sqlite; 
		bRet = sqlite.Open("C:\\Njsamples\\CGS_DB\\WEB_PROXY_DB.db");
		if (!bRet)
		{
			LOG("Can't open WEB_PROXY_DB.db");
			return 0;
		}

		//CSqlStatement *stmt = sqlite.Statement("SELECT MQPATH FROM WH_MQPATH_CONFIG");
		CSqlStatement *stmt = sqlite.Statement(szSql);
		if (stmt != NULL)
		{
			//CString szText  = "";
			//int nFields = stmt->Fields();  //取得数据库表的列数
			//szText = stmt->FieldName(0);        //得到列名

			while (stmt->NextRow())
			{
				CString strText = "";
				strText = stmt->ValueString(0);        //得到列值
				mqPathArray.Add(strText);
				// TRACE("%s\n", strText);
			}
		}

		delete stmt;


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

int CMT4101::SaveBLOB()
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
		sprintf_s(szSql, sizeof(szSql), "select * from D_CUSTOMREPORT_RESPONSE where MESSAGE_ID='%s'", m_mtResponse.Head.MessageID.c_str());

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
				CString strOpFlag;
				strOpFlag.Format("%c", m_chOpFlag);
				oRs->PutCollect("OP_FLAG",(_variant_t)strOpFlag); 
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

