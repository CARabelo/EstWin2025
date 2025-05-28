
#if !defined(AFX_DSELGEOL_H__30C55F01_775F_11D4_BDFB_00104BC6689A__INCLUDED_)
#define AFX_DSELGEOL_H__30C55F01_775F_11D4_BDFB_00104BC6689A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSelGreide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDConfGeologico dialog

class CDConfGeologico : public CDialog
{
  CToolBar* mpCDesGeologico;        //--- Ponteiros para o toolbar do greide. 
                                    //--- usado quando o usuário seleciona se quer 
                                    //--- visualizar o toolbar ou não
  COLORREF* pCores;                 //--- Cores do dialogo
  int* pDados;                      //--- Estados das configuraçoes

  // Construction
public:
  CDConfGeologico(CWnd* pParent = NULL,CRect& Rect = CRect(10,10,10,10),COLORREF* PpCores=NULL,int* PpDados = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDConfGeologico)
  enum { IDD = IDD_CONFPERFGEOLOGICO };
  BOOL	m_CBCoordenadas;
  BOOL	m_CBDesenharMalha;
  BOOL	m_CBCotar;
  BOOL  m_CBCircularPontos;
  UINT	m_iEquidistancia;
  UINT	m_iEquidistanciaY;
  UINT	m_iEquidistCotas;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDConfGeologico)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDConfGeologico)
  afx_msg void OnVerCoordenadas();
  virtual BOOL OnInitDialog();
  afx_msg void OnMbok();
  afx_msg void OnCores();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  CString m_EquidistanciaY;
};
#endif //AFX_DSELGREIDE_H__30C55F01_775F_11D4_BDFB_00104BC6689A__INCLUDED_
