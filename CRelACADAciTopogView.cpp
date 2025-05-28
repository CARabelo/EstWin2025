#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "CEditEx.h"
#include "ImageListBox.h"
#include <map>
#include "CRelACADAciTopogView.h"
#include "supercstring.h"
#include <set>
#include "CArqAciTopograficos.h"
#include "monologo.h"
#include "cponto.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define ULTIMO_CAMPO 7
#define PRIMEIRO_CAMPO 1

/////////////////////////////////////////////////////////////////////////////
// CTresPontosView

IMPLEMENT_DYNCREATE(CRelACADAciTopogView, CTypedPtrListView)

CRelACADAciTopogView::CRelACADAciTopogView() : CTypedPtrListView(CRelACADAciTopogView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO),
                                       EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT3),EdUltimoCampo(ListaCampos.end(),-1),
                                       m_ForcarSelecionado(FALSE),nSel(-1),Forcar(false),Icone(0),Erro(0)
{
}

CRelACADAciTopogView::~CRelACADAciTopogView()
{
}

void CRelACADAciTopogView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX, 14);
  DDX_Control(pDX, IDC_EDIT3, EdPrimeiroCampo);
  DDX_Control(pDX, IDC_EDIT5, EdUltimoCampo);
  DDX_Control(pDX, IDC_LISICONRELACI, ListaIcones);
  DDX_Control(pDX, IDC_LISFORCARRELACI, ListCheckBox);
  DDX_Control(pDX, IDC_STABITICONERELACI, m_IconeSelecionado);
  DDX_Control(pDX, IDC_COMLAYACAD, ComboLayAutocad);
  DDX_Control(pDX, IDC_COMACIVUE, ComboAciTopogVUE);
  DDX_Control(pDX, IDC_LISLAYAUTOCAD, CListaLayerAutocad);
  DDX_Control(pDX, IDC_LISACITOPOGVUE, CListaAciTopogVUE);
  DDX_Check(pDX, IDC_CHEFORCARRELACI, m_ForcarSelecionado);
  DDX_Control(pDX, IDC_CHEFORCARRELACI, CheForcarAresta);
}

BEGIN_MESSAGE_MAP(CRelACADAciTopogView, CFormView)
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
  ON_LBN_SELCHANGE(IDC_LISFORCAR, &CRelACADAciTopogView::OnLbnSelchangeLisforcar)
  ON_LBN_SELCHANGE(IDC_LISICON, &CRelACADAciTopogView::OnSelchangeLisicon)
  ON_BN_CLICKED(IDC_BUTPADRAO, &CRelACADAciTopogView::OnClickedButpadrao)
  ON_CLBN_CHKCHANGE(IDC_LISFORCAR,&CRelACADAciTopogView::AlterouEstadoMarca)
  ON_BN_CLICKED(IDC_GRAVAR, &CRelACADAciTopogView::OnBnClickedGravar)
  ON_CBN_SELCHANGE(IDC_COMACIVUE, &CRelACADAciTopogView::OnCbnSelchangeComacivue)
  ON_LBN_SELCHANGE(IDC_LISLAYAUTOCAD, &CRelACADAciTopogView::OnLbnSelchangeLislayautocad)
  ON_LBN_SELCHANGE(IDC_LISACITOPOGVUE, &CRelACADAciTopogView::OnLbnSelchangeLisacitopogvue)
  ON_LBN_SELCHANGE(IDC_LISICONRELACI, &CRelACADAciTopogView::OnLbnSelchangeLisiconrelaci)
  ON_LBN_SELCHANGE(IDC_LISFORCARRELACI, &CRelACADAciTopogView::OnLbnSelchangeLisforcarrelaci)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRelACADAciTopogView diagnostics

#ifdef _DEBUG
void CRelACADAciTopogView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CRelACADAciTopogView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRelACADAciTopogView message handlers

