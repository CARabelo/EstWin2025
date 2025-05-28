//------------------------------------------------------------
#define TODOS -1

typedef multiset<Ponto&,Ponto::compara(const Ponto&)>::iterator itmsetPontos;

class CmsetPontos : public multiset<Ponto&,Ponto::compara(const Ponto&)>
{
  itmsetPontos Atual;     //--- Iterador para o atual da lista.
  
  //--- rotinas de buscas no set
  
  itmsetPontos PegaMaiorIgual(itmsetPontos it) { return lower_bound(it);};
  itmsetPontos PegaMenor(itmsetPontos it) { return (lower_bound(it))-- ;};
  itmsetPontos PegaMaior(itmsetPontos it) { return upper_bound(it);};
  itmsetPontos PegaMaiorIgual(itmsetPontos it) { itmserPontos pit(find(it)) ; pit != end() ? pit : upper_bound(it);};
  
  public:

  //--- Iteracão.
                
  CLPontos();     
  
  inline const Ponto& PegaPontoAtual(){ return *Atual;}
  
  const Ponto& PegaPontoAtual();
  double BuscaMenorRampa();
  
  int Compara(const Ponto &No1,const Ponto &No2)
  { return No1.x < No2.x ? MENOR :  No1.x> No2.x ? MAIOR : IGUAL;};
  
  void Destroi();

  //--- Inclusão.

  void InsereOrdenada(Ponto& No,int CALCULARRAMPAS = FALSE);
  void InsereLista(CmsetPontos& ListaAInserir);
  void InsereListaLado(CmsetPontos& ListaAInserir, int Lado);

  //--- Cálculos.

  static BOOL CalculaIntersecao(Ponto& NoProjAnte,Ponto& NoProjPost,Ponto& NoTerrAnte,Ponto& NoTerrPost,Ponto& Intersecao);
  double Calculay(double x);
  void CalculaIs();
  double CalculaRampa(CmsetPontos PAtual = end());

  //--- Remoção.

  void RemoveTudo();
  BOOL ExcluiPonto(double x);
  BOOL ExcluiPonto(itmsetPontos PontoAExcluir = end());
  void TrimRight(double Afast);
  void TrimLeft(double Afast);

  //--- Posicionamento.

  int PosicionaNoMaisProximo(double x);
  POSITION PosicionaPonto(double x,double y,double Toleranciax=1e-5,double Toleranciay=1e-5);
  BOOL PosicionaCabeca();
  BOOL PosicionaPe();

  //--- Alteração.

  void AlteraPonto(Ponto& PNovo,itmsetPontos Pos = end());  //--- Altera as coordenadas de um determinado ponto.
  void AlteraRampas(double Rampa,int LADO = TODOS,itmsetPontos Pos = end());

  //--- Serializaçao.

  void Serialize(CArchive& ar);

    //--- Operadores.

  const CmsetPontos operator=(const CmsetPontos& No);
  BOOL operator++();    
  BOOL operator--();
};
