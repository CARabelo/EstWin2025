typedef std::set<std::string> tysetstring;
typedef tysetstring::iterator ittysetstring;

//------------------------------------------------------------------
//--- no Da lista do arquivo de curvas verticais.

class ArqAciTopograficos
{
//   tysetstring SetChaves;

public:

  tlstwstr ListaCampos;

  ArqAciTopograficos(CString& PIcone,CString& PCodigo,CString& PDescCurta,CString& PDescLonga,CString& PForcarAresta);
  ArqAciTopograficos(CString& PCodigo)          //--- Temporário para find do set;
  {
    ListaCampos.push_back("");
    ListaCampos.push_back(PCodigo.GetBuffer());
  }

  ArqAciTopograficos(tlstwstr& Dados);
  ArqAciTopograficos(){};

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  bool operator<(const ArqAciTopograficos& RHS) const 
  {
    if(ListaCampos.size() < 2 || RHS.ListaCampos.size() < 2) return false;

    cittlstwstr it1(ListaCampos.begin()),it2(RHS.ListaCampos.begin());
    ++it1,++it2;

    return *it1 < *it2;
  }

  const std::string PegaCodigo() const
  {
    if(ListaCampos.size() > 1)
      return *(++(ListaCampos.begin()));

    return "";
  }


  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das curvas

class CArqAciTopograficos : public CList<ArqAciTopograficos,ArqAciTopograficos&>
{
  POSITION Atual;

  void Serialize(CArchive& ar);

public:

  CArqAciTopograficos(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;

  int Consiste();

};
