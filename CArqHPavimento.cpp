#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqhpavimento.h"

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

CArchive& ArqHPavimento::operator<<(CArchive& ar)
{
  size_t QuanCampos(0);
  std::string CampoAtual;

  for(ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--)
  {
    ar >> CampoAtual;
    ListaCampos.push_back(CampoAtual);
  }

  return ar;
}

CArchive& ArqHPavimento::operator>>(CArchive& ar)
{
  ar << ListaCampos.size();

  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}

int ArqHPavimento::Consiste()
{
  int Erro(0);

  ittlstwstr It(ListaCampos.begin());

  if(!SuperCString(It->c_str()).EEstaca()) Erro = 1;
  else 
  {
    It++;
    It++;
    if(!SuperCString(It->c_str()).ENumero()) Erro = 2;
  }

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

void CArqHPavimento::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    Atual = GetHeadPosition();
    ar << (unsigned int) GetCount();

    for( Atual = GetHeadPosition() ; Atual != NULL ; Atual++)
    {
      try
      {
        GetNext(Atual) >> ar;
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Expessuras do pavimento : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    unsigned int QuanReg(0);

    for(ar >> QuanReg ; QuanReg > 0 ; QuanReg--)
    {
      ArqHPavimento NoAtual;

      try
      {
        NoAtual << ar;
        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(16,CString(" Expessuras do pavimento : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
}

CArqHPavimento::CArqHPavimento(CString& NomeArq,CString& Ext) 
{
  CFile ArqHPavimento;
  CFileException e;

  NomeArquivo = NomeArq + Ext;

  if (ArqHPavimento.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqHPavimento,CArchive::load);
    Serialize(ar);
    ArqHPavimento.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqHPavimento::GravarArquivo()
{
  CFile ArqHPavimento;
  CFileException e;

  if (ArqHPavimento.Open(NomeArquivo,CFile::modeCreate | CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqHPavimento,CArchive::store);
    Serialize(ar);
    ArqHPavimento.Close();
    ar.Close();
  }
}
