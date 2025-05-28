
struct ltMSecPonto
{
  bool operator()(const Ponto* p1, const Ponto* p2) const
  {
    double d(p1->x - p2->x);

    return d < -1e-5 || (d < 1e-5 && (p1->y - p2->y < -1e-5));
  }
};

typedef std::map<CEstaca, Ponto> MSecaoGeom;    //--- Secoes geometricas para acesso randomico
typedef MSecaoGeom::iterator itMSecaoGeom;

typedef std::map<Ponto*, const CEstaca*, ltMSecPonto> MSecaoGeomPonto;  //--- Ordenado por ponto
typedef MSecaoGeomPonto::iterator itMSecaoGeomPonto;
typedef std::pair<Ponto, Ponto> PairDoisPontos;

class CTrecho
{
  std::string NomeTrecho;

  public:

  CSecTipo SecoesTipo;
  CCurHorizontais CurvasHorizontais;
  CSuperficie SuperficieTerr;           

  const MCurHorizontaisTS& mapCurvasHorizontaisTS;
  const MCurHorizontaisPI& mapCurvasHorizontaisPI;
  const MCurHorizontaisCC& mapCurvasHorizontaisCC;

  ItCMCurHorizontaisTS ItCurHorizSendoArrastadaTS;
  ItCMCurHorizontaisPI ItCurHorizSendoArrastadaPI;
  ItCMCurHorizontaisCC ItCurHorizSendoArrastadaCC;
  ItCMCurHorizontaisPI ItCurHorizSendoArrastadaPIAnterior;
  ItCMCurHorizontaisPI itUltimaCurvaArrastadaPI;
  ItCMCurHorizontaisCC itUltimaCurvaArrastadaCC;

  lstPontos LPSecoes;
  CSecoesAvulsas SecoesAvulsas;
  CAlargamento Alargamentos;
  CArqAciTopograficos ArquivoAciTopograficos;
  MSecaoGeom MapaSecoesGeom;         //--- Mapa de secoes geometricas.
  MSecaoGeomPonto MapaSecoesPonto;   //--- Set de secoes orddenadas por ponto, os elementos são ponteiros para o mapa de 
  itmapSecoesAvulsas ItSecaoAvulsaAtual;

  CTrecho(const std::string& pNomeTrecho);
  ~CTrecho();

  bool operator<(const CTrecho& RHS) const
  {
    return NomeTrecho < RHS.NomeTrecho;
  }
};

