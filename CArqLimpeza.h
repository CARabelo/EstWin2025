//--- no Da lista do arquivo de curvas Horizontais.

class ArqLimpeza
{
public:

  tlstwstr ListaCampos;

  ArqLimpeza(tlstwstr& ListaCampos);
  ArqLimpeza() {};

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqLimpeza : public CList<ArqLimpeza,ArqLimpeza&>
{
  void Serialize(CArchive& ar);

  POSITION Atual;

public:

  CArqLimpeza(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
