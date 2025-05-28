#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "cponto.h"
#include <string>
#include "mat.h"
#include "supercstring.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "Projeto.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include <vector>
#include <iostream>
#include "csstack.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1
#define ESQUERDO   0
#define DIREITO    1
#define MENOR     -1
#define MAIOR      1
#define IGUAL      0
#define TRAS      -1
#define FRENTE     1
#define X          0
#define Y          1
#define ESTPCV     1
#define ESTPIV     2
#define ESTPTV     3

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação do nó da lista.

CurVertical::CurVertical(double x, double y,int Ig,double y1,double y2,CString Desc,double pI0): PIV(x,y,Ig,Desc),Y1(y1),Y2(y2),Erro(0),
  I1(INFINITO),I2(INFINITO),E(0.0),PCV(x-(y1 == INFINITO ? INFINITO : y1),y,Ig,"PCV"),PTV(x+(y2 == INFINITO ? INFINITO : y2),y,Ig,"PTV"),I0(pI0){}

double CurVertical::e(double Estaca,int Lado)
{
  double YCalc(Y1),xcalc(Estaca - PCV.EstVirtual);  //--- Inicializa, assume o Lado como Anterior.

  if (Lado == POSTERIOR) 	//--- Se o lado for o posterior recalcula dados.
  {
    YCalc = Y2;
    xcalc = PTV.EstVirtual - Estaca;
  }
  else xcalc = PCV.EstVirtual - Estaca;

  return E * (pow(xcalc,2) / pow(YCalc,2));   //--- Calcula o e do ponto.
}

inline double CurVertical::CalculaE()
{
  if (Y1 < 0.00 || Y1 == INFINITO  || Y2 < 0.00 || Y2 == INFINITO || I2 == INFINITO || I1 == INFINITO) return E = 0.0;
  else
  {
    E = ((I2 - I1) * Y1 * Y2) / (2 * (Y1 + Y2));
    
    //--- Calcula o ponto onde Dx = 0

    if((I1 > 0.0 && I2 < 0.0) || (I1 < 0.0 && I2 > 0.0))
    {
      double Ip((Y1 * I1 + Y2 * I2) / (Y1 + Y2));

      if(fabs(I1) > fabs(I2)) 
      {
        I0 = (I2 * Y2) / (I2 - Ip);
        I0 = PTV.EstVirtual - I0; 
      }
      else 
      {
        I0 = (I1 * Y1) / (I1 - Ip);
        I0 += PCV.EstVirtual; 
      }
    }
  }

  return E;
}

inline double CurVertical::CalculaCotaPCV()
{
  PCV.Cota = Y1 < 0.00 || Y1 == INFINITO ? INFINITO : PIV.Cota - I1 * Y1 ; 

  return PCV.Cota;
}

inline double CurVertical::CalculaCotaPTV()
{
  PTV.Cota = Y2 < 0.00 || Y2 == INFINITO ? INFINITO : PIV.Cota + I2 * Y2 ; 

  return PTV.Cota;
}

bool CurVertical::operator==(const CurVertical& RHS)
{
  return PCV == RHS.PCV && PIV == RHS.PIV &&  PTV == RHS.PTV && 
    (Matem::ComparaDoubles(Y1,RHS.Y1,3) == 0) && (Matem::ComparaDoubles(Y2,RHS.Y2,3) == 0) &&
    (Matem::ComparaDoubles(I1,RHS.I1,3) == 0) && (Matem::ComparaDoubles(E,RHS.E,3) == 0);
}

int CurVertical::Consiste()
{
  if(PIV.EstVirtual == INFINITO ) Erro = 1;
  else if(Y != 0 && fabs(PCV.EstVirtual + Y1 - PIV.EstVirtual) > 1e-5) Erro = 2;
  else if( fabs(PTV.EstVirtual - Y2 - PIV.EstVirtual) > 1e-5) Erro = 3;
  else if(I1 == INFINITO) Erro = 4;
  else if(I2 == INFINITO) Erro = 5;
  else if(PCV.Cota == INFINITO) Erro = 6;
  else if(PIV.Cota == INFINITO) Erro = 7;
  else if(PTV.Cota == INFINITO) Erro = 8;
  else if(E == INFINITO) Erro = 9;

  return Erro;
}

//----------------------------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas lstas são dinamicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

