#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "cponto.h"
#include "clpontos.h"
#include <string>
#include "CSecaoAvulsa.h"
#include "CArqSecoesAvulsas.h" 
#include <map>
//#include"CPonto.h"
#include "CSecoesAvulsas.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENOR        -1
#define MAIOR         1
#define IGUAL         0
#define TRAS         -1
#define FRENTE        1
#define LER           1
#define GRAVAR        2

extern class monologo monolog;

//----------------------------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas listas são dinâmicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

/*
CSecoesAvulsas::CSecoesAvulsas(CString& NomeArquivo) : CArqSecoesAvulsas(NomeArquivo)
{
  Atual = NULL;
  POSITION At = GetHeadPosition();

  if (Consiste()) throw -1;

  //--- Para cada os nos  da lista de Curvas lidas cria um nó na lista

  while (At)
  {
    CRegSecoesAvulsas NoArqAtual = GetNext(At);
   
    if (NoArqAtual.XInicial.ENumero() && NoArqAtual.XFinal.ENumero() &&  NoArqAtual.YInicial.ENumero() && NoArqAtual.YFinal.ENumero())
    {
      InsereOrdenada(CSecaoAvulsa(NoArqAtual.Nome.GetBuffer(),NoArqAtual.XInicial.GetBuffer(),NoArqAtual.YInicial.GetBuffer(),NoArqAtual.XFinal.GetBuffer(),
		             NoArqAtual.YFinal.GetBuffer(),NoArqAtual.Azimute.GetBuffer(),NoArqAtual.Distancia.GetBuffer()),true);
    }
  }									 
 // PassaSecoesParaMap();

  Atual = LSecoesAvulsas.GetHeadPosition();
}
*/

CSecoesAvulsas::CSecoesAvulsas(CString& NomeArquivo) : Atual(NULL)
{
	Serialize(NomeArquivo.GetBuffer(), 1);
}

void CSecoesAvulsas::Serialize(const std::string& NomeProjeto,int Operacao)
{
  CFile ArqSecAvulsas;
  CFileException e;
  int Flags((Operacao == 2 ? (CFile::modeCreate | CFile::modeWrite) : CFile::modeRead) | CFile::shareDenyNone);

  std::string NomeArquivo(NomeProjeto + ".pea");    //--- Perfil Avulso
 
  if (ArqSecAvulsas.Open(NomeArquivo.c_str(),Flags,&e) == 0)
  {
 //   monolog.mensagem(1,NomeArquivo.c_str());
    return ;
  }
  else
  {
    int FlagsAr(Operacao == LER ? CArchive::load : CArchive::store); 

    CArchive ar(&ArqSecAvulsas,FlagsAr);

		if (Operacao == LER)
		{
			unsigned int  QuanReg;

			for (ar >> QuanReg; QuanReg > 0; QuanReg--)
			{
				CSecaoAvulsa SecAvulsaAtual;

				SecAvulsaAtual.Serialize(ar);
        MapSecoesAvulsasP1.insert(pairPontoSecAvulsa(SecAvulsaAtual.PInicial, LSecoesAvulsas.AddTail(SecAvulsaAtual)));
			}
		}
		else
		{
			ar << (unsigned int) LSecoesAvulsas.GetCount();

			for (POSITION Atual = LSecoesAvulsas.GetHeadPosition(); Atual != NULL; LSecoesAvulsas.GetNext(Atual))
			{
				LSecoesAvulsas.GetAt(Atual).Serialize(ar);
			}
		}
  }
}

int CSecoesAvulsas::Compara(class CSecaoAvulsa &No1,class CSecaoAvulsa &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.Nome < No2.Nome) return MENOR;
  else if (No1.Nome > No2.Nome) return MAIOR;
  else return IGUAL; 
}

