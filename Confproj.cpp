// confproj.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"       // main symbols
#include <winnetwk.h>
#include "confproj.h"
#include <direct.h>
#include "dialogo.h"
#include "monologo.h"
#include <math.h>
#include "estaca.h"
#include "cponto.h"
#include <strstream>
#include "mat.h"
#include "dlggetpath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define OK   1

extern class Matem Mat;

/////////////////////////////////////////////////////////////////////////////
// confproj dialog

confproj::confproj(CWnd* pParent,const CString& NomeProj,const CString& Dir,int pTipo) : CDialog(confproj::IDD, pParent),AngRotacao(0),Tipo(pTipo)
{
  //{{AFX_DATA_INIT(confproj)
  m_azimute = 0.0;
  m_coore = 60000.0;
  m_coorn = 100000.0;
  m_empresa = _T("");
  m_equicurhor = 10.0;
  m_equioae = 5.0;
  m_equipar = 10.0;
  m_equitan = 20.0;
  m_linha1 = _T("");
  m_linha2 = _T("");
  m_linha3 = _T("");
  m_nomelogra = _T("");
  m_valest = 20.0;
  m_CSEstIni = _T("0+0");
  m_CSEstFim = _T("999+0");
  m_nomeproj = NomeProj;
  m_dir = Dir;
  //}}AFX_DATA_INIT
}

void confproj::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  m_coore = Mat.Arredonda(m_coore,4);
  m_coorn = Mat.Arredonda(m_coorn,4);
  m_azimute =  Mat.Arredonda(m_azimute,6);

  //{{AFX_DATA_MAP(confproj)
  DDX_Control(pDX, IDC_NOMEPROJ, m_cednomeProj);
  DDX_Control(pDX, IDC_DIR, m_cedDir);
  DDX_Text(pDX, IDC_AZIMUTE, m_azimute);
  DDV_MinMaxDouble(pDX, m_azimute, 0., 359.9999999);
  DDX_Text(pDX, IDC_COORE, m_coore);
  DDX_Text(pDX, IDC_COORN, m_coorn);
  DDX_Text(pDX, IDC_DIR, m_dir);
  DDV_MaxChars(pDX, m_dir, 255);
  DDX_Text(pDX, IDC_EMPRESA, m_empresa);
  DDV_MaxChars(pDX, m_empresa, 255);
  DDX_Text(pDX, IDC_EQUICURHOR, m_equicurhor);
  DDV_MinMaxDouble(pDX, m_equicurhor, 0.1, 100000.);
  DDX_Text(pDX, IDC_EQUIOAE, m_equioae);
  DDV_MinMaxDouble(pDX, m_equioae, 0.1, 10000.);
  DDX_Text(pDX, IDC_EQUIPAR, m_equipar);
  DDV_MinMaxDouble(pDX, m_equipar, 0.1, 10000.);
  DDX_Text(pDX, IDC_EQUITAN, m_equitan);
  DDV_MinMaxDouble(pDX, m_equitan, 0.1, 10000.);
  DDX_Text(pDX, IDC_LINHA1, m_linha1);
  DDV_MaxChars(pDX, m_linha1, 100);
  DDX_Text(pDX, IDC_LINHA2, m_linha2);
  DDV_MaxChars(pDX, m_linha2, 100);
  DDX_Text(pDX, IDC_LINHA3, m_linha3);
  DDV_MaxChars(pDX, m_linha3, 100);
  DDX_Text(pDX, IDC_NOMELOGRA, m_nomelogra);
  DDV_MaxChars(pDX, m_nomelogra, 255);
  DDX_Text(pDX, IDC_NOMEPROJ, m_nomeproj);
  DDV_MaxChars(pDX, m_nomeproj, 255);
  DDX_Text(pDX, IDC_VALEST, m_valest);
  DDV_MinMaxDouble(pDX, m_valest, 0.1, 10000.);
  DDX_Text(pDX, IDC_ESTINI, m_CSEstIni);
  DDV_MaxChars(pDX, m_CSEstIni, 20);
  DDX_Text(pDX, IDC_ESTFIM, m_CSEstFim);
  DDV_MaxChars(pDX, m_CSEstFim, 15);
  //}}AFX_DATA_MAP

  CEstaca EstacaTemp(m_CSEstFim);

  if(pDX->m_bSaveAndValidate && EstacaTemp.EstReal == INFINITO)  
  {
    AfxMessageBox("Estaca inválida");
    pDX->Fail();
  }
  else
  {
    EstacaTemp = m_CSEstIni;
    if(pDX->m_bSaveAndValidate && EstacaTemp.EstReal == INFINITO)  
    {
      AfxMessageBox("Estaca inválida");
      pDX->Fail();
    }
  }
  DDX_Control(pDX, IDC_PROCURAR, CBProcurar);
  DDX_Control(pDX, IDC_PORCDIRET, CButProcDiret);
}

