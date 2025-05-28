#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include <set>
#include "CTipoSolos.h"
#include "carqtipossolos.h"

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
//----------------------------------------------------------------
//--- Implementação da lista

CArqTiposSolos::CArqTiposSolos(const CString& NomeArq)
{
  CFile CArqTiposSolos;
  CFileException e;

  NomeArquivo = NomeArq + ".tps";

  if (CArqTiposSolos.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&CArqTiposSolos,CArchive::load);
    Serialize(ar);
    CArqTiposSolos.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqTiposSolos::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    CTipoSolo NoAtual;
    Atual = GetHeadPosition();

    unsigned long QuanReg(GetCount());

    ar << QuanReg;

    for ( ; QuanReg > 0 ; QuanReg--)
    {
      ar << NoAtual.ListaCampos.size();

      try
      {
        NoAtual = GetNext(Atual);
        for(ittlstwstr It = NoAtual.ListaCampos.begin() ; It != NoAtual.ListaCampos.end() ; It++)
        {
          ar << *It;
        }
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Tipos de solos : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    unsigned long QuanReg(0);
    
    for(ar >> QuanReg ; QuanReg > 0 ; QuanReg--)
    {
      CTipoSolo NoAtual;

      try
      {
        std::string CampoAtual;
        size_t QuanCampos(0);

        for(ar >> QuanCampos ; QuanCampos > 0  ; QuanCampos-- )
        {
          ar >> CampoAtual;
          NoAtual.ListaCampos.push_back(CampoAtual);
        }

        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(16,CString(" Tipos de solos : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
}

void CArqTiposSolos::GravarArquivo()
{
  CFile CArqTiposSolos;
  CFileException e;

  if (CArqTiposSolos.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&CArqTiposSolos,CArchive::store);
    Serialize(ar);
    CArqTiposSolos.Close();
    ar.Close();
  }
}

void CArqTiposSolos::CriaSet(tysetCTipoSolo& SetArqSolos)
{
  CTipoSolo NoAtual;

  SetArqSolos.clear();

  for(Atual = GetHeadPosition() ; Atual!= NULL ; GetNext(Atual))  
  {
    if(GetAt(Atual).ListaCampos.size() == 4)
    {
      ittlstwstr It(GetAt(Atual).ListaCampos.begin());

      std::string Codigo,Descricao,Traco,Cor;
      Codigo = *It++;
      Descricao = *It++;
      Traco = *It++;
      Cor = *It;

      SetArqSolos.insert(CTipoSolo(Codigo,Descricao,Traco,Cor));
    }
  }
}