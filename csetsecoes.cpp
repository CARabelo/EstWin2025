#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
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
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "csecao.h"
#include <set>
#include "csetsecoes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENOR     -1
#define MAIOR      1
#define IGUAL      0
#define TRAS      -1
#define FRENTE     1
#define GRAVAR     1
#define LER        0 

CSetSecoes::CSetSecoes(CString& NomeProjeto) : TaludesCorte(NomeProjeto),TaludesAterro(NomeProjeto),Atual(NULL),Projeto(NomeProjeto)
{
  Serialize(LER);
  
  Atual = begin();
}

void CSetSecoes::Serialize(const int Tipo)
{
  if (Projeto.IsEmpty()) return;

  CString NomeArquivo(Projeto + ".not");
  CFile ArqNota,ArqTerreno;
  CFileException e;

  UINT Modo(Tipo == GRAVAR ? CFile::modeWrite : CFile::modeRead);

  if (ArqNota.Open(NomeArquivo, Modo | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) 
    {
      //monolog.mensagem(1,NomeArquivo.GetBuffer(1));
      //    return;
    }
  }

  NomeArquivo = Projeto + ".tca";

  if (ArqTerreno.Open(NomeArquivo,Modo | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) 
    {
      //monolog.mensagem(1,NomeArquivo.GetBuffer(1));
      //return;
    }
  }

  Modo = (Tipo == GRAVAR ? CArchive::store : CArchive::load);

  CArchive arp(&ArqNota,Modo);
  CArchive art(&ArqTerreno,Modo);

  if (Tipo == GRAVAR)
  {
    //--- Serializa o projeto

    arp << size();

    for (Atual = begin() ; Atual != end() ; Atual++)
    {
      try
      {
        (*Atual).Serialize(arp);
       
        //--- aproveita a lista de terrenos orf�os e insere nela os perfis de terreno n�o orf�os 
        //--- (que t�em projeto), assim poderemos serializar as se��es de terreno sem perdas e 
        //--- de uma s� vez

        TerrenoOrfao.InsereOrdenada(*Atual).Terreno);
      }
      catch(CArchiveException* e)
      {
        //--- Trata Erro.
        break;
      } 
    }
    
    //--- Serializa o terreno
    //--- A lista de terreno completa est� na lista terrenoorf�o, as se��es n�o
    //--- orf�s (que t�em terreno) foram colocadas aqui para poderem ser serializadas

    TerrenoOrfao.Serialize(art);
  }
  else
  {
    int i;
         
    for (arp >> i ; i ; i--)
    {
      CSecao* NoAtual = new CSecao;

      try
      {
        NoAtual.Serialize(arp);
        InsereOrdenada(NoAtual);
      }
      catch(CArchiveException* e)
      {
        //--- Tratar Erro; 
        break;
      }
    }

    Atual = end();

    for (art >> i ; i ; i--)
    {
      //--- Insere os perf�s de terreno nas se��es, os perf�s de terreno que n�o tiverem se��es de projeto s�o 
      //--- mantidas na lista de terreno orf�os para que a lista de terreno possa ser serializada na sa�da dos desenhos.
      //--- se estes perf�s n�o fossem mantidos nessa lista seriam perdidos na serializa��o.

      Perfil TerrenoAtual;

      TerrenoAtual.Serialize(art);
      if (PosicionaNoMaisProximo(TerrenoAtual.Estaca,FRENTE) == IGUAL) (*Atual).Terreno = TerrenoAtual;
      else TerrenoOrfao.InsereOrdenada(TerrenoAtual);
    }
  }

  if (ArqTerreno)
  {
    art.Close();
    ArqTerreno.Close();

  }
  
  if (ArqNota)
  {
    arp.Close();
    ArqNota.Close();
  }
}

int CSetSecoes::PosicionaNoMaisProximo(CEstaca& Estaca)
{ 
  if (empty()) return MAIOR;
  
  //--- Posiciona o atual na estaca mais proxima

  itMapaSecoes itMaI(PegaMaiorIgual(CSecao(Estaca))),         //--- Pega o maior ou igual
               itMeI(RascMaI ? (RascMaI)-- : end());         //--- Pega o anterior ao maior ou igual
  
  //--- Verifica qual esta mais perto o proximo ou o anterior
  
  double DistMaI(itMaI != end() ? (*itMaI).Estaca.EstVirtual - Estaca.EstVirtual : INFINITO),
         DistMei(itMeI != end() ? Estaca.EstVirtual - (*itMeI).Estaca.EstVirtual : INFINITO);
  
  Atual = DistMaI < DistMeI ? itMaI : itMeI;   ///--- atualiza o ponteiro para o atual. Ele passa a ser o mais pr�ximo
 
  if (Atual == end() || (*Atual).Estaca < Estaca) return MENOR;

  if ((*Atual).Estaca > Estaca) return MAIOR;

  //--- Achou igual
  
  return IGUAL;
}

void CSetSecoes::TaludesDefault()
{
  TaludesAterro.TaludesDefault();
  TaludesCorte.TaludesDefault();
}

