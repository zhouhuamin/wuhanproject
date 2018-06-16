#pragma once
#include "StructDef.h"
#include "clmbasic.h"
#include <string>

class CCLMNullOrErrorObject :
	public CCLMBasic
{
public:
	CCLMNullOrErrorObject(void);
	virtual ~CCLMNullOrErrorObject(void);

public:
	virtual void	InitConfigInfo(structConfigInfo *pConfig);
	virtual void	SaveFileName(const std::string &strFileName);
	//virtual int		SaveLocal(BYTE *pMessage, int iLen);
	virtual void	ParseMessage();
	virtual int		SaveToDB();
	virtual void	SendResponse();
	virtual int		UpdateDB();
	virtual std::string GetFileName();
//private:
//	BOOL MoveFile(char *pTo,char *pFrom);
//	BOOL CreateIntermediateDirectory (const char *strDirectory);

//private:
//	std::string m_strFileName;
//	structConfigInfo m_config;
};
