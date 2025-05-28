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
#include "AlaSarPasview.h"
#include "supercstring.h"
#include "carqalargamentos.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define ULTIMO_CAMPO 8
#define PRIMEIRO_CAMPO 3

/////////////////////////////////////////////////////////////////////////////
// CTresPontosView

IMPLEMENT_DYNCREATE(CAlaSarPasView, CTypedPtrListView)

CAlaSarPasView::CAlaSarPasView() : CTypedPtrListView(CAlaSarPasView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO),
                                   EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT3),EdUltimoCampo(ListaCampos.end(),-1)
{
}

CAlaSarPasView::~CAlaSarPasView()
{
}

void CAlaSarPasView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,14);

  DDX_Control(pDX, IDC_EDIT3, EdPrimeiroCampo);
  DDX_Control(pDX, IDC_EDIT8, EdUltimoCampo);
}

BEGIN_MESSAGE_MAP(CAlaSarPasView, CFormView)
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
// CAlaSarPasView diagnostics

#ifdef _DEBUG
void CAlaSarPasView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CAlaSarPasView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CAlaSarPasView message handlers

void CAlaSarPasView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void CAlaSarPasView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CAlaSarPasView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CAlaSarPasView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CAlaSarPasView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CAlaSarPasView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CAlaSarPasView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();
 
  int Tabs[10] = {7,61,82,99,137,175,213,251,289,320};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(10,&Tabs[0]);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CAlaSarPasView::Consiste(tlstwstr& ListaCampos)
{
 ArqAlargamento RegTemp(ListaCampos);

  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosAlarg[Erro-1]);

  return Erro;
}
