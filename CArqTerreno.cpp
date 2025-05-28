#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqterreno.h"

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

CArchive& ArqTerreno::operator>>(CArchive& ar)
{
  ar >> Tipo >> Ponto[X] >> Ponto[Y];

  return ar;
}

CArchive& ArqTerreno::operator<<(CArchive& ar)
{
  ar << Tipo << Ponto[X] << Ponto[Y];

  return ar;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqTerreno::CArqTerreno(CString& NomeArq) :Atual(NULL)
{
  CFile ArqTerreno;
  CFileException e;

  NomeArquivo = NomeArq + ".tca";

  if (ArqTerreno.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqTerreno,CArchive::load);
    Serialize(ar);
    ArqTerreno.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqTerreno::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    Atual = GetHeadPosition();

    while (Atual)
    {
      try
      {
        GetNext(Atual) << ar;
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(15,CString(" Terreno : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    do
    {
      try
      {
        class ArqTerreno NoAtual;
        NoAtual >> ar;
        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(16,CString(" Terreno : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }while(true);   //--- Laço infinito, sai nos break
  }
}


void CArqTerreno::GravarArquivo()
{
  CFile ArqTerreno;
  CFileException e;

  if (ArqTerreno.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqTerreno,CArchive::store);
    Serialize(ar);
    ArqTerreno.Close();
    ar.Close();
  }
}

