#include "stdafx.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "math.h"
#include "estaca.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "supercstring.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqTaludesPavim.h" 
#include "TaludesPavim.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "csecao.h"
#include "math.h"
#include "float.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define FINAL 1
#define EIXO  3
//#define PROJETO_TERRENO 2
#define IGUAL 0

//-------------------------------------------------------------------------

CSecao::CSecao(Perfil& Terreno,Perfil& Projeto,SecTipo& SecaoTip) : Terreno(Terreno),
        Projeto(Projeto),SecaoTipo(SecaoTip),E(0.0),SE(0.0),IAnte(INFINITO),IPost(INFINITO),
                                HPavimento(0.0),Gradiente(0.0),Estaca(Projeto.Estaca)
{
        BordosTerraplanagem[ESQUERDO].Descricao = "BORTEE";
        BordosTerraplanagem[DIREITO].Descricao = "BORTED";
        OffSets[ESQUERDO].Descricao = "OFFSEE";
        OffSets[DIREITO].Descricao = "OFFSED";

  //--- Limpa as áreas. Verificar o princípio de que as inicializações de 
        //--- números são = 0

  for (int i= ESQUERDO ; i<= DIREITO ; i++)
  {
          Area[i] = AreaTotal[i] = Volume[i] = Volume[i+2] = CG[i] = 0.0;
          CVs[i] = INFINITO;
  }
}

CSecao::CSecao(CSecao& Secao) { *this = Secao;}

CSecao::~CSecao(){}     

CSecao& CSecao::operator=(CSecao &No)
{
  Menores[X] = Menores[X];
        Menores[Y] = Menores[Y];
  Maiores[X] = Maiores[X];
        Maiores[Y] = Maiores[Y];

        Estaca = No.Estaca;
        Terreno = No.Terreno;
        Projeto = No.Projeto;
        Pavimento = No.Pavimento;
        SecaoTipo = No.SecaoTipo;
        Intersecoes = No.Intersecoes;
        Intersecao = No.Intersecao;
        AreaAtual = No.AreaAtual;
        HPavimento = No.HPavimento;
        Gradiente = No.Gradiente;

  for (int i= ESQUERDO ; i<= DIREITO ; i++)
  {
          Area[i] = No.Area[i];
                AreaTotal[i] = No.AreaTotal[i];
                Volume[i] = No.Volume[i];
                Volume[i+2]  = No.Volume[i+2];
                CG[i] = No.CG[i];
          CVs[i] = No.CVs[i];
                OffSets[i] = No.OffSets[i];
  }

        E = No.E;
        SE = No.SE;
        IAnte = No.IAnte;
        IPost = No.IPost;

        return *this;
}

