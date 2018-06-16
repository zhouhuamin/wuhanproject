#pragma once
#include "clmbasic.h"
#include "StructDef.h"
#include <string>
#include <vector>

class CCLM90031 :
	public CCLMBasic
{
public:
	CCLM90031(void);
	virtual ~CCLM90031(void);

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
	int SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray);
	void UTF8ToGBK( char *&szOut );

private:
	structMESSAGE_HEAD				m_messageHead;
	structCOMPANY_INFO_CLM90031		m_clm90031;
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