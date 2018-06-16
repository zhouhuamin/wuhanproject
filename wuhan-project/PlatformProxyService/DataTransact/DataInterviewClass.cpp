// DataInterviewClass.cpp: implementation of the DataInterviewClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DataInterviewClass.h"
#include "Common.h"
#include "CxLog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

char	 DataInterviewClass::strDataBaseName[100] = {0};
int      DataInterviewClass::m_iFlag = 0;
DataInterviewClass::DataInterviewClass()
{

	m_pConnection=NULL;
	m_ConnectFlag=FALSE;
	memset(strConnection, 0x00, sizeof(strConnection));
//	::CoInitialize(NULL);
/*	try
	{
		hr = m_pConnection.CreateInstance(__uuidof(Connection));	
		m_pConnection->put_CommandTimeout( 10 );
		
	}
	catch(_com_error e)
	{
		CCommon common;
		CXMLLogfile::Instance()->WriteXmlLog("数据库","数据库操作失败:%s",common.DisplayAdoError(m_pConnection));
	
	//	CXMLLogfile::Instance()->Flush();
//		AfxMessageBox(common.DisplayAdoError(m_pConnection));
		return ;
	} 
*/	
}

DataInterviewClass::~DataInterviewClass()
{
/*	if(m_commandptr)
		m_commandptr.Release();
	if(m_pUserSet)
		m_pUserSet.Release();
	if(m_pConnection)
		m_pConnection.Release();
*/
	CloseDB();
//	CoUninitialize();	
}

//打开数据库
BOOL DataInterviewClass::OpenDB()
{
	HRESULT hr;
	
//	CString str = "Provider=SQLOLEDB.1;Data Source=sxy;Initial Catalog=GateHisDataDB;User ID=sa; PWD=sa";
	if(m_ConnectFlag==TRUE)
		return TRUE;

	try
	{
		if(m_pConnection==NULL)
		{
			
		}
		else
		{
			m_pConnection->Close();
			m_pConnection.Release();
			m_pConnection=NULL;
		}
		hr = m_pConnection.CreateInstance(__uuidof(Connection));	
		m_pConnection->put_CommandTimeout( 10 );
		if(FAILED(hr))
		{
			LOG_ERR("初始化失败");
			return FALSE;
		}

		hr=m_pConnection->Open((_bstr_t)strConnection,"","",-1);
		if(FAILED(hr))
		{
			LOG_ERR("数据库打开失败 fail");
			return FALSE;
		}
		
		m_commandptr.CreateInstance (__uuidof(Command));
		
//		m_pUserSet.CreateInstance("ADODB.Recordset");
		
//		m_pUserSet.CreateInstance (__uuidof(Recordset));
	
		m_commandptr->ActiveConnection =m_pConnection;
		m_ConnectFlag=TRUE;
	}
	catch(...)
	{
		CCommon common;

		LOG_ERR1("数据库打开失败:%s",common.DisplayAdoError(m_pConnection));
		
		
	//	CXMLLogfile::Instance()->Flush();

//		AfxMessageBox(common.DisplayAdoError(m_pConnection));
		return FALSE;
	}
	return TRUE;
}
//关闭数据库
BOOL DataInterviewClass::CloseDB()
{
//	if(m_commandptr)
//		m_commandptr.Release();
//	if(m_pUserSet)
//		m_pUserSet.Release();
//	if(m_pConnection)
//		m_pConnection.Release();

	try
	{
		if(!m_ConnectFlag)
			return FALSE;
		m_ConnectFlag=FALSE;
		if (m_pConnection != NULL)
		{
			if (m_pConnection->State)
			{
				m_ConnectFlag=FALSE;
				m_pConnection->Close ();
			
			//	m_pConnection=NULL;
			}
			m_pConnection.Release();
		}
		else
			return FALSE;
	}
	catch(...)
	{
		return FALSE;
	}
	
	return true;
}

