//------------------------------------------------------------

class TaludeCorte : public CLPontos 
{
public:

  CEstaca EstFinal;
  CString Lado;

  TaludeCorte();  //--- Construtor default
  TaludeCorte(TaludeCorte &Alarg);  //--- Construtor de cópia;
  TaludeCorte &operator=(TaludeCorte &No);
};

//------------------------------------------------------------

class CTaludesCorte : public CArqTalCorte   
{
  POSITION Atual[2];      //--- Ponteiro para o Atual da lista. 
  CString NomeArquivo;           //--- Nome do arquivo para Serializar.
  CList<TaludeCorte,TaludeCorte&>  LTaludesCorte[2];
  //--- Posiciona o ponteiro da lista na seção igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(CEstaca Estaca,int Lado,int Direcao = FALSE); 

public:

  CTaludesCorte(CString& NomeArq);       //--- Construtor
  void InsOrdenada(class TaludeCorte &No,int INSERIRIGUAL = FALSE); //--- Insersão ordenada na lista.
  BOOL BuscaTalude(CEstaca &Estaca,int Lado,CLPontos &PontosAlarg);
  int Compara(TaludeCorte &No1,TaludeCorte &No2);  //--- compara dois nós.
  void TaludesDefault();
  bool Consiste();
  int ConsisteDados();
};
