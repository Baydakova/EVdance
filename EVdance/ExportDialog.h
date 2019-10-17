#if !defined(AFX_EXPORTDIALOG_H__CC002652_27D4_4683_BFB5_0E2B552C689D__INCLUDED_)
#define AFX_EXPORTDIALOG_H__CC002652_27D4_4683_BFB5_0E2B552C689D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CExportDialog dialog

class CExportDialog : public CDialog
{
// Construction
public:
	CExportDialog(CWnd* pParent = NULL);

// Dialog Data
	//{{AFX_DATA(CExportDialog)
	enum { IDD = IDD_DIALOG_EXPORT };
	CStatic	m_stripInfoCtrl;
	CButton	m_stripCtrl;
	CButton	m_onlyhCtrl;
	int		m_format;
	BOOL	m_strip;
	BOOL	m_onlyh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExportDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CExportDialog)
	afx_msg void OnRadioEmf();
	afx_msg void OnRadioCpp();
	afx_msg void OnRadioHtml();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTDIALOG_H__CC002652_27D4_4683_BFB5_0E2B552C689D__INCLUDED_)
