#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqoac.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO   1.7976931348623158e+308
#define ANTERIOR   0
#define POSTERIOR  1

extern class monologo monolog;

//--- Implementação do nó da lista

ArqOAC::ArqOAC(tlstwstr& pLstCampos):ListaCampos(pLstCampos)
{
}


CArchive& ArqOAC::operator>>(CArchive& ar)
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

CArchive& ArqOAC::operator<<(CArchive& ar)
{
  ar << ListaCampos.size();

  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}
int ArqOAC::Consiste()
{
  int Erro(0);

  if(ListaCampos.size() > 1)
  {
    ittlstwstr It(ListaCampos.begin());

    if(!SuperCString::EEstaca(It->c_str())) Erro = 1;
    else
    {
      It++;
     if(!SuperCString::ENumero(It->c_str())) Erro = 2;
    }
  }

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqOAC::CArqOAC(const CString& NomeArq)
{
  CFile ArqOAC;
  CFileException e;

  NomeArquivo = NomeArq + ".oac";

  if (ArqOAC.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqOAC,CArchive::load);
    Serialize(ar);
    ArqOAC.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqOAC::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << (unsigned long) GetCount();

    for(Atual = GetHeadPosition() ; Atual != NULL ; GetNext(Atual))
    {
      try
      {
       GetAt(Atual) << ar;
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Obras de artes correntes : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    unsigned long QuanReg(0);

    for (ar >> QuanReg ; QuanReg > 0 ; QuanReg--)
    {
      ArqOAC NoAtual;

      try
      {
        NoAtual >> ar;

        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(16,CString(" Obras de artes correntes : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
}

void CArqOAC::GravarArquivo()
{
  CFile ArqOAC;
  CFileException e;

  if (ArqOAC.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqOAC,CArchive::store);
    Serialize(ar);
    ArqOAC.Close();
    ar.Close();
  }
}

