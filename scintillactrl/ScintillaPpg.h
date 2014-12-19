#if !defined(AFX_SCINTILLAPPG_H__4A2DFF70_C146_11D3_9CDF_482471000000__INCLUDED_)
#define AFX_SCINTILLAPPG_H__4A2DFF70_C146_11D3_9CDF_482471000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ScintillaPpg.h : Declaration of the CScintillaPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CScintillaPropPage : See ScintillaPpg.cpp.cpp for implementation.

class CScintillaPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CScintillaPropPage)
	DECLARE_OLECREATE_EX(CScintillaPropPage)

// Constructor
public:
	CScintillaPropPage();

// Dialog Data
	//{{AFX_DATA(CScintillaPropPage)
	enum { IDD = IDD_PROPPAGE_SCINTILLA };
	//}}AFX_DATA

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CScintillaPropPage)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCINTILLAPPG_H__4A2DFF70_C146_11D3_9CDF_482471000000__INCLUDED)
