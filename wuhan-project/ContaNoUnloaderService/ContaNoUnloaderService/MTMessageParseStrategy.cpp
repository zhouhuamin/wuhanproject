#include "StdAfx.h"
#include "MTMessageParseStrategy.h"
#include "tinyxml/tinyxml.h"

CMTMessageParseStrategy::CMTMessageParseStrategy(void) : RECEIVER("")
{
}

CMTMessageParseStrategy::~CMTMessageParseStrategy(void)
{
}

void CMTMessageParseStrategy::ParseXml(const std::string &fileName)
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

	pNode = doc.FirstChild("Manifest");
	if (pNode == NULL)
		return ;
	xmlRootElement = pNode->ToElement();
	if(xmlRootElement)
	{
		if ((pNode = xmlRootElement->FirstChild("Head")) != NULL)
		{
			std::string strRECEIVER = "";

			xmlSubElement = pNode->ToElement() ;
			if (xmlSubElement != NULL)
				pNodeTmp = xmlSubElement->FirstChildElement("ReceiverId");
			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
				strRECEIVER = pNodeTmp->ToElement()->GetText();
			RECEIVER = strRECEIVER;
		}
	}
	return;
}

std::string CMTMessageParseStrategy::GetReceiver()
{
	return RECEIVER;
}


