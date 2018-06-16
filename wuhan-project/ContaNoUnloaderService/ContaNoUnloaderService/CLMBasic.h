#pragma once
#include "StructDef.h"

class CCLMBasic
{
public:
	CCLMBasic(void);
	virtual ~CCLMBasic(void);

public:
	virtual void	InitConfigInfo(structConfigInfo *pConfig) = 0;
	virtual void	SaveFileName(const std::string &strFileName) = 0;
	//virtual int		SaveLocal(BYTE *pMessage, int iLen) = 0;
	virtual void	ParseMessage() = 0;
	virtual int		SaveToDB() = 0;
	virtual void    SendResponse() = 0;
	virtual int		UpdateDB() = 0;
	virtual std::string GetFileName()	= 0;      

public:
	void ProcessMessage(structConfigInfo *pConfig, const std::string &strFileName);

protected:
	std::string m_strFileName;
	std::string m_strMQIp;
	std::string m_strQueueName;
	structConfigInfo m_config;
};
