// typlstvw.h : header file
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrListView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#define NUM_MAX_CAMPOS 12

class CTypedPtrListView : public CFormView
{
  int TecShift;
  int Template,
      Estado,                      //--- Indica o estado da edição: inserçao, deleção ou edição
      PagAtual;
  CPoint paper_res;
  unsigned int UltimoCampo,PrimeiroCampo,QuanCampos;

  void MostraRegistroSendoEditado();
  ittlstwstr ItUltimoCampo,ItPrimeiroCampo;
  size_t TextExtent;
public:

  CTypedPtrListView::CTypedPtrListView(UINT i= CTypedPtrListView::IDD, int pQuanCampos=0, int pUltCampo=0, int pPrimCampo=0);
  DECLARE_DYNCREATE(CTypedPtrListView)

  tlstwstr ListaCampos;
  unsigned long PosArqRav;                   //--- O sort do listbox não é binario porisso essa var

  // Attributes
public:									 
  CEstDoc *GetDocument();
  int TeclaShift(int Valor = -1) { if (Valor != -1) TecShift = Valor; return TecShift ; }

  // Overrides
public:
  void OnInitialUpdate();

  // Form Data
public:
  //{{AFX_DATA(CTypedPtrListView)
  enum { IDD = IDD_IGUALDADES };
  CListBox m_ctlList;
  CString m_str,m_Campo[12];

  //}}AFX_DATA

  // Implementation
protected:

  enum{ INSERCAO,DELECAO,EDICAO,INSERIRANTES};

  CMyStruct* FindMyStruct(int& nSel, POSITION& pos);
  void AddMyStructToListBox(CMyStruct* pMyStruct, int nSel = -1);
  inline void LimpaBrancos(CString& CSDado);          
  inline void TabulaString(CString& CSDado,int TamTab);
  void LimpaCampos() 
  { 
    for (register int C = 0; C < NUM_MAX_CAMPOS; C++) m_Campo[C] = "";

    ListaCampos.clear();
  };
  void LimpaUltimosCamposBrancos();

  // Overrides
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CTypedPtrListView)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);
  virtual void DoDataExchange(CDataExchange* pDX,int Arquivo = 0);    // DDX/DDV support
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
  virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
  virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~CTypedPtrListView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
  //{{AFX_MSG(CTypedPtrListView)
  afx_msg void OnAdd();
  afx_msg void OnUpdate();
  afx_msg void OnRemove();
  afx_msg void OnRemoveAll();
  afx_msg void OnSelChangeList();
  afx_msg void OnInsertBefore();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  //afx_msg void OnFilePrint();
  afx_msg void OnFilePrint();
protected:
  virtual void OnDraw(CDC* /*pDC*/);
  //virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
public:
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  virtual int Consiste(tlstwstr& LstDados);
  afx_msg void OnBnClickedGravar();
  afx_msg LRESULT OnKillFocusUltimoCampo(WPARAM WP, LPARAM LP);
  virtual BOOL PreTranslateMessage(MSG* pMsg);
  afx_msg void OnGravar();
  afx_msg void OnLimparCampos();
};

#ifndef _DEBUG
inline CEstDoc* CTypedPtrListView::GetDocument() { return (CEstDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
