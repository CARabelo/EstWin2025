#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "cponto.h" 
#include "clpontos.h"
#include "math.h"
#include "estaca.h"
#include <list>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1
#define MENOR     -1
#define MAIOR      1
#define IGUAL      0
#define TRAS      -1
#define FRENTE     1
#define X          0
#define Y          1
#define ESQUERDO   0
#define DIREITO    1
#define TODOS     -1
#define dx        1e-15 

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação do nó da lista.
//--- Nesta lista o no é uma classe separada da lista pois pode ser criado separadamente.
//----------------------------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas lstas são dinamicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

CLPontos::CLPontos() : Atual(NULL) 
{
} 

int CLPontos::Compara(const Ponto &No1,const Ponto &No2)
{
  double Dx(No1.x - No2.x);

  if(Dx < -1e-6) return MENOR;
  if(Dx > 1e-6) return MAIOR;
  
  return IGUAL;  
}

void CLPontos::Insere(Ponto &No,int CALCULARRAMPAS)
{
  AddTail(No);

  //--- Calcula as Rampas

  if (CALCULARRAMPAS) CalculaRampa(GetTailPosition());
}

void CLPontos::InsereOrdenada(const Ponto &No,int INSERIRIGUAL,int CALCULARRAMPAS)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  Ponto PontoAinserir(No);

  Atual = NULL;

  if (!Atual) Atual = GetHeadPosition();
  if (!Atual) 
  {
    AddHead(PontoAinserir);  //--- Se a lista esta vazia , insere na cabeça.
    Atual = GetHeadPosition();
  }
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir  e for para inserir,
    //--- insere o nó depois do atual.

    if ((Res = Compara(GetAt(Atual),PontoAinserir)) == MAIOR)
    {
      GetPrev(Atual); 

      while (Atual && (Res = Compara(GetAt(Atual),PontoAinserir)) == MAIOR) GetPrev(Atual);

      if (Res == IGUAL && ! INSERIRIGUAL) return;

      Atual = !Atual ? AddHead(PontoAinserir) : InsertAfter(Atual,PontoAinserir);
    }
    else 
    {
      if (Res == MENOR)
      {
        while (Atual && (Res = Compara(GetAt(Atual),PontoAinserir)) == MENOR) GetNext(Atual);

        if (Res == IGUAL && !INSERIRIGUAL) return;

        Atual =  !Atual ? AddTail(PontoAinserir) : InsertBefore(Atual,PontoAinserir);
      }
      else if (INSERIRIGUAL) Atual = InsertAfter(Atual,PontoAinserir);
    }

    //--- Calcula as Rampas

    if (CALCULARRAMPAS && Atual)
    {
      POSITION Anterior(Atual),Posterior(Atual);

      GetNext(Posterior);
      GetPrev(Anterior);
      if (Posterior) CalculaRampa(Atual);
      if (Anterior) CalculaRampa(Anterior);
    }
  }
}

double CLPontos::CalculaRampa(POSITION P)
{
  if (P) Atual = P;
  if (!Atual || GetCount() < 2) return INFINITO;

  Ponto &PontoAtual(GetNext(Atual));

  if (Atual) 
  {
    Ponto &ProximoPonto = GetNext(Atual);

    PontoAtual.i = (ProximoPonto.y - PontoAtual.y) / (ProximoPonto.x - PontoAtual.x + 1e-10);

    if (!Atual) ProximoPonto.i = PontoAtual.i;  //--- Ultimo Ponto, iguala o i.
  }
  else
  {
    Atual = GetTailPosition();
    GetPrev(Atual);

    if(Atual)
    {
      Ponto& PontoAnterior = GetAt(Atual);
      PontoAtual.i = (PontoAtual.y - PontoAnterior.y) / (PontoAtual.x - PontoAnterior.x);
    }
    else PontoAtual.i = 0.0;
  }

  return PontoAtual.i;
}

double CLPontos::BuscaMenorRampa()
{
  if (GetCount() < 2) return INFINITO;

  double IMenor = INFINITO; 
  Ponto PontoAtual = GetHead();

  for(Atual = GetHeadPosition() ; Atual ; PontoAtual = GetNext(Atual))
    if (PontoAtual.i < IMenor) IMenor = PontoAtual.i;
  return IMenor;
}

