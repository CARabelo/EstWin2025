// CDiaThread.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CDiaThread.h"
#include "afxdialogex.h"

// caixa de diálogo de CDiaThread

IMPLEMENT_DYNAMIC(CDiaThread, CDialogEx)

CDiaThread::CDiaThread(CWnd* pParent)	: CDialogEx(IDD_DIATHREAD, pParent), Mensagem(_T(""))
{
}

CDiaThread::~CDiaThread()
{
}

void CDiaThread::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROGRESSO, Progresso);
  DDX_Text(pDX, IDC_STATEXTO, Mensagem);
}

BEGIN_MESSAGE_MAP(CDiaThread, CDialogEx)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDiaThread

void CDiaThread::PostNcDestroy()
{
  CDialog::PostNcDestroy();
  delete this;

  CDialogEx::PostNcDestroy();
}

BOOL CDiaThread::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  Progresso.SetRange(0, 100);
  Progresso.SetStep(1);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}
