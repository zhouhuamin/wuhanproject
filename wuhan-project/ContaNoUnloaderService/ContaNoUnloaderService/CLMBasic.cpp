#include "StdAfx.h"
#include "CLMBasic.h"

CCLMBasic::CCLMBasic(void) : m_strFileName(""),m_strMQIp(""),m_strQueueName(""),m_config()
{
}

CCLMBasic::~CCLMBasic(void)
{
}

void CCLMBasic::ProcessMessage(structConfigInfo *pConfig, const std::string &strFileName)
{
	InitConfigInfo(pConfig);
	SaveFileName(strFileName);
	//SaveLocal(pMessage, iLen);
	ParseMessage();
	SaveToDB();
	SendResponse();
	UpdateDB();
}
