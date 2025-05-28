// CurVerView.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "supercstring.h"
#include "CSecoesAvulsasView.h"
#include "carqSecoesAvulsas.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;        //--- Dialogo para monologos.

/////////////////////////////////////////////////////////////////////////////
// CCurVerView

IMPLEMENT_DYNCREATE(CSecoesAvulsasView, CTypedPtrListView)

CSecoesAvulsasView::CSecoesAvulsasView() : CTypedPtrListView(CSecoesAvulsasView::IDD)
{
}

CSecoesAvulsasView::~CSecoesAvulsasView()
{
}

void CSecoesAvulsasView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,1);
}

BEGIN_MESSAGE_MAP(CSecoesAvulsasView, CFormView)
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
// CCurVerView diagnostics

#ifdef _DEBUG
void CSecoesAvulsasView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CSecoesAvulsasView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCurVerView message handlers

void CSecoesAvulsasView::OnAdd()
{
  UpdateData(TRUE);
  CTypedPtrListView::OnAdd();
}

void CSecoesAvulsasView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CSecoesAvulsasView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CSecoesAvulsasView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CSecoesAvulsasView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CSecoesAvulsasView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CSecoesAvulsasView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[11] = {0,58,84,142,200,256,311,370,420,470,528};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(11,&Tabs[0]);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CSecoesAvulsasView::Consiste(tlstwstr& ListaCampos)
{
  return 0;
}

void CSecoesAvulsasView::Serialize(CArchive& ar) 
{
}

