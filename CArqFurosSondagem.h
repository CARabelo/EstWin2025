//--- no Da lista do arquivo de curvas Horizontais.

class ArqFurosSondagem
{
public:

  tlstwstr ListaCampos;

  ArqFurosSondagem() {};
  ArqFurosSondagem(tlstwstr& pLstDados);

  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqFurosSondagem : public CList<ArqFurosSondagem,ArqFurosSondagem&> 
{
  POSITION Atual;

public:

  CArqFurosSondagem(const std::string& NomeArq = "");
  void GravarArquivo();
  void Serialize(CArchive& ar);

protected:

  std::string NomeArquivo;
};
