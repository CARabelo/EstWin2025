// SecTriangulacao.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "sectriangulacao.h"
#include "supercstring.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern monologo monolog;

/////////////////////////////////////////////////////////////////////////////
// CTresPontosView

IMPLEMENT_DYNCREATE(CSecTRiangView, CTypedPtrListView)

CSecTRiangView::CSecTRiangView() : CTypedPtrListView(CSecTRiangView::IDD)
{
  //{{AFX_DATA_INIT(CSecTRiangView)
  m_Campo1 = _T("");
  m_Campo2 = _T("");
  m_Campo3 = _T("");
  m_Campo4 = _T("");
  m_Campo5 = _T("");
  m_Campo6 = _T("");
  m_Campo7 = _T("");
  m_Campo8 = _T("");
  m_Campo9 = _T("");
  m_Campo10= _T("");
  m_Campo11= _T("");
  //}}AFX_DATA_INIT
}

CSecTRiangView::~CSecTRiangView()
{
}

void CSecTRiangView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,13);
  //{{AFX_DATA_MAP(CSecTRiangView)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSecTRiangView, CFormView)
  //{{AFX_MSG_MAP(CSecTRiangView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSecTRiangView diagnostics

#ifdef _DEBUG
void CSecTRiangView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CSecTRiangView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSecTRiangView message handlers

void CSecTRiangView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void CSecTRiangView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CSecTRiangView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CSecTRiangView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CSecTRiangView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CSecTRiangView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CSecTRiangView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[11] = {50,65,100,138,170,206,240,275,310,342,375};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(11,&Tabs[0]);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CSecTRiangView::Consiste(tlstwstr& ListaCampos)
{
/*
  SuperCString CampoTemp;
  int Erro(0);

  if(!SuperCString(*Dados).EEstaca()) Erro = 1;
  else if(!SuperCString(*(Dados+1)).ENumero()) Erro = 2;

  for (int i=2 ; i < 12 && ! Erro; i++) if(! SuperCString(*(Dados+i)).ENumero()) Erro = i+1;

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosSecTri[Erro-1]);

  return Erro;
  */

  return 0;

}

void CSecTRiangView::Serialize(CArchive& ar) 
{
}