CSetSecoes &CSetSecoes::operator=(CSetSecoes &No)
{
  Projeto = No.Projeto;  
  TerrenoOrfao = No.TerrenoOrfao;
  TaludesCorte = TaludesCorte;
  TaludesAterro = TaludesAterro;

  (*this)::operator=(No);

  return *this;
}

BOOL CSetSecoes::operator++(int i)
{ 
  //--- Funciona como um buffer circular, se chegar no fim, volta para o come�o.
  //-- EVITAR ISSO, POIS NAO PODE SER USADO PARA VARRER DO INICIO AO FIM...

  if (Atual == end()) Atual = begin();  
  
  Atual++;

  return Atual == end();
}

BOOL CSetSecoes::operator--(int i)
{
  //--- Funciona como um buffer circular, se chegar no come�o, volta para o fim.

  if (Atual == begin()) Atual = end();
  
  Atual++; 

  return Atual != end();
}

void CtyMapaSecoes::DefRetangDes()
{
  //--- Varre o mapa e define todos os retangulos de desenho

  for (Atual = Begin() ; Atual != end() ; Atual++) (*Atual).DefRetangDes();
}

void CSetSecoes::CalculaIs()
{
  //--- Varre o mapa e calcula todos os is.
 
  for (Atual = Begin() ; Atual != end() ; Atual++) (*Atual).CalculaIs();
}

//--- Posiciona na se��o cuja a estaca seja igual a estaca dada, considerando apenas o eixo x, aceitando uma tolerancia de 
//--- +- ToleranciaX. Procura no projeto ou no terreno
//--- Como toda a se��o tem projeto a busca � feita no projeto, caso a
//--- solicita��o seja de terreno � verificado se a se��o tem terreno, se tiver
//--- s�o feitas as verifica��es de tolerancia com o terreno.
		
bool CSetSecoes::Posiciona(CEstaca& Estaca,double ToleranciaX,int Tipo)
{	
  PosicionaNoMaisProximo(Estaca.EstVirtual);
	
	if (Atual != end())
	{ 
    return Tipo == PROJETO ? (fabs(Estaca.EstVirtual - (*Atual).Estaca.EstVirtual) < ToleranciaX) :
		  	                     (!SecAtual.Terreno.empty() ? fabs(Estaca.EstVirtual - (*Atual).Terreno.Estaca.EstVirtual) < ToleranciaX : false); 		
  }

  return false;   //--- Mapa vazio, retorna false
}

//--- Posiciona na se��o cuja a estaca seja igual a estaca dada, considerando os dois eixos, x e y (Estaca e cota)  
//--- aceitando uma tolerancia de +- ToleranciaX e Y. Procura no projeto ou no terreno

bool CSetSecoes::Posiciona(CEstaca& Estaca,double ToleranciaX. double ToleranciaY, int Tipo)
{	
  PosicionaNoMaisProximo(Estaca.EstVirtual);
  
	if (Atual != end())      //--- Se achou alguma se��o...
	{
	  double DeltaY(Tipo == PROJETO ? fabs(Estaca.Cota - SecAtual.Projeto.Estaca.Cota) : 
                              			fabs(Estaca.Cota - SecAtual.Terreno.Estaca.Cota));
	 		
		return Tipo == PROJETO ? (fabs(Estaca.EstVirtual - (*Atual).Projeto.Estaca.EstVirtual) < ToleranciaX  && DeltaY < ToleranciaY) :
			                       (!(*Atual).Terreno.empty() ? fabs(Estaca.EstVirtual - (*Atual).Terreno.Estaca.EstVirtual) < ToleranciaX  && DeltaY < ToleranciaY : false); 		
  }
		
  return false;    //--- Mapa vazio retorna false
}


void CSetSecoes::CalculaIsLongitudinais()
{
  //--- Calcula os is das retas do terreno.
  //--- Nem todas as se��es t�m terreno, ent�o � preciso verificar quais as se��es t�m e calcular os is entre elas
  
  if (size()) < 2 return;       //--- Para calcular os is tem q ter mais que 2 secoes

  itMapaSecoes AtualTemp(begin());

  CSecao& SecAnt = (*AtualTemp);   //--- Pega a primeira se��o.

  AtualTemp++; 

  while(AtualTemp != end())          //-- Enquanto houverem se��es.            
  {
    CSecao& SecAtual = (*AtualTemp);   //--- Pega a proxima se��o.
    
    if (SecAnt->Terreno.Estaca != INFINITO)       //---A se��o anterior  tem terreno?
    {
      if (SecAtual->Terreno.Estaca != INFINITO)   //--- A se��o atual tem terreno?
      {
        //--- Calcula o i e passa para o pr�ximo ponto.

        SecAnt->IPost = SecAtual->IAnte = (SecAtual->Terreno.Estaca.Cota - SecAnt->Terreno.Estaca.Cota) / 
                                          (SecAtual->Terreno.Estaca - SecAnt->Terreno.Estaca);
        SecAnt = SecAtual;
      }
    }
    else SecAnt = SecAtual;        //--- Passa a se��o que n�o tem terreno.  
   
    AtualTemp++;  
  }
}

void CSetSecoes::~CSetSecoes()
{
  //--- Deleta o as se��es

  itMapaSecoes it;
  
  while(begin() != end())
  {
    it = begin();
    erase(it);
    delete (*it);
  }
}
