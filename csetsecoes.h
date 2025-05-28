typedef set<CSecao&,const CSecao::compara(const CSecao& Sec)> ::iterator itsetSecoes;

class CSetSecoes : public set<CSecao&,const CSecao::compara(const CSecao& Sec)> 
{    
   enum{PROJETO,TERRENO,PROJETO_TERRENO,NENHUM};

   itsetSecoes Atual;        //--- Ponteiro para o Atual do mapa. 

   CString Projeto;           //--- Nome do projeto em uso.
   CLPerfis TerrenoOrfao;     //--- Lista de terreno sem projeto

   protected:

   int Compara(CSecao &No1,CSecao &No2){ return No1.Compara(No2);};

   //--- Posiciona o ponteiro da lista na seção igual ou mais 
   //--- proxima no sentido indicado
   
   int PosicionaNoMaisProximo(CEstaca& Estaca,int Direcao = FALSE); 

   //--- rotinas de buscas no set

   itsetSecoes PegaMaiorIgual(CSecao& Sec) { return lower_bound(Sec);};
   itsetSecoes PegaMenor(CSecao& Sec) { itsetPontos pit(find(Sec)) ; return pit != end() ? pit-- : pit; };
   itsetSecoes PegaMaior(CSecao& Sec) { itsetPontos pit(find(Sec)) ; return  pit != end() ? pit++ : pit;};
   itsetSecoes PegaMenorIgual(Csecao& Sec) { itsetPontos pit(find(Sec)) ; return pit != end() pit ? pit--;};

   public:      

   CTaludesCorte TaludesCorte;
   CTaludesAterro TaludesAterro;

   CListaSecoes(CString &NomeProjeto = (CString) "");

   void TaludesDefault();
   void DefRetangDes();        //--- Define as areas de desenhos (Usada nos desenhos).
   void CalculaIs();
   void CalculaIsLongitudinais();       //--- Calcula is longitudinais.
   void Serialize(const int Tipo);

   //--- Retorna um ponteiro para a estaca que estiver dentro da tolerância x e y.
   itsetSecoes PosicionaEstaca(double x,double y,double Toleranciax,double Toleranciay,int Tipo);  

   //--- Posicionadores públicos do mapa.

   itsetSecoes CMapaSecoes::PosicionaPrimeira(){ return Atual = Begin();
   itsetSecoes CMapaSecoes::PosicionaUltima(){ return Atual = end();}

   bool Posiciona(CEstaca& Estaca) { Atual = find(Estaca ; 
                                            return  Atual != end();}

   bool Posiciona(CEstaca& Estaca){	return (Atual = find(Estaca)) != end();};

   bool Posiciona(CEstaca& Estaca,double ToleranciaX,int Tipo);
   bool Posiciona(CEstaca& Estaca,double ToleranciaX. double ToleranciaY, int Tipo);

    //--- Insertores

   void InsereOrdenada(CSecao &No) { insert(No);};

   //--- Extratores

   itMapaSecoes PegaAtual(){return Atual;};

   //--- Operadores

   CListaSecoes& operator=(CListaSecoes &No);    
   BOOL operator++(int i);      //--- Avança no mapa
   BOOL operator--(int i);      //--- retrocede no mapa. 
};
