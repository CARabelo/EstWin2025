//--- no Da lista do arquivo de curvas Horizontais.

class ArqOAC
{
public:

  tlstwstr ListaCampos;

  ArqOAC() {};
  ArqOAC(tlstwstr& pLstCampos);

  int Consiste();

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqOAC : public CList<ArqOAC,ArqOAC&> 
{
  POSITION Atual;

public:

  CArqOAC(const CString& NomeArq = CString(""));
  void GravarArquivo();
  void Serialize(CArchive& ar);

protected:

  CString NomeArquivo;
};
