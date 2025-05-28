//--- no Da lista do arquivo de curvas Horizontais.

class ArqSecTipo
{
public:

  tlstwstr LstCampos;

  int Consiste();
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqSecTipo : public CList<ArqSecTipo,ArqSecTipo&> 
{
  POSITION Atual;
  void Serialize(CArchive& ar);

  int Erro;

public:

  CArqSecTipo() {};
  CArqSecTipo(CString& NomeArq);

  bool Constroi(std::string& NomeArq);

protected:

  CString NomeArquivo;
};
