#pragma once
#include "messageparsestrategy.h"
#include <string>

class CMTMessageParseStrategy :
	public CMessageParseStrategy
{
public:
	CMTMessageParseStrategy(void);
	virtual ~CMTMessageParseStrategy(void);

	virtual void	ParseXml(const std::string &fileName);
	virtual std::string		GetReceiver();

private:
	std::string RECEIVER;
};
