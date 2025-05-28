// DeslizantesGeom.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "DeslizantesGeom.h"
#include "math.h"
#include "Estaca.h"
#include "cponto.h"
#include "afxtempl.h"
#include "clpontos.h" 
#include "perfil.h"
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
#include "listamedicoes.h"
#include "dvolumes.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include "carqpistasfixas.h"
#include "cpistasfixas.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
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

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeslizantesGeom dialog

CDeslizantesGeom::CDeslizantesGeom(void* DesGeo,CWnd* pParent): CDialog(CDeslizantesGeom::IDD, pParent),m_pDesView(DesGeo)
{
  //{{AFX_DATA_INIT(CDeslizantesGeom)
  m_intEscala = 0;
  //}}AFX_DATA_INIT
}

void CDeslizantesGeom::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDeslizantesGeom)
  DDX_Control(pDX, IDC_DESLIZANTE, m_SLValorEscala);
  DDX_Control(pDX, IDC_VALOR, m_CEEscala);
  DDX_Text(pDX, IDC_VALOR, m_intEscala);
 // DDV_MinMaxUInt(pDX, m_intEscala, 5, 5000);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDeslizantesGeom, CDialog)
  //{{AFX_MSG_MAP(CDeslizantesGeom)
  ON_EN_CHANGE(IDC_VALOR, OnChangeEscala)
  ON_WM_HSCROLL()
  //}}AFX_MSG_MAP
  ON_WM_MOUSEHWHEEL()
  ON_WM_SETFOCUS()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeslizantesGeom message handlers

void CDeslizantesGeom::MudarEscala(double Valor)
{
  m_intEscala = Valor * 100.0;
  if(m_intEscala < 1) m_intEscala = 1;
  m_SLValorEscala.SetPos(m_intEscala);	
  UpdateData(false);
}

void CDeslizantesGeom::OnOK(){}    //--- Anula o OnOK

void CDeslizantesGeom::OnChangeEscala() 
{
//  // TODO: If this is a RICHEDIT control, the control will not
//  // send this notification unless you override the CDialog::OnInitDialog()
//  // function and call CRichEditCtrl().SetEventMask()
//  // with the ENM_CHANGE flag ORed into the mask.
//
//  // TODO: Add your control notification handler code here
//
  UpdateData(TRUE);
//
  if (m_intEscala > 3 && m_intEscala < 50000)
  {
    m_SLValorEscala.SetPos(m_intEscala);	

    ((DesPontosGeomView*) m_pDesView)->MudarEscala(m_intEscala);
    ((DesPontosGeomView*)m_pDesView)->RedrawWindow();
  }
}

BOOL CDeslizantesGeom::OnInitDialog() 
{
  CDialog::OnInitDialog();

  m_SLValorEscala.SetRange(10,20000);
  m_SLValorEscala.SetPageSize(1000);

  for (int i=0 ; i < 20000 ; i+=1000) m_SLValorEscala.SetTic(i);

  long	lStyleOld(GetWindowLong( m_SLValorEscala.GetSafeHwnd(), GWL_STYLE));
  lStyleOld |= TBS_ENABLESELRANGE;
  SetWindowLong( m_SLValorEscala.GetSafeHwnd(), GWL_STYLE, lStyleOld );

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDeslizantesGeom::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  // TODO: Add your message handler code here and/or call default

//  if(((DesPontosGeomView*)m_pDesView)->GeoReferenciado() && !((DesPontosGeomView*)m_pDesView)->MapaEstaSolto()) return;

  m_intEscala = m_SLValorEscala.GetPos();

  UpdateData(FALSE);

  ((DesPontosGeomView*) m_pDesView)->MudarEscala(m_intEscala); 
  ((DesPontosGeomView*)m_pDesView)->RedrawWindow();

  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CDeslizantesGeom::MouseWheelOn(UINT nFlags, short zDelta, CPoint pt)
{
  // This feature requires Windows Vista or greater.
  // The symbol _WIN32_WINNT must be >= 0x0600.
  // TODO: Add your message handler code here and/or call default

  if ((m_intEscala > 0 &&  m_intEscala < 10000) || (m_intEscala <= 1 && zDelta > 0) || (m_intEscala >= 10000 && zDelta < 0))
  {
    double Delta( m_intEscala * (zDelta / 120.0 * .1));

    if(Delta < 1.0 && Delta > 0.0) Delta = 1.0;
    if(Delta > -1.0 && Delta < 0.0) Delta = -1.0;

    m_intEscala += Delta;

	  if(m_intEscala < 1) m_intEscala = 1;

    m_SLValorEscala.SetPos(m_intEscala);	

    UpdateData(false);

    ((DesPontosGeomView*) m_pDesView)->MudarEscala(m_intEscala,zDelta);
  }
}

void CDeslizantesGeom::OnSetFocus(CWnd* pOldWnd)
{
  POINT PontoMouse;
   
  GetCursorPos(&PontoMouse);
  
  CRect Rect;

  m_CEEscala.GetWindowRect(&Rect);

  if(Rect.PtInRect(PontoMouse)) m_CEEscala.SetFocus();
}

