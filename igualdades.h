/////////////////////////////////////////////////////////////////////////////
//
// Igualdades.h : interface da classe CIgualdades.
//
/////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------
//--- Classe NoIgual.
//--- Esta classe representa o n� de uma lista de igualdades duplamente ligada, o 
//------------------------------------------------------------------------- 
class NoIgual
{
public:

  enum{ESTACA1,ESTACA2};  //--- Refer�ncias as estacas.    

  //--- Atributos  

  class NoIgual *Proximo,   //--- Ponteiro para o pr�ximo n�.
    *Anterior;  //--- Ponteiro para o n� anterior.
  class CEstaca *Estaca1;    //--- Primeira estaca da igualdade.
  double          Diferenca;  //--- Diferenca de igualdade do ponto.   

  //--- M�todos.

  NoIgual(double Estaca1,double Estaca2);
};

//---------------------------------------------------------------------------------
//--- Classe ListaIgual
//--- Esta classe representa uma lista ligada de igualdades.
//---------------------------------------------------------------------------------

class CIgualdades
{
protected:

  //--- M�todos

  void Inclui(double Est1,double Est2,class NoIgual *Anterior=NULL);   //--- Inclui uma igualdade na lista.
  void Exclui(class NoIgual *NoAExcluir);           //--- Exclui um no da lista
  BOOL Exclui(class NoIgual* Inicio,class NoIgual *Fim);  //--- Exclui uma faixa de registros.
  //--- para uma determinada igualdade.
  int Nitens;  //--- Numero de itens na lista.

  class NoIgual *operator++();   //--- Incrementador da lista.
  class NoIgual *operator--();   //--- Decrementador da lista.

public:
  //--- Propriedades

  class NoIgual *Primeiro,*Atual,*Ultimo;   //--- Ponteiros para a lista.      

  //--- M�todos.

  CIgualdades();                              //--- construtor.    
  ~CIgualdades();                             //--- Destrutor.

  void Criar(CString Nome_Projeto);   //--- Cria a lista de igualdades.
  double BuscaDifIgualdade(int Numero_Igualdade);  //--- Busca a soma das diferencas de igualdade
  double BuscaEstacaReal(double EstacaVirtual,int &NumIg);

};

