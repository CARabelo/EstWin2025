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
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "math.h"
#include "monologo.h"

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
#define MEDICAO    3

extern class monologo monolog;

CListaSecoes::CListaSecoes(CString& NomeProjeto) : TaludesCorte(NomeProjeto),TaludesAterro(NomeProjeto),Atual(NULL),Projeto(NomeProjeto),
  VolumeTotalCorte(0.0),VolumeTotalAterro(0.0),VolumeTotalEmpola(0.0)
{
  Serialize(LER);

  /*
  if (NomeProjeto.IsEmpty()) return;

  CString NomeArquivo(NomeProjeto + ".not");
  CFile ArqNota,ArqTerreno;
  CFileException e;

  if (ArqNota.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
  if (e.m_cause != CFileException::fileNotFound) 
  {
  //monolog.mensagem(1,NomeArquivo.GetBuffer(1));
  //    return;
  }
  }

  NomeArquivo = NomeProjeto + ".tca";

  if (ArqTerreno.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
  if (e.m_cause != CFileException::fileNotFound) 
  {
  //monolog.mensagem(1,NomeArquivo.GetBuffer(1));
  //     return;
  }
  }

  CArchive arp(&ArqNota,CArchive::load);
  CArchive art(&ArqTerreno,CArchive::load);

  Serialize(LER);

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
  */
  Atual = GetHeadPosition();
}

void CListaSecoes::InsereOrdenada(CSecao &No, int INSERIRIGUAL)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  if (!Atual) Atual = GetHeadPosition();
  if (!Atual) 
  {
    AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
    Atual = GetHeadPosition();
  }
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

