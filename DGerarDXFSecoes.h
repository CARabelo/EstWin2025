#if !defined(AFX_DGERARDXFSECOES_H__1145D513_FFC1_41D9_ADEF_379DE447BC37__INCLUDED_)
#define AFX_DGERARDXFSECOES_H__1145D513_FFC1_41D9_ADEF_379DE447BC37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DGerarDXFSecoes.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFSecoes dialog
/*
class CxCotas2
{
  double Valorx,xCota;   //--- x onde o ponto será escrito

  public:
  
    CxCotas2(double XCota) : Valorx(XCota) , xCota(XCota) {};
    double PegaxCota() const { return xCota;};
    double PegaValorx() const { return Valorx;};
    double operator-=(double Valor) { xCota -= Valor; return xCota;};
    bool operator<(const CxCotas2& RHS) const { return Valorx < RHS.Valorx;};
};

typedef std::multiset<CxCotas2> tysetxCotas2;
typedef tysetxCotas2::iterator ittysetxCotas2;
typedef tysetxCotas2::reverse_iterator rittysetxCotas2;
*/
typedef std::list<POSITION> tylstPOSITION;
typedef tylstPOSITION::iterator ittylstPOSITION;

typedef std::list<const Perfil*> tylstpPerfil; 
typedef tylstpPerfil::iterator ittylstpPerfil;

typedef std::list<CString> tylststdcstr;
typedef tylststdcstr::iterator ittylststdcstr;

class CDGerarDXFSecoes : public CDialog
{
  CListaSecoes Secoes,SecoesAvulsas;                //--- Dados das seções.  
  CSecoesAvulsas SecoesAvulsasOrig;
/*
  CPerfilGeologico PerfilGeologico;  //--- Perfís geológicos do terreno
  tysetCTipoSolo SetTiposSolos;
  itsetCDadosPerfGeologicos itPerfGeo;
  Perfil PerfOffSetsPerfGeologico;   //--- Temporário, para mostrar as descrições dos offsets dos perfis geologicos
  */

  CDrawing drawing;
  tysetxCotas SetxCotas;
  std::string NomeProjeto;
  double MenorCota,MaiorCota,MenorAfast,MaiorAfast;

  void PassaSecoesAvulsasOriSecoes();

  void ConvPontoPVertex(REALPOINT& Vertex,const Ponto& P,const Ponto& OffSet,double YAjuste)   //--- converte coordenadas reais para coordenadas do vídeo
  {
    Vertex.x = P.x + OffSet.x;
    Vertex.y = P.y + OffSet.y + YAjuste;
  };

  std::string& RemoveBrancos(const std::string& String)
  {
    static std::string Resposta;

    Resposta.clear();

    for(register std::string::const_iterator it = String.begin() ; it != String.end() ; ++it) 
    {
      if(*it != ' ') Resposta += *it;
    }

    return Resposta;
  }

  Ponto CentralizaSecao(CSecao& SecaoAtual);

  void SeparaMenores(const Ponto& PontoAtual)
  {
     if(MenorCota > PontoAtual.y) MenorCota = PontoAtual.y;
     if(MaiorCota < PontoAtual.y) MaiorCota = PontoAtual.y;

     if(MenorAfast > PontoAtual.x) MenorAfast = PontoAtual.x;
     if(MaiorAfast < PontoAtual.x) MaiorAfast = PontoAtual.x;
  }

   int DirecaoAtual;

   enum { VU_E_LADOESQUERDO, VU_E_LADODIREITO, VU_E_ACABOUESPACO };

   void DistribuiCotas(double XCota,tysetxCotas& SetxCotas);

   int DesenhaMarca(ENTVERTEX P1,double XCota,double MeioTamLetra);

  // Construction
public:
  CDGerarDXFSecoes(CWnd* pParent,CString& NomeProj);  
  ~CDGerarDXFSecoes();

  double CalcCotaTerreno(double x, Perfil* PerfTerreno);

  // Dialog Data
  //{{AFX_DATA(CDGerarDXFSecoes)
  enum { IDD = IDD_GERARDXFSEC };
  CButton	m_BUGerar,m_BUSair;
  CProgressCtrl	m_ProgProgresso;
  CString	m_CStrMensagem;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDGerarDXFSecoes)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDGerarDXFSecoes)
  afx_msg void OnGerarSecoes();
  afx_msg void OnSair();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  BOOL m_ChePerfisGeologicos;
  double AlturaCotas,DistMinCotas,DistEntreCotas,AltCotas;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DGERARDXFSECOES_H__1145D513_FFC1_41D9_ADEF_379DE447BC37__INCLUDED_)
