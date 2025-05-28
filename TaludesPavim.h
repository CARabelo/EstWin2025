#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

//------------------------------------------------------------

class TaludesPavim 
{
public:

  CEstaca EstFinal;
  double Rampas[4];

  TaludesPavim();  //--- Construtor default
  TaludesPavim(class TaludesPavim &Alarg);  //--- Construtor de c�pia;
  const TaludesPavim &operator=(const TaludesPavim &No);
};

//------------------------------------------------------------

class CTaludesPavim : public CArqTaludesPavim   
{
  POSITION Atual;      //--- Ponteiro para o Atual da lista. 
  CString NomeArquivo;           //--- Nome do arquivo.
  CList<TaludesPavim,TaludesPavim&>  LTaludesPavim;

protected:

  int Compara(class TaludesPavim &No1,class TaludesPavim &No2);  //--- compara dois n�s.
  //--- Posiciona o ponteiro da lista na se��o igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(CEstaca Estaca,int Direcao = FALSE); 

public:

  CTaludesPavim(CString& NomeArquivo);       //--- Construtor
  void InsOrdenada(class TaludesPavim &No,int INSERIRIGUAL = FALSE); //--- Insers�o ordenada na lista.
  //	TaludesPavim& operator=(class CTaludesPavim &No);
  BOOL BuscaTaludesPavim(CEstaca& Estaca,TaludesPavim& TaludesPavimento);
  void TaludesDefault();
  bool Consiste();
};
