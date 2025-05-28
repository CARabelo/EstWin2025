class CSecao
{
 enum {ESQUERDO,DIREITO,NENHUM};
 enum {X,Y};
 enum {PROJETO,TERRENO};
 enum {CORTE,ATERRO,ATUAL,TOTAL};

 friend class DesSecao;

 double CalcCV(Ponto &PontoProj,Ponto &NoTerrAnte,Ponto &NoTerrPost);
 double CalculaAreaProjecao(Ponto PonAnt,Ponto PonPost);
 void FechaArea(Ponto NoProjAnte,Ponto NoTerrAnte);      //--- Fecha o cálculo das áreas, até a interseção   

 double Area[2],AreaAtual,HPavimento,Gradiente;
 Ponto Intersecao;                                       //--- Intersecao atual;
 CSetPontos Intersecoes;      //--- Lista das Intersecoes encontraas durante o 

public:
  BOOL CalculaOffSet(int  Lado,int Tipo,CLPontos& Talude);

   Perfil  Terreno,Projeto,Pavimento;  //--- Referencia para os perfís de projeto, terreno e pavimentação
         SecTipo SecaoTipo;
   double  Menores[2],Maiores[2],   //--- X e Y menores e maiores (usado nos desenhos).
                 AreaTotal[2],      //--- Areas. 
                 Volume[4],CG[2],   //--- Volumes e centros de grravidade.
                                         CVs[2];            //--- OffSets, Cotas Vermelhas 
   CEstaca Estaca;            //--- Valor da estaca.
   Ponto   OffSets[2],                  //--- OffSets. 
                 BordosTerraplanagem[2];      //--- Bordos de terrapalnagem

   //--- Dados para serialização da nota de serviço;
   double  E,                 //--- E da parabola   
                                         SE,                //--- SuperElevaçao
                                         IAnte,             //--- Rampaanterior 
                                         IPost;             //--- Rampa posterior

   CSecao(Cestaca& Estaca = Estaca(), Perfil&  Terreno = Perfil(), Perfil& Projeto=Perfil(),SecTipo& SecaoTip = SecTipo()); 
    CSecao(CSecao& Secao); //--- construtor de cópia
   ~CSecao();           //--- Destrutor.   
   void CalculaArea();      //--- Calcula as areas da seção.
   void CalculaVolumes(const CSecao &Anterior);   //--- Calcula os volumes da seção
   double CalculaCG();      //--- Calcula o centro de gravicade da seção.
   int CalculaIntersecao(Ponto& NoProjAnte, Ponto& NoProjPost, Ponto& NoTerrAnte, Ponto& NoTerrPost);  
   void PegaCVs(BOOL PAVIMENTACAO=FALSE);
   BOOL CalculaOffSets(CTaludesCorte& TaludesCorte, CTaludesAterro& TaludesAterro);
         void CotaSecao(double Gradi,double HPavim);
   void GravaNota(CArchive &ar,double E,double SE,double IAnte,double IPost);
         void LeNota(CArchive &ar,double& E,double& SE,double& IAnte,double& IPost);
         void Serialize(CArchive& ar);
   void DefRetangDes();       //--- Define o retangulo mínimo para desenho (usada nos desenhos/)
         void CalculaIs();
         int Compara(CSecao& No);
         int Compara(CEstaca& Est);
         void CalculaBordos(TaludesPavim &TalPavimento,double HPavim,CCanteiros& Canteiros);
         void AlteraYPontos(double Deltay);

         CSecao &operator=(CSecao &No);
};
