//--- no Da lista do arquivo de curvas Horizontais.

class ArqTerreno
{
public:

  CString Tipo;
  double  Ponto[2];

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqTerreno : public CList<ArqTerreno,ArqTerreno&>
{
  POSITION Atual;
  void Serialize(CArchive& ar);

public:

  CArqTerreno(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
