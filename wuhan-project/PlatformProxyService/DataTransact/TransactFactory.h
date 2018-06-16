// TransactFactory.h: interface for the CTransactFactory class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TRANSACTFACTORY_H__389D004C_3928_4159_83C2_F77DF192C847__INCLUDED_)
#define AFX_TRANSACTFACTORY_H__389D004C_3928_4159_83C2_F77DF192C847__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicTransact.h"

class CTransactFactory : public CObject  
{
public:
	CTransactFactory();
	virtual ~CTransactFactory();

public:
	BOOL InitInfo();
	BOOL InfoTransact(CBasicConfigInfo &PackInfo);
 
private:
    CBasicTransact* GetInfoType(char szMsg);
   
private:
	CList<CBasicTransact*, CBasicTransact*> m_ListTable;
};

#endif // !defined(AFX_TRANSACTFACTORY_H__389D004C_3928_4159_83C2_F77DF192C847__INCLUDED_)