void CRelACADAciTopogView::OnAdd()
{
  CString TextoChaveACAD, TextoChaveVUE;
  int PosLinha(-1);

  //--- Acha a posição da camada autocad no combobox

  int SelAcad(ComboLayAutocad.GetCurSel()), Pos(-1);

  if (SelAcad != LB_ERR)
  {
    ComboLayAutocad.GetLBText(SelAcad, TextoChaveACAD);
    Pos = CListaLayerAutocad.FindString(-1, TextoChaveACAD);

    if (Pos != LB_ERR)
    {
      monolog.mensagem(SEMMENSPADRAO, "Esta camada do Autocad já esta mapeada.");
      return;
    }

    //--- Adiciona-a no listbox na posição correta ( O listbox esta classificando, sort = true)

    CListaLayerAutocad.AddString(TextoChaveACAD);
    SetDlgItemText(IDC_EDIT, TextoChaveACAD.GetBuffer());

    //--- Pega a posição em que a linha foi adicionada, será a mesma nos outros objetos 

    PosLinha = CListaLayerAutocad.FindString(-1, TextoChaveACAD);

    //--- Pega o acidente topográfico do VUE que fou selecionado

    int SelVUE(ComboAciTopogVUE.GetCurSel());

    if (SelVUE != LB_ERR)
    {
      //--- Insere-o no list dos acidentes topográficos na posição correta (PosLinha)

      ComboAciTopogVUE.GetLBText(SelVUE, TextoChaveVUE);
      CListaAciTopogVUE.InsertString(PosLinha, TextoChaveVUE);
      SetDlgItemText(IDC_EDIT1, TextoChaveVUE.GetBuffer());
    }

    //--- Insere o checkbox, na posição correta

    Pos = ListCheckBox.InsertString(PosLinha, "");

    //--- Marca se o acidente topográfico gera arestas forçadas

    tlstlstwstr& AciTopogVUE(GetDocument()->LstRegAciTopog);

    bool Achou(false);
    std::string ChaveVUE(TextoChaveVUE.Left(TextoChaveVUE.Find(' ')));
    auto ItAciVUE(AciTopogVUE.begin());
    int PosicaoIcone(0);

    while (!Achou && ItAciVUE != AciTopogVUE.end())
    {
      if ((++(ItAciVUE->begin()))->compare(ChaveVUE) != 0) 
      {
        ItAciVUE++;
        PosicaoIcone++;
      }
      else Achou = true;
    }

    if (ItAciVUE != AciTopogVUE.end())
    {
      ListCheckBox.SetCheck(PosLinha, ItAciVUE->rbegin()->find("1") != std::string::npos);
    }

    //-- Coloca o ícone do acidente topográfico na posição correta

    int NumIcone(atoi(ItAciVUE->begin()->c_str()));

    ColocaIconePosicaoCcorreta(PosLinha,PosicaoIcone);

    //--- Os itens são salvos nos edit box

    SetDlgItemText(IDC_EDIT2, ItAciVUE->begin()->c_str());
		SetDlgItemText(IDC_EDIT5, ItAciVUE->rbegin()->c_str());

    //--- Desceleciona tudo

    CListaAciTopogVUE.SetCurSel(PosLinha);
    CListaLayerAutocad.SetCurSel(PosLinha);
    ListaIcones.SetCurSel(PosLinha);
    ListCheckBox.SetCurSel(PosLinha);
    ComboLayAutocad.SetCurSel(-1);
    ComboAciTopogVUE.SetCurSel(-1);
    m_IconeSelecionado.SetBitmap(nullptr);
  }
  PosArqRav = PosLinha;

  CTypedPtrListView::OnAdd();
}

void CRelACADAciTopogView::OnUpdate()
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

void CRelACADAciTopogView::OnRemove()
{
  nSel = CListaLayerAutocad.GetCurSel();

  if (nSel != LB_ERR)
  {
    CListaLayerAutocad.DeleteString(nSel);
    CListaAciTopogVUE.DeleteString(nSel);
    ListCheckBox.DeleteString(nSel);
    ListaIcones.DeleteString(nSel);

    ComboLayAutocad.SetCurSel(-1);
    ComboAciTopogVUE.SetCurSel(-1);

    m_IconeSelecionado.SetBitmap(nullptr);
  }

  CTypedPtrListView::OnRemove();
}

