// CriarMedicao.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "CriarMedicao.h"
#include "io.h"
#include <string>
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
// CCriarMedicao dialog

CCriarMedicao::CCriarMedicao(CWnd* pParent /*=NULL*/)
: CDialog(CCriarMedicao::IDD, pParent)
{
  //{{AFX_DATA_INIT(CCriarMedicao)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

void CCriarMedicao::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CCriarMedicao)
  // NOTE: the ClassWizard will add DDX and DDV calls here
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CCriarMedicao, CDialog)
  //{{AFX_MSG_MAP(CCriarMedicao)
  ON_LBN_DBLCLK(IDC_LISTMEDICAO, OnDblclkListmedicao)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCriarMedicao message handlers

BOOL CCriarMedicao::OnInitDialog() 
{
  CDialog::OnInitDialog();

  struct _finddata_t Arquivo;
  char Temp[255];
  CString NomeMedicao;

  for (int i = 1 ; i != 1000 ; i++)
  {
    _itoa(i,Temp,10);

    NomeMedicao = "Medição ";
    for (size_t z = strlen(Temp) ; z < 3 ; z++) NomeMedicao += "0";
    NomeMedicao += Temp;

    ((CListBox *) GetDlgItem(IDC_LISTMEDICAO))->AddString(NomeMedicao);    
  }

  intptr_t Handler;

  CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
  CString NomeProjComp = MainFrame->m_sNomeComp;

  if ((Handler = _findfirst((char *)LPCTSTR(NomeProjComp + "med???.cab"),&Arquivo)) != -1)
  {
    CString NomeMedicao = Arquivo.name;
    NomeMedicao = NomeMedicao.Mid(NomeMedicao.GetLength()-7,3);

    ((CListBox *) GetDlgItem(IDC_LISTMEDICAO))->DeleteString(((CListBox *) GetDlgItem(IDC_LISTMEDICAO))->FindString(-1,"Medição " + NomeMedicao));    

    while(!_findnext(Handler,&Arquivo))
    {
      NomeMedicao = Arquivo.name;
      NomeMedicao = NomeMedicao.Mid(NomeMedicao.GetLength()-7,3);
      ((CListBox *) GetDlgItem(IDC_LISTMEDICAO))->DeleteString(((CListBox *) GetDlgItem(IDC_LISTMEDICAO))->FindString(-1,"Medição " + NomeMedicao));    
    }
  }
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CCriarMedicao::OnDblclkListmedicao() 
{
  OnOK();	
}

void CCriarMedicao::OnOK() 
{
  CListBox *Lista = (CListBox *) GetDlgItem(IDC_LISTMEDICAO);
  int Atual = Lista->GetCurSel();
  if (Atual != LB_ERR)
  {
    CString NomeMedicao;
    Lista->GetText(Atual,NomeMedicao);	
    CMainFrame *MainFrame = (CMainFrame*)AfxGetMainWnd();
    MainFrame->m_sMedicao = NomeMedicao.Mid(8,3); 
    NomeMedicao = MainFrame->m_sNomeComp + "med" + NomeMedicao.Mid(8,3) + ".cab";
    CopyFile(MainFrame->m_sNomeComp + ".cab",NomeMedicao,FALSE);
  }
  CDialog::OnOK();
}
