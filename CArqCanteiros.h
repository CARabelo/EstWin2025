//--- no Da lista do arquivo de curvas Horizontais.

class ArqCanteiros
{
public:

  tlstwstr LstCampos;

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  ArqCanteiros(tlstwstr& LstDados);
  ArqCanteiros() {};

  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqCanteiros : public CList<ArqCanteiros,ArqCanteiros&>
{
  void Serialize(CArchive& ar);
  POSITION Atual;

public:

  CArqCanteiros(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;

  int Consiste();
};
