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
#include "TaludesPavimView.h"
#include "supercstring.h"
#include "carqtaludespavim.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define PRIMEIRO_CAMPO  2
#define ULTIMO_CAMPO 6

/////////////////////////////////////////////////////////////////////////////
// CTaludesPavimView

IMPLEMENT_DYNCREATE(CTaludesPavimView, CTypedPtrListView)

CTaludesPavimView::CTaludesPavimView() : CTypedPtrListView(CTaludesPavimView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO),EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT3),EdUltimoCampo(ListaCampos.end(),-1)
{
}

CTaludesPavimView::~CTaludesPavimView()
{
}

void CTaludesPavimView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,8);
}

BEGIN_MESSAGE_MAP(CTaludesPavimView, CFormView)
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
// CTaludesPavimView diagnostics

#ifdef _DEBUG
void CTaludesPavimView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CTaludesPavimView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTaludesPavimView message handlers

void CTaludesPavimView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void CTaludesPavimView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CTaludesPavimView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CTaludesPavimView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CTaludesPavimView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CTaludesPavimView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CTaludesPavimView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[6] = {10,59,79,126,173,221};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(6,Tabs);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CTaludesPavimView::Consiste(tlstwstr& LstCampos)
{
  ArqTaludesPavim RegTemp(LstCampos); 

  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosAlarg[Erro-1]);

  return Erro;
}

void CTaludesPavimView::Serialize(CArchive& ar) 
{
}

void CTaludesPavimView::OnDraw(CDC* pDC)
{
  CTypedPtrListView::OnDraw(pDC);
}