void CSecao::CalculaArea()
{
  Intersecoes.erase(inter);    //--- Exclui as interseções anteriores,

        //--- Remove os pontos anteriores e posteriores ao off-set.
        //--- Ou seja faz com que o primeiro e o último ponto de terreno sejam os off-sets.

  Terreno.TrimLeft(Projeto.GetHead().x);
  Terreno.TrimRight(Projeto.GetTail().x);

  //--- Deve haver no mínimo 2 pontos em cada perfil e a soma de pontos nos 
        //--- 2 perfís tem que ser >= 5, aborta avisando, há um erro nos arquivos...

  if (Projeto.size() < 2 || Terreno.size() < 2 ||
            Projeto.size() + Terreno.size() < 5) return ;


/*  NUNCA ACONTECERÁ, OS OFF SETS FORAM CALCULADO HÁ POUCO.....PARANOIA....
        //--- Se o primeiro e último ponto de terreno não são os off sets, os arquivos são incompatíveis...
        //--- Pode ter havido uma troca de nome..., alguma coisa assim, aborta avisando.  
        //--- testar a assertiva por um tempo....
        */
  if (Terreno.begin().x != Projeto.begin().x || 
                        ((Terreno.end())--).x != ((Projeto.end().x))--) {ASSERT(0);return;}  //-- Arquivos incompatíveis!!!

        //--- Inicializa os ponteiros dos perfís.

  itmCProjeto ProjAtual(Projeto.begin());
  itmCTerreno TerrAtual(Terreno.begin()));

  //--- Pegua o primeiro e o segundo ponto de cada perfil para iniciar o cálculo
  
        Ponto NoProjAnte = *(ProjAtual++),
  NoTerrAnte = *(TerrAtual++),
  NoProjPost = *(ProjAtual++),
  NoTerrPost = *(TerrAtual++);

        //--- rotina para o cálculo das àreas. O do interno calcula as áreas de 
        //--- interseçao a interseçao e o do externo vai somando as areas até
        //--- chegar no off-set direito.

        do  //--- faça (até acabarem os pontos...)  (de terreno e de projeto)
  {
    do  //--- Faça (até achar uma interseção...(vai haver ao menos uma, o off-set direito))   
          {
                  //--- Se o ponto posterior de projeto está antes do ponto posterior de terreno,
                        //--- calcula a área de projeçao desta reta de projeto, pois não pode haver interseçao nela
                        //--- (neste ponto sabe-se que não pode haver interseçao entre as retas atuais, se houve
                        //---  o ponto anterior é a intersecão), pega a proxima reta de projeto, caso contrário 
                        //---  faz o mesmo com o terreno.

            if (NoProjPost.x < NoTerrPost.x) 
                        {
              Area[PROJETO] += CalculaAreaProjecao(NoProjAnte,NoProjPost);
                                NoProjAnte = NoProjPost;
                          NoProjPost = Projeto.GetNext(ProjAtual);
                        }
            else 
                        {
              Area[TERRENO] += CalculaAreaProjecao(NoTerrAnte,NoTerrPost);
                                NoTerrAnte = NoTerrPost;
                                NoTerrPost = Terreno.GetNext(TerrAtual);
                        }

                 //--- Calcula a interseção com as retas atuais, se ela não foi encontrada volta ao do,
                 //--- Para continuar o cáculo da figura.

                }while (!CalculaIntersecao(NoProjAnte,NoProjPost,NoTerrAnte,NoTerrPost));

                //--- Foi encontrada uma interseção, então fecha a área da figura, inicializa 
                //--- os pontos anteriores (eles passam a ser a interseçao encontrada), e 
                //--- prepara para calcular a área da nova figura, se existir.

          FechaArea(NoProjAnte,NoTerrAnte);
                NoTerrAnte = NoProjAnte = Intersecao;
                Area[TERRENO] = Area[PROJETO] = 0.0;

  //--- quando a interseçao é o off-set direito, chegou o fim, 
        //--- um dos ponteiros (ou os dois) ficam nulos, então termina os pontos da outra lista...
         
        }while(TerrAtual || ProjAtual);
}

void CSecao::FechaArea(Ponto NoProjAnte,Ponto NoTerrAnte)
{
  //--- Calcula as àreas remanescentes (do ponto anterior até a interseção) 
        //--- de terreno e de Projeto.
        //--- Subtrai uma da outra o resultado é a área requerida. Se a área for
        //--- negativa ela é de corte, caso contrário é de aterro.

  Area[PROJETO] += CalculaAreaProjecao(NoProjAnte,Intersecao);
  Area[TERRENO] += CalculaAreaProjecao(NoTerrAnte,Intersecao);

  AreaAtual = Area[PROJETO] - Area[TERRENO];  //--- Area da figura atual.

  if (AreaAtual < 0.0) AreaTotal[CORTE] += AreaAtual;  //--- Se < 0 a àrea é corte caso contrario aterro
  else AreaTotal[ATERRO] += AreaAtual;
}

BOOL CSecao::CalculaIntersecao(Ponto& NoProjAnte,Ponto& NoProjPost,Ponto& NoTerrAnte,Ponto& NoTerrPost)
{
        return CSetPontos::CalculaIntersecao(NoProjAnte,NoProjPost,NoTerrAnte,NoTerrPost,Intersecao);
}

