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
#include "ImageListBox.h"
#include "EdiAciTopogView.h"
#include "supercstring.h"
#include <set>
#include "CArqAciTopograficos.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define ULTIMO_CAMPO 5
#define PRIMEIRO_CAMPO 1

/////////////////////////////////////////////////////////////////////////////
// CTresPontosView

IMPLEMENT_DYNCREATE(CEdiAciTopogView, CTypedPtrListView)

CEdiAciTopogView::CEdiAciTopogView() : CTypedPtrListView(CEdiAciTopogView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO),
                                       EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT1),EdUltimoCampo(ListaCampos.end(),-1),
                                       m_ForcarSelecionado(FALSE),nSel(-1),Forcar(false),Icone(0)
{
}

CEdiAciTopogView::~CEdiAciTopogView()
{
}

void CEdiAciTopogView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,14);
  DDX_Control(pDX, IDC_EDIT1, EdPrimeiroCampo);
  DDX_Control(pDX, IDC_EDIT5, EdUltimoCampo);
  DDX_Control(pDX, IDC_LISICON, ListaIcones);
  DDX_Control(pDX, IDC_LISFORCAR, ListCheckBox);
  DDX_Control(pDX, IDC_STABITICONE, m_IconeSelecionado);
  DDX_Check(pDX, IDC_CHEFORCAR, m_ForcarSelecionado);
  DDX_Control(pDX, IDC_LISESPACO, ListaEspacos);
}

BEGIN_MESSAGE_MAP(CEdiAciTopogView, CFormView)
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
  ON_BN_CLICKED(IDC_GRAVAR, &CEdiAciTopogView::OnGravar)
 // ON_BN_CLICKED(IDC_GRAVAR, &CTypedPtrListView::OnGravar)
  ON_BN_CLICKED(IDC_LIMPAR_CAMPOS,&CTypedPtrListView::OnLimparCampos)
  ON_LBN_SELCHANGE(IDC_LISFORCAR, &CEdiAciTopogView::OnLbnSelchangeLisforcar)
  ON_LBN_SELCHANGE(IDC_LISICON, &CEdiAciTopogView::OnSelchangeLisicon)
  ON_LBN_SELCHANGE(IDC_LISESPACO, &CEdiAciTopogView::OnSelchangeLisespaco)
  ON_BN_CLICKED(IDC_BUTPADRAO, &CEdiAciTopogView::OnClickedButpadrao)
  ON_CLBN_CHKCHANGE(IDC_LISFORCAR,&CEdiAciTopogView::AlterouEstadoMarca)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEdiAciTopogView diagnostics

#ifdef _DEBUG
void CEdiAciTopogView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CEdiAciTopogView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CEdiAciTopogView message handlers

void CEdiAciTopogView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void CEdiAciTopogView::OnGravar()
{
  int count(0);
  CMyStructList& mystructList(GetDocument()->m_mystructList);
  POSITION pos(mystructList.GetHeadPosition());

  for (auto Pos = mystructList.GetHeadPosition(); Pos; mystructList.GetNext(Pos))
  {
    auto ListaCampos =  mystructList.GetAt(Pos);

    std::stringstream strNum;

    strNum << count++;

    *ListaCampos->lstRegAtual.begin() = strNum.str().c_str();
  }

  CTypedPtrListView::OnGravar();
}


void CEdiAciTopogView::OnUpdate()
{
  UpdateData(true);

  POSITION pos;
  CMyStruct* pMyStruct(FindMyStruct(nSel, pos));
  *pMyStruct->lstRegAtual.rbegin() = m_ForcarSelecionado ? "1" : "0";
  m_Campo[4] =  m_ForcarSelecionado ? "1" : "0";

  UpdateData(false);

  ListCheckBox.SetCheck(nSel,m_ForcarSelecionado);

  CTypedPtrListView::OnUpdate();
}

void CEdiAciTopogView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CEdiAciTopogView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CEdiAciTopogView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();

  POSITION pos;
  CMyStruct* pMyStruct(FindMyStruct(nSel, pos));

  Icone = Forcar = 0;

  if(nSel != -1)
  {
    CBitmap BM;

    ListaIcones.GetImageFromList(nSel,&BM);
    m_IconeSelecionado.SetBitmap(BM);
    m_ForcarSelecionado = atoi((pMyStruct->lstRegAtual.rbegin())->c_str()) > 0;

    UpdateData(false);

    ListaIcones.SetCurSel(nSel);
    ListaEspacos.SetCurSel(nSel);
    ListCheckBox.SetCurSel(nSel);
  }
}

