#pragma once
#include <string>

class CMessageParseStrategy
{
public:
	CMessageParseStrategy(void);
	virtual ~CMessageParseStrategy(void);

	virtual void			ParseXml(const std::string &fileName) = 0;
	virtual std::string		GetReceiver() = 0;
};
