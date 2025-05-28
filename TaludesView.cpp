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
#include "taludesview.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqtalAterro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define PRIMEIRO_CAMPO  3
#define ULTIMO_CAMPO    8

/////////////////////////////////////////////////////////////////////////////
// CTaludesView

IMPLEMENT_DYNCREATE(CTaludesView, CTypedPtrListView)

CTaludesView::CTaludesView() : CTypedPtrListView(CTaludesView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO), EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT3),EdUltimoCampo(ListaCampos.end(),-1)
{
}

CTaludesView::~CTaludesView()
{
}

void CTaludesView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,9);
  DDX_Control(pDX, IDC_EDIT3, EdPrimeiroCampo);
  DDX_Control(pDX, IDC_EDIT8, EdUltimoCampo);
}

BEGIN_MESSAGE_MAP(CTaludesView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
  ON_MESSAGE(VUE_USRMSG_ROLAR_REGISTRO, &CTypedPtrListView::OnKillFocusUltimoCampo)
  ON_BN_CLICKED(IDC_GRAVAR,&CTypedPtrListView::OnGravar)
  ON_BN_CLICKED(IDC_LIMPAR_CAMPOS,&CTypedPtrListView::OnLimparCampos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaludesView diagnostics

#ifdef _DEBUG
void CTaludesView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CTaludesView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTaludesView message handlers

void CTaludesView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void CTaludesView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CTaludesView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CTaludesView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CTaludesView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CTaludesView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CTaludesView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[10] = {8,65,86,108,147,186,225,264,303,340};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(10,Tabs);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CTaludesView::Consiste(tlstwstr& ListaCampos)
{
  ArqTalAterro RegTemp(ListaCampos); 

  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosAlarg[Erro-1]);  //--- Os erros são iguais aos dos alargamentos
///  else  ListaCampos = RegTemp.ListaCampos;   //--- Aguardar-> muita formatação no dialogo
 
  return Erro;
}

void CTaludesView::Serialize(CArchive& ar) 
{
}
