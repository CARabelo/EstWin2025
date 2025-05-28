// CRelACADAciTopogView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRelACADAciTopogView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include "afxwin.h"

class CRelACADAciTopogView : public CTypedPtrListView
{
  int Icone;
  bool Forcar;
  int nSel;
  int Erro;

  std::map<std::string,int> MapaPosicaoIcones;

protected:
  CRelACADAciTopogView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CRelACADAciTopogView)

  int Consiste(tlstwstr& ListaCampos);

  // Form Data
public:
  enum { IDD = IDD_RELACADACITOPOG};

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
  void ColocaIconePosicaoCcorreta(int PosicaoCorreta, int Icone);

  // Implementation
protected:
  CEditEx EdUltimoCampo,EdPrimeiroCampo;
  CImageListBox ListaIcones;
  CComboBox ComboLayAutocad;
  CComboBox ComboAciTopogVUE;

  virtual ~CRelACADAciTopogView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnSelChangeList();
  afx_msg void AlterouEstadoMarca();
  DECLARE_MESSAGE_MAP()

public:

  CCheckListBox ListCheckBox;
  afx_msg void OnLbnSelchangeLisforcar();
  CStatic m_IconeSelecionado;
  BOOL m_ForcarSelecionado;
  afx_msg void OnSelchangeLisicon();
  afx_msg void OnClickedButpadrao();
  afx_msg void OnCbnSelchangeComacivue();
  afx_msg void OnLbnSelchangeLislayautocad();
  afx_msg void OnLbnSelchangeLisacitopogvue();
  afx_msg void OnLbnSelchangeLisiconrelaci();
  afx_msg void OnLbnSelchangeLisforcarrelaci();

  CListBox CListaLayerAutocad;
  CListBox CListaAciTopogVUE;
  CListBox ListaDescricao;
  CButton CheForcarAresta;
};

/////////////////////////////////////////////////////////////////////////////
