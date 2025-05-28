// estView.cpp : implementation of the CEstView class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"
#include "afxtempl.h"
#include "est.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "estview.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEstView

IMPLEMENT_DYNCREATE(CEstView, CView)

BEGIN_MESSAGE_MAP(CEstView, CView)
  //{{AFX_MSG_MAP(CEstView)
  //}}AFX_MSG_MAP
  // Standard printing commands

  ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEstView construction/destruction

CEstView::CEstView()
{
}

CEstView::~CEstView()
{
}

/////////////////////////////////////////////////////////////////////////////
// CEstView drawing

void CEstView::OnDraw(CDC* pDC)
{
  CEstDoc* pDoc = GetDocument();
  ASSERT_VALID(pDoc);

  //	pDC;    // not used
}

/////////////////////////////////////////////////////////////////////////////
// CEstView printing

BOOL CEstView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  pInfo->m_bContinuePrinting = TRUE; 
  return DoPreparePrinting(pInfo);
}

void CEstView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CEstView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CEstView::OnPrint(CDC* pDC,CPrintInfo* CP)
{
}
/////////////////////////////////////////////////////////////////////////////
// CEstView diagnostics

#ifdef _DEBUG
void CEstView::AssertValid() const
{
  CView::AssertValid();
}

void CEstView::Dump(CDumpContext& dc) const
{
  CView::Dump(dc);
}

CEstDoc* CEstView::GetDocument() // non-debug version is inline
{
  return STATIC_DOWNCAST(CEstDoc, m_pDocument);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEstView message handlers

