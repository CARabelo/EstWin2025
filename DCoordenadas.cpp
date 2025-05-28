// DCoordenadas.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "CPonto.h"
#include "CMEdit.h"
#include "DCoordenadas.h"
#include "Mat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern Matem Mat;

/////////////////////////////////////////////////////////////////////////////
// CDCoordenadas dialog

CDCoordenadas::CDCoordenadas(CWnd* pParent,std::string pProjeto) : CDialog(CDCoordenadas::IDD, pParent), CEditCoordx(Bufferx), CEditCoordy(Buffery),Projeto(pProjeto)
, BooTrocarXY(FALSE)
{
  //{{AFX_DATA_INIT(CDCoordenadas)
  m_Nome = _T("");
  //}}AFX_DATA_INIT

}

void CDCoordenadas::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDCoordenadas)
  DDX_Text(pDX, IDC_EDINOME, m_Nome);
  DDV_MaxChars(pDX, m_Nome, 50);
  DDX_Control(pDX, IDC_CHECOORDENADAS, m_cheCoordenadas);
  DDX_Control(pDX, IDC_CHENOME, m_cheNome);
  DDX_Control(pDX, IDC_COORDN, CEditCoordx);
  DDX_Control(pDX, IDC_COORDE, CEditCoordy);
  DDX_Check(pDX, IDC_CHETROCARXY, BooTrocarXY);
  //}}AFX_DATA_MAP

}

BEGIN_MESSAGE_MAP(CDCoordenadas, CDialog)
  //{{AFX_MSG_MAP(CDCoordenadas)
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_CHECOORDENADAS, &CDCoordenadas::OnClickedChecoordenadas)
  ON_BN_CLICKED(IDC_CHENOME, &CDCoordenadas::OnClickedChenome)
  ON_WM_COPYDATA()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCoordenadas message handlers

void CDCoordenadas::OnOK() 
{
  Bufferx.clear();
  Buffery.clear();

  CEditCoordx.FormataPast();
  CEditCoordy.FormataPast();

  bool OK(false);
  int C(0);

  if (Bufferx.size() > 3 || Buffery.size() > 3)
  {
    OK = true;

    while (C++ < 2 && OK)
    {
      std::stringstream BufferDigitado(C == 1 ? Bufferx : Buffery);

      BufferDigitado << std::right << std::fixed << std::setfill(' ') << std::setprecision(2);

      std::string Numero;

      while (BufferDigitado.good() && OK)
      {
        BufferDigitado >> Numero;

        OK = Mat.ENumero(Numero);
      }
    }

    if (OK)
    {
      UpdateData(true);

      Buffer = Bufferx;
      Buffer += " ";
      Buffer += Buffery;

      CDialog::OnOK();

      return;
    }
  }
  else return;

  AfxMessageBox("Dados Inválidos, apenas números são aceitos nos campos de coordenadas");
}

void CDCoordenadas::OnClickedChecoordenadas()
{
  if(m_cheCoordenadas.GetCheck() == BST_CHECKED)
  {
    m_cheNome.SetCheck(BST_UNCHECKED);
     
    GetDlgItem(IDC_STANOMEPONTO)->EnableWindow(false);
    GetDlgItem(IDC_EDINOME)->EnableWindow(false);
    GetDlgItem(IDC_COORDN)->EnableWindow(true);
    GetDlgItem(IDC_COORDE)->EnableWindow(true);
    GetDlgItem(IDC_STACOORDN)->EnableWindow(true);
    GetDlgItem(IDC_STACOORDE)->EnableWindow(true);
    GetDlgItem(IDC_STACOORDPONTO)->EnableWindow(true);
  }
  else
  {
    m_cheNome.SetCheck(BST_CHECKED);

     GetDlgItem(IDC_STANOMEPONTO)->EnableWindow(true);
     GetDlgItem(IDC_EDINOME)->EnableWindow(true);
     GetDlgItem(IDC_COORDN)->EnableWindow(false);
     GetDlgItem(IDC_COORDE)->EnableWindow(false);
     GetDlgItem(IDC_STACOORDN)->EnableWindow(false);
     GetDlgItem(IDC_STACOORDE)->EnableWindow(false);
     GetDlgItem(IDC_STACOORDPONTO)->EnableWindow(false);
  }
}

void CDCoordenadas::OnClickedChenome()
{
  if(m_cheNome.GetCheck() == BST_CHECKED) m_cheCoordenadas.SetCheck(BST_UNCHECKED);
  OnClickedChecoordenadas();
}

BOOL CDCoordenadas::OnInitDialog()
{
  CDialog::OnInitDialog();

  char Buffer[64] = { 0 };
  CString Default;
  std::string NomeArquivo(Projeto + std::string(".ini"));

  Default.Format("%d,%d",1,0);
  ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosLocalizar"), Default, Buffer, 64, NomeArquivo.c_str());

  std::stringstream strstrBuffer(Buffer);
  char Virgula;
  int CheCoordenadas(0), TrocarXY(0);

  strstrBuffer >> CheCoordenadas >> Virgula >> TrocarXY;
    
  BooCoordenadas = CheCoordenadas;
  BooTrocarXY = TrocarXY;
  if(!BooCoordenadas)  m_cheNome.SetCheck(BST_CHECKED);
  else m_cheCoordenadas.SetCheck(BST_CHECKED);

  UpdateData(false);

  OnClickedChecoordenadas();

   bPorNome = !BooCoordenadas;

   CEditCoordx.Clear();
   CEditCoordy.Clear();

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

CDCoordenadas::~CDCoordenadas()
{
  CString CSDados;
  std::string NomeArquivo = Projeto + std::string(".ini");

  CSDados.Format("%d,%d",BooCoordenadas, BooTrocarXY);

::WritePrivateProfileString(_T("DadosDesenhos"), _T("DadosLocalizar"), CSDados, NomeArquivo.c_str());
}