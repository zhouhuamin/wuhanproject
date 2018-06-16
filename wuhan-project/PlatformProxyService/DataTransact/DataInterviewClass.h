// DataInterviewClass.h: interface for the DataInterviewClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAINTERVIEWCLASS_H__6DCC5DF0_A8B8_40EE_9A7B_D26E01A04F00__INCLUDED_)
#define AFX_DATAINTERVIEWCLASS_H__6DCC5DF0_A8B8_40EE_9A7B_D26E01A04F00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SPStruct.h"
//#include "Common.h"
//#include "interfacedefine.h"

class DataInterviewClass  
{
public:
	DataInterviewClass();
	virtual ~DataInterviewClass();
public:
	BOOL ExecSQL_NoRecord(LPCTSTR lpszSql);
	//���÷������ݿ���û������롢������
	void			SetConnectParam(LPCTSTR strUser, LPCTSTR strPwd,LPCTSTR strServer);	

	//����SQL����������
	_RecordsetPtr	exeSqlForAllData(LPCTSTR StrCMD);
	
	//����ɼ�����
	BOOL			InsertGateGatherData(_CHNL_GATHER_DATA &_struGatherData);


	BOOL			OpenDB();
	BOOL			CloseDB();
private:
	void			BeginTrans(); // 
	void			CommitTrans(); //  
	void			RollbackTrans(); //	���ݻع�
	
	void		    FuncInsertOneImage(unsigned char  * ImageData,int iImageSize,CString field);
	
	_RecordsetPtr	WorkFunc(LPCTSTR adCmd);

	BOOL PutCollectInfo(_RecordsetPtr Recordset, CString strField, char * pValue);
	BOOL PutCollectInfo(_RecordsetPtr Recordset, CString strField, int lValue);
	GUID GetGUID(void);

private:
	char			strConnection[1000];//�����ַ���
	_ConnectionPtr	m_pConnection;
   	_RecordsetPtr	m_pUserSet;
	_CommandPtr		m_commandptr;
	BOOL            m_ConnectFlag;
	static char			strDataBaseName[100];
	static int      m_iFlag;
	
};

#endif // !defined(AFX_DATAINTERVIEWCLASS_H__6DCC5DF0_A8B8_40EE_9A7B_D26E01A04F00__INCLUDED_)



















