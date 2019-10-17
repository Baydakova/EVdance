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

// EVdanceView.cpp : implementation of the CEVdanceView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "EVdance.h"
#endif

#include "EVdanceDoc.h"
#include "EVdanceView.h"

#include "UMLEntityClass.h"
#include "UMLLineSegment.h"
#include "UMLEntityNote.h"
#include "UMLEntityPackage.h"
#include "UMLEntityLabel.h"
#include "UMLEntityInterface.h"
#include "UMLEntityClassTemplate.h"
#include "UMLEditorPropertyDialog.h"

#include "ExportDialog.h"
 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEVdanceView

IMPLEMENT_DYNCREATE(CEVdanceView, CView)

BEGIN_MESSAGE_MAP(CEVdanceView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CEVdanceView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_SIZE()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CEVdanceView construction/destruction

CEVdanceView::CEVdanceView()
{
	// TODO: add construction code here

}

CEVdanceView::~CEVdanceView()
{
}

BOOL CEVdanceView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CEVdanceView drawing

void CEVdanceView::OnDraw(CDC* pDC)
{
	CEVdanceDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	if (pDC->IsPrinting())
	{

		COLORREF col = m_editor.GetBackgroundColor();

		// Print zoom is the difference between screen- 
		// and printer resolution.
		double zoom = pDC->GetDeviceCaps(LOGPIXELSX) / m_screenResolutionX;

		CRect rect(0, 0,
			round(static_cast< double >(m_editor.GetVirtualSize().cx) * zoom),
			round(static_cast< double >(m_editor.GetVirtualSize().cy) * zoom));

		m_editor.SetRedraw(FALSE);
		m_editor.SetBackgroundColor(RGB(255, 255, 255));
		m_editor.Print(pDC, rect, zoom);
		m_editor.SetBackgroundColor(col);
		m_editor.SetRedraw(TRUE);

	}
}


// CEVdanceView printing


void CEVdanceView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CEVdanceView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CEVdanceView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CEVdanceView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CEVdanceView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CEVdanceView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CEVdanceView diagnostics

#ifdef _DEBUG
void CEVdanceView::AssertValid() const
{
	CView::AssertValid();
}

void CEVdanceView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CEVdanceDoc* CEVdanceView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CEVdanceDoc)));
	return (CEVdanceDoc*)m_pDocument;
}
#endif //_DEBUG


// CEVdanceView message handlers


void CEVdanceView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	if (!m_editor.m_hWnd)
	{
		// Creating the editor window
		CEVdanceDoc* pDoc = GetDocument();
		CRect rect;
		GetClientRect(rect);
		GetWindowRect(rect);
		rect.SetRect(0, 0, rect.right, rect.bottom);
		pDoc->GetData()->SetClipboardHandler(&theApp.m_clip);
		m_editor.Create(WS_CHILD | WS_VISIBLE, rect, this, pDoc->GetData());
		// We get the screen resolution, which we will use 
		// for scaling to printer. See also OnDraw.
		CClientDC dc(this);
		m_screenResolutionX = dc.GetDeviceCaps(LOGPIXELSX);

		// Setting up the virtual screen size
		// We want this to represent a single paper 
		// from the default printer.

		CPrintDialog	printer(TRUE, PD_RETURNDC);
		printer.GetDefaults();
		HDC	hdc = printer.GetPrinterDC();
		if (hdc)
		{

			double zoom = GetDeviceCaps(hdc, LOGPIXELSX) / m_screenResolutionX;
			int horzSize = ::GetDeviceCaps(hdc, PHYSICALWIDTH);
			int vertSize = ::GetDeviceCaps(hdc, PHYSICALHEIGHT);

			m_editor.SetVirtualSize(CSize(round(static_cast< double >(horzSize) / zoom), round(static_cast< double >(vertSize) / zoom)));

			int leftMarg = ::GetDeviceCaps(hdc, PHYSICALOFFSETX);
			int topMarg = ::GetDeviceCaps(hdc, PHYSICALOFFSETY);

			int horzPrintable = ::GetDeviceCaps(hdc, HORZRES);
			int vertPrintable = ::GetDeviceCaps(hdc, VERTRES);

			int rightMarg = horzSize - (horzPrintable + leftMarg);
			int bottomMarg = vertSize - (vertPrintable + topMarg);

			m_editor.SetMargins(round(static_cast< double >(leftMarg) / zoom), round(static_cast< double >(topMarg) / zoom), round(static_cast< double >(rightMarg) / zoom), round(static_cast< double >(bottomMarg) / zoom));

			::DeleteDC(hdc);

		}
		else
		{
			// No default printer installed
			m_editor.SetVirtualSize(CSize(8 * m_screenResolutionX, 11 * m_screenResolutionX));
		}

		m_editor.SetScrollWheelMode(WHEEL_ZOOM);
		//m_editor.SetModified(FALSE);

	}
	else
		m_editor.Clear();
}


void CEVdanceView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}
