typedef map<CEstaca&,CSecao&>::iterator itMapaSecoes;
typedef map<CEstaca&,CSecao&> tyMapaSecoes;

class CListaSecoes : public  set<CEstaca&,const CEstaca::compara(const CEstaca& Est)> 
{
  enum{PROJETO,TERRENO,PROJETO_TERRENO,NENHUM};
 
        itMapaSecoes Atual;        //--- Ponteiro para o Atual do mapa. 

        CString Projeto;           //--- Nome do projeto em uso.
        CLPerfis TerrenoOrfao;     //--- Lista de terreno sem projeto

        protected:

        int Compara(CSecao &No1,CSecao &No2);  //--- compara dois nós.
         //--- Posiciona o ponteiro da lista na seção igual ou mais 
         //--- proxima no sentido indicado
        int PosicionaNoMaisProximo(CEstaca Estaca,int Direcao = FALSE); 

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
        itMapaSecoes PosicionaEstaca(double x,double y,double Toleranciax,double Toleranciay,int Tipo);  

        //--- Posicionadores públicos do mapa.

        inline itMapaSecoes CMapaSecoes::PosicionaPrimeira(){ return Atual = Begin();
        inline itMapaSecoes CMapaSecoes::PosicionaUltima(){ return Atual = end();}

        inline bool Posiciona(CEstaca& Estaca) { Atual = find(Estaca ; 
                                                 return  Atual != MapaSecoes::end();}
        bool Posiciona(CEstaca& Estaca,double ToleranciaX,int Tipo);
        bool Posiciona(CEstaca& Estaca,double ToleranciaX. double ToleranciaY, int Tipo);

         //--- Insertores

         void InsereOrdenada(CSecao &No); //--- Insersão no mapa.

        //--- Extratores

        itMapaSecoes PegaAtual(){return Atual;};

        //--- Operadores

        CListaSecoes& operator=(CListaSecoes &No);    
        BOOL operator++(int i);      //--- Avança no mapa
        BOOL operator--(int i);      //--- retrocede no mapa. 
};
