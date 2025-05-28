class CLPerfis : public	CList<Perfil,Perfil&>
{
  POSITION Atual;      //--- Ponteiro para o Atual da lista. 

  CString NomeArquivo;           //--- Nome do arquivo.

public:

  POSITION PegaAtual() { return Atual;}; 

  //--- Posiciona o ponteiro da lista na seção igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(CEstaca& Estaca,int Direcao = FALSE); 

  void Constroi(CString& NomeArq);   //--- construtor atrasado.
  CLPerfis(CString& NomeArquivo);    //--- Construtor
  CLPerfis();
  ~CLPerfis();                       
  void InsereOrdenada(class Perfil &No,int INSERIRIGUAL = FALSE); //--- Insersão ordenada na lista.
  int Compara(class Perfil &No1,class Perfil &No2);               //--- compara dois nós.
  void Serialize(CArchive& ar,int pEquidis=0);
  void GravarArquivo(CString &NomeProjeto);
  void CalculaIs();
  int BuscaPerfil(CEstaca& Estaca,Perfil& rPerfil);

  void CLPerfis::Limpa() 
  {
    RemoveAll();   //--- Remove todos os elementos da lista e limpa o atual
    Atual = NULL;  
  };

  BOOL operator++();
  BOOL operator--();
};
