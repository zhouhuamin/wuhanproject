#pragma once
#include "StructDef.h"
#include "clmbasic.h"
#include <string>
#include <vector>
#include <boost/shared_array.hpp>

class CGUITransaction :
	public CCLMBasic
{
public:
	CGUITransaction(void);
	virtual ~CGUITransaction(void);

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


protected:
	int		GetXmlFromDB(const std::string &strParaGUID, std::string &strMessageType,std::string &strSender, boost::shared_array<char> &spXmlData, int &nXmlLen);
	int		SaveLocalFile(const std::string &strGUID, boost::shared_array<char> &spXmlData, int &nXmlLen);
	void	GetReceiverFromLocal(std::string &strMessageType, std::string &strRECEIVER);
	int     UpdateDB(const std::string &strGUID);

	int		GetGUIDFromDB(std::vector<std::string> &guidVect);

private:
	int SelectMQ(std::string &strMessageType, std::string &strSender, std::string &strRECEIVER);

//private:
//	std::string m_strGUID;
};
