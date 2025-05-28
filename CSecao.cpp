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
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "csectipo.h"
#include <list>
#include "CSecaoAvulsa.h"
#include "csecao.h"
#include "math.h"
#include "float.h"
#include "resource.h"
#include "monologo.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define FINAL 1
#define MENOR -1
#define IGUAL 0

extern class monologo monolog;

//-------------------------------------------------------------------------

CSecao::CSecao(Perfil& Terreno, Perfil& Projeto, SecTipo& SecaoTip, Perfil& Limp) : Terreno(Terreno),
Projeto(Projeto), SecaoTipo(SecaoTip), Limpeza(Limp), E(0.0), SE(0.0), IAnte(INFINITO), IPost(INFINITO),
HPavimento(0.0), Gradiente(0.0), Estaca(Projeto.Estaca), CompLateral(0.0), Empolamento(0.0)
{
  BordosTerraplanagem[ESQUERDO].Descricao = "BORTEE";
  BordosTerraplanagem[DIREITO].Descricao = "BORTED";
  OffSets[ESQUERDO].Descricao = "OFFSEE";
  OffSets[DIREITO].Descricao = "OFFSED";

  //Estaca = Projeto.Estaca;

  //--- Limpa as áreas. Verificar o princípio de que as inicializações de 
  //--- números são = 0

  for (int i = ESQUERDO; i <= DIREITO; i++)
  {
    Area[i] = AreaTotal[i] = Volume[i] = Volume[i + 2] = Volume[i + 4] = Volume[i + 6] = Volume[i + 8] = CGs[i] = 0.0;
    CVs[i] = INFINITO;
  }
}

void CSecao::Constroi(Perfil& pTerreno, Perfil& pProjeto, SecTipo& pSecaoTip)
{
  Terreno = pTerreno;
  Projeto = pProjeto;
  SecaoTipo = pSecaoTip;

  E = 0.0;
  SE = 0.0;
  IAnte = INFINITO;
  IPost = INFINITO;
  HPavimento = 0.0;
  Gradiente = 0.0;
  Estaca = pProjeto.Estaca;
  CompLateral = 0.0;
  Empolamento = 0.0;
  Limpeza = 0.0;

  BordosTerraplanagem[ESQUERDO].Descricao = "BORTEE";
  BordosTerraplanagem[DIREITO].Descricao = "BORTED";
  OffSets[ESQUERDO].Descricao = "OFFSEE";
  OffSets[DIREITO].Descricao = "OFFSED";

  //--- Limpa as áreas. 

  for (int i = ESQUERDO; i <= DIREITO; i++)
  {
    Area[i] = AreaTotal[i] = Volume[i] = Volume[i + 2] = Volume[i + 4] = Volume[i + 6] = Volume[i + 8] = CGs[i] = 0.0;
    CVs[i] = INFINITO;
  }
};

CSecao::CSecao(const CSecao& Secao) { *this = Secao; }

CSecao::~CSecao() {}

CSecao& CSecao::operator=(const CSecao &No)
{
  Menores[X] = No.Menores[X];
  Menores[Y] = No.Menores[Y];
  Maiores[X] = No.Maiores[X];
  Maiores[Y] = No.Maiores[Y];

  Estaca = No.Estaca;
  Terreno = No.Terreno;
  Projeto = No.Projeto;
  Pavimento = No.Pavimento;
  Limpeza = No.Limpeza;
  SecaoTipo = No.SecaoTipo;
  Intersecoes = No.Intersecoes;
  Intersecao = No.Intersecao;
  AreaAtual = No.AreaAtual;
  HPavimento = No.HPavimento;
  Gradiente = No.Gradiente;
  CompLateral = No.CompLateral;
  Empolamento = No.Empolamento;
  Limpeza = No.Limpeza;
  AcidentesTopograficos = No.AcidentesTopograficos;
  AlinhamentosTerrap = No.AlinhamentosTerrap;

  for (int i = ESQUERDO; i <= DIREITO; i++)
  {
    Area[i] = No.Area[i];
    AreaTotal[i] = No.AreaTotal[i];
    Volume[i] = No.Volume[i];
    Volume[i + 2] = No.Volume[i + 2];
    Volume[i + 4] = No.Volume[i + 4];
    Volume[i + 6] = No.Volume[i + 6];
    Volume[i + 8] = No.Volume[i + 8];
    CGs[i] = No.CGs[i];
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
  Area[CORTE] = Area[ATERRO] = AreaTotal[CORTE] = AreaTotal[ATERRO] = 0.0;

  if (Projeto.GetCount() > 1 && Projeto.GetHead().Descricao == "OFFSEE" && Projeto.GetTail().Descricao == "OFFSED")
  {
    Terreno.InsereOrdenada(Projeto.GetHead());
    Terreno.InsereOrdenada(Projeto.GetTail());
  }
  else return;   //--- A seção não tem off-sets, não há nada a calcular.

  Intersecoes.RemoveAll();    //--- Exclui as interseções anteriores,

  double XOffSetDir(Projeto.GetTail().x);  //--- Guarda o OFFSet Direito

  //--- Remove os pontos anteriores e posteriores aos off-sets.
  //--- Ou seja faz com que o primeiro e o último ponto de terreno sejam os off-sets.

  Terreno.TrimLeft(Projeto.GetHead().x);
  Terreno.TrimRight(Projeto.GetTail().x);

  //--- Inicializa os ponteiros dos perfís.

  POSITION ProjAtual = Projeto.GetHeadPosition(),
    // TerrAtual = Terreno.PosicionaPonto(XOffSetEsq,Projeto.GetHead().y);  //--- Início do terreno,
           TerrAtual = Terreno.GetHeadPosition();  //--- Início do terreno,
    //--- Tem que achar, acabou de ser inserido
    //--- Pega o primeiro e o segundo ponto de cada perfil para iniciar o cálculo

  Ponto NoProjAnte = Projeto.GetNext(ProjAtual),
        NoTerrAnte = Terreno.GetNext(TerrAtual),
        NoProjPost = Projeto.GetNext(ProjAtual),
        NoTerrPost = Terreno.GetNext(TerrAtual);

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

      if (NoProjPost.x <= NoTerrPost.x)
      {
        Area[PROJETO] += CalculaAreaProjecao(NoProjAnte, NoProjPost);
        NoProjAnte = NoProjPost;
        NoProjPost = Projeto.GetNext(ProjAtual);
      }
      else
      {
        Area[TERRENO] += CalculaAreaProjecao(NoTerrAnte, NoTerrPost);
        NoTerrAnte = NoTerrPost;
        NoTerrPost = Terreno.GetNext(TerrAtual);
      }

      //--- Calcula a interseção com as retas atuais, se ela não foi encontrada volta ao do,
      //--- Para continuar o cáculo da figura.

    } while (!CalculaIntersecao(NoProjAnte, NoProjPost, NoTerrAnte, NoTerrPost));

    //--- Foi encontrada uma interseção, então fecha a área da figura, inicializa 
    //--- os pontos anteriores (eles passam a ser a interseçao encontrada), e 
    //--- prepara para calcular a área da nova figura, se existir.

    FechaArea(NoProjAnte, NoTerrAnte);
    NoTerrAnte = NoProjAnte = Intersecao;
    Area[TERRENO] = Area[PROJETO] = 0.0;

    //--- quando a interseçao é o off-set direito, chegou o fim, 
    //--- um dos ponteiros (ou os dois) ficam nulos, então termina o cálculo das áreas

  }while (!(NoTerrPost.x == XOffSetDir && NoProjPost.x == XOffSetDir));
}

void CSecao::FechaArea(Ponto NoProjAnte, Ponto NoTerrAnte)
{
  //--- Calcula as àreas remanescentes (do ponto anterior até a interseção) 
  //--- de terreno e de Projeto.
  //--- Subtrai uma da outra o resultado é a área requerida. Se a área for
  //--- negativa ela é de corte, caso contrário é de aterro.

  Area[PROJETO] += CalculaAreaProjecao(NoProjAnte, Intersecao);
  Area[TERRENO] += CalculaAreaProjecao(NoTerrAnte, Intersecao);

  AreaAtual = Area[PROJETO] - Area[TERRENO];  //--- Area da figura atual.

  if (AreaAtual < 0.0) AreaTotal[CORTE] += AreaAtual;  //--- Se < 0 a àrea é corte caso contrario aterro
  else AreaTotal[ATERRO] += AreaAtual;
}

