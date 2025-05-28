//--- no Da lista do arquivo de curvas Horizontais.

class ArqTaludesPavim
{
public:

  tlstwstr ListaCampos;

  ArqTaludesPavim() {};
  ArqTaludesPavim(tlstwstr& Dados);

  int Consiste();

  void Serialize(CArchive &ar);

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqTaludesPavim : public CList<ArqTaludesPavim,ArqTaludesPavim&>
{
  void Serialize(CArchive& ar);
  POSITION Atual;

public:

  CArqTaludesPavim(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