const CLPontos &CLPontos::operator=(const CLPontos &No)
{
  Atual = NULL;		 
  RemoveAll();

  if (!No.IsEmpty())
  {
    POSITION Temp = No.GetHeadPosition();

    while(Temp)
    {
      Ponto PAtual = No.GetAt(Temp);
      AddTail(PAtual);
      if (Temp == No.Atual) Atual = GetTailPosition();
      No.GetNext(Temp);
    }
  }							 
  return *this;
}

BOOL CLPontos::operator++()
{
  if(Atual) GetNext(Atual);
  return Atual ? TRUE : FALSE;
}

BOOL CLPontos::operator--()
{
  if(Atual) GetPrev(Atual);
  return Atual ? TRUE : FALSE;
}

void CLPontos::TrimLeft(double Afast)
{
  while( GetHeadPosition() && GetHead().x  < Afast) RemoveHead();
}

void CLPontos::TrimRight(double Afast)
{
  while(GetTailPosition() && GetTail().x  > Afast) RemoveTail();
}

void CLPontos::RemoveFaixa(double Inicio,double Fim)
{
  POSITION PInicial = GetHeadPosition();

  while(PInicial && GetNext(PInicial).x < Inicio);

  POSITION PFinal = GetTailPosition();

  while(PFinal && GetPrev(PFinal).x > Fim);

  if (PFinal) GetNext(PFinal);

  POSITION Atual = PInicial;

  while(Atual != PFinal)
  {
    POSITION Anterior = Atual;

    GetNext(Atual);
    RemoveAt(Anterior);
  }
}

void CLPontos::RemoveFaixa2(double Inicio,double Fim)
{
  POSITION PInicial = GetHeadPosition();

  while(PInicial && GetAt(PInicial).x < Inicio) GetNext(PInicial); 

  POSITION PFinal = GetTailPosition();

  while(PFinal && GetAt(PFinal).x > Fim) GetPrev(PFinal);

  if (PFinal) GetNext(PFinal);

  POSITION Atual = PInicial;

  while(Atual != PFinal)
  {
    POSITION Anterior = Atual;

    GetNext(Atual);
    RemoveAt(Anterior);
  }
}

void CLPontos::Destroi()
{
  RemoveAll();
  Atual = NULL;
}

void CLPontos::InsereLista(CLPontos &ListaAInserir,int INSERIRIGUAL)
{
  POSITION Atual = ListaAInserir.GetHeadPosition();
  while(Atual) InsereOrdenada(ListaAInserir.GetNext(Atual),INSERIRIGUAL);
}

void CLPontos::CalculaIs()
{
  POSITION At = GetHeadPosition();

  while(At) 
  {
    CalculaRampa(At);
    GetNext(At);
  }
}

void CLPontos::InsereListaLado(CLPontos &ListaAInserir,int Lado,int INSERIRIGUAL)
{
  POSITION At = Lado == DIREITO ? ListaAInserir.GetHeadPosition() : ListaAInserir.GetTailPosition();
  if (!At) return;      //--- Nada a inserir.

  Ponto PAnte(0.0,0.0,0.0),P = ListaAInserir.GetAt(At);;

  if (GetCount()) PAnte = (Lado == DIREITO) ? GetTail() : GetHead(); 

  while(At) 
  {

    //--- SE VAI FAZER INSERE ORDENADA, PODE PEGAR OS PONTOS DE QUALQUER JEITO......??????!!!!!!
    //  pORQUE ESTÁ PEGANDO EM ORDEM??? bastaria...
    //---{
    //---  At =  listaainserir.getheadposition();
    //---  while (At)
    //---  {
    //---    insereordenada(ListaAInserir.GetNext(At),insereigual);
    //---	 }
    //---}

    P = (Lado == ESQUERDO ? ListaAInserir.GetPrev(At) : ListaAInserir.GetNext(At));
    P+= PAnte;
    InsereOrdenada(P,INSERIRIGUAL);
    PAnte = P;
  }
}

void CLPontos::RemoveTudo()
{
  RemoveAll();
  Atual = NULL;
}

BOOL CLPontos::ExcluiPonto(double x)
{
  //--->>>>>>>>>>>>>>>>>> Incluir o método posiciona no mais proximo. <<<<<<<<<<<

  Atual = GetHeadPosition();

  while (Atual && GetAt(Atual).x < x)
  {
    Ponto P = GetAt(Atual);
    GetNext(Atual);
  }

  if(Atual && GetAt(Atual).x == x) 
  {
    RemoveAt(Atual);
    Atual = NULL;
    return TRUE;
  }
  return FALSE;
}

