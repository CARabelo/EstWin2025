//------------------------------------------------------------
#define TODOS -1

typedef set<Ponto>::iterator itSetPontos;

class CSetPontos : public set<Ponto>
{
  itSetPontos Atual;     //--- Iterador para o atual da lista.
  
  //--- rotinas de buscas no set    

  itSetPontos PegaMaiorIgual(Ponto& Pon) { return lower_bound(Pon);};
  itSetPontos PegaMenor(Ponto& Pon) { itSetPontos pit(find(Pon)) ; return pit != end() ? pit-- : pit; };
  itSetPontos PegaMaior(Ponto& Pon) { itSetPontos pit(find(Pon)) ; return  pit != end() ? pit++ : pit;};
  itSetPontos PegaMenorIgual(Ponto& Pon) { itSetPontos pit(find(Pon)) ; return
      pit != end() ? pit : pit--;};

  public:

  //--- Iteracão.
                
  CSetPontos();     
  
  inline Ponto& PegaPontoAtual(){ return *Atual;}
  
  double BuscaMenorRampa();
  
  int Compara(Ponto &No1,Ponto &No2)
  { return No1.x < No2.x ? -1 :  No1.x> No2.x ? 1 : 0;};
 
  //--- Inclusão.

  void InsereOrdenada(Ponto& No,int CALCULARRAMPAS = false);
  void InsereLista(CSetPontos& ListaAInserir);
  void InsereListaLado(CSetPontos& ListaAInserir, int Lado);

  //--- Cálculos.

  static bool CalculaIntersecao(Ponto& NoProjAnte,Ponto& NoProjPost,Ponto& NoTerrAnte,Ponto& NoTerrPost,Ponto& Intersecao);
  double Calculay(double x);
  void CalculaIs();
  double CalculaRampa(itSetPontos PAtual);

  //--- Remoção.

  void RemoveTudo();
  bool ExcluiPonto(double x);
  bool ExcluiPonto(itSetPontos PontoAExcluir/* = end()*/);
  void TrimRight(double Afast);
  void TrimLeft(double Afast);

  //--- Posicionamento.

  int PosicionaNoMaisProximo(double x);
  itSetPontos PosicionaPonto(double x,double y,double Toleranciax=1e-5,double Toleranciay=1e-5);
  
  bool PosicionaCabeca() { return (Atual = this->begin()) != this->end();};
  bool PosicionaPe() { return (Atual = lower_bound(*this->end())) != this->end();};
  
  //--- Alteração.

  void AlteraPonto(Ponto& PNovo,itSetPontos Pos);  //--- Altera as coordenadas de um determinado ponto.
  void AlteraRampas(double Rampa,int LADO,itSetPontos Pos);

  //--- Serializaçao.

  void Serialize(fstream& ar);

  //--- Operadores.

  CSetPontos& operator=(CSetPontos& No);
  
  bool operator--(){ return (Atual--) != end();};
  bool operator++(){ return (Atual++) != end();};
  
};
