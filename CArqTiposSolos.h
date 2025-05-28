//--- no Da lista do arquivo de curvas Horizontais.

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqTiposSolos : public CList<CTipoSolo,CTipoSolo&> 
{
  POSITION Atual;

  tysetCTipoSolo SetTipoSolos;

public:

  CArqTiposSolos(const CString& NomeArq = CString(""));
  void GravarArquivo();
  void Serialize(CArchive& ar);
  void CriaSet(tysetCTipoSolo& SetCTipoSolos);

  void ConverteGDI(tysetCTipoSolo& SetCTipoSolos)
  {
    unsigned long TiposGDI[6]  =  {PS_SOLID,PS_SOLID,PS_DOT,PS_DASH,PS_DASHDOTDOT,PS_DASHDOT};

    for(itsetCTipoSolo it = SetCTipoSolos.begin() ; it != SetCTipoSolos.end() ; ++it)
    {
       CTipoSolo* pTS(const_cast<CTipoSolo*>(&*it));

       pTS->TracoGDI = TiposGDI[pTS->Traco];
    }
  }

protected:

  CString NomeArquivo;
};
