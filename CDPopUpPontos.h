#if !defined(AFX_CDPOPUPPONTOS_H__B3CAECA1_202C_11D8_BE61_A82CAE0E0C19__INCLUDED_)
#define AFX_CDPOPUPPONTOS_H__B3CAECA1_202C_11D8_BE61_A82CAE0E0C19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDPopUpPontos.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDPopUpPontos dialog

class CDPopUpPontos : public CDialog
{
  // Construction
  bool MostrandoProgr;

public:
  CDPopUpPontos(CWnd* pParent,COLORREF CorFundo = 0);   // standard constructor
  BOOL Create(CWnd* pWnd = NULL);                     //--- modeless.
  void MostraDescricao(int x,int y,std::string& Descricao,bool ComProgresso=false);
  bool MostrandoProgresso() { return MostrandoProgr; };
  CProgressCtrl* Progresso() { return &ProgAuxPontos;}

  CWnd* JanPai;

  // Dialog Data
  //{{AFX_DATA(CDPopUpPontos)
  enum { IDD = IDD_AUXDESPONTOS };
  CMelEditBox	m_EDNumDesPontos;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDPopUpPontos)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDPopUpPontos)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  //  afx_msg void OnEnSetfocusDescricao();
protected:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
  virtual BOOL OnInitDialog();
  CProgressCtrl ProgAuxPontos;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDPOPUPPONTOS_H__B3CAECA1_202C_11D8_BE61_A82CAE0E0C19__INCLUDED_)
