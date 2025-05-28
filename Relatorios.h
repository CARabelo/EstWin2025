#if !defined(AFX_RELATORIOS_H__4B55CFD5_A4A7_483D_AF64_A520D1566D18__INCLUDED_)
#define AFX_RELATORIOS_H__4B55CFD5_A4A7_483D_AF64_A520D1566D18__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Relatorios.h : header file
//
typedef std::map<CEstaca,Ponto> MSecaoGeom;    //--- Secoes geometricas para acesso randomico
typedef MSecaoGeom::iterator itMSecaoGeom;

/////////////////////////////////////////////////////////////////////////////
// CRelatorios view

class CRelatorios : public CScrollView
{
  int Extremox,Extremoy;
  std::string NomeArquivo;
  int NLinhas;
  CSize TamLetra;
  CPoint LocalAtual;
  int IDRelatorio,CasasDecimais;

  void DesenhaRelatorio(CDC* pDC);
 // void DesenhaProjGeoDetalhado(CDC* pDC);
 // void DesenhaProjGreide(CDC* pDC);
 // void DesenharNotaServicoPavim(CDC* pDC);
 // void DesenharTerrenoPrimitivo(CDC* pDC);

protected:
  DECLARE_DYNCREATE(CRelatorios)

  // Attributes
public:

  CRelatorios();           // protected constructor used by dynamic creation

  enum{REL_NOTA_DE_SERVICO};

  int PagAtual;
  CPoint paper_res;

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CRelatorios)
public:
  virtual void OnInitialUpdate();
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CRelatorios();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CRelatorios)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  //  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RELATORIOS_H__4B55CFD5_A4A7_483D_AF64_A520D1566D18__INCLUDED_)