void CRelACADAciTopogView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();

  POSITION pos;
  CMyStruct* pMyStruct(FindMyStruct(nSel, pos));

  Icone = Forcar = 0;

  if(nSel != -1)
  {
    CBitmap BM;

    int Icone((int)ListaIcones.GetItemData(nSel));
    ListaIcones.GetImageFromList(Icone,&BM);
    m_IconeSelecionado.SetBitmap(BM);
    //ComboAciTopogVUE.SetCurSel(Icone);

    m_ForcarSelecionado = atoi((pMyStruct->lstRegAtual.rbegin())->c_str()) > 0;
    CheForcarAresta.SetCheck(m_ForcarSelecionado);

    m_ctlList.SetCurSel(nSel);
    CListaAciTopogVUE.SetCurSel(nSel);
    ListCheckBox.SetCurSel(nSel);
    CListaLayerAutocad.SetCurSel(nSel);
    ListaIcones.SetCurSel(nSel);

    //--- Posiciona os combobox nos respectivos itens

    CString Texto;

    CListaLayerAutocad.GetText(nSel, Texto);
    int Pos(ComboLayAutocad.FindString(-1, Texto.GetBuffer()));
  //  ComboLayAutocad.SetCurSel(Pos);

    UpdateData(true);
  }
}

void CRelACADAciTopogView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();
 
  int Tabs[3] = {2,32,77};
  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(3,&Tabs[0]);
  
  m_ImageList.Create(IDB_BITMAPACITOPOG,16,0,RGB(127,127,127));
  ListaIcones.SetImageList(&m_ImageList);
  
  CFont m_Font;

  m_Font.CreatePointFont(140, "Tahoma");
  m_ctlList.SetFont(&m_Font);

  ListCheckBox.SetFont(&m_Font);
  ListCheckBox.ResetContent();

  CListaLayerAutocad.SetFont(&m_Font);
  CListaLayerAutocad.ResetContent();

  CListaAciTopogVUE.SetFont(&m_Font);
  CListaAciTopogVUE.ResetContent();

  ListCheckBox.SetCheckStyle(BS_AUTOCHECKBOX);

  CMyStructList& mystructList(GetDocument()->m_mystructList);
  int C(0);
  POSITION pos(mystructList.GetHeadPosition());

  while (pos != NULL)
  {
    CMyStruct* pMyStruct(mystructList.GetNext(pos));

    auto It(pMyStruct->lstRegAtual.begin());

    CListaLayerAutocad.AddString((It++)->c_str());
    CListaAciTopogVUE.AddString((It++)->c_str()); 
    ListaIcones.AddItem(atoi(It->c_str()));
    ListCheckBox.AddString("");

    ListaIcones.SetItemData(ListaIcones.GetCount() - 1, atoi(It->c_str()));

    bool Check(false);

    if(pMyStruct->lstRegAtual.size() > 4)
    {
      Check = atoi(std::next(pMyStruct->lstRegAtual.begin(),4)->c_str()) > 0;
    }

    ListCheckBox.SetCheck(C++,Check);
  }

  ListCheckBox.SetCheckStyle(BS_AUTOCHECKBOX);

  //--- Combobox das camadas do autocad

  CFile ArqCamada;
  CFileException e;

  std::string NomeCompleto(GetDocument()->PegaNomeCompleto());
  std::string NomeCompArquivo(NomeCompleto.substr(0,NomeCompleto.find_last_of('\\')));

 // NomeCompArquivo = NomeCompArquivo.substr(0, NomeCompArquivo.find_last_of('\\'));

  size_t IniNome(NomeCompArquivo.find_last_of('\\')+1),
         TamStr(NomeCompArquivo.size());
  
  std::string NomeProj(NomeCompArquivo.substr(IniNome, TamStr-IniNome));

  NomeCompArquivo += "\\" + NomeProj + ".cam";

  if (ArqCamada.Open(NomeCompArquivo.c_str(), CFile::modeRead, &e) == 0)
  {
    monolog.mensagem(13, " O Arquivo de camadas do Autocad (.cam) não foi encontrado ou não pode ser aberto");

    return;
  }
  else
  {
    CArchive arCamadas(&ArqCamada, CArchive::load);
    size_t QuanReg(0);
    std::string Camada;

    for(arCamadas >> QuanReg ; QuanReg ; QuanReg--)
    {
       arCamadas >> Camada;
       ComboLayAutocad.AddString(Camada.c_str());
    }
  }

  /*
  if(1)for (int i = 0 ; i < 10 ; i++)
  {
    CString Texto;

    Texto.Format ("%i%i%i",i,i,i);
    ComboLayAutocad.InsertString(i,Texto.GetBuffer());
  }
  */

  //--- Le os acidentes topograficos do VU&E e os insere no listbox

  int ContaReg(0);
  auto& AciTopogVUE(GetDocument()->LstRegAciTopog);

  for (ittlstlstwstr it = AciTopogVUE.begin(); it != AciTopogVUE.end(); it++,ContaReg++)
  {
    std::string ChaveNomeCurto, Descricao, Forcar;
    int C(0);

    for (ittlstwstr itCampo = it->begin(); itCampo != it->end(); itCampo++, C++)
    {
      switch (C)
      {
        case 1:  
        {
          ChaveNomeCurto = *itCampo; 
          MapaPosicaoIcones.emplace(itCampo->substr(0,3), ContaReg);
        }
        break;
        case 2:
        {
          ChaveNomeCurto += "  ";
          ChaveNomeCurto += *itCampo;
        }
        break;
        case 3:  Descricao = *itCampo; break;
        case 4:  Forcar = *itCampo; break;
      }
    }

    CString Texto(ChaveNomeCurto.c_str());

    ComboAciTopogVUE.InsertString(ContaReg,Texto.GetBuffer());
  }

  //--- Posiciona no último.

  int Ultimo(m_ctlList.GetCount() - 1);

  m_ctlList.SetCurSel(Ultimo);
  ListaIcones.SetCurSel(Ultimo);
  ListCheckBox.SetCurSel(Ultimo);
  CListaAciTopogVUE.SetCurSel(Ultimo);
  CListaLayerAutocad.SetCurSel(Ultimo);

  m_IconeSelecionado.SetBitmap(nullptr);
}

