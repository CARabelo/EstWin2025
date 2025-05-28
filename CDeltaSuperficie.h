// CDeltaSuperficie.h: interface for the CDeltaSuperficie class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDELTASUPERFICIE_H__95E0BAE1_D0DA_11D7_BE61_CABF7BB0A41D__INCLUDED_)
#define AFX_CDELTASUPERFICIE_H__95E0BAE1_D0DA_11D7_BE61_CABF7BB0A41D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct ltPontoS 
{ 
  bool operator()(const Ponto& p1, const Ponto& p2) const 
  { 
    return (p1.z - p2.z) < -5E-4;
  }
};

typedef std::set<Ponto,ltPontoS> setPontosS;
typedef setPontosS::iterator itsetPontosS;

class CAresta
{
public:
  Ponto PontoLigado;
  setPontosS PontosInterpolacao;
  int Lado;     //--- auxiliar

  CAresta(const Ponto& P): PontoLigado(P),Lado(0) {};

  itsetPontosS AchaPontoNaCota(double Cota,bool pBaliza = false) const
  {
    itsetPontosS it(PontosInterpolacao.find(Ponto(0.0,0.0,Cota)));
    
  /*  for(it = PontosInterpolacao.begin() ; it != PontosInterpolacao.end() ;  it++)
    {
       double dif = it->z - Cota;
      if(fabs(it->z - Cota) < (1E-3)) break;
    }
    */   
	if(it != PontosInterpolacao.end())
	{
	  if(pBaliza == true) (const_cast<Ponto*>(&*it))->Baliza = true;
	  return it;
	}    
	   
    return PontosInterpolacao.end();
   
  }

  bool operator==(const CAresta& RHS) const { return PontoLigado == RHS.PontoLigado;}; 
  bool operator==(const Ponto* P) const { return PontoLigado == P;}; 

  const CAresta& operator=(const CAresta& RHS)
  {
    PontoLigado = RHS.PontoLigado;
    Lado = RHS.Lado;

    PontosInterpolacao.clear();

    for(register itsetPontosS it = PontosInterpolacao.begin() ; it != PontosInterpolacao.end() ; it++)
    {
      PontosInterpolacao.insert(*it);
    }
  }

  bool operator<(const CAresta& RHS) const
  {
    return PontoLigado < RHS.PontoLigado;
  }
};

struct ltArestaS 
{ 
  bool operator()(const CAresta& A1, const CAresta& A2) const 
  { 
    double DeltaS(A1.PontoLigado.fi - A2.PontoLigado.fi);
   // Deltax(fabs(A1.PontoLigado.x-A2.PontoLigado.x)),Deltay(fabs(A1.PontoLigado.y - A2.PontoLigado.y));

    //--- Os pontos podem ser iguais e os azimutes ligeiramente diferentes, porisso é preciso verificar se os pontos não são iguais - 30/07/2022
    //--- (Deltax > 1e5 || Deltay > 1e5) && esta dando efeitos colaterais @Carlos verificar

    return DeltaS < 0.0 ? true : false;
  }
};

typedef std::set<CAresta,ltArestaS> tysetArestas;                  //--- Lista de pontos ligados a esta superficie
typedef tysetArestas::iterator ittysetArestas;   

typedef std::set<CAresta> tysetArestasP;                  //--- set de pontos ligados a esta superficie = a lista
typedef tysetArestasP::iterator ittysetArestasP;

class CDeltaSuperficie  
{
public:

  enum EnuStatus{HABILITADO,DESABILITADO,INSERIDO_USUARIO,INSERIDO_DÈLAUNAY,INSERIDO_GABRIEL,DESENHADO};

  tysetArestas PontosLigados;

  Ponto PCentral;
  bool EBordo,ERestricao;
  int TipoObjeto,Status;          

  const	Ponto& PegaPCentral() const {return PCentral;};
  Ponto& PegaPCentralNC() { return PCentral;};                 //--- NC = Não constante
  int PegaQuanPLigados() const { return (int) PontosLigados.size();};
  void IniciaPontosLigados() {itSuperfLigadaAtual = PontosLigados.begin();};
  void RotacionaPcentral(double Seno,double Coseno) { PCentral.RotacionaPlano(Seno,Coseno,0.0);};
  void DeixouDeSerRestricao() {ERestricao = false;EBordo=false;};   
  void DeixouDeSerBordo() {EBordo = false;}; 
  bool EstaLigado(const Ponto& P) const
  {
	  register ittysetArestas it;

	  for(it = PontosLigados.begin() ; it != PontosLigados.end() ; it++)
	  {
		  if(*it == P) break;
	  }

	  return it != PontosLigados.end();
  }

  ittysetArestas PegaPontoLigadoXY(const Ponto& p) const
  {
    for (register ittysetArestas itPL = PontosLigados.begin() ; itPL != PontosLigados.end() ; itPL++) if( itPL->PontoLigado == p  ) return itPL;

    return PontosLigados.end();
  }

  bool operator<(const CDeltaSuperficie& D) const {	return PCentral < D.PCentral;}
  bool operator<(const CDeltaSuperficie* D) const { return PCentral < D->PCentral;}  
  bool operator==(const CDeltaSuperficie* D) const { return PCentral == D->PCentral;}
  bool operator==(const CDeltaSuperficie& D) const { return PCentral == D.PCentral;}
  bool operator!=(const CDeltaSuperficie& D) const { return PCentral != D.PCentral;}
  const CDeltaSuperficie& operator=(const CDeltaSuperficie& D);
  CDeltaSuperficie& operator+=(const CDeltaSuperficie& D);                //--- Inclui as arestas de D na deltasup atual

  bool Ebordo() const { return EBordo; };

  ittysetArestas itSuperfLigadaAtual;

  void Serialize(CArchive& ar,void* vpSetSuperficie,bool SoPCentral=false,bool PontosCN=false);

  CDeltaSuperficie(double x=INFINITO,double y=INFINITO, double z=INFINITO,double i=INFINITO,double fi=INFINITO,double S=INFINITO,CString& Desc=CString(""),int pSta=HABILITADO,
                   int pTipoObj=0,const CString& pNome=CString("")) : PCentral(x,y,z,i,fi,S,Desc,false,pTipoObj==CDeltaSuperficie::INSERIDO_USUARIO,pNome),ERestricao(false),
                   EBordo(false),Status(pSta),TipoObjeto(pTipoObj){};

  CDeltaSuperficie(const Ponto& Pon) : PCentral(Pon),ERestricao(false),EBordo(false),Status(HABILITADO) {};

  CDeltaSuperficie(const Ponto& Pon,int TipObj) : PCentral(Pon), TipoObjeto(TipObj), ERestricao(false), EBordo(false), Status(HABILITADO) {};

  void MontaChave() {PCentral.MontaChaves();};

  virtual ~CDeltaSuperficie(){};
};


typedef std::list<CDeltaSuperficie> LDeltaSup;
typedef LDeltaSup::iterator ItLDeltaSup;
typedef LDeltaSup::const_iterator ItCLDeltaSup;
typedef LDeltaSup::reverse_iterator ItRLDeltaSup;

typedef std::list<LDeltaSup> LLDeltaSup;
typedef LLDeltaSup::iterator ItLLDeltaSup;
typedef LLDeltaSup::const_iterator ItCLLDeltaSup;
typedef LLDeltaSup::reverse_iterator ItRLLDeltaSup;

typedef std::list<ItLLDeltaSup> LItLLDeltaSup;
typedef LItLLDeltaSup::iterator ItLItLLDeltaSup;
typedef LItLLDeltaSup::const_iterator CItLItLLDeltaSup;

typedef std::list<ItCLLDeltaSup> LItCLLDeltaSup;
typedef LItLLDeltaSup::iterator ItLItCLLDeltaSup;

typedef std::list<ItLDeltaSup> LItLDeltaSup;
typedef LItLLDeltaSup::iterator ItLItLDeltaSup;;

#endif // !defined(AFX_CDELTASUPERFICIE_H__95E0BAE1_D0DA_11D7_BE61_CABF7BB0A41D__INCLUDED_)


