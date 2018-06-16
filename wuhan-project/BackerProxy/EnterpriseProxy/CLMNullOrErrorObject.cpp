#include "StdAfx.h"
#include "CLMNullOrErrorObject.h"
#include <string>

using namespace std;

CCLMNullOrErrorObject::CCLMNullOrErrorObject(void)
{
}

CCLMNullOrErrorObject::~CCLMNullOrErrorObject(void)
{
}

void CCLMNullOrErrorObject::InitConfigInfo(structConfigInfo *pConfig)
{
	m_config.strQueueIP		= pConfig->strQueueIP;
	m_config.strQueueName	= pConfig->strQueueName;
	m_config.strQueueLabel	= pConfig->strQueueLabel;

	m_config.strSqlServerDataUser	= pConfig->strSqlServerDataUser;
	m_config.strSqlServerDataPwd	= pConfig->strSqlServerDataPwd;
	m_config.strSqlServerDataName	= pConfig->strSqlServerDataName;
	m_config.strSqlServerDataServer	= pConfig->strSqlServerDataServer;
	m_config.strFilePath			= pConfig->strFilePath;
	m_config.strErrorFilePath		= pConfig->strErrorFilePath;
}

void CCLMNullOrErrorObject::SaveFileName(const std::string &strFileName)
{
	m_strFileName = strFileName;
	return;
}
//
//int CCLM10041::SaveLocal(BYTE *pMessage, int iLen)
//{
//	if (pMessage == NULL || iLen <= 1)
//	{
//		return 0;
//	}
//
//	CStdioFile  file;
//	file.Open(m_strFileName.c_str(),CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
//	file.Write(pMessage, iLen);
//	file.Close();
//	delete []pMessage;
//	pMessage = NULL;
//	//m_strFileName = "C:\\whcontainer.xml";
//
//	return 1;
//}

void CCLMNullOrErrorObject::ParseMessage()
{
	if (m_strFileName.empty())
		return;

	string strFileName = m_strFileName;
	// delete file
	if (strFileName != "")
	{
		//CFile::Remove(strFileName.c_str());
		CString strTmpName = strFileName.c_str();
		int index = strTmpName.ReverseFind('\\');
		if (index > 0)
			strTmpName = strTmpName.Mid(index + 1);

		SYSTEMTIME time;
		GetLocalTime(&time);
		CString strDateTime;
		strDateTime.Format(".%04d%02d%02d%02d%02d%02d%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);
		char tmpPath[MAX_PATH] = {0};
		strncpy_s(tmpPath, sizeof(tmpPath), m_config.strErrorFilePath.c_str(), MAX_PATH);
		strcat_s(tmpPath, sizeof(tmpPath), "\\");
		strcat_s(tmpPath, sizeof(tmpPath), (LPCTSTR)strTmpName);
		strcat_s(tmpPath, sizeof(tmpPath), (LPCTSTR)strDateTime);
		try
		{
			::DeleteFile(tmpPath);
			::MoveFile(m_strFileName.c_str(), tmpPath);
		}
		catch(...)
		{
			int ia = 3;
		}
	}
	return ;
}

int CCLMNullOrErrorObject::SaveToDB()
{
	return 1;
}

void CCLMNullOrErrorObject::SendResponse(const std::string &strPreFlag)
{
	return;
}

int CCLMNullOrErrorObject::UpdateDB()
{
	return 1;
}

std::string CCLMNullOrErrorObject::GetFileName()
{
	return m_strFileName;
}


////移动文件或文件夹
//BOOL CCLMNullOrErrorObject::MoveFileXX(char *pTo,char *pFrom)
//{
//	CreateIntermediateDirectory(pTo);
//	SHFILEOPSTRUCT FileOp={0}; 
//	FileOp.fFlags = FOF_NOCONFIRMATION|   //不出现确认对话框 
//		FOF_NOCONFIRMMKDIR ; //需要时直接创建一个文件夹,不需用户确定
//	FileOp.pFrom = pFrom; 
//	FileOp.pTo = pTo; 
//	FileOp.wFunc = FO_MOVE; 
//	return SHFileOperation(&FileOp) == 0;   
//}
//
//BOOL CCLMNullOrErrorObject::CreateIntermediateDirectory (const char *strDirectory)
//{
//	if (strDirectory==NULL || strDirectory[0]==0)
//	{
//		return FALSE;
//	}
//
//	BOOL bErrorOccur = FALSE;
//	CString csDirectory = strDirectory;
//	CString csIntermediateDirectory;
//
//	csDirectory.Replace ('/', '\\');
//	while (csDirectory.Replace ("\\\\", "\\") > 0);
//	csDirectory.TrimRight ('\\');
//
//	int iLastIndex = 0;
//	while (true)
//	{
//		iLastIndex = csDirectory.Find ('\\', iLastIndex);
//
//		if (iLastIndex == -1)
//		{
//			csIntermediateDirectory = csDirectory;
//		}
//		else
//		{
//			csIntermediateDirectory = csDirectory.Left (iLastIndex);
//			iLastIndex ++;
//		}
//
//		// 如果该文件夹不存在，则创建之
//		HANDLE hDirectory = 
//			CreateFile (
//			csIntermediateDirectory,
//			GENERIC_READ,
//			FILE_SHARE_READ,
//			NULL,
//			OPEN_EXISTING,
//			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS ,
//			NULL
//			);
//
//		if (hDirectory == INVALID_HANDLE_VALUE)
//		{
//			BOOL bCreated = CreateDirectory (csIntermediateDirectory, NULL);
//			if (!bCreated)
//			{
//				//logger.Log (LL_ERROR, "Create directory %s error! ErrorCode=%d",
//				//	csIntermediateDirectory,
//				//	GetLastError ()
//				//	);
//				bErrorOccur = TRUE;
//				break;
//			}
//		}
//		else {
//			CloseHandle (hDirectory);
//		}
//
//		if (iLastIndex == -1)
//		{
//			break;
//		}
//	}
//
//	return !bErrorOccur;
//}

