// AbrirMedicao.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AbrirMedicao.h"
#include "io.h"
#include "direct.h" 
#include <string>
using std::string;
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// AbrirMedicao dialog

AbrirMedicao::AbrirMedicao(CWnd* pParent /*=NULL*/)
: CDialog(AbrirMedicao::IDD, pParent)
{
  //{{AFX_DATA_INIT(AbrirMedicao)
  //}}AFX_DATA_INIT
}

void AbrirMedicao::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(AbrirMedicao)
  DDX_Control(pDX, IDC_LISTMEDICAO, m_listaMedicao);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(AbrirMedicao, CDialog)
  //{{AFX_MSG_MAP(AbrirMedicao)
  ON_BN_CLICKED(IDC_CHSOLEITURA, OnChsoleitura)
  ON_LBN_DBLCLK(IDC_LISTMEDICAO, OnDblclkListmedicao)
  ON_WM_CHAR()
  ON_WM_KEYUP()
  ON_WM_VKEYTOITEM()
  ON_WM_DRAWITEM()
  //}}AFX_MSG_MAP
  ON_UPDATE_COMMAND_UI(ID_TERRENO_GERAR_CN, &AbrirMedicao::OnUpdateTerrenoGerarCn)
  ON_BN_CLICKED(IDOK, &AbrirMedicao::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AbrirMedicao message handlers

void AbrirMedicao::OnChsoleitura() 
{

}

void AbrirMedicao::OnDblclkListmedicao() 
{
  OnOK();
}


void AbrirMedicao::OnOK() 
{
  CListBox * Lista = (CListBox *) GetDlgItem(IDC_LISTMEDICAO);
  int Atual(Lista->GetCurSel());

  if (Atual != LB_ERR)    //--- Se selecionou uma medição..
  {
    Lista->GetText(Atual,Selecionada);	
    CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
    MainFrame->m_sMedicao = Selecionada.Mid(8,3); 
    Selecionada = NomeProjComp + "med" + Selecionada.Mid(8,3);
    Selecionada.MakeUpper();
    MainFrame->InsLstProjetos(Selecionada);

    //--- guarda o número da medicao anterior.

    if (Atual)
    {
      Lista->GetText(--Atual,Selecionada);
      MainFrame->m_sMedicaoAnterior = Selecionada.Mid(8,3);
    }
    else MainFrame->m_sMedicaoAnterior.Empty();
  }

  CDialog::OnOK();
}

BOOL AbrirMedicao::OnInitDialog() 
{
  CDialog::OnInitDialog();

  struct _finddata_t Arquivo;
  long Handler;

  CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
  NomeProjComp = MainFrame->m_sNomeComp;

  if ((Handler = _findfirst((char *)LPCTSTR(NomeProjComp + "med???.cab"),&Arquivo)) != -1)
  {
    CString NomeMedicao = Arquivo.name;
    NomeMedicao = NomeMedicao.Mid(NomeMedicao.GetLength()-7,3);
    CTime Tempo(Arquivo.time_create);
    CString Data = Tempo.Format("           %d %B %Y, %A %Hh %Mm %Ss");

    ((CListBox *) GetDlgItem(IDC_LISTMEDICAO))->AddString("Medição "+ NomeMedicao + Data);    

    while(!_findnext(Handler,&Arquivo))
    {
      NomeMedicao = Arquivo.name;
      NomeMedicao = NomeMedicao.Mid(NomeMedicao.GetLength()-7,3);
      Tempo = Arquivo.time_create;
      Data = Tempo.Format("           %d %B %Y, %A %Hh %Mm %Ss");
      ((CListBox *) GetDlgItem(IDC_LISTMEDICAO))->AddString("Medição " +NomeMedicao + Data);    
    }
  }
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void AbrirMedicao::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  // TODO: Add your message handler code here and/or call default

  CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void AbrirMedicao::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  // TODO: Add your message handler code here and/or call default

  CDialog::OnKeyUp(nChar, nRepCnt, nFlags);
}


int AbrirMedicao::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
  // TODO: Add your message handler code here and/or call default

  return CDialog::OnVKeyToItem(nKey, pListBox, nIndex);
}

void AbrirMedicao::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
  // TODO: Add your message handler code here and/or call default

  CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void AbrirMedicao::OnUpdateTerrenoGerarCn(CCmdUI *pCmdUI)
{
  // TODO: Add your command update UI handler code here
}



void AbrirMedicao::OnBnClickedOk()
{
  // TODO: Adicione seu código de manipulador de notificações de controle aqui
  CDialog::OnOK();
}

