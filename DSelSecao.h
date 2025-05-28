#if !defined(AFX_DSELSECAO_H__38702E23_E907_11D3_86CC_00A6FD000000__INCLUDED_)
#define AFX_DSELSECAO_H__38702E23_E907_11D3_86CC_00A6FD000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DSelSecao.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDSelSecao dialog

class CDSelSecao : public CDialog
{
  BOOL ESecoes;

  CWnd *Pai;
  CRect &pSecao;  

  CToolBar *mpCDesTool1,           //--- Ponteiros para os toolbars do desenho da secao
           *mpCDesTool2,           //--- Qundo desenhado com o greidde é preciso guardar estes ponteiros 
           *mpCDesTool3,           //--- para poder fazer os tollbars aparecem e desamarecererm 
           *mpCDesTool4;

  CDialog *mpDVolumes;             //--- Ponteiro para o dialogo de volumes
  int **pDados;                    //--- Ponteiro para o array ded ponteiros de dados a configurar
  COLORREF* pCores;                //--- Ponteiro para o arrey de ponteiros de cores a configurar

  // Construction
public:

  //--- Posicao da janela da secao;

  CDSelSecao(CWnd *pParent = NULL,CToolBar *DesTool1 = NULL,CToolBar *DesTool2 = NULL,CToolBar *DesTool3 = NULL,CToolBar *DesTool4 = NULL,CDialog* DVolumes = NULL,
             CRect& Jan = CRect(0,0,0,0),BOOL PSecoes = FALSE,int** pDadosConfig=NULL,COLORREF* pCoresConfig=NULL);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDSelSecao)
  enum { IDD = IDD_SELECSECAO };
  CButton	m_CBVolumes;
  CButton	m_CBCoordenadas;
  CButton	m_CBPonto;
  CButton	m_CBEscalas;
  CButton	m_CBProcurar;
  CButton	m_CBFerramentas;
  CButton m_CBPavimento;
  CButton m_CBTerraplanagem;
  CButton m_CBTerreno;
  CButton m_CBMalha;
  CButton m_CBCotarMalha;
  CButton m_CBCircularPontos;
  CButton m_CBPerfisGeologicos;
  CButton m_RADMostrarTodasSecoes;
  CButton m_RADTerrenoeProjeto;
  CButton m_RAD1OffSet;
  CButton m_RAD2OffSets;
  CButton m_RADTerreno;
  UINT	m_uintEquidistancia;
  UINT	m_uintEquidisMalha;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDSelSecao)
public:
  virtual INT_PTR DoModal();
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDSelSecao)
  afx_msg void OnVerFerramentas();
  afx_msg void OnVerEscalas();
  afx_msg void OnVerProcurar();
  virtual BOOL OnInitDialog();
  afx_msg void OnVerCoordenadas();
  afx_msg void OnDestroy();
  afx_msg void OnMbok();
  afx_msg void OnVolumes();
  afx_msg void OnCores();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSELSECAO_H__38702E23_E907_11D3_86CC_00A6FD000000__INCLUDED_)
