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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "EVdance.h"

#include "MainFrm.h"
#include "LinkSettings.h"
#include "UMLEntity.h"
#include "UMLLineSegment.h"
#include "EVdanceDoc.h"
#include "EVdanceView.h"

#include "UMLEditor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWndEx)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_STREET_MANAGER, OnStreetManager)
	ON_COMMAND(ID_ACTION_MANAGER, OnActionManager)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_SETTINGS_LINKS, OnSettingsLinks)
	ON_COMMAND_RANGE(ID_SETTINGS_STREETS_PREFLOP, ID_SETTINGS_STREETS_RIVER, OnSettingsStreets)
	ON_COMMAND_RANGE(ID_SETTINGS_ACTIONS_CHECK, ID_SETTINGS_ACTIONS_FOLD, OnSettingsActions)
	ON_COMMAND(ID_VIEW_OUTPUTWND, &CMainFrame::OnViewOutputwnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUTWND, &CMainFrame::OnUpdateViewOutputwnd)
	ON_COMMAND(ID_VIEW_CLASSWND, &CMainFrame::OnViewClasswnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_CLASSWND, &CMainFrame::OnUpdateViewClasswnd)
	ON_COMMAND(ID_VIEW_FILEWND, &CMainFrame::OnViewFilewnd)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILEWND, &CMainFrame::OnUpdateViewFilewnd)
	//menu->Actions
	ON_COMMAND(ID_ACTIONS_CHECK, OnButtonActionCheck)
	ON_COMMAND(ID_ACTIONS_BET, OnButtonActionBet)
	ON_COMMAND(ID_ACTIONS_CALL, OnButtonActionCall)
	ON_COMMAND(ID_ACTIONS_RAISE, OnButtonActionRaise)
	ON_COMMAND(ID_ACTIONS_FOLD, OnButtonActionFold)
	//menu->Streets
	ON_COMMAND(ID_STREETS_PREFLOP, OnButtonStreetPreflop)
	ON_COMMAND(ID_STREETS_FLOP, OnButtonStreetFlop)
	ON_COMMAND(ID_STREETS_TURN, OnButtonStreetTurn)
	ON_COMMAND(ID_STREETS_RIVER, OnButtonStreetRiver)
	ON_WM_SETTINGCHANGE()
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLUE);
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	m_wndRibbonBar.Create(this);
	m_wndRibbonBar.LoadFromResource(IDR_RIBBON);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	CString strTitlePane1;
	CString strTitlePane2;
	bNameValid = strTitlePane1.LoadString(IDS_STATUS_PANE1);
	ASSERT(bNameValid);
	bNameValid = strTitlePane2.LoadString(IDS_STATUS_PANE2);
	ASSERT(bNameValid);
	m_wndStatusBar.AddElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE1, strTitlePane1, TRUE), strTitlePane1);
	m_wndStatusBar.AddExtendedElement(new CMFCRibbonStatusBarPane(ID_STATUSBAR_PANE2, strTitlePane2, TRUE), strTitlePane2);

	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);

	// Load menu item image (not placed on any standard toolbars):
	CMFCToolBar::AddToolBarForImageCollection(IDR_MENU_IMAGES, theApp.m_bHiColorIcons ? IDB_MENU_IMAGES_24 : 0);

	// create docking windows
	if (!CreateDockingWindows())
	{
		TRACE0("Failed to create docking windows\n");
		return -1;
	}

	m_wndFileView.EnableDocking(CBRS_ALIGN_ANY);
	m_wndClassView.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndFileView);
	CDockablePane* pTabbedBar = NULL;
	m_wndClassView.AttachToTabWnd(&m_wndFileView, DM_SHOW, TRUE, &pTabbedBar);
	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndOutput);

	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

