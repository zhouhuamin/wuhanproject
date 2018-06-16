#include "StdAfx.h"
#include "CSMMessageParseStrategy.h"
#include "tinyxml/tinyxml.h"

CCSMMessageParseStrategy::CCSMMessageParseStrategy(void) : RECEIVER("")
{
}

CCSMMessageParseStrategy::~CCSMMessageParseStrategy(void)
{
}

void CCSMMessageParseStrategy::ParseXml(const std::string &fileName)
{
	std::string strFileName(fileName);
	TiXmlDocument doc(strFileName.c_str());
	doc.LoadFile();

	TiXmlNode*    node = 0;
	TiXmlElement* itemElement = 0;
	TiXmlElement* xmlRootElement = NULL;
	TiXmlElement* xmlListElement = NULL;
	TiXmlElement* xmlSubElement = NULL;
	TiXmlElement* xmlSonElement = NULL;
	TiXmlNode * pNode = NULL;
	TiXmlNode *pNodeTmp = NULL;

	pNode = doc.FirstChild("Message");
	if (pNode == NULL)
		return ;
	xmlRootElement = pNode->ToElement();
	if(xmlRootElement)
	{
		if ((pNode = xmlRootElement->FirstChild("MessageHead")) != NULL)
		{
			std::string strRECEIVER = "";

			xmlSubElement = pNode->ToElement() ;
			if (xmlSubElement != NULL)
				pNodeTmp = xmlSubElement->FirstChildElement("ReceiverID");
			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
				strRECEIVER = pNodeTmp->ToElement()->GetText();
			RECEIVER = strRECEIVER;
		}
	}
	return;
}

std::string CCSMMessageParseStrategy::GetReceiver()
{
	return RECEIVER;
}

