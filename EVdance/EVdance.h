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

// EVdance.h : main header file for the EVdance application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols

#include "UMLClipboardHandler.h"
// CEVdanceApp:
// See EVdance.cpp for the implementation of this class
//

class CEVdanceApp : public CWinAppEx
{
public:
	CEVdanceApp();

	CUMLClipboardHandler	m_clip;
	COLORREF m_LinkColor;
	COLORREF m_EntityPreflopColor, m_EntityFlopColor, m_EntityTurnColor, m_EntityRiverColor; //menu->Settings->Streets
	COLORREF m_EntityCheckColor, m_EntityBetColor, m_EntityCallColor, m_EntityRaiseColor, m_EntityFoldColor;    ////menu->Settings->Actions
	int m_LinkWidth;
																												// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	void GetRegEditValue();
	void GetOpenDocumentList(CPtrList& pList);
// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CEVdanceApp theApp;
