#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqsecoesAvulsas.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "math.h"
#include "estaca.h"

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

int CRegSecoesAvulsas::Consiste()
{
  int Erro(0);

  return Erro;

  if (Nome.GetLength() == 0) Erro = 1;
  if(!XInicial.ENumero()) Erro = 3;
  else if(!YInicial.ENumero()) Erro = 4;
    else if(!XFinal.ENumero()) Erro = 5;
      else if(!YFinal.ENumero()) Erro = 6;
        else if(!Azimute.ENumero()) Erro = 7;
          else
          {
            double Az= atof(Azimute.GetBuffer());
            if(Az < 0.0 || Az >359.5959) Erro = 8;
          }
  return Erro;
}

CRegSecoesAvulsas::CRegSecoesAvulsas(CString* Dados)
{
  Nome = Dados;
  YInicial = (Dados+1);
  XInicial = (Dados+2);
  YFinal = (Dados+3);
  XFinal = (Dados+4);
  Azimute = (Dados+5);
  Distancia = (Dados+6);
}

CArchive& CRegSecoesAvulsas::operator>>(CArchive& ar)
{
  ar >> Nome >> YInicial >> XInicial >> YFinal >> XFinal >> Azimute >> Distancia;

  return ar;
}

CArchive& CRegSecoesAvulsas::operator<<(CArchive& ar)
{
  ar << Nome<< YInicial << XInicial << YFinal << XFinal << Azimute << Distancia;

  return ar;
}

const CRegSecoesAvulsas& CRegSecoesAvulsas::operator=(const CRegSecoesAvulsas& RHS)
{
  Nome = RHS.Nome;
  XInicial = RHS.XInicial;
  YInicial = RHS.YInicial;
  XFinal = RHS.XFinal;
  YFinal = RHS.YFinal;
  Azimute = RHS.Azimute;
  Distancia = RHS.Distancia;

  return *this;
}

//----------------------------------------------------------------
//--- Implementação da lista

CArqSecoesAvulsas::CArqSecoesAvulsas(CString& NomeArq) 
{
  CFile ArqSecAvulsas;
  CFileException e;

  NomeArquivo = NomeArq + ".pea";

  if (ArqSecAvulsas.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (!((CMainFrame*)AfxGetMainWnd())->PegaSilencioso()) 
      monolog.mensagem(14,CString(" Secoes-Tipo: ") + NomeArquivo.GetBuffer(1),e.m_cause);

    return;
  }
  else
  {	
    CArchive ar(&ArqSecAvulsas,CArchive::load);
    Serialize(ar);
    ArqSecAvulsas.Close();
    ar.Close();
    Atual = GetHeadPosition();
  }
}

void CArqSecoesAvulsas::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    Atual = GetHeadPosition();

    while (Atual)
    {
      try
      {
        class CRegSecoesAvulsas NoAtual(GetNext(Atual));

        ar << NoAtual.Nome << NoAtual.YInicial << NoAtual.XInicial << NoAtual.YFinal
           << NoAtual.XFinal << NoAtual.Azimute << NoAtual.Distancia; 
      }
      catch(CArchiveException* e)
      {
        monolog.mensagem(16,CString(" Seções tipo : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }
  }
  else
  {
    do
    {
      try
      {
        class CRegSecoesAvulsas NoAtual;

       ar >> NoAtual.Nome >> NoAtual.YInicial >> NoAtual.XInicial >> NoAtual.YFinal
          >> NoAtual.XFinal >> NoAtual.Azimute >> NoAtual.Distancia;  

        AddTail(NoAtual);
      }
      catch(CArchiveException* e)
      {
        if (e->m_cause != CArchiveException::endOfFile)   //--- fim do arquivo?, ok, sai do bloco..
          monolog.mensagem(15,CString(" Secoes Avulsas : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

        e->Delete();

        break;
      }
    }while(true);   //--- Laço infinito, sai nos break
  }
}

bool CArqSecoesAvulsas::RemoveReg(const CString& pNome)
{
  for(Atual = GetHeadPosition(); Atual ; GetNext(Atual))
  {
    if(GetAt(Atual).Nome == pNome) break;
  }

  if(Atual == NULL) return false;
  
  RemoveAt(Atual);

  return true;
}

