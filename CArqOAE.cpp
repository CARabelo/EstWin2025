#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqoae.h"
#include "Estaca.h"

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

ArqOAE::ArqOAE(tlstwstr& LstDados) : ListaCampos(LstDados)
{
   
}

const ArqOAE& ArqOAE::operator=(const ArqOAE& RHS)
{
  ListaCampos = RHS.ListaCampos;

  return *this;
}

int ArqOAE::Consiste()
{
  int Erro(0);
  CEstaca EstIni,EstFim;
  ittlstwstr It(ListaCampos.begin());

  if(ListaCampos.size() > 3)
  {
    if(!SuperCString::EEstaca(It->c_str())) Erro = 1;
    else 
    {
      EstIni.Constroi(It->c_str());
      It++;
      if(!SuperCString::ENumero(It->c_str())) Erro = 2;
      else
      {
        It++;
        if(!SuperCString::EEstaca(It->c_str())) Erro = 3;
        else
        {
          EstFim.Constroi(It->c_str());
          It++;
          if(!SuperCString::ENumero(It->c_str())) Erro = 4;
          else
          {
            if(EstIni.EstVirtual > EstFim.EstVirtual) Erro = 6;
          }
        }
      }
    }
  }
  else Erro = 5;

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqOAE::CArqOAE(const CString& NomeArq)
{
  CFile ArqOAE;
  CFileException e;

  NomeArquivo = NomeArq + ".oae";

  if (ArqOAE.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqOAE,CArchive::load);
    Serialize(ar);
    ArqOAE.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqOAE::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ArqOAE NoAtual(GetNext(Atual));
    Atual = GetHeadPosition();

    for(int C = 0 ; C < NoAtual.ListaCampos.size() ; C++)
    {
      try
      {
        NoAtual = GetNext(Atual);

        ar << NoAtual.ListaCampos.size();

        for (ittlstwstr It = NoAtual.ListaCampos.begin() ; It !=  NoAtual.ListaCampos.end() ; It++ )
        {
           ar << *It;
        }
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Obras de artes especiais : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();
      }
    }
  }
  else
  {
    unsigned long QuanReg(0);

    for(ar >> QuanReg ; QuanReg > 0 ; QuanReg--)
    {
      try
      {
        std::string CampoAtual;
        size_t QuanCampos(0);
        ArqOAE NoAtual;

        for(ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--)
        {
          ar >> CampoAtual;
          NoAtual.ListaCampos.push_back(CampoAtual);
        }

        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Obras de artes especiais : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();
      }
    }
  }
}

void CArqOAE::GravarArquivo()
{
  CFile ArqOAE;
  CFileException e;

  if (ArqOAE.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqOAE,CArchive::store);
    Serialize(ar);
    ArqOAE.Close();
    ar.Close();
  }
}

