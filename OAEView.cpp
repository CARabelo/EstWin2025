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
#include "OAEView.h"
#include "supercstring.h"
#include "carqOAE.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define PRIMEIRO_CAMPO  4
#define ULTIMO_CAMPO 4

/////////////////////////////////////////////////////////////////////////////
// COAEView

IMPLEMENT_DYNCREATE(COAEView, CTypedPtrListView)

COAEView::COAEView() : CTypedPtrListView(COAEView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO), EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT3),EdUltimoCampo(ListaCampos.end(),-1)
{
  //{{AFX_DATA_INIT(COAEView)
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

COAEView::~COAEView()
{
}

void COAEView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,8);
}

BEGIN_MESSAGE_MAP(COAEView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
  ON_BN_CLICKED(IDC_BUTTON1, &COAEView::OnBnClickedGravar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COAEView diagnostics

#ifdef _DEBUG
void COAEView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void COAEView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// COAEView message handlers

void COAEView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void COAEView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void COAEView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void COAEView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void COAEView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void COAEView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void COAEView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();


  int Tabs[5] = {7,61,83,137,158};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(5,Tabs);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int COAEView::Consiste(tlstwstr& ListaCampos)
{
  ArqOAE RegTemp(ListaCampos); 

  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosOAE[Erro-1]);

  return Erro;
}

void COAEView::Serialize(CArchive& ar) 
{
}

void COAEView::OnBnClickedGravar()
{
  CTypedPtrListView::OnBnClickedGravar();
}
