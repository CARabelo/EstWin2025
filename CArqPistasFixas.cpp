#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqpistasfixas.h"
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

int ArqPistasFixas::Consiste()
{
  int Erro(0);

  ittlstwstr ItCampoAtual(LstCampos.begin()), ItxAnterior(ItCampoAtual);

  if (LstCampos.size() > 0)
  {
    SuperCString EstacaFinal((ItCampoAtual++)->c_str());
    if (!EstacaFinal.EEstaca()) Erro = 1;
    else
    {
      if (LstCampos.size() == 1)
      {
        LstCampos.push_back("0");       //--- Digitou só a estaca, simula a igualdade
        ItCampoAtual = LstCampos.begin();
        ItCampoAtual++;
      }
      else
      {
        if (LstCampos.size() < 4) Erro = 9;
        else
        {
          if (!SuperCString((ItCampoAtual)->c_str()).ENumero()) Erro = 2;
          else
          {
            bool Par(false);

            ++ItCampoAtual;
            if (!SuperCString((ItCampoAtual)->c_str()).ENumero())
            {
              Erro = 3;
              return Erro;
            }

            double xAnterior(atof((ItCampoAtual)->c_str()));

            for (ItCampoAtual++; ItCampoAtual != LstCampos.end() && Erro == 0; ItCampoAtual++)
            {
              if (!SuperCString((ItCampoAtual)->c_str()).ENumero())
              {
                if (!Par) Erro = 3;
                else Erro = 4;
              }
              else
              {
                if (Par)
                {
                  double xAtual(atof(ItCampoAtual->c_str()));
                  if ((xAtual - xAnterior) < 1e-3) Erro = 10;
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

CArqPistasFixas::CArqPistasFixas(CString& NomeArq) : Erro(0)
{
  CFile ArqPistasFixas;
  CFileException e;

  NomeArquivo = NomeArq + ".fix";

  if (ArqPistasFixas.Open(NomeArquivo, CFile::modeRead | CFile::shareDenyNone, &e) == 0)
  {
   // if (!((CMainFrame*)AfxGetMainWnd())->PegaSilencioso())
    //  monolog.mensagem(14, CString(" Pistas Fixas: ") + NomeArquivo.GetBuffer(1), e.m_cause);

    return;
  }
  else
  {
    if (ArqPistasFixas.GetLength() != 0)
    {
      CArchive ar(&ArqPistasFixas, CArchive::load);
      Serialize(ar);
      ArqPistasFixas.Close();
      ar.Close();
    }
    Atual = GetHeadPosition();
  }
}

void CArqPistasFixas::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    try
    {
      ar << (unsigned long)GetCount();

      for (POSITION Atual = GetHeadPosition(); Atual != NULL; GetNext(Atual))
      {
        ArqPistasFixas NoAtual(GetAt(Atual));

        ar << NoAtual.LstCampos.size();

        for (ittlstwstr It = NoAtual.LstCampos.begin(); It != NoAtual.LstCampos.end(); It++)
        {
          ar << *It;
        }
      }
    }
    catch (CArchiveException* e)
    {
  //    monolog.mensagem(16, CString(" Pistas Fixas : ") + NomeArquivo.GetBuffer(1), -1, e->m_cause);

      e->Delete();
    }
  }
  else
  {
    try
    {
      unsigned long QuanRegistros;

      for (ar >> QuanRegistros; QuanRegistros > 0; QuanRegistros--)
      {
        std::string CampoAtual;
        size_t QuanCampos(0);

        ArqPistasFixas NoAtual;

        for (ar >> QuanCampos; QuanCampos > 0; QuanCampos--)
        {
          ar >> CampoAtual;
          NoAtual.LstCampos.push_back(CampoAtual);
        }

        AddTail(NoAtual);
      }
    }
    catch (CArchiveException* e)
    {
  //    monolog.mensagem(15, CString(" Pistas Fixas : ") + NomeArquivo.GetBuffer(1), -1, e->m_cause);

      e->Delete();
    }
  }
}
