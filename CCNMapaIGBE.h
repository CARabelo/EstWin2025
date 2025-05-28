class CCNMapaIGBE
{
  double Cota;

  lstPontos LPontos;

  public:

    CCNMapaIGBE::CCNMapaIGBE(double pCota=INFINITO,const lstPontos& pLPontos=lstPontos()) : Cota(pCota),LPontos(pLPontos) {}

    lstPontos& LstPontos() { return LPontos;}; 
};

typedef std::list<CCNMapaIGBE> tylstCN;
typedef tylstCN::iterator ittylstCN;
typedef tylstCN::const_iterator cittylstCN;
typedef tylstCN::reverse_iterator rittylstCN;