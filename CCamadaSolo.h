class CCamadaSolo : public CPerfilSolo
{
  int Camada;

  public:

    CCamadaSolo(int Cam,const std::string& strTipSol,const Perfil& Perf):CPerfilSolo(strTipSol,Perf),Camada(Cam){};
    ~CCamadaSolo(void){};

    bool operator<(const CCamadaSolo& RHS) const 
    {
      return Camada < RHS.Camada ? true :
             Camada > RHS.Camada ? false : 
             ((CPerfilSolo*)(this))->operator<(RHS);
    }; 
};

typedef std::set<CCamadaSolo> tysetCCamadaSolo;
typedef tysetCCamadaSolo::iterator ittysetCCamadaSolo;
