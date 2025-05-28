// CConfigProj.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include <winnetwk.h>
#include "CConfigProj.h"
#include <direct.h>
#include "dlggetpath.h"
#include <string>
#include "supercstring.h"

// CConfigProj dialog

IMPLEMENT_DYNAMIC(CConfigProj, CDialog) 
CConfigProj::CConfigProj(CWnd* pParent,const CString& pNomeProj,const CString& pDir,int pTipo,int pBalizaRua)
                                       : CDialog(CConfigProj::IDD, pParent),Tipo(pTipo),Ok(false),ProjetoExistente(pBalizaRua),
                                         ChaveGoogleEarth(_T(""))
{
  //{{AFX_DATA_INIT(confproj)
  NomeProj = pNomeProj;
  Diretorio = pDir;
  ValEstaca = 20.0;
  EquiCurHoriz = 10.0;
  EquiOAE = 5.0;
  EquiParabola = 10.0;
  EquiTangente = 20.0;
  ValEstaca = 20.0;
  EstacaIni = _T("0+0");
  EstacaFim = _T("9999+0");
  FormArqPontos = "n N E C O";
  //}}AFX_DATA_INIT

  ProjetoExistente = false;
}

CConfigProj::~CConfigProj()
{
}

void CConfigProj::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  //{{AFX_DATA_MAP(confproj)
  DDX_Text(pDX, IDC_NOMEPROJ, NomeProj);
  DDX_Text(pDX, IDC_DIR, Diretorio);
  DDX_Text(pDX, IDC_EQUIPAR, EquiParabola);
  DDV_MinMaxDouble(pDX, EquiParabola, 0.1, 10000.0);
  DDX_Text(pDX, IDC_EQUICURHOR, EquiCurHoriz);
  DDV_MinMaxDouble(pDX, EquiCurHoriz, 0.1, 10000.0);
  DDX_Text(pDX, IDC_EQUITAN, EquiTangente);
  DDV_MinMaxDouble(pDX, EquiTangente, 0.1, 10000.0);
  DDX_Text(pDX, IDC_EQUIOAE, EquiOAE);
  DDV_MinMaxDouble(pDX, EquiOAE, 0.1, 10000.0);
  DDX_Text(pDX, IDC_VALEST, ValEstaca);
  DDV_MinMaxDouble(pDX, ValEstaca, 0.1, 10000.0);
  DDX_Text(pDX, IDC_ESTINI, EstacaIni);
  DDX_Text(pDX, IDC_ESTFIM, EstacaFim);
  DDV_MaxChars(pDX, EstacaFim, 15);
  DDV_MaxChars(pDX, EstacaIni, 15);
  DDX_Text(pDX, IDC_FORMARQPONTOS, FormArqPontos);
  //}}AFX_DATA_MAP
  DDX_Text(pDX, IDC_EDICHAVEGOOGLEEARTH, ChaveGoogleEarth);
}

BEGIN_MESSAGE_MAP(CConfigProj, CDialog)
  ON_BN_CLICKED(IDOK, OnBnClickedOk)
  ON_BN_CLICKED(IDC_PORCDIRET, OnBnClickedPorcdiret)
END_MESSAGE_MAP()

// CConfigProj message handlers

void CConfigProj::OnBnClickedOk()
{
  Ok = false;

  if(UpdateData(true) == TRUE)
  {
    if(!NomeProj.IsEmpty())
    {
      EstacaIni.TrimRight();
      SuperCString Estaca(EstacaIni);

      if (EstacaIni.IsEmpty() || !Estaca.EEstaca())
      {
        AfxMessageBox("Estaca inicial Inválida.", MB_ICONERROR);
        return;
      }

      Estaca = EstacaFim.GetBuffer();

      if (EstacaFim.IsEmpty() || !Estaca.EEstaca())
      {
        AfxMessageBox("Estaca Final Inválida.", MB_ICONERROR);
        return;
      }

      if (ConsisteFormArqPontos() == false) return;

      if(GravaCab())
      {
        OnOK();
        Ok = true;
      }
      else
      {
        CString strMsg("Erro ao criar arquivos no diretório selecionado ");
        strMsg += " \r\nVocê tem permissão para usar arquivos nesta pasta?  (Chame o programa como administrador)";

        AfxMessageBox(strMsg.GetBuffer(),MB_ICONERROR);
      }
    }
    else
    {
      AfxMessageBox("O nome do projeto deve ser preenchido, o projeto não foi criado",MB_ICONERROR);
    }
  }
}

void CConfigProj::Dialoga(CString &Projeto,CString &Diretorio,int Projeto_Trecho)
{
  if(Tipo == CConfigProj::EXISTENTE) LeArquivo(Projeto,Diretorio);

  DoModal();
}