void CSecoesAvulsas::InsereOrdenada(CSecaoAvulsa &No,int INSERIRIGUAL)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  POSITION PSecAvInserida(NULL);

  //if (!Atual) Atual = LSecoesAvulsas.GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista

  Atual = LSecoesAvulsas.GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista


  if (!Atual) PSecAvInserida = LSecoesAvulsas.AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LSecoesAvulsas.GetAt(Atual),No)) == MAIOR)
    {
      LSecoesAvulsas.GetPrev(Atual); 

      while (Atual && Compara(LSecoesAvulsas.GetPrev(Atual),No) == MAIOR);

      if (!Atual) 
      { 
        PSecAvInserida = LSecoesAvulsas.AddHead(No);   //-- Se voltou a lista toda, adiciona na cabeça.
        Atual = LSecoesAvulsas.GetHeadPosition();
      }
      else PSecAvInserida = LSecoesAvulsas.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
     {
       LSecoesAvulsas.GetNext(Atual); 

       while (Atual && Compara(LSecoesAvulsas.GetNext(Atual),No) == MENOR);

       if (!Atual) 
       {
         PSecAvInserida = LSecoesAvulsas.AddTail(No);	//--- Se passou a lista toda adiciona no fim.
         Atual = LSecoesAvulsas.GetTailPosition();
       }
       else PSecAvInserida = LSecoesAvulsas.InsertBefore(Atual,No);
     }
      else if (INSERIRIGUAL) PSecAvInserida = LSecoesAvulsas.InsertAfter(Atual,No);
    }
  }
  if (PSecAvInserida) MapSecoesAvulsasP1.insert(mapSecoesAvulsas::value_type(No.PInicial, PSecAvInserida));
}
/*
void CSecoesAvulsas::InsereOrdenadaArq(class CSecaoAvulsa &No,int INSERIRIGUAL)
{
  CRegSecoesAvulsas RegArqSecAvulsas;

  //--- Passa a secao calculada para o registo de seção avulsa digitada.

  std::string NomeTemp(No.Nome);

  RegArqSecAvulsas.Nome = NomeTemp.c_str();
  RegArqSecAvulsas.XInicial.Format("%6.3lf",No.PInicial.x);
  RegArqSecAvulsas.YInicial.Format("%6.3lf",No.PInicial.y);
  RegArqSecAvulsas.XFinal.Format("%6.3lf",No.PFinal.x);
  RegArqSecAvulsas.YFinal.Format("%6.3lf",No.PFinal.y);
  RegArqSecAvulsas.Azimute.Format("%3.4lf",No.Azimute);
  RegArqSecAvulsas.Distancia.Format("%6.3lf",No.Distancia);

  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  POSITION Atual = GetHeadPosition();  //--- Se atual esta nulo volta para o inicio da lista

  if (!Atual)AddHead(RegArqSecAvulsas);  //--- Se a lista esta vazia , insere na cabeça.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = GetAt(Atual).Nome.Compare(RegArqSecAvulsas.Nome)) == MAIOR)
    {
      GetPrev(Atual); 

      while (Atual && (GetPrev(Atual).Nome.Compare(RegArqSecAvulsas.Nome)) == MAIOR);

      if (!Atual) 
      { 
        AddHead(RegArqSecAvulsas);   //-- Se voltou a lista toda, adiciona na cabeça.
        Atual = GetHeadPosition();
      }
      else InsertAfter(Atual,RegArqSecAvulsas);
    }
    else 
    {
      if (Res == MENOR)
     {
       GetNext(Atual); 

       while (Atual && (GetNext(Atual).Nome.Compare(RegArqSecAvulsas.Nome)) == MENOR);

       if (!Atual) 
       {
         AddTail(RegArqSecAvulsas);	//--- Se passou a lista toda adiciona no fim.
         Atual = GetTailPosition();
       }
       else InsertBefore(Atual,RegArqSecAvulsas);
     }
      else if (INSERIRIGUAL) InsertAfter(Atual,RegArqSecAvulsas);
    }
  }
}
*/
int CSecoesAvulsas::PosicionaNoMaisProximo(std::string& Nome,int Direcao)
{
//  if (!Atual) Atual = LSecoesAvulsas.GetHeadPosition();
  Atual = LSecoesAvulsas.GetHeadPosition();
  if (!Atual) return MENOR;   //-- Menor = não achou, lista vazia


  while (Atual && LSecoesAvulsas.GetAt(Atual).Nome < Nome) LSecoesAvulsas.GetNext(Atual);

  if (!Atual) return MENOR;

  return IGUAL;







 // if ((Direcao = Nome < LSecoesAvulsas.GetAt(Atual).Nome ? TRAS : 
  //               Nome > LSecoesAvulsas.GetAt(Atual).Nome ? FRENTE : IGUAL) == IGUAL) return IGUAL;



  /*
  Direcao = FRENTE;

  if (Direcao == FRENTE)
  {
    LSecoesAvulsas.GetNext(Atual);

    while(Atual && LSecoesAvulsas.GetAt(Atual).Nome < Nome) LSecoesAvulsas.GetNext(Atual);

    if (!Atual) Atual = LSecoesAvulsas.GetTailPosition();
  }
  else
  {
    LSecoesAvulsas.GetPrev(Atual);

    while(Atual && LSecoesAvulsas.GetAt(Atual).Nome > Nome) LSecoesAvulsas.GetNext(Atual);

    if (!Atual) Atual = LSecoesAvulsas.GetHeadPosition();
  }

  if (LSecoesAvulsas.GetAt(Atual).Nome > Nome) return MAIOR;
  if (LSecoesAvulsas.GetAt(Atual).Nome < Nome) return MENOR;

  return IGUAL;
  */
}

