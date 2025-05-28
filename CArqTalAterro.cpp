#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqtalAterro.h"
#include <sstream>
#include <iomanip>

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

ArqTalAterro::ArqTalAterro(tlstwstr& pLstCampos):ListaCampos(pLstCampos)
{
}

CArchive& ArqTalAterro::operator>>(CArchive& ar)
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

CArchive& ArqTalAterro::operator<<(CArchive& ar)
{
  ar << ListaCampos.size();

  for(ittlstwstr It = ListaCampos.begin() ; It != ListaCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}

int ArqTalAterro::Consiste()
{
  int Erro(0);

  if(ListaCampos.size() > 2)
  {
    ittlstwstr It(ListaCampos.begin());

    CString EstacaFinal(It->c_str());
    It++;
    CString Ig(It->c_str());
    It++;
    CString Lado(It->c_str());

    Lado.MakeUpper(); 

    if(!SuperCString::EEstaca(EstacaFinal)) Erro = 1;
    else if(!SuperCString::ENumero(Ig)) Erro = 2;
    else if(Lado != "E" && Lado != "D") Erro = 3;

    if(!Erro)
    {
      It++;

      double xAnterior(INFINITO);
      std::stringstream ValorFormatado;

      ValorFormatado << std::right << std::fixed << std::setfill(' ') << std::setprecision(2);

      //--- Passa o x e y do primeiro ponto deve ser 0,00

      if (!SuperCString(It->c_str()).ENumero()) Erro = 4;
      else
      {
        if(atof(It->c_str()) != 0.0) Erro = 9;
        else
        {
          ValorFormatado << atof(It->c_str());
          *It = ValorFormatado.str();
        }

        It++;

        if (!Erro)
        {
          if(!SuperCString(It->c_str()).ENumero()) Erro = 5;
          else
          {
            if (atof(It->c_str()) != 0.0) Erro = 9;
            else
            {
              ValorFormatado.str("");
              ValorFormatado << atof(It->c_str());
              *It = ValorFormatado.str();
            }
          }
        }
      }

      bool Par(false);

      for( ; It != ListaCampos.end() && Erro == 0 ; It++) 
      {
        if (!SuperCString(It->c_str()).ENumero())
        {
          if(!Par) Erro = 4;
          else Erro = 5;
        }

        if(!Erro)
        {
          if(Par) 
          {
            if(Lado == "E" && !(atof(It->c_str()) < -0.001)) Erro = 7;
            else if(Lado == "D" && (atof(It->c_str()) < 0.001))  Erro = 7;
          }
        }

        //--- Formata o campo

        if (!Erro)
        {
          if (Par)
          {
            ValorFormatado.str("");
            ValorFormatado << std::setprecision(2);
            ValorFormatado << atof(It->c_str());
            *It = ValorFormatado.str();
          }
          else
          {
            ValorFormatado.str("");
            ValorFormatado << std::setprecision(3);
            ValorFormatado << atof(It->c_str());
            *It = ValorFormatado.str();
          }
        }

        Par = !Par;
      }
    }
    if(Erro == 0 && (ListaCampos.size() % 2) == 0) Erro = 6;
  }

  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqTalAterro::CArqTalAterro(CString& NomeArq) :Atual(NULL)
{
  CFile ArqTalAterro;
  CFileException e;

  NomeArquivo = NomeArq + ".taa";

  if (ArqTalAterro.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqTalAterro,CArchive::load);
    Serialize(ar);
    ArqTalAterro.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqTalAterro::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    ar << (unsigned int) GetCount();

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
          monolog.mensagem(15,CString(" Taludes de aterro : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

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
      ArqTalAterro NoAtual;

      try
      {
        NoAtual >> ar;
        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Taludes de aterro : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
}

void CArqTalAterro::GravarArquivo()
{
  CFile ArqTalAterro;
  CFileException e;

  if (ArqTalAterro.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqTalAterro,CArchive::store);
    Serialize(ar);
    ArqTalAterro.Close();
    ar.Close();
  }
}