BOOL CMainFrame::CreateDockingWindows()
{
	BOOL bNameValid;

	// Create class view
	CString strClassView;
	bNameValid = strClassView.LoadString(IDS_CLASS_VIEW);
	ASSERT(bNameValid);
	if (!m_wndClassView.Create(strClassView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_CLASSVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Class View window\n");
		return FALSE; // failed to create
	}

	// Create file view
	CString strFileView;
	bNameValid = strFileView.LoadString(IDS_FILE_VIEW);
	ASSERT(bNameValid);
	if (!m_wndFileView.Create(strFileView, this, CRect(0, 0, 200, 200), TRUE, ID_VIEW_FILEVIEW, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT| CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create File View window\n");
		return FALSE; // failed to create
	}

	// Create output window
	CString strOutputWnd;
	bNameValid = strOutputWnd.LoadString(IDS_OUTPUT_WND);
	ASSERT(bNameValid);
	if (!m_wndOutput.Create(strOutputWnd, this, CRect(0, 0, 100, 100), TRUE, ID_VIEW_OUTPUTWND, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create Output window\n");
		return FALSE; // failed to create
	}

	SetDockingWindowIcons(theApp.m_bHiColorIcons);
	return TRUE;
}

void CMainFrame::SetDockingWindowIcons(BOOL bHiColorIcons)
{
	HICON hFileViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_FILE_VIEW_HC : IDI_FILE_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndFileView.SetIcon(hFileViewIcon, FALSE);

	HICON hClassViewIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_CLASS_VIEW_HC : IDI_CLASS_VIEW), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndClassView.SetIcon(hClassViewIcon, FALSE);

	HICON hOutputBarIcon = (HICON) ::LoadImage(::AfxGetResourceHandle(), MAKEINTRESOURCE(bHiColorIcons ? IDI_OUTPUT_WND_HC : IDI_OUTPUT_WND), IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), 0);
	m_wndOutput.SetIcon(hOutputBarIcon, FALSE);

	UpdateMDITabbedBarsIcons();
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnStreetManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnActionManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(TRUE);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
		m_wndRibbonBar.SetWindows7Look(FALSE);
	}

	m_wndOutput.UpdateFonts();
	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}


void CMainFrame::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CMDIFrameWndEx::OnSettingChange(uFlags, lpszSection);
	m_wndOutput.UpdateFonts();
}

void CMainFrame::OnSettingsLinks()
{
	CLinkSettings	dlg;
	dlg.m_DlgLinkColor = theApp.m_LinkColor;
	dlg.m_DlgLinkWidth = theApp.m_LinkWidth;
	if (dlg.DoModal() != IDOK) return;
	int count;
	CUMLEntityContainer* objs;
	CDiagramEntity* obj;
	CEVdanceDoc* pDoc;
	CPtrList pDocList;
	CEVdanceView* pView;
	CUMLLineSegment* pLine;
	CString strObj;
	theApp.GetOpenDocumentList(pDocList);
	POSITION pos, posdoc = pDocList.GetHeadPosition();

	while (posdoc != NULL)
	{
		pDoc = (CEVdanceDoc*)pDocList.GetNext(posdoc);
		pDoc->UpdateAllViews(NULL);
		pos = pDoc->GetFirstViewPosition();
		pView = (CEVdanceView*)(pDoc->GetNextView(pos));
		count = pView->m_editor.GetObjectCount();
		if (count > 0)
		{
			objs = pDoc->GetData();
			count = 0;
			while ((obj = objs->GetAt(count++)))
			{
				strObj = obj->GetString();
				pLine = dynamic_cast<CUMLLineSegment*>(obj);
				if (pLine)
				{
					pLine->SetColor(dlg.m_DlgLinkColor);
					pLine->SetWidth(dlg.m_DlgLinkWidth);
					//pDoc->SetModifiedFlag();
				}
			}
		}
		theApp.m_LinkColor = dlg.m_DlgLinkColor;
		theApp.m_LinkWidth = dlg.m_DlgLinkWidth;
	}
	
}

void CMainFrame::OnSettingsStreets(UINT nID)
{
	COLORREF color;
	if (nID == ID_SETTINGS_STREETS_PREFLOP) color = theApp.m_EntityPreflopColor;
	if (nID == ID_SETTINGS_STREETS_FLOP) color = theApp.m_EntityFlopColor;
	if (nID == ID_SETTINGS_STREETS_TURN) color = theApp.m_EntityTurnColor;
	if (nID == ID_SETTINGS_STREETS_RIVER) color = theApp.m_EntityRiverColor;

	CColorDialog	dlg(color);
	if (dlg.DoModal() == IDOK) color = dlg.GetColor();
	else return;

	int count, nFound;
	CUMLEntityContainer* objs;
	CDiagramEntity* obj;
	CEVdanceDoc* pDoc;
	CPtrList pDocList;
	CEVdanceView* pView;
	CUMLEntity* pEntity;
	CString strObj;
	theApp.GetOpenDocumentList(pDocList);
	POSITION pos, posdoc = pDocList.GetHeadPosition();

	while (posdoc != NULL)
	{
		pDoc = (CEVdanceDoc*)pDocList.GetNext(posdoc);
		pDoc->UpdateAllViews(NULL);
		pos = pDoc->GetFirstViewPosition();
		pView = (CEVdanceView*)(pDoc->GetNextView(pos));
		count = pView->m_editor.GetObjectCount();
		if (count > 0)
		{
			objs = pDoc->GetData();
			count = 0;
			while ((obj = objs->GetAt(count++)))
			{
				strObj = obj->GetString();
				pEntity = dynamic_cast<CUMLEntity*>(obj);
				if (pEntity)
				{
					if (nID == ID_SETTINGS_STREETS_PREFLOP)
					{
						nFound = strObj.Find(_T(",PREFLOP,"));
						if (nFound>0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityPreflopColor = color;
						}
					}
					else if (nID == ID_SETTINGS_STREETS_FLOP)
					{
						nFound = strObj.Find(_T(",FLOP,"));
						if (nFound>0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityFlopColor = color;
						}
					}
					else if (nID == ID_SETTINGS_STREETS_TURN)
					{
						nFound = strObj.Find(_T(",TURN,"));
						if (nFound > 0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityTurnColor = color;
						}
					}
					else if (nID == ID_SETTINGS_STREETS_RIVER)
					{
						nFound = strObj.Find(_T(",RIVER,"));
						if (nFound > 0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityRiverColor = color;
						}
					}
				}
			}
		}
	}
}

