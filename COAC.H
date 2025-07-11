//------------------------------------------------------------

class OAC
{
public:

  CEstaca Estaca;
  CString Descricao;

  const class OAC &operator=(const class OAC &No);
};

//------------------------------------------------------------

class COAC : public CArqOAC
{
public:

  COAC(const CString& NomeArquivo = CString(""));       //--- Construtor
  void InsOrdenada(class OAC &No); //--- Insers�o ordenada na lista.
  void PegaEstacasObrigatorias(CList<CEstaca,CEstaca&>& EstObrig);
  bool Consiste();

protected:

  int Compara(class OAC &No1,class OAC &No2);  //--- compara dois n�s.
  //--- Posiciona o ponteiro da lista na se��o igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(CEstaca Estaca,int Direcao = FALSE); 

  POSITION Atual;      //--- Ponteiro para o Atual da lista. 
  OAC NoAtual;         //--- Contem os dados do n� atual.        

  public:

  CList<OAC,OAC&> LOAC;
  POSITION PosicionaEstaca(double x,double y,double Toleranciax,double Toleranciay,int Tipo = 1);

  bool GravarArquivo(); 
};
  