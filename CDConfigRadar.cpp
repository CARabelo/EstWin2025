// CDConfigRadar.cpp : implementation file
//

#include "stdafx.h"
#include "Est.h"
#include "CDConfigRadar.h"
#include "afxdialogex.h"

// CDConfigRadar dialog

IMPLEMENT_DYNAMIC(CDConfigRadar, CDialogEx)

CDConfigRadar::CDConfigRadar(CWnd* pParent,int* ppDados) : CDialogEx(CDConfigRadar::IDD, pParent),pDados(ppDados)
{
  m_bTodosPontos = *(pDados) != 0;
  m_bSoPontosInteresse = *(pDados+1) != 0;
  m_bRealcarPontosInteresse = *(pDados+2) != 0;
  Raio = *(pDados+3);
  m_butCor.SetColor(*((COLORREF*)pDados+13));
}

CDConfigRadar::~CDConfigRadar()
{
}

void CDConfigRadar::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Text(pDX, IDC_EDIRAIO, Raio);
  DDV_MinMaxInt(pDX, Raio, 5, 10000);
  DDX_Check(pDX,IDC_RADTODOSPONTOS,m_bTodosPontos);
  DDX_Check(pDX,IDC_RADINTERESSE,m_bSoPontosInteresse);
  DDX_Check(pDX,IDC_CHEREALCAR,m_bRealcarPontosInteresse);
  DDX_Control(pDX, IDC_MFCCOLORBUTTON, m_butCor);
}

BEGIN_MESSAGE_MAP(CDConfigRadar, CDialogEx)
  ON_BN_CLICKED(IDOK, &CDConfigRadar::OnBnClickedOk)
END_MESSAGE_MAP()

void CDConfigRadar::OnBnClickedOk()
{
  UpdateData(true);

  *pDados = m_bTodosPontos != 0;
  *(pDados+1) = m_bSoPontosInteresse != 0;
  *(pDados+2) = m_bRealcarPontosInteresse != 0;
  *(pDados+3) = (unsigned int) Raio;
  *(pDados+13) = m_butCor.GetColor();

  CDialogEx::OnOK();
}