int CRelACADAciTopogView::Consiste(tlstwstr& ListaCampos)
{
  //--- Nao há consistencias a fazer nos registros
  //--- A única consistencia a fazer é se a camada do autocad já foi inserida
  //--- e isso é feito no OnAdd

  return 0;
}

void CRelACADAciTopogView::OnLbnSelchangeLisforcar()
{
  int nSel(ListCheckBox.GetCurSel());
  
  ListaIcones.SetCurSel(nSel);
  m_ctlList.SetCurSel(nSel);
  ListCheckBox.SetCurSel(nSel);
}

void CRelACADAciTopogView::OnSelchangeLisicon()
{
 nSel = ListaIcones.GetCurSel();
 
 ListCheckBox.SetCurSel(nSel);
 m_ctlList.SetCurSel(nSel);
}

void CRelACADAciTopogView::OnClickedButpadrao()
{
 if(AfxMessageBox("Deseja realmente colocar no padrão todos os tipos (Forçar arestas ou não forçar) de acidentes topográficos?",
                     MB_YESNO|MB_ICONEXCLAMATION) != IDYES) return;

  bool Padrao[28] = {false,false,false,false,false,false,true,true,true,true,true,false,false,false,
                     false,false,true,false,true,true,false,true,false,false,false,true,false,false};

  for(int C = 0 ; C < 28 ; C++)
    ListCheckBox.SetCheck(C,Padrao[C]);

  UpdateData(false);
}

void CRelACADAciTopogView::AlterouEstadoMarca()
{
  int ItemSel(ListCheckBox.GetCurSel());

  if(ItemSel != -1)  ListCheckBox.SetCheck(ListCheckBox.GetCurSel(),!ListCheckBox.GetCheck(ListCheckBox.GetCurSel()));
}

void CRelACADAciTopogView::OnCbnSelchangeComacivue()
{
  int nSel(ComboAciTopogVUE.GetCurSel());
  CString ChaveStr;

  ComboAciTopogVUE.GetLBText(nSel, ChaveStr);
  //ChaveStr = ChaveStr.Left(ChaveStr.Find(' '));
  ChaveStr = ChaveStr.Left(3);

  tlstlstwstr& AciTopogVUE(GetDocument()->LstRegAciTopog);
  ittlstlstwstr ItReg(AciTopogVUE.begin());
  ittlstwstr ItCampo;
  bool Achou(false);

  while(!Achou && ItReg != AciTopogVUE.end())
  {
    ItCampo = ItReg->begin();
    ItCampo++;
    Achou = ItCampo->compare(0,3,ChaveStr) == 0;
    if(!Achou) ItReg++;
  }

  if (Achou)
  {
    ItCampo++;
    ItCampo++;

    EdPrimeiroCampo.SetWindowTextA(ItCampo->c_str());

    CBitmap BM;

    auto PosIcone(MapaPosicaoIcones.find(std::string(ChaveStr)));

    ListaIcones.GetImageFromList(PosIcone->second, &BM);
    m_IconeSelecionado.SetBitmap(BM);

    ItCampo++;

    CheForcarAresta.SetCheck(atoi((ItCampo)->c_str()) > 0);
  }
  UpdateData(true);
}