CCurVerticais::CCurVerticais(CString& NomeArquivo) : CArqCurVerticais(NomeArquivo),Atual(NULL),
  LimiteEsq(INFINITO),LimiteInf(INFINITO),LimiteSup(-INFINITO),LimiteDir(-INFINITO),Erro(0)
{
  if (Consiste()) throw -1;

  if (IsEmpty()) return;  //--- Lista vazia? retorna.

  CurVertical CurvaAtual;

  Atual = GetHeadPosition();  //--- Atual = Cabeca da lista.
  ArqCurVerticais NoArqAtual= GetNext(Atual);

  //--- Cria a lista de PIVs calculados. 
  //--- Inicializa a lista criando o primeiro nó, neste nó não há cálculo de I1 e E.

  ittlstwstr It(NoArqAtual.ListaCampos.begin());
  CString Estaca,Cota,Ig;

  if(NoArqAtual.ListaCampos.size() == 5)
  {
    Estaca = It->c_str();
    It++;
    Ig = It->c_str();
    It++;
    Cota = It->c_str();
    CurvaAtual.PIV.Constroi(Estaca,Cota,Ig);
    It++;
    CurvaAtual.Y1 = atof(It->c_str());
    It++;
    CurvaAtual.Y2 = atof(It->c_str());
    CurvaAtual.PCV.Constroi(CurvaAtual.PIV.EstVirtual - CurvaAtual.Y1,CurvaAtual.PIV.Cota);
    CurvaAtual.PTV.Constroi(CurvaAtual.PIV.EstVirtual + CurvaAtual.Y2,CurvaAtual.PIV.Cota);
    LCurvasVerticais.AddTail(CurvaAtual);
  }

  AtualizaLimites(CurvaAtual);

  //--- Para cada os nos restantes da lista de PIVS lidos cria um nó na lista
  //--- de PIVS calculados, calculando od dados do PIV.

  while(Atual) 
  {
    NoArqAtual = GetNext(Atual);
    It = NoArqAtual.ListaCampos.begin();

    if(NoArqAtual.ListaCampos.size() == 5)
    {
      Estaca = It->c_str();
      It++;
      Ig = It->c_str();
      It++;
      Cota = It->c_str();
      CurvaAtual.PIV.Constroi(Estaca,Cota,Ig); 
      It++;
      CurvaAtual.Y1 = atof(It->c_str());
      It++;
      CurvaAtual.Y2 = atof(It->c_str());
      CurVertical& CurvaAnterior = LCurvasVerticais.GetTail();
      CurvaAtual.I2 = CurvaAtual.I1 = CurvaAnterior.I2 = (CurvaAtual.PIV.Cota - CurvaAnterior.PIV.Cota) / (CurvaAtual.PIV - CurvaAnterior.PIV);
      CurvaAnterior.PTV.Cota = CurvaAnterior.Y2 * CurvaAnterior.I2 + CurvaAnterior.PIV.Cota;
      CurvaAnterior.CalculaE();
      CurvaAtual.PCV.Constroi(CurvaAtual.PIV.EstVirtual - CurvaAtual.Y1,CurvaAtual.Y1 * -CurvaAtual.I1 + CurvaAtual.PIV.Cota);
      CurvaAtual.PTV.Constroi(CurvaAtual.PIV.EstVirtual + CurvaAtual.Y2,CurvaAtual.PIV.Cota);

      LCurvasVerticais.AddTail(CurvaAtual);    

      AtualizaLimites(CurvaAtual);
    }
  }

  CurVertical& PrimPIV =  LCurvasVerticais.GetHead();

  PrimPIV.I1 = PrimPIV.I2;

  RemoveAll();  //--- Remove os dados lidos, ficam só os calculados.

  Consiste();   //--- Seta OK = true se tudo OK;
}

CCurVerticais::~CCurVerticais()
{
}

double CCurVerticais::CalculaCota(CEstaca& Estaca) 
{
  double E,IAnte,IPost;

  return CalculaCota(Estaca,E,IAnte,IPost);
}

double CCurVerticais::CalculaCota(CEstaca& Estaca,double &E,double& IAnte,double& IPost) 
{
  //--- Calcula a cota de uma determinada estaca

  if (!Atual) Atual = LCurvasVerticais.GetHeadPosition();
  if (!Atual) return Estaca.Cota = INFINITO;

  //--- Busca por um determinado PIV.
  //--- Se o atual for maior, busca para tras , senão para frente

  BOOL Frente(LCurvasVerticais.GetAt(Atual).PIV <= Estaca); 

  for	( ; Atual && (Frente ? LCurvasVerticais.GetAt(Atual).PIV < Estaca : LCurvasVerticais.GetAt(Atual).PIV > Estaca) ;
                    Frente ? LCurvasVerticais.GetNext(Atual) : LCurvasVerticais.GetPrev(Atual));

    if (!Atual) return Estaca.Cota = INFINITO;  //--- A Estaca está fora do trecho projetado

  CurVertical NoAtual = LCurvasVerticais.GetAt(Atual);

  //--- Calcula a cota.

  if (Frente)
  {
    //--- Calcula a cota no greide reto.

    Estaca.Cota = -NoAtual.I1 * (NoAtual.PIV.EstVirtual - Estaca.EstVirtual) + NoAtual.PIV.Cota; //-- Cota no greide reto.

    if (Atual == LCurvasVerticais.GetHeadPosition()) return Estaca.Cota;  //-- Se = primeiro piv não tem "e'...

    //---	Se a estaca está na parábola, calcula o "e", note que a estaca pode estar na parabola
    //--- atual (PCV ao PIV) ou Anterior (PIV ao PTV)
    //--- Atualiza as variaveis que vieram como parâmetros.

    LCurvasVerticais.GetPrev(Atual);
    CurVertical &No2 = LCurvasVerticais.GetAt(Atual);

    if (Estaca.EstVirtual  > NoAtual.PCV.EstVirtual) 
    {
      Estaca.Cota += NoAtual.e(Estaca.EstVirtual,ANTERIOR);
      E = NoAtual.E;
    }
    else 
    {
      if (Estaca.EstVirtual < No2.PTV.EstVirtual) 
      {
        Estaca.Cota += No2.e(Estaca.EstVirtual,POSTERIOR);
        E = No2.E;
      }
    }
  }
  else  //--- Procurou para tras...
  {
    //--- Calcula a cota no greide reto.

    Estaca.Cota = NoAtual.I2 * (Estaca.EstVirtual - NoAtual.PIV.EstVirtual) + NoAtual.PIV.Cota;

    if (Atual == LCurvasVerticais.GetTailPosition()) return Estaca.Cota;  //-- Se = ultimo piv não tem "e'...

    //---	Se a estaca está na parábola, calcula o "e", note que a estaca pode estar na parabola
    //--- atual (PIV ao PTV) ou na posterior (PCV ao PIV)
    //--- Atualiza as variaveis que vieram como parâmetros.

    LCurvasVerticais.GetNext(Atual);
    CurVertical &No2 = LCurvasVerticais.GetAt(Atual);

    if (Estaca.EstVirtual < NoAtual.PTV.EstVirtual) 
    {
      Estaca.Cota += NoAtual.e(Estaca.EstVirtual,POSTERIOR);
      E = NoAtual.E;
    }
    else 
    {
      if (Estaca.EstVirtual > No2.PCV.EstVirtual) 
      {
        Estaca.Cota += No2.e(Estaca.EstVirtual,ANTERIOR);
        E= No2.E;
      }
    }
  }
  return Estaca.Cota;
}

