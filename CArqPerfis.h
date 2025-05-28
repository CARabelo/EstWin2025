class ArqPerfil
{
public:

  CString  Tipo;
  double   Ponto[2];
};

//-----------------------------------------------------------------
//--- classe lista das secoes

class CArqPerfil : public CList<ArqPerfil,ArqPerfil&> 
{
  POSITION Atual;

public:

  CArqPerfil(CString& NomeArq=(CString)(""));
  void GravaArquivo(CString& NomeArq);

protected:

  void Serialize(CArchive& ar);
  void LeArquivo(CString& NomeArq);

  CString NomeArquivo;
};
