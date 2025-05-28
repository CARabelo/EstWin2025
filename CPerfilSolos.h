
typedef std::set<CTipoSolo> tysetTipoSolos;
typedef tysetTipoSolos::iterator ittysetTipoSolos;

class CPerfilSondagem
{ 
  CEstaca Estaca;

  tysetTipoSolos TiposSolos;

  public:

    CPerfilSondagem(void);
    ~CPerfilSondagem(void);

    void ConstroiPerfil(const CEstaca& Est,const CFuroSondagem& FuroSon)
    {
      Estaca = Est;
    }
};