void CLPontos::ExcluiPontos(CString& Rotulo)
{
  std::list<POSITION> LstRemover;
  std::list<POSITION>::iterator itLstRemover;

  //--->>>>>>>>>>>>>>>>>> Incluir o método posiciona no mais proximo. <<<<<<<<<<<
  Atual = GetHeadPosition();

  while (Atual)
  {
    Ponto P = GetAt(Atual);
    if(P.Descricao.CompareNoCase(Rotulo.GetBuffer()) == 0)
    {
      LstRemover.push_back(Atual);
    }
    GetNext(Atual);
  } 

   for(itLstRemover = LstRemover.begin() ; itLstRemover != LstRemover.end() ; itLstRemover++) RemoveAt(*itLstRemover);
}

BOOL CLPontos::ExcluiPonto(POSITION PontoAExcluir)
{
  if (!PontoAExcluir) PontoAExcluir = Atual;
  if (!PontoAExcluir) return FALSE;

  if (PontoAExcluir == Atual) Atual = NULL;

  RemoveAt(PontoAExcluir);

  return TRUE;
}


BOOL CLPontos::CalculaIntersecao(const Ponto& PP1, const Ponto& PP2, const Ponto& PT1, const Ponto& PT2, Ponto& Intersecao, bool NaReta,double Erro)
{
  //--- Ordena os pontos por x, colocando os menores a esquerda.

  Ponto NoProjAnte = PP1.x < PP2.x ? PP1 : PP2, NoProjPost = PP1.x < PP2.x ? PP2 : PP1,
        NoTerrAnte = PT1.x < PT2.x ? PT1 : PT2, NoTerrPost = PT1.x < PT2.x ? PT2 : PT1;

  //  double DeltaxP(PP2.x-PP1.x),DeltaxT(PT2.x-PT1.x),DeltayP(PP2.y-PP1.y),DeltayT(PT2.y-PT1.y);

  double DeltaxP(NoProjPost.x - NoProjAnte.x), DeltaxT(NoTerrPost.x - NoTerrAnte.x),
         DeltayP(NoProjPost.y - NoProjAnte.y), DeltayT(NoTerrPost.y - NoTerrAnte.y);
         
  if (fabs(DeltaxP) < 1E-6) DeltaxP += 1E-5;
  if (fabs(DeltaxT) < 1E-6) DeltaxT += 1E-5;
  if (fabs(DeltayP) < 1E-6) DeltayP += 1E-5;
  if (fabs(DeltayT) < 1E-6) DeltayT += 1E-5;
  
  //--- Mesma coisa mas não funciona!!!!!!
  /*
  double ErroX10(1E-5);

  if (fabs(DeltaxP) < Erro) DeltaxP += ErroX10;
  if (fabs(DeltaxT) < Erro) DeltaxT += ErroX10;
  if (fabs(DeltayP) < Erro) DeltayP += ErroX10;
  if (fabs(DeltayT) < Erro) DeltayT += ErroX10;
  */

  //--- Calcula o i  das retas de projeto de terreno.

  double iProj((DeltayP) / ((DeltaxP)+1e-12)),
         iTerr((DeltayT) / ((DeltaxT)+1e-12));

  // Retas paralelas, retorna false.

  if (fabs(iProj - iTerr) < 1e-10) 
  {
    Intersecao.x = Intersecao.y = INFINITO;
    return FALSE;
  }

  //--- Calcula o b  das retas de projeto de terreno

  double  BProj(NoProjPost.y - (iProj * NoProjPost.x)),
          BTerr(NoTerrPost.y - (iTerr * NoTerrPost.x));

  //--- Calcula o x da interseção pela fórmula x = (B1 - B2) / (i2-i1)  

  double Xinte = (BProj - BTerr) / (iTerr - iProj), Yinte = Xinte * iProj + BProj;

  Intersecao.x = Xinte;
  Intersecao.y = Yinte;

  if (NaReta)
  {
    //--- Verifica se o ponto esta dentro do retangulo inflado com +Erro

    double MenorxP1(PP1.x < PP2.x ? PP1.x : PP2.x),
           MenoryP1(PP1.y < PP2.y ? PP1.y : PP2.y),
           MaiorxP1(PP1.x > PP2.x ? PP1.x : PP2.x),
           MaioryP1(PP1.y > PP2.y ? PP1.y : PP2.y),
           MenorxP2(PT1.x < PT2.x ? PT1.x : PT2.x),
           MenoryP2(PT1.y < PT2.y ? PT1.y : PT2.y),
           MaiorxP2(PT1.x > PT2.x ? PT1.x : PT2.x),
           MaioryP2(PT1.y > PT2.y ? PT1.y : PT2.y);

    MenorxP1 -= Erro;
    MenoryP1 -= Erro;
    MaiorxP1 += Erro,
    MaioryP1 += Erro,
    MenorxP2 -= Erro;
    MenoryP2 -= Erro;
    MaiorxP2 += Erro;
    MaioryP2 += Erro;

    return (Intersecao.x >= MenorxP1 && Intersecao.x <= MaiorxP1 &&
            Intersecao.y >= MenoryP1 && Intersecao.y <= MaioryP1 &&
            Intersecao.x >= MenorxP2 && Intersecao.x <= MaiorxP2 &&
            Intersecao.y >= MenoryP2 && Intersecao.y <= MaioryP2);
  }

  return TRUE;        //--- A interseção existe mas não foi solicitado que estivessem nos segmentos das retas
}

