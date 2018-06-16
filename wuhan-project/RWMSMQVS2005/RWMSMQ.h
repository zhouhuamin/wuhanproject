// RWMSMQ.h : main header file for the RWMSMQ application
//

#if !defined(AFX_RWMSMQ_H__713AE685_28CE_4322_B747_B7EAA73E8401__INCLUDED_)
#define AFX_RWMSMQ_H__713AE685_28CE_4322_B747_B7EAA73E8401__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CRWMSMQApp:
// See RWMSMQ.cpp for the implementation of this class
//

class CRWMSMQApp : public CWinApp
{
public:
	CRWMSMQApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRWMSMQApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CRWMSMQApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RWMSMQ_H__713AE685_28CE_4322_B747_B7EAA73E8401__INCLUDED_)
