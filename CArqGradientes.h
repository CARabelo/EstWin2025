//--- no Da lista do arquivo de curvas Horizontais.

class ArqGradiente
{
public:

  tlstwstr ListaCampos;

  ArqGradiente(tlstwstr& pLstDados);
  ArqGradiente() {};

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  int Consiste(int TipoArq = 0);
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqGradiente : public CList<ArqGradiente,ArqGradiente&>
{
  void Serialize(CArchive& ar);

  POSITION Atual;

public:

  CArqGradiente(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