void DataInterviewClass::BeginTrans()
{
	CString strSQLCmd;
	strSQLCmd="BEGIN TRAN";

	OpenDB();
	WorkFunc(strSQLCmd);
	CloseDB();
}
void DataInterviewClass::CommitTrans()
{
	
	CString strSQLCmd;
	strSQLCmd="COMMIT TRAN";
	OpenDB();
	WorkFunc(strSQLCmd);
}
void DataInterviewClass::RollbackTrans()
{
	CString strSQLCmd;
	strSQLCmd="ROLLBACK TRAN";
	
	OpenDB();
	WorkFunc(strSQLCmd);
	CloseDB();

}
//根据SQL语句自由查询
_RecordsetPtr DataInterviewClass::exeSqlForAllData(LPCTSTR StrCMD)
{
	_RecordsetPtr       _TemPtr = NULL;

	if(!OpenDB())
		return NULL;
	_TemPtr = WorkFunc(StrCMD);
	if(_TemPtr==NULL)
	{
		LOG_ERR1("在exeSqlForAllData执行SQL失败:%s",StrCMD);
	
	//	CXMLLogfile::Instance()->Flush();
	}
//	CloseDB();
	return _TemPtr;
}

//插入采集记录
BOOL DataInterviewClass::InsertGateGatherData(_CHNL_GATHER_DATA &_struGatherData)
{
	try
	{
		char sSql[4096] = {0};//lzq update 2048 -->3072 2007-07-22 主要因为IC卡的单证长度太长
		
		if(!OpenDB())
		{
			return FALSE;
		}

		char szCONNECT_ID[40 + 1]	= {0};
		char szI_E_TIME[21 + 1]		= {0};
		char FREE_CMD[10 + 1]		= {0};
		char FREE_ID[20 + 1]		= {0};
		CTime tTime            = CTime::GetCurrentTime();
		strcpy_s(szI_E_TIME, sizeof(szI_E_TIME), tTime.Format("%Y-%m-%d %H:%M:%S"));

		GUID guid = GetGUID();
		char buf[64] = {0};
		_snprintf_s(buf, sizeof(buf), sizeof(buf)
			, "%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X"
			, guid.Data1
			, guid.Data2
			, guid.Data3
			, guid.Data4[0], guid.Data4[1]
		, guid.Data4[2], guid.Data4[3], guid.Data4[4], guid.Data4[5]
		, guid.Data4[6], guid.Data4[7]
		);

		memcpy(szCONNECT_ID, buf, sizeof(szCONNECT_ID));

		////				'%s','%s','%s','%s','%s','%s','%s',GetDate())",
		sprintf_s(sSql, sizeof(sSql) - 1, "insert into R_GTW_GATHER_INFO(CONNECT_ID, AREA_ID,CHNL_NO,I_E_TYPE,SEQ_NO,\
							DR_IC_NO,IC_BILL_NO,IC_GROSS_WT,IC_VE_NAME,IC_CONTA_ID,\
							IC_ESEAL_ID,IC_FORM_TYPE,IC_REG_DATETIME,IC_PER_DAY_DUE,VE_CUSTOMS_NO,\
							CAR_EC_NO,VE_NAME,VE_WT,GROSS_WT,CONTA_ID_F,\
							CONTA_ID_B,ESEAL_ID,SEAL_KEY,ARRIVE_TIME,FREE_CMD,FREE_ID) \
		 values('%s','%s','%s','%c','%s',\
				'%s','%s',%.4f,'%s','%s',\
				'%s',%d,'%s',%d,'%s',\
				'%s','%s',%.4f,%.4f,'%s',\
				'%s','%s','%s',GetDate(),'%s','%s')",
		szCONNECT_ID,
		_struGatherData.szAREA_ID,
		_struGatherData.szCHNL_NO,
		_struGatherData.byI_E_TYPE,
		_struGatherData.szSEQ_NO,

		_struGatherData.szDR_IC_NO,
		_struGatherData.szIC_BILL_NO,
		_struGatherData.IC_GROSS_WT,
		_struGatherData.szIC_VE_NAME,
		_struGatherData.szIC_CONTA_ID,

		_struGatherData.szIC_ESEAL_ID,
		_struGatherData.byIC_FORM_TYPE,
		_struGatherData.szIC_REG_DATETIME,
		_struGatherData.nIC_PER_DAY_DUE,
		_struGatherData.szVE_CUSTOMS_NO,

		_struGatherData.szCAR_EC_NO,
		_struGatherData.szVE_NAME,
		_struGatherData.VE_WT,
		_struGatherData.GROSS_WT,
		_struGatherData.szCONTA_ID_F,

		_struGatherData.szCONTA_ID_B,
		_struGatherData.szESEAL_ID,
		_struGatherData.szESEAL_KEY,
		FREE_CMD,
		FREE_ID);

		if(!ExecSQL_NoRecord(sSql))
			return FALSE;
		
		CloseDB();
	}catch(_com_error &e)
	{
		CloseDB();
		_bstr_t bstrSource (e.Source());
		_bstr_t bstrDescription (e.Description());
		LOG_ERR1( "\tCode meaning = %s\n", e.ErrorMessage ());
		LOG_ERR1("InsertGateGatherData失败:%s",e.ErrorMessage ());
		return FALSE;
	}

	return TRUE;
}

