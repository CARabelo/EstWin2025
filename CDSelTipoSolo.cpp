// CDSelTipoSolo.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include <list>
#include "CDSelTipoSolo.h"
#include "afxdialogex.h"

// CDSelTipoSolo dialog

IMPLEMENT_DYNAMIC(CDSelTipoSolo, CDialogEx)

CDSelTipoSolo::CDSelTipoSolo(CWnd* pParent,tylststring& pLstTiposSolos,std::string& pResposta) : CDialogEx(CDSelTipoSolo::IDD, pParent),ListaTiposSolos(pLstTiposSolos),
                             StrResposta(pResposta)            
{
}

CDSelTipoSolo::~CDSelTipoSolo()
{
}

void CDSelTipoSolo::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMTIPOSSOLOS, ComTipoSolo);

  CString CStrResposta;
  
  int nResposta(ComTipoSolo.GetCurSel());
  
  if(nResposta != -1) 
  {
    ComTipoSolo.GetLBText(nResposta,CStrResposta);
    StrResposta = CStrResposta.GetBuffer();
    if(StrResposta.find("Desconhecido") != -1) StrResposta = "Desconhecido";
  }
}

BEGIN_MESSAGE_MAP(CDSelTipoSolo, CDialogEx)
END_MESSAGE_MAP()

// CDSelTipoSolo message handlers

INT_PTR CDSelTipoSolo::DoModal()
{
  return CDialogEx::DoModal();
}

BOOL CDSelTipoSolo::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  for(ittylststring ItTipo = ListaTiposSolos.begin() ;  ItTipo != ListaTiposSolos.end() ; ++ItTipo )
  {
    ComTipoSolo.AddString(ItTipo->c_str());
  }

  ComTipoSolo.AddString("DE - Desconhecido");

  ComTipoSolo.SetCurSel(0);

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}
