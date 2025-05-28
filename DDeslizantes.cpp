// DDeslizantes.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h"
#include "afxtempl.h"
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
#include <list>
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include "xymouse.h"
#include "carqgradientes.h"
#include <set>
#include <string>
#include "cgradientes.h"
#include "dselsecao.h"
#include "ddeslizantes.h"
#include "listamedicoes.h"
#include "dvolumes.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
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
#include <map>
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
#include "xygreide.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "listamedicoes.h"
#include "CArqOAC.h"
#include "COAC.h"
#include "CArqOAE.h"
#include "COAE.h"
#include "CMemDC.h"
#include "desperfilview.h"
#include "CEquipDrenagem.h"
#include "superficie.h"
#include <vector>
#include "CLembrete.h"
#include "CRadar.h"
#include "CArqAciTopograficos.h"
#include "CTrecho.h"
#include "CArqTrechos.h"
#include "CTrechos.h"
#include "Picture.h"
//#include "ximage.h"
//#include ".\cximage600_full\CxImage\ximage.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "CSecoesTerrap.h"
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include <array>
#include "ConvUTMGDC\ConvUTMGDC\Gdc_To_Utm_Converter.h"
#include <math.h>
#include "ConvUTMGDC\ConvUTMGDC\CConvUTMGDC.h"
#include "CCamadaDGN.h"

#include "CTextoMapaIBGE.h"
#include "CCNMapaIGBE.h"
#include "CMapaCNIBGE.h"
#include "CDiaImportarArqCtrlDrones.h"
#include "despontosgeomview.h"
#include <algorithm>
#include "DesPerfilGeologicoView.h"
#include "mat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

////////////////////////////////////////////////////////////////////////////
// CDDeslizantes dialog

CDDeslizantes::CDDeslizantes(void* DesSec,CWnd* pParent,int GreSec) : CDialog(CDDeslizantes::IDD, pParent), m_pDesView(DesSec),m_INGreSec(GreSec)
{
  //{{AFX_DATA_INIT(CDDeslizantes)
  m_UIEscala = 1.0;
  m_DORazao = 0.1;
  //}}AFX_DATA_INIT
}

void CDDeslizantes::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDDeslizantes)
  DDX_Control(pDX, IDC_VALOR2, m_EDRazao);
  DDX_Control(pDX, IDC_VALOR, m_CEEscala);
  DDX_Control(pDX, IDC_DESLIZANTE2, m_SLValorRazao);
  DDX_Control(pDX, IDC_DESLIZANTE, m_SLValorEscala);
  DDX_Text(pDX, IDC_VALOR, m_UIEscala);
  DDX_Text(pDX, IDC_VALOR2, m_DORazao);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDDeslizantes, CDialog)
  //{{AFX_MSG_MAP(CDDeslizantes)
  ON_EN_CHANGE(IDC_VALOR, OnChangeEscala)
  ON_WM_HSCROLL()
  ON_EN_CHANGE(IDC_VALOR2, OnChangeRazao)
  //}}AFX_MSG_MAP
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDDeslizantes message handlers

BOOL CDDeslizantes::OnInitDialog() 
{
  CDialog::OnInitDialog();

  switch(m_INGreSec)
  {
    case GEOLOGICO:
    case GREIDE:
    {
      //--- Deslizante da escala ---------------------

      m_SLValorEscala.SetRange(10,800);
      m_SLValorEscala.SetPageSize(60);

      int i(0); 
      for (; i < 800 ; i+=80) m_SLValorEscala.SetTic(i);

      long	lStyleOld(GetWindowLong( m_SLValorEscala.GetSafeHwnd(), GWL_STYLE));
      lStyleOld |= TBS_ENABLESELRANGE;
      SetWindowLong( m_SLValorEscala.GetSafeHwnd(), GWL_STYLE, lStyleOld );

      //--- Deslizante da razão ----------------------

      m_SLValorRazao.SetRange(6,60);
      m_SLValorRazao.SetPageSize(6);

      for (i=0 ; i < 60 ; i+=6) m_SLValorRazao.SetTic(i);

      m_SLValorRazao.SetTic(10);

      break;
    };

  case SECOES:
    {
      //--- Deslizante da escala ---------------------

      m_SLValorEscala.SetRange(10,500);
      m_SLValorEscala.SetPageSize(50);

      int i(0);
      for (; i < 500 ; i+=50) m_SLValorEscala.SetTic(i);

      long	lStyleOld(GetWindowLong( m_SLValorEscala.GetSafeHwnd(), GWL_STYLE));
      lStyleOld |= TBS_ENABLESELRANGE;
      SetWindowLong( m_SLValorEscala.GetSafeHwnd(), GWL_STYLE, lStyleOld );

      //--- Deslizante da razão ----------------------

      m_SLValorRazao.SetRange(10,200);
      m_SLValorRazao.SetPageSize(20);

      for (i=0 ; i < 200 ; i+= 20) m_SLValorRazao.SetTic(i);

      lStyleOld = GetWindowLong( m_SLValorRazao.GetSafeHwnd(), GWL_STYLE);
      lStyleOld |= TBS_ENABLESELRANGE;
      SetWindowLong( m_SLValorRazao.GetSafeHwnd(), GWL_STYLE, lStyleOld );

      UpdateData(FALSE);
      break;
    }
  }

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDDeslizantes::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  if((CSliderCtrl *) pScrollBar == &m_SLValorEscala)
  {
    m_UIEscala = m_SLValorEscala.GetPos();
    UpdateData(FALSE);

    switch(m_INGreSec)
    {
      case SECOES :  ((DesSecaoView*) m_pDesView)->MudarEscala(m_UIEscala);break;
      case GREIDE :  ((DesPerfilView*) m_pDesView)->MudarEscala(m_UIEscala); break;
      case GEOLOGICO : ((DesPerfilGeoView*) m_pDesView)->MudarEscala(m_UIEscala); break;
    }
  }
  else 
  {
    if((CSliderCtrl *) pScrollBar == &m_SLValorRazao)
    {
      m_DORazao = m_SLValorRazao.GetPos() / 100.0;

      if (m_DORazao > 0.0009)
      {
        UpdateData(FALSE);

        switch (m_INGreSec)
        {
          case SECOES: ((DesSecaoView*)m_pDesView)->MudarRazao(m_DORazao); break;
          case GREIDE: ((DesPerfilView*)m_pDesView)->MudarRazao(m_DORazao); break;
          case GEOLOGICO: ((DesPerfilGeoView*)m_pDesView)->MudarRazao(m_DORazao); break;
        }
      }
      else CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
    }
    else	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
  }
}

