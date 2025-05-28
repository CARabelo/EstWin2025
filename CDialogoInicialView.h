// CurVerView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurVerView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CDialogoInicialView : public CFormView //public CTypedPtrListView
{
	int* pTipoProjeto;
	CString* pLstProjRecentes;
	bool ProjExistente;
	CBrush BrushWhite;

	int VerfTipoProjeto(std::string& Projeto);

protected:

  CDialogoInicialView(int* pTipoProj=nullptr, CWnd* pParent=nullptr, CString* ppLstProjRec=nullptr);          
	void AtualizaListaProjRecentes();

  DECLARE_DYNCREATE(CDialogoInicialView)

  // Form Data
public:
	std::string ProjSelecionado;

	const std::string& ProjetoSelecionado() const { return ProjSelecionado; };

  //{{AFX_DATA(CCurVerView)
  enum { IDD = IDD_DIAINICIAL };
  //}}AFX_DATA

  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCurVerView)
public:
	virtual void OnInitialUpdate();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
  virtual ~CDialogoInicialView();
#ifdef _DEBUG
#endif
  DECLARE_MESSAGE_MAP()
public:
    CMListBox CLstProjExistente;
    BOOL CheIniDesSuperf;

	afx_msg void OnBnClickedButtopdrones();
	afx_msg void OnBnClickedButtoptradicional();
	afx_msg void OnBnClickedButtopmapasibge();
	afx_msg void OnBnClickedButtopgoogle();
	afx_msg void OnBnClickedButmaistoptradi();
	afx_msg void OnBnClickedButtopacad();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedButordenarproj();
	afx_msg void OnLbnDblclkLisprojexistente();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButprocurarproj();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButmaisgoogle();
	afx_msg void OnBnClickedButmaisibge();
	afx_msg void OnBnClickedButmaisdrones();
	afx_msg void OnUpdateButOK(CCmdUI* pCmdUI);
	afx_msg void OnBnClickedButtopsig();
};

/////////////////////////////////////////////////////////////////////////////
