//------------------------------------------------------------

class RegAciTopografico : public CLPontos 
{
  int Erro;
public:

  int Icone;
  std::string DescricaoCurta,DescricaoLonga;
  bool ForcarAresta;

  RegAciTopografico();  //--- Construtor default
  RegAciTopografico(class RegAciTopografico &AciTopog);  //--- Construtor de c�pia;
  class RegAciTopografico &operator=(class RegAciTopografico &No);
};

//------------------------------------------------------------

class CAciTopografico : public CArqAciTopograficos   
{
  POSITION Atual;      //--- Ponteiro para o Atual da lista por lado. 
  CString NomeArquivo;           //--- Nome do arquivo.
  CList<RegAciTopografico,RegAciTopografico&>  LRegAciTopografico;
  int Erro;

protected:

  int Compara(class RegAciTopografico &No1,class RegAciTopografico &No2);  //--- compara dois n�s.
  //--- Posiciona o ponteiro da lista na se��o igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(int Icone,int Direcao = FALSE); 

public:

  CAciTopografico(CString& NomeArquivo);       //--- Construtor
  void InsereOrdenada(class RegAciTopografico &No,int INSERIRIGUAL = FALSE); //--- Insers�o ordenada na lista.
  BOOL BuscaRegAciTopografico(int Icone,CLPontos &PontosAciTopog);
  bool Consiste();
  int PegaErro() { return Erro;};
};
