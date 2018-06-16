#include "StdAfx.h"
#include "CLMMessageParseStrategy.h"
#include "tinyxml/tinyxml.h"

CCLMMessageParseStrategy::CCLMMessageParseStrategy(void) : RECEIVER("")
{
}

CCLMMessageParseStrategy::~CCLMMessageParseStrategy(void)
{
}

void CCLMMessageParseStrategy::ParseXml(const std::string &fileName)
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

	pNode = doc.FirstChild("MESSAGE");
	if (pNode == NULL)
		return ;
	xmlRootElement = pNode->ToElement();
	if(xmlRootElement)
	{
		if ((pNode = xmlRootElement->FirstChild("MESSAGE_HEAD")) != NULL)
		{
			std::string strRECEIVER = "";

			xmlSubElement = pNode->ToElement() ;
			if (xmlSubElement != NULL)
				pNodeTmp = xmlSubElement->FirstChildElement("RECEIVER");
			if (pNodeTmp != NULL && pNodeTmp->ToElement() != NULL && pNodeTmp->ToElement()->GetText() != 0)
				strRECEIVER = pNodeTmp->ToElement()->GetText();
			RECEIVER = strRECEIVER;
		}
	}
	return;
}

std::string CCLMMessageParseStrategy::GetReceiver()
{
	return RECEIVER;
}

