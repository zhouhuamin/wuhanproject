#pragma once

#include "clmbasic.h"
#include "StructDef.h"
#include "MTInfoStruct.h"
#include <string>
#include <vector>


class CMT2101 :
	public CCLMBasic
{
public:
	CMT2101(void);
	virtual ~CMT2101(void);

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
	int SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray);
	int SelectMQForQiYe(CStringArray &queueNameArray, CStringArray &mqIPArray);

private:
	//structMESSAGE_HEAD	m_messageHead;
	//structBILL_INFO		m_billInfo;
	//structRESPOND_INFO  m_respondInfo;

	char		m_chOpFlag;
	char        m_chSEND_FLAG;
	std::string m_strNote;
	//std::string m_strFileName;
	//std::string m_strMQIp;
	//std::string m_strQueueName;
	//structConfigInfo m_config;
	//std::vector<structCONTAINER_INFO> m_contaInfoVect;
	structMT2010 m_mt2010;
	structMT_RESPONSE	m_mtResponse;
	int m_nFlag;
};
