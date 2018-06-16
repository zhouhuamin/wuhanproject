#pragma once

#include "StructDef.h"
#include "CLMBasic.h"
#include <string>
#include <vector>

// CLM10041-ºØ◊∞œ‰–∂¥¨÷∏¡Ó
class CCLM10041 : public CCLMBasic
{
public:
	CCLM10041(void);
	virtual ~CCLM10041(void);

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
	int SelectMQ(CStringArray &queueNameArray, CStringArray &mqIPArray);

private:
	structMESSAGE_HEAD	m_messageHead;
	structSHIP_INFO_CLM10041					m_shipInfo;
	std::vector<structBILL_INFO_CLM10041>		m_billInfo;
	structRESPOND_INFO  m_respondInfo;
	
	char		m_chOpFlag;
	char        m_chSEND_FLAG;
	std::string m_strNote;
	//std::string m_strFileName;
	//std::string m_strMQIp;
	//std::string m_strQueueName;
	//structConfigInfo m_config;
	//std::vector<structCONTAINER_INFO> m_contaInfoVect;
};
