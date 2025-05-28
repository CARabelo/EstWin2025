#include "stdafx.h"
#include "Est.h"
#include "CDiaInsereRua.h"
#include "afxdialogex.h"
#include "DlgGetPath.h"

IMPLEMENT_DYNAMIC(CDiaInsereRua, CDialogEx)

CDiaInsereRua::CDiaInsereRua(CWnd* pParent,const tlstwstr& pListaRuas,std::string& ppNovaRua,std::string& ppCopiar,std::string* pptCopiarOutroProjeto) : CDialogEx(CDiaInsereRua::IDD),ListaRuas(pListaRuas),pNovaRua(ppNovaRua),pCopiar(ppCopiar),ptCopiarOutroProjeto(pptCopiarOutroProjeto)
{
}

void CDiaInsereRua::OnBnClickedOk()
{
  UpdateData(true);

  pNovaRua = NomeRua.GetBuffer();

  int Sel(ComCopiarArquivos.GetCurSel());
  CString Copiar;

  if (Sel != LB_ERR) 
  {
    GetDlgItemText(IDC_COMCOPIARARQUIVOS,Copiar);

    pCopiar = Copiar;
  }

  CDialogEx::OnOK();
}

void CDiaInsereRua::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDINOMETRECHO, NomeRua);
  DDX_Control(pDX, IDC_COMCOPIARARQUIVOS, ComCopiarArquivos);
}

BEGIN_MESSAGE_MAP(CDiaInsereRua, CDialogEx)
  ON_BN_CLICKED(IDOK, &CDiaInsereRua::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTOUTROPROJETO, &CDiaInsereRua::OnBnClickedButoutroprojeto)
END_MESSAGE_MAP()

BOOL CDiaInsereRua::OnInitDialog()
{
  CDialogEx::OnInitDialog();
  
  for(cittlstwstr cit=ListaRuas.begin() ; cit != ListaRuas.end() ; cit++ )
  {
    ComCopiarArquivos.AddString(cit->c_str());
  }

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDiaInsereRua::OnBnClickedButoutroprojeto()
{
  CFolderPickerDialog DlgPegaDir;

  DlgPegaDir.DoModal();

  std::string Diret(DlgPegaDir.GetFolderPath().GetBuffer());

  *ptCopiarOutroProjeto = Diret;

  ComCopiarArquivos.AddString(Diret.c_str());
  ComCopiarArquivos.SetCurSel(0);

  UpdateData(true);
}
