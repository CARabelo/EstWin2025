// CEdiAciTopogView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEdiAciTopogView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "afxwin.h"

class CEdiAciTopogView : public CTypedPtrListView
{
  int Icone;
  bool Forcar;
  int nSel;

protected:
  CEdiAciTopogView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CEdiAciTopogView)

  int Consiste(tlstwstr& ListaCampos);

  // Form Data
public:
  enum { IDD = IDD_CADASTROACITOPOG};

   // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
public:
  virtual void OnInitialUpdate();
protected:

  CImageList  m_ImageList;

  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  // Implementation
protected:
  CEditEx EdUltimoCampo,EdPrimeiroCampo;
  CImageListBox ListaIcones;
  CListBox ListaEspacos;

  virtual ~CEdiAciTopogView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void AlterouEstadoMarca();
  afx_msg void OnGravar();

  DECLARE_MESSAGE_MAP()

public:

  afx_msg void OnClickedGravar();
  afx_msg void OnClickedLimparCampos();
  CCheckListBox ListCheckBox;
  CFont m_font,m_font2;
  afx_msg void OnLbnSelchangeLisforcar();
  CStatic m_IconeSelecionado;
  BOOL m_ForcarSelecionado;
  afx_msg void OnSelchangeLisicon();
  afx_msg void OnSelchangeLisespaco();
  afx_msg void OnClickedButpadrao();
};

/////////////////////////////////////////////////////////////////////////////
