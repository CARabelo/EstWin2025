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
#include "COACView.h"
#include "supercstring.h"
#include "carqoac.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define PRIMEIRO_CAMPO 3
#define ULTIMO_CAMPO 3

/////////////////////////////////////////////////////////////////////////////
// COACView

IMPLEMENT_DYNCREATE(COACView, CTypedPtrListView)

COACView::COACView() : CTypedPtrListView(COACView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO), EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT2),EdUltimoCampo(ListaCampos.end(),-1)
{
}

COACView::~COACView()
{
}

void COACView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,8);
}

BEGIN_MESSAGE_MAP(COACView, CFormView)
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
// COACView diagnostics

#ifdef _DEBUG
void COACView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void COACView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COACView message handlers

void COACView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void COACView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void COACView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void COACView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void COACView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void COACView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void COACView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[3] = {13,67,89};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(4,Tabs);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int COACView::Consiste(tlstwstr& ListaCampos)
{
  ArqOAC RegTemp(ListaCampos); 

  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosOAC[Erro-1]);

  return Erro;
}

void COACView::Serialize(CArchive& ar) 
{
}

void COACView::OnBnClickedGravar()
{
  CTypedPtrListView::OnBnClickedGravar();
}