POSITION CLPontos::PosicionaPonto(double x,double y,double Toleranciax,double Toleranciay)
{
  Atual = GetHeadPosition();
  BOOL Achou(FALSE);

  while (Atual && ! Achou)
  {
    Ponto p(GetAt(Atual));

    if (p.x + Toleranciax > x && p.x - Toleranciax < x && p.y + Toleranciay > y && p.y - Toleranciay < y) Achou = TRUE;
    else 	GetNext(Atual);
  }
  return Atual;
}

POSITION CLPontos::PosicionaPontoX(double x,double Toleranciax)
{
  Atual = GetHeadPosition();
  BOOL Achou(FALSE);

  while (Atual && ! Achou)
  {
    Ponto p(GetAt(Atual));

    if (p.x + Toleranciax > x && p.x - Toleranciax < x) Achou = TRUE;
    else 	GetNext(Atual);
  }
  return Atual;
}

void CLPontos::AlteraPonto(const Ponto& PNovo,POSITION Pos)  //--- Altera as coordenadas de um determinado ponto.
{
  if (Pos) Atual = Pos;
  else Pos = Atual;
  if (Pos)
  {
    SetAt(Pos,*const_cast<Ponto*>(&PNovo));
    CalculaRampa();

    POSITION Anterior(Pos),Proximo(Pos);
    GetPrev(Anterior);
    GetNext(Proximo);

    //--- Recalcula as rampas.

    if (Anterior) CalculaRampa(Anterior);
    if (Proximo) CalculaRampa(Proximo);
  }
}

void CLPontos::Serialize(CArchive& ar)
{
  POSITION Atual(GetHeadPosition());

  if (ar.IsStoring())
  {
    ar << (int) GetCount();
    while(Atual) GetNext(Atual) >> ar;
  }
  else
  {
    int i;
    Ponto P;

    for (ar >> i ; i ; i--)
    {
      P << ar;
      AddTail(P);
    }
  }
}

int CLPontos::PosicionaNoMaisProximo(double x)
{
  Atual = GetHeadPosition();
  if (!Atual) return MENOR;

  while(Atual && GetAt(Atual).x < x) GetNext(Atual);

  if(!Atual) return MENOR;

  if (GetAt(Atual).x > x) return MAIOR;
  if (GetAt(Atual).x < x) return MENOR;

  return IGUAL;
}

double CLPontos::Calculay(double x,int TIPO) 
{
  if (GetCount() < 2 || x < GetHead().x ||x > GetTail().x) return 0.0;    //--- Gradiente fora da seção tipo assume 0.0
  
  Atual = GetHeadPosition();

  double xAtual(GetAt(Atual).x);
	
  do
  {
     xAtual = GetNext(Atual).x;
  }while(xAtual < x); 

  if (Atual == NULL) Atual = GetTailPosition();
  else GetPrev(Atual);

  Ponto P2(GetPrev(Atual));
  Ponto P1(GetAt(Atual));

  if (TIPO == TIPO_DELTA_H) 
  {
    return (P2.y - P1.y) / (P2.x - P1.x) * (x - P2.x);
  }
  else return (P2.y - P1.y) / (P2.x - P1.x) * (x - P2.x) + P2.y;
}