BEGIN_MESSAGE_MAP(confproj, CDialog)
  //{{AFX_MSG_MAP(confproj)
  ON_BN_CLICKED(IDC_PROCURAR, OnProcurar)
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
  ON_BN_CLICKED(IDC_PORCDIRET, &confproj::OnBnClickedPorcdiret)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// confproj message handlers

extern class dialogo dialogar;
extern class monologo monolog;

int confproj::DoModal() 
{
  return CDialog::DoModal();
}

void confproj::dialoga(CString &Projeto,CString &Diretorio)
{
  LeArquivo(Projeto,Diretorio);

  DoModal();
}

int confproj::gravacab(const CString& NomeArquivo)
{
  CFile arqcab;
  _mkdir(m_dir);

  CString nomecompl = NomeArquivo.GetLength() == 0 ? CString(m_dir + "\\" + m_nomeproj + ".CAB"):NomeArquivo;

  if(NomeArquivo.GetLength() != 0 || UpdateData(TRUE) == TRUE)
  {
    if (arqcab.Open(nomecompl,CFile::modeCreate |CFile::modeWrite,NULL))
    {
      CArchive ar(&arqcab,CArchive::store);

      ar << AngRotacao <<  m_azimute << m_coore << m_coorn << m_CSEstIni <<	m_CSEstFim << m_dir << m_equicurhor << m_equipar 
        << m_equitan << m_equioae << m_linha1 << m_linha2 << m_linha3 << m_nomelogra <</* m_nomeproj <<*/ m_valest << m_empresa; 

      ar.Close();
      arqcab.Close();
      return TRUE;
    }
    return FALSE;
  }
  return FALSE;
}

bool confproj::LeArquivo(const CString& NomeProj,const CString Diretorio)
{
  CString nomecompl;
  CFile   arqcab;
  CFileException e;

  m_dir = Diretorio;

  if(NomeProj.GetLength() > 0) nomecompl = Diretorio + NomeProj + ".cab";
  else nomecompl = m_dir + '\\'+ m_nomeproj + ".cab";

  if (arqcab.Open(nomecompl,CFile::modeRead,&e))
  {
    CArchive ar(&arqcab,CArchive::load);

    ar >> AngRotacao >> m_azimute >> m_coore >> m_coorn >>	m_CSEstIni >>	m_CSEstFim >>	m_dir >> m_equicurhor >> m_equipar 
      >> m_equitan >> m_equioae >> m_linha1 >> m_linha2 >> m_linha3 >> m_nomelogra >> m_valest >> m_empresa;

   //  m_dir = Diretorio;

    ar.Close();
    arqcab.Close(); 

    m_coore = Mat.Arredonda(m_coore,4);
    m_coorn = Mat.Arredonda(m_coorn,4);
    m_azimute = Mat.Arredonda(m_azimute,6);

    return true;
  }
  return false;
}

