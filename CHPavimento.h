//------------------------------------------------------------

class HPavimento
{
public:

  CEstaca EstFinal;
  double h;
  double Exesso;             //--- usado apenas na limpeza

  const HPavimento &operator=(const HPavimento &No);
};

//------------------------------------------------------------

class CHPavimento : public CArqHPavimento
{
  CList<HPavimento,HPavimento&> LHPavimento;
  POSITION Atual;      //--- Ponteiro para o Atual da lista. 

  int PosicionaNoMaisProximo(const CEstaca& Estaca,int Direcao = FALSE); //--- Posiciona numa curva igual ou prox.

public:

  CHPavimento(CString& NomeArquivo,CString& ext=CString(".pav"));       //--- Construtor
  void InsereOrdenada(class HPavimento &No,int INSERIRIGUAL = FALSE);  //--- Insersão ordenada na lista.
  double BuscaH(const CEstaca& Estaca,double* Exesso=NULL);     			 //--- Retorna o valor de h para uma dete. estaca.
  bool Consiste();

protected:

  CString NomeArquivo,Ext;           //--- Nome do arquivo.

public:

  int Compara(class HPavimento &No1,class HPavimento &No2);  //--- compara dois nós.

};
