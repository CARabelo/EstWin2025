#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqoac.h" 
#include "cponto.h"
#include "clpontos.h"
#include "coac.h"

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
#define SO_X       0

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementa��o do n� da lista.

const class OAC &OAC::operator=(const class OAC &No)
{
  Estaca = No.Estaca;
  Descricao = No.Descricao;
  return *this;
}

//----------------------------------------------------------------------------------
//--- Implementa��o da lista
//--- Neste objeto s�o criadas duas listas na mem�ria, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto � gen�rico,
//--- as duas listas ficam na memoria e devem ser destru�das pelo recipiente 
//--- a medida do necess�rio. Estas listas s�o din�micas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as altera��es feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

COAC::COAC(const CString& NomeArquivo) : CArqOAC(NomeArquivo),Atual(NULL)
{
  if(Consiste()) throw -1;

  if (IsEmpty()) return;  //--- Lista vazia? retorna.

  OAC OACAtual;

  //--- Para cada os nos  da lista de Curvas lidas cria um n� na lista
  //--- de curvas calculadas, calculando i i da tangente que � a menor rampa das retas da
  //--- se�ao.

  POSITION RegAtual(GetHeadPosition());

  while(RegAtual)
  {
    ArqOAC NoArqAtual(GetNext(RegAtual));

    if(NoArqAtual.ListaCampos.size() > 1)
    {
      ittlstwstr It(NoArqAtual.ListaCampos.begin());

      CString Estaca(It->c_str());
      It++;
      CString Ig(It->c_str());

      OACAtual.Estaca.Constroi(Estaca,CString(""),Ig);
      OACAtual.Estaca.Cota = INFINITO;
      It++;
      OACAtual.Descricao = It->c_str();

      InsOrdenada(OACAtual);
    }		
  }			
}
int COAC::Compara(class OAC &No1,class OAC &No2)
{
  //--- Compara 2 n�s da lista.

  if (No1.Estaca.EstVirtual < No2.Estaca.EstVirtual) return MENOR;
  else if (No1.Estaca.EstVirtual > No2.Estaca.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void COAC::InsOrdenada(class OAC &No)
{
  //--- Insere um n� ordenadamente na lista.
  //--- O ponteiro para o atual � a chave da insers�o, o metodo presume que o n�
  //--- a ser inserido est� pr�ximo do atual, a posic�o correta do n� �
  //--- procurada a partir do no atual. para tr�s ou para frente. 

  Atual = GetHeadPosition();

  if (!Atual) LOAC.AddHead(No);  //--- Se a lista esta vazia , insere na cabe�a.
  else
  {
    int Res;

    //--- Se o no atual for maior que o a inserir procura para tr�s 
    //--- Sen�o se o no atual for menor que o a inserir procura para a frente
    //--- Sen�o se o n� atual for igual ao no a inserir insere o n�  depois do atual

    if ((Res = Compara(LOAC.GetAt(Atual),No)) == MAIOR)
    {
      LOAC.GetPrev(Atual); 

      while (Atual && Compara(LOAC.GetPrev(Atual),No) == MAIOR);

      if (!Atual) LOAC.AddHead(No);
      else LOAC.InsertAfter(Atual,No);
    }
    else 
    {
      if (Res == MENOR)
      {
        LOAC.GetNext(Atual); 

        while (Atual && Compara(LOAC.GetNext(Atual),No) == MENOR );

        if (!Atual) LOAC.AddTail(No);
        else LOAC.InsertBefore(Atual,No);

      }
      else LOAC.InsertAfter(Atual,No);
    }
  }
}

int COAC::PosicionaNoMaisProximo(CEstaca Estaca,int Direcao)
{
  Atual = LOAC.GetHeadPosition();
  if (!Atual) return MENOR;

  CEstaca EstacaAtual = LOAC.GetAt(Atual).Estaca;

  Direcao = FRENTE;

  if (Direcao == FRENTE)
  {
    while(Atual && EstacaAtual < Estaca)
    {
      LOAC.GetNext(Atual);
      if (Atual) EstacaAtual = LOAC.GetAt(Atual).Estaca;
    }
  }

  if (EstacaAtual > Estaca) return MAIOR;
  if (EstacaAtual < Estaca) return MENOR;
  return IGUAL;
}

void  COAC::PegaEstacasObrigatorias(CList<CEstaca,CEstaca&>& EstObrig)
{
  Atual = LOAC.GetHeadPosition();

  while(Atual) EstObrig.AddTail((LOAC.GetNext(Atual)).Estaca);
}


bool COAC::Consiste()
{
  POSITION Atual = GetHeadPosition();
  int i(0),Erro(0);

  while(Atual && Erro == 0)
  {
    i++;
    Erro = GetNext(Atual).Consiste();

    if (Erro)
    {
      CString Mensg;

      Mensg.Format("Erro no arquivo de obras de arte correntes: o registro %d est� invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}

POSITION  COAC::PosicionaEstaca(double x,double y,double Toleranciax,double Toleranciay,int Tipo)
{
   //--- Posiciona na secao com estaca = x, aceitando a tolerancias x e y

  int EMaisProx(PosicionaNoMaisProximo(x));

  if (!Atual) return NULL;

  const CEstaca& EstacaAtual(LOAC.GetAt(Atual).Estaca);

  if(Tipo == SO_X)  return EstacaAtual.EstVirtual + Toleranciax > x && EstacaAtual.EstVirtual - Toleranciax < x ? Atual : NULL;

  return EstacaAtual.EstVirtual + Toleranciax > x && EstacaAtual.EstVirtual - Toleranciax < x &&
         EstacaAtual.Cota + Toleranciay > y && EstacaAtual.Cota - Toleranciay < y ? Atual : NULL;
}

bool COAC::GravarArquivo()
{
  CFile ArqOAC;
  CFileException e;

  if (ArqOAC.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(13,NomeArquivo.GetBuffer(1));
    return false;
  }
  else
  {	
    CArchive ar(&ArqOAC,CArchive::store);
    Serialize(ar);
    ar.Close();
    ArqOAC.Close();
    Atual = GetHeadPosition();
  }

  return true;
};
