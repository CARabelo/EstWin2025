//--- no Da lista do arquivo de curvas Horizontais.

class ArqTalAterro
{
public:

  tlstwstr ListaCampos;

  int Consiste();

  ArqTalAterro(tlstwstr& pLstCampos);
  ArqTalAterro() {};

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqTalAterro : public CList<ArqTalAterro,ArqTalAterro&>
{
  POSITION Atual;
  void Serialize(CArchive& ar);

public:

  CArqTalAterro(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
