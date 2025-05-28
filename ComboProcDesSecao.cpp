// ComboProcurar.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "estaca.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "xymouse.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "cponto.h"
#include "clpontos.h" 
#include "perfil.h"
#include "afxtempl.h"
#include "supercstring.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "carqtalaterro.h"
#include "ctalcorte.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include "clistaperfis.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "carqgradientes.h"
#include <set>
#include <string>
#include "cgradientes.h" 
#include "dselsecao.h"
#include "comboprocdessecao.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "calcnota.h"
#include <string>
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "carqcurhorizontais.h"
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "dessecaoview.h"
#include "carqcurverticais.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurverticais.h"
#include "xygreide.h"
#include "listamedicoes.h"
#include "CArqOAC.h"
#include "COAC.h"
#include "CArqOAE.h"
#include "COAE.h"
#include "CMemDC.h"
#include "desperfilview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CComboProcurarDesSecao

CComboProcurarDesSecao::CComboProcurarDesSecao()
{
}

CComboProcurarDesSecao::~CComboProcurarDesSecao()
{
}

BEGIN_MESSAGE_MAP(CComboProcurarDesSecao, CComboBox)
  //{{AFX_MSG_MAP(CComboProcurarDesSecao)
  ON_CONTROL_REFLECT(CBN_SELENDOK, OnSelendok)
  //}}AFX_MSG_MAP
  ON_WM_KEYDOWN()
  ON_CONTROL_REFLECT(CBN_SETFOCUS, OnCbnSetfocus)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CComboProcurarDesSecao message handlers

void CComboProcurarDesSecao::OnSelendok() 
{
  bool SecAvulsa(((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())->DesenhandoSecAvulsa()),
       Talvegue(((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())->DesenhandoTalvegue()),
       Hipsograma(((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())->DesenhandoHistograma());

  ((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())->RedrawWindow();

  GetLBText(GetCurSel(),Selecao);
  if(!SecAvulsa && !Talvegue && !Hipsograma) Selecao = Selecao.Left(Selecao.ReverseFind(' '));
  CEstaca EstacaSelecionada(Selecao);

  //--- Se estiver com o desenho do perfil e não for medicao chama o método para recálculo da cota, caso contrario chama direto no desenho da seção

  if (((CMainFrame*) AfxGetMainWnd())->DesPerfil && !((CMainFrame*) AfxGetMainWnd())->EMedicao)
  {
    ((DesPerfilView*)((CMainFrame*) AfxGetMainWnd())->DesPerfil)->CalculaCotaEPosiciona(EstacaSelecionada);
  }
  else
  {
    std::string NomeSelecionado(Selecao.GetBuffer());
    int Tipo(SecAvulsa ? 1 : Talvegue ? 2 : Hipsograma ? 3 : 0);

    ((DesSecaoView*)((CMainFrame*)AfxGetMainWnd())->PegaDesenhoSecoes())->Posiciona(EstacaSelecionada.EstVirtual, INFINITO, true, Tipo, &NomeSelecionado);
  }
}

void CComboProcurarDesSecao::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
  // TODO: Add your message handler code here and/or call default

  PressionouSetaAbaixo = nChar == VK_DOWN;
  PressionouSetaAcima = nChar == VK_UP;

  CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CComboProcurarDesSecao::OnCbnSetfocus()
{
   CEstaca EstAtual(((DesSecaoView*)((CMainFrame*) AfxGetMainWnd())->PegaDesenhoSecoes())->PegaEstacaAtual());

   if(EstAtual.EstVirtual != INFINITO) SetCurSel(FindString(0,EstAtual));
}