class CChaveAlinhamento
{
  double EstacaInicial;
  double Afastamento;
  std::string strDescricao;
  int Tipo;                 //---corte ou aterro

public:
  CChaveAlinhamento(double pEstIni, double pAfast,std::string& pDescr,int pTipo) : EstacaInicial(pEstIni), Afastamento(pAfast), strDescricao(pDescr),Tipo(pTipo) {};
  CChaveAlinhamento() {};

  bool operator<(const CChaveAlinhamento& RHS) const
  {
    double dx(EstacaInicial - RHS.EstacaInicial),
           dy(Afastamento - RHS.Afastamento);

    return dx < -0.0005 ? true : dx > 0.0005 ? false :
           dy < -0.0005 ? true : dy > 0.0005 ? false :
           strDescricao < RHS.strDescricao ? true : false;
  }

  const CChaveAlinhamento& operator=(const CChaveAlinhamento RHS)
  {
    EstacaInicial = RHS.EstacaInicial;
    Afastamento = RHS.Afastamento;
    strDescricao = RHS.strDescricao;

    return *this;
  }

  void CChaveAlinhamento::Serialize(CArchive& ar)
  {
    if (ar.IsStoring())
    {
      ar << EstacaInicial << Afastamento << strDescricao;
    }
    else
    {
      ar >> EstacaInicial >> Afastamento >> strDescricao;
    }
  }

  const std::string& PegaDescricao() const {return strDescricao;};
  double PegaAfastamento() const { return Afastamento; };
};

typedef std::list<Ponto>  typlstPontos;
typedef typlstPontos::iterator ittyplstPontos;
typedef typlstPontos::const_iterator cittyplstPontos;
typedef typlstPontos::const_reverse_iterator crittyplstPontos;

class CAlinhamento
{
  CChaveAlinhamento Chave;

  public:

    typlstPontos LstPontos;

    bool Ativo;

    CAlinhamento(double Est,double pAfst,std::string& Desc,int pTipo);
     
    CAlinhamento(): Ativo(false) {};
    ~CAlinhamento();
    void InserePonto(const Ponto& P) {LstPontos.push_back(P);}
    const CChaveAlinhamento& PegaChave() const {return Chave;} 

    const CAlinhamento& operator=(const CAlinhamento RHS)
    {
      Chave = RHS.Chave;
      LstPontos = RHS.LstPontos;

      return *this;
    }

    const CChaveAlinhamento& PegaChaveAlinhamento() const { return Chave; };

    void Serialize(CArchive& ar,int Tipo);

    std::tuple<Ponto,Ponto> PegaInicioFim() const
    {
      return std::make_tuple(*LstPontos.begin(), *LstPontos.rbegin());
    };
};
