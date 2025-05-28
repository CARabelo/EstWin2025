// DCalcGeometrico.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "math.h"
#include "estaca.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include <string>
#include "CPonto.h"
#include "Mat.h"
#include "dcalcgeometrico.h"
#include "afxtempl.h"
#include "resource.h"
#include "math.h"
#include "monologo.h"
#include "carqcurhorizontais.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "CArqSecTipo.h"
#include "csectipo.h"
#include <set>
#include <map>
#include "ccurhorizontal.h"
#include "ccurhorizontais.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "cestacasacalcular.h"
#include "dcalcgeoseccdireto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDCalcGeometrico dialog

CDCalcGeometrico::CDCalcGeometrico(CWnd* pParent,CString& pProjeto,CProjeto* CProj) : CDialog(CDCalcGeometrico::IDD, pParent),Projeto(pProjeto),pDialogo(this),CalcDireto(false),CPProjeto(CProj)
{
  //{{AFX_DATA_INIT(CDCalcGeometrico)
  m_Descricao = _T("");
  m_douErroAjuste = 1.0;
  //}}AFX_DATA_INIT
}

void CDCalcGeometrico::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDCalcGeometrico)
  DDX_Control(pDX, IDC_BUTCANCELAR, m_buCancelar);
  DDX_Control(pDX, IDOK, m_buOK);
  DDX_Control(pDX, IDC_INICIAR, m_buIniciar);
  DDX_Control(pDX, IDC_PROGRESS1, m_Progresso);
  DDX_Text(pDX, IDC_STDESCRICAO, m_Descricao);
  DDX_Text(pDX, IDC_EDI_ERROMAXIMO, m_douErroAjuste);
  DDV_MinMaxDouble(pDX,m_douErroAjuste, 0.005, 1000.0);
  //}}AFX_DATA_MAP
  DDX_Control(pDX, IDC_EDI_ERROMAXIMO, m_EdiErroMaximo);
  DDX_Control(pDX, IDC_CHE_AJUSTARCURVAS, m_cheAjustarCurvas);
}

BEGIN_MESSAGE_MAP(CDCalcGeometrico, CDialog)
  //{{AFX_MSG_MAP(CDCalcGeometrico)
  ON_BN_CLICKED(IDC_INICIAR, OnIniciar)
  ON_BN_CLICKED(IDC_BUTCANCELAR, OnButcancelar)
  //}}AFX_MSG_MAP
  //  ON_WM_CREATE()
  ON_BN_CLICKED(IDC_CHE_AJUSTARCURVAS, OnBnClickedCheAjustarcurvas)
  ON_EN_KILLFOCUS(IDC_EDI_ERROMAXIMO, OnEnKillfocusEdiErromaximo)
  ON_EN_CHANGE(IDC_EDI_ERROMAXIMO, OnEnChangeEdiErromaximo)
  //ON_WM_CHAR()
  //ON_EN_UPDATE(IDC_EDI_ERROMAXIMO, OnEnUpdateEdiErromaximo)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDCalcGeometrico message handlers

void CDCalcGeometrico::OnOK() 
{
  CDialog::OnOK();
}

BOOL CDCalcGeometrico::Create(CWnd* pWnd,CDialog* ppDialogo)     //--- para não modal.
{
  pDialogo = ppDialogo;
  return CDialog::Create(IDD,pWnd);
}

void CDCalcGeometrico::OnIniciar() 
{
  CProgressCtrl* LimpezaoDireto = NULL;
  CString* pDescricao = NULL;

  if(!UpdateData(true)) 
  {
    m_EdiErroMaximo.SetFocus();
    return;
  }

  if(pDialogo == this)
  {
    m_buIniciar.EnableWindow(false); 
    m_buCancelar.EnableWindow(false); 
  }
  else
  {
    //---A barra de progresso e a descrição são do dialogo direto

    pDescricao=  &((CDCalcGeoSeccDireto*) pDialogo)->m_CSExecutando2;
    LimpezaoDireto= &((CDCalcGeoSeccDireto*) pDialogo)->m_Progresso; 
  }

  //--- Executa o Cálculo geometrico 

  CSecTipo SecoesTipo(Projeto);
  int Erro(SecoesTipo.VerfErro());

  if(Erro == 0)
  {
    CCurHorizontais CurvasHorizontais(Projeto,SecoesTipo);	
    Erro = CurvasHorizontais.VerfErro();

    if(Erro == 0)
    {
      double ErroAjuste(CalcDireto ? 0.10 :  m_cheAjustarCurvas.GetCheck() == BST_CHECKED ? m_douErroAjuste : CEstaca::ERRO_MINIMO);

      //--- Não precisa mais calcular o geometrico porque ele é calculado on line no desenho geometrico. Porisso o ultimo parametro = true -> Só seccionar
      //--- Revogado: ta recalculando o geometrico no desenho geométrico, o usuário não precisa sair do desenho para recalcular o geométrico

      Erro = CurvasHorizontais.CalcGeometrico(pDialogo == this ? m_Descricao : *pDescricao,pDialogo == this ?  &m_Progresso : LimpezaoDireto,pDialogo,ErroAjuste,CPProjeto,true);
    }
    else Erro = CCurHorizontais::ERRO_LER_ARQUIVO;
  
    if (Erro != CCurHorizontais::CALCULO_OK)
    {
      std::string strTexto("Erro genérico");

      switch (Erro)
      {
        case CCurHorizontais::ERRO_CRIAR_ARQUIVO_GEO: strTexto = "Erro ao criar Arquivo .geo."; break;
        case CCurHorizontais::ERRO_CRIAR_ARQUIVO_CHC: strTexto = "Erro ao criar Arquivo .chc."; break;
        case CCurHorizontais::ERRO_CALCULO_CURVA: strTexto = "Erro no cálculo das curvas."; break;
        case CCurHorizontais::ERRO_NAO_HA_CH: strTexto = "Não há curvas suficientes para executar o cálculo. "; break;
        case CCurHorizontais::ERRO_NAO_HA_PN: strTexto = "Não há pontos notáveis"; break;
        case CCurHorizontais::CALCULO_NOK: strTexto = "Erro no arquivo de curvas horizontais."; break;
        case CCurHorizontais::ERRO_LER_ARQUIVO: strTexto = "Erro ao ler o arquivo de curvas horizontais."; break; 
      }

      pDialogo->SetDlgItemText(IDC_STDESCRICAO,strTexto.c_str());
      pDialogo->UpdateData(true);
    }
  }
  else
  {
    pDialogo->SetDlgItemText(IDC_STDESCRICAO,"Erro ao ler o arquivo de seçoes tipo");
    pDialogo->UpdateData(true);
  }

  if(pDialogo == this) m_buOK.EnableWindow(true); 
}

void CDCalcGeometrico::OnButcancelar() 
{
  EndDialog(true);	
}

void CDCalcGeometrico::OnBnClickedCheAjustarcurvas()
{
  m_EdiErroMaximo.EnableWindow(m_cheAjustarCurvas.GetCheck() == BST_CHECKED);

  m_buIniciar.EnableWindow(true);
}

void CDCalcGeometrico::OnEnKillfocusEdiErromaximo()
{
  double ErroAnt(m_douErroAjuste);

  UpdateData(true);

  if(m_douErroAjuste != ErroAnt) m_buIniciar.EnableWindow(true); 
}

void CDCalcGeometrico::OnEnChangeEdiErromaximo()
{
  UpdateData(true);

  m_buIniciar.EnableWindow(true); 
}


LRESULT CDCalcGeometrico::DisablePauseButton(WPARAM wParam, LPARAM lParam)
{
    return 0;
}