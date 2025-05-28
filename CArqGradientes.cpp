#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqgradientes.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

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

ArqGradiente::ArqGradiente(tlstwstr& pLstDados):ListaCampos(pLstDados)
{
}

CArchive& ArqGradiente::operator<<(CArchive& ar)
{
  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++) ar << *It;

  return ar;
}

CArchive& ArqGradiente::operator>>(CArchive& ar)
{
  size_t QuanCampos(0);
  std::string CampoAtual;

  for(ar >> QuanCampos ; QuanCampos != 0 ; QuanCampos--)
  {
    ar >> CampoAtual;
    ListaCampos.push_back(CampoAtual);
  }
  
  return ar;
}

int ArqGradiente::Consiste(int TipoArq)
{
  int Erro(0);

  if(ListaCampos.size() >= 3)
  {
    ittlstwstr It(ListaCampos.begin());
    if (!SuperCString::EEstaca(It->c_str())) Erro = 1;
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
          double Valor(atof(It->c_str()));

          switch (TipoArq)
          {
            case 0:  break;    //--- Tem q verificar na seção tipo, sera verificado na view 
            case 1:  if(Valor < 0.10 || Valor > 5.00) Erro = 4; break;
            case 2:  if(Valor < 1.00 || Valor > 90.00) Erro = 4; break;

          }
        }
      }
    }
  }
  else Erro = 1;

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

void CArqGradiente::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    Atual = GetHeadPosition();

    ar << (unsigned long) GetCount();

    while (Atual)
    {
      try
      {
        GetNext(Atual) << ar;
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(15,CString(" Gradientes : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    unsigned long Count(0);

    for (ar >> Count ; Count > 0 ; Count--)
    {
      ArqGradiente NoAtual;

      try
      {
        NoAtual >> ar;

        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
          monolog.mensagem(16,CString(" Gradientes : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
}

CArqGradiente::CArqGradiente(CString& NomeArq) 
{
  CFile ArqGradiente;
  CFileException e;

  NomeArquivo = NomeArq + ".gra";

  if (ArqGradiente.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (!((CMainFrame*)AfxGetMainWnd())->PegaSilencioso()) 
      monolog.mensagem(14,CString(" Gradientes - ") + NomeArquivo.GetBuffer(1),e.m_cause);

    return;
  }
  else
  {	
    CArchive ar(&ArqGradiente,CArchive::load);
    Serialize(ar);
    ArqGradiente.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqGradiente::GravarArquivo()
{
  CFile ArqGradiente;
  CFileException e;

  if (ArqGradiente.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(16,CString(" Gradientes : ") + NomeArquivo.GetBuffer(1),e.m_cause);
    return;
  }
  else
  {
    CArchive ar(&ArqGradiente,CArchive::store);
    Serialize(ar);
    ArqGradiente.Close();
    ar.Close();
  }
}

