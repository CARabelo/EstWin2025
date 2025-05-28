// CDiaRemoverRua.cpp : implementation file
//
#include "stdafx.h"
#include "Est.h"
#include "CDiaRemoverRua.h"
#include "afxdialogex.h"

IMPLEMENT_DYNAMIC(CDiaRemoverRua, CDialogEx)

CDiaRemoverRua::CDiaRemoverRua(CWnd* pParent,const tlstwstr& pListaRuas,std::string& ppRua) : CDialogEx(CDiaRemoverRua::IDD),ListaRuas(pListaRuas),pRua(ppRua)
{
}

void CDiaRemoverRua::OnBnClickedOk()
{
  UpdateData(true);

  int Sel(ComRuas.GetCurSel());
  CString RuaRemover;

  if (Sel != LB_ERR) 
  {
    GetDlgItemText(IDC_COMCOPIARARQUIVOS,RuaRemover);

    pRua = RuaRemover;
  }

  CDialogEx::OnOK();
}

void CDiaRemoverRua::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_COMCOPIARARQUIVOS, ComRuas);
}

BEGIN_MESSAGE_MAP(CDiaRemoverRua, CDialogEx)
  ON_BN_CLICKED(IDOK, &CDiaRemoverRua::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CDiaRemoverRua::OnInitDialog()
{
  CDialogEx::OnInitDialog();
  
  for(cittlstwstr cit=ListaRuas.begin() ; cit != ListaRuas.end() ; cit++ )
  {
    ComRuas.AddString(cit->c_str());
  }

  return TRUE;  // return TRUE unless you set the focus to a control
}
