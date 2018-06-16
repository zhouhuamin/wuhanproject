#pragma once
#include "messageparsestrategy.h"
#include <string>

class CCLMMessageParseStrategy :
	public CMessageParseStrategy
{
public:
	CCLMMessageParseStrategy(void);
	virtual ~CCLMMessageParseStrategy(void);

	virtual void	ParseXml(const std::string &fileName);
	virtual std::string		GetReceiver();

private:
	std::string RECEIVER;
};
