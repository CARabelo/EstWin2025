//------------------------------------------------------------

class Canteiros : public CLPontos 
{
public:

  CEstaca EstFinal;
  CString Lado;

  Canteiros();  //--- Construtor default
  Canteiros(class Canteiros &Alarg);  //--- Construtor de c�pia;
  class Canteiros &operator=(class Canteiros &No);
};

//------------------------------------------------------------

class CCanteiros : public CArqCanteiros   
{
  POSITION Atual;                //--- Ponteiro para o Atual da lista. 
  CString NomeArquivo;           //--- Nome do arquivo.
  CList<Canteiros,Canteiros&>  LCanteiros;	

protected:

  int Compara(class Canteiros &No1,class Canteiros &No2);  //--- compara dois n�s.
  //--- Posiciona o ponteiro da lista na se��o igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(CEstaca& Estaca);


public:

  enum{INICIO,FIM,MEIO};

  CCanteiros(CString& NomeArquivo);       //--- Construtor
  void InsereOrdenada(class Canteiros &No,int INSERIRIGUAL = FALSE); //--- Insers�o ordenada na lista.
  BOOL BuscaCanteiros(CEstaca& Estaca,CLPontos &PontosCant);
  bool ECanteiro(CEstaca& Estaca,double Afast,int& Ini_Fim);
  bool Consiste();

};