//执行数据库操作
_RecordsetPtr DataInterviewClass::WorkFunc(LPCTSTR adCmd)
{
//	_variant_t fieldCount;
 //	VariantInit (&fieldCount);
	
	try
	{
	/*	m_commandptr->CommandType =adCmdText;
		m_commandptr ->CommandText =(_bstr_t)adCmd;
		m_commandptr->CommandTimeout=50;

		m_pUserSet->put_CursorType(adOpenStatic);

		m_pUserSet = m_commandptr->Execute (&fieldCount,NULL,adCmdUnknown);
		
		VariantClear (&fieldCount);*/
		try
		{
			if (m_pUserSet != NULL && m_pUserSet->State != adStateClosed)
			{
				SUCCEEDED(m_pUserSet->Close());
				//m_pUserSet.Release();	// 周华民 关闭记录集
			}
		}
		catch (const _com_error& e)
		{
			LOG_ERR1(_T(":( 关闭记录集发生异常: %s\n"), e.ErrorMessage());
			LOG_ERR1(" 关闭记录集发生异常: %s\n", e.ErrorMessage());
			return FALSE;
		}
		m_pUserSet.CreateInstance("ADODB.Recordset");
		m_pUserSet->Open(_bstr_t(adCmd), _variant_t((IDispatch*)m_pConnection, true),
							adOpenStatic, adLockOptimistic, adCmdText);
		if (m_pUserSet == NULL) 
		{
			return FALSE;
		}
		m_pUserSet->BOF;
	}
	catch(...)
	{
		CCommon common;
		LOG_ERR1("数据库执行失败:%s",common.DisplayAdoError(m_pConnection));
	
	//	CXMLLogfile::Instance()->Flush();
//		AfxMessageBox(common.DisplayAdoError(m_pConnection));
		return NULL;
	}
	return m_pUserSet;
}

//插入一张图片函数
void DataInterviewClass::FuncInsertOneImage(unsigned char  *ImageData,int iImageSize,CString field)
{
	BYTE					*pBuf =	ImageData;     
	VARIANT					varBLOB;
	SAFEARRAY				*psa;
	SAFEARRAYBOUND			rgsabound[1];
	
	if(pBuf)
	{    
		rgsabound[0].lLbound = 0;
		rgsabound[0].cElements = iImageSize;
		psa = SafeArrayCreate(VT_UI1, 1, rgsabound);
		for (long i = 0; i < (long)iImageSize; i++)
			SafeArrayPutElement (psa, &i, pBuf++);
		varBLOB.vt = VT_ARRAY | VT_UI1;
		varBLOB.parray = psa;
		m_pUserSet->GetFields()->GetItem(_variant_t (field.GetBuffer(0)))->AppendChunk(varBLOB);

		::SafeArrayDestroyData( psa);
		::SafeArrayDestroyDescriptor(psa);
	}
	pBuf=0;               
	
}

