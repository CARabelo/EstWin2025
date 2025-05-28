class CTipoSoloSondagem
{
  double Profundidade,Cota;
  std::string strTipoSolo;

public:
  CTipoSoloSondagem(double Prof=0.0,const std::string& StrTS="SemTipo",double pCota=INFINITO) : Profundidade(Prof),strTipoSolo(StrTS),Cota(pCota){};
  CTipoSoloSondagem(const std::string& Prof,const std::string& StrTS,double pCota=INFINITO) : Profundidade(atof(Prof.c_str())),strTipoSolo(StrTS),Cota(pCota){};
  void AlteraCota(double NovaCota) { Cota = NovaCota;};
  double PegaCota() const { return Cota;};
  double PegaProfundidade() const { return Profundidade;};
  const std::string& PegaTipoSolo() const { return strTipoSolo;};

  ~CTipoSoloSondagem(void){};

  bool operator<(const CTipoSoloSondagem& RHS) const { return Profundidade < RHS.Profundidade;};

  CArchive& operator>>(CArchive& ar) 
  {
    ar << Profundidade << Cota << strTipoSolo;
   
    return ar;
 };

  CArchive& operator<<(CArchive& ar) 
  {
    ar >> Profundidade >> Cota >> strTipoSolo;

    return ar;
  };
};

typedef std::set<CTipoSoloSondagem> tysetCTipoSoloSondagem; 
typedef tysetCTipoSoloSondagem::iterator ittysetCTipoSoloSondagem;
typedef tysetCTipoSoloSondagem::const_iterator itctysetCTipoSoloSondagem;
typedef std::pair<ittysetCTipoSoloSondagem,bool> partysetCTipoSoloSondagem;