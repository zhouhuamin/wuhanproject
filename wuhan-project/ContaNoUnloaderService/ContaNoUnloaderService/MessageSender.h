#pragma once

class CMessageSender
{
public:
	CMessageSender(void);
	virtual ~CMessageSender(void);

	virtual int SendMessage(const char *pMessage, int nLen) = 0;
};
