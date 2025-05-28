#if !defined(AFX_DCORESGREIDE_H__4124B721_A386_11D8_BE66_000AE6E79B25__INCLUDED_)
#define AFX_DCORESGREIDE_H__4124B721_A386_11D8_BE66_000AE6E79B25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DcoresGreide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDcoresGreide dialog

class CDcoresGreide : public CDialog
{
  int Tipo;

  COLORREF* pCoresRef;
  void MudaCor(CMelEditBox* EditBox);

  // Construction
public:
  CDcoresGreide(int PTipo = 0,CWnd* pParent = NULL,COLORREF* pCores = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDcoresGreide)
  enum { IDD = IDD_CORESDESGREIDE };
  CMelEditBox	m_EDCorTexto;
  CMelEditBox	m_EDCorTerreno;
  CMelEditBox	m_EDCorTerraplanagem;
  CMelEditBox	m_EDCorPavimento;
  CMelEditBox	m_EDCorMalha;
  CMelEditBox	m_EDCorFundo;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDcoresGreide)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDcoresGreide)
  afx_msg void OnRadiofundo();
  afx_msg void OnRadiomalha();
  afx_msg void OnRadiopavim();
  afx_msg void OnRadioterraplanagem();
  afx_msg void OnRadioterreno();
  afx_msg void OnRadiotexto();
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DCORESGREIDE_H__4124B721_A386_11D8_BE66_000AE6E79B25__INCLUDED_)
