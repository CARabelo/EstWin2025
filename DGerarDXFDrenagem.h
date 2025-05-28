#if !defined(AFX_DGERARDXF_H__367555DC_F7B9_4F9F_B3FE_4E3A85AB65FB__INCLUDED_)
#define AFX_DGERARDXF_H__367555DC_F7B9_4F9F_B3FE_4E3A85AB65FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGerarDXF.h : header file
//
typedef std::map<CEstaca,Ponto> MSecaoGeom;                           //--- Secoes geometricas para acesso randomico
typedef MSecaoGeom::iterator itMSecaoGeom;

typedef std::vector<ENTVERTEX> VENTVERTEX;
typedef VENTVERTEX::iterator itVENTVERTEX;

typedef std::vector<Ponto> vecPontos;
typedef VENTVERTEX::iterator itvecPontos;

#define ESQUERDO 0
#define DIREITO 1
#define BORDOS 0
#define RESTRICOES 1

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXF dialog

class CDGerarDXF : public CDialog
{
  double Deltas[2],Escala;

  CString NomeProjeto,NomeProjetoBase;

  bool OK;                              //--- Se inicializou ok, fica true

  Ponto Ppixels,PAtual,PLocal;

  CSuperficie Superficie;                //--- Superficie com os pontos  

  void DesenhaEspiral(const CurHorizontal& Curva,int* result, CSecTipo& SecoesTipo, CCurHorizontais& CurvasHorizontais);
  inline void IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,Ponto& PCalcular);
  inline void IrradiaPonto(double Angulo,double x,double y, double s,double Distancia,REALPOINT& PCalcular);
  inline double CalculaDeltaDes(const CurHorizontal& Curva,double l);
  void DesenhaCircular(const CurHorizontal& Curva, int* result,CSecTipo& SecoesTipo,CCurHorizontais& CurvasHorizontais);

  CDrawing drawing;
    
 inline void CDGerarDXF::ConvPRealPVertex(REALPOINT& Vertex,const Ponto& P)   //--- converte coordenadas reais para coordenadas do vídeo
  {
    Vertex.x = (P.x - Deltas[0]);
    Vertex.y = (P.y - Deltas[1]);
    Vertex.z = P.z;
  }

  // Construction
public:
  CDGerarDXF(CWnd* pParent = NULL,CString& NomeProj = CString(""),CString& NomeProjBase = CString(""),double pCx=0.0,double pCy=0.0, double pEscala=1.0);   // standard constructor

  // Dialog Data
  //{{AFX_DATA(CDGerarDXF)
  enum { IDD = IDD_GERARDXFGEO };
  CButton	m_BUGerar,m_BUSair;
  CProgressCtrl	m_PrgProgresso;
  CString	m_CStrMensagem;
  CButton CheTodosTrechos, CheTrechoAtual;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDGerarDXF)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDGerarDXF)
  afx_msg void OnGerarDXF();
  afx_msg void OnSair();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedChetodostrechos();
  afx_msg void OnBnClickedChectrechoatual();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGERARDXF_H__367555DC_F7B9_4F9F_B3FE_4E3A85AB65FB__INCLUDED_)
