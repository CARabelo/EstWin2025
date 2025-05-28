//------------------------------------------------------------

class PistasFixas : public CLPontos
{
public:

  CEstaca EstFinal;

  PistasFixas();
  PistasFixas(PistasFixas& PistaFix);
  const PistasFixas& operator=(const PistasFixas& No);
};

//------------------------------------------------------------

class CPistasFixas : public CArqPistasFixas
{
  CList<PistasFixas, PistasFixas&> LPistasFixas;
  POSITION Atual;  //--- Ponteiro para o Atual da lista. 
  int Erro;

public:

  enum{INICIO,FIM,MEIO};

  CPistasFixas(CString& NomeArquivo);       
  void InsOrdenada(PistasFixas &No); //--- Insersão ordenada na lista.
//  bool BuscaPistasFixas(CEstaca& Estaca, PistasFixas& SecaoABuscar);
  bool VerfPistaFixas(const CEstaca& Estaca,double x,int& Ini_Fim);

  bool Consiste();
  int VerfErro() { return Erro; };

protected:

  int Compara(class PistasFixas &No1, class PistasFixas &No2);  //--- compara dois nós.
                                                                //--- Posiciona o ponteiro da lista na seção igual ou mais 
                                                                //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(const CEstaca& Estaca, int Direcao = FALSE);

  CString NomeArquivo;       
};
