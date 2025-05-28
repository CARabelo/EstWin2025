 #include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include <set>
#include "CArqAciTopograficos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1

extern class monologo monolog;

//----------------------------------------------------------------
//--- Implementação do nó da lista.

ArqAciTopograficos::ArqAciTopograficos(CString& Icone,CString& Codigo,CString& DescCurta,CString& DescLonga,CString& ForcarAresta) 
{
  ListaCampos.push_back(Icone.GetBuffer());
  ListaCampos.push_back(Codigo.GetBuffer());
  ListaCampos.push_back(DescCurta.GetBuffer());
  ListaCampos.push_back(DescLonga.GetBuffer());
  ListaCampos.push_back(ForcarAresta.GetBuffer());
}

ArqAciTopograficos::ArqAciTopograficos(tlstwstr& LstDados):ListaCampos(LstDados)
{
}

CArchive& ArqAciTopograficos::operator<<(CArchive& ar)
{
  ar << ListaCampos.size();

  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}

CArchive& ArqAciTopograficos::operator>>(CArchive& ar)
{
  std::string CampoAtual;
  size_t QuanCampos(0);

  for (ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--) 
  {
    ar >> CampoAtual;
    ListaCampos.push_back(CampoAtual);
  }

  return ar;
}

int ArqAciTopograficos::Consiste()
{
  int Erro = 0;

  if(ListaCampos.size() == 4)
  {
    ittlstwstr It(ListaCampos.begin()) ;

    if(!SuperCString::EEstaca(It->c_str())) Erro = 1;
    else
    {
      It++;
      if(!SuperCString::ENumero(It->c_str())) Erro = 2;
      else
      {
        It++;
        if(!SuperCString::ENumero(It->c_str())) Erro = 3;
        else
        {
          It++;
          if(!SuperCString::ENumero(It->c_str())) Erro = 4;
        }
      }
    }
  }

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

void CArqAciTopograficos::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << (unsigned long) GetSize();

    Atual = GetHeadPosition();

    while (Atual)
    {
      try
      {
        GetNext(Atual) << ar;
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Acidentes Topográficos : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    unsigned long QuanReg(0);

    for( ar >> QuanReg ; QuanReg > 0 ; QuanReg--)
    {
      ArqAciTopograficos NoAtual;
      try
      {
        NoAtual >> ar;
        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
       
       monolog.mensagem(16,CString(" Acidentes Topográficos : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
}

CArqAciTopograficos::CArqAciTopograficos(CString& NomeArq) 
{
  CFile ArqVerticais;
  CFileException e;

  NomeArquivo = NomeArq.Left(NomeArq.ReverseFind('\\'));
  NomeArquivo = NomeArq.Left(NomeArquivo.ReverseFind('\\'));
  std::string NomeProjeto(NomeArquivo.Right(NomeArquivo.GetLength() - NomeArquivo.ReverseFind('\\')-1));

  NomeArquivo += "\\";
  NomeArquivo += NomeProjeto.c_str();
  NomeArquivo += ".ato";

  if (ArqVerticais.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqVerticais,CArchive::load);
    Serialize(ar);
    Atual = GetHeadPosition();
  }
}

void CArqAciTopograficos::GravarArquivo()
{
  CFile ArqVerticais;
  CFileException e;

  if (ArqVerticais.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
  }
  else
  {
    CArchive ar(&ArqVerticais,CArchive::store);
    Serialize(ar);
    ar.Close();
    ArqVerticais.Close();
  }
}

