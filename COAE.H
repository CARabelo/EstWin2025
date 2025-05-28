//------------------------------------------------------------

class OAE
{
public:

  CEstaca EstInicial,
          EstFinal;  
  CString Descricao;

  const class OAE &operator=(const class OAE &No);
};

//------------------------------------------------------------

class COAE : public CArqOAE
{
public:
  
  CList<OAE,OAE&> LOAE;

  COAE(const CString& NomeArquivo = CString(""));       //--- Construtor
  void InsOrdenada(class OAE &No); //--- Insersão ordenada na lista.
  void PegaEstacasObrigatorias(CList<CEstaca,CEstaca&>& EstObrig,double Equidis);
  bool Consiste();

protected:

  int Compara(class OAE &No1,class OAE &No2);  //--- compara dois nós.
  //--- Posiciona o ponteiro da lista na seção igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(CEstaca Estaca,int EstInicialFinal); 

  CString NomeArquivo;           //--- Nome do arquivo.
  POSITION Atual;      //--- Ponteiro para o Atual da lista. 
  OAE NoAtual;     //--- Contem os dados do nó atual.        

  public:

  POSITION PosicionaEstaca(double x,double y,double Toleranciax,double Toleranciay,int* InicioFim,int Tipo = 1);
};
