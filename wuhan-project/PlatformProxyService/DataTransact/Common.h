#ifndef SP_COMMON_H
#define SP_COMMON_H

class CCommon
{
// Construction
public:
	CCommon();

// Attributes
public:

// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommon)
	//}}AFX_VIRTUAL

// Implementation
public:
	static CString DisplayAdoError(_ConnectionPtr m_pConnection);
	virtual ~CCommon();

};

#endif

/////////////////////////////////////////////////////////////////////////////
