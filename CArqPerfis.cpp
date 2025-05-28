#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "math.h"
#include "estaca.h"
#include "carqPerfis.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1
#define X          0
#define Y          1

extern class monologo monolog;


//--- Implementação do nó da lista.
/*
ArqPerfil::ArqPerfil()
{
}
*/
//----------------------------------------------------------------
//--- Implementação da lista

CArqPerfil::CArqPerfil(CString& NomeArq)
{
  if  (NomeArq.IsEmpty()) return;
  LeArquivo(NomeArq);
}

void CArqPerfil::LeArquivo(CString& NomeArq)
{
  CFile ArqPerfil;
  CFileException e;

  // LeTerrenoAtual();
  //return;
  NomeArquivo = NomeArq + ".tca";

  if (ArqPerfil.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
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


void CArqPerfil::GravaArquivo(CString& NomeArq)
{
  CFile ArqPerfil;
  CFileException e;

  NomeArquivo = NomeArq + ".tca";

  if (ArqPerfil.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqPerfil,CArchive::store);
    Serialize(ar);
    ar.Close();
    ArqPerfil.Close();

  }
}

void CArqPerfil::Serialize(CArchive& ar)
{
  class ArqPerfil NoAtual;

  if (ar.IsStoring())
  {
    Atual = GetHeadPosition();

    try
    {
      ar << GetCount();

      for(int Nitens = GetCount(); Nitens ; Nitens--)
      {
        NoAtual = GetNext(Atual);
        ar << NoAtual.Tipo << NoAtual.Ponto[X] << NoAtual.Ponto[Y]; 
      }
    }
    catch(CArchiveException* e)
    {
      if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
        monolog.mensagem(15,NomeArquivo.GetBuffer(1),-1,e->m_cause);

      e->Delete();
    }
  }
  else
  {
    try
    {
      unsigned int Nitens;

      for(ar >> Nitens ; Nitens ; Nitens--)
      {
        ar >> NoAtual.Tipo >> NoAtual.Ponto[X] >> NoAtual.Ponto[Y]; 
        AddTail(NoAtual);
      }
    }
    catch(CArchiveException* e)
    { 
      if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
        monolog.mensagem(16,NomeArquivo.GetBuffer(1),-1,e->m_cause);

      e->Delete();
    }	
  }
}

/*
int CArqPerfil::LeTerrenoAtual()
{
_fcloseall();

CString NomeArquivo = "c:\\estradas\\canal01.tca";

CFileException CFe;
int Lidos,Tam = sizeof(struct ter);
CFile ArqMedAtu;
ArqMedAtu.Open(NomeArquivo,CFile::modeRead | CFile::typeBinary,&CFe);
if (CFe.m_cause != CFileException::none)
{
return FALSE;
}

struct ter MedAtu;
Lidos = ArqMedAtu.Read(&MedAtu,Tam);

do
{  

TerrenoAtual.Inclui(MedAtu.dados.estaca.valor,0);
TerrenoAtual.Atual->IncluiPonto(0.0,MedAtu.dados.ponto.y);  //-- Deve ser a x da linha base!!!!!

while((Lidos = ArqMedAtu.Read(&MedAtu,Tam)) && MedAtu.tipo != 'e')
{
TerrenoAtual.Atual->IncluiPonto(MedAtu.dados.ponto.x,MedAtu.dados.ponto.y);
}

TerrenoAtual.Atual->Pontos->Ordena();

}while (Lidos == Tam);

ArqMedAtu.Close();
NomeArquivo = "c:\\estradas\\teste2.tca";
CFile Arq;
Arq.Open(NomeArquivo,CFile::modeCreate | CFile::typeBinary |CFile::modeWrite,&CFe);

CArchive ar(&Arq,CArchive::store);

ar << TerrenoAtual.Nitens;

int N;

for(N = //TerrenoAtual.Nitens

100, TerrenoAtual.Atual = TerrenoAtual.Primeira ; N ; N--,TerrenoAtual++)
{
ar <<	TerrenoAtual.Atual->EstVirtual;
TerrenoAtual.Atual->Pontos->Nitens = 2;

ar << TerrenoAtual.Atual->Pontos->Nitens;
int  Nitens2;
for(Nitens2 = TerrenoAtual.Atual->Pontos->Nitens,TerrenoAtual.Atual->Pontos->Atual = TerrenoAtual.Atual->Pontos->Primeiro ; Nitens2 ; Nitens2--,TerrenoAtual.Atual->Pontos->Atual = TerrenoAtual.Atual->Pontos->Atual->Proximo)
{
if (Nitens2 == 2) TerrenoAtual.Atual->Pontos->Atual->Ponto[0] = -1.0;
else	TerrenoAtual.Atual->Pontos->Atual->Ponto[0] = 1.0;

TerrenoAtual.Atual->Pontos->Atual->Ponto[1] + 1.0;
ar << TerrenoAtual.Atual->Pontos->Atual->Ponto[0] << TerrenoAtual.Atual->Pontos->Atual->Ponto[1]; 
}
}

ar.Close();
Arq.Close();
return TRUE;
*/
/*
ar << TerrenoAtual.Nitens;

int N;


for(N = TerrenoAtual.Nitens, TerrenoAtual.Atual = TerrenoAtual.Primeira ; N ; N--,TerrenoAtual++)
{
ar <<	TerrenoAtual.Atual->EstVirtual;
ar << TerrenoAtual.Atual->Pontos->Nitens;
int  Nitens2;
for(Nitens2 = TerrenoAtual.Atual->Pontos->Nitens,TerrenoAtual.Atual->Pontos->Atual = TerrenoAtual.Atual->Pontos->Primeiro ; Nitens2 ; Nitens2--,TerrenoAtual.Atual->Pontos->Atual = TerrenoAtual.Atual->Pontos->Atual->Proximo)
{
ar << TerrenoAtual.Atual->Pontos->Atual->Ponto[0] << TerrenoAtual.Atual->Pontos->Atual->Ponto[1]; 
}
}

ar.Close();
Arq.Close();
return TRUE;

}

*/