//------------------------------------------------------------

class SecTipo : public CLPontos
{
public:

  double SE;            //--- contém o valor da super elevação na estaca.
 //   iTangente;     //--- contém o valor do i na tangente.

  CEstaca EstFinal;

  SecTipo();
  SecTipo(SecTipo& SecaoTip);
  const SecTipo& operator=(const SecTipo& No);

  void IgualPerfil(const Perfil& Perf);

  void InsereGradiente(double xGradiente)
  {
    if(PosicionaPontoX(xGradiente,0.00005)) GetAt(Atual).Descricao = "GRADIE";
    else InsereOrdenada(Ponto(xGradiente,Calculay(xGradiente,CLPontos::TIPO_DELTA_H),0.0),false,true);
  }
};

//------------------------------------------------------------

class CSecTipo : public CArqSecTipo 
{
  CList<SecTipo,SecTipo&> LSecTipo;
  POSITION Atual;  //--- Ponteiro para o Atual da lista. 
  int Erro;

public:
   
  CSecTipo() {};

  CSecTipo(CString& pNomeArquivo);          //--- Construtor
  bool Constroi(std::string& NomeArquivo);   //--- Construtor atrasado
  void InsOrdenada(SecTipo &No); //--- Insersão ordenada na lista.
  bool BuscaSecTipo(CEstaca& Estaca,SecTipo& SecaoABuscar);
  bool BuscaLargurasPistas(const CEstaca& Estaca,double& LargEsq,double &LargDir);
  bool Consiste();
  int VerfErro() { return Erro;};
  bool EstMudancaSecTipo(double Estaca)
  {
    return PosicionaNoMaisProximo(CEstaca(Estaca)) == 0;
  }

protected:

  int Compara(class SecTipo &No1,class SecTipo &No2);  //--- compara dois nós.
  //--- Posiciona o ponteiro da lista na seção igual ou mais 
  //--- proxima no sentido indicado
  int PosicionaNoMaisProximo(const CEstaca& Estaca,int Direcao = FALSE); 

  CString NomeArquivo;           //--- Nome do arquivo.
};
