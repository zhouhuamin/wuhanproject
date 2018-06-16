#pragma once
#include "clmbasic.h"
#include "StructDef.h"
#include <string>
#include <vector>

class CCLM32101 :
	public CCLMBasic
{
public:
	CCLM32101(void);
	virtual ~CCLM32101(void);

public:
	virtual void	InitConfigInfo(structConfigInfo *pConfig);
	virtual void	SaveFileName(const std::string &strFileName);
	//virtual int		SaveLocal(BYTE *pMessage, int iLen);
	virtual void	ParseMessage();
	virtual int		SaveToDB();
	virtual void	SendResponse();
	virtual int		UpdateDB();
	virtual std::string GetFileName();

private:
	int SelectMQ();
	void UTF8ToGBK( char *&szOut );

private:
	structMESSAGE_HEAD				m_messageHead;
	//structBILL_INFO_CLM10011		m_billInfo;
	structRESPOND_INFO				m_respondInfo;

	char		m_chOpFlag;
	char        m_chSEND_FLAG;
	std::string m_strNote;
	//std::string m_strFileName;
	//std::string m_strMQIp;
	//std::string m_strQueueName;
	//structConfigInfo m_config;
	structSHIP_INFO_CLM32101					m_shipInfo;
	std::vector<structBILL_INFO_CLM32101>		m_billInfo;
};
