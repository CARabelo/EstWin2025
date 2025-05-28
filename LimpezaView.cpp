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
#include "Limpezaview.h"
#include "supercstring.h"
#include "carqLimpeza.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern monologo monolog;

#define PRIMEIRO_CAMPO  3
#define ULTIMO_CAMPO  4

/////////////////////////////////////////////////////////////////////////////
// CLimpezaView

IMPLEMENT_DYNCREATE(CLimpezaView, CTypedPtrListView)

CLimpezaView::CLimpezaView() : CTypedPtrListView(CLimpezaView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO), EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT3),EdUltimoCampo(ListaCampos.end(),-1)
{
}

CLimpezaView::~CLimpezaView()
{
}

void CLimpezaView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,6);
}

BEGIN_MESSAGE_MAP(CLimpezaView, CFormView)
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
// CLimpezaView diagnostics

#ifdef _DEBUG
void CLimpezaView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CLimpezaView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLimpezaView message handlers

void CLimpezaView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void CLimpezaView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CLimpezaView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CLimpezaView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CLimpezaView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CLimpezaView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CLimpezaView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[4] = {8,59,78,127};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(4,&Tabs[0]);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CLimpezaView::Consiste(tlstwstr& ListaCampos)
{
  ArqLimpeza RegTemp(ListaCampos); 

  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosLimpeza[Erro-1]);

  return Erro;
}

void CLimpezaView::Serialize(CArchive& ar) 
{
}
