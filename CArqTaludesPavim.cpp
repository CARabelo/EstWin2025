#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "cArqTaludesPavim.h"

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

ArqTaludesPavim::ArqTaludesPavim(tlstwstr& LstDados):ListaCampos(LstDados)
{
}

int ArqTaludesPavim::Consiste()
{
  int Erro(0);

  ittlstwstr It(ListaCampos.begin());

  if(ListaCampos.size() > 4)
  {
    if(!SuperCString(It->c_str()).EEstaca()) Erro = 1;
    else 
    { 
      It++; 
      if(!SuperCString(It->c_str()).ENumero()) Erro = 2;
    }

    for (int i = 0 ; i < 4 && !Erro; i++) 
    {
      if(!SuperCString(It->c_str()).ENumero()) Erro = 3+i;
      It++;
    }
  }
  else Erro = 7;

  return Erro;
}

CArchive& ArqTaludesPavim::operator>>(CArchive& ar)
{
  std::string CampoAtual;
   size_t QuanCampos(0);
 
  for( ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--)
  {
    ar >> CampoAtual;
    ListaCampos.push_back(CampoAtual);
  }

  return ar;
}

CArchive& ArqTaludesPavim::operator<<(CArchive& ar)
{
  ar << ListaCampos.size();

  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqTaludesPavim::CArqTaludesPavim(CString& NomeArq) :Atual(NULL)
{
  CFile ArqTaludesPavim;
  CFileException e;

  NomeArquivo = NomeArq + ".tpa";

  if (ArqTaludesPavim.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqTaludesPavim,CArchive::load);
    Serialize(ar);
    ArqTaludesPavim.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqTaludesPavim::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << (unsigned long) GetCount();

    Atual = GetHeadPosition();

    for (unsigned long C = 0 ; C < GetCount() ; C++)
    {
      try
      {
        GetNext(Atual) << ar;
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Taludes do pavimento : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    ArqTaludesPavim NoAtual;
    unsigned long QuanReg(0);
    
    for (ar >> QuanReg ;  QuanReg > 0 ;  QuanReg--) 
    {
      try
      {
        NoAtual >> ar;
        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(16,CString(" Taludes do pavimento : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
}

void CArqTaludesPavim::GravarArquivo()
{
  CFile ArqTaludesPavim;
  CFileException e;

  if (ArqTaludesPavim.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqTaludesPavim,CArchive::store);
    Serialize(ar);
    ArqTaludesPavim.Close();
    ar.Close();
  }
}

