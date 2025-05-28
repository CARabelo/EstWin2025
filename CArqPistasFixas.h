//--- no Da lista do arquivo de Pistas Fixas.

//-----------------------------------------------------------------
//--- classe lista das pistas fixas
	
class ArqPistasFixas
{
public:

  tlstwstr LstCampos;

  int Consiste();
};

//-----------------------------------------------------------------

class CArqPistasFixas : public CList<ArqPistasFixas, ArqPistasFixas&>
{
  POSITION Atual;
  void Serialize(CArchive& ar);

  int Erro;

public:

  CArqPistasFixas(CString& NomeArq);

protected:

  CString NomeArquivo;
};
