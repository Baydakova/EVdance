/* ==========================================================================
	Class :			CExportDialog

	Author :		Johan Rosengren, Abstrakt Mekanik AB

	Date :			2004-06-20

	Purpose :		"CExportDialog", derived from "CDialog", wraps the 
					Export dialog.

	Description :	A standard Wizard-generated dialog class.

	Usage :			Instantiate and call "DoModal".

   ========================================================================*/

#include "stdafx.h"
#include "EVdance.h"
#include "ExportDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportDialog dialog


CExportDialog::CExportDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CExportDialog::IDD, pParent)
/* ============================================================
	Function :		CExportDialog::CExportDialog
	Description :	Constructor
	Access :		Public

	Return :		void
	Parameters :	CWnd* pParent	-	Dialog parent
					
	Usage :			Created from the application view

   ============================================================*/
{
	//{{AFX_DATA_INIT(CExportDialog)
	m_format = 0;
	m_strip = FALSE;
	//}}AFX_DATA_INIT
}


void CExportDialog::DoDataExchange(CDataExchange* pDX)
/* ============================================================
	Function :		CExportDialog::DoDataExchange
	Description :	Data-exchange handler.
	Access :		Protected

	Return :		void
	Parameters :	CDataExchange* pDX	-	Pointer to exchange object
					
	Usage :			Called from MFC to exchange and validate 
					dialog data.

   ============================================================*/
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportDialog)
	DDX_Control(pDX, IDC_STATIC_STRIP_INFO, m_stripInfoCtrl);
	DDX_Control(pDX, IDC_CHECK_STRIP_LEADING, m_stripCtrl);
	DDX_Control(pDX, IDC_CHECK_ONLY_H, m_onlyhCtrl);
	DDX_Radio(pDX, IDC_RADIO_EMF, m_format);
	DDX_Check(pDX, IDC_CHECK_STRIP_LEADING, m_strip);
	DDX_Check(pDX, IDC_CHECK_ONLY_H, m_onlyh );
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportDialog, CDialog)
	//{{AFX_MSG_MAP(CExportDialog)
	ON_BN_CLICKED(IDC_RADIO_EMF, OnRadioEmf)
	ON_BN_CLICKED(IDC_RADIO_CPP, OnRadioCpp)
	ON_BN_CLICKED(IDC_RADIO_HTML, OnRadioHtml)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDialog message handlers

void CExportDialog::OnRadioCpp() 
/* ============================================================
	Function :		CExportDialog::OnRadioCpp
	Description :	Handler for the Export to CPP button
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Enables CPP-specific 
					controls

   ============================================================*/
{

	m_stripCtrl.EnableWindow( TRUE );
	m_stripInfoCtrl.EnableWindow( TRUE );
	m_onlyhCtrl.EnableWindow( TRUE );
	
}

void CExportDialog::OnRadioHtml() 
/* ============================================================
	Function :		CExportDialog::OnRadioHtml
	Description :	Handler for the Export to HTML button
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Disables CPP-specific 
					controls

   ============================================================*/
{

	m_stripCtrl.EnableWindow( FALSE );
	m_stripInfoCtrl.EnableWindow( FALSE );
	m_onlyhCtrl.EnableWindow( FALSE );
	
}

void CExportDialog::OnRadioEmf() 
/* ============================================================
	Function :		CExportDialog::OnRadioEmf
	Description :	Handler for the Export to EMF button
	Access :		Protected

	Return :		void
	Parameters :	none

	Usage :			Called from MFC. Disables CPP-specific 
					controls

   ============================================================*/
{

	m_stripCtrl.EnableWindow( FALSE );
	m_stripInfoCtrl.EnableWindow( FALSE );
	m_onlyhCtrl.EnableWindow( FALSE );
	
}
