//--- no Da lista do arquivo de curvas Horizontais.

class ArqHPavimento
{
public:

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  tlstwstr ListaCampos;

  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqHPavimento : public CList<ArqHPavimento,ArqHPavimento&>
{
  void Serialize(CArchive& ar);

  POSITION Atual;

public:

  CArqHPavimento(CString& NomeArq,CString& Ext);
  void GravarArquivo();


protected:

  CString NomeArquivo;
};