void DataInterviewClass::SetConnectParam(LPCTSTR strUser, LPCTSTR strPwd,LPCTSTR strServer)
{
	// 修改ADO连接字符串
	CString strConn = "";
//	strConnection = "Provider=SQLOLEDB.1;Data Source=" + strServer + 
//		";Initial Catalog=WHHGWL;User ID=" + strUser + "; PWD=" + strPwd;
		//sprintf(strConnection,"DSN=CGS;UID=%s;PWD=%s",strUser,strPwd);

	if (m_iFlag == 0)
	{
		char szAppName[260] = {0};
		size_t  len = 0;
		::GetModuleFileName(NULL, szAppName, sizeof(szAppName));
		len = strlen(szAppName);
		for(size_t i=len; i>0; i--)
		{
			if(szAppName[i] == '\\')
			{
				szAppName[i+1]='\0';
				break;
			}
		}
		
		strcat_s(szAppName, sizeof(szAppName), "SysConfig.Ini");
		::GetPrivateProfileString("DATABASE", "DataName", "WHHGWL", DataInterviewClass::strDataBaseName, 100, szAppName);

		m_iFlag = 1;
	}




	strConn = "Provider=SQLOLEDB.1;Password="; //sa;Persist Security Info=True;User ID=sa;Initial Catalog=WHHGWL;Data Source=SANBAO-ZHOUHM
	strConn += strPwd;
	strConn += ";Persist Security Info=True;User ID=";
	strConn += strUser;
	strConn += ";Initial Catalog="; // WHHGWL";
	strConn += DataInterviewClass::strDataBaseName;
	strConn += ";Data Source=";
	strConn += strServer;
	memcpy(strConnection, strConn.GetBuffer(0), strConn.GetLength());
	strConn.ReleaseBuffer();

}

BOOL DataInterviewClass::PutCollectInfo(_RecordsetPtr Recordset, CString strField, char * pValue)
{
	try
	{
		CString strTmp;
		_variant_t vt;
		
		strTmp = pValue;
		if(strTmp.CompareNoCase("") != 0)
		{
			vt = _variant_t(strTmp);
			Recordset->PutCollect((_variant_t)strField, vt);
			
			return TRUE;
		}
	}catch(...)
	{
		return FALSE;
	}

	return FALSE;
}

BOOL DataInterviewClass::PutCollectInfo(_RecordsetPtr Recordset, CString strField, int lValue)
{
	try
	{
		CString strTmp;
		_variant_t vt;
		
		strTmp.Format("%d", lValue);
		if(lValue >= 0)
		{
			vt = _variant_t(strTmp);
			Recordset->PutCollect((_variant_t)strField, vt);
			
			return TRUE;
		}
	}catch(...)
	{
		return FALSE;
	}

	return FALSE;
}

BOOL DataInterviewClass::ExecSQL_NoRecord(LPCTSTR lpszSql)
{
	_CommandPtr pCmd = NULL;

		try
		{
			if(!OpenDB())
			{
				LOG_ERR1("ExecSQL_NoRecord 数据库打开失败 :%s",lpszSql);
				return FALSE;
			}
			pCmd.CreateInstance(__uuidof(Command));
			pCmd->putref_ActiveConnection(m_pConnection);
			pCmd->put_CommandText(_bstr_t(lpszSql));
			SysFreeString(_bstr_t(lpszSql));
			pCmd->CommandTimeout = 30000; // 原来是50;
			pCmd->Execute( NULL, NULL, NULL);
			CloseDB();	// 周华民 关闭数据库
		}

		catch(_com_error &ce)
		{
	//		TRACE(lpszSql);
			CString message;
			message.Format("在ExecSQL_NoRecord执行SQL语句:%s 失败:%s",lpszSql,ce.ErrorMessage());
			LOG_ERR1("数据库:%s",message);
		
	
//			CXMLLogfile::Instance()->Flush();


	//		ConnectDB();
			CloseDB();	// 周华民 关闭数据库
			return FALSE;
		}


    return TRUE;
}

GUID DataInterviewClass::GetGUID(void)
{
	GUID guid;
	CoInitialize(NULL);
	::CoCreateGuid(&guid);
	CoUninitialize();
	return guid;
}

