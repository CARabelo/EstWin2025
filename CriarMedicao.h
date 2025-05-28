#if !defined(AFX_CRIARMEDICAO_H__697BF1A3_55BF_11D1_A9BF_006097BB88F1__INCLUDED_)
#define AFX_CRIARMEDICAO_H__697BF1A3_55BF_11D1_A9BF_006097BB88F1__INCLUDED_

#if _MSC_VER >= 1000
#endif // _MSC_VER >= 1000
// CriarMedicao.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCriarMedicao dialog

class CCriarMedicao : public CDialog
{
  // Construction
public:
  CCriarMedicao(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CCriarMedicao)
  enum { IDD = IDD_CRIARMEDICAO };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCriarMedicao)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CCriarMedicao)
  virtual BOOL OnInitDialog();
  afx_msg void OnDblclkListmedicao();
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CRIARMEDICAO_H__697BF1A3_55BF_11D1_A9BF_006097BB88F1__INCLUDED_)
