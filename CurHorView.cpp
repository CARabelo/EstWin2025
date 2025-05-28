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
#include "curhorview.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqcurhorizontais.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define PRIMEIRO_CAMPO  4
#define ULTIMO_CAMPO 10

/////////////////////////////////////////////////////////////////////////////
// CCurHorView

IMPLEMENT_DYNCREATE(CCurHorView, CTypedPtrListView)

CCurHorView::CCurHorView() : CTypedPtrListView(CCurHorView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO),EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT3),EdUltimoCampo(ListaCampos.end(),-1)
{
}

CCurHorView::~CCurHorView()
{
}

void CCurHorView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,3);
  DDX_Control(pDX, IDC_EDIT8, m_CEEdit8);
  DDX_Control(pDX, IDC_EDIT9, m_CEEdit9);
  DDX_Control(pDX, IDC_EDIT10, m_CEEdit10);
}

BEGIN_MESSAGE_MAP(CCurHorView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_WM_SHOWWINDOW()
  ON_WM_CLOSE()
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
  ON_EN_KILLFOCUS(IDC_EDIT5, OnEnKillfocusEdit5)
  ON_BN_CLICKED(IDC_GRAVAR,&CTypedPtrListView::OnGravar)
  ON_BN_CLICKED(IDC_LIMPAR_CAMPOS,&CTypedPtrListView::OnLimparCampos)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurHorView diagnostics

#ifdef _DEBUG
void CCurHorView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CCurHorView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCurHorView message handlers

void CCurHorView::OnAdd()
{
  if(!VerificaLC()) return;

  CTypedPtrListView::OnAdd();
}

void CCurHorView::OnUpdate()
{
  if(!VerificaLC()) return;

  CTypedPtrListView::OnUpdate();

  OnEnKillfocusEdit5();
}

void CCurHorView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CCurHorView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CCurHorView::OnSelChangeList()
{
  static int SelAtual(-1); 

  if(!VerificaLC(1)) 
  {
    m_ctlList.SetCurSel(SelAtual);
    AfxMessageBox("Curva Espiral com Lc muito pequeno, (< 5m)");

    return;
  }

  SelAtual = m_ctlList.GetCurSel();;

  CTypedPtrListView::OnSelChangeList();

  OnEnKillfocusEdit5();
}

void CCurHorView::OnInsertBefore()
{
  if(!VerificaLC()) return;
  CTypedPtrListView::OnInsertBefore();
}

void CCurHorView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[11] = {2,64,76,138,157,173,190,257,307,344,380};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(110,&Tabs[0]);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

void CCurHorView::Serialize(CArchive& ar) 
{
}

void CCurHorView::OnClose()
{
  // TODO: Add your message handler code here and/or call default

  CTypedPtrListView::OnClose();
}

void CCurHorView::OnEnKillfocusEdit5()
{
  UpdateData(true);

  if(m_Campo[6] == "C")
  {
    m_Campo[9] = m_Campo[10] = m_Campo[11] = "";

    UpdateData(false);

    m_CEEdit8.EnableWindow(false);
    m_CEEdit9.EnableWindow(false);
    m_CEEdit10.EnableWindow(false);
  }
  else
  {
    m_CEEdit8.EnableWindow(true);
    m_CEEdit9.EnableWindow(true);
    m_CEEdit10.EnableWindow(true);
  }
}

bool CCurHorView::VerificaLC(int Tipo)
{
  UpdateData(true);

  if(m_Campo[6] == "E")
  {
    if(atoi(m_Campo[9].GetBuffer(1)) <= 5.0)
    {
      if(Tipo == 0) AfxMessageBox("Curva Espiral com Lc muito pequeno, (< 5m)");
      return false;
    }
  }

  return true;
}

int CCurHorView::Consiste(tlstwstr& ListaCampos)
{
  ArqCurHorizontais RegTemp(ListaCampos); 

  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosCurHor[Erro-1]);

  return Erro;
}

