class CSecaoAvulsa:public CLPontos
{
  public:
	
    enum TIPO_SECAO_AVULSA{SECAO_COMUM,SECAO_AVULSA,PERFIL_BUEIRO};

  std::string Nome;

  int Tipo;

  Ponto PInicial, PFinal;
  double Azimute,Distancia;
  CLPontos AcidentesTopograficos,AlinhamentosTerrap;
  CLPontos LstPontosProjeto;

  CSecaoAvulsa() {};

  CSecaoAvulsa::CSecaoAvulsa(const CSecaoAvulsa& SecAv) : Nome(SecAv.Nome), PInicial(SecAv.PInicial), PFinal(SecAv.PFinal), Azimute(SecAv.Azimute), Distancia(SecAv.Distancia),Tipo(SecAv.Tipo)
  {
  }
  
  CSecaoAvulsa::CSecaoAvulsa(const std::string& pNome,double pXIni,double pYIni,double pXFim,const double pYFim,const double pAzimute,double pDistancia,int pTipo=SECAO_COMUM) : Nome(pNome),PInicial(pXIni,pYIni),PFinal(pXFim,pYFim),Azimute(pAzimute),Distancia(pDistancia),Tipo(pTipo)
  {
  }

  CSecaoAvulsa::CSecaoAvulsa(const std::string& pNome,const Ponto& PIni,const Ponto& PFim, const double pAzimute, double pDistancia,int pTipo = SECAO_COMUM) : Nome(pNome), PInicial(PIni), PFinal(PFim), Azimute(pAzimute), Distancia(pDistancia),Tipo(pTipo)
  {
  }

  CSecaoAvulsa(const std::string& pNome, const std::string& XIni, const std::string& YIni, const std::string& XFim, const std::string& YFim, const std::string& pAzimute,const std::string& pDistancia,int pTipo=SECAO_COMUM);

  CSecaoAvulsa& operator=(CSecaoAvulsa& SecAv);
  CSecaoAvulsa(const std::string& pNome) : Nome(pNome),Azimute(0.0),Distancia(0.0),Tipo(SECAO_COMUM) {};

  CArchive& operator<<(CArchive& ar);
  CArchive& operator>>(CArchive& ar);

  int Consiste() {};
  void RotacionaPlano(double Sin,double Cos,double Angulo);
  void Serialize(CArchive& ar);

  bool operator<(const CSecaoAvulsa& RHS) const { return Nome < RHS.Nome;};

  void AcertaOffSets();
};