double CSecao::CalculaAreaProjecao(Ponto PonAnt,Ponto PonPost)
{
  //--- Calcula a àrea de projeção entre a reta dada e a reta f(x) = 0.
  //--- fabs é usado para o caso de cotas negativas, o programa verifica
        //--- o tipo de àrea (corte ou aterro) pelo sinal dela, e para cotas menores que zero o sinal
        //--- ficaria trocado.... 

  return (fabs(PonPost.y + PonAnt.y)) * (PonPost.x - PonAnt.x) / 2.0;
}

void CSecao::CalculaVolumes(const CSecao &Anterior)
{
  double SemiDist = Estaca.EstVirtual - Anterior.Estaca.EstVirtual;

  for (int Tipo = CORTE ; Tipo <= ATERRO ; Tipo++)
        {
                Volume[Tipo] = (AreaTotal[Tipo] + Anterior.AreaTotal[Tipo]) * SemiDist;  //--- Volume da secao
          Volume[Tipo+2] = Anterior.Volume[Tipo+2] + Volume[Tipo];   //--- volumes acumulados.
        }  
}

double CSecao::CalculaCG()
{
  return 0.0;
}

double CSecao::CalcCV(Ponto &PontoProj,Ponto &NoTerrAnte,Ponto &NoTerrPost)
{
        //@@@@--------->>>>>>>>>> verificar a necessidade de calcular o i

        double i = (NoTerrPost.y - NoTerrAnte.y) /
                         (NoTerrPost.x - NoTerrAnte.x + 1e-10),
                                 Cota =  i * (PontoProj.x - NoTerrPost.x) + NoTerrPost.y;

  return PontoProj.y - Cota;
}

void CSecao::PegaCVs(BOOL PAVIMENTACAO)
{
        if (Terreno.size() < 2) 
        {
          CVs[ESQUERDO] = CVs[DIREITO] = INFINITO;
                return;
        }
                
  itmCTerreno TerrAtual;   
  Ponto NoTerrAnte, NoTerrPost,BordoEsquerdo,BordoDireito;
        
  if (PAVIMENTACAO)  
        {
          BordoEsquerdo = SecaoTipo.begin();
                BordoDireito = *(SecaoTipo.end()--);
        }
        else 
        {
          BordoEsquerdo = Projeto.begin();
                BordoDireito = *(Projeto.end()--);
        }
 
  for (TerrAtual = Terreno.begin(), NoTerrAnte = *TerrAtual++, 
             NoTerrPost = Terreno.*TerrAtual++;
       TerrAtual!- CmTerreno.end() && NoTerrPost.x < BordoEsquerdo.x;
                         NoTerrAnte = NoTerrPost, NoTerrPost = *(TerrAtual++));

  if (TerrAtual) CVs[ESQUERDO] = CalcCV(BordoEsquerdo,NoTerrAnte,NoTerrPost);

        for (; TerrAtual &&     NoTerrPost.x < BordoDireito.x ;
               NoTerrAnte = NoTerrPost, NoTerrPost = Terreno.GetNext(TerrAtual));

  if (NoTerrPost.x < BordoDireito.x) return;
        else CVs[DIREITO] = CalcCV(BordoDireito,NoTerrAnte,NoTerrPost);
}

