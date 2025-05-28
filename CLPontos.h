//------------------------------------------------------------
#define TODOS -1

class CLPontos : public	CList<Ponto,Ponto&>
{
protected:

  POSITION Atual;     //--- Ponteiro para o atual da lista.

public:

  enum{TIPO_COTA,TIPO_DELTA_H};  //--- Tipo de calculo das cotas

  //--- Iteracão.

  CLPontos();   
  CLPontos(CLPontos& RHS) { Atual = NULL;};
  const Ponto& PegaPontoAtual() {	return GetAt(Atual);}
  Ponto* PegaPontoAnterior() 
  {
    if (Atual) GetPrev(Atual);
    if(Atual) return &GetAt(Atual);
    else return NULL;
  }

  POSITION PegaAtual() {return Atual;}

  double BuscaMenorRampa();
  int Compara(const Ponto &No1,const Ponto &No2);
  void Destroi();
  const Ponto& Interpola(double x,CString& Rotulo=CString(""));

  //--- Inclusão.

  void Insere(Ponto &No,int CALCULARRAMPAS = TRUE);
  void InsereOrdenada(const Ponto &No,int INSERIRIGUAL = FALSE,int CALCULARRAMPAS = FALSE);
  void InsereLista(CLPontos &ListaAInserir,int INSERIRIGUAL = FALSE);
  void InsereListaLado(CLPontos& ListaAInserir, int Lado,int INSERIRIGUAL= FALSE);

  //--- Cálculos.

  static BOOL CalculaIntersecao(const Ponto& PP1,const Ponto& PP2,const Ponto& PT1,const Ponto& PT2,Ponto& Intersecao,bool NaReta = true,double Erro = 1E-8);
  double Calculay(double x,int TIPO = TIPO_COTA);
  void CalculaIs();
  double CalculaRampa(POSITION PAtual = NULL);
  void AlteraFaixaY(const Ponto& P); 
  void LimpaTudo() { RemoveAll() ; Atual = NULL;};

  //--- Remoção.

  void RemoveTudo();
  void RemoveFaixa(double Inicio,double Fim);
  void RemoveFaixa2(double Inicio,double Fim);
  BOOL ExcluiPonto(double x);
  BOOL ExcluiPonto(POSITION PontoAExcluir = NULL);
  void ExcluiPontos(CString& Rotulo);
  void TrimRight(double Afast);
  void TrimLeft(double Afast);
  void RemovePontos(const tlstwstr& LstRotulos);
  void TrimLeftMenor(double Afast);
  void TrimRightMaior(double Afast);

  //--- Posicionamento.

  int PosicionaNoMaisProximo(double x);
  POSITION PosicionaPonto(double x,double y,double Toleranciax=1e-6,double Toleranciay=1e-6);
  POSITION PosicionaPontoX(double x,double Toleranciax=1e-6);
  BOOL PosicionaCabeca();
  BOOL PosicionaPe();

  //--- Alteração.

  void AlteraPonto(const Ponto& PNovo,POSITION Pos = NULL);  //--- Altera as coordenadas de um determinado ponto.
  void AlteraRampas(double Rampa,int LADO = TODOS,POSITION Pos = NULL);

  //--- Serializaçao.

  void Serialize(CArchive& ar);

  //--- Operadores.

  const CLPontos& operator=(const CLPontos& No);
  BOOL operator++();    
  BOOL operator--();
};

typedef std::list<Ponto> LPontos;
typedef LPontos::iterator ItLPontos;
typedef LPontos::const_iterator ItCLPontos;
typedef LPontos::reverse_iterator ItRLPontos;

typedef std::list<LPontos> LLPontos;
typedef LLPontos::iterator ItLLPontos;
typedef LLPontos::const_iterator ItCLLPontos;

typedef std::list<ItLPontos> LITLPontos;
typedef LITLPontos::iterator ItLITLPontos;
typedef LITLPontos::reverse_iterator RItLITLPontos;

struct ltCN
{
  bool operator()(const ItLLPontos& it1, const ItLLPontos& it2) const
  {
    return *it1->begin() < *it2->begin();
  }
};

typedef std::set<ItLLPontos, ltCN> tysetItLLPontos;
typedef tysetItLLPontos::iterator ittysetItLLPontos;

