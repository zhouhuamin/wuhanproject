#include "StdAfx.h"
#include "MSMQSender.h"
#include "msmq/MSMQApiWrapper.h"

CMSMQSender::CMSMQSender(void) :strIP(""),pszQueueName(""),strQueueLabel(""),szLabel("")
{
}

CMSMQSender::CMSMQSender(std::string strIP, std::string pszQueueName, std::string strQueueLabel, std::string szLabel)
{
	this->strIP			= strIP;
	this->pszQueueName	= pszQueueName;
	this->strQueueLabel	= strQueueLabel;
	this->szLabel		= szLabel;
}

CMSMQSender::~CMSMQSender(void)
{
}

int CMSMQSender::SendMessage(const char *pMessage, int nLen)
{
	// ·¢ËÍMSMQ
	CMSMQApiWrapper ob;
	if ((!strIP.empty()) && (!pszQueueName.empty()))
	{
		int nRet = ob.SendStreamMessage2(strIP.c_str(), pszQueueName.c_str(), szLabel.c_str(), szLabel.c_str(), pMessage, nLen);
	}

	return 1;
}