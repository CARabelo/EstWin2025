// CurHorView.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "ImageListBox.h"
#include "IconComboBox.h"
#include "CEditEx.h"
#include "CTiposSolosView.h"
#include "supercstring.h"
#include <set>
#include "CTipoSolos.h"
#include "carqtipossolos.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define PRIMEIRO_CAMPO  1
#define ULTIMO_CAMPO 3

/////////////////////////////////////////////////////////////////////////////
// CTiposSolosView

IMPLEMENT_DYNCREATE(CTiposSolosView, CTypedPtrListView)

CTiposSolosView::CTiposSolosView() : CTypedPtrListView(CTiposSolosView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO),EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT1),EdUltimoCampo(ListaCampos.end(),-1)
{
}

CTiposSolosView::~CTiposSolosView()
{
}

void CTiposSolosView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,8);
  DDX_Control(pDX, IDC_COMTIPOSLINHA, m_ComboImageTracos);
  DDX_Control(pDX, IDC_LISLINHAS, m_ListImageTracos);
  DDX_Control(pDX, IDC_LISCORES, m_ListCoresTracos);
  DDX_Control(pDX, IDC_MFCCORTRACO, m_ComboCorTraco);
}

BEGIN_MESSAGE_MAP(CTiposSolosView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_LBN_SELCHANGE(IDC_LISLINHAS, OnSelChangeListLinhas)
  ON_LBN_SELCHANGE(IDC_LISCORES, OnSelChangeListCores)
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
  ON_BN_CLICKED(IDC_GRAVAR,&CTypedPtrListView::OnGravar)
  ON_BN_CLICKED(IDC_LIMPAR_CAMPOS,&CTypedPtrListView::OnLimparCampos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiposSolosView diagnostics

#ifdef _DEBUG
void CTiposSolosView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CTiposSolosView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTiposSolosView message handlers

void CTiposSolosView::OnAdd()
{
  PegaTracoCor(INSERCAO);
  CTypedPtrListView::OnAdd();
}

void CTiposSolosView::OnUpdate()
{
  CString TipoSoloAnterior(ListaCampos.begin()->c_str());

  UpdateData(true);

  if(ListaCampos.begin()->compare(m_Campo[0]) != 0)
  {
   if(AfxMessageBox("Atenção: Você está alterando o tipo do solo, se o tipo de solo anterior foi usado em algum perfil ele não será "
                    "mais encontrado e passará a ser tratado por \"desconhecido\", tem certeza que deseja fazer esta alteração?",
                     MB_YESNO|MB_ICONEXCLAMATION) != IDYES) return;
  }

  PegaTracoCor(EDICAO);
  CTypedPtrListView::OnUpdate();

  int ProxCurSel(m_ListImageTracos.GetCurSel()+1);

  m_ListImageTracos.SetCurSel(ProxCurSel);
  m_ListCoresTracos.SetCurSel(ProxCurSel);
}

void CTiposSolosView::OnRemove()
{
  PegaTracoCor(DELECAO);
  CTypedPtrListView::OnRemove();
}

void CTiposSolosView::OnRemoveAll()
{
  PegaTracoCor(DELECAO);
  CTypedPtrListView::OnRemoveAll();
}

void CTiposSolosView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();

  int nSel;
  POSITION pos;
 
  CMyStruct* pMyStruct(FindMyStruct(nSel, pos));

  CorTraco = Traco = 0;

  if(nSel != -1)
  {
    if(pMyStruct->lstRegAtual.size() == 4)
    {
      rittlstwstr rIt(pMyStruct->lstRegAtual.rbegin());
      CorTraco = atol(rIt->c_str());
      rIt++;
      Traco = atoi(rIt->c_str());
      m_ComboImageTracos.SelectIcon(Traco);
    }

    m_ComboCorTraco.SetColor(CorTraco);
    m_ComboImageTracos.SetCurSel(Traco);

    m_ListImageTracos.SetCurSel(nSel);
    m_ListCoresTracos.SetCurSel(nSel);
  }
}

void CTiposSolosView::OnInsertBefore()
{
  PegaTracoCor(INSERIRANTES);
  CTypedPtrListView::OnInsertBefore();
}

void CTiposSolosView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[2] = {8,92};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(2,Tabs);

  //--- Cria a lista de traços. A do combo tem que ser separada da do list box por causa das alterações  e remoções
  
  ImageList.Create(IDB_BITTRACOS, 90, 0, RGB(255,255,255));
  ImageListLista.Create(IDB_BITMAPTRACO, 90, 0, RGB(255,255,255));

  m_ComboImageTracos.SetImageList(&ImageList);
  m_ListImageTracos.SetImageList(&ImageListLista);

  //--- Coloca os traços possíveis no combo box

  COMBOBOXEXITEM ItemCB = {CBEIF_IMAGE| CBEIF_SELECTEDIMAGE,0,0,0,0,0,0,0,0};

  for (int i = 0; i < 6; i++)
  {
    ItemCB.iImage = ItemCB.iSelectedImage = i;
    m_ComboImageTracos.InsertItem(&ItemCB);
  }

  //--- Cria a lista de cores. Esta lista comtém apenas um item que será usado para colocar a cor no listbox de cores
  //--- Esta lista é dinâmica. Ela crescendo a medida que o usuário vai selecionado cores diferentes
 
  ImageListCor.Create(IDB_BITMAPCOR,25,5, RGB(0,0,0));
  ImageListCor.SetBkColor(RGB(0,0,0));

  m_ListCoresTracos.SetImageList(&ImageListCor);

  BitmapCor.LoadBitmap(IDB_BITMAPCOR);     //--- Bitmap Temporário para criar a cor selecionada pelo usuario

  CMyStructList& mystructList(GetDocument()->m_mystructList);
  POSITION pos = mystructList.GetHeadPosition();
  CString str;

  CDC dc;
  dc.CreateCompatibleDC(NULL);

  //--- Inclui os traços e as cores para cada registro existente

 CImageList* pImageListCombo(m_ComboImageTracos.GetImageList());

  while (pos != NULL)
  {
    CMyStruct* pMyStruct(mystructList.GetNext(pos));

    //--- Pega o traço e a cor na lista de campos -------------------------------------------------------------

    int Traco(0),C(0);
    unsigned long Cor(0);

    for(ittlstwstr It = pMyStruct->lstRegAtual.begin() ; It != pMyStruct->lstRegAtual.end() && C < 4; It++,C++)
    {
      if(C == 2) Traco = atoi(It->c_str());
      if(C == 3) Cor = atol(It->c_str());
    }

    //---------------------------------------------------------------------------------------------------------

    HICON hIcon = pImageListCombo->ExtractIcon(5-Traco);

    ImageListLista.Add(hIcon);
    m_ListImageTracos.AddItem(ImageListLista.GetImageCount()-1); 
    
  //  m_ListImageTracos.AddItem(5-atoi(pMyStruct->Campos[2]));

    HGDIOBJ hOld(dc.SelectObject(BitmapCor));
    dc.FillSolidRect(0, 0,37,10,Cor);
    dc.SelectObject(hOld);

    ImageListCor.Add(&BitmapCor,RGB(0,0,0));
    m_ListCoresTracos.AddItem(ImageListCor.GetImageCount()-1);
 }

 //--- Posiciona no último.

 m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CTiposSolosView::Consiste(tlstwstr& ListaCampos)
{
  CTipoSolo RegTemp(ListaCampos); 
  int Erro(RegTemp.Consiste());

  if(Erro) monolog.mensagem(24,-1,monolog.ErrosTiposSolos[Erro-1]);

  return Erro;
}

void CTiposSolosView::Serialize(CArchive& ar) 
{
}

void CTiposSolosView::PegaTracoCor(int Acao)
{
  unsigned int TracoAtual(m_ComboImageTracos.GetCurSel());

  //--- Muda o traço

  if(TracoAtual != -1) 
  {  
    CImageList* pImageListCombo(m_ComboImageTracos.GetImageList()),
              * pImageListList(m_ListImageTracos.GetImageList());
    HICON hIcon(pImageListCombo->ExtractIcon(5-TracoAtual));

    if(Acao == DELECAO)
    {
      int SelAtual(m_ListImageTracos.GetCurSel());

      if(SelAtual != -1)
      {
         m_ListImageTracos.GetImageList()->Remove(SelAtual);
      }
    }
    else
    if(Acao == INSERCAO)
    {
      pImageListList->Add(hIcon);
      m_ListImageTracos.AddItem(pImageListList->GetImageCount()-1); 
    }
    else
    {   
      pImageListList->Replace(m_ListImageTracos.GetCurSel()+1,hIcon);
    }
  }

  m_ListImageTracos.RedrawWindow();

  //--- Muda a cor

  if(Acao == DELECAO)
  {
    int SelAtual(m_ListCoresTracos.GetCurSel());

    if(SelAtual != -1)
    {
       m_ListCoresTracos.GetImageList()->Remove(SelAtual);
    }
  }
  else
  {
    CDC dc;
    dc.CreateCompatibleDC(NULL);

    COLORREF CorAtual(m_ComboCorTraco.GetColor());

    HGDIOBJ hOld(dc.SelectObject(BitmapCor));
    dc.FillSolidRect(0, 0,37,10,CorAtual);
    dc.SelectObject(hOld);

    ImageListCor.Add(&BitmapCor,RGB(0,0,0));            //--- Adiciona a cor na lista de cores (podem se repetir)

    if(Acao == INSERCAO) m_ListCoresTracos.AddItem(ImageListCor.GetImageCount() - 1);
    else
    {
      CImageList* pImageList(m_ListCoresTracos.GetImageList());
      pImageList->Replace(m_ListCoresTracos.GetCurSel()+1,&BitmapCor,RGB(0,0,0));
    }
 
    //-- Salva os valores da cor e do traço atual

    CString CSTraco,CSCorTraco;

    CSTraco.Format("%i",TracoAtual);
    CSCorTraco.Format("%ul",m_ComboCorTraco.GetColor());
  
    SetDlgItemText(IDC_EDIT2,CSTraco.GetBuffer());
    SetDlgItemText(IDC_EDIT3,CSCorTraco.GetBuffer());
  }

  m_ListCoresTracos.RedrawWindow();
}

void CTiposSolosView::OnSelChangeListLinhas()
{
  int ItemSelecionado(m_ListImageTracos.GetCurSel());
  
  m_ListCoresTracos.SetCurSel(ItemSelecionado);
  m_ctlList.SetCurSel(ItemSelecionado);

  OnSelChangeList();
}

void CTiposSolosView::OnSelChangeListCores()
{
  int ItemSelecionado(m_ListCoresTracos.GetCurSel());
  
  m_ListImageTracos.SetCurSel(ItemSelecionado);
  m_ctlList.SetCurSel(ItemSelecionado);

  OnSelChangeList();
}