void CListaSecoes::Serialize(const int Tipo)
{
  bool TemProjeto(1),TemTerreno(1);

  if (Projeto.IsEmpty()) return;

  bool EMedicao(Projeto.Find("med") != -1 || Projeto.Find("MED") != -1);  //--- Medicao tem med no nome

  //--- Se for medicao os dois arquivos a serem lidos são: 
  //--- o do terreno primitivo -> que é igual ao nomedoprojeto - "mednnn" +.tca (ex.: teste.tca)
  //--- o do projeto fica sendo o  do terreno nivelado da seção que é: nomedoprojeto+.tca (ex.: testemed003.tca)
  //--- No caso de ser medição esta rotina só é chamado no desenho das seções. no programa de desenho
  //--- das seções funcionará ok, porque afinal ambas as seções são formadas por  listas. 

  CFile ArqNota,ArqTerreno;  
  CString NomeArquivo(Projeto.Left(Projeto.GetLength()) + ".not");

  CFileException e;

  //--- No cálculo do Volume não precisa gravar o terreno (na verdade não pode, porque ele é aparado nos off-sets) 

  UINT Modo(Tipo == GRAVAR || Tipo == GRAVAR_SO_PROJETO ? CFile::modeWrite : CFile::modeRead);

  if (ArqNota.Open(NomeArquivo, Modo | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) 
    {
      monolog.mensagem(14,e.m_cause);
      return;
    }
    else TemProjeto = 0;
  }

  if (Tipo != GRAVAR_SO_PROJETO)
  {
    NomeArquivo =  Projeto +".tca";  

    if (ArqTerreno.Open(NomeArquivo,Modo | CFile::shareDenyNone,&e) == 0)
    {
      if (e.m_cause != CFileException::fileNotFound) 
      {
        monolog.mensagem(14,e.m_cause);
        return;
      }
      else TemTerreno = 0;
    }
  }

  Modo = (Tipo == GRAVAR || Tipo == GRAVAR_SO_PROJETO ? CArchive::store : CArchive::load);

  CArchive arp(&ArqNota,Modo);
  CArchive art(&ArqTerreno,Modo);     //--- Se for medicao não tem este arquivo, fica nulo

  if (Tipo == GRAVAR || Tipo == GRAVAR_SO_PROJETO)
  {
    //--- Serializa o projeto

    if(TemProjeto)
    {
      arp << VolumeTotalCorte << VolumeTotalAterro << VolumeTotalEmpola << (unsigned int) GetCount();

      Atual = GetHeadPosition();

      while (Atual)
      {
        try
        {
          GetNext(Atual).Serialize(arp,EMedicao);

          //--- aproveita a lista de terrenos orfãos e insere nela os perfis de terreno não orfãos 
          //--- (que têm projeto), assim pode-se serializar as seções de terreno sem perdas e 
          //--- de uma só vez

          //      TerrenoOrfao.InsereOrdenada(NoAtual.Terreno);
        }
        catch(CArchiveException* e)
        {
          if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
            monolog.mensagem(15,CString(" Seções : ") + arp.GetFile()->GetFileName(),-1,e->m_cause);

          e->Delete();

          break;
        }
      }
    }

    //--- Serializa o terreno
    //--- A lista de terreno completa está na lista terrenoorfão, as seções não
    //--- orfãs (que têem terreno) foram colocadas aqui para poderem ser serializadas

    if (Tipo != GRAVAR_SO_PROJETO && TemTerreno) 
    {
      // TerrenoOrfao.Serialize(art,EquidisLateral);
      // TerrenoOrfao.Limpa();

      Atual = GetHeadPosition();

      //--- O terreno pode ter sido editado, porisso tem que ser regravado aqui. Acontece que algumas seções podem ter vindo sem terreno,
      //--- Neste caso o terreno desta seção nao pode ser gravado porque ele não existe, sua estaca esta com INFINITO. É preciso descontar 
      //--- a contagem das seções que estão nestas condições.

      unsigned int SecSemTerreno(0);

      for( Atual = GetHeadPosition() ; Atual != NULL ; GetNext(Atual))
      {
        if(GetAt(Atual).Terreno.Estaca.EstVirtual == INFINITO) ++SecSemTerreno;
      }

      art << (int) GetCount() - SecSemTerreno << EquidisLateral;

      Atual = GetHeadPosition();

      while (Atual)
      {
        try
        {
          if(GetAt(Atual).Terreno.Estaca.EstVirtual != INFINITO)     //--- A seção tem terreno
          {
            GetAt(Atual).Terreno.Serialize(art);
			GetAt(Atual).AcidentesTopograficos.Serialize(art);
          }
          GetNext(Atual);
        }
        catch(CArchiveException* e)
        {
          if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
            monolog.mensagem(15,CString(" Seções : ") + art.GetFile()->GetFileName(),-1,e->m_cause);

          e->Delete();

          break;
        }
      }
    }
  }
  else
  {
    unsigned int i(0); /// troquei aqui

    if (TemProjeto)
    {
      try
      {
        arp >> VolumeTotalCorte >> VolumeTotalAterro >> VolumeTotalEmpola;

        for (arp >> i ; i ; i--)
        {
          CSecao NoAtual;

          NoAtual.Serialize(arp,EMedicao);
          InsereOrdenada(NoAtual);
        }
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(15,CString(" Seções : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();
      }
    }

    if (!EMedicao && TemTerreno)    //--- Medicao não tem terreno, só projeto
    {
      unsigned int i(0);

      art >> i  >> EquidisLateral;

      for ( ; i ; i--)
      {
        //--- Insere os perfís de terreno nas seções, os perfís de terreno que não tiverem seções de projeto são 
        //--- mantidas na lista de terreno orfãos para que a lista de terreno possa ser serializada na saída dos desenhos.
        //--- se estes perfís não fossem mantidos nessa lista seriam perdidos na serialização.

        Perfil TerrenoAtual;
        Perfil AcidentesTopograficos;

        TerrenoAtual.Serialize(art);
        AcidentesTopograficos.Serialize(art);

        //	if (PosicionaNoMaisProximo(TerrenoAtual.Estaca,FRENTE) == IGUAL) GetAt(Atual).Terreno = TerrenoAtual;
        //else  TerrenoOrfao.InsereOrdenada(TerrenoAtual);

        Atual = PosicionaNoMaisProximo(TerrenoAtual.Estaca);

        if (Atual && fabs(GetAt(Atual).Estaca.EstVirtual -  TerrenoAtual.Estaca.EstVirtual) < 0.0001) 
        {
          GetAt(Atual).Terreno = TerrenoAtual;
          GetAt(Atual).AcidentesTopograficos = AcidentesTopograficos;;
        }
        //else  TerrenoOrfao.InsereOrdenada(TerrenoAtual);
        else
        {
          CSecao Sc;
          Sc.Estaca = TerrenoAtual.Estaca;
          Sc.Terreno = TerrenoAtual;
          Sc.AcidentesTopograficos = AcidentesTopograficos;

          InsereOrdenada(Sc);
        }
      }
    }
  }
}

inline int CListaSecoes::Compara(CSecao &No1,CSecao &No2)
{
  return No1.Compara(No2);
}

/*
POSITION CListaSecoes::PosicionaNoMaisProximo(CEstaca Estaca,int Direcao)
{
//--- Posiciona o atual na estaca mais proxima, assume que a estaca a procurar esteja próxima da estaca atual.
//--- se a estaca a procurar for menor que a atual procura para trás caso contrário para frente, caso
//--- não haja estaca atual a procura é feita a partir da primeira estaca.

POSITION Rasc(Atual);

if (!Rasc) Rasc = GetHeadPosition();
if (!Rasc) return NULL;

CEstaca EstacaAtual(GetAt(Rasc).Estaca);

if ((Direcao = Estaca < EstacaAtual ? TRAS : Estaca > EstacaAtual ? FRENTE : FALSE)) 
{
double DistAnte(-INFINITO),DistProx(INFINITO);

if (Direcao == FRENTE)
{
GetNext(Rasc);

while(Rasc && (EstacaAtual = GetAt(Rasc).Estaca) < Estaca) GetNext(Rasc); 

//--- Parou no posterior então verifica se a distancia com o anterior é menor e retorna 
//--- a posicao que tiver a menor distancia ( a posterior ou a anterior)

if (Rasc)
{
POSITION Poste(Rasc);

CEstaca T =GetAt(Rasc).Estaca; 

DistProx = GetAt(Rasc).Estaca.EstVirtual - Estaca.EstVirtual;

GetPrev(Rasc);

if(Rasc) return Estaca.EstVirtual - GetAt(Rasc).Estaca.EstVirtual < DistProx ? Rasc : Poste;
else return NULL;
}
}
else
{
GetPrev(Rasc);

while(Rasc && (EstacaAtual = GetAt(Rasc).Estaca) > Estaca) GetPrev(Rasc);

//--- Parou no anterior então verifica se a distancia com o posterior é menor e retorna 
//--- a posicao que tiver a menor distancia (a posterior ou a anterior)

if (Rasc)
{
POSITION Ante(Rasc);

DistAnte =  EstacaAtual.EstVirtual- GetAt(Rasc).Estaca.EstVirtual ;

GetNext(Rasc);

if(Rasc) return GetAt(Rasc).Estaca.EstVirtual - EstacaAtual.EstVirtual  < DistAnte ? Rasc : Ante;
else return NULL;
}
}
}

return Atual = Rasc;  //=== Achou igual!. atualiza o ponteiro par a estaca atual
}

*/
/*
POSITION CListaSecoes::PosicionaNoMaisProximo(CEstaca Estaca,int Direcao)
{
//--- Posiciona o atual na estaca mais proxima, assume que a estaca a procurar esteja próxima da estaca atual.
//--- se a estaca a procurar for menor que a atual procura para trás caso contrário para frente, caso
//--- não haja estaca atual a procura é feita a partir da primeira estaca.

//--- Tive q fazer o Rasc porque existe uma condicao  q o atual não pode ser mudado

if (!Atual) Atual = GetHeadPosition();
if (!Atual) return NULL;

if ((Direcao = Estaca < GetAt(Atual).Estaca.EstVirtual ? TRAS : Estaca > GetAt(Atual).Estaca.EstVirtual  ? FRENTE : FALSE)) 
{
double DistAnte(-INFINITO),DistProx(INFINITO);

if (Direcao == FRENTE)
{
GetNext(Atual);

while(Atual && GetAt(Atual).Estaca < Estaca) GetNext(Atual); 

//--- Parou no posterior então verifica se a distancia com o anterior é menor e retorna 
//--- a posicao que tiver a menor distancia ( a posterior ou a anterior)

if (Atual)
{
POSITION Poste(Atual);

DistProx = GetAt(Atual).Estaca.EstVirtual - Estaca.EstVirtual;

GetPrev(Atual);

if(!(Atual && (Estaca.EstVirtual - GetAt(Atual).Estaca.EstVirtual)  < DistProx)) Atual = Poste;
}
else Atual = GetTailPosition();
}
else
{

while(Atual && GetAt(Atual).Estaca > Estaca) GetPrev(Atual);

//--- Parou no anterior então verifica se a distancia com o posterior é menor e retorna 
//--- a posicao que tiver a menor distancia (a posterior ou a anterior)

if (Atual)
{
POSITION Ante(Atual);

DistAnte =  Estaca.EstVirtual - GetAt(Atual).Estaca.EstVirtual ;

GetNext(Atual);

if(!(Atual && (GetAt(Atual).Estaca.EstVirtual - Estaca.EstVirtual)  > DistAnte)) Atual = Ante;
}
else Atual = GetHeadPosition();
}
}

return Atual;   //--- O Atual está apontando para o mais próximo 
}

*/
POSITION CListaSecoes::PosicionaNoMaisProximo(CEstaca Estaca,int Direcao)
{
  //--- Posiciona o atual na estaca mais proxima, assume que a estaca a procurar esteja próxima da estaca atual.
  //--- se a estaca a procurar for menor que a atual procura para trás caso contrário para frente, caso
  //--- não haja estaca atual a procura é feita a partir da primeira estaca.

  //--- Tive q fazer o Rasc porque existe uma condicao  q o atual não pode ser mudado

  Atual = NULL;

  if (!Atual) Atual = GetHeadPosition();
  if (!Atual) return NULL;

  if ((Direcao = Estaca < GetAt(Atual).Estaca.EstVirtual ? TRAS : Estaca > GetAt(Atual).Estaca.EstVirtual  ? FRENTE : FALSE)) 
  {
    double DistAnte(-INFINITO),DistProx(INFINITO);

    if (Direcao == FRENTE)
    {
      GetNext(Atual);

      while(Atual && GetAt(Atual).Estaca < Estaca) GetNext(Atual); 

      //--- Parou no posterior então verifica se a distancia com o anterior é menor e retorna 
      //--- a posicao que tiver a menor distancia ( a posterior ou a anterior)

      if (Atual)
      {
        POSITION Poste(Atual);

        DistProx = GetAt(Atual).Estaca.EstVirtual - Estaca.EstVirtual;

        GetPrev(Atual);

        if(Atual) return Estaca.EstVirtual - GetAt(Atual).Estaca.EstVirtual < DistProx ? Atual : Poste;
        else return NULL;
      }
    }
    else
    {

      while(Atual && GetAt(Atual).Estaca > Estaca) GetPrev(Atual);

      //--- Parou no anterior então verifica se a distancia com o posterior é menor e retorna 
      //--- a posicao que tiver a menor distancia (a posterior ou a anterior)

      if (Atual)
      {
        POSITION Ante(Atual);

        DistAnte =  Estaca.EstVirtual - GetAt(Atual).Estaca.EstVirtual ;

        GetNext(Atual);

        if(Atual) return GetAt(Atual).Estaca.EstVirtual - Estaca.EstVirtual  < DistAnte ? Atual : Ante;
        else return NULL;
      }
    }
  }

  return Atual;  //=== Achou igual!. atualiza o ponteiro par a estaca atual
}
void CListaSecoes::TaludesDefault()
{
  TaludesAterro.TaludesDefault();
  TaludesCorte.TaludesDefault();
}

CListaSecoes &CListaSecoes::operator=(CListaSecoes &No)
{
  Atual = NULL;
  RemoveAll();

  if (!No.IsEmpty())
  {
    POSITION Temp = No.GetHeadPosition();
    CSecao &SecaoAtual = No.GetNext(Temp);

    while(Temp)
    {
      AddTail(SecaoAtual);
      if (Temp == No.Atual) Atual = GetTailPosition();
      SecaoAtual = No.GetNext(Temp); 
    }
  }
  return *this;
}

POSITION CListaSecoes::PosicionaPrimeira()
{
  Atual = GetHeadPosition();
  return Atual;
}

POSITION CListaSecoes::PosicionaUltima()
{
  Atual = GetTailPosition();
  return Atual;
}

POSITION CListaSecoes::Posiciona(CEstaca& Estaca) 
{
  //  return PosicionaNoMaisProximo(Estaca) == IGUAL ? Atual : NULL;
  //  return PosicionaNoMaisProximo(Estaca) == Atual ? Atual : NULL;
  return PosicionaNoMaisProximo(Estaca);
}

BOOL CListaSecoes::operator++(int i)
{ 
  if (!Atual) Atual = GetHeadPosition();
  else GetNext(Atual); 
  return Atual ? TRUE : FALSE;
}

BOOL CListaSecoes::operator--(int i)
{
  if (!Atual) Atual = GetTailPosition();
  else GetPrev(Atual); 
  return Atual ? TRUE : FALSE;
}

/*
CListaSecoes &CListaSecoes::MontaSecoes(C)
{

POSITION TerrenoAtual(Terreno.GetHeadPosition()),ProjetoAtual(Secoes.GetHeadPosition());


while(AtualMedAnt && AtualMedAtual)
{
//--- Se os perfís são da mesma estaca inclui uma secao na lista de secoes

if (SecaoAnterior.Estaca.EstVirtual == SecaoAtual.Estaca.EstVirtual)
{ 
CSecao Temp(SecaoAnterior,SecaoAtual);
Secoes.AddTail(Temp);
SecaoAnterior.RemoveAll();
SecaoAtual.RemoveAll();
SecaoAnterior = MedicaoAnterior.Perfis.GetNext(AtualMedAnt);
SecaoAtual = MedicaoAtual.Perfis.GetNext(AtualMedAtual);
}
else
{
if (SecaoAnterior.Estaca.EstVirtual > SecaoAtual.Estaca.EstVirtual)
{
SecaoAtual.RemoveAll();
SecaoAtual = MedicaoAtual.Perfis.GetNext(AtualMedAtual);
}
else
{
SecaoAnterior.RemoveAll();
SecaoAnterior = MedicaoAnterior.Perfis.GetNext(AtualMedAnt);
}
}
}

MedicaoAnterior.RemoveAll();
MedicaoAtual.RemoveAll();

//--- Inclui um ponteiro para os taludes nas secoes.
//--- Implementar para ler o arquivo;

if (Secoes.GetCount() < 2) ; //-- abortar explicando . tem que haver no mínimo 2 secoes.
*/

void CListaSecoes::DefRetangDes()
{
  POSITION Atual = GetHeadPosition();

  while(Atual) GetNext(Atual).DefRetangDes();
}

void CListaSecoes::CalculaIs()
{
  POSITION Atual = GetHeadPosition();

  while(Atual) GetNext(Atual).CalculaIs();
}

POSITION CListaSecoes::PosicionaEstaca(double x,double y,double Toleranciax,double Toleranciay,int Tipo)
{
  //--- Posiciona na secao com estaca = x, aceitando a tolerancias x e y

  POSITION EMaisProx(PosicionaNoMaisProximo(x));

  if (!EMaisProx) return NULL;

  CEstaca& EstacaAtual = Tipo == PROJETO || Tipo == MEDICAO ? GetAt(EMaisProx).Estaca : GetAt(EMaisProx).Terreno.Estaca;

  return EstacaAtual.EstVirtual + Toleranciax > x && EstacaAtual.EstVirtual - Toleranciax < x &&
    EstacaAtual.Cota + Toleranciay > y && EstacaAtual.Cota - Toleranciay < y ? Atual = EMaisProx : NULL;
}

void CListaSecoes::CalculaIsLongitudinais()
{
  //--- Calcula os is das retas do terreno.
  //--- Nem todas as seções têm terreno, então é preciso verificar quais as seções têm e calcular os is entre elas

  POSITION Atual(GetHeadPosition());

  if (!Atual || this->GetCount() < 2) return;                 //--- Não tem seções... retorna, nada a calcular.  

  register CSecao* SecAnt = &GetNext(Atual);   //--- Pega a primeira seção.

  register CSecao* SecAtual = &GetNext(Atual);  //--- Pega a segunda seção.

  while(SecAtual)          //-- Enquanto houver seções.            
  {
    if (SecAnt->Terreno.Estaca.EstVirtual != INFINITO && SecAnt->Terreno.Estaca.Cota!= INFINITO)       //---A seção anterior  tem terreno?
    {
      if (SecAtual->Terreno.Estaca.EstVirtual != INFINITO && SecAtual->Terreno.Estaca.Cota != INFINITO)   //--- A seção atual tem terreno?
      {
        //--- Calcula o i e passa para o próximo ponto.

        SecAnt->IPost = SecAtual->IAnte = (SecAtual->Terreno.Estaca.Cota - SecAnt->Terreno.Estaca.Cota) / 
          (SecAtual->Terreno.Estaca - SecAnt->Terreno.Estaca);
        SecAnt = SecAtual;
      }
    }
    else SecAnt = SecAtual;        //--- Passa a seção que não tem terreno.  

    SecAtual = Atual? &GetNext(Atual) : NULL;  //--- pega a próxima reta, se houver
  }
}

void CListaSecoes::LimpaTudo()                    //--- Remove todos os elemntos da lista
{
  RemoveAll();            
  Atual = NULL;
}