void CEdiAciTopogView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CEdiAciTopogView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();
 
  int Tabs[3] = {2,32,77};
  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(3,&Tabs[0]);
  
  m_ImageList.Create(IDB_BITMAPACITOPOG,16,0,RGB(127,127,127));

  ListaIcones.SetImageList(&m_ImageList);
  
  //--- Coloca os ícones no list box

  for (int i = 0; i < 38 ; i++)
  {
    ListaIcones.AddItem(i);
  }

  //--- List dos checkbox

  m_font.CreatePointFont(140,"Arial"); 
  ListCheckBox.SetFont(&m_font);
  ListCheckBox.ResetContent();

  m_font2.CreatePointFont(147, "Arial");
  ListaEspacos.SetFont(&m_font2);
  ListaEspacos.ResetContent();

  ListCheckBox.SetCheckStyle(BS_AUTOCHECKBOX);

  CMyStructList& mystructList(GetDocument()->m_mystructList);
  int C(0);
  POSITION pos(mystructList.GetHeadPosition());

  while (pos != NULL)
  {
    CMyStruct* pMyStruct(mystructList.GetNext(pos));

    ListCheckBox.AddString("");
    bool Check(false);

    if(pMyStruct->lstRegAtual.size() > 4)
    {
      Check = atoi(std::next(pMyStruct->lstRegAtual.begin(),4)->c_str()) > 0;
    }

    ListCheckBox.SetCheck(C++,Check);

    ListaEspacos.AddString("");
  }
  
  //--- Posiciona no último.

  int Ultimo(m_ctlList.GetCount()-1);

  m_ctlList.SetCurSel(Ultimo);
  ListaIcones.SetCurSel(Ultimo);
  ListaEspacos.SetCurSel(Ultimo);
  ListCheckBox.SetCurSel(Ultimo);
}

int CEdiAciTopogView::Consiste(tlstwstr& ListaCampos)
{
 ArqAciTopograficos RegTemp(ListaCampos);

  int Erro(RegTemp.Consiste());

  //if(Erro) monolog.mensagem(24,-1,monolog.ErrosAciTopog[Erro-1]);

  return Erro;
}

void CEdiAciTopogView::OnLbnSelchangeLisforcar()
{
  int ItemSelecionado(ListCheckBox.GetCurSel());
  
  ListaIcones.SetCurSel(ItemSelecionado);
  m_ctlList.SetCurSel(ItemSelecionado);
  ListaEspacos.SetCurSel(ItemSelecionado);
}

void CEdiAciTopogView::OnSelchangeLisespaco()
{
  int ItemSelecionado(ListaEspacos.GetCurSel());
  
  ListaIcones.SetCurSel(ItemSelecionado);
  ListCheckBox.SetCurSel(ItemSelecionado);
  m_ctlList.SetCurSel(ItemSelecionado);
}

void CEdiAciTopogView::OnSelchangeLisicon()
{
 int ItemSelecionado(ListaIcones.GetCurSel());
  
 ListaEspacos.SetCurSel(ItemSelecionado);
 ListCheckBox.SetCurSel(ItemSelecionado);
 m_ctlList.SetCurSel(ItemSelecionado);
}

void CEdiAciTopogView::OnClickedButpadrao()
{
 if(AfxMessageBox("Deseja realmente colocar no padrão todos os tipos (Forçar arestas ou não forçar) de acidentes topográficos?",
                     MB_YESNO|MB_ICONEXCLAMATION) != IDYES) return;

  bool Padrao[28] = {false,false,false,false,false,false,true,true,true,true,true,false,false,false,
                     false,false,true,false,true,true,false,true,false,false,false,true,false,false};

  for(int C = 0 ; C < 28 ; C++)
    ListCheckBox.SetCheck(C,Padrao[C]);

  UpdateData(false);
}

void CEdiAciTopogView::AlterouEstadoMarca()
{
  int ItemSel(ListCheckBox.GetCurSel());

  if(ItemSel != -1)  ListCheckBox.SetCheck(ListCheckBox.GetCurSel(),!ListCheckBox.GetCheck(ListCheckBox.GetCurSel()));

}
