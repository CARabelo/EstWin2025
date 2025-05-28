// NivelView.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "estdoc.h"
#include "typlstvw.h"
#include "NivelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNivelView

IMPLEMENT_DYNCREATE(CNivelView, CTypedPtrListView)

CNivelView::CNivelView() : CTypedPtrListView(CNivelView::IDD)
{
  //{{AFX_DATA_INIT(CNivelView)
  m_Campo[1] = _T("");
  m_Campo[2] = _T("");
  m_Campo[3] = _T("");
  m_Campo[4] = _T("");
  m_Campo[5] = _T("");
  m_Campo[6] = _T("");
  m_Campo[7] = _T("");
  m_Campo[8] = _T("");
  m_Campo[9] = _T("");
  m_Campo[10]= _T("");
  m_Campo[11]= _T("");
  //}}AFX_DATA_INIT
}

CNivelView::~CNivelView()
{
}

void CNivelView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,1);
  //{{AFX_DATA_MAP(CNivelView)
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNivelView, CFormView)
  //{{AFX_MSG_MAP(CNivelView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_WM_VKEYTOITEM()
  //}}AFX_MSG_MAP
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNivelView diagnostics

#ifdef _DEBUG
void CNivelView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CNivelView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CNivelView message handlers

void CNivelView::OnAdd()
{
  UpdateData(TRUE);
  CTypedPtrListView::OnAdd();
}

void CNivelView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CNivelView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CNivelView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CNivelView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CNivelView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CNivelView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[4] = {80,113,156,207};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(4,&Tabs[0]);

  //--- Posiciona no último.

  //	m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

void CNivelView::Serialize(CArchive& ar) 
{
  if (ar.IsStoring())
  {	// storing code
  }
  else
  {	// loading code
  }
}

int CNivelView::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
  // TODO: Add your message handler code here and/or call default

  return CFormView::OnVKeyToItem(nKey, pListBox, nIndex);
}

