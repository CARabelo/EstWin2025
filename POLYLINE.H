// PolyLine.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PolyLine form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class PolyLine //: public CView
{
  CLPontos LPontos;
	CPen Pena;

  protected:

	  DECLARE_DYNCREATE(PolyLine)

  // Form Data
  public:
	  //{{AFX_DATA(PolyLine)
	  enum { IDD = IDD_ALASAR };
		// NOTE: the ClassWizard will add data members here
	  //}}AFX_DATA

  // Attributes
  public:

  // Operations
  public:

  // Overrides
	  // ClassWizard generated virtual function overrides
	  //{{AFX_VIRTUAL(PolyLine)
	protected:
	  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnDraw(CDC* pDC);
	//}}AFX_VIRTUAL

  // Implementation
  protected:
    #ifdef _DEBUG
	  virtual void AssertValid() const;
	  virtual void Dump(CDumpContext& dc) const;
    #endif

	  // Generated message map functions
	  //{{AFX_MSG(PolyLine)
	afx_msg void OnProcAnte();
	//}}AFX_MSG
//	  DECLARE_MESSAGE_MAP()

	public:
    PolyLine(int Estilo = PS_SOLID,int Expessura = 2,
		         COLORREF Cor = RGB(0,0,0));  // protected constructor used by dynamic creation
};

/////////////////////////////////////////////////////////////////////////////
