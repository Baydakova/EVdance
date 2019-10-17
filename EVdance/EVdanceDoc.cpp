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

// EVdanceDoc.cpp : implementation of the CEVdanceDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "EVdance.h"
#endif

#include "EVdanceDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CEVdanceDoc

IMPLEMENT_DYNCREATE(CEVdanceDoc, CDocument)

BEGIN_MESSAGE_MAP(CEVdanceDoc, CDocument)
END_MESSAGE_MAP()


// CEVdanceDoc construction/destruction

CEVdanceDoc::CEVdanceDoc()
{
	// TODO: add one-time construction code here
	m_objs.SetClipboardHandler(&theApp.m_clip);
}

CEVdanceDoc::~CEVdanceDoc()
{
}

BOOL CEVdanceDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CEVdanceDoc serialization

void CEVdanceDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		m_objs.Save(ar);
	}
	else
	{
		// TODO: add loading code here
		m_objs.Load(ar);
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CEVdanceDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CEVdanceDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CEVdanceDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CEVdanceDoc diagnostics

#ifdef _DEBUG
void CEVdanceDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CEVdanceDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CEVdanceDoc commands

BOOL CEVdanceDoc::SaveModified()
/* ============================================================
Function :		CUMLEditorDemoDoc::SaveModified
Description :	Called before a check is made to see if
the data is modified.
Access :		Public

Return :		BOOL	-	From the framework
Parameters :	none

Usage :			We set the modified-flag from the data
container.

============================================================*/
{

	SetModifiedFlag(m_objs.IsModified());
	return CDocument::SaveModified();

}

CUMLEntityContainer* CEVdanceDoc::GetData()
/* ============================================================
Function :		CUMLEditorDemoDoc::GetData
Description :	Accessor for the internal data container.
Access :		Public

Return :		CUMLEntityContainer*	-	A pointer to the
data container.
Parameters :	none

Usage :			Call to get a pointer to the data.

============================================================*/
{

	return &m_objs;

}