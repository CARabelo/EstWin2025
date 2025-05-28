#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqCanteiros.h"

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

CArchive& ArqCanteiros::operator>>(CArchive& ar)
{
 unsigned long QuanReg(0);
 size_t QuanCampos(0);
 std::string CampoAtual;
 
 for (ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--)
 {
   ar >> CampoAtual;
   LstCampos.push_back(CampoAtual);
 }

 return ar;
}

CArchive& ArqCanteiros::operator<<(CArchive& ar)
{
  ar << LstCampos.size();

  for(ittlstwstr It = LstCampos.begin() ; It != LstCampos.end() ; It++)
  {
    ar << *It;
  }

  return ar;
}

ArqCanteiros::ArqCanteiros(tlstwstr& LstDados)
{
  LstCampos = LstDados;
}

int ArqCanteiros::Consiste()
{
  int Erro(0);

  ittlstwstr ItCampoAtual(LstCampos.begin()),ItxAnterior(ItCampoAtual);

  if(LstCampos.size() > 0)
  {
    SuperCString EstacaFinal((ItCampoAtual++)->c_str());
    if (!EstacaFinal.EEstaca()) Erro = 1;
    else
    {
      if(LstCampos.size() == 1) 
      {
        LstCampos.push_back("0");       //--- Digitou só a estaca, simula a igualdade
        ItCampoAtual = LstCampos.begin();
        ItCampoAtual++;
      } 
      else
      {
        if(LstCampos.size() < 4) Erro = 9;
        else
        {
          if(!SuperCString((ItCampoAtual)->c_str()).ENumero()) Erro = 2;
          else
          {
            bool Par(false);

            ++ItCampoAtual;
            if(!SuperCString((ItCampoAtual)->c_str()).ENumero()) 
            {
              Erro = 3;
              return Erro;
            }

            double xAnterior(atof((ItCampoAtual)->c_str()));

            for(ItCampoAtual++ ; ItCampoAtual != LstCampos.end() && Erro == 0 ; ItCampoAtual++)
            {
              if(!SuperCString((ItCampoAtual)->c_str()).ENumero()) 
              {
                if (!Par) Erro = 3;
                else Erro = 4;
              }
              else
              {
                if(Par)
                {
                  double xAtual(atof(ItCampoAtual->c_str()));
                  if((xAtual - xAnterior) < 1e-3) Erro = 10;
                  xAnterior = xAtual;
                }
              }
            }
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

CArqCanteiros::CArqCanteiros(CString& NomeArq) :Atual(NULL)
{
  CFile ArqCanteiros;
  CFileException e;

  NomeArquivo = NomeArq + ".can";

  if (ArqCanteiros.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound) monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {	
    CArchive ar(&ArqCanteiros,CArchive::load);
    Serialize(ar);
    ArqCanteiros.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqCanteiros::Serialize(CArchive& ar)
{
  unsigned long QuanReg(this->GetCount()); 
  size_t QuanCampos(0);

  if (ar.IsStoring())
  {
    ar << QuanReg;

    for( ; QuanReg > 0 ; QuanReg--)
    {
      GetNext(Atual) >> ar;
    }
  }
  else
  {
    ar >> QuanReg;
    
    for( ; QuanReg > 0 ; QuanReg--)
    {
      ArqCanteiros NoAtual;

      NoAtual >> ar;
      AddTail(NoAtual);
    }
  }

}

void CArqCanteiros::GravarArquivo()
{
  CFile ArqCanteiros;
  CFileException e;

  if (ArqCanteiros.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqCanteiros,CArchive::store);
    Serialize(ar);
    ArqCanteiros.Close();
    ar.Close();
  }
}