bool CConfigProj::LeArquivo(const CString& pNomeProj, const CString pDiretorio)
{
  CString NomeCompleto;
  CFile   arqcab;
  CFileException e;

  if (pNomeProj.GetLength() > 0)
  {
    Diretorio = pDiretorio.Left(pDiretorio.GetLength() - 1) + '\\' + pNomeProj;
    NomeCompleto = Diretorio + ".cab";
  }
  else NomeCompleto = NomeProj + ".cab";

  if (arqcab.Open(NomeCompleto,CFile::modeRead,&e))
  {
    CArchive ar(&arqcab,CArchive::load);

    ar >> Diretorio >> EquiParabola >> EquiCurHoriz >> EquiTangente >> EquiOAE >> ValEstaca 
	     >> EstacaIni >> EstacaFim >> FormArqPontos;

    LeChave(ar);

    ar.Close();
    arqcab.Close(); 

    ProjetoExistente = true;

    return true;
  }
  return false;
}

bool CConfigProj::GravaCab(const CString& NomeArquivo)
{
  if(!Diretorio.IsEmpty())
  {
    CFile ArqCab;

    if(Diretorio.GetAt(Diretorio.GetLength()-1) != '\\') Diretorio.Append("\\"); 

    if(!ProjetoExistente)
    {
    /*
      if (GetFileAttributes(Diretorio) == INVALID_FILE_ATTRIBUTES)
      {
        if(AfxMessageBox("Este diretório não existe, deseja criá-lo? (Um novo projeto será criado neste local.)") == IDOK) _mkdir(Diretorio);
        else return false;
      }
      */
    }
    else
    {
      if (GetFileAttributes(Diretorio) == INVALID_FILE_ATTRIBUTES) _mkdir(Diretorio);
      else
      {
        if (!ProjetoExistente && AfxMessageBox("Este diretório já existe, tem certeza que é este o local onde deseja colocar os arquivos deste projeto?") != IDOK) return false;
      }
    }

    CString NomeCompl(NomeArquivo.GetLength() == 0 ? CString(Diretorio + "\\" + NomeProj + ".cab"):NomeArquivo);

    if(UpdateData(TRUE) == TRUE)
    {
      CFileException e;

      if (ArqCab.Open(NomeCompl,CFile::modeCreate |CFile::modeWrite,NULL,&e))
      {
        CArchive ar(&ArqCab,CArchive::store);

        ar << Diretorio << EquiParabola << EquiCurHoriz << EquiTangente << EquiOAE << ValEstaca 
           << EstacaIni << EstacaFim << FormArqPontos;

        SalvaChave(ar);

        ar.Close();
        ArqCab.Close();

        return true;
      }
    }
  }
  else
  {
    AfxMessageBox("O nome do diretório deve ser preenchido, o projeto não foi criado.",MB_ICONERROR);
  }

  return false;
}

void CConfigProj::OnBnClickedPorcdiret()
{
  CFolderPickerDialog DlgPegaDir;

  DlgPegaDir.DoModal();

 // std::string Diret(DlgPegaDir.Get.GetPath().GetBuffer());
  //SetDlgItemText(IDC_DIR,DlgPegaDir.GetPath().GetBuffer());
}

bool CConfigProj::ConsisteFormArqPontos()
{
  std::string CaracValidos(" nNECO");

  FormArqPontos.TrimRight();
  
  if (FormArqPontos.GetLength() != 9)
  {
    AfxMessageBox(" O formato do arquivo de pontos não permitido, há caracteres faltando ou sobrando.");
    return false;
  }

  for (int C = 0; C < 9; C++)
  {
    if (CaracValidos.find(FormArqPontos[C]) == -1)
    {
      AfxMessageBox(" Há caracteres não permitidos no formato do arquivo de pontos");
      return false;
    }
  }

  for (int C = 1; C < 5; C++)
  {
    if (std::count(FormArqPontos.GetBuffer(),FormArqPontos.GetBuffer()+9, CaracValidos[C]) > 1)
    {
      AfxMessageBox(" Há caracteres duplicados formato do arquivo de pontos");
      return false;
    }
  }

  return true;
}

void CConfigProj::SalvaChave(CArchive& ar)
{
  char BXOR(char(0x01010101)),ByteAtual;

  ar << ChaveGoogleEarth.GetLength();

  for (int i = 0; i < ChaveGoogleEarth.GetLength(); i++)
  {
    ByteAtual = ChaveGoogleEarth[i] ^ BXOR;
    ar << ByteAtual;
  }
}

void CConfigProj::LeChave(CArchive& ar)
{
  char BXOR(char(0x01010101)), ByteAtual;
  int Tam(0);

  ChaveGoogleEarth = "";

  try
  {
    ar >> Tam;

    for (int i = 0; i < Tam; i++)
    {
      ar >> ByteAtual;

      ByteAtual = ByteAtual ^ BXOR;

      ChaveGoogleEarth += ByteAtual;
    }
  }
  catch (...)
  {
    AfxMessageBox("Houve erro ao ler achave do Google Earth.");
  }
}

BOOL CConfigProj::OnInitDialog()
{
  CDialog::OnInitDialog();

  GetDlgItem(IDC_NOMEPROJ)->SetFocus();

  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}