BOOL CSecao::CalculaIntersecao(Ponto& NoProjAnte, Ponto& NoProjPost, Ponto& NoTerrAnte, Ponto& NoTerrPost)
{
  return CLPontos::CalculaIntersecao(NoProjAnte, NoProjPost, NoTerrAnte, NoTerrPost,Intersecao,true,1E-4);
}

double CSecao::CalculaAreaProjecao(Ponto PonAnt, Ponto PonPost)
{
  //--- Calcula a área do segmento de reta dado.
  //--- fabs é usado para o caso de cotas negativas, o programa verifica
  //--- o tipo de àrea (corte ou aterro) pelo sinal dela, e para cotas menores que zero o sinal
  //--- ficaria trocado.... 

  return (fabs(PonPost.y + PonAnt.y)) * (PonPost.x - PonAnt.x) / 2.0;
}

bool CSecao::CalculaVolumes(const CSecao &Anterior, double Empola)
{
  //if (AreaTotal[CORTE] == 0.0 && AreaTotal[ATERRO] == 0.0) return false; 

  double CoefEmpola(1.0 + Empola / 100.0);

  bool TemCorteOuAterro(false);

  double SemiDist((Estaca.EstVirtual - Anterior.Estaca.EstVirtual) / 2.0);

  for (int Tipo = CORTE; Tipo <= ATERRO; Tipo++)
  {
    Volume[Tipo] = (AreaTotal[Tipo] + Anterior.AreaTotal[Tipo]) * SemiDist;	 //--- Volume da secao
    Volume[Tipo + 2] = Anterior.Volume[Tipo + 2] + Volume[Tipo];                 //--- volumes acumulados.
    if (Volume[Tipo] != 0.0)
    {
      TemCorteOuAterro = true;                                               //--- Secao válida

      Volume[Tipo + 4] = Anterior.Volume[Tipo + 4] + Volume[Tipo];               //--- Volumes das massas
      if (Tipo == ATERRO)
      {
        Volume[Tipo + 5] = Volume[ATERRO] * CoefEmpola;//(1.0+CoefEmpola);                //--- Empolado Aterro
        Volume[Tipo + 6] = Anterior.Volume[Tipo + 6] + Volume[Tipo + 5];           //--- Massa empolado aterro
        Volume[Tipo + 7] = Anterior.Volume[Tipo + 7] + Volume[Tipo + 5];           //--- Acumulado empolado
        Volume[Tipo + 8] = Anterior.Volume[Tipo + 8] + Volume[Tipo + 5];           //--- Total Empolado  
      }
    }
    else
    {
      Volume[Tipo + 4] = Volume[Tipo];

      if (Tipo == ATERRO)
      {
        Volume[Tipo + 5] = 0.0;                                  //--- Empolado Aterro  
        Volume[Tipo + 6] = 0.0;                                  //--- Massa empolado aterro 
        Volume[Tipo + 7] = Anterior.Volume[Tipo + 7];              //--- Acumulado empolado
        Volume[Tipo + 8] = Anterior.Volume[Tipo + 8];              //--- Total Empolado  
      }
    }
  }

  CompLateral = Volume[CORTE] + Volume[EMPOLADOATERRO];
  Empolamento = Empola;

  return TemCorteOuAterro;
}

double CSecao::CalcCV(Ponto &PontoProj, Ponto &NoTerrAnte, Ponto &NoTerrPost)
{
  //@@@@--------->>>>>>>>>> verificar a necessidade de calcular o i

  double i = (NoTerrPost.y - NoTerrAnte.y) / (NoTerrPost.x - NoTerrAnte.x + 1e-10), Cota = i * (PontoProj.x - NoTerrPost.x) + NoTerrPost.y;

  return PontoProj.y - Cota;
}

void CSecao::PegaCVs(BOOL PAVIMENTACAO)
{
  if (Terreno.GetCount() < 2 || Projeto.GetCount() < 2)
  {
    CVs[ESQUERDO] = CVs[DIREITO] = CVs[EIXO] = INFINITO;
    return;
  }

  POSITION TerrAtual;
  Ponto NoTerrAnte, NoTerrPost, BordoEsquerdo, BordoDireito, Eixo(0.0, Projeto.Calculay(0.0));

  if (PAVIMENTACAO)
  {
    BordoEsquerdo = SecaoTipo.GetHead();
    BordoDireito = SecaoTipo.GetTail();
  }
  else
  {
    /*
    //---- Quando esta no terreno natural o head e o tail estão com os bordos de terraplanagem da pista. mas quando é perfil geologico
    //---- Estes pontos estão com os offsets que foram calculados no terrreno natual. Porisso é preciso posicionar novamente aqui.
    //---- O bordo é o primeiro ponto com "BORTE" na descrição

    POSITION Atual(Projeto.GetHeadPosition());

    while(Atual != NULL && Projeto.GetAt(Atual).Descricao != "SECTIP")
    {
     Projeto.GetNext(Atual);
    }

    if(Atual)  BordoEsquerdo = Projeto.GetAt(Atual);

    Atual = Projeto.GetTailPosition();

    while(Atual != NULL  && Projeto.GetAt(Atual).Descricao != "SECTIP") Projeto.GetPrev(Atual);

    if(Atual) BordoDireito = Projeto.GetAt(Atual);
    */

    BordoEsquerdo = Projeto.GetHead();
    BordoDireito = Projeto.GetTail();
  }

  for (TerrAtual = Terreno.GetHeadPosition(), NoTerrAnte = Terreno.GetNext(TerrAtual), NoTerrPost = Terreno.GetNext(TerrAtual);
       TerrAtual && NoTerrPost.x < BordoEsquerdo.x;
       NoTerrAnte = NoTerrPost, NoTerrPost = Terreno.GetNext(TerrAtual));

  //--- Verifica se o ponto está antes do bordo esquerdo, se não estiver não há como calcular a CV do lado esquerdo

  if (NoTerrAnte.x < BordoEsquerdo.x) CVs[ESQUERDO] = CalcCV(BordoEsquerdo, NoTerrAnte, NoTerrPost);

  for (; TerrAtual && NoTerrPost.x < Eixo.x; NoTerrAnte = NoTerrPost, NoTerrPost = Terreno.GetNext(TerrAtual));

  //--- CV no eixo

  if (NoTerrPost.x < Eixo.x) return;
  else if (Eixo.y != INFINITO) CVs[EIXO] = CalcCV(Eixo, NoTerrAnte, NoTerrPost);

  for (; TerrAtual && NoTerrPost.x < BordoDireito.x; NoTerrAnte = NoTerrPost, NoTerrPost = Terreno.GetNext(TerrAtual));

  //--- Verifica se o ponto passou o bordo esquerdo, se não passou não há como calcular a CV do lado direito

  if (NoTerrPost.x < BordoDireito.x) return;
  else CVs[DIREITO] = CalcCV(BordoDireito, NoTerrAnte, NoTerrPost);
}

