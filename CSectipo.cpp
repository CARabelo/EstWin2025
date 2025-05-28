#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqsectipo.h" 
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "csectipo.h"
#include "math.h"

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

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas listas são dinâmicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

SecTipo::SecTipo() {} 

SecTipo::SecTipo(SecTipo& SecaoTip){ *this = SecaoTip;}

const SecTipo& SecTipo::operator=(const SecTipo &No)
{
  EstFinal = No.EstFinal;
  SE = No.SE;
  CLPontos::operator=(No);

  return* this;
}

void SecTipo::IgualPerfil(const Perfil& Perf)
{
  EstFinal = Perf.Estaca;
  CLPontos::operator=(Perf);
}

CSecTipo::CSecTipo(CString& pNomeArquivo) : CArqSecTipo(pNomeArquivo),LSecTipo(),Atual(NULL),NomeArquivo(pNomeArquivo),Erro(0)
{
  Consiste();

  CEstaca EstacaAtual;
  double EstacaAnterior(INFINITO);
  POSITION At(GetHeadPosition());

  if(Erro == 0)
  {
    //--- Para cada os nos  da lista de Secoes lidas cria um nó na lista

    while (At)
    {
      ArqSecTipo &NoArqAtual(GetNext(At));

      ittlstwstr ItCampoAtual(NoArqAtual.LstCampos.begin());

      CString Estaca(ItCampoAtual->c_str());
      ItCampoAtual++;
      CString Ig(ItCampoAtual->c_str());

      SecTipo SecTipoAtual;
      SecTipoAtual.EstFinal.Constroi(Estaca,"",Ig);

      for(ItCampoAtual++ ; ItCampoAtual != NoArqAtual.LstCampos.end() ; ItCampoAtual++)
      {
        SuperCString Coordx(ItCampoAtual->c_str());
        ItCampoAtual++;
        SuperCString Coordy(ItCampoAtual->c_str());

        if (Coordx.ENumero() == 1 && Coordy.ENumero())
        {
          Ponto PontoAtual(atof(Coordx),atof(Coordy),INFINITO,INFINITO,INFINITO,INFINITO,CString("SECTIP"));
          SecTipoAtual.InsereOrdenada(PontoAtual);
        }
      }

      SecTipoAtual.GetHead().Descricao = "BORPIE";
      SecTipoAtual.GetTail().Descricao = "BORPID";

      SecTipoAtual.CalculaIs();
      InsOrdenada(SecTipoAtual);
    }
  }									 
  Atual = LSecTipo.GetHeadPosition();
}

bool CSecTipo::Constroi(std::string& pNomeArquivo)
{
  NomeArquivo = pNomeArquivo.c_str();
// CArqSecTipo(pNomeArquivo) //--- Acertar

  Consiste();

  CEstaca EstacaAtual;
  double EstacaAnterior(INFINITO);
  POSITION At(GetHeadPosition());

  if (Erro == 0)
  {
  //--- Para cada os nos  da lista de Secoes lidas cria um nó na lista

    while (At)
    {
      ArqSecTipo &NoArqAtual(GetNext(At));

      ittlstwstr ItCampoAtual(NoArqAtual.LstCampos.begin());

      CString Estaca(ItCampoAtual->c_str());
      ItCampoAtual++;
      CString Ig(ItCampoAtual->c_str());

      SecTipo SecTipoAtual;
      SecTipoAtual.EstFinal.Constroi(Estaca, "", Ig);

      for (ItCampoAtual++; ItCampoAtual != NoArqAtual.LstCampos.end(); ItCampoAtual++)
      {
        SuperCString Coordx(ItCampoAtual->c_str());
        ItCampoAtual++;
        SuperCString Coordy(ItCampoAtual->c_str());

        if (Coordx.ENumero() == 1 && Coordy.ENumero())
        {
          Ponto PontoAtual(atof(Coordx), atof(Coordy), INFINITO, INFINITO, INFINITO, INFINITO, CString("SECTIP"));
          SecTipoAtual.InsereOrdenada(PontoAtual);
        }
      }

      SecTipoAtual.GetHead().Descricao = "BORPIE";
      SecTipoAtual.GetTail().Descricao = "BORPID";

      SecTipoAtual.CalculaIs();
      InsOrdenada(SecTipoAtual);
    }
  }
  Atual = LSecTipo.GetHeadPosition();

  return true;
}

