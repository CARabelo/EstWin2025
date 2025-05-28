//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000
// DCoordenadas.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDCoordenadas dialog

class CDCoordenadas : public CDialog
{
  std::string Projeto;
  // Construction
public:
  CDCoordenadas(CWnd* pParent,std::string pProjeto);   // standard constructor
  ~CDCoordenadas();

  bool bPorNome;

  // Dialog Data
  //{{AFX_DATA(CDCoordenadas)
  enum { IDD = IDD_ENTRACOORDXY };
  //}}AFX_DATA

  std::string Buffer,Bufferx,Buffery;

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDCoordenadas)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDCoordenadas)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  CString m_Nome;
  CButton m_cheCoordenadas;
  CButton m_cheNome;
  afx_msg void OnClickedChecoordenadas();
  afx_msg void OnClickedChenome();
  virtual BOOL OnInitDialog();
  CMEdit CEditCoordx,CEditCoordy;
  BOOL BooTrocarXY,BooCoordenadas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
