// CDPopUpPontos.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "meleditbox.h"
#include <string>
#include "CDPopUpPontos.h"
#include <list>
#include <algorithm>
#include ".\cdpopuppontos.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDPopUpPontos dialog

CDPopUpPontos::CDPopUpPontos(CWnd* pParent,COLORREF CorFundo):CDialog(CDPopUpPontos::IDD, pParent),JanPai(pParent),m_EDNumDesPontos(CorFundo),MostrandoProgr(false)
{
  //{{AFX_DATA_INIT(CDPopUpPontos)
  //}}AFX_DATA_INIT
}

void CDPopUpPontos::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CDPopUpPontos)
    DDX_Control(pDX, IDC_DESCRICAO, m_EDNumDesPontos);
    DDX_Control(pDX, IDC_PROGRESS2, ProgAuxPontos);
    //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CDPopUpPontos, CDialog)
  //{{AFX_MSG_MAP(CDPopUpPontos)
  //}}AFX_MSG_MAP
  //  ON_EN_SETFOCUS(IDC_DESCRICAO, OnEnSetfocusDescricao)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDPopUpPontos message handlers

BOOL CDPopUpPontos::Create(CWnd* pWnd)    //--- modeless.
{
  return CDialog::Create(IDD,pWnd);
}

void  CDPopUpPontos::MostraDescricao(int x,int y,std::string& Descricao,bool ComProgresso)
{
  static int TABSIZE(2);
  CEdit* pEdPopUp((CEdit*) GetDlgItem(IDC_DESCRICAO));
  CDC* hDC(pEdPopUp->GetDC());
  if(!hDC) return;
  CFont* FonteAnterior(hDC->SelectObject(pEdPopUp->GetFont()));

  //--- conta as linhas e separa a maior em extensão 

  register size_t i(0), iAnterior(0);
  int MaiorTamanho(0), ContaLinhas(0);
  CSize TamLinha(hDC->GetTextExtent('A'));
  pEdPopUp->SetTabStops(TamLinha.cx * TABSIZE);

  do
  {
    i = Descricao.find("\r\n",iAnterior);
    std::string StringAtual(Descricao.substr(iAnterior,i-iAnterior));
    
    //--- Teve que ser feito assim porque o GetTextMetrics não é exato

    //--- Separa o maior tamanho
    
    TamLinha.cx = hDC->GetTextExtent(StringAtual.c_str()).cx;
    TamLinha.cx += std::count(StringAtual.begin(), StringAtual.end(), '\t') * TABSIZE;

    if (TamLinha.cx > MaiorTamanho) MaiorTamanho = TamLinha.cx;

    iAnterior = i+2;
    ContaLinhas++;

  }while(i != -1);

  //--- mostra a descrição.
  TamLinha.cy = TamLinha.cy * ContaLinhas + 3;
  MaiorTamanho = (MaiorTamanho * 1.1) + 3;  

  int Dy(ComProgresso ? 15 :0);

  pEdPopUp->MoveWindow(0,0,MaiorTamanho,TamLinha.cy+Dy,true);

  //--- quando faço esse move window o dialogo fica cinza.....?????????
  //Progresso()->MoveWindow(5, TamLinha.cy,120, 10);

  CRect listRect;
  GetWindowRect(&listRect);
  ScreenToClient(&listRect);
 // listRect.top = listRect.bottom + 3;
//  listRect.bottom = listRect.top + 20;
 
  //--- Não funciona, VERIFICAR

 /* if (ComProgresso)
  {
    ProgAuxPontos.ShowWindow(SW_SHOW);
    MostrandoProgr = true;
  }
  else
  {
    ProgAuxPontos.ShowWindow(SW_HIDE);
    MostrandoProgr = false;
  }
  */
    
  pEdPopUp->SetWindowText(Descricao.c_str());

  MoveWindow(x, y, MaiorTamanho, TamLinha.cy + Dy, true);

  //ShowWindow(SW_RESTORE);
  //pEdPopUp->HideCaret();


  hDC->SelectObject(FonteAnterior);
  pEdPopUp->ReleaseDC(hDC);

  ShowWindow(SW_RESTORE);
  pEdPopUp->HideCaret();
}

//void CDPopUpPontos::OnEnSetfocusDescricao()
//{
//  if(JanPai) JanPai->SetFocus();
//}

BOOL CDPopUpPontos::PreCreateWindow(CREATESTRUCT& cs)
{
  cs.style |= CS_OWNDC;    //--- DC Proprietário

  return CDialog::PreCreateWindow(cs);
}

BOOL CDPopUpPontos::OnInitDialog()
{
  CDialog::OnInitDialog();
  
  ProgAuxPontos.SetRange(0,100);
  ProgAuxPontos.SetStep(1);
  ProgAuxPontos.SetPos(0);
 

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}