BOOL CSecao::CalculaOffSet(int Lado,int Tipo,CLPontos& Talude)
{
  if (Talude.size() < 2) return FALSE;
        if (Terreno.size() < 2) return FALSE;

  Ponto PontoTalAnte(Lado == ESQUERDO ? Projeto.begin() : *(Projeto.end()--)),
        PontoTalPost(PontoTalAnte);
  int Achou = FALSE; //--- Indica se achou a interseção

        //--- Inicia o for, Para todas as retas do talude, enquanto tiver pontos de talude e 
        //--- não achar a interseção, incremente o talude.

        itmPontos TalAtual(Talude.begin());
        TalAtual++;
        PontoTalPost = *(TalAtual++) ; 

  while (!Achou)
  {
    //--- Passa para a proxima reta de talude.

                PontoTalPost.x += PontoTalAnte.x;
                PontoTalPost.y += PontoTalAnte.y;

    //--- Prepara o terreno, inicializa os pontos pelo primeiro ponto,
                //--- Se o lado a ser calculado for o direito, posiciona o terreno
                //--- no primeiro ponto antes do bordo direito do Projeto, para adiantar, 
                //--- senão teria que calcular todas as interseçoes até ele, que com 
                //--- certeza não são a interseção desejada. 
    
                itmPontos TerrAtual(Terreno.begin());
                Ponto PontoTerrAnte = *TerrAtual++,
                      PontoTerrPost = *TerrAtual++;

                if (Lado == DIREITO)    
                {
                  while(TerrAtual && PontoTerrPost.x < PontoTalAnte.x)
                        {
                                 PontoTerrAnte = PontoTerrPost;
                                 PontoTerrPost = *TerrAtual++;                        
                        }
                        if (PontoTerrPost.x < PontoTalAnte.x) return FALSE;
                }

    //--- Para todos os pontos de terreno ate o bordo para o lado esquerdo , 
                //--- ou o fim do terreno para o lado direito procure pela 
                //--- interseção, caso ache sinalize Achou.
                //--- A Propriedade <Interseção> ficará com o ponto de interseção, caso encontre.

    while (!Achou)
                {
                  Achou = (Lado == ESQUERDO ? CalculaIntersecao(PontoTalPost,PontoTalAnte,PontoTerrAnte,PontoTerrPost) :
                                                    CalculaIntersecao(PontoTalAnte,PontoTalPost,PontoTerrAnte,PontoTerrPost));
            if (Achou) 
            {
                    OffSets[Lado].x = Intersecao.x;
                          OffSets[Lado].y = Intersecao.y;
      }
                        else
                        {
                          //--- Se o lado é esquerdo e o ponto posterior ultrapassou o valor x do bordo
                                //--- então a interseção não pode mais ser encontrada.

                    if (Lado == ESQUERDO && PontoTerrPost.x > PontoTalAnte.x) break;
                                if (!TerrAtual) break;  

              //--- Passa para o proximo ponto de terreno.
  
                    PontoTerrAnte = PontoTerrPost;
                                PontoTerrPost = *TerrAtual++;
                        }
                }
                if (!TalAtual) break;  //--- acabaram-se os pontos de talude? fim....

                //-- Passa para o proximo ponto de talude.

          PontoTalAnte = PontoTalPost;
          PontoTalPost = *TalAtual++;
        }
        return Achou;
}

BOOL CSecao::CalculaOffSets(CTaludesCorte& TaludesCorte, CTaludesAterro& TaludesAterro)
{
  CLPontos TCE,TCD,TAE,TAD;
        BOOL PavMixta[2],CalculouEsquerdo(FALSE);

        PegaCVs(TRUE);    //---  Pega as cotas vermelhas de pavimentação.

        //--- Verifica se o offset está dentro do talude de pavimentaçao.

        PavMixta[ESQUERDO] = CVs[ESQUERDO] > 0.0 && CVs[ESQUERDO] < -HPavimento ? TRUE : FALSE;
        PavMixta[DIREITO] = CVs[DIREITO] > 0.0 && CVs[DIREITO] < -HPavimento ? TRUE : FALSE;

        TaludesCorte.BuscaTalude(Estaca,ESQUERDO,TCE);
        TaludesCorte.BuscaTalude(Estaca,DIREITO,TCD);
        TaludesAterro.BuscaTalude(Estaca,ESQUERDO,TAE);
        TaludesAterro.BuscaTalude(Estaca,DIREITO,TAD);

  PegaCVs();  //--- Busca as cotas vermelhas de terraplanagem.

        //--- Se o talude de pavimentaçao for mixto, não calcula o off-set, ele já
        //--- foi calculado no cálculo dos bordos.
        
  if (!PavMixta[ESQUERDO])
        {
                if (CalculaOffSet(ESQUERDO,CVs[ESQUERDO] < 0.0 ? CORTE : ATERRO,CVs[ESQUERDO] < 0.0 ? TCE : TAE))
                {
            Projeto.InsereOrdenada(OffSets[ESQUERDO],FALSE);
                        CalculouEsquerdo = TRUE;
                }
        }
        else
        {
    Projeto.InsereOrdenada(Pavimento.GetHead(),FALSE);
                CalculouEsquerdo = TRUE;
        }

        if (!PavMixta[DIREITO])
        {
          if (CalculaOffSet(DIREITO,CVs[DIREITO] < 0.0 ? CORTE : ATERRO,CVs[DIREITO] < 0.0 ? TCD : TAD))
                {
            Projeto.InsereOrdenada(OffSets[DIREITO],FALSE);
                        return CalculouEsquerdo ? TRUE : FALSE;
                }
        }
        else
        {
    Projeto.InsereOrdenada(*(Pavimento.end()--),FALSE);
                return CalculouEsquerdo ? TRUE : FALSE;
        }
        return FALSE;
}