int CSecTipo::Compara(class SecTipo &No1,class SecTipo &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstFinal.EstVirtual < No2.EstFinal.EstVirtual) return MENOR;
  else if (No1.EstFinal.EstVirtual > No2.EstFinal.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CSecTipo::InsOrdenada(class SecTipo &No)
{
  //--- Insere um nó ordenadamente na lista.
  //--- O ponteiro para o atual é a chave da insersão, o metodo presume que o nó
  //--- a ser inserido está próximo do atual, a posicão correta do nó é
  //--- procurada a partir do no atual. para trás ou para frente. 

  Atual = LSecTipo.GetHeadPosition();

  if (!Atual) 
  {
    LSecTipo.AddHead(No);  //--- Se a lista esta vazia , insere na cabeça.

    Atual = LSecTipo.GetHeadPosition();
  }
  else
  {
    int Res;

     Atual = LSecTipo.GetHeadPosition();

    //--- Se o no atual for maior que o a inserir procura para trás 
    //--- Senão se o no atual for menor que o a inserir procura para a frente
    //--- Senão se o nó atual for igual ao no a inserir insere o nó  depois do atual

    if ((Res = Compara(LSecTipo.GetAt(Atual),No)) == MAIOR)
    {
      LSecTipo.GetPrev(Atual); 

      while (Atual && Compara(LSecTipo.GetPrev(Atual),No) == MAIOR);

      if (!Atual) LSecTipo.AddHead(No);
      else LSecTipo.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LSecTipo.GetNext(Atual); 

        while (Atual && Compara(LSecTipo.GetNext(Atual),No) == MENOR );

        if (!Atual) LSecTipo.AddTail(No);
        else LSecTipo.InsertBefore(Atual,No);

      }
      else LSecTipo.InsertAfter(Atual,No);
    }
  }
}

int CSecTipo::PosicionaNoMaisProximo(const CEstaca& Estaca,int Direcao)
{
  Atual = NULL;

  if (!Atual) Atual = LSecTipo.GetHeadPosition();
  if (!Atual) return MENOR;

 // if (!(Direcao = Estaca > LSecTipo.GetAt(Atual).EstFinal ? FRENTE : Estaca < LSecTipo.GetAt(Atual).EstFinal ? FRENTE : FALSE)) return IGUAL;

  Direcao = FRENTE;

  if (Direcao == FRENTE)
  {
    while(Atual && LSecTipo.GetAt(Atual).EstFinal.EstVirtual < Estaca.EstVirtual) 
    {
      LSecTipo.GetNext(Atual);
    }

    if (!Atual) Atual = LSecTipo.GetTailPosition();
  }
  else
  {
    LSecTipo.GetPrev(Atual);

    while(Atual && LSecTipo.GetAt(Atual).EstFinal > Estaca) LSecTipo.GetNext(Atual);

    if (!Atual) Atual = LSecTipo.GetHeadPosition();
  }

  if (!Atual || LSecTipo.GetAt(Atual).EstFinal < Estaca) return MENOR;
  if (LSecTipo.GetAt(Atual).EstFinal > Estaca) return MAIOR;
  return IGUAL;
}

bool CSecTipo::BuscaSecTipo(CEstaca& Estaca,SecTipo& SecaoABuscar)
{
  if (PosicionaNoMaisProximo(Estaca) == MENOR) return FALSE;
  SecaoABuscar = LSecTipo.GetAt(Atual);

  return TRUE;
}

bool CSecTipo::BuscaLargurasPistas(const CEstaca& Estaca,double& LargEsq,double &LargDir)
{
  LargEsq = LargDir = 0.0;

  if (PosicionaNoMaisProximo(Estaca) == MENOR) return FALSE;
  SecTipo& Secao = LSecTipo.GetAt(Atual);

  LargEsq = Secao.GetHead().x;
  LargDir = Secao.GetTail().x;

  return true;
}

bool CSecTipo::Consiste()
{
  POSITION Atual = GetHeadPosition();
  int i(0);

  if(!Atual) return true;

  while(Atual && Erro == 0)
  {
    i++;
    Erro = GetNext(Atual).Consiste();

    if (Erro)
    {
      CString Mensg;

      Mensg.Format("Erro no arquivo de seções tipo: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosSecTipo[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}