BOOL CLPontos::PosicionaCabeca()
{
  Atual = GetHeadPosition();
  return Atual ? TRUE : FALSE;
}

BOOL CLPontos::PosicionaPe()
{
  Atual = GetTailPosition();
  return Atual ? TRUE : FALSE;
}

void CLPontos::AlteraRampas(double Rampa,int Lado,POSITION Pos)
{
  if (!Atual) return;

  if (Pos) Atual = Pos;
  POSITION PAnterior(Atual),PPosterior(Atual);
  GetPrev(PAnterior);
  GetNext(PPosterior);

  Ponto PAtual(GetAt(Atual));

  if ((Lado == ESQUERDO || Lado == TODOS) && PAnterior)
  {
    Ponto PAnte(GetAt(PAnterior));
    PAnte.y = (PAtual.x - PAnte.x) * Rampa + PAtual.y; 
    PAnte.i = Rampa;
    SetAt(PAnterior,PAnte);
  }
  if ((Lado == DIREITO || Lado == TODOS) && PAnterior)
  {
    Ponto PPost(GetAt(PPosterior));
    PPost.y = (PPost.x - PAtual.x) * Rampa + PAtual.y; 
    SetAt(PPosterior,PPost);
    PAtual.i = Rampa;
  }
}

const Ponto& CLPontos::Interpola(double x,CString& Rotulo)      //--- Insere o ponto em x na lista
{
  static Ponto PCalc;

  PCalc.x = PCalc.y = PCalc.z = INFINITO;

  PosicionaNoMaisProximo(x);

  if (!Atual) return PCalc;

  const Ponto& P = GetAt(Atual);

  if ((Atual == GetHeadPosition() && x < P.x) || (Atual == GetTailPosition() && x > P.x)) return PCalc;

  if (!Atual) return PCalc;

  if (x > P.x)
  {
    GetNext(Atual);
    const Ponto& PProx = GetAt(Atual);
    PCalc.i = (PProx.y - P.y) / (PProx.x - P.x);
  }
  else
  {
    if(x < P.x)
    {
      GetPrev(Atual);
      const Ponto& PAnte = GetAt(Atual);
      PCalc.i = (PAnte.y - P.y) / (PAnte.x - P.x);
    }
  }

  PCalc.y = PCalc.i * ( x - P.x) + P.y;  
  PCalc.x = x;
  PCalc.Descricao = Rotulo;

  InsereOrdenada(PCalc);

  return PCalc;
}

void CLPontos::AlteraFaixaY(const Ponto& P)   // Inicio em x, fim em y e valor em z
{
  PosicionaNoMaisProximo(P.x);

  while(Atual && GetAt(Atual).x <= P.y) GetNext(Atual).y += P.z; 
}

void CLPontos::RemovePontos(const tlstwstr& LstRotulos)
{
  std::list<POSITION> LstPosition;

  for (cittlstwstr it = LstRotulos.begin() ; it != LstRotulos.end() ; it++)
  {
    for(Atual = GetHeadPosition() ; Atual != NULL ; GetNext(Atual))
    {
      if(GetAt(Atual).Descricao.Find(it->c_str()) != -1)
      {
       LstPosition.push_back(Atual);
      }
    } 
  }

  for(std::list<POSITION>::iterator itLstPosition = LstPosition.begin() ; itLstPosition != LstPosition.end() ; itLstPosition++)
  {
    RemoveAt(*itLstPosition);
  }
}


void CLPontos::TrimLeftMenor(double Afast)
{
  if(GetSize() < 2) return;

  POSITION Anterior(GetHeadPosition()),Proximo(GetHeadPosition());

  GetNext(Proximo);
  
  while(Proximo && GetAt(Proximo).x  < Afast) 
  {
    Anterior = Proximo;
    GetNext(Proximo);

    RemoveHead();
   }
}

void CLPontos::TrimRightMaior(double Afast)
{
  if(GetSize() < 2) return;

  POSITION Proximo(GetTailPosition()),Anterior(GetTailPosition());

  GetPrev(Anterior);

  while(Anterior && GetAt(Anterior).x > Afast) 
  {
    Proximo = Anterior;
    GetPrev(Anterior);

    RemoveTail();
  }
}
