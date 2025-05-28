//--- no Da lista do arquivo de curvas Horizontais.

class ArqAlargamento
{
public:

  tlstwstr LstCampos;

  ArqAlargamento(tlstwstr& LstCampos);

  ArqAlargamento() {};

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqAlargamento : public CList<ArqAlargamento,ArqAlargamento&>
{
  void Serialize(CArchive& ar);
  POSITION Atual;

public:

  CArqAlargamento(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
