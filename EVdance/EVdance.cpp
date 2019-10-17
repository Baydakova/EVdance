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

// EVdance.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "EVdance.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "EVdanceDoc.h"
#include "EVdanceView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEVdanceApp

BEGIN_MESSAGE_MAP(CEVdanceApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CEVdanceApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
END_MESSAGE_MAP()


// CEVdanceApp construction

CEVdanceApp::CEVdanceApp()
{
	m_bHiColorIcons = TRUE;

	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("EVdance.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CEVdanceApp object

CEVdanceApp theApp;


// CEVdanceApp initialization

BOOL CEVdanceApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction();

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("NewPokerSoft"));
	GetRegEditValue();
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_EVdanceTYPE,
		RUNTIME_CLASS(CEVdanceDoc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CEVdanceView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

int CEVdanceApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CEVdanceApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CEVdanceApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CEVdanceApp customization load/save methods

void CEVdanceApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CEVdanceApp::LoadCustomState()
{
}

void CEVdanceApp::SaveCustomState()
{
}

void CEVdanceApp::GetRegEditValue()
{
	m_LinkColor = theApp.GetProfileInt(_T("Link"), _T("Color"), RGB(0, 0, 0));
	m_LinkWidth = theApp.GetProfileInt(_T("Link"), _T("Width"), 2);


	m_EntityPreflopColor = theApp.GetProfileInt(_T("EntityPreflop"), _T("Color"), RGB(0, 255, 0));
	m_EntityFlopColor = theApp.GetProfileInt(_T("EntityFlop"), _T("Color"), RGB(0, 255, 0));
	m_EntityTurnColor = theApp.GetProfileInt(_T("EntityTurn"), _T("Color"), RGB(0, 255, 0));
	m_EntityRiverColor = theApp.GetProfileInt(_T("EntityRiver"), _T("Color"), RGB(0, 255, 0));

	m_EntityCheckColor = theApp.GetProfileInt(_T("EntityCheck"), _T("Color"), RGB(0, 0, 255));
	m_EntityBetColor = theApp.GetProfileInt(_T("EntityBet"), _T("Color"), RGB(0, 0, 255));
	m_EntityCallColor = theApp.GetProfileInt(_T("EntityCall"), _T("Color"), RGB(0, 0, 255));
	m_EntityRaiseColor = theApp.GetProfileInt(_T("EntityRaise"), _T("Color"), RGB(0, 0, 255));
	m_EntityFoldColor = theApp.GetProfileInt(_T("EntityFold"), _T("Color"), RGB(0, 0, 255));

}
void CEVdanceApp::GetOpenDocumentList(CPtrList& pList)
{
	CDocument* pDoc;
	CDocTemplate* pTemplate;
	POSITION pos, posDoc;

	pList.RemoveAll();
	pos = GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		pTemplate = GetNextDocTemplate(pos);
		ASSERT(pTemplate->IsKindOf(RUNTIME_CLASS(CDocTemplate)));
		posDoc = pTemplate->GetFirstDocPosition();
		while (posDoc != NULL)
		{
			pDoc = pTemplate->GetNextDoc(posDoc);
			if (pDoc != NULL)
			{
				if (pDoc->IsKindOf(RUNTIME_CLASS(CEVdanceDoc)))
					pList.AddTail(pDoc);
			}
		}
	}
	return;
}
// CEVdanceApp message handlers



