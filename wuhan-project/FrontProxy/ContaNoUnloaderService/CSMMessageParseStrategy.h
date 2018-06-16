#pragma once
#include "messageparsestrategy.h"
#include <string>

class CCSMMessageParseStrategy :
	public CMessageParseStrategy
{
public:
	CCSMMessageParseStrategy(void);
	virtual ~CCSMMessageParseStrategy(void);

	virtual void	ParseXml(const std::string &fileName);
	virtual std::string		GetReceiver();
private:
	std::string RECEIVER;
};