BOOL CSecao::CalculaOffSet(int Lado, int TipoPerfil, CLPontos& Talude)
{
  if (Talude.GetCount() < 2) return FALSE;
  if (Terreno.GetCount() < 2) return FALSE;
  if (Projeto.GetCount() < 2) return FALSE;

  Ponto PontoTalAnte, PontoTalPost;

  if (1 || TipoPerfil == VUE_TERRENO_PRIMITIVO) PontoTalAnte = Lado == ESQUERDO ? Projeto.GetHead() : Projeto.GetTail();
  else PontoTalAnte = Lado == ESQUERDO ? BordosTerraplanagem[ESQUERDO] : BordosTerraplanagem[DIREITO];        //--- Perfil geológico não tem camada de pavimentação

  PontoTalPost = PontoTalAnte;

  int Achou = FALSE;              //--- Indica se achou a interseção
  Perfil ListaPontosTalude;       //--- Se achar o offset os pontos intermediários dos taludes tem q ser incluidos no projeto

  //--- Inicia o for, Para todas as retas do talude, enquanto tiver pontos de talude e 
  //--- não achar a interseção, incremente o talude.

  POSITION TalAtual(Talude.GetHeadPosition());
  Talude.GetNext(TalAtual);
  PontoTalPost = Talude.GetNext(TalAtual);

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

    POSITION TerrAtual(Terreno.GetHeadPosition());
    Ponto PontoTerrAnte(Terreno.GetNext(TerrAtual)),
          PontoTerrPost(Terreno.GetNext(TerrAtual));

    if (Lado == DIREITO)
    {
      while (TerrAtual && PontoTerrPost.x < PontoTalAnte.x)
      {
        PontoTerrAnte = PontoTerrPost;
        PontoTerrPost = Terreno.GetAt(TerrAtual);
        Terreno.GetNext(TerrAtual);
      }
      if (PontoTerrPost.x < PontoTalAnte.x) return FALSE;
    }

    //--- Para todos os pontos de terreno ate o bordo para o lado esquerdo , 
    //--- ou o fim do terreno para o lado direito procure pela 
    //--- interseção, caso ache sinalize Achou.
    //--- A Propriedade <Interseção> ficará com o ponto de interseção, caso encontre.

    while (!Achou)
    {
      Achou = (Lado == ESQUERDO ? CalculaIntersecao(PontoTalPost, PontoTalAnte, PontoTerrAnte, PontoTerrPost) :
                                  CalculaIntersecao(PontoTalAnte, PontoTalPost, PontoTerrAnte, PontoTerrPost));

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
        PontoTerrPost = Terreno.GetNext(TerrAtual);
      }
    }
    //--- Ponto de talude
    //--- No lado esquerdo está começando a procurar o off-set do inicio do terreno para a direita (deveria começar 
    //--- do primeiro ponto antes do bordo seguindo para a esquerda)
    //--- Neste caso o primeiro ponto do talude ja foi inserido e se o talude tem so 2 pontos não pode inserir de novo

    if (Lado == ESQUERDO)  ListaPontosTalude.InsereOrdenada(PontoTalAnte);
    if (Lado == DIREITO && !Achou) ListaPontosTalude.InsereOrdenada(PontoTalPost);

    //-- Passa para a próxima reta do talude.

    if (TalAtual)
    {
      PontoTalAnte = PontoTalPost;
      PontoTalPost = Talude.GetNext(TalAtual);
    }
    else break;
  }

  int ContaPontos(1);   //--- conta os pontos de talude, usado no projeto de terraplanagem, para separar os alinhamentos

  if (Achou)
  {
    //--- Insere os pontos de talude no projeto
    //--- Pontos de bermas ou mudança de rampa do talude
    //--- Precisa marcar e numerar estes pontos para serem usados no cálculo do projeto de terraplanagem
    //--- O lado esquerdo tem que ser invertido senão a ordem fica errada a medida de novas bermas/pontos foram surgindo

    if (Lado == DIREITO)
    {
      for (POSITION ItPonTalude = ListaPontosTalude.GetHeadPosition(); ItPonTalude != NULL; ListaPontosTalude.GetNext(ItPonTalude))
      {
        Ponto& PAtual(ListaPontosTalude.GetAt(ItPonTalude));

        if (PAtual.Descricao.Find("TALCT") != -1 || PAtual.Descricao.Find("TALAT") != -1)
        {
          CString NumFormatado;
          NumFormatado.Format("%02d", ContaPontos++);
          PAtual.Descricao += NumFormatado;

          Projeto.InsereOrdenada(ListaPontosTalude.GetAt(ItPonTalude));
        }
      }
    }
    else
    {
      for (POSITION ItPonTalude = ListaPontosTalude.GetTailPosition(); ItPonTalude != NULL; ListaPontosTalude.GetPrev(ItPonTalude))
      {
        Ponto& PAtual(ListaPontosTalude.GetAt(ItPonTalude));

        if (PAtual.Descricao.Find("TALCT") != -1 || PAtual.Descricao.Find("TALAT") != -1)
        {
          CString NumFormatado;
          NumFormatado.Format("%02d", ContaPontos++);
          PAtual.Descricao += NumFormatado;

          Projeto.InsereOrdenada(ListaPontosTalude.GetAt(ItPonTalude));
        }
      }
    }
  }

  return Achou;
}

BOOL CSecao::CalculaOffSets(CTaludesCorte& TaludesCorte, CTaludesAterro& TaludesAterro, int TipoPerfil, Ponto* rCVs, Ponto* pOffSetsE, Ponto* pOffSetsD)
{
  if (rCVs != NULL) rCVs->x = rCVs->y = pOffSetsE->x = pOffSetsE->y = pOffSetsD->x = pOffSetsD->y = INFINITO;

  BOOL PavMixta[2] = { FALSE,FALSE }, CalculouEsquerdo(FALSE);

  //  if (TipoPerfil == VUE_TERRENO_PRIMITIVO)
  {
    PegaCVs(TRUE);    //---  Pega as cotas vermelhas de pavimentação.

    //--- Verifica se o offset está dentro do talude de pavimentaçao.
    //--- Se tiver alargamento nunca estará, pois o talude de pavimentação é forçado para corte.

    PavMixta[ESQUERDO] = !TemAlargEsquerdo() && CVs[ESQUERDO] > 0.0 && CVs[ESQUERDO] < -HPavimento ? TRUE : FALSE;
    PavMixta[DIREITO] = !TemAlargDireito() && CVs[DIREITO] > 0.0 && CVs[DIREITO] < -HPavimento ? TRUE : FALSE;
  }

  CLPontos TCE, TCD, TAE, TAD;

  TaludesCorte.BuscaTalude(Estaca, ESQUERDO, TCE);
  TaludesCorte.BuscaTalude(Estaca, DIREITO, TCD);
  TaludesAterro.BuscaTalude(Estaca, ESQUERDO, TAE);
  TaludesAterro.BuscaTalude(Estaca, DIREITO, TAD);

  PegaCVs();                                      //--- Busca as cotas vermelhas de terraplanagem.

  if (rCVs != NULL)                                //--- Perfís Geológicos? salva as cotas vermelhas
  {
    rCVs->x = CVs[ESQUERDO];
    rCVs->y = CVs[DIREITO];
  }

  //--- A baliza nos pontos de offset indica o tipo, corte ou aterro.

  OffSets[ESQUERDO].Baliza = CVs[ESQUERDO] < 0.0;    
  OffSets[DIREITO].Baliza = CVs[DIREITO] < 0.0;    

  OffSets[ESQUERDO].AcidenteTopografico = (CVs[ESQUERDO] > 0.0) ? 898 : 899;  //--- 898 corte, 899 aterro
  OffSets[DIREITO].AcidenteTopografico = (CVs[DIREITO] > 0.0) ? 898 : 899;
  
  //--- Se o talude de pavimentaçao for mixto, não calcula o off-set, ele já
  //--- foi calculado no cálculo dos bordos.

  if (!PavMixta[ESQUERDO])
  {
    if (CalculaOffSet(ESQUERDO, TipoPerfil, CVs[ESQUERDO] < 0.0 ? TCE : TAE))
    {
      if (TipoPerfil != VUE_PERFIL_GEOLOGICO) Projeto.InsereOrdenada(OffSets[ESQUERDO], FALSE);
      else *pOffSetsE = OffSets[ESQUERDO];

      CalculouEsquerdo = TRUE;
    }
  }
  else
  {
    //--- Verifica o caso raro de a seção ser mixta, achar a cota vermelha (CV) e não achar o 
    //--- offset no talude de pavimentação. 

    if (TipoPerfil == VUE_TERRENO_PRIMITIVO)
    {
      if (Pavimento.GetCount())         //--- Se achou a interseção no talude de terraplanagem. (Se achou a seção tipo vai para o pavimento.)
      {
        Projeto.InsereOrdenada(Pavimento.GetHead(), FALSE);
        CalculouEsquerdo = TRUE;
      }
    }
    else   //--- é perfil geológico
    {
      if (pOffSetsE) *pOffSetsE = Pavimento.GetHead();
      CalculouEsquerdo = TRUE;
    }
  }

  if (!PavMixta[DIREITO])
  {
    if (CalculaOffSet(DIREITO, TipoPerfil, CVs[DIREITO] < 0.0 ? TCD : TAD))
    {
      if (TipoPerfil != VUE_PERFIL_GEOLOGICO) Projeto.InsereOrdenada(OffSets[DIREITO], FALSE);
      else *pOffSetsD = OffSets[DIREITO];

      return CalculouEsquerdo ? TRUE : FALSE;
    }
  }
  else
  {
    //--- Verrifica o caso raro de a seção ser mixta, achar a cota vermelha (CV) e não achar o 
    //--- offset no talude de pavimentação. 

    if (TipoPerfil == VUE_TERRENO_PRIMITIVO)
    {
      if (Pavimento.GetCount())    //--- se achou a interseção no talude de terraplanagem (Se achou a seção tipo vai para o pavimento.)
      {
        Projeto.InsereOrdenada(Pavimento.GetTail(), FALSE);
        return CalculouEsquerdo ? TRUE : FALSE;
      }
    }
    else
    {
      if (pOffSetsD) *pOffSetsD = Pavimento.GetTail();
      return CalculouEsquerdo ? TRUE : FALSE;
    }
  }
  return FALSE;
}

