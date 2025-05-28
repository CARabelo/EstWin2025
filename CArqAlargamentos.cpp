#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqalargamentos.h"

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

ArqAlargamento::ArqAlargamento(tlstwstr& pLstCampos)
{
  for(ittlstwstr& It = pLstCampos.begin() ; It != pLstCampos.end() ; It++)
  {
    LstCampos.push_back(*It);
  }
}

CArchive& ArqAlargamento::operator>>(CArchive& ar)
{
  std::string CampoAtual;
  size_t QuanCampos(0);

  LstCampos.clear();

  for(ar >> QuanCampos ; QuanCampos!= 0 ; QuanCampos--)
  {
    ar >> CampoAtual;
    LstCampos.push_back(CampoAtual);
  }

  return ar;
}

CArchive& ArqAlargamento::operator<<(CArchive& ar)
{
  ar << LstCampos.size();

  for(ittlstwstr It = LstCampos.begin() ; It != LstCampos.end() ; It++)
  {
    ar << *It;  
  }

  return ar;
}

int ArqAlargamento::Consiste()
{
  int Erro(0);

  ittlstwstr ItCampoAtual(LstCampos.begin()),ItxAnterior(ItCampoAtual);

  if(LstCampos.size() > 0)
  {
    SuperCString EstacaFinal((ItCampoAtual++)->c_str());
    if (!EstacaFinal.EEstaca()) Erro = 1;
    else
    {
      if(LstCampos.size() > 2) 
      {
        ItCampoAtual++;                              //---- Pula igualdade

        CString strLado(ItCampoAtual->c_str());
        strLado.MakeUpper();

        if(strLado != "E" && strLado != "D") Erro = 3;
        else 
        {
          if(LstCampos.size() != 3)                  //---- Pode ter so estaca ig e lado,  sem pontos
          {
            if(LstCampos.size() > 6)                 //--- Mas se tiver pontos tem q ter pelo menos 2 (x e y)
            {
              bool Par(false);

              ++ItCampoAtual;

              if(!SuperCString((ItCampoAtual)->c_str()).ENumero()) 
              {
                Erro = 4;
                return Erro;
              }

              double xAnterior(atof((ItCampoAtual)->c_str()));

              for(ItCampoAtual++ ; ItCampoAtual != LstCampos.end() && Erro == 0 ; ItCampoAtual++)
              {
                Par = !Par;

                if(!SuperCString((ItCampoAtual)->c_str()).ENumero()) 
                {
                  if (!Par) Erro = 4;
                  else Erro = 5;
                } 
                else
                {
                  if(!Par)
                  {
                    double xAtual(atof(ItCampoAtual->c_str()));
                    if((strLado == "D" && (xAtual - xAnterior) < 1e-3) || (strLado == "E" && (xAtual - xAnterior) > -1e-3)) Erro = 7;
                    xAnterior = xAtual;
                  }
                }
              }
            }
            else Erro = 8;
          }
        }
      }
      else Erro = 3;
    }
  }
  else Erro = 1;
  
  return Erro;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqAlargamento::CArqAlargamento(CString& NomeArq) :Atual(NULL)
{
  CFile ArqAlargamento;
  CFileException e;

  NomeArquivo = NomeArq + ".als";

  if (ArqAlargamento.Open(NomeArquivo,CFile::modeRead,&e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)  { monolog.mensagem(1, NomeArquivo.GetBuffer(1));}
    return;
  }
  else
  {	
    CArchive ar(&ArqAlargamento,CArchive::load);
    Serialize(ar);
    ArqAlargamento.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqAlargamento::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    unsigned long QuanReg(GetCount());

    ar << QuanReg;

    for(Atual = GetHeadPosition() ; Atual != NULL ; GetNext(Atual))
    {
      try
      {
        ArqAlargamento& NoAtual(GetAt(Atual));
        NoAtual << ar;
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(15,CString(" Alargamentos : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    try
    {
      unsigned long QuanReg(0);

      for (ar >> QuanReg ; QuanReg > 0  ; QuanReg--)
      {
        ArqAlargamento NoAtual;

        NoAtual >> ar;
        AddTail(NoAtual);
      }
    }
    catch(CArchiveException* e)
    {
      monolog.mensagem(15,CString(" Alargamentos : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

      e->Delete();
    }
  }
}

void CArqAlargamento::GravarArquivo()
{
  CFile ArqAlargamento;
  CFileException e;

  if (ArqAlargamento.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return;
  }
  else
  {
    CArchive ar(&ArqAlargamento,CArchive::store);
    Serialize(ar);
    ArqAlargamento.Close();
    ar.Close();
  }
}