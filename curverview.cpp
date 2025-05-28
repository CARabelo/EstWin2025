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
#include "CEditEx.h"
#include "CurVerView.h"
#include "carqcurverticais.h"
#include "monologo.h"
#include "math.h"
#include "Estaca.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;        //--- Dialogo para monologos.

#define PRIMEIRO_CAMPO  3
#define ULTIMO_CAMPO 6

/////////////////////////////////////////////////////////////////////////////
// CCurVerView

IMPLEMENT_DYNCREATE(CCurVerView, CTypedPtrListView)

CCurVerView::CCurVerView() : CTypedPtrListView(CCurVerView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO),EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT3),EdUltimoCampo(ListaCampos.end(),-1)
{
  //{{AFX_DATA_INIT(CCurVerView)
  m_Campo1 = _T("");
  m_Campo2 = _T("");
  m_Campo3 = _T("");
  m_Campo4 = _T("");
  m_Campo5 = _T("");
  //}}AFX_DATA_INIT
}

CCurVerView::~CCurVerView()
{
}

void CCurVerView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,1);
  //{{AFX_DATA_MAP(CCurVerView)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCurVerView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
  ON_BN_CLICKED(IDC_BUTTON1, &CCurVerView::OnBnClickedGravar)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurVerView diagnostics

#ifdef _DEBUG
void CCurVerView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CCurVerView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCurVerView message handlers

void CCurVerView::OnAdd()
{
  UpdateData(TRUE);
  CTypedPtrListView::OnAdd();
}

void CCurVerView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CCurVerView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CCurVerView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CCurVerView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CCurVerView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CCurVerView::OnInitialUpdate() 
{
  CMyStructList& mystructList = GetDocument()->m_mystructList;
  POSITION pos(mystructList.GetHeadPosition());
  while (pos != NULL)
  {
    CMyStruct* pMyStruct = mystructList.GetNext(pos);
    pMyStruct->Arredonda(3,3);
    pMyStruct->Arredonda(4,2);
    pMyStruct->Arredonda(5,2);
  }

  CTypedPtrListView::OnInitialUpdate();

  int Tabs[5] = {5,78,103,155,205};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(5,&Tabs[0]);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CCurVerView::Consiste(tlstwstr& ListaCampos)
{
  ArqCurVerticais RegTemp(ListaCampos); 

  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosCurVer[Erro-1]);

  return Erro;
}

void CCurVerView::OnBnClickedGravar()
{
  //--- não são mais feitas alterações nesta parefa

 /// if(ConsistePIVS()) CTypedPtrListView::OnBnClickedGravar();   
}

bool CCurVerView::ConsistePIVS()
{
  CMyStructList& mystructList(GetDocument()->m_mystructList);

  if(mystructList.GetCount() < 2) return true;

  POSITION PosAnt(mystructList.GetHeadPosition()),PosAtual(PosAnt);
  CMyStruct* pMyStructAnt(mystructList.GetAt(PosAnt));
  CEstaca EstAnterior(pMyStructAnt->Campos[0]);
  double Y1Atual,Y1Ant(atof(pMyStructAnt->Campos[1])),Y2Atual,Y2Ant(atof(pMyStructAnt->Campos[4]));

  mystructList.GetNext(PosAtual);

  while (PosAtual != NULL)
  {
    CMyStruct* pMyStructAtual(mystructList.GetNext(PosAtual));

    CEstaca EstacaAtual(pMyStructAtual->Campos[0]);
    Y1Atual = atof(pMyStructAtual->Campos[3]);
    Y2Atual = atof(pMyStructAtual->Campos[4]);

    if((EstAnterior + Y2Ant) > (EstacaAtual - Y1Atual)) 
    {
      CString Msg("Erro, PIVs transpassando, Estaca ");
      Msg.Append(EstAnterior);

      AfxMessageBox(Msg);

      return false;
    }

    Y2Ant = Y1Atual;
    EstAnterior = EstacaAtual;
  }

  return true;
}
