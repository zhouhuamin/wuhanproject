#pragma once
#include "clmbasic.h"
#include "StructDef.h"
#include <string>
#include <vector>

class CREL10010 :
	public CCLMBasic
{
public:
	CREL10010(void);
	virtual ~CREL10010(void);

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
	int SelectResponseInfo(CString &strMESSAGE_ID, CString &strOP_FLAG, CString &strNOTE);
	int InsertResponseInfo();
	int DeleteResponseInfo();
	int SelectMQForWeb(CStringArray &queueNameArray, CStringArray &mqIPArray);	

private:
	structMESSAGE_HEAD				m_messageHead;
	structBILL_INFO_CLM10011		m_billInfo;
	structRESPOND_INFO				m_respondInfo;

	char		m_chOpFlag;
	char        m_chSEND_FLAG;

	std::string ENTRY_ID;
	std::string TRAF_NAME;
	std::string VOYAGE_NO;
	std::string BILL_NO;
	std::string AGENT_NAME;
	std::string PACK_NO;
	std::string GROSS_WT;
};