bool CSecoesAvulsas::Consiste()
{
	/*
  POSITION Atual = GetHeadPosition();
  int i(0),Erro(0);

  while(Atual && Erro == 0)
  {
    i++;
    Erro = GetAt(Atual).Consiste();

    if (Erro)
    {
      CString Mensg;

      Mensg.Format("Erro no arquivo de SecaoAvulsas: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }

    GetNext(Atual);
  }

  return Erro > 0;
	*/

	return 0;
}

bool CSecoesAvulsas::SerializeSecoesCalculadas(const std::string& NomeProj,int Operacao)
{
  CFile ArqSecAvCalculadas;
  CFileException e;
  int Flags(Operacao == LER ? CFile::modeRead : (CFile::modeWrite | CFile::modeCreate) | CFile::shareDenyNone);
  std::string NomeArquivo(NomeProj + ".sac");    //--- secao avulsa calculada
 
  if (ArqSecAvCalculadas.Open(NomeArquivo.c_str(),Flags,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.c_str());
    return false;
  }
  else
  {
    int FlagsAr(Operacao == LER ? CArchive::load : CArchive::store); 

    CArchive ar(&ArqSecAvCalculadas,FlagsAr);

    for(Atual = LSecoesAvulsas.GetHeadPosition() ; Atual != NULL ; LSecoesAvulsas.GetNext(Atual))
    {
      CSecaoAvulsa& SecAtual(LSecoesAvulsas.GetAt(Atual)); 

   //   if (ar.IsStoring()) SecAtual.Estaca >> ar;
   //   else SecAtual.Estaca << ar; 

      SecAtual.Serialize(ar);
    }

    ar.Close();
    ArqSecAvCalculadas.Close();
  }

  //--- Passas as seçoes calculadas para o arquivo digitado
	/*
  RemoveAll();

  CRegSecoesAvulsas RegArqSecAvulsas;
  
  for(Atual = LSecoesAvulsas.GetHeadPosition() ; Atual != NULL ; LSecoesAvulsas.GetNext(Atual))
  {
    CSecaoAvulsa SecAv(LSecoesAvulsas.GetAt(Atual));

    RegArqSecAvulsas.Nome = SecAv.Nome.c_str();
    RegArqSecAvulsas.XInicial.Format("%6.3lf",SecAv.PInicial.x);
    RegArqSecAvulsas.YInicial.Format("%6.3lf",SecAv.PInicial.y);
    RegArqSecAvulsas.XFinal.Format("%6.3lf",SecAv.PFinal.x);
    RegArqSecAvulsas.YFinal.Format("%6.3lf",SecAv.PFinal.y);
    RegArqSecAvulsas.YFinal.Format("%6.3lf",SecAv.PFinal.y);
    RegArqSecAvulsas.Azimute.Format("%3.4lf",SecAv.Azimute);
    RegArqSecAvulsas.Distancia.Format("%6.3lf",SecAv.Distancia);

    AddTail(RegArqSecAvulsas);
  }
  Serialize(NomeProj,Operacao);
	*/

  return true;
}
/*
void CSecoesAvulsas::PassaSecoesParaMap()
{
  POSITION Atual(LSecoesAvulsas.GetHeadPosition());

  while(Atual)
  {
    CSecaoAvulsa& SecTemp(LSecoesAvulsas.GetAt(Atual));

    MapSecoesAvulsas.insert(mapSecoesAvulsas::value_type(SecTemp.PEstaca,SecTemp));
    LSecoesAvulsas.GetNext(Atual);
  }
}
*/