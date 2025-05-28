//--- Esta classe contem os parametros para o set de interseções 
//--- Usada no Desenhos geométrico, Ajuste curvas de nivel de terraplanagem com o terreno

class CParamSetCN
{
public:

  Ponto PontoCN,Intersecao;
  ItLDeltaSup itBordo;
  ItLLPontos itLLCN;
  ItLPontos itPontoAtualCN;

  CParamSetCN(const Ponto& pPonto, ItLDeltaSup pitBordo, ItLLPontos pitLLCN, ItLPontos pitPAtualCN,Ponto &Inter) : PontoCN(pPonto), itBordo(pitBordo), itLLCN(pitLLCN), itPontoAtualCN(pitPAtualCN),Intersecao(Inter){};
  CParamSetCN(const Ponto& pPonto) : PontoCN(pPonto) {};

  bool operator<(const CParamSetCN& RHS) const
  {
    double Dx(PontoCN.x - RHS.PontoCN.x), Dy(PontoCN.y - RHS.PontoCN.y), Dz(PontoCN.z - RHS.PontoCN.z);

    return Dx < -1e-6 ? true : Dx > 1e-6 ? false :
           Dy < -1e-6 ? true : Dy > 1e-6 ? false :
           Dz < -1e-6 ? true : false;
  }
};

typedef std::set<CParamSetCN> tySetCParamSetCN;
typedef tySetCParamSetCN::iterator ittySetCParamSetCN;

//--- Esta classe contem os parametros para o set de interseções das CN com os bordos
//--- Usada no Desenhos geométrico, Ajuste curvas de nivel de terraplanagem com o terreno

class CParamSetInterBordos
{
public:
  const Ponto& PontoBordo, &PontoBordoAnt;
  ItLPontos ItLPontoMaisProxCN;
  ItLLPontos ItCNPonMaisProx;
  double Distancia;

  CParamSetInterBordos(const Ponto& PonBordo,const Ponto& PontoBorAnt, ItLPontos pTITLPCN,ItLLPontos pItLLCN,double pDist) : PontoBordo(PonBordo), 
                      PontoBordoAnt(PontoBorAnt), ItLPontoMaisProxCN(pTITLPCN), ItCNPonMaisProx(pItLLCN), Distancia(pDist) {};

  bool operator<(const CParamSetInterBordos& RHS) const
  {
    double Dx(ItLPontoMaisProxCN->x - RHS.ItLPontoMaisProxCN->x), Dy(ItLPontoMaisProxCN->y - RHS.ItLPontoMaisProxCN->y), Dz(ItLPontoMaisProxCN->z - RHS.ItLPontoMaisProxCN->z);

    return Dx < -1e-6 ? true : Dx > 1e-6 ? false :
           Dy < -1e-6 ? true : Dy > 1e-6 ? false :
           Dz < -1e-6 ? true : false;
  };
};

typedef std::set<CParamSetInterBordos> tySetInterBordosDouble;
typedef tySetInterBordosDouble::iterator ittySetInterBordosDouble;


