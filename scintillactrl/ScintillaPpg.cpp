// ScintillaPpg.cpp : Implementation of the CScintillaPropPage property page class.

#include "stdafx.h"
#include "ScintillaApp.h"
#include "ScintillaPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CScintillaPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CScintillaPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CScintillaPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CScintillaPropPage, "SCINTILLA.ScintillaPropPage.1",
	0x4a2dff63, 0xc146, 0x11d3, 0x9c, 0xdf, 0x48, 0x24, 0x71, 0, 0, 0)


/////////////////////////////////////////////////////////////////////////////
// CScintillaPropPage::CScintillaPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CScintillaPropPage

BOOL CScintillaPropPage::CScintillaPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SCINTILLA_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CScintillaPropPage::CScintillaPropPage - Constructor

CScintillaPropPage::CScintillaPropPage() :
	COlePropertyPage(IDD, IDS_SCINTILLA_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CScintillaPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CScintillaPropPage::DoDataExchange - Moves data between page and properties

void CScintillaPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CScintillaPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CScintillaPropPage message handlers
