// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "FileView.h"
#include "ClassView.h"
#include "OutputWnd.h"

class CMainFrame : public CMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCRibbonBar     m_wndRibbonBar;
	CMFCRibbonApplicationButton m_MainButton;
	CMFCToolBarImages m_PanelImages;
	CMFCRibbonStatusBar  m_wndStatusBar;
	CFileView         m_wndFileView;
	CClassView        m_wndClassView;
	COutputWnd        m_wndOutput;
	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnStreetManager();
	afx_msg void OnActionManager();
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
public:
	afx_msg void OnViewOutputwnd();
	afx_msg void OnUpdateViewOutputwnd(CCmdUI *pCmdUI);
	afx_msg void OnViewClasswnd();
	afx_msg void OnUpdateViewClasswnd(CCmdUI *pCmdUI);
	afx_msg void OnViewFilewnd();
	afx_msg void OnUpdateViewFilewnd(CCmdUI *pCmdUI);
	virtual BOOL DestroyWindow();
	//menu->Settings
	afx_msg void OnSettingsLinks();
	afx_msg void OnSettingsStreets(UINT nID);
	afx_msg void OnSettingsActions(UINT nID);

	//menu->Actions
	afx_msg void OnButtonActionCheck();
	afx_msg void OnButtonActionBet();
	afx_msg void OnButtonActionCall();
	afx_msg void OnButtonActionRaise();
	afx_msg void OnButtonActionFold();
	//menu->Streets
	afx_msg void OnButtonStreetPreflop();
	afx_msg void OnButtonStreetFlop();
	afx_msg void OnButtonStreetTurn();
	afx_msg void OnButtonStreetRiver();
};


