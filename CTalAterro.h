//------------------------------------------------------------

class TaludeAterro : public CLPontos 
{
public:

  CEstaca EstFinal;
  CString Lado;

  TaludeAterro();  //--- Construtor default
  TaludeAterro(TaludeAterro &Alarg);  //--- Construtor de c�pia;
  class TaludeAterro &operator=(TaludeAterro &No);
};

//------------------------------------------------------------

class CTaludesAterro : public CArqTalAterro   
{
  POSITION Atual[2];      //--- Ponteiro para o Atual da lista. 
  CString NomeArquivo;           //--- Nome do arquivo para Serializar.
  CList<TaludeAterro,TaludeAterro&>  LTaludesAterro[2];
  //--- Posiciona o ponteiro da lista na se��o igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(CEstaca Estaca,int Lado,int Direcao = FALSE); 

public:

  CTaludesAterro(CString& NomeArq);       //--- Construtor
  void InsOrdenada(TaludeAterro &No,int INSERIRIGUAL = FALSE); //--- Insers�o ordenada na lista.
  BOOL BuscaTalude(CEstaca Estaca,int Lado,CLPontos& PontosAInserir);
  int Compara(TaludeAterro &No1,TaludeAterro &No2);  //--- compara dois n�s.
  void TaludesDefault();
  bool Consiste();                      //--- consiste a digita��o
  int CTaludesAterro::ConsisteDados();   //--- consiste os pontos do talude
};
