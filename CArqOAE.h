//--- no Da lista do arquivo de curvas Horizontais.

class ArqOAE
{
public:
  tlstwstr ListaCampos;

  ArqOAE() {} ;
  ArqOAE(tlstwstr& LstDados);

  const ArqOAE& operator=(const ArqOAE& RHS);

  int Consiste();

};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqOAE	:public CList<ArqOAE,ArqOAE&> 
{
public:

  CArqOAE(const CString& NomeArq = CString(""));
  void GravarArquivo();
  void Serialize(CArchive& ar);

protected:

  CString NomeArquivo;
  POSITION Atual;
};
