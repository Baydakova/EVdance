// LinkSettings.cpp : implementation file
//

#include "stdafx.h"
#include "EVdance.h"
#include "LinkSettings.h"
#include "afxdialogex.h"


// CLinkSettings dialog

IMPLEMENT_DYNAMIC(CLinkSettings, CDialog)

CLinkSettings::CLinkSettings(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_LINK_SETTINGS, pParent)
	, m_DlgLinkWidth()
	, m_DlgLinkColor()
{
	//m_DlgLinkColor = theApp.m_LinkColor;
}

CLinkSettings::~CLinkSettings()
{
}

void CLinkSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_DlgLinkWidth);
}


BEGIN_MESSAGE_MAP(CLinkSettings, CDialog)
	ON_BN_CLICKED(IDC_LINK_BUTTON_COLOR, &CLinkSettings::OnButtonColor)
	ON_BN_CLICKED(IDOK, &CLinkSettings::OnBnClickedOk)
END_MESSAGE_MAP()


// CLinkSettings message handlers


void CLinkSettings::OnButtonColor()
{
	CColorDialog	dlg(m_DlgLinkColor);
	if (dlg.DoModal() == IDOK)
	{
		m_DlgLinkColor = dlg.GetColor();
	}
}


void CLinkSettings::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialog::OnOK();

}
