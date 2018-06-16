#pragma once
#include "clmbasic.h"
#include "StructDef.h"
#include <string>
#include <vector>

class CCLM40031 :
	public CCLMBasic
{
public:
	CCLM40031(void);
	virtual ~CCLM40031(void);

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
	structBILL_INFO_CLM40031		m_billInfo;
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
