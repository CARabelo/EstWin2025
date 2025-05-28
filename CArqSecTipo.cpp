#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqsectipo.h"
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

int ArqSecTipo::Consiste()
{
  SuperCString strEstaca,strIg;
  ittlstwstr It(LstCampos.begin());

  if(LstCampos.size() > 2)
  {
    strEstaca = (It++)->c_str();
    strIg = (It++)->c_str();
  }

  int Erro(0);

  if (!strEstaca.EEstaca()) Erro = 1;
  else 
  {
    if(!strIg.ENumero()) Erro = 2;
    else 
    {
      if(LstCampos.size() < 4) Erro = 5;
      else if (LstCampos.size() % 2 != 0) Erro = 6;

      bool Par(false);

      for (;It != LstCampos.end() && !Erro ; It++)
      {
        if(!(SuperCString(It->c_str())).ENumero())
        {
          Erro = !Par ? 3 : 4; 
        }
        Par = !Par;
      }
    }
  }

  return Erro;
}
//----------------------------------------------------------------
//--- Implementação da lista

CArqSecTipo::CArqSecTipo(CString& NomeArq) : Erro(0)
{
  CFile ArqSecTipo;
  CFileException e;

  NomeArquivo = NomeArq + ".sec";

  if (ArqSecTipo.Open(NomeArquivo,CFile::modeRead | CFile::shareDenyNone,&e) == 0)
  {
    if (!((CMainFrame*)AfxGetMainWnd())->PegaSilencioso()) 
      monolog.mensagem(14,CString(" Secoes-Tipo: ") + NomeArquivo.GetBuffer(1),e.m_cause);

    return;
  }
  else
  {	
    if(ArqSecTipo.GetLength() != 0)
    { 
      CArchive ar(&ArqSecTipo,CArchive::load);
      Serialize(ar);
      ArqSecTipo.Close();
      ar.Close();
    }
    Atual = GetHeadPosition();
  }
}

void CArqSecTipo::Serialize(CArchive& ar)
{
  if (ar.IsStoring())
  {
    try
    {
      ar << (unsigned long) GetCount();
  
      for(POSITION Atual = GetHeadPosition() ;  Atual != NULL ; GetNext(Atual))
      {
        ArqSecTipo NoAtual(GetAt(Atual));
        
        ar <<  NoAtual.LstCampos.size();

        for (ittlstwstr It = NoAtual.LstCampos.begin() ;  It != NoAtual.LstCampos.end() ;  It++)
        {
          ar << *It;
        }
      }
    } 
    catch(CArchiveException* e)
    {
      monolog.mensagem(16,CString(" Seções tipo : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

      e->Delete();
    }
  }
  else
  {
    try
    {
      unsigned long QuanRegistros;

      for(ar >> QuanRegistros ; QuanRegistros > 0 ; QuanRegistros--) 
      {
        std::string CampoAtual;
        size_t QuanCampos(0);

        ArqSecTipo NoAtual;
      
        for(ar >> QuanCampos ; QuanCampos > 0 ; QuanCampos--)
        {
          ar >> CampoAtual;
          NoAtual.LstCampos.push_back(CampoAtual);
        }

        AddTail(NoAtual);
      }
    }
    catch(CArchiveException* e)
    {
      monolog.mensagem(15,CString(" Seções Tipo : ") + NomeArquivo.GetBuffer(1),-1,e->m_cause);

      e->Delete();
    }
  }
}