int CCurVerticais::Compara(class CurVertical &No1,class CurVertical &No2)
{
  if (No1.PIV < No2.PIV) return MENOR;
  else if (No1.PIV > No2.PIV) return MAIOR;
  else return IGUAL; 
}

void CCurVerticais::InsOrdenada(class CurVertical &No)
{
  PosicionaNoMaisProximo(No.PIV);

  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  if (!Atual) Atual = LCurvasVerticais.GetHeadPosition();
  if (!Atual) 
  {
    No.Y1 = No.Y2 = 0.0;
    Atual = LCurvasVerticais.AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  }
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir  e for para inserir,
    //--- insere o nó depois do atual.

    if ((Res = Compara(LCurvasVerticais.GetAt(Atual),No)) == MAIOR)
    {
      LCurvasVerticais.GetPrev(Atual); 

      while (Atual && (Res = Compara(LCurvasVerticais.GetAt(Atual),No)) == MAIOR) LCurvasVerticais.GetPrev(Atual);

      if(!Atual)
      {
        No.Y1 = No.Y2 = 0.0;
        Atual = LCurvasVerticais.AddHead(No);
      }
      else Atual = LCurvasVerticais.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        while (Atual && (Res = Compara(LCurvasVerticais.GetAt(Atual),No)) == MENOR) LCurvasVerticais.GetNext(Atual);

        if(!Atual)
        {
          No.Y1 = No.Y2 = 0.0;
          Atual = LCurvasVerticais.AddTail(No);
        }
        else Atual = LCurvasVerticais.InsertBefore(Atual,No);
      }
    }
  }
}

int CCurVerticais::PosicionaNoMaisProximo(CEstaca Estaca,int Direcao,bool DESDEOINICIO)
{
  //-- FORÇADO DESDEOINICIO , não estava funcionando ok da posição atual;

  DESDEOINICIO = true;

  if (!Atual || DESDEOINICIO) Atual = LCurvasVerticais.GetHeadPosition();
  if (!Atual) return MENOR;

  if (!(Direcao = Estaca < LCurvasVerticais.GetAt(Atual).PIV ? TRAS : Estaca > LCurvasVerticais.GetAt(Atual).PIV ? FRENTE : FALSE)) return IGUAL;

  if (Direcao == FRENTE)
  {
    LCurvasVerticais.GetNext(Atual);

    while(Atual && LCurvasVerticais.GetAt(Atual).PIV < Estaca) LCurvasVerticais.GetNext(Atual);

    if (Atual)
    {
      double Dist1(LCurvasVerticais.GetAt(Atual).PIV - Estaca);

      POSITION Prev(Atual);

      LCurvasVerticais.GetPrev(Prev);

      if (Prev)
      {
        double Dist2(Estaca - LCurvasVerticais.GetAt(Prev).PIV);

        if (Dist2 < Dist1) Atual = Prev;
      }
    }
    else Atual = LCurvasVerticais.GetTailPosition();
  }
  else
  {
    LCurvasVerticais.GetPrev(Atual);

    while(Atual && LCurvasVerticais.GetAt(Atual).PIV > Estaca) LCurvasVerticais.GetPrev(Atual);

    if (Atual)
    {
      double Dist1(Estaca -  LCurvasVerticais.GetAt(Atual).PIV);

      POSITION Next(Atual);

      LCurvasVerticais.GetNext(Next);

      if (Next)
      {
        double Dist2(Estaca - LCurvasVerticais.GetAt(Next).PIV);

        if (Dist2 < Dist1) Atual = Next;
      }
    }
    else Atual = LCurvasVerticais.GetHeadPosition();
  }

  if (LCurvasVerticais.GetAt(Atual).PIV > Estaca) return MAIOR;
  if (LCurvasVerticais.GetAt(Atual).PIV < Estaca) return MENOR;
  return IGUAL;
}

