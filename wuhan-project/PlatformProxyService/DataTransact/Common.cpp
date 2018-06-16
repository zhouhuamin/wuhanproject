// Common.cpp : implementation file
//
#include "stdafx.h"

#include "Common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCommon

CCommon::CCommon()
{
}

CCommon::~CCommon()
{
}

CString CCommon::DisplayAdoError(_ConnectionPtr m_pConnection)
{
	long errorcount=m_pConnection->GetErrors ()->GetCount ();
	_bstr_t add;
	CString ErrorMessage,temp;
	for (short i=0;i<errorcount;i++)
	{
		add=m_pConnection->GetErrors ()->GetItem (_variant_t((short)i))->GetDescription ();
		temp = (char *)add;
		ErrorMessage +=temp;
	}
	return ErrorMessage;
}