void CDDeslizantes::OnChangeEscala() 
{
//  // TODO: If this is a RICHEDIT control, the control will not
//  // send this notification unless you override the CDialog::OnInitDialog()
//  // function to send the EM_SETEVENTMASK message to the control
//  // with the ENM_CHANGE flag ORed into the lParam mask.
//

  {
    if(UpdateData(TRUE))
    {
      m_SLValorEscala.SetPos(m_UIEscala);	
//
      switch(m_INGreSec)
      { 
        case SECOES: ((DesSecaoView*) m_pDesView)->MudarEscala(m_UIEscala);break;
        case GREIDE: ((DesPerfilView*) m_pDesView)->MudarEscala(m_UIEscala);break;
        case GEOMETRICO: ((DesPontosGeomView*) m_pDesView)->MudarEscala(m_UIEscala);break;
        case GEOLOGICO: ((DesPerfilGeoView*) m_pDesView)->MudarEscala(m_UIEscala);break;
      }
    }
  }
}

void CDDeslizantes::OnChangeRazao() 
{
  // TODO: If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function to send the EM_SETEVENTMASK message to the control
  // with the ENM_CHANGE flag ORed into the lParam mask.

  // TODO: Add your control notification handler code here

 // if (m_EDRazao.LineLength() > 1)
  {
    if(UpdateData(TRUE))
    {
      m_SLValorRazao.SetPos(int (m_DORazao * 50.0));	

      switch (m_INGreSec)
      {
      case SECOES: ((DesSecaoView*) m_pDesView)->MudarRazao(m_DORazao);break;
      case GREIDE: ((DesPerfilView*) m_pDesView)->MudarRazao(m_DORazao);break;
      case GEOLOGICO : ((DesPerfilGeoView*) m_pDesView)->MudarRazao(m_UIEscala); break;
      }
    }
  }
}

void CDDeslizantes::MudarEscala(double Valor)
{
 // m_UIEscala = Valor * 100.0;

  if(m_INGreSec == GREIDE) Valor *= 100.0;

  m_UIEscala = Valor;

  m_SLValorEscala.SetPos(m_UIEscala);	
  UpdateData(false);
}

void CDDeslizantes::MudarRazao(double Valor)
{
  m_DORazao = Valor;
  m_SLValorRazao.SetPos((int) (m_DORazao * 100.0));	
  UpdateData(false);
}

void CDDeslizantes::OnOK(){}    //--- Anula o OnOK

void CDDeslizantes::MouseWheelOn(UINT nFlags, short zDelta, CPoint pt)
{
  unsigned int LimSup(m_INGreSec == SECOES ? 1000 : 1000);

  if ((zDelta < 0 && m_UIEscala > 0) || m_UIEscala < LimSup)
  {
    double Delta(ceil(m_UIEscala * (zDelta / 120.0 * .1)));

    if(Delta == 0.0) Delta = 1.0 * Matem::Sgn(zDelta);
    if(Delta > 0 || m_UIEscala > abs(Delta)) m_UIEscala +=  Delta;
    if(m_UIEscala == 0) m_UIEscala = 1;

    m_SLValorEscala.SetPos(m_UIEscala);	

    switch(m_INGreSec)
    {
    case SECOES :  ((DesSecaoView*) m_pDesView)->MudarEscala(m_UIEscala);break;
    case GREIDE :  ((DesPerfilView*) m_pDesView)->MudarEscala(m_UIEscala); break;
    case GEOLOGICO :  ((DesPerfilGeoView*) m_pDesView)->MudarEscala(m_UIEscala); break;
    }

    UpdateData(false);
  }
}

void CDDeslizantes::OnSetFocus(CWnd* pOldWnd)
{
  POINT PontoMouse;
   
  GetCursorPos(&PontoMouse);
  
  CRect Rect;

  m_CEEscala.GetWindowRect(&Rect);

  if(Rect.PtInRect(PontoMouse)) m_CEEscala.SetFocus();

  m_EDRazao.GetWindowRect(&Rect);

  if(Rect.PtInRect(PontoMouse)) m_EDRazao.SetFocus();
}
