#pragma once
#include "messagesender.h"

class CMSMQSender :
	public CMessageSender
{
public:
	CMSMQSender(void);
	CMSMQSender(std::string strIP, std::string pszQueueName, std::string strQueueLabel, std::string szLabel);
	virtual ~CMSMQSender(void);

	virtual int SendMessage(const char *pMessage, int nLen);

protected:
	std::string strIP;
	std::string pszQueueName;
	std::string strQueueLabel;
	std::string szLabel;
};