bool CCurVerticais::VerfInsercao(const CurVertical& CurvaInserir)
{
  if (LCurvasVerticais.GetCount() == 0 || !PosicionaNoMaisProximo(CurvaInserir.PIV.EstVirtual,FRENTE)) return true;

  const CurVertical &NoAtual = LCurvasVerticais.GetAt(Atual);

  if((CurvaInserir.PCV > NoAtual.PCV && CurvaInserir.PCV < NoAtual.PTV) || (CurvaInserir.PTV > NoAtual.PCV && CurvaInserir.PTV < NoAtual.PTV))
    return false;

  if(NoAtual.PIV > CurvaInserir.PIV) LCurvasVerticais.GetNext(Atual);
  else LCurvasVerticais.GetPrev(Atual);

  if(!Atual) return true;

  const CurVertical &OutroNo = LCurvasVerticais.GetAt(Atual);

  if((CurvaInserir.PCV > OutroNo.PCV && CurvaInserir.PCV < OutroNo.PTV) || (CurvaInserir.PTV > OutroNo.PCV && CurvaInserir.PTV < OutroNo.PTV)) return false;

  return true;
}

double CCurVerticais::PegaPontoNotavel(CEstaca &Anterior)
{
  //--- Retorna o primeiro ponto notavel da curva mais proxima de Anterior
  //--- Retorna em anterior o valor encontrado bem como o tipo da estaca.

  if (!PosicionaNoMaisProximo(Anterior.EstVirtual,FRENTE)) return -INFINITO;

  CurVertical &NoAtual = LCurvasVerticais.GetAt(Atual);

  if (NoAtual.PCV > Anterior) Anterior.Constroi(NoAtual.PCV.EstVirtual,INFINITO,0,CString("PCV"));
  else if (NoAtual.PIV > Anterior) Anterior.Constroi(NoAtual.PIV.EstVirtual,INFINITO,0,CString("PIV"));
  else if (NoAtual.PTV > Anterior) Anterior.Constroi(NoAtual.PTV.EstVirtual,INFINITO,0,CString("PTV"));

  return Anterior.EstVirtual;
}

void CCurVerticais::PegaPontosNotaveis(CList<CEstaca,CEstaca&>& PontosNotaveis) 
{
  Atual = LCurvasVerticais.GetHeadPosition();
  CurVertical CurvaAtual;

  while(Atual)
  {
    CurvaAtual = LCurvasVerticais.GetAt(Atual);
    if (CurvaAtual.Y1 != 0.0)
    {
      PontosNotaveis.AddTail(CEstaca(CurvaAtual.PCV,(CString) "PCV"));
      PontosNotaveis.AddTail(CEstaca(CurvaAtual.PIV,(CString) "PIV"));
      PontosNotaveis.AddTail(CEstaca(CurvaAtual.PTV,(CString) "PTV"));
    }
    else PontosNotaveis.AddTail(CEstaca(CurvaAtual.PIV,(CString) "PIV"));

    LCurvasVerticais.GetNext(Atual);
  }
}

void CCurVerticais::PegaEstacasObrigatorias(CList<CEstaca,CEstaca&>& EstObrig,double Equidis,CProjeto* ProjAtual)
{
  CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
 // MainFrame->m_ProjetosBox.GetLBText(0,MainFrame->m_sNomeComp);
 // CProjeto* ProjAtual = MainFrame->AchaProjetoNoVetor();

  Atual = LCurvasVerticais.GetHeadPosition();
  CurVertical CurvaAtual;

  while(Atual)
  {
    CurvaAtual = LCurvasVerticais.GetAt(Atual);
    if (CurvaAtual.Y1 != 0.0)
    {
      for (double Est = CurvaAtual.PCV.EstVirtual + Equidis ; Est < CurvaAtual.PTV.EstVirtual ; Est += Equidis)
      {
        int NumIg(0); 
        double Int,EstReal(Est),//EstReal = ProjAtual->Igualdades.BuscaEstacaReal(Est,NumIg)+1e-5,

        EstInt = EstReal / Equidis;
        modf(EstInt,&Int);
        EstObrig.AddTail((CEstaca) (Int * Equidis));  
      }
    }
    LCurvasVerticais.GetNext(Atual);
  }
}

POSITION CCurVerticais::PosicionaPontoNotavel(double x,double y,double Toleranciax,double Toleranciay,int& Tipo)
{
  //--- Verifica se o ponto x,y é um dos pontos notáveis (PCV,PIV ou PTV) com a tolerância dada

  int Res(PosicionaNoMaisProximo(x));     //--- Posiciona no PIV com o x mais próximo

  if (!Atual) return NULL;

  CEstaca* EstacaAtual =  &LCurvasVerticais.GetAt(Atual).PIV;

  //--- Verifica o PIV

  POSITION P(EstacaAtual->EstVirtual + Toleranciax > x && EstacaAtual->EstVirtual - Toleranciax < x  &&
    EstacaAtual->Cota + Toleranciay > y && EstacaAtual->Cota - Toleranciay < y ? Atual : NULL);
  if (P) 
  {
    Tipo = ESTPIV;
    return P;  //--- Esta no PIV!!!
  }
  else
  {
    //--- Verifica o PCV ou PTV 

    if (*EstacaAtual < x)   //--- O PIV está antes, então verifica o PTV desta e o PCV da posterior
    {
      EstacaAtual = &LCurvasVerticais.GetAt(Atual).PTV;

      P = EstacaAtual->EstVirtual + Toleranciax > x && EstacaAtual->EstVirtual - Toleranciax < x &&
        EstacaAtual->Cota + Toleranciay > y && EstacaAtual->Cota - Toleranciay < y ? Atual : NULL;

      if (P) 
      {
        Tipo = ESTPTV;
        return P;  //--- Está no PTV .
      }

      GetNext(Atual);   //--- Passa para o PIV posterior

      if (Atual)     //--- Tem PIV Posterior??  
      {
        EstacaAtual = &LCurvasVerticais.GetAt(Atual).PCV;

        Tipo = ESTPCV;

        return EstacaAtual->EstVirtual + Toleranciax > x && EstacaAtual->EstVirtual - Toleranciax < x &&
          EstacaAtual->Cota + Toleranciay > y && EstacaAtual->Cota - Toleranciay < y ? Atual : NULL;
      }
    }
    else       //--- Senão procura no PCV deste e no PTV anterior.
    {
      EstacaAtual = &LCurvasVerticais.GetAt(Atual).PCV;

      P = EstacaAtual->EstVirtual + Toleranciax > x && EstacaAtual->EstVirtual - Toleranciax < x &&
        EstacaAtual->Cota + Toleranciay > y && EstacaAtual->Cota - Toleranciay < y ? Atual : NULL;

      if (P) 
      {
        Tipo = ESTPCV;
        return P;  //--- Está no PCV .
      }

      GetPrev(Atual);

      if (Atual)     //--- Tem PIV anterior?? Verifica o PTV dele.  
      {
        EstacaAtual = &LCurvasVerticais.GetAt(Atual).PTV;

        Tipo = ESTPTV;

        return EstacaAtual->EstVirtual + Toleranciax > x && EstacaAtual->EstVirtual - Toleranciax < x &&
          EstacaAtual->Cota + Toleranciay > y && EstacaAtual->Cota - Toleranciay < y ? Atual : NULL;
      }
    }
  }
  return NULL;   //--- Não achou nada!!!
}


bool CCurVerticais::AlteraPIV(const CEstaca& Est, POSITION P)
{
  POSITION PRasc(P),                                              //--- Salva o ponteiro para o PIV a alterar.
           PPrev(NULL),PProx(NULL);                               //--- Ponteiros para o anterior e o proximo tb serão salvos.
  CurVertical C(LCurvasVerticais.GetPrev(PRasc)),CAnte,CPoste;    //--- Pega o PIV Pulando para trás.
  double Diff(Est.EstVirtual - C.PIV.EstVirtual);

  C.PIV = Est;         //--- Altera o Rasc PIV Atual, é preciso criar a estaca est porque a classe CEstaca 
                       //--- gerencia automaticamente as igualdades. NUM GERENCIA NÃO, NÃO NESTE CASO!!DA|||| 

  if (PRasc)           //--- Se tem PIV Anterior...
  {
    PPrev = PRasc;     //--- Salva o ponteiro para o anterior;

    //--- Recalcula o I1 do atual e o I2 do anterior.

    CAnte = LCurvasVerticais.GetAt(PRasc);

    if(CAnte.PTV.EstVirtual > (C.PCV.EstVirtual + Diff)) 
    {
      monolog.mensagem(34,"");
      return false;
    }

    C.I1 = CAnte.I2 = (C.PIV.Cota -  CAnte.PIV.Cota) / (C.PIV - CAnte.PIV + 1e-10);  //--- +1 e-10 Evita o division by zero...

    //--- Esta inserindo o último PIV Deszera o Y anterior...exceto se for o primeiro PIV

    if(P == LCurvasVerticais.GetTailPosition() && CAnte.PIV != LCurvasVerticais.GetHead().PIV) 
    {
      CAnte.Y1 = CAnte.Y2 = 1.0;  
      CAnte.PCV.EstVirtual = CAnte.PCV.EstReal = CAnte.PIV.EstVirtual - CAnte.Y1;
      CAnte.PTV.EstVirtual = CAnte.PTV.EstReal = CAnte.PIV.EstVirtual + CAnte.Y2;
      CAnte.PCV.Cota = CAnte.Y1 * -CAnte.I1 + CAnte.PIV.Cota;            //--- Neste caso é preciso ajustar o PCV, o PTV será ajustado em seguida      
    }

    //--- Ajusta o PTV do anterior

    if (CAnte.Y2 != INFINITO && CAnte.Y2 >= 0.0)
    {
      //  CAnte.PTV.EstReal = CAnte.PTV.EstVirtual = CAnte.PIV.EstVirtual + CAnte.Y2;
      CAnte.PTV.Cota = CAnte.Y2 * CAnte.I2 + CAnte.PIV.Cota;
    }

    CAnte.CalculaE();  //--- Recalcula o E da curva anterior
  }

  PRasc = P;
  LCurvasVerticais.GetNext(PRasc);   //--- Pula para a frente... 

  if (PRasc)        //--- Se tem PIV Posterior...
  {
    PProx = PRasc;

    //--- Recalcula o I2 do atual e o I1 do posterior.

    CPoste = LCurvasVerticais.GetAt(PRasc);

    if(CPoste.PCV.EstVirtual < (C.PTV.EstVirtual + Diff)) 
    {
      monolog.mensagem(34,"");
      return false;
    }

    C.I2 = CPoste.I1 = (CPoste.PIV.Cota - C.PIV.Cota) / (CPoste.PIV - C.PIV + 1e-10);

    //--- Ajusta o PCV do posterior

    if (CPoste.Y1 != INFINITO && CPoste.Y1 > 0.0)
    {
      //CPoste.PCV.EstReal = CPoste.PCV.EstVirtual = CPoste.PIV.EstVirtual - CPoste.Y1;
      CPoste.PCV.Cota = CPoste.Y1 * -CPoste.I1 + CPoste.PIV.Cota;
    }

    CPoste.CalculaE();   //--- Recalcula o E
  }

  //--- PIV Atual, recalcula o PCV, o PTV e o E

  if (C.Y1 != INFINITO && C.Y1 >= 0.0)   		//--- PCV
  {

    C.PCV.EstReal = C.PCV.EstVirtual = C.PIV.EstVirtual - C.Y1;
    C.PCV.Cota = C.Y1 * -C.I1 + C.PIV.Cota;
  }

  if (C.Y2 != INFINITO && C.Y2 >= 0.0)    		//--- PTV
  {
    C.PTV.EstReal = C.PTV.EstVirtual = C.PIV.EstVirtual + C.Y2;
    C.PTV.Cota = C.Y2 * C.I2 + C.PIV.Cota;
  }

  C.CalculaE();   //--- Recalcula o E 

  if(LCurvasVerticais.GetCount() > 2 && (fabs(C.I1) != INFINITO && fabs(C.I1) > 10.0) || (fabs(C.I2) != INFINITO && fabs(C.I2) > 10.0)) 
  {
    monolog.mensagem(35,"");
    return false;
  }

  //--- Efetiva as alterações

  PRasc = P;
  LCurvasVerticais.GetPrev(PRasc) = C;
  if(PRasc) LCurvasVerticais.GetAt(PRasc) = CAnte;
  PRasc = P;
  LCurvasVerticais.GetNext(PRasc);
  if(PRasc) LCurvasVerticais.GetAt(PRasc) = CPoste;

  return true;
}

