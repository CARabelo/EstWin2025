/ CDiaCriarProjeto.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "CDiaCriarProjeto.h"
#include "afxdialogex.h"
#include <direct.h>
#include "dlggetpath.h"
#include "Dialogo.h"

extern dialogo dialogar;

#define OK_ARQ 0

IMPLEMENT_DYNAMIC(CDiaCriarProjeto, CDialogEx)

CDiaCriarProjeto::CDiaCriarProjeto(CWnd* pParent,const CString& CStrDir,int pProjACAD) : CDialogEx(CDiaCriarProjeto::IDD, pParent),CStrNomeProjeto(_T("")),OK(false),
          CStrDiretorio(CStrDir),ProjAcad(pProjACAD)
{
}

CDiaCriarProjeto::~CDiaCriarProjeto()
{
}

void CDiaCriarProjeto::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDI_PROJETO, CStrNomeProjeto);
  DDV_MaxChars(pDX, CStrNomeProjeto, 50);
  DDX_Text(pDX, IDC_EDI_DIRETORIO, CStrDiretorio);
}

BEGIN_MESSAGE_MAP(CDiaCriarProjeto, CDialogEx)
  ON_BN_CLICKED(IDC_BUTDIRETORIO, &CDiaCriarProjeto::OnClickedButdiretorio)
  ON_BN_CLICKED(IDOK, &CDiaCriarProjeto::OnBnClickedOk)
END_MESSAGE_MAP()


void CDiaCriarProjeto::OnClickedButdiretorio()
{
  CFolderPickerDialog DlgPegaDir;

  DlgPegaDir.DoModal();

  SetDlgItemText(IDC_EDI_DIRETORIO,DlgPegaDir.GetFolderPath());
}

void CDiaCriarProjeto::OnBnClickedOk()
{
  if (UpdateData(true) == TRUE)
  {
    if (!CStrNomeProjeto.IsEmpty())
    {
      int Resp(GravaProjeto());

      if (Resp == OK_ARQ)
      {
        OnOK();
        OK = true;
      }
      else
      {
        std::string strMsg;

        switch (Resp)
        {
          case EEXIST: strMsg = "Já existe um projeto com este nome neste local "; break;
          default:
          {
            CString strMsg("Erro ao criar arquivos no diretório selecionado ");
            strMsg += " \r\nVocê tem permissão para usar arquivos neste local?  (Chame o programa como administrador)";
          }
        }

        AfxMessageBox(strMsg.c_str(), MB_ICONERROR);
      }
    }
    else
    {
      AfxMessageBox("O nome do projeto deve ser preenchido, o projeto não foi criado", MB_ICONERROR);
    }
  }

  CDialogEx::OnOK();
}

int CDiaCriarProjeto::GravaProjeto()
{
  CFile ArqCab;

  if(CStrDiretorio.GetAt(CStrDiretorio.GetLength()-1) != '\\') CStrDiretorio.Append("\\"); 

  std::string ArqPrj(CStrDiretorio + CStrNomeProjeto + "\\" + CStrNomeProjeto + ".prj");
  DWORD dwAttrib(GetFileAttributes(ArqPrj.c_str()));
  int ArqExiste(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));

  if (ArqExiste)
  {
    if (dialogar.DoModal(-1, "Este diretório já existe, tem certeza que é este o local onde deseja colocar os arquivos deste projeto?") != IDOK)
    {
      return EEXIST;
    }
  }
  if(!ArqExiste) mkdir(CStrDiretorio);   

  int Resp(-1);
  CString DirCompl(CString(CStrDiretorio + CStrNomeProjeto));

  _mkdir(DirCompl);  //--- Diretório da primeira rua, pode estar somente com os arquivos do drone e das cns 

  CString NomeCompl(CString(DirCompl + "\\" + CStrNomeProjeto + ".prj"));

  if(UpdateData(TRUE) == TRUE)
  {
    CFileException e;

    if (ArqCab.Open(NomeCompl,CFile::modeCreate |CFile::modeWrite,NULL,&e))
    {
      CArchive ar(&ArqCab,CArchive::store);

      ar << CStrNomeProjeto << CStrDiretorio;

      ar.Close();
      ArqCab.Close();
      std::string NomeArqAtoOri(".\\ArquivosBasicos\\ArquivosBasicos.ato"),
                  NomeArqAtoDest(DirCompl);
      NomeArqAtoDest += "\\" + CStrNomeProjeto + ".ato";
      CopyFile(NomeArqAtoOri.c_str(), NomeArqAtoDest.c_str(), false);

      //if (ProjAcad)
      if(0)
      {
        NomeArqAtoDest = DirCompl;
        NomeArqAtoDest += "\\" + CStrNomeProjeto + ".rav";
        CopyFile(NomeArqAtoOri.c_str(), NomeArqAtoDest.c_str(), false);
      }

    }
  }

  return OK_ARQ; 
}

