//--- no Da lista do arquivo de curvas Horizontais.

class ArqIgualdade
{
public:

  SuperCString  Estaca1,
    Estaca2;

  ArqIgualdade() {};
  ArqIgualdade(CString* Dados) { Estaca1 = Dados ; Estaca2 = (Dados+1) ; } ;

  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqIgualdade	: public CList<ArqIgualdade,ArqIgualdade&>
{
  POSITION Atual;
  void Serialize(CArchive& ar);

public:

  CArqIgualdade(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