void CMainFrame::OnSettingsActions(UINT nID)
{
	COLORREF color;
	if (nID == ID_SETTINGS_ACTIONS_CHECK) color = theApp.m_EntityCheckColor;
	if (nID == ID_SETTINGS_ACTIONS_BET) color = theApp.m_EntityBetColor;
	if (nID == ID_SETTINGS_ACTIONS_CALL) color = theApp.m_EntityCallColor;
	if (nID == ID_SETTINGS_ACTIONS_RAISE) color = theApp.m_EntityRaiseColor;
	if (nID == ID_SETTINGS_ACTIONS_FOLD) color = theApp.m_EntityFoldColor;

	CColorDialog	dlg(color);
	if (dlg.DoModal() == IDOK) color = dlg.GetColor();
	else return;

	int count, nFound;
	CUMLEntityContainer* objs;
	CDiagramEntity* obj;
	CEVdanceDoc* pDoc;
	CPtrList pDocList;
	CEVdanceView* pView;
	CUMLEntity* pEntity;
	CString strObj;
	theApp.GetOpenDocumentList(pDocList);
	POSITION pos, posdoc = pDocList.GetHeadPosition();

	while (posdoc != NULL)
	{
		pDoc = (CEVdanceDoc*)pDocList.GetNext(posdoc);
		pDoc->UpdateAllViews(NULL);
		pos = pDoc->GetFirstViewPosition();
		pView = (CEVdanceView*)(pDoc->GetNextView(pos));
		count = pView->m_editor.GetObjectCount();
		if (count > 0)
		{
			objs = pDoc->GetData();
			count = 0;
			while ((obj = objs->GetAt(count++)))
			{
				strObj = obj->GetString();
				pEntity = dynamic_cast<CUMLEntity*>(obj);
				if (pEntity)
				{
					if (nID == ID_SETTINGS_ACTIONS_CHECK)
					{
						nFound = strObj.Find(_T(",CHECK,"));
						if (nFound > 0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityCheckColor = color;
						}
					}
					else if (nID == ID_SETTINGS_ACTIONS_BET)
					{
						nFound = strObj.Find(_T(",BET,"));
						if (nFound > 0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityBetColor = color;
						}
					}
					else if (nID == ID_SETTINGS_ACTIONS_CALL)
					{
						nFound = strObj.Find(_T(",CALL,"));
						if (nFound > 0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityCallColor = color;
						}
					}
					else if (nID == ID_SETTINGS_ACTIONS_RAISE)
					{
						nFound = strObj.Find(_T(",RAISE,"));
						if (nFound > 0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityRaiseColor = color;
						}
					}
					else if (nID == ID_SETTINGS_ACTIONS_FOLD)
					{
						nFound = strObj.Find(_T(",FOLD,"));
						if (nFound > 0)
						{
							pEntity->SetBkColor(color);
							theApp.m_EntityFoldColor = color;
						}
					}
				}
			}
		}
	}
	//pDoc->SetModifiedFlag();
}

void CMainFrame::OnViewOutputwnd()
{
	BOOL bShow = m_wndOutput.IsVisible() ? FALSE : TRUE;
	m_wndOutput.ShowPane(bShow, FALSE, FALSE);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewOutputwnd(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndOutput.IsVisible());
}

void CMainFrame::OnViewClasswnd()
{
	BOOL bShow = m_wndClassView.IsVisible() ? FALSE : TRUE;
	m_wndClassView.ShowPane(bShow, FALSE, FALSE);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewClasswnd(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndClassView.IsVisible());
}

void CMainFrame::OnViewFilewnd()
{
	BOOL bShow = m_wndFileView.IsVisible() ? FALSE : TRUE;
	m_wndFileView.ShowPane(bShow, FALSE, FALSE);
	RecalcLayout(FALSE);
}

void CMainFrame::OnUpdateViewFilewnd(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndFileView.IsVisible());
}

