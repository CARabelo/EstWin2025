//--- no Da lista do arquivo de curvas Horizontais.

class CRegSecoesAvulsas
{
  public:

  CRegSecoesAvulsas(CString* Dados);

  CRegSecoesAvulsas() {};

  SuperCString  Nome,XInicial,YInicial,XFinal,YFinal,Azimute,Distancia;

  int Consiste();

  CArchive& operator>>(CArchive& ar);
  CArchive& operator<<(CArchive& ar);

  int operator==(const CRegSecoesAvulsas RHS)
  {
    return Nome.Compare(RHS.Nome);
  }

  const CRegSecoesAvulsas& operator=(const CRegSecoesAvulsas& RHS);
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqSecoesAvulsas : public CList<CRegSecoesAvulsas,CRegSecoesAvulsas&> 
{
  POSITION Atual;

public:

  CArqSecoesAvulsas(CString& NomeArq);
  void Serialize(CArchive& ar);
  bool RemoveReg(const CString& Estaca);

protected:

  CString NomeArquivo;
};