void CCurVerticais::RemovePIV(POSITION Pos)
{
  POSITION PAnte(Pos),PPost(Pos);

  LCurvasVerticais.GetPrev(PAnte);   //--- Posiciona o anterior
  LCurvasVerticais.GetNext(PPost);   //--- Posiciona o posterior.

  LCurvasVerticais.RemoveAt(Pos);    //--- Remove o PIV.

  if (PAnte)    //--- Se tem PIV Anterior ajusta-o
  {
    Atual = PAnte; //--- Aproveita e atualiza o atual da lista.

    CurVertical &CAnte = LCurvasVerticais.GetAt(PAnte);

    if (PPost)  //--- Se tem posterior Tembem...
    {
      //--- Recalcula o I2 do anterior e o I1 do proximo.

      CurVertical &CPoste = LCurvasVerticais.GetAt(PPost);

      CAnte.I2 = CPoste.I1 = (CPoste.PIV.Cota -  CAnte.PIV.Cota) / (CPoste.PIV - CAnte.PIV + 1e-10);  //--- +1 e-10 Evita o division by zero...

      //--- Ajusta o PTV do anterior

      if (CAnte.Y2 != INFINITO && CAnte.Y2 > 0.1)
      {
        CAnte.PTV.Cota = CAnte.Y2 * CAnte.I2 + CAnte.PIV.Cota;
        CAnte.CalculaE();  //--- Recalcula o E da curva anterior
      }

      //--- Ajusta o PCV Posterior

      if(CPoste.Y1 != INFINITO && CPoste.Y1 > 0.1)
      {
        CPoste.PCV.Cota = CPoste.Y1 * -CPoste.I1 + CPoste.PIV.Cota;
        CPoste.CalculaE();
      }
    }
    else   //--- Não tem posterior...
    {
      CAnte.I2 = INFINITO;
      CAnte.Y1 = CAnte.Y2 = CAnte.E = 0.0;
    }
  }
  else  //--- Não tem anterior.
  {
    if (PPost)   //--- se tem posterior...
    {
      Atual = PPost; //--- AProveita e atualiza o atual da lista.

      CurVertical &CPoste = LCurvasVerticais.GetAt(PPost);

      CPoste.I2 = INFINITO;
      CPoste.Y1 = CPoste.Y2 = CPoste.E = 0.0;
    }
    else Atual = NULL; //--- A lista ficou vazia, o último piv foi removido.
  }
}

