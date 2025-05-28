#if !defined(AFX_DVOLUMES_H__8806C901_592F_11D7_BE61_849412050C1B__INCLUDED_)
#define AFX_DVOLUMES_H__8806C901_592F_11D7_BE61_849412050C1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DVolumes.h : header file
//

#define COM_DES_PONTOS 0
#define COM_DES_PERFIL 1
#define COM_DES_SECOES 3

/////////////////////////////////////////////////////////////////////////////
// CDVolumes dialog

class CDVolumes : public CDialog
{
  // Construction

  bool ReadWindowPlacement(LPWINDOWPLACEMENT pwp);
  void WriteWindowPlacement(LPWINDOWPLACEMENT pwp);
  int Ambiente;
  std::string NomeProjeto;

public:
  void Atualiza(const CSecao& Secao,bool SecAvulsa = false);
  void AtualizaVolumeTotal(double VolTotCorte,double VolTotAterro,double VolTotEmpola);

  CDVolumes(CWnd* pParent = NULL,int pAmbiente = COM_DES_SECOES,const std::string &NomeProj="");   // standard constructor

  ~CDVolumes();

  BOOL Create(CWnd* pWnd = NULL);   //--- modeless.

  // Dialog Data
  //{{AFX_DATA(CDVolumes)
  enum { IDD = IDD_VOLUMES };
  CString	m_StrVolTotCorte;
  CString	m_StrVolTotAterro;
  CString   m_StrVolTotEmpola;
  CString	m_StrAterro;
  CString	m_StrCorte;
  CString	m_strAcumAterro;
  CString	m_strAcumCorte;
  CString	m_strMassaAterro;
  CString	m_strMassaCorte;
  CString	m_strAterroEmpol;
  CString m_strMassaAterroEmpol;
  CString m_strAcumulAterroEmpol;
  CString m_strCompensacaoLateral;

  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDVolumes)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDVolumes)
  afx_msg void OnClose();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnEnChangeVsecaterro();
  afx_msg void OnEnChangeVoacumaterroemp();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DVOLUMES_H__8806C901_592F_11D7_BE61_849412050C1B__INCLUDED_)
