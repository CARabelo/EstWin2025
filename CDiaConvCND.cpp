// CDiaConvCND.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "CPonto.h"
#include "CPontoAnte.h"
#include <list>
#include "CDiaConvCND.h"
#include "afxdialogex.h"

// caixa de diálogo de CDiaConvCND

IMPLEMENT_DYNAMIC(CDiaConvCND, CDialogEx)

CDiaConvCND::CDiaConvCND(CWnd* pParent)	: CDialogEx(IDD_DIALOG7, pParent)
{
}

CDiaConvCND::~CDiaConvCND()
{
}

void CDiaConvCND::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDiaConvCND, CDialogEx)
	ON_BN_CLICKED(IDC_BUTCONVERTER, &CDiaConvCND::OnBnClickedButconverter)
END_MESSAGE_MAP()

int CDiaConvCND::LeArqCN_Ante(const std::string& NomeProj, tyLstLstPontos& LCN)
{
  CFile Arq;
  CFileException e;

  CString NomeCNDrone(NomeProj.c_str() + CString(".cnd"));

  if (Arq.Open(NomeCNDrone, CFile::modeRead, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      return false;
    }
  }

  CArchive CArcCurvasNivel(&Arq, CArchive::load);
  size_t QtdCurvasNivel(0), QtdPontosCNAtual(0);

  if (e.m_cause != CFileException::fileNotFound)
  {
    double EquidisCN, AngRotacaoBordos;

    CArcCurvasNivel >> AngRotacaoBordos >> EquidisCN;

    for (CArcCurvasNivel >> QtdCurvasNivel; QtdCurvasNivel; QtdCurvasNivel--)
    {
      PontoAnte PAtual;
      size_t QtdCurvasNivel(0);

      tyLstPontos CNAtual;

      for (CArcCurvasNivel >> QtdPontosCNAtual; QtdPontosCNAtual; QtdPontosCNAtual--)
      {
        PAtual << CArcCurvasNivel;

        CNAtual.emplace_back(Ponto(PAtual.x,PAtual.y, PAtual.z));
      }

      LCN.emplace_back(CNAtual);
    }
  }

  return true;
}

int CDiaConvCND::GravaArqCN_Atual(const std::string& NomeProj, tyLstLstPontos& LCN)
{
  CFile Arq;
  CFileException e;

  CString NomeCNDrone(NomeProj.c_str() + CString("Atual.cnd"));

  if (Arq.Open(NomeCNDrone, CFile::modeCreate | CFile::modeReadWrite, &e) == 0)
  {
    AfxMessageBox("Erro");
    return false;
  }

  CArchive CArcCurvasNivel(&Arq, CArchive::store);
  size_t QtdCurvasNivel(0), QtdPontosCNAtual(0);

  if (e.m_cause != CFileException::fileNotFound)
  {
    double EquidisCN, AngRotacaoBordos;

    CArcCurvasNivel << (double) 0.0 << (double) 1.0 << LCN.size();

    for (ittyLstLstPontos itCNAtual = LCN.begin(); itCNAtual != LCN.end(); itCNAtual++)
    {
      CArcCurvasNivel << itCNAtual->size();

      for (ittyLstPontos itPAtual = itCNAtual->begin(); itPAtual != itCNAtual->end(); itPAtual++)
      {
         Ponto PAtual(itPAtual->x, itPAtual->y, itPAtual->z);

        PAtual >> CArcCurvasNivel;
      }
    }
  }

  return true;
}

// manipuladores de mensagens de CDiaConvCND

void CDiaConvCND::OnBnClickedButconverter()
{
  tyLstLstPontos LCN;
  std::string NomeProj("C:\\Users\\carlo\\Documents\\Visual Studio 2019\\EstWin\\Projetos\\CampoGolf\\CampoGolf_2");

  LeArqCN_Ante(NomeProj,LCN);
  GravaArqCN_Atual(NomeProj,LCN);
}


