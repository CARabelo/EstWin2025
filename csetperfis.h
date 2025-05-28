typedef set<Perfil&,const Perfil::compara(const Perfil& Perf)> itsetPerfis;

class CSetPerfis : public set<Perfil&,const Perfil::compara(const Perfil& Perf)> 
{
  itsetPerfil Atual;      //--- Ponteiro para o Atual do set. 

  //--- Posiciona o ponteiro da lista na seção igual ou mais 
  //--- proxima no sentido indicado
  
  int PosicionaNoMaisProximo(CEstaca& Estaca); 
        
  CString NomeArquivo;           //--- Nome do arquivo dos perfis.
        
  protected:
               
   //--- rotinas de buscas no set

   itsetPerfis PegaMaiorIgual(Perfil& Perf) { return lower_bound(Perf);};
   itsetPerfis PegaMenor(Perfil& Perf) { itsetPerfis pit(find(Perf)) ; return pit != end() ? pit-- : pit; };
   itsetPerfis PegaMaior(Perfil& Perf) { itsetPerfis pit(find(Perf)) ; return  pit != end() ? pit++ : pit;};
   itsetPerfis PegaMenorIgual(Perfil& Perf) { itsetPerfis pit(find(Perf)) ; return pit != end() pit ? pit--;};

  public:

  void Constroi(CString& NomeArq);   //--- construtor atrasado.
  CsetPerfis(CString& NomeArquivo);    //--- Construtor
  CsetPerfis();
 
  ~CSetPerfis();                       
 
  void InsereOrdenada(class Perfil &No,int INSERIRIGUAL){ insert(No);};

  int Compara(class Perfil &No1,class Perfil &No2)
  {
    //--- Compara 2 nós da lista.

    return No1.Estaca.EstVirtual < No2.Estaca.EstVirtual) ? MENOR :
           No1.Estaca.EstVirtual > No2.Estaca.EstVirtual) ? MAIOR : IGUAL;
  };

  double PegaPontoNotavel(CEstaca &Anterior);
 
  int BuscaPerfil(CEstaca& Estaca,Perfil& PerfilABuscar);
  void Serialize(CArchive& ar);
  void GravarArquivo(CString &NomeProjeto);
  void CalculaIs();

  BOOL CSetPerfis::operator++() { return (Atual++) != end();};
  BOOL CSetPerfis::operator--() { return (Atual--) != end();};
};
