class CFuroSondagem 
{
  CEstaca Estaca;
  tysetCTipoSoloSondagem SetSolos;
  double CotaFundo;
  
public:
  
  CFuroSondagem(const CEstaca& Est=CEstaca(0.0)):Estaca(Est),CotaFundo(INFINITO){};
  ~CFuroSondagem(void){};
  const CEstaca& PegaEstaca() const { return Estaca;};
  const tysetCTipoSoloSondagem& PegaSetSolos()  const { return SetSolos;};
  void Alteracota(double NovaCota) {Estaca.Cota = NovaCota;};
  bool IncluiTipoSolo(CTipoSoloSondagem& TS) {return (SetSolos.insert(TS)).second;};
  ittysetCTipoSoloSondagem PegaTipoSoloMaisProximo(double Cota,double Tolerancia) const;
  void AlteraCotaFundo(double Profundidade) { if(Estaca.Cota != INFINITO) CotaFundo = Estaca.Cota - Profundidade;};
  double PegaCotaFundo() const {return CotaFundo;};

  bool operator<(const CFuroSondagem& RHS) const {return Estaca.EstVirtual < RHS.Estaca.EstVirtual;}

 CArchive& operator>>(CArchive& ar) const 
    {
      (const_cast<CEstaca*>(&Estaca))->operator>>(ar);

      ar << (size_t) SetSolos.size(); 

      for (ittysetCTipoSoloSondagem it = SetSolos.begin() ; it != SetSolos.end() ; ++it)
      {
        *(const_cast<CTipoSoloSondagem*>(&*it)) >> ar;
      }
    
      return ar;
    };

  CArchive& operator<<(CArchive& ar) 
  {
    Estaca.operator<<(ar);

     CTipoSoloSondagem TSS;

     size_t TamSet(0);

     for( ar >> TamSet ; TamSet > 0 ; --TamSet)
     {
       TSS << ar;
       SetSolos.insert(TSS);
     }
     return ar;
  };
};

typedef std::set<CFuroSondagem> tysetFurosSondagem;
typedef tysetFurosSondagem::iterator ittysetFurosSondagem;
typedef tysetFurosSondagem::const_iterator itctysetFurosSondagem;
typedef std::pair<ittysetFurosSondagem,bool> paitysetFurosSondagem;
typedef std::pair<ittysetFurosSondagem,ittysetFurosSondagem> pairERSFS;
