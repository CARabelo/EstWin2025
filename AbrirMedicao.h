#if !defined(AFX_ABRIRMEDICAO_H__87FFDD62_3339_11D1_A9BF_444553540000__INCLUDED_)
#define AFX_ABRIRMEDICAO_H__87FFDD62_3339_11D1_A9BF_444553540000__INCLUDED_

#if _MSC_VER >= 1000
//#pragma once
#endif // _MSC_VER >= 1000
// AbrirMedicao.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AbrirMedicao dialog

class AbrirMedicao : public CDialog
{
  // Construction
public:
  AbrirMedicao(CWnd* pParent = NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(AbrirMedicao)
  enum { IDD = IDD_ABRIRMEDICAO };
  CListBox	m_listaMedicao;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(AbrirMedicao)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(AbrirMedicao)
  afx_msg void OnChsoleitura();
  virtual void OnOK();
  virtual BOOL OnInitDialog();
  afx_msg void OnDblclkListmedicao();
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg int OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex);
  afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()

  CString NomeProjComp,Selecionada;

public:
  afx_msg void OnUpdateTerrenoGerarCn(CCmdUI *pCmdUI);
  afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABRIRMEDICAO_H__87FFDD62_3339_11D1_A9BF_444553540000__INCLUDED_)
