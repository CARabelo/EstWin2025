// ChildFrm.h : interface of the CChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

class CChildFrame : public CMDIChildWnd
{
  xyMouse MouseXY;
  CXYGreide MouseXYGre,MouseXYGeo;
 
  // DiapontosGeom DiaPonGeom;

  BOOL CreatePaletteBar(CPaletteBar& Palette,UINT Recurso);
  void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);
  int CriaToolBar(CToolBar& Barra,UINT Recurso);

  DECLARE_DYNCREATE(CChildFrame)

public:
   
  CMelEditBox EdCorPerfAtual;

  CChildFrame();

  BOOL DesligaStatusBar() { return m_wndStatusBar.ShowWindow(FALSE) ; };
  void Fechar() { OnClose(); };
  void MudaCorPerfGeoSel(COLORREF NovaCor) 
  { 
    EdCorPerfAtual.MudaCorFundo(NovaCor);

    CDC* pDC(EdCorPerfAtual.GetDC());

    CRect rc;                   
    EdCorPerfAtual.GetClientRect(&rc); 
    pDC->FillSolidRect( rc, NovaCor);		

    EdCorPerfAtual.ReleaseDC(pDC);
  };

  // Attributes
public:
  CDDeslizantes Deslizantes,DeslizantesGre,DeslizantesGeol;
  CDeslizantesGeom DeslizanteGeom;
 
  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CChildFrame)
public:
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext);
  virtual void ActivateFrame(int nCmdShow = -1);
  //}}AFX_VIRTUAL

  // Implementation
public:
  CStatusBar  m_wndStatusBar;

  virtual ~CChildFrame();

  //--- extratores

  CComboBox& PegaComboProcurarDesSecao() { return m_SearchBox; };


#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  CToolBar               Subtit/*,m_wndToolBarGre/*,m_wndToolBarPonGeo*/;
  CMyToolBar             m_wndToolBarDes,m_wndToolBarDes2,m_wndToolBarDes3,m_wndToolBarDes4,m_wndToolBarDes5,
                         m_wndToolBarTalvegues;
  CPaletteBar            m_wndSecoesPalette;
  CComboProcurarDesSecao m_SearchBox;
  CDialog                Subtitulo;
  int                    NumTar;

  //{{AFX_MSG(CChildFrame)
  afx_msg void OnClose();
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
  afx_msg void OnUpdateButFechar(CCmdUI* pCmdUI);
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  //  virtual BOOL DestroyWindow();

  CToolBar m_wndToolBarPonGeo,m_wndToolBarGre,m_wndToolBarPerfGeo;
  CMyToolBar m_wndToolBarAciTopograficos, m_wndToolBarEquiDrenagem;
  afx_msg void OnSetFocus(CWnd* pOldWnd);
};

/////////////////////////////////////////////////////////////////////////////
