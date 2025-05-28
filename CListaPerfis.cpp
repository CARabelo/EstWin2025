#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqperfis.h" 
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "clistaperfis.h"

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
#define NULO      -2
#define SIM       TRUE
#define NAO       FALSE

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação do nó da lista.
//----------------------------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas listas são dinâmicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

CLPerfis::CLPerfis() : Atual(NULL) {}

CLPerfis::CLPerfis(CString& NomeArq) : Atual(NULL) 
{
  Constroi(NomeArq);  //--- constroi a lista a partir de um arquivo
}


CLPerfis::~CLPerfis() 
{
  RemoveAll();   //--- Remove todos os elementos da lista;
}

int CLPerfis::Compara(class Perfil &No1,class Perfil &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.Estaca.EstVirtual < No2.Estaca.EstVirtual) return MENOR;
  else if (No1.Estaca.EstVirtual > No2.Estaca.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CLPerfis::InsereOrdenada(class Perfil &No,int INSERIRIGUAL)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  if (!Atual) Atual = GetHeadPosition();
  if (!Atual) AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir  e for para inserir,
    //--- insere o nó depois do atual.

    if ((Res = Compara(GetAt(Atual),No)) == MAIOR)
    {
      GetPrev(Atual); 

      while (Atual && (Res = Compara(GetAt(Atual),No)) == MAIOR) GetPrev(Atual);

      if (Res == IGUAL && ! INSERIRIGUAL) return;

      if (!Atual)
      {
        AddHead(No);
        Atual = GetHeadPosition();
      }
      else InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        while (Atual && (Res = Compara(GetAt(Atual),No)) == MENOR) GetNext(Atual);

        if (Res == IGUAL && !INSERIRIGUAL) return;

        if (!Atual) 
        {
          AddTail(No);
          Atual = GetTailPosition();
        }
        else InsertBefore(Atual,No);
      }
      else if (INSERIRIGUAL) InsertAfter(Atual,No);
    }
  }
}

int CLPerfis::PosicionaNoMaisProximo(CEstaca& Estaca,int Direcao)
{
  if (!Atual) Atual = GetHeadPosition();
  if (!Atual) return MENOR;

  if (!(Direcao = Estaca < GetAt(Atual).Estaca ? TRAS : Estaca > GetAt(Atual).Estaca ? FRENTE : FALSE)) return IGUAL;

  if (Direcao == FRENTE)
  {
    GetNext(Atual);

    while(Atual && GetAt(Atual).Estaca < Estaca) GetNext(Atual);

    if (!Atual) Atual = GetTailPosition();
  }
  else
  {
    GetPrev(Atual);

    while(Atual && GetAt(Atual).Estaca > Estaca) GetNext(Atual);

    if (!Atual) Atual = GetHeadPosition();
  }

  if (fabs(GetAt(Atual).Estaca - Estaca) < 0.0005) return IGUAL;
  else if (GetAt(Atual).Estaca > Estaca) return MAIOR;
  else return MENOR;
}

//--- Operadores

BOOL CLPerfis::operator++()
{ 
  if (Atual) GetNext(Atual); 
  return Atual ? TRUE : FALSE;
}

BOOL CLPerfis::operator--()
{
  if (Atual) GetPrev(Atual); 
  return Atual ? TRUE : FALSE;
}

void CLPerfis::Constroi(CString &NomeArq)
{
  NomeArquivo = NomeArq;

  CFile ArqPerfil;
  CFileException e;

  if (ArqPerfil.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
  //  monolog.mensagem(23,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqPerfil,CArchive::load);
    Serialize(ar);
    ar.Close();
    ArqPerfil.Close();

  }
}

void CLPerfis::Serialize(CArchive& ar,int pEquidis)
{
  ////////////////////////////////////////////////////////////////

  POSITION Atual(GetHeadPosition());
  static int Equidistancia(0);     

  if(pEquidis != 0) Equidistancia = pEquidis;

  if (ar.IsStoring())
  {
    ar << (int) GetCount() << Equidistancia;

    while (Atual)
    {
      Perfil& PerfilAtual = GetNext(Atual);
      PerfilAtual.Serialize(ar);
      Perfil AcidentesTopograficos;
      AcidentesTopograficos.Serialize(ar);
    }
  }
  else
  {
    int i;

    ar >> i >> Equidistancia;
                    
    for (; i ; i--)
    {
      Perfil PerfilAtual;
      Perfil AcidentesTopograficos;
      PerfilAtual.Serialize(ar);
      AcidentesTopograficos.Serialize(ar);
      AddTail(PerfilAtual);
    } 
  }		
}

void CLPerfis::GravarArquivo(CString &NomeProjeto)
{
  CFile ArqPerfil;
  CFileException e;

  NomeArquivo = NomeProjeto + ".tca";

  if (ArqPerfil.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
  }
  else
  {
    CArchive ar(&ArqPerfil,CArchive::store);
    Serialize(ar);
    ar.Close();
    ArqPerfil.Close();
  }
}

void CLPerfis::CalculaIs()
{
  Atual = GetHeadPosition();

  while (Atual) GetNext(Atual).CalculaIs();
}

int CLPerfis::BuscaPerfil(CEstaca& Estaca,Perfil& rPerfil)
{       
  if(PosicionaNoMaisProximo(Estaca) == IGUAL)
  {
    rPerfil = GetAt(Atual);
    return true;
  }

  return false;
}