void CCurVerticais::AlteraI1(const CurVertical& PIV,double Valor, POSITION No)
{
  PosicionaNoMaisProximo(PIV.PIV); 
  No = Atual;
  if (!No) return;

  POSITION PAnte(No);

  LCurvasVerticais.GetPrev(PAnte);   //--- Posiciona PAnte no anterior

  if (!PAnte) return;                    //--- primeiro no da lista, não existe anterior, retorna

  CurVertical& CAtual = LCurvasVerticais.GetAt(No);   //--- Pega o PIV Atual
  CurVertical& CAnte = LCurvasVerticais.GetAt(PAnte); //--- Pega o Anterior

  if(fabs(Valor - CAtual.I1) > 0.03)
  {
    monolog.mensagem(33," (I1)");
    return;
  }

  POSITION PAnteAnte(PAnte);
  LCurvasVerticais.GetPrev(PAnteAnte);                //--- Posiciona no anterior do anterior.

  //--- simula uma alteração no piv anterior e chama a rotina AlteraPIV.

  CAnte.PIV.Cota = CAtual.PIV.Cota - (CAtual.PIV.EstVirtual - CAnte.PIV.EstVirtual + 1e-10) * Valor;  

  //	Ponto PIVAnterior(CAnte.PIV.EstVirtual,CAtual.PIV.Cota - (CAtual.PIV.EstVirtual - CAnte.PIV.EstVirtual + 1e-10) * atof(Valor));  
  AlteraPIV(CAnte.PIV,PAnte);
}

void CCurVerticais::AlteraI2(const CurVertical& PIV,double Valor, POSITION No)
{
  PosicionaNoMaisProximo(PIV.PIV); 
  No = Atual;
  if (!No) return;

  POSITION PPost(No);

  LCurvasVerticais.GetNext(PPost);   //--- Posiciona PAnte no anterior

  if (!PPost) return;                    //--- primeiro no da lista, não existe anterior, retorna

  CurVertical& CAtual = LCurvasVerticais.GetAt(No);   //--- Pega o PIV Atual
  CurVertical& CPost = LCurvasVerticais.GetAt(PPost); //--- Pega o Anterior

  if(fabs(CAtual.I2 - Valor) > 0.03)
  {
    monolog.mensagem(33," (I2)");
    return;
  }

  //--- simula a alteração no piv anterior e chama a rotina AlteraPIV.

  CPost.PIV.Cota = CAtual.PIV.Cota - (CAtual.PIV.EstVirtual - CPost.PIV.EstVirtual + 1e-10) * Valor;  

  //	Ponto PIVPosterior(CPost.PIV.EstVirtual,CAtual.PIV.Cota - (CAtual.PIV.EstVirtual - CPost.PIV.EstVirtual + 1e-10) * atof(Valor));  
  AlteraPIV(CPost.PIV,PPost);
}

void CCurVerticais::AlteraCota(const CEstaca& EstPIV,POSITION No)
{
  PosicionaNoMaisProximo(EstPIV); 
  No = Atual;
  if (!No) return;

  CurVertical& CAtual(LCurvasVerticais.GetAt(No));

  CEstaca Est(CAtual.PIV);
  Est.Cota = EstPIV.Cota;

  AlteraPIV(Est,No);
}

void CCurVerticais::AlteraEstaca(double Valor,int Ig, POSITION No)
{
  if (!No) No = Atual;
  if (!No) return;

  CurVertical& CAtual = LCurvasVerticais.GetAt(No);

  CEstaca Est(Valor,INFINITO,Ig);      //--- constroi a estaca para poder pegar o valor da estaca virtual

  //Ponto Estaca(Est.EstVirtual,CAtual.PIV.Cota);

  AlteraPIV(Est,No);   //--->>>>>>>>>>>>> mudar para receber uma estaca, não um ponto!!!!
}

void CCurVerticais::AlteraEstaca(const CEstaca &Est,const CEstaca& EstAnterior, POSITION No)
{
  PosicionaNoMaisProximo(EstAnterior,true);
  No = Atual;
  if (!No) return;

  CurVertical& CAtual = LCurvasVerticais.GetAt(No);

  //	Ponto Estaca(Est.EstVirtual,CAtual.PIV.Cota);

  if(!AlteraPIV(Est,No))   //--->>>>>>>>>>>>> mudar para receber uma estaca, não um ponto!!!!
    monolog.mensagem(34,"");
}

bool CCurVerticais::AlteraY1(const CEstaca& EstPIV,double Valor,int Tipo, POSITION No)
{
  PosicionaNoMaisProximo(EstPIV);
  No = Atual;
  if(!No || No == LCurvasVerticais.GetHeadPosition() || No == LCurvasVerticais.GetTailPosition()) return false;

  if(LCurvasVerticais.GetAt(No).PIV.EstVirtual < EstPIV.EstVirtual) LCurvasVerticais.GetNext(No);

  CurVertical& CAtual(LCurvasVerticais.GetPrev(No)),CAnt(LCurvasVerticais.GetAt(No));

  double ValorY1(Tipo == CURVER_VALOR_Y ? Valor : CAtual.PIV.EstVirtual - EstPIV.EstVirtual);

  if (ValorY1 < 0.0) return false;      //--- Y negativo??? retorna.

  if(CAtual.PIV.EstVirtual - ValorY1 < CAnt.PTV.EstVirtual)
  {
    monolog.mensagem(34,"");
    return false;
  }

  CAtual.Y1 = ValorY1;
  CAtual.PCV.AlteraValor(CAtual.PIV.EstVirtual - CAtual.Y1);
  CAtual.CalculaCotaPCV();
  CAtual.CalculaE();

  return true;
}

