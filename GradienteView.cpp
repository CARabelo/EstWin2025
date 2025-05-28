// CurHorView.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "CEditEx.h"
#include "Estaca.h"
#include "carqsectipo.h" 
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "csectipo.h"
#include "gradienteview.h"
#include "supercstring.h"
#include "carqgradientes.h"
#include "monologo.h"
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

extern monologo monolog;

#define PRIMEIRO_CAMPO  2
#define ULTIMO_CAMPO  2

/////////////////////////////////////////////////////////////////////////////
// CGradienteView

IMPLEMENT_DYNCREATE(CGradienteView, CTypedPtrListView)

CGradienteView::CGradienteView() : CTypedPtrListView(CGradienteView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO), EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT2),EdUltimoCampo(ListaCampos.end(),-1),
                                   SecoesTipo(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()))  
{
}

CGradienteView::~CGradienteView()
{
}

void CGradienteView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,6);
}

BEGIN_MESSAGE_MAP(CGradienteView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
  ON_BN_CLICKED(IDC_GRAVAR,&CTypedPtrListView::OnGravar)
  ON_BN_CLICKED(IDC_LIMPAR_CAMPOS,&CTypedPtrListView::OnLimparCampos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGradienteView diagnostics

#ifdef _DEBUG
void CGradienteView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CGradienteView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGradienteView message handlers

void CGradienteView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void CGradienteView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CGradienteView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CGradienteView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CGradienteView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CGradienteView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CGradienteView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[3] = {9,62,85};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(3,&Tabs[0]);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);

  GetDlgItem(IDC_EDIT2)->SetWindowText("0.00");

}

int CGradienteView::Consiste(tlstwstr& ListaCampos)
{
  int Erro(0);
  
  const CString& Titulo(GetDocument()->GetTitle());

  ArqGradiente RegTemp(ListaCampos); 

  int Tipo(Titulo.Find("GRADIENTES") != -1 ? 0 :
           Titulo.Find("EXPESSURAS") != -1 ? 1 :  
           Titulo.Find("EMPOLAMENTOS") != -1 ? 2 : 3);

  Erro = RegTemp.Consiste(Tipo);

  if(Erro) 
  {
    switch (Tipo)
    {
      case 0:  monolog.mensagem(24,-1,monolog.ErrosGradi[Erro-1]); break;
      case 1:  monolog.mensagem(24,-1,monolog.ErrosExpess[Erro-1]); break;
      case 2:  monolog.mensagem(24,-1,monolog.ErrosEmpola[Erro-1]); break;
  //    default: assert(0);
    }
  }
  else
  {
    if (Tipo == 0)      //---  Verifica se o gradiente está dentro sa secao tipo
    {
      SecTipo SecaoTipo;

      if(SecoesTipo.BuscaSecTipo(CEstaca(RegTemp.ListaCampos.begin()->c_str()),SecaoTipo))
      {
        ittlstwstr ItCampoAtual(RegTemp.ListaCampos.begin());

        ItCampoAtual++;
        ItCampoAtual++;

        double xGradiente(atof(ItCampoAtual->c_str()));

        if(xGradiente < SecaoTipo.GetHead().x || xGradiente > SecaoTipo.GetTail().x)
        {
          Erro = 5;
          monolog.mensagem(24,-1,monolog.ErrosGradi[Erro-1]);
        }
      }
      else 
      {
         Erro = 6;
         monolog.mensagem(24,-1,monolog.ErrosGradi[Erro-1]);
         Erro = 0;

      }
    }
  }

  return Erro;
}

void CGradienteView::Serialize(CArchive& ar) 
{
}

void CGradienteView::OnBnClickedGravar()
{
  CTypedPtrListView::OnBnClickedGravar();
}
