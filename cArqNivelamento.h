//------------------------------------------------------------------
//--- no Da lista do arquivo de curvas verticais.

class ArqNivelamento
{
public:

  CVisada       Visada;
  CString				Ig,
    PlRef,
    Leitura,
    Cota; 
};

//-----------------------------------------------------------------
//--- classe lista das curvas

class CArqNivelamento : public CList<ArqNivelamento,ArqNivelamento&>
{
  POSITION Atual;
  void Serialize(CArchive &Arquivo);

public:

  CArqNivelamento(CString& NomeArq);
  void GravarArquivo();

protected:

  CString NomeArquivo;
};