bool CCurVerticais::AlteraY2(const CEstaca& EstPIV,double Valor,int Tipo,POSITION No)
{
  PosicionaNoMaisProximo(CEstaca(EstPIV));
  No = Atual;
  if(!No || No == LCurvasVerticais.GetHeadPosition() || No == LCurvasVerticais.GetTailPosition()) return false;

  if(LCurvasVerticais.GetAt(No).PIV.EstVirtual > EstPIV.EstVirtual) LCurvasVerticais.GetPrev(No);

  CurVertical& CAtual(LCurvasVerticais.GetNext(No)),CPost(LCurvasVerticais.GetAt(No));

  double ValorY2(Tipo == CURVER_VALOR_Y ?  Valor : EstPIV.EstVirtual - CAtual.PIV.EstVirtual);

  if (ValorY2 < 0.1)  return false;      //--- Y negativo??? retorna false.

  if(CAtual.PIV.EstVirtual + ValorY2 > CPost.PTV.EstVirtual)
  {
    monolog.mensagem(34,"");
    return false;
  }

  CAtual.Y2 = ValorY2;
  CAtual.PTV.AlteraValor(CAtual.PIV.EstVirtual + CAtual.Y2);
  CAtual.CalculaCotaPTV();
  CAtual.CalculaE();

  return true;
}

BOOL CCurVerticais::Serialize()
{
  RemoveAll();

  //--- Cria a nova lista com os novos dados  Grava o arquivo

  Atual = LCurvasVerticais.GetHeadPosition();

  if (Atual)
  {
    while(Atual) 
    {
      CurVertical& CurvaAtual = LCurvasVerticais.GetNext(Atual);

      CString Cota,Y1,Y2;

      //--- PIV e Igualdade já são strings...

      Cota.Format("%lf",CurvaAtual.PIV.Cota);
      Y1.Format("%lf",CurvaAtual.Y1);
      Y2.Format("%lf",CurvaAtual.Y2);

      AddTail(ArqCurVerticais(CurvaAtual.PIV,CurvaAtual.PIV.Igualdade,Cota,Y1,Y2));
    }
  }

  GravarArquivo();  //--- Equivalente a serialize...


  /*


  NoArqAtual = GetNext(Atual);
  CurvaAtual.PIV.Constroi(NoArqAtual.PIV,NoArqAtual.Cota,NoArqAtual.Ig); 
  CurvaAtual.Y1 = atof(NoArqAtual.Y1);
  CurvaAtual.Y2 = atof(NoArqAtual.Y2);
  CurVertical& CurvaAnterior = LCurvasVerticais.GetTail();
  CurvaAtual.I1 = CurvaAnterior.I2 = (CurvaAtual.PIV.Cota - CurvaAnterior.PIV.Cota) / (CurvaAtual.PIV - CurvaAnterior.PIV);
  CurvaAnterior.PTV.Cota = CurvaAnterior.Y2 * CurvaAnterior.I2 + CurvaAnterior.PIV.Cota;
  CurvaAnterior.CalculaE();
  CurvaAtual.PCV.Constroi(CurvaAtual.PIV.EstVirtual - CurvaAtual.Y1,CurvaAtual.Y1 * -CurvaAtual.I1 + CurvaAtual.PIV.Cota);
  CurvaAtual.PTV.Constroi(CurvaAtual.PIV.EstVirtual + CurvaAtual.Y2);

  LCurvasVerticais.AddTail(CurvaAtual);    
  }
  RemoveAll();  //--- Remove os dados lidos, ficam só os calculados.
  */

  return TRUE;
}

CCurVerticais& CCurVerticais::operator=(CCurVerticais& CurAIgualar)
{
  RemoveAll();
  POSITION Atual(LCurvasVerticais.GetHeadPosition()),Prox(Atual);

  while(Atual)
  {
    LCurvasVerticais.GetNext(Prox);;
    LCurvasVerticais.RemoveAt(Atual);
    Atual = Prox;
  }

  Atual = CurAIgualar.LCurvasVerticais.GetHeadPosition();

  while(Atual) LCurvasVerticais.AddTail(CurAIgualar.LCurvasVerticais.GetNext(Atual));

  NomeArquivo = CurAIgualar.NomeArquivo;

  return *this;
}

int CCurVerticais::Consiste()
{
  Atual = LCurvasVerticais.GetHeadPosition();
  int i = 0;

  while(Atual && Erro == 0)
  {
    i++;
    Erro = LCurvasVerticais.GetNext(Atual).Consiste();

    if (Erro) 
    {
      if(!(i == 1 && (Erro == 4 || Erro == 5)))
      {
        CString Mensg;

        Mensg.Format("Curva vertical, o registro %d está invalido: ",i);
        Mensg += monolog.ErrosCurVer[Erro-1];

        monolog.mensagem(15,Mensg);
      }
      else Erro = 0;
    }

  }
  return Erro;
}
