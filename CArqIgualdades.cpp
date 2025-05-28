#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqIgualdades.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1

extern class monologo monolog;

//--- Implementação do nó da lista.

int ArqIgualdade::Consiste()
{
  int Erro(0);

  if (!Estaca1.EEstaca()) Erro = 1;
  else if (!Estaca2.EEstaca()) Erro = 2;

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

void CArqIgualdade::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ArqIgualdade NoAtual;
    Atual = GetHeadPosition();

    while (Atual)
    {
      try
      {
        NoAtual = GetNext(Atual);
        ar << NoAtual.Estaca1 << NoAtual.Estaca2; 
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(16,CString(" Expessuras do pavimento : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    class ArqIgualdade NoAtual;
    do
    {
      try
      {
        ar >> NoAtual.Estaca1 >> NoAtual.Estaca2; 

        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(16,CString(" Expessuras do pavimento : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }while(true);   //--- Laço infinito, sai nos break
  }
}

CArqIgualdade::CArqIgualdade(CString& NomeArq) 
{
  CFile ArqIgualdade;
  CFileException e;

  NomeArquivo = NomeArq;

  if (ArqIgualdade.Open(NomeArq,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqIgualdade,CArchive::load);
    Serialize(ar);
    ArqIgualdade.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqIgualdade::GravarArquivo()
{
  CFile ArqIgualdade;
  CFileException e;

  if (ArqIgualdade.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqIgualdade,CArchive::store);
    Serialize(ar);
    ArqIgualdade.Close();
    ar.Close();
  }
}