void CSecao::CalculaBordos(TaludesPavim &TalPavimento,double HPavim,CCanteiros& CanteirosCentrais)
{
  if (HPavim >= 0.0) return;   //--- Altura inválida!

  HPavimento = HPavim;
  PegaCVs(TRUE);       //--- Pega as cotas vermelhas de Pavimentação.
        Ponto Bordo[2] = {*(SecaoTipo.begin()),*(SecaoTipo.end()--)},  //--- bordos de pavimentação.
              Extremo[2] = {*(Projeto.begin(),*Projeto.end()--};          //--- Bordos da seçao de projeto.
        
        for(int Lado = ESQUERDO ; Lado <= DIREITO ; Lado++)
        {
          //--- Verifica se o talude é mixto e se tem alargamentos.

                BOOL TEMALARG(Bordo[Lado].x != Extremo[Lado].x);
          BOOL MIXTO(!TEMALARG && CVs[Lado] > 0.0 && CVs[Lado] < -HPavim);  //-- HPavim esta com sinal negativo....

                //--- Verifica o tipo de talude a calcular (corte ou aterro).
                //--- Se tem alargamento (TEMALARG) o talude é forçado para corte.

                int Tipo(TEMALARG || CVs[Lado] < 0.0 ? CORTE : ATERRO);

                if (!MIXTO)        //--- Se for para calcular talude comum. (Não mixto).
                {
                  //--- Calcula a interseção pela fórmula (B2-B1)/(m1-m2), a altura do 
                        //--- Pavimento (HPavim) é igual a B2-B1. 

                  double x(HPavim / (TalPavimento.Rampas[Lado*2+Tipo] - Bordo[Lado].i)),
                               y(x *TalPavimento.Rampas[Lado*2+Tipo]);

      //--- Calcula os Bordos de tarraplanagem.
                         
                  BordosTerraplanagem[Lado].x = x + Bordo[Lado].x;
                  BordosTerraplanagem[Lado].y = y + Bordo[Lado].y;

                        //--- Insere os bordos na lista dos pontos de terraplanagem.

                        Projeto.InsereOrdenada(BordosTerraplanagem[Lado]);

                        //--- Se o talude é de aterro o bordo da pista não faz parte da terraplanagem,
                        //--- Caso Contrario, (se for corte) volta o bordo para a pavimentação, somando 
                        //--- a altura do pavimento.
                        //--- Os bordos estão com os valores da secao-tipo (cota de pavimentacao) e o 
                        //--- projeto esta com as cotas de terraplanagem.

                        if (Tipo == ATERRO)     
                        {
                          Projeto.ExcluiPonto(Bordo[Lado].x);    //--- Exclui o bordo da pista.

                                //--- insere o bordo na lista da pavimentação. No caso de corte o bordo calculado não 
                                //--- faz parte dos pontos de pavimentacao.

                                Pavimento.InsereOrdenada(BordosTerraplanagem[Lado]); 
                        }
                        else //--- Se for corte volta a cota do bordo para a de pavimentação.
                               *(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y+HPavim))->y -= HPavim;
                }
                else     //--- Calcular o talude Mixto.
                {
                  //--- Cria um talude de aterro para o cálculo do off-set de pavimentação.

                  CLPontos TalPavim;
                        TalPavim.insert(Ponto(TalPavimento.Rampas[Lado*2+1]*-100.0,-100.0));
                        TalPavim.insert(Ponto(0.0,0.0));

                         //--- Passa a cota do bordo para a cota de pavimentaçao, para cálculo da interseção.
                         //--- HPavim está negativo, porisso o -=.
                          
                        *(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y+HPavim))->y -= HPavim;

      //--- Calcula o off-set de pavimentação (Tem que achar).

      if (!CalculaOffSet(Lado,ATERRO,TalPavim)) ASSERT(0);

                        //--- volta novamente a cota do bordo para a cota de terraplangem.

                        (Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y)).y += HPavim; 

      //--- Calcula o x e o y do offset para o bordo de terraplanagem.
                        //--- A rampa do talude usada é a de corte para os taludes de pavimento.

                        double x((-HPavim - (Bordo[Lado].y - OffSets[Lado].y)-(OffSets[Lado].x-Bordo[Lado].x)*Bordo[Lado].i) / 
                                       (TalPavimento.Rampas[Lado*2] - Bordo[Lado].i)),
                               y(x *TalPavimento.Rampas[Lado*2]);

                        //--- Calcula o bordo da terraplanagem.

                  BordosTerraplanagem[Lado].x = OffSets[Lado].x - x;
                  BordosTerraplanagem[Lado].y = OffSets[Lado].y - y;

                        Projeto.InsereOrdenada(BordosTerraplanagem[Lado]);

                        //--- Neste caso o bordo de terraplanagem não é o bordo da seçao,exclui-o

                        Projeto.ExcluiPonto(Bordo[Lado].x);

                        //--- Neste caso o off-set faz parte da lista de pavimentação.

                        Pavimento.InsereOrdenada(OffSets[Lado]);
                }
        }
        
        //--- Insere o restante dos pontos de pavimentação. O primeiro e o último, que são od bordos
        //--- foram inseridos no cálculo dos bordos.

        Pavimento.InsereLista(SecaoTipo); 
        Pavimento.Estaca = Terreno.Estaca;  //--- Coloca a estaca na lista de pavimentação
  
        //--- Calcula os bordos de terraplanagem dos canteiros centrais.
        //--- Calcula a interseção pela fórmula (B2-B1)/(m1-m2), a altura do 
        //--- Pavimento (HPavim) é igual a B2-B1. 

        CmPontos Canteiros;
  if (!CanteirosCentrais.BuscaCanteiros(Estaca,Canteiros)) return;  //--- não tem canteiros nesta seção.
 
        itmPontos PCantAtual(Canteiros.begin());
        SecaoTipo.CalculaIs();

        while (PCantAtual)
        {
          Ponto PCanteiro(*(PCantAtual++));

    for(int Lado = ESQUERDO ; Lado <= DIREITO ; Lado++)    ///--- Cada canteiro tem 2 lados
                {
      if (SecaoTipo.PosicionaNoMaisProximo(Lado ? PCanteiro.y : PCanteiro.x) != IGUAL) break;

                        const Ponto& PPavimentacao = SecaoTipo.PegaPontoAtual();
            double x(HPavim / (TalPavimento.Rampas[!Lado*2+CORTE] - PPavimentacao.i)),
                   y(x *TalPavimento.Rampas[!Lado*2+CORTE]);

      //--- Calcula os Bordos de tarraplanagem.
                 
                        Ponto Bordo(x + PPavimentacao.x,y + PPavimentacao.y);

                        //--- Insere o bordo calculado na lista de projeto.

                        Projeto.InsereOrdenada(Bordo);
                }
        }

        //--- Soma o Hpavim nos pontos de projeto que estiverem dentro do canteiro central.

        PCantAtual = Canteiros.begin();

        while (PCantAtual)
        {
          Ponto PCanteiro(*(PCantAtual)++);
   
                if (Projeto.PosicionaNoMaisProximo(PCanteiro.x) == IGUAL) 
                {
                        Ponto P;
                        do
                        {
                    P.x = Projeto.PegaPontoAtual().x;
                                P.y = Projeto.PegaPontoAtual().y - HPavim;
                    Projeto.AlteraPonto(P);
                        }while(P.x < PCanteiro.y && Projeto++);    
                }
        }
}

void CSecao::CalculaIs()    //--- Calcula os is de todas as retas dos perfis de terreno e de projeto.
{
  Projeto.CalculaIs();
        Terreno.CalculaIs();
        Pavimento.CalculaIs();
}

void CSecao::CotaSecao(double Gradi,double HPavim)
{
  Gradiente = Gradi;

  //--- cota a seçao tipo - verificar porque tem que cotar a secao tipo!!!

  itmSetPontos At(SecaoTipo.begin());   
        for ( ;  At!= end() ; At++)  *A.y += Projeto.Estaca.Cota;
       
  //--- cota a seçao tipo de pavimentação

        At = Pavimento.begin();
       for ( ;  At!= end() ; At++)  *A.y += Projeto.Estaca.Cota;
       
  //--- cota a seção tipo de Projeto.
        
        At = Projeto.begin();
        for ( ;  At!= end() ; At++)  *A.y += Projeto.Estaca.Cota;
       

void CSecao::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
        {
      PegaCVs();

      //--- Serializa o projeto.
      //---   Serializa o cabecalho da secao de projeto

      ar << Projeto.Estaca <<   SE << 0.0 //--- Superlargura
                     << CVs[ESQUERDO] << CVs[EIXO] << CVs[DIREITO] 
                                 << HPavimento << Gradiente << IAnte << IPost << E;
  }
        else
        {
            CString Desc;
            double SuperLargura;

            ar >> Projeto.Estaca >> SE >> SuperLargura  
                           >> CVs[ESQUERDO] >> CVs[EIXO] >> CVs[DIREITO] >> HPavimento 
                                 >> Gradiente >> IAnte >> IPost >> E;
        }

  //--- Serializa a lista de pontos da seção.

        Projeto.Serialize(ar);
  Terreno.Serialize(ar);
        Pavimento.Serialize(ar);
  Estaca = Projeto.Estaca;
} 

