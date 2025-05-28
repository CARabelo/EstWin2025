#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqTalCorte.h"

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

ArqTalCorte::ArqTalCorte(tlstwstr& pLstCampos):ListaCampos(pLstCampos)
{
}

CArchive& ArqTalCorte::operator>>(CArchive& ar)
{
  size_t QuanCampos;
  std::string CampoAtual;

  for(ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--)
  {
    ar >> CampoAtual;
    ListaCampos.push_back(CampoAtual);
  }

  return ar;
}

CArchive& ArqTalCorte::operator<<(CArchive& ar)
{
  ar << ListaCampos.size();

  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}

int ArqTalCorte::Consiste()
{
  int Erro(0);

  if(ListaCampos.size() > 2)
  {
    ittlstwstr It(ListaCampos.begin());

    SuperCString EstacaFinal(It->c_str());
    It++;
    SuperCString Ig(It++->c_str());
    It++;
    SuperCString Lado(It++->c_str());

    Lado.MakeUpper(); 

    if(!EstacaFinal.EEstaca()) Erro = 1;
    else if(!Ig.ENumero()) Erro = 2;
         else if(Lado != "E" && Lado != "D") Erro = 3;

    if(!Erro)
    {
      bool Par(false);

      for( ; It != ListaCampos.end() && Erro == 0 ; It++) 
      {
        if (!SuperCString(It->c_str()).ENumero()) Erro = 4;
        It++;
        if(It != ListaCampos.end())
        {
         if (!SuperCString(It->c_str()).ENumero()) Erro = 5;
        }

        Par = !Par;
      }

      if(Erro == 0 && (ListaCampos.size() % 2) == 0) Erro = 6;
    }
  }
  else Erro = 1;

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqTalCorte::CArqTalCorte(CString& NomeArq) :Atual(NULL)
{
  CFile ArqTalCorte;
  CFileException e;

  NomeArquivo = NomeArq + ".tac";

  if (ArqTalCorte.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqTalCorte,CArchive::load);
    Serialize(ar);
    ArqTalCorte.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqTalCorte::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << (unsigned int) GetCount();

    for(Atual = GetHeadPosition() ; Atual != NULL ; GetNext(Atual))
    {
      try
      {
        GetAt(Atual) << ar;
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Taludes de corte : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);
        e->Delete();

        break;
      }
    }
  }
  else
  {
    unsigned int QuanReg(0);

    for (ar >> QuanReg ; QuanReg > 0 ; QuanReg--)
    {
      ArqTalCorte NoAtual;
      try
      {
        NoAtual >> ar;
        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Taludes de corte : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);
        e->Delete();

        break;
      }
    }
  }
}

void CArqTalCorte::GravarArquivo()
{
  CFile ArqTalCorte;
  CFileException e;

  if (ArqTalCorte.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqTalCorte,CArchive::store);
    Serialize(ar);
    ArqTalCorte.Close();
    ar.Close();
  }
}