int CSecao::CalculaBordos(TaludesPavim &TalPavimento, double HPavim, CCanteiros& CanteirosCentrais, bool Cancelar)
{
  if (HPavim > 0.0 || HPavim < -3.0 || SecaoTipo.GetCount() == 0 || Projeto.GetCount() == 0)
  {
    std::string strMensg(HPavim > 0.0 || HPavim < -3.0 ? monolog.ErrosHPavim[1] : SecaoTipo.GetCount() == 0 ? "Falta seção tipo" : "Falta Projeto");

    ///   int Resp(monolog.mensagem(-1,strMensg.data(),Cancelar));   Aguardar Criação da opção desconsiderar proximas ocorrencias deste erro
       //if(!Cancelar || Resp != IDCANCEL) return ERROCALCBORDOS;   idem idem 

    int Resp(monolog.mensagem(-1, strMensg.data(), false));

    throw(1);
  }

  if (HPavim > -0.01) return CALCOK;     //--- HPavim entre 0.01 e 0 é considerado = 0, que não muda nada na seção
  
  HPavimento = HPavim;

 //--- Tem qu calcular a cota vermelha no bordo de terraplanagem e no bordo de pavimentação.
 //--- Se na terraplanagem for corte e na pavimentação for aterro é mixta --->ACERTAR NA PROXIMA VERSÃO

  Projeto.GetHead().y += HPavim;       //--- Tem que ser no x do bordo de terraplanagem, esta no x da pavimentação  -->>> ACERTAR NA PROXIMA VERSÃO
  Projeto.GetTail().y += HPavim;       //--- Idem
  PegaCVs(FALSE);                      //--- Pega as cotas vermelhas de terraplanagem.
  double CVsTerrp[3] = { CVs[0],CVs[1],CVs[2] };       //--- guarda-as
  Projeto.GetHead().y -= HPavim;
  Projeto.GetTail().y -= HPavim;
  PegaCVs(TRUE);                       //--- Pega as cotas vermelhas de Pavimentação.                
                                      
  Ponto Bordo[2] = { SecaoTipo.GetHead(),SecaoTipo.GetTail() },  //--- bordos de pavimentação.
        Extremo[2] = { Projeto.GetHead(),Projeto.GetTail() };	   //--- Bordos da seçao de projeto.

  for (int Lado = ESQUERDO; Lado <= DIREITO; Lado++)
  {
    double xBordoTerrap[2] = {
                               (Bordo[Lado].x + (HPavim / (TalPavimento.Rampas[Lado * 2 + CORTE] - (Bordo[Lado].i)))),
                               (Bordo[Lado].x + (HPavim / (TalPavimento.Rampas[Lado * 2 + ATERRO] - (Bordo[Lado].i))))
                             };

    //--- Verifica se o talude é mixto e se tem alargamentos.

    BOOL TEMALARG(Bordo[Lado].x != Extremo[Lado].x);
    BOOL MIXTO(!TEMALARG && CVsTerrp[Lado] < 0.0 && CVs[Lado] > 0.0);

    //--- Verifica o tipo de talude a calcular (corte ou aterro).
    //--- Se tem alargamento (TEMALARG) o talude é forçado para corte.

    int Tipo(TEMALARG || CVs[Lado] < 0.0 || CVs[Lado] == INFINITO ? CORTE : ATERRO);

    if (!MIXTO)	   //--- Se for para calcular talude comum. (Não mixto).
    {
      CalculaTerraplanagemNormal(HPavim,TalPavimento,Lado,Tipo,&Bordo[0],&BordosTerraplanagem[0]);
    }
    else     //--- Calcular o talude Mixto.
    {
      //--- Cria um talude de aterro para o cálculo do off-set de pavimentação.

      double X(Lado == ESQUERDO ? -100.0 : 100);

      CLPontos TalPavim;
      TalPavim.AddTail(Ponto(X, TalPavimento.Rampas[Lado * 2 + 1] * X));
      TalPavim.AddHead(Ponto(0.0, 0.0));

      //--- Passa a cota do bordo para a cota de pavimentaçao, para cálculo da interseção.
      //--- HPavim está negativo, porisso o -=.

      //Projeto.GetAt(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y+HPavim)).y -= HPavim;

      //--- Calcula o off-set de pavimentação 

      int CalculouOffSet(CalculaOffSet(Lado, CSecao::VUE_TERRENO_PRIMITIVO, TalPavim));

      //-- verifica o caso raro do offset de pavimentação estar depois da terraplanagem (o terreno caiu muito logo apos o fim do pavimento)
      
      double xBordoTerrap(Bordo[Lado].x + (HPavim / (TalPavimento.Rampas[Lado * 2 + Tipo] - (Bordo[Lado].i))));
            
      if ((Lado == ESQUERDO && OffSets[Lado].x < xBordoTerrap) || (Lado == DIREITO && OffSets[Lado].x > xBordoTerrap))
      {
        //--- Neste caso o talude é mixto no bordo da pista mas o offset de terraplanagem caiu depois (ou antes) da terraplanagem. 
        //--- volta para o caso normal

        CalculaTerraplanagemNormal(HPavim, TalPavimento, Lado, Tipo, &Bordo[0], &BordosTerraplanagem[0]);
      }
      else
      {
        //--- Talude Mixto.
        //--- Volta novamente a cota do bordo para a cota de terraplangem.

        Projeto.GetAt(Projeto.PosicionaPonto(Bordo[Lado].x, Bordo[Lado].y)).y += HPavim;

        //--- Caso não encontre ocorreu o raro caso em que o 
        //--- achou a cota vermelha mas não achou o off-set no talude de pavimentação
        //--- neste caso, não há nada mais a fazer,passa para o  calculo do outro lado

        if (!CalculouOffSet) continue;

        //--- Calcula o x e o y do offset para o bordo de terraplanagem.
        //--- A rampa do talude usada é a de corte para os taludes de pavimento.

        double x((-HPavim - (Bordo[Lado].y - OffSets[Lado].y) - (OffSets[Lado].x - Bordo[Lado].x) * Bordo[Lado].i) / (TalPavimento.Rampas[Lado * 2] - Bordo[Lado].i)),
               y(x * TalPavimento.Rampas[Lado * 2]);

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
  }

  //--- Insere o restante dos pontos de pavimentação. O primeiro e o último, que são od bordos
  //--- foram inseridos no cálculo dos bordos.

  Pavimento.InsereLista(SecaoTipo);
  Pavimento.Estaca = Terreno.Estaca;  //--- Coloca a estaca na lista de pavimentação

  //--- Calcula os bordos de terraplanagem dos canteiros centrais.
  //--- Calcula a interseção pela fórmula (B2-B1)/(m1-m2), a altura do 
  //--- Pavimento (HPavim) é igual a B2-B1. 

  CLPontos Canteiros;
  if (CanteirosCentrais.BuscaCanteiros(Estaca, Canteiros) == MENOR) return CALCOK;  //--- não tem canteiros nesta seção.

  POSITION PCantAtual(Canteiros.GetHeadPosition());
  SecaoTipo.CalculaIs();
  int ContaCanteiros(0);

  while (PCantAtual)
  {
    ContaCanteiros++;

    Ponto PCanteiro(Canteiros.GetNext(PCantAtual));

    for (int Lado = ESQUERDO; Lado <= DIREITO; Lado++)    ///--- Cada canteiro tem 2 lados
    {
      if (SecaoTipo.PosicionaNoMaisProximo(Lado == ESQUERDO ? PCanteiro.x : PCanteiro.y) != IGUAL)
      {
        //int Resp(monolog.mensagem(27,Estaca,-1,-1,Cancelar));  /// Aguardar: implementar a opçao de desconsiderar este erro
         // if(!Cancelar || Resp != IDCANCEL) return ERROCANTEIROS1;    idem idem
        ///  int Resp(monolog.mensagem(27,Estaca,-1,-1,false)); 
       //     throw(1);
      }

      //--- Se o lado for o esquerdo o i que deve ser usado é o i do ponto anterior (se houver ponto anterior) 

      const Ponto& PPavimentacao(SecaoTipo.PegaPontoAtual());

      //--- Pega o i, se for o lado esquerdo o i deve ser o do ponto anterior. caso haja ponto anterior 

      double i(PPavimentacao.i);

      if (Lado == ESQUERDO)
      {
        Ponto* PAnterior = SecaoTipo.PegaPontoAnterior();

        if (PAnterior) i = PAnterior->i;  //--- Se tiver ponto anterior pega o i (senão o i é o do ponto atual)
      }

      double x(HPavim / (TalPavimento.Rampas[!Lado * 2 + CORTE] - i)), y(x *TalPavimento.Rampas[!Lado * 2 + CORTE]);

      //--- Calcula os Bordos de tarraplanagem.

      Ponto Bordo(x + PPavimentacao.x, y + PPavimentacao.y);
      if (Lado == ESQUERDO) Bordo.Descricao.Format("BOCC%02dE", ContaCanteiros);
      else Bordo.Descricao.Format("BOCC%02dD", ContaCanteiros);

      //--- Insere o bordo calculado na lista de projeto.

      Projeto.InsereOrdenada(Bordo);
    }
  }

  //--- Soma o Hpavim nos pontos de projeto que estiverem dentro do canteiro central.

  PCantAtual = Canteiros.GetHeadPosition();

  while (PCantAtual)
  {
    Ponto PCanteiro(Canteiros.GetNext(PCantAtual));
    PCanteiro.z = -HPavim;

    Projeto.AlteraFaixaY(PCanteiro);
  }

  /*while (PCantAtual)
  {
  Ponto PCanteiro(Canteiros.GetNext(PCantAtual));

  if (Projeto.PosicionaNoMaisProximo(PCanteiro.x) == IGUAL)
  {
  Ponto P;

  do
  {
  P = Projeto.PegaPontoAtual();

  if(P.x <= PCanteiro.y)
  {
  P.y -=  HPavim;            // HPavim é negativo...
  Projeto.AlteraPonto(P);

  Projeto.PosicionaNoMaisProximo(P.x);
  ++Projeto;
  }

  }while(P.x < PCanteiro.y);
  }
  }*/

  return CALCOK;
}
/*


void CSecao::CalculaBordos(TaludesPavim &TalPavimento,double HPavim,CCanteiros& CanteirosCentrais)
{
if (HPavim >= 0.0 || SecaoTipo.GetCount() == 0 || Projeto.GetCount() == 0) return;   //--- Altura inválida ou sem seção tipo ou sem projeto

HPavimento = HPavim;
PegaCVs(TRUE);       //--- Pega as cotas vermelhas de Pavimentação.
Ponto Bordo[2] = {SecaoTipo.GetHead(),SecaoTipo.GetTail()},  //--- bordos de pavimentação.
Extremo[2] = {Projeto.GetHead(),Projeto.GetTail()};	   //--- Bordos da seçao de projeto.

for(int Lado = ESQUERDO ; Lado <= DIREITO ; Lado++)
{
//--- Verifica se o talude é mixto e se tem alargamentos.

BOOL TEMALARG(Bordo[Lado].x != Extremo[Lado].x);
BOOL MIXTO(!TEMALARG && CVs[Lado] > 0.0 && CVs[Lado] < -HPavim);  //-- HPavim esta com sinal negativo....

//--- Verifica o tipo de talude a calcular (corte ou aterro).
//--- Se tem alargamento (TEMALARG) o talude é forçado para corte.

int Tipo(TEMALARG || CVs[Lado] < 0.0 ? CORTE : ATERRO);

if (!MIXTO)	   //--- Se for para calcular talude comum. (Não mixto).
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
Projeto.GetAt(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y)).y -= HPavim;
}
else     //--- Calcular o talude Mixto.
{
//--- Cria um talude de aterro para o cálculo do off-set de pavimentação.

double X(Lado == ESQUERDO ? -100.0 : 100);

CLPontos TalPavim;
TalPavim.AddTail(Ponto(X,TalPavimento.Rampas[Lado*2+1] * X));
TalPavim.AddHead(Ponto(0.0,0.0));

//--- Passa a cota do bordo para a cota de pavimentaçao, para cálculo da interseção.
//--- HPavim está negativo, porisso o -=.

Projeto.GetAt(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y+HPavim)).y -= HPavim;

//--- Calcula o off-set de pavimentação

int CalculouOffSet(CalculaOffSet(Lado,ATERRO,TalPavim));

//--- volta novamente a cota do bordo para a cota de terraplangem.

Projeto.GetAt(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y)).y += HPavim;

//--- Caso não encontre ocorreu o raro caso em que o
//--- achou a cota vermelha mas não achou o off-set no talude de pavimentação
//--- neste caso, não há nada mais a fazer,passa para o  calculo do outro lado

if (!CalculouOffSet) break;

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

CLPontos Canteiros;
if (!CanteirosCentrais.BuscaCanteiros(Estaca,Canteiros)) return;  //--- não tem canteiros nesta seção.

POSITION PCantAtual(Canteiros.GetHeadPosition());
SecaoTipo.CalculaIs();

while (PCantAtual)
{
Ponto PCanteiro(Canteiros.GetNext(PCantAtual));

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

PCantAtual = Canteiros.GetHeadPosition();

while (PCantAtual)
{
Ponto PCanteiro(Canteiros.GetNext(PCantAtual));

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
*/

/*
void CSecao::CalculaBordos(TaludesPavim &TalPavimento,double HPavim,CCanteiros& CanteirosCentrais)
{
if (HPavim >= 0.0 || SecaoTipo.GetCount() == 0 || Projeto.GetCount() == 0) return;   //--- Altura inválida ou sem seção tipo ou sem projeto

HPavimento = HPavim;
PegaCVs(TRUE);       //--- Pega as cotas vermelhas de Pavimentação.
Ponto Bordo[2] = {SecaoTipo.GetHead(),SecaoTipo.GetTail()},  //--- bordos de pavimentação.
Extremo[2] = {Projeto.GetHead(),Projeto.GetTail()};	   //--- Bordos da seçao de projeto.

for(int Lado = ESQUERDO ; Lado <= DIREITO ; Lado++)
{
//--- Verifica se o talude é mixto e se tem alargamentos.

BOOL TEMALARG(Bordo[Lado].x != Extremo[Lado].x);
BOOL MIXTO(!TEMALARG && CVs[Lado] > 0.0 && CVs[Lado] < -HPavim);  //-- HPavim esta com sinal negativo....

//--- Verifica o tipo de talude a calcular (corte ou aterro).
//--- Se tem alargamento (TEMALARG) o talude é forçado para corte.

int Tipo(TEMALARG || CVs[Lado] < 0.0 ? CORTE : ATERRO);

if (!MIXTO)	   //--- Se for para calcular talude comum. (Não mixto).
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
Projeto.GetAt(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y)).y -= HPavim;
}
else     //--- Calcular o talude Mixto.
{
//--- Cria um talude de aterro para o cálculo do off-set de pavimentação.

double X(Lado == ESQUERDO ? -100.0 : 100);

CLPontos TalPavim;
TalPavim.AddTail(Ponto(X,TalPavimento.Rampas[Lado*2+1] * X));
TalPavim.AddHead(Ponto(0.0,0.0));

//--- Passa a cota do bordo para a cota de pavimentaçao, para cálculo da interseção.
//--- HPavim está negativo, porisso o -=.

Projeto.GetAt(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y)).y -= HPavim;

//--- Calcula o off-set de pavimentação

int CalculouOffSet(CalculaOffSet(Lado,ATERRO,TalPavim));

//--- volta novamente a cota do bordo para a cota de terraplangem.

Projeto.GetAt(Projeto.PosicionaPonto(Bordo[Lado].x,Bordo[Lado].y-HPavim)).y += HPavim;

//--- Caso não encontre ocorreu o raro caso em que o
//--- achou a cota vermelha mas não achou o off-set no talude de pavimentação
//--- neste caso, não há nada mais a fazer,passa para o  calculo do outro lado

if (!CalculouOffSet) break;

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

CLPontos Canteiros;
if (!CanteirosCentrais.BuscaCanteiros(Estaca,Canteiros)) return;  //--- não tem canteiros nesta seção.

POSITION PCantAtual(Canteiros.GetHeadPosition());
SecaoTipo.CalculaIs();

while (PCantAtual)
{
Ponto PCanteiro(Canteiros.GetNext(PCantAtual));

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

PCantAtual = Canteiros.GetHeadPosition();

while (PCantAtual)
{
Ponto PCanteiro(Canteiros.GetNext(PCantAtual));

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
*/
void CSecao::CalculaIs()    //--- Calcula os is de todas as retas dos perfis de terreno e de projeto.
{
  Projeto.CalculaIs();
  Terreno.CalculaIs();
  Pavimento.CalculaIs();
  Limpeza.CalculaIs();
}

void CSecao::CotaSecao(double Gradi, double HPavim, const SecTipo& pSecaoTipo)
{
  Gradiente = Gradi;

  //--- cota a seçao tipo - verificar porque tem que cotar a secao tipo!!!
  //-- Porque quando a seção é montada, ela é montada com os deltas h, não com as cotas

  POSITION At(SecaoTipo.GetHeadPosition());
  while (At) SecaoTipo.GetNext(At).y += Projeto.Estaca.Cota;

  //--- cota a seçao tipo de pavimentação

  At = Pavimento.GetHeadPosition();
  while (At) Pavimento.GetNext(At).y += Projeto.Estaca.Cota;

  //--- cota a seção tipo de Projeto.

  At = Projeto.GetHeadPosition();
  Ponto* p;

  //--- No Projeto pode haver pontos inseridos que já vem com a cota, não mais

  while (At)
  {
    p = &Projeto.GetNext(At);

    //--- Find retorna 0 se achou 

    //		if (p->Descricao.Find("PONINS")) 
    {
      p->y += Projeto.Estaca.Cota;

      if (p->x > pSecaoTipo.GetHead().x && p->x < pSecaoTipo.GetTail().x) p->y -= HPavim;
    }
  }
}

void CSecao::Serialize(CArchive& ar, bool EMedicao)
{
  if (ar.IsStoring())
  {
    PegaCVs();

    //--- Serializa o projeto.
    //---   Serializa o cabecalho da secao de projeto, se for medição, não tem cabeçalho DE PROJETO

    if (!EMedicao)
    {
      ar << Projeto.Estaca << SE << 0.0 //--- Superlargura
        << CVs[ESQUERDO] << CVs[EIXO] << CVs[DIREITO]
        << HPavimento << Gradiente << IAnte << IPost << E;
    }

    ar << AreaTotal[CORTE] << AreaTotal[ATERRO]
      << Volume[CORTE] << Volume[ATERRO] << Volume[ACUMULADOCORTE] << Volume[ACUMULADOATERRO]
      << Volume[EMPOLADOATERRO] << Volume[MASSAEMPOLATERRO] << Volume[ACUMEMPOLATERRO] << Volume[TOTALEMPOLATERRO]
      << Volume[MASSAATUALCORTE] << Volume[MASSAATUALATERRO]
      << CGs[CORTE] << CGs[ATERRO]
      << CompLateral << Empolamento;
  }
  else
  {
    CString Desc;
    double SuperLargura;

    //--- se for medição não tem cabeçalho.

    if (!EMedicao)
    {
      ar >> Projeto.Estaca >> SE >> SuperLargura
        >> CVs[ESQUERDO] >> CVs[EIXO] >> CVs[DIREITO] >> HPavimento
        >> Gradiente >> IAnte >> IPost >> E;
    }
    ar >> AreaTotal[CORTE] >> AreaTotal[ATERRO]
      >> Volume[CORTE] >> Volume[ATERRO] >> Volume[ACUMULADOCORTE] >> Volume[ACUMULADOATERRO]
      >> Volume[EMPOLADOATERRO] >> Volume[MASSAEMPOLATERRO] >> Volume[ACUMEMPOLATERRO] >> Volume[TOTALEMPOLATERRO]
      >> Volume[MASSAATUALCORTE] >> Volume[MASSAATUALATERRO]
      >> CGs[CORTE] >> CGs[ATERRO]
      >> CompLateral >> Empolamento;
  }

  //--- Serializa a lista de pontos da seção.

  Projeto.Serialize(ar);

  //--- quando não é medição o terreno é o terreno primitivo que já está no .tca, portanto não 
  //--- precisa ser serializado.
  //--- entretanto o terreno da medição o resultado da superposição do terreno primitivo com a medição anterior
  //--- portanto precisa ser serializado

  if (EMedicao) Terreno.Serialize(ar);
  Pavimento.Serialize(ar);
  Limpeza.Serialize(ar);
  AcidentesTopograficos.Serialize(ar);

  Estaca = Projeto.Estaca;
}

void CSecao::DefRetangDes()    //--- Separa as menores e as maiores coordenadas do perfil.
{                              //--- Pega so o projeto (para centralizar o desenho da seção) 
  Menores[X] = Menores[Y] = INFINITO;
  Maiores[X] = Maiores[Y] = -INFINITO;

  for (int i = 0; i < 2; i++)
  {
    Perfil& PerfilAtual = i ? Projeto : Terreno;

    POSITION Atual(PerfilAtual.GetHeadPosition());

    while (Atual)
    {
      Ponto& PontoAtual = PerfilAtual.GetNext(Atual);

     // if(PontoAtual.x >= 0.0 && PontoAtual.y >= 0.0)
      {
        if (PontoAtual.x < Menores[X]) Menores[X] = PontoAtual.x;
        if (PontoAtual.y < Menores[Y]) Menores[Y] = PontoAtual.y;
        if (PontoAtual.x > Maiores[X]) Maiores[X] = PontoAtual.x;
        if (PontoAtual.y > Maiores[Y]) Maiores[Y] = PontoAtual.y;
      }
    }
  }
}


/*
gravaponto(t->no->estacatual.valor,t->no->estacatual.cota,"EST");

gravaponto(t->no->estacatual.m,t->no->secaocalc.sl,t->no->estacatual.notavel);

gravaponto(t->no->cvboter[ESQUERDO],t->no->estacatual.cv[TRR][EIXO],"DIV");

gravaponto(t->no->cvboter[DIREITO],t->no->estacatual.difigual,"DIV");

gravaponto(t->no->estacatual.cv[PRO][ESQUERDO],t->no->estacatual.cv[PRO][EIXO],"DIV");

gravaponto(t->no->estacatual.cv[PRO][DIREITO],t->no->hpavim.valor,"DIV");

gravaponto(t->no->gradiente.x,t->no->vert[RAMPAS].i1,"DIV");

gravaponto(t->no->boter[ESQUERDO].x,t->no->boter[ESQUERDO].y,"DIV");

gravaponto(t->no->boter[DIREITO].x,t->no->boter[DIREITO].y,"DIV");

if (!strcmp(t->no->estacatual.notavel,"PIV"))
gravaponto(t->no->vert[RAMPAS].i2,t->no->vert[RAMPAS].E,"DIV");

*/
/*
void CSecao::GravaNota(CArchive &ar,double E,double SE,double IAnte,double IPost)
{
PegaCVs();

ar << CString("EST") << Projeto.Estaca.EstVirtual << Projeto.Estaca.Cota
<< Estaca.Descricao <<	SE << 0.0 //--- Superlargura
<< CVs[ESQUERDO] << CVs[EIXO]
<< CVs[DIREITO] << Estaca.EstVirtual - Estaca.EstReal
<< HPavimento << Gradiente
<< IAnte << IPost
<< E;

POSITION Atual = Projeto.GetHeadPosition();

ar << Projeto.GetCount();  //--- Numero de pontos da secao.

while (Atual) Projeto.GetNext(Atual) >> ar;
}

void CSecao::LeNota(CArchive &ar,double& E,double& SE,double& IAnte,double& IPost)
{
PegaCVs();
CString Desc;
double SuperLargura;

ar >> Desc;
if (Desc.Compare("EST")) ASSERT(0);

ar >> Projeto.Estaca.EstVirtual >> Projeto.Estaca.Cota
>> Estaca.Descricao >>	SE >> SuperLargura
>> CVs[ESQUERDO] >> CVs[EIXO]
>> CVs[DIREITO] >> Estaca.EstReal
>> HPavimento >> Gradiente
>> IAnte >> IPost
>> E;

POSITION Atual = Projeto.GetHeadPosition();

int i; //--- Numero de pontos da secao.

for( ar >> i ; i ; i--) Projeto.GetNext(Atual) << ar;
}
*/
int CSecao::Compara(CSecao& No)
{
  return Estaca.Compara(No.Estaca);
}


void CSecao::AlteraYPontos(double Deltay)
{
  //--- Move a seção no eixo y o valor de Deltay

  POSITION Atual(Pavimento.GetHeadPosition());

  while (Atual)
  {
    Ponto PAtual(Pavimento.GetAt(Atual));

    if (PAtual.Descricao == "BORTEE" || PAtual.Descricao == "BORTED")
    {
      POSITION  PTemp(Atual);
      Pavimento.GetNext(PTemp);
      Pavimento.RemoveAt(Atual);
      Atual = PTemp;
    }
    else
    {
      Pavimento.GetNext(Atual);
    }
  }

  /*
  //--- ESTA IGUALDADE NÃO PODE SER FEITA, TEM QUE PASSAR SO A LISTA DE PONTOS1
  //---- ESTÁ MUDANDO TAMBEM O VALOR DA ESTACA E A COTA PARA INFINITO
  Projeto = Pavimento;
  */
}


/*



void CSecao::AlteraYPontos(double Deltay)
{
//--- Move a seção no eixo y o valor de Deltay

//-- Remove os pontos alem da secao Tipo (OffSets, Taludes e Bordos de terraplanagem

POSITION Atual(Pavimento.GetHeadPosition());

while (Atual && Pavimento.GetAt(Atual).Descricao != "GRADIE")
{
Pavimento.GetNext(Atual);
}
if (!Atual) ASSERT(0);    //--- Tem q achar o gradiente

double CotaGradiente = Pavimento.GetAt(Atual).y;    //--- Pega a cota do gradiente

Atual = Pavimento.GetHeadPosition();

while (Atual)
{
Ponto& PAtual = Pavimento.GetAt(Atual);

if (PAtual.Descricao == "BORTEE" || PAtual.Descricao == "BORTED")
{
POSITION  PTemp(Atual);
Pavimento.GetNext(PTemp);
Pavimento.RemoveAt(Atual);
Atual = PTemp;
}
else
{
if (Deltay != INFINITO)
{
PAtual.y -= CotaGradiente;
PAtual.z -= CotaGradiente;
}
Pavimento.GetNext(Atual);
}
}


//--- ESTA IGUALDADE NÃO PODE SER FEITA, TEM QUE PASSAR SO A LISTA DE PONTOS1
//---- ESTÁ MUDANDO TAMBEM O VALOR DA ESTACA E A COTA PARA INFINITO
//	Projeto = Pavimento;



}
*/
/*
void AlteraPontos(double Deltay)
{
POSITION  Atual(Pavimento.GetHeadPosition());

while (Atual)
{

Ponto PAtual(Pavimento.GetAt(Atual));
Pavimento.GetNext(PTemp);
Pavimento.RemoveAt(Atual);
Atual = PTemp;
}
else Pavimento.GetNext(Atual);
}
*/

void CSecao::ExcluiPonTaludes()
{
  for (int Lado = ESQUERDO; Lado <= DIREITO; Lado++)
  {
    POSITION Atual(Lado == ESQUERDO ? Projeto.GetHeadPosition() : Projeto.GetTailPosition());

    while (Atual)
    {
      Ponto& PProjeto = Lado == ESQUERDO ? Projeto.GetNext(Atual) : Projeto.GetPrev(Atual);

      if (PProjeto.Descricao == "OFFSEE" || PProjeto.Descricao == "TALUDE" || PProjeto.Descricao == "BORTEE") Projeto.RemoveHead();
      else if (PProjeto.Descricao == "OFFSED" || PProjeto.Descricao == "TALUDD" || PProjeto.Descricao == "BORTED") Projeto.RemoveTail();
      else break;
    }
  }
}

void CSecao::IncluiListaLimpeza(double Limp, double ExessoLateral, Perfil& TerrenoAtual)
{
  if (Projeto.GetCount() < 2 || Limp < 0.05 || TerrenoAtual.GetCount() < 2) return;

  Ponto OffSetEsq(Projeto.GetHead()), OffSetDir(Projeto.GetTail());

  if (Projeto.GetCount() > 1)
  {
    POSITION PonAtual(TerrenoAtual.GetHeadPosition());

    Terreno.ExcluiPontos(CString("LIM"));
    Limpeza.RemoveAll();

    if (OffSetEsq.Descricao == "OFFSEE")
    {
      Ponto LimOffEsq(TerrenoAtual.Interpola(OffSetEsq.x - ExessoLateral - 0.001, CString("LIM")));  ///*** no desenho da pb com os pontos com x = ver setxcotas com x iguais

      if (LimOffEsq.x != INFINITO)    //--- Conseguiu calcular o excesso lateral? (não tem pontos suficientes)
      {
        Limpeza.AddTail(LimOffEsq);
        LimOffEsq.x += 0.001;
        LimOffEsq.y -= Limp;
        Limpeza.AddTail(LimOffEsq);

        Ponto PontoAtual(TerrenoAtual.GetNext(PonAtual));

        while (PontoAtual.x < (OffSetEsq.x - ExessoLateral + 0.001)) PontoAtual = TerrenoAtual.GetNext(PonAtual);  //--- passa os pontos antes do offset

        if (OffSetDir.x != INFINITO && PontoAtual.x < OffSetDir.x)
        {
          PontoAtual.y -= Limp;                            //--- Primeiro ponto depois do offset esquerdo
          Limpeza.AddTail(PontoAtual);
        }
      }
    }

    if (OffSetDir.Descricao == "OFFSED")
    {
      Ponto PontoAtual;

      if (PonAtual)                           //--- Se == NULL o proximo ponto do terreno está depois do offset direito
      {
        PontoAtual = TerrenoAtual.GetAt(PonAtual);

        while (PonAtual && PontoAtual.x < (OffSetDir.x + ExessoLateral - 0.0001))    //--- Passa para a limpeza os pontos entre os offsets
        {
          PontoAtual.y -= Limp;
          Limpeza.AddTail(PontoAtual);
          PontoAtual = TerrenoAtual.GetNext(PonAtual);
        }
      }

      Ponto LimOffDir(TerrenoAtual.Interpola(OffSetDir.x + ExessoLateral + 0.001, CString("LIM")));  ///*** no desenho da pb com os pontos com x = ver setxcotas com x iguais

      if (LimOffDir.x != INFINITO)      //--- Adiciona o ponto no exesso lateral, se conseguir calculá-lo (há pontos suficientes?)
      {
        LimOffDir.y -= Limp;
        Limpeza.AddTail(LimOffDir);
        LimOffDir.x += 0.001;
        LimOffDir.y += Limp;
        Limpeza.AddTail(LimOffDir);
      }
      else                             //--- Não conseguiu calcular o excesso lateral, não tem pontos suficientes, insere os pontos restantes
      {
        if (PonAtual)
        {
          while (PonAtual)
          {
            PontoAtual = TerrenoAtual.GetNext(PonAtual);
            PontoAtual.y -= Limp;
            Limpeza.AddTail(PontoAtual);
          }
        }
        else                          //--- Se não tem pontos depois do exesso direito fica o ultimo ponto sobrando, é preciso incluí-lo
        {
          PontoAtual.y -= Limp;
          Limpeza.AddTail(PontoAtual);
        }
      }
    }
    else         //--- Não Tem offset direito, simplesmente passa os pontos restantes
    {
      if (PonAtual)
      {
        Ponto PontoAtual(TerrenoAtual.GetAt(PonAtual));

        while (PonAtual)
        {
          PontoAtual = TerrenoAtual.GetNext(PonAtual);
          PontoAtual.y -= Limp;
          Limpeza.AddTail(PontoAtual);
        }
      }
    }
  }
}

void CSecao::LimpaTerreno(double HLimp)
{
  for (POSITION Pos = Terreno.GetHeadPosition(); Pos; Terreno.GetNext(Pos))
  {
    Terreno.GetAt(Pos).y -= HLimp;
  }
}

bool CSecao::PodeAlterar(double x) const
{
  bool OkBTED(false);

  //--- verifica se o ponto esta dentro da seção e fora da seção tipo.

  for (POSITION Pos = Projeto.GetHeadPosition(); Pos; Projeto.GetNext(Pos))
  {
    Ponto PAtual(Projeto.GetAt(Pos));

    //  if (PAtual.Descricao == "OFFSEE" &&  x <  PAtual.x) return true;  

    if (PAtual.Descricao == "BORTEE" && x < PAtual.x) return true;

    if (PAtual.Descricao == "BORTED" && x > PAtual.x) return true;

    //   if (PAtual.Descricao == "BORTED" && x > PAtual.x) OkBTED = true;

    //   if (PAtual.Descricao == "OFFSED" &&  x <  PAtual.x  && OkBTED) return true;  
  }
  // return OkBTED;
  return false;
}

void CSecao::CalcPrimUltIntersProjTerreno(Ponto* pIntersecIni, Ponto* pIntersecFim)
{
  //--- Remove os pontos anteriores e posteriores aos Primeiros pontos de projeto (podem ser os off-sets).
  //--- Estes pontos não interessam.

  Terreno.TrimLeftMenor(Projeto.GetHead().x);
  Terreno.TrimRightMaior(Projeto.GetTail().x);

  //--- Inicializa os ponteiros dos perfís.

  POSITION ProjAtual = Projeto.GetHeadPosition(),
    TerrAtual = Terreno.GetHeadPosition();

  //--- Pega o primeiro e o segundo ponto de cada perfil para iniciar o cálculo

  Ponto NoProjAnte(Projeto.GetNext(ProjAtual)),
        NoTerrAnte(Terreno.GetNext(TerrAtual)),
        NoProjPost(Projeto.GetNext(ProjAtual)),
        NoTerrPost(Terreno.GetNext(TerrAtual));
  bool Fim(false), PrimeiraIntersecao(true);

  //---  Se veio sem o primeiro offset a primeira interseção pode estar nas 2 primeiras retas

  if (pIntersecIni->x == INFINITO)
  {
    if (CalculaIntersecao(NoProjAnte, NoProjPost, NoTerrAnte, NoTerrPost))
    {
      *pIntersecIni = Intersecao;
      pIntersecIni->Descricao = "OFFSEE";
    }
  }

  //--- rotina para o cálculo das interseções. O do interno calcula as áreas de 
  //--- interseçao a interseçao e o do externo vai verificando se chegou no fim (do projeto ou do terreno)

  do  //--- faça (até acabarem os pontos)  (de terreno e de projeto)
  {
    do  //--- Faça (até achar uma interseção)
    {
      //--- Se o ponto posterior de projeto está antes do ponto posterior de terreno,
      //--- não pode haver interseçao nela (neste ponto sabe-se que não pode haver interseçao 
      //--- entre as retas atuais, se houve o ponto anterior é a intersecão), pega a proxima 
      //--- reta de projeto, caso contrário faz o mesmo com o terreno.

      if (NoProjPost.x <= NoTerrPost.x)
      {
        NoProjAnte = NoProjPost;
        NoProjPost = Projeto.GetNext(ProjAtual);
      }
      else
      {
        NoTerrAnte = NoTerrPost;
        NoTerrPost = Terreno.GetNext(TerrAtual);
      }

      //--- Calcula a interseção com as retas atuais, se ela não foi encontrada volta ao do,
      //--- Para continuar o cáculo da figura.

    } while (!CalculaIntersecao(NoProjAnte, NoProjPost, NoTerrAnte, NoTerrPost) &&
      ((NoProjPost.x <= NoTerrPost.x && ProjAtual) || (NoProjPost.x > NoTerrPost.x && TerrAtual)));

    //--- Foi encontrada uma interseção, inicializa os pontos anteriores (eles passam a ser a interseçao encontrada
    //--- Se não encontrou e acabaram-se os pontos a interseção estara com infinito que será passado para a inicial ou final... sem problemas
    //--- As interseções que interessam são a primeira e a última.

    //--- Pode ter vindo so com o offset direito ou so com o offset esquerdo, sai quando tiver os 2.
    //--- Se não veio com o offset direito ele será a ultima interseção encontrada;

    if (Intersecao.x != INFINITO)     //--- Se saiu do while porque achou uma interseção
    {
      if (pIntersecIni->x == INFINITO)
      {
        *pIntersecIni = Intersecao;
        pIntersecIni->Descricao = "OFFSEE";          //--- Para calcular os volumes no caso dos perfis geologicos tudo se passa como se essa interseção fosse o offset esquerdo
        if (pIntersecFim->x != INFINITO) Fim = true;  //--- O offset direito já veio calculado e não precisa ser recalculado, fim.

      }
      else
      {
        *pIntersecFim = Intersecao;
        pIntersecFim->Descricao = "OFFSED";         //--- A última interseção será o offset direito;
      }

      NoTerrAnte = NoProjAnte = Intersecao;    //--- começa nova área, iguala os primeiros pontos com a interseção calculada
    }
    else Fim = true;                         //--- Senão so pode ser porque acabaram-se os pontos, fim.

  }// while(!Fim && (ProjAtual || TerrAtual));
  while (!Fim && ((NoProjPost.x <= NoTerrPost.x && ProjAtual) || (NoProjPost.x > NoTerrPost.x && TerrAtual)));
}

void CSecao::CalculaTerraplanagemNormal(double HPavim, TaludesPavim& TalPavimento, int Lado, int Tipo, Ponto* pBordo, Ponto* pBordosTerraplanagem)
{
  //--- Calcula a terraplanagem normal quer dizer não mixta.

  //--- Calcula a interseção pela fórmula (B2-B1)/(m1-m2), a altura do 
  //--- Pavimento (HPavim) é igual a B2-B1. 

  double x(HPavim / (TalPavimento.Rampas[Lado * 2 + Tipo] - (pBordo + Lado)->i)), y(x * TalPavimento.Rampas[Lado * 2 + Tipo]);

  //--- Calcula os Bordos de tarraplanagem.

  (pBordosTerraplanagem + Lado)->x = x + (pBordo + Lado)->x;
  (pBordosTerraplanagem + Lado)->y = y + (pBordo + Lado)->y;

  //--- Insere os bordos na lista dos pontos de terraplanagem.

  Projeto.InsereOrdenada(BordosTerraplanagem[Lado]);

  //--- Se o talude é de aterro o bordo da pista não faz parte da terraplanagem,
  //--- Caso Contrario, (se for corte) volta o bordo para a pavimentação, somando 
  //--- a altura do pavimento.
  //--- Os bordos estão com os valores da secao-tipo (cota de pavimentacao) e o 
  //--- projeto esta com as cotas de terraplanagem.

  if (Tipo == ATERRO)
  {
    Projeto.ExcluiPonto(pBordo[Lado].x);    //--- Exclui o bordo da pista.

    //--- insere o bordo na lista da pavimentação. No caso de corte o bordo calculado não 
    //--- faz parte dos pontos de pavimentacao.

    Pavimento.InsereOrdenada(BordosTerraplanagem[Lado]);
  }
}
