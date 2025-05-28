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
#define ESPIRAL   'E'
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
//--- Implementação do set de perfis
//----------------------------------------------------------------------------------

CSetPerfis::CSetPerfis() : Atual(end()) {}

CSetPerfis::CSetPerfis(CString& NomeArq) : Atual(end()) 
{
  Constroi(NomeArq);  //--- constroi o set a partir de um arquivo
}

CSetPerfis::~CSetPerfis() 
{
  for (Atual = begin() ; Atual != end() ; Atual++) delete *Atual; 
}

void CSetPerfis::Constroi(CString &NomeArq)
{
  NomeArquivo = NomeArq;

  CFile ArqPerfil;
        CFileException e;

  if (ArqPerfil.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {     
    CArchive ar(&ArqPerfil,CArchive::load);
    Serialize(ar);
    
    ArqPerfil.Close();
    ar.Close();
  }
}

int CSetPerfis::PosicionaNoMaisProximo(CEstaca& Estaca,int Direcao)
{
  if (empty()) return MAIOR;
  
  //--- Posiciona o atual na estaca mais proxima

  itSetPerfis itMaI(PegaMaiorIgual(CSecao(Estaca))),         //--- Pega o maior ou igual
              itMeI(RascMaI ? (RascMaI)-- : end());         //--- Pega o anterior ao maior ou igual
  
  //--- Verifica qual esta mais perto o proximo ou o anterior
  
  double DistMaI(itMaI != end() ? (*itMaI).Estaca.EstVirtual - Estaca.EstVirtual : INFINITO),
         DistMei(itMeI != end() ? Estaca.EstVirtual - (*itMeI).Estaca.EstVirtual : INFINITO);
  
  Atual = DistMaI < DistMeI ? itMaI : itMeI;   ///--- atualiza o ponteiro para o atual. Ele passa a ser o mais próximo
 
  if (Atual == end() || (*Atual).Estaca < Estaca) return MENOR;

  if ((*Atual).Estaca > Estaca) return MAIOR;

  //--- Achou igual
  
  return IGUAL;
}

//--- Operadores

double CSetPerfis::PegaPontoNotavel(CEstaca &Anterior)
{
  //--- Retorna a primeira estaca mias próxima de Anterior.
        //--- Retorna em anterior o valor encontrado bem como o tipo da estaca.
/*
  if (!PosicionaNoMaisProximo(Anterior,FRENTE)) return -INFINITO;

  Perfil &NoAtual = GetAt(Atual);

        Anterior.Constroi(NoAtual.Estaca.EstVirtual,INFINITO,0); */
                                                                                                                
        return Anterior.EstVirtual; 
}

int CSetPerfis::BuscaPerfil(CEstaca& Estaca,Perfil& PerfilABuscar)
{
  if (PosicionaNoMaisProximo(Perfil(Estaca)) == IGUAL)
  {
    PerfilABuscar = *Atual;
    return TRUE;
  }
  return FALSE;
}

void CSetPerfis::Serialize(CArchive& ar)
{
  Atual(Begin());

  if (ar.IsStoring())
  { 
    ar << size();
    
    while (Atual != end())
    {
      Perfil& PerfilAtual = *Atual;
      PerfilAtual.Serialize(ar);
      Atual++;
    }
  }
  else
  {
    int i;

    for (ar >> i ; i ; i--)
    {
      Perfil* PerfilAtual = new Perfil;
      
      PerfilAtual.Serialize(ar);
      insert(PerfilAtual);
    } 
  }             
}

void CSetPerfis::GravarArquivo(CString &NomeProjeto)
{
  CFile ArqPerfil;
  CFileException e;

  NomeArquivo = NomeProjeto + ".tca";
  
  if (ArqPerfil.Open(NomeArquivo,CFile::modeCreate |CFile::modeWrite,&e) == 0)
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

void CSetPerfis::CalculaIs()
{
  for(Atual = begin(); atual != end() ; Atual++) (*Atual).CalculaIs());
}
