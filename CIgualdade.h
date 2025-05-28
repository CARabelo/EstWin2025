//------------------------------------------------------------

class Igualdade
{
public:

  CEstaca Estaca1,
    Estaca2;
  double  Diferenca,
    DifAcumulada;
  const class Igualdade &operator=(const class Igualdade &No);
};

//------------------------------------------------------------

class CIgualdade : public CArqIgualdade 
{
  CList<Igualdade,Igualdade&> LIgualdade;
  POSITION Atual;      //--- Ponteiro para o Atual da lista. 

public:

  CIgualdade(CString& NomeArquivo);       //--- Construtor
  void InsOrdenada(class Igualdade &No); //--- Insersão ordenada na lista.
  int PosicionaCurvaProxima(double Estaca,int Direcao); //--- Posiciona numa curva igual ou prox.
  bool Consiste();

protected:

  int Compara(class Igualdade &No1,class Igualdade &No2);  //--- compara dois nós.
  //--- Posiciona o ponteiro da lista na seção igual ou mais 
  //--- proxima no sentido indicado
  POSITION PosicionaSecaoProxima(double Estaca,int Direcao = FALSE); 
  CString NomeArquivo;           //--- Nome do arquivo.

  //	class Igualdade &operator=(class CIgualdade &No);

};