void CSecao::DefRetangDes()    //--- Separa as memores e as maiores coorddenadas do perfil.
{                              //--- Pega so o projeto (para centralizar o desenho da seção) 
        Menores[X] = Menores[Y] = INFINITO;
        Maiores[X] = Maiores[Y] = -INFINITO;

//  for(int i = 0 ; i < 2 ; i++)
        //{
         // Perfil& PerfilAtual = i ? Projeto : Terreno;

          Perfil& PerfilAtual(Projeto);


    POSITION Atual(PerfilAtual.begin());

          while(Atual)
    {
      Ponto& PontoAtual = *Atual++;
                
                  if (PontoAtual.x < Menores[X]) Menores[X] = PontoAtual.x;     
                  if (PontoAtual.y < Menores[Y]) Menores[Y] = PontoAtual.y;     
                  if (PontoAtual.x > Maiores[X]) Maiores[X] = PontoAtual.x;     
                  if (PontoAtual.y > Maiores[Y]) Maiores[Y] = PontoAtual.y;     
          }  
        //}
}

int CSecao::Compara(CSecao& No)
{
  return Estaca.Compara(No.Estaca);
}

int CSecao::Compara(CEstaca& Est)
{
  return Estaca.Compara(Est);
}

void CSecao::AlteraYPontos(double Deltay)
{
        //--- Move a seção no eixo y o valor de Deltay

        //-- Remove os pontos alem da secao Tipo (OffSets, Taludes e Bordos de terraplanagem

        itmSetPontos Atual(Pavimento.Gbegin());

        while (Atual)
        {
    Ponto PAtual(*Atual);

                if (PAtual.Descricao == "BORTEE" || PAtual.Descricao == "BORTED")
                {
      itmSetPontos  PTemp(Atual);
                        PTemp++;
                        Pavimento.Remove(Atual);
      Atual = PTemp;
                }
                else 
                {
                        Atual++;
                }
        }

}