BOOL CMainFrame::DestroyWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	theApp.WriteProfileInt(_T("Link"), _T("Color"), theApp.m_LinkColor);
	theApp.WriteProfileInt(_T("Link"), _T("Width"), theApp.m_LinkWidth);

	theApp.WriteProfileInt(_T("EntityPreflop"), _T("Color"), theApp.m_EntityPreflopColor);
	theApp.WriteProfileInt(_T("EntityFlop"), _T("Color"), theApp.m_EntityFlopColor);
	theApp.WriteProfileInt(_T("EntityTurn"), _T("Color"), theApp.m_EntityTurnColor);
	theApp.WriteProfileInt(_T("EntityRiver"), _T("Color"), theApp.m_EntityRiverColor);
	theApp.WriteProfileInt(_T("EntityCheck"), _T("Color"), theApp.m_EntityCheckColor);
	theApp.WriteProfileInt(_T("EntityBet"), _T("Color"), theApp.m_EntityBetColor);
	theApp.WriteProfileInt(_T("EntityCall"), _T("Color"), theApp.m_EntityCallColor);
	theApp.WriteProfileInt(_T("EntityRaise"), _T("Color"), theApp.m_EntityRaiseColor);
	theApp.WriteProfileInt(_T("EntityFold"), _T("Color"), theApp.m_EntityFoldColor);

	return CMDIFrameWndEx::DestroyWindow();
}

void CMainFrame::OnButtonActionCheck()
/* ============================================================
obj->SetFont(_T("Century Gothic"));
Font    Vladimir Script || Century Gothic||   Copperplate Gothic Bold ||

//-name::type[multiplicity] = defaultvalue
============================================================*/
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	CUMLEditor      m_editor;
	/*m_editor.GetActiveWindow();
	m_editor.StartDrawingObject(obj);
	m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("CHECK"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9106685);
	CString strdsd;
}

void CMainFrame::OnButtonActionBet()
/* ============================================================*/
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	/*m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("BET"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9106685);

	CAttribute* at = NULL;
	at = new CAttribute;
	CString	str_name(_T("Size")), str_value(_T("22")), str_type(_T("BB")), str_multiplicity(_T("multiplicity"));
	at->name = str_name;
	at->type = str_type;
	at->defaultvalue = str_value;
	obj->AddAttribute(at);*/
}

void CMainFrame::OnButtonActionCall()
/* ============================================================*/
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	/*m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("CALL"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9106685);*/
}

void CMainFrame::OnButtonActionRaise()
/* ============================================================*/
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	/*m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("RAISE"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9106685);*/
}

void CMainFrame::OnButtonActionFold()
/* ============================================================*/
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	/*m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("FOLD"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9106685);*/
}

void CMainFrame::OnButtonStreetPreflop()
/* ============================================================*/
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	/*m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("PREFLOP"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9109436);
	CAttribute* at = NULL;
	at = new CAttribute;
	at->name = _T("Cards");
	at->type = _T("Text");
	obj->AddAttribute(at);

	CAttribute* at2 = NULL;
	at2 = new CAttribute;
	at2->name = _T("Pot");
	at2->type = _T("BBs");
	obj->AddAttribute(at2);*/

}

void CMainFrame::OnButtonStreetFlop()
/* ============================================================*/
{
	/*CUMLEntityClass* obj = new CUMLEntityClass;
	m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("FLOP"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9109436);
	CAttribute* at = NULL;
	at = new CAttribute;
	at->name = _T("Cards");
	at->type = _T("Text");
	obj->AddAttribute(at);

	CAttribute* at2 = NULL;
	at2 = new CAttribute;
	at2->name = _T("Pot");
	at2->type = _T("BBs");
	obj->AddAttribute(at2);*/

}

void CMainFrame::OnButtonStreetTurn()
/* ============================================================*/
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	/*m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("TURN"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9109436);
	CAttribute* at = NULL;
	at = new CAttribute;
	at->name = _T("Cards");
	at->type = _T("Text");
	obj->AddAttribute(at);

	CAttribute* at2 = NULL;
	at2 = new CAttribute;
	at2->name = _T("Pot");
	at2->type = _T("BBs");
	obj->AddAttribute(at2);*/

}

void CMainFrame::OnButtonStreetRiver()
/* ============================================================*/
{
	CUMLEntityClass* obj = new CUMLEntityClass;
	/*m_editor.StartDrawingObject(obj);
	obj->SetTitle(_T("RIVER"));
	obj->SetFont(_T("Century Gothic"));
	obj->SetBkColor(9109436);
	CAttribute* at = NULL;
	at = new CAttribute;
	at->name = _T("Cards");
	at->type = _T("Text");
	obj->AddAttribute(at);

	CAttribute* at2 = NULL;
	at2 = new CAttribute;
	at2->name = _T("Pot");
	at2->type = _T("BBs");
	obj->AddAttribute(at2);*/

}
