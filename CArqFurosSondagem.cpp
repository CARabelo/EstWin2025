#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqfurossondagem.h"

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

ArqFurosSondagem::ArqFurosSondagem(tlstwstr& pListaDados) : ListaCampos(pListaDados)
{
}

int ArqFurosSondagem::Consiste()
{
  int Erro(-1);

  ittlstwstr It(ListaCampos.begin());

  if(!SuperCString::EEstaca(It->c_str())) Erro = 12;
  It++;
  std::remove(It->begin(),It->end(),' ');
  if(It->empty()) Erro = 13;
  if(!SuperCString::ENumero(It->c_str())) Erro = 13;
  double Cota(atof(It->c_str()));
  if(Cota < -100.0 || Cota > 20000.0) Erro = 13;

  for(It++ ; It != ListaCampos.end() && ! Erro; It++)
  {
    if(!SuperCString::ENumero(It->c_str())) Erro = 2;
    double Prof(atof(It->c_str()));
    if(Prof < 0.0 || Prof > 50.0) Erro = 3;
  }

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqFurosSondagem::CArqFurosSondagem(const std::string& NomeArq): NomeArquivo(NomeArq)
{
  CFile CArqTiposSolos;
  CFileException e;

  NomeArquivo.append(".fso");

  if (CArqTiposSolos.Open(NomeArquivo.c_str(),CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.c_str());
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

void CArqFurosSondagem::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ArqFurosSondagem NoAtual;
    Atual = GetHeadPosition();

    ar << (unsigned int) GetCount();

    for(int QuanReg = GetCount() ; QuanReg > 0 ; QuanReg--)
    {
      try
      {
        NoAtual = GetNext(Atual);

        ar << NoAtual.ListaCampos.size();

        for (ittlstwstr It = NoAtual.ListaCampos.begin() ; It != NoAtual.ListaCampos.end() ; It++)
        {
          ar << *It;
        }
      }
      catch(CArchiveException* e)
      {
          monolog.mensagem(15,CString(" Furos de sondagem : ") + NomeArquivo.c_str(),-1,e->m_cause);

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
      ArqFurosSondagem NoAtual;
      try
      {
        size_t QuanCampos;
        std::string CampoAtual;

        for (ar >> QuanCampos ;  QuanCampos > 0 ; QuanCampos--)
        {
          ar >> CampoAtual;
          NoAtual.ListaCampos.push_back(CampoAtual); 
        }

        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause == CFileException::endOfFile) break;  //--- fim do arquivo?, ok, sai do bloco..
        else
        {
          monolog.mensagem(16,CString(" Furos de Sondagem : ") + NomeArquivo.c_str(),-1,e->m_cause);

          e->Delete();

          break;
        }
      }
    }
  }
}

void CArqFurosSondagem::GravarArquivo()
{
  CFile CArqTiposSolos;
  CFileException e;

  if (CArqTiposSolos.Open(NomeArquivo.c_str(),CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.c_str());
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

