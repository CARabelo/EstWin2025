#if !defined(AFX_DGERARDXFGREIDE_H__AD2E4002_492A_4593_9695_DD792E069353__INCLUDED_)
#define AFX_DGERARDXFGREIDE_H__AD2E4002_492A_4593_9695_DD792E069353__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGerarDXFGreide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFGreide dialog

class CDGerarDXFGreide : public CDialog
{
  double Deltas[2],Escala[2];

  CString NomeProjeto;

  bool OK;                           //--- fica trtue se inicializou ok;

  Ponto Ppixels,PAtual;

  CDrawing drawing;

  tysetxCotas SetxCotas,SetxCotasGreide;

  CCurVerticais CurvasVerticais;      //--- Dados do Greide de projeto.  
  CListaSecoes Secoes;                //--- Dados das seções, para pegar o perfil do terreno.  
  CHPavimento HPavimento,HLimpeza;
  int DirecaoAtual;

  void ConvEstRealPVertex(REALPOINT& Vertex,const CEstaca& Estaca)   //--- converte coordenadas reais para coordenadas do vídeo
  {
    Vertex.x = Estaca.EstVirtual;
    Vertex.y = Estaca.Cota * 10.0;
  };

  bool DesenhaParabola(ENTVERTEX PCV,ENTVERTEX PIV,ENTVERTEX ENTVERTEXPTV,PENTVERTEX Auxiliares=NULL);

  bool BuscaGradiente(Ponto& p,const Perfil& PerfilPav)
  {
    POSITION Atual(PerfilPav.GetHeadPosition());

    while (Atual && PerfilPav.GetAt(Atual).Descricao != "GRADIE")  PerfilPav.GetNext(Atual);

    if(Atual) 
    {
      p = PerfilPav.GetAt(Atual);

      return true;
    }

    return false;
  }


  void DistribuiCotas(double XCota,tysetxCotas& SetxCotas);
  // Construction
public:
  CDGerarDXFGreide(CWnd* pParent,CString& NomeProj);	

  // Dialog Data
  //{{AFX_DATA(CDGerarDXFGreide)
  enum { IDD = IDD_GERARDXFGRE };
  CButton	m_BUGerar;
  CButton	m_BUSair;
  CProgressCtrl	m_Progresso;
  double	m_CStrRazaoHV;
  CString	m_CStrMensagem;
  //}}AFX_DATA

  enum { VU_E_LADOESQUERDO, VU_E_LADODIREITO, VU_E_ACABOUESPACO };

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDGerarDXFGreide)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDGerarDXFGreide)
  afx_msg void OnGerar();
  afx_msg void OnSair();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  double AlturaCotas,DistMinCotas,DistEntreCotas;

  ~CDGerarDXFGreide();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGERARDXFGREIDE_H__AD2E4002_492A_4593_9695_DD792E069353__INCLUDED_)
