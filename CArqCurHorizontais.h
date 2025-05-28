//--- no Da lista do arquivo de curvas Horizontais.

class ArqCurHorizontais
{
public:

  tlstwstr  ListaCampos;
  int Erro;

  ArqCurHorizontais() {};
  ArqCurHorizontais(CString& PEstTs,int PIgTs,CString& PEstSt,int PIgSt,int PLado,int PTipo,double PAc,double PRaio,double PLc,double PSe, double PSl);
  ArqCurHorizontais(tlstwstr& pDados);


  int Consiste();

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  const ArqCurHorizontais& operator=(const ArqCurHorizontais& RHS);

};

//-----------------------------------------------------------------
//--- classe lista das curvas

class CArqCurHorizontais : public CList<ArqCurHorizontais,ArqCurHorizontais&> 
{
  POSITION Atual;
  void Serialize(CArchive& ar);
public:

  CArqCurHorizontais(CString& NomeArq);
  void GravarArquivo();
protected:

  CString NomeArquivo;
};

