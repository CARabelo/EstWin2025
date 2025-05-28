//------------------------------------------------------------------
//--- no Da lista do arquivo de curvas verticais.

class ArqCurVerticais
{
public:

  tlstwstr ListaCampos;

  ArqCurVerticais(CString& PPIV,CString& PIg,CString& PCota,CString& Y1,CString& Y2);

  ArqCurVerticais(tlstwstr& Dados);
  ArqCurVerticais(){};

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das curvas

class CArqCurVerticais : public CList<ArqCurVerticais,ArqCurVerticais&>
{
  POSITION Atual;

  void Serialize(CArchive& ar);

public:

  CArqCurVerticais(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;

  int Consiste();

};
