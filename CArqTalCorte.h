//--- no Da lista do arquivo de curvas Horizontais.

class ArqTalCorte
{
public:

  tlstwstr ListaCampos;

  int Consiste();

  ArqTalCorte(tlstwstr& pLstCampos);
  ArqTalCorte() {};

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqTalCorte : public CList<ArqTalCorte,ArqTalCorte&>
{
  POSITION Atual;

  void Serialize(CArchive& ar);

public:

  CArqTalCorte(CString& NomeArq);
  void GravarArquivo();
 

protected:

  CString NomeArquivo;
};
