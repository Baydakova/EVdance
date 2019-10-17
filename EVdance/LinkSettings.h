#pragma once


// CLinkSettings dialog

class CLinkSettings : public CDialog
{
	DECLARE_DYNAMIC(CLinkSettings)

public:
	CLinkSettings(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLinkSettings();
	COLORREF m_DlgLinkColor;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINK_SETTINGS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString str_width;
	afx_msg void OnButtonColor();
	afx_msg void OnBnClickedOk();
	int m_DlgLinkWidth;
};
