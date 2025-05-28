class CPerfilSolo
{
  std::string strTipoSolo;
  const Perfil& PerfilSolo;

  public:

    CPerfilSolo(const std::string& TipSolo,const Perfil& PerfSolo) : strTipoSolo(TipSolo),PerfilSolo(PerfSolo) {};
    ~CPerfilSolo(void) {};

    const Perfil& PegaPerfil() const { return PerfilSolo;}; 

    const bool operator<(const CPerfilSolo& RHS) const 
    {
      return strTipoSolo.compare(RHS.strTipoSolo) < 0;
    };
};

