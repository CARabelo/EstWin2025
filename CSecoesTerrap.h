
typedef std::set<Ponto> tysetPonto;
typedef tysetPonto::iterator ittysetPonto;
typedef std::pair<ittysetPonto,bool> typairsetPonto;

class CSecaoTerrap
{
  CEstaca Estaca;
  Ponto CoordEixo;  
  
public:

  tysetPonto PontosTerrap;

  CSecaoTerrap(const CSecaoTerrap& SecTerrap) : Estaca(SecTerrap.Estaca), CoordEixo(SecTerrap.CoordEixo)
  {
    for (ittysetPonto It = SecTerrap.PontosTerrap.begin(); It != SecTerrap.PontosTerrap.end(); ++It)
    {
      PontosTerrap.emplace(*It);
    }
  }
  CSecaoTerrap(const CEstaca& pEstaca) : Estaca(pEstaca) {};

  bool operator<(const CSecaoTerrap& RHS) const { return Estaca < RHS.Estaca; }

  const CEstaca& PegaEstaca() const { return Estaca;};
  const Ponto& PegaCoordEixo() const { return CoordEixo;};
  void MudaCoordEixo(const Ponto& pCoordEixo){CoordEixo = pCoordEixo;};

  void Rotaciona(double VUx,double VUy,double Angulo)
  {
    tysetPonto SetPonAnte(PontosTerrap);
    PontosTerrap.clear();

    for (ittysetPonto It = SetPonAnte.begin(); It != SetPonAnte.end(); ++It)
    {
      PontosTerrap.emplace(It->RotacionaPlano(VUx,VUy,Angulo,true));
    }

    CoordEixo.RotacionaPlano(VUx,VUy,Angulo);
  }

  CArchive& operator<<(CArchive& ar);
  CArchive& operator>>(CArchive& ar);
};

typedef std::set<CSecaoTerrap> tysetSecoesTerrap;
typedef tysetSecoesTerrap::iterator ittysetSecoesTerrap;
typedef std::pair<ittysetSecoesTerrap, bool> typairsetSecoesTerrap;

typedef std::list<CSecaoTerrap> tylstSecoesTerrap;
typedef tysetSecoesTerrap::iterator ittylstSecoesTerrap;