void CRelACADAciTopogView::OnLbnSelchangeLislayautocad()
{
  //--- Posiciona as listas no item atual

  nSel = CListaLayerAutocad.GetCurSel();
  CListaAciTopogVUE.SetCurSel(nSel);
  ListCheckBox.SetCurSel(nSel);
  m_ctlList.SetCurSel(nSel);
  ListaIcones.SetCurSel(nSel);

  CString Texto;

  //--- Posiciona os combobox nos respectivos itens

  CListaLayerAutocad.GetText(nSel, Texto);
  int Pos(ComboLayAutocad.FindString(-1,Texto.GetBuffer()));
  ComboLayAutocad.SetCurSel(Pos);
  
  CListaAciTopogVUE.GetText(nSel, Texto);
  //Pos = CListaAciTopogVUE.FindString(-1, Texto.GetBuffer());
  Pos = ComboAciTopogVUE.FindString(-1, Texto.GetBuffer());
  ComboAciTopogVUE.SetCurSel(Pos);
  ComboAciTopogVUE.GetLBText(Pos,Texto);

  if (nSel != -1)  
  { 
    int GGG = ListCheckBox.GetCheck(ListCheckBox.GetCurSel());

    ListCheckBox.SetCheck(ListCheckBox.GetCurSel(), GGG);
    CheForcarAresta.SetCheck(GGG);
  }
  
  OnSelChangeList();
}

void CRelACADAciTopogView::OnLbnSelchangeLisacitopogvue()
{
  //--- O layer do Autocad é mandatario
  //--- A relação Autocad VUE é 1:1
  //--- A relação VUE Autocad é 1:n

  nSel = CListaAciTopogVUE.GetCurSel();
  CListaLayerAutocad.SetCurSel(nSel);

  OnLbnSelchangeLislayautocad();   
}

void CRelACADAciTopogView::OnLbnSelchangeLisiconrelaci()
{
  nSel = ListaIcones.GetCurSel();

  CListaLayerAutocad.SetCurSel(nSel);
  OnLbnSelchangeLislayautocad();
}

void CRelACADAciTopogView::OnLbnSelchangeLisforcarrelaci()
{
  nSel = ListCheckBox.GetCurSel();

  if (nSel != -1)  ListCheckBox.SetCheck(ListCheckBox.GetCurSel(), !ListCheckBox.GetCheck(ListCheckBox.GetCurSel()));
  
  CListaLayerAutocad.SetCurSel(nSel);
  OnLbnSelchangeLislayautocad();
}

void CRelACADAciTopogView::ColocaIconePosicaoCcorreta(int PosicaoCorreta, int IconeInserir)
{
  int QtdIcones(ListaIcones.GetCount());

  if (PosicaoCorreta == QtdIcones)
  {
    ListaIcones.AddItem(IconeInserir);
    ListaIcones.SetItemData(PosicaoCorreta, IconeInserir);

    return;
  }

  std::list<int> LstIcones;

  for (int C = 0 ; C < ListaIcones.GetCount() + 1 ; C++)
  {
    if(C < PosicaoCorreta) LstIcones.push_back((int)ListaIcones.GetItemData(C));
      else if(C == PosicaoCorreta)  LstIcones.push_back(IconeInserir);
         else LstIcones.push_back((int)ListaIcones.GetItemData(C-1));
  }
  
  while (ListaIcones.GetCount() > 0) ListaIcones.DeleteString(0);

  std::list<int>::iterator it;

  for(std::list<int>::iterator it = LstIcones.begin() ; it != LstIcones.end() ; it++)
  {
    ListaIcones.AddItem(*it);
    ListaIcones.SetItemData(ListaIcones.GetCount()-1, *it);
  }
}