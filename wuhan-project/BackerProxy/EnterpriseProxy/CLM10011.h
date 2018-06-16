#pragma once
#include "StructDef.h"
#include "clmbasic.h"
#include <string>
#include <vector>

// CLM10011-集装箱提箱指令
class CCLM10011 :
	public CCLMBasic
{
public:
	CCLM10011(void);
	virtual ~CCLM10011(void);

public:
	virtual void	InitConfigInfo(structConfigInfo *pConfig);
	virtual void	SaveFileName(const std::string &strFileName);
	//virtual int		SaveLocal(BYTE *pMessage, int iLen);
	virtual void	ParseMessage();
	virtual int		SaveToDB();
	virtual void	SendResponse(const std::string &strPreFlag);
	virtual int		UpdateDB();
	virtual std::string GetFileName();

private:
	int SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray);

private:
	structMESSAGE_HEAD				m_messageHead;
	structBILL_INFO_CLM10011		m_billInfo;
	structRESPOND_INFO				m_respondInfo;

	char		m_chOpFlag;
	char        m_chSEND_FLAG;
	std::string m_strNote;
	//std::string m_strFileName;
	//std::string m_strMQIp;
	//std::string m_strQueueName;
	//structConfigInfo m_config;
	std::vector<structCONTAINER_INFO_CLM10011> m_contaInfoVect;
};
