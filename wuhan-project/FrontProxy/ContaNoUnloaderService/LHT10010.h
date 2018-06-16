#pragma once
#include "clmbasic.h"
#include "StructDef.h"
#include <string>
#include <vector>

class CLHT10010 :
	public CCLMBasic
{
public:
	CLHT10010(void);
	virtual ~CLHT10010(void);

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
	int SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray);

private:
	structMESSAGE_HEAD				m_messageHead;
	structLHT10010					m_lht10010;
	//structBILL_INFO_CLM10011		m_billInfo;
	//structRESPOND_INFO				m_respondInfo;

	char		m_chOpFlag;
	char        m_chSEND_FLAG;
	std::string m_strNote;
	//std::string m_strFileName;
	//std::string m_strMQIp;
	//std::string m_strQueueName;
	//structConfigInfo m_config;
	//std::vector<structCONTAINER_INFO_CLM10011> m_contaInfoVect;
};
