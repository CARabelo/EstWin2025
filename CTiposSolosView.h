// OAEView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTiposSolosView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTiposSolosView : public CTypedPtrListView
{
  void PegaTracoCor(int Acao=0);
  CBitmap BitmapCor,BitmapTraco;

  int Traco;
  COLORREF CorTraco;

protected:

  CTiposSolosView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CTiposSolosView)

  // Form Data
public:

  enum { IDD = IDD_TIPOSSOLOS};
public:
  CIconComboBox m_ComboImageTracos;
  CImageListBox m_ListImageTracos;
  CImageListBox m_ListCoresTracos;
  CMFCColorButton m_ComboCorTraco;
  CEditEx EdUltimoCampo,EdPrimeiroCampo;
  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides

public:
  virtual void OnInitialUpdate();
  virtual void Serialize(CArchive& ar);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  // Implementation
protected:

  virtual ~CTiposSolosView();
  CImageList ImageList,ImageListLista;
  CImageList ImageListCor;

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  int Consiste(tlstwstr& ListaCampos);

  // Generated message map functions

  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  afx_msg void OnSelChangeListLinhas();
  afx_msg void OnSelChangeListCores();
  DECLARE_MESSAGE_MAP()
public:
};

/////////////////////////////////////////////////////////////////////////////
