#if !defined(AFX_DESVIEW_H__77214A13_E40C_11D1_8327_004033523BB2__INCLUDED_)
#define AFX_DESVIEW_H__77214A13_E40C_11D1_8327_004033523BB2__INCLUDED_

#if _MSC_VER >= 1000
#pragma once		
#endif // _MSC_VER >= 1000
// DesView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DesView view

class DesView : public CView
{
  CLPerfis Terreno;	
  
  bool CirculaPonto,   //    Parametro : define se a cruz no ponto deve ser desenhada.
       DesenhaCruz;    //    Parametro : Define se o pondo deve ser circulado.
              	
protected:
	DesView(bool Desponto = true,boolCircPonto = true);         // protected constructor used by dynamic creation
	
	DECLARE_DYNCREATE(DesView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DesView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~DesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(DesView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESVIEW_H__77214A13_E40C_11D1_8327_004033523BB2__INCLUDED_)
