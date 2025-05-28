/////////////////////////////////////////////////////////////////////////////
// CEndaView message handlers 




// collevw.h : interface of the CCollectView class
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

class CEstView : public CView
{
protected: // create from serialization only
  CEstView();
  DECLARE_DYNCREATE(CEstView)
  void OnPrint(CDC* pDC,CPrintInfo* CP);


  // Attributes
public:
  CEstDoc* GetDocument();

protected:
  CStringList m_stringList;

  // Operations
public:

  // Overrides
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CCollectView)
public:
  virtual void OnDraw(CDC* pDC);  // overridden to draw this view
protected:
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CEstView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:

  // Generated message map functions
protected:
  //{{AFX_MSG(CCollectView)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnFilePrint();
};

#ifndef _DEBUG  // debug version in collevw.cpp
inline CEstDoc* CEstView::GetDocument()
{ return (CEstDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
