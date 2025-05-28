//--- no Da lista do arquivo de curvas Horizontais.

class ArqTerrPontos
{
public:

  SuperCString  EstFinal,
    Ig,
    Pontos[6];
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqTerrPontos : public CList<ArqTerrPontos,ArqTerrPontos&> 
{
  void Serialize(CFile &Arquivo,int Gravar);
  POSITION Atual;

public:

  CArqTerrPontos(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
