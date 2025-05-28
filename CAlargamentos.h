//------------------------------------------------------------

class Alargamento : public CLPontos 
{
public:

  CEstaca EstFinal;
  CString Lado;

  Alargamento();  //--- Construtor default
  Alargamento(class Alargamento &Alarg);  //--- Construtor de cópia;
  class Alargamento &operator=(class Alargamento &No);
};

//------------------------------------------------------------

class CAlargamento : public CArqAlargamento   
{
  POSITION Atual[2];      //--- Ponteiro para o Atual da lista por lado. 
  CString NomeArquivo;           //--- Nome do arquivo.
  CList<Alargamento,Alargamento&>  LAlargamento[2];	//--- 1 para cada lado
  int Erro;

protected:

  int Compara(class Alargamento &No1,class Alargamento &No2);  //--- compara dois nós.
  //--- Posiciona o ponteiro da lista na seção igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(CEstaca& Estaca,int Lado,int Direcao = FALSE); 

public:

  CAlargamento(CString& NomeArquivo);       //--- Construtor
  void InsereOrdenada(class Alargamento &No,int INSERIRIGUAL = FALSE); //--- Insersão ordenada na lista.
  BOOL BuscaAlargamento(CEstaca Estaca,int Lado,CLPontos& PontosAInserir);
  bool Consiste();
  int PegaErro() { return Erro;};

};
