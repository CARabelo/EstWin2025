#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqLimpeza.h"
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

ArqLimpeza::ArqLimpeza(tlstwstr& pListaCampos) : ListaCampos(pListaCampos)
{
}

CArchive& ArqLimpeza::operator<<(CArchive& ar)
{
  ar << ListaCampos.size();

  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}

CArchive& ArqLimpeza::operator>>(CArchive& ar)
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

int ArqLimpeza::Consiste()
{
  int Erro(0);

  if(ListaCampos.size() == 4)
  {
    ittlstwstr It(ListaCampos.begin());

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
          double Valor(atof(It->c_str()));

          if(Valor < 0.0 || Valor > 3.00) Erro = 6;
          else
          {
            It++;
            if(!SuperCString::ENumero(It->c_str())) Erro = 4;
            else
            {
               double Valor(atof(It->c_str()));

               if(Valor < 0.00 || Valor > 30.00) Erro = 7;
            }
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

void CArqLimpeza::Serialize(CArchive& ar)
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
          monolog.mensagem(15,CString(" Limpeza : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    class ArqLimpeza NoAtual;
    do
    {
      try
      {
        NoAtual >> ar;
        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(16,CString(" Limpeza : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }while(true);   //--- Laço infinito, sai nos break
  }
}

CArqLimpeza::CArqLimpeza(CString& NomeArq) 
{
  CFile ArqLimpeza;
  CFileException e;

  NomeArquivo = NomeArq + ".lim";

  if (ArqLimpeza.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (!((CMainFrame*)AfxGetMainWnd())->PegaSilencioso()) 
      monolog.mensagem(14,CString(" Limpeza - ") + NomeArquivo.GetBuffer(1),e.m_cause);

    return;
  }
  else
  {	
    CArchive ar(&ArqLimpeza,CArchive::load);
    Serialize(ar);
    ArqLimpeza.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqLimpeza::GravarArquivo()
{
  CFile ArqLimpeza;
  CFileException e;

  if (ArqLimpeza.Open(NomeArquivo,CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(16,CString(" Limpeza : ") + NomeArquivo.GetBuffer(1),e.m_cause);
    return;
  }
  else
  {
    CArchive ar(&ArqLimpeza,CArchive::store);
    Serialize(ar);
    ArqLimpeza.Close();
    ar.Close();
  }
}

