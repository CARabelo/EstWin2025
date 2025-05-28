#if !defined(AFX_DGerarDXFGre_H__367555DC_F7B9_4F9F_B3FE_4E3A85AB65FB__INCLUDED_)
#define AFX_DGerarDXFGre_H__367555DC_F7B9_4F9F_B3FE_4E3A85AB65FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGerarDXFGre.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFGre dialog

class CDGerarDXFGre : public CDialog
{
  double Deltas[2],
    Escala[2];

  bool OK;          //--- Se inicializou ok, fica true

  Ponto Ppixels,PAtual;

  CDrawing drawing;

  CCurVerticais CurvasVerticais;      //--- Dados do Greide de projeto.  
  CListaSecoes Secoes;                //--- Dados das seções, para pegar o perfil do terreno.  
  CHPavimento HPavimento;

  void ConvEstRealPVertex(REALPOINT& Vertex,const CEstaca& Estaca)   //--- converte coordenadas reais para coordenadas do vídeo
  {
    Vertex.x = Estaca.EstVirtual;
    Vertex.y = Estaca.Cota;
  };

  // Construction
public:
  CDGerarDXFGre(CWnd* pParent = NULL,CString& NomeProj = CString(""));   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDGerarDXFGre)
  enum { IDD = IDD_GERARDXFGEO };
  CButton	m_BUGerar;
  CProgressCtrl	m_PrgProgresso;
  CString	m_CStrMensagem;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDGerarDXFGre)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDGerarDXFGre)
  afx_msg void OnGerarDXF();
  afx_msg void OnSair();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  CButton CheTodosTrechos;
  CButton CheTrechoAtual;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGerarDXFGre_H__367555DC_F7B9_4F9F_B3FE_4E3A85AB65FB__INCLUDED_)