void  confproj::GravaAngRotacao(const CString& NomeProj,double AngRot)
{
  LeArquivo(NomeProj);
  AngRotacao = AngRot;
  gravacab(NomeProj);

}

void confproj::GravaPIAZInicial(const CString& NomeProj, double AzInicial,double y, double x,double AngRot)
{
  //if(NomeProj.GetLength() > 0) m_nomeproj = NomeProj;

  LeArquivo(NomeProj);

  AngRotacao = AngRot;
  m_azimute = AzInicial;
  m_coore = x;
  m_coorn = y;

  gravacab(NomeProj);
}
void  confproj::PegaAngRotacao(const CString& NomeProj,double& AngRot)
{
  LeArquivo(NomeProj);

  AngRot = AngRotacao;
}

void confproj::PegaPIAZInicial(const CString& NomeProj, double& AzInicial,double& y, double& x,double& AngRot,CString& EstIni)
{
  // if(NomeProj.GetLength() > 0) m_nomeproj = NomeProj;

  LeArquivo(NomeProj);

  AngRot = AngRotacao;
  AzInicial =  m_azimute;
  x =  m_coore;
  y = m_coorn;
  EstIni = m_CSEstIni;
}

BOOL confproj::OnInitDialog() 
{
  CDialog::OnInitDialog();

  if(Tipo == EXISTENTE)
  {
    m_cedDir.EnableWindow(false);
    m_cednomeProj.EnableWindow(false);
    CButProcDiret.EnableWindow(false);
  }
  else 
  {
    CBProcurar.EnableWindow(false);
  }

  //---Verifica se o geometrico já foi calculado caso positivO inibe a edIção das coordenadas iniciais,
  //---Caso o usuário deseje muda-lás, só através do desenho geométrico

  CFileException e;
  CString nomecompl;

  nomecompl = m_dir + '\\' + m_nomeproj + ".chc";

  CFile ArqCHC;

  if(ArqCHC.Open(nomecompl,CFile::modeRead,&e))
  {
    GetDlgItem(IDC_COORE)->EnableWindow(false);
    GetDlgItem(IDC_COORN)->EnableWindow(false);
    GetDlgItem(IDC_AZIMUTE)->EnableWindow(false);
    CButProcDiret.EnableWindow(false);

    ArqCHC.Close();
  }

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void confproj::OnProcurar() 
{
  CFileDialog diaabrearq(TRUE,NULL,"*.cab",OFN_EXPLORER | OFN_FILEMUSTEXIST,NULL,NULL);
  diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Procurar projeto :";
  if (diaabrearq.DoModal() == OK)
  {
    CString nomearq = diaabrearq.GetFileTitle(),nomeproj = diaabrearq.GetPathName(),
      dir = nomeproj.Left(nomeproj.GetLength() - nomearq.GetLength() - 4);

    nomeproj = nomeproj.Left(nomeproj.GetLength() - 4); 
    SetDlgItemText(IDC_DIR,dir);
    SetDlgItemText(IDC_NOMEPROJ,nomearq);
  }
}

void confproj::OnKillfocusNomeproj() 
{
  CString CSNomeProj(' ',255);
  std::strstream stmm_dir;

  UpdateData(true);

  if(m_cednomeProj.GetLine(0,CSNomeProj.GetBuffer(255),254)) 
  {
    stmm_dir << "C:\\ESTRADAS\\" << CSNomeProj.GetBuffer(1) << "\\" << std::ends;
    m_dir = stmm_dir.str();

    UpdateData(false);
  }
}

void confproj::OnBnClickedOk()
{
  UpdateData(TRUE);
  gravacab();

  OnOK();
}

void confproj::OnBnClickedPorcdiret()
{
  CDlgGetPath DlgPegaDir;

  DlgPegaDir.DoModal();

  std::string Diret(DlgPegaDir.GetPath().GetBuffer());
  SetDlgItemText(IDC_DIR,DlgPegaDir.GetPath().GetBuffer());
 
}