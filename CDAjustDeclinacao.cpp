// CDAjustDeclinacao.cpp : arquivo de implementação
//

#include "stdafx.h"
#include "Est.h"
#include "afxdialogex.h"
#include "DRotacionar.h"
#include <vector>
#include "CPonto.h"
#include <map>
#include "Estaca.h"
#include "CLPontos.h"
#include "Perfil.h"
#include "carqsectipo.h"
#include "CSecTipo.h"
#include "SuperCString.h"
#include "Mat.h"
#include "igualdades.h"
#include "CArqIgualdades.h"
#include "CIgualdade.h"
#include "Projeto.h"
#include "CArqCanteiros.h"
#include "CCanteiros.h"
#include "CCurhorizontal.h"
#include "CArqcurhorizontais.h"
#include "CCurHorizontais.h"
#include "CDeltaSuperficie.h" 
#include "CSegmento.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CCotasAreas.h"
#include "CDadosBacias.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "Superficie.h"
#include "CSecaoAvulsa.h"
#include "CSecoesAvulsas.h"
#include "CArqAlargamentos.h"
#include "CAlargamentos.h"
#include "CArqAciTopograficos.h"
#include "CArqTrechos.h"
#include "CTrecho.h"
#include "CTrechos.h"
#include "CLembrete.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "CRadar.h"
#include "CSecoesTerrap.h"
#include "ConvUTMGDC\ConvUTMGDC\Gdc_Coord_3d.h"
#include <set>
#include "ConvUTMGDC\ConvUTMGDC\Utm_Coord_3d.h"
#include "CCamadaDGN.h"
#include "XYGreide.h"
#include "meleditbox.h"
#include "CDPopUpPontos.h"

#include "CTextoMapaIBGE.h"
#include "CCNMapaIGBE.h"
#include "CMapaCNIBGE.h"
#include "CDiaImportarArqCtrlDrones.h"
#include "DesPontosGeomView.h"
#include "CDAjustDeclinacao.h"


// caixa de diálogo de CDAjustDeclinacao

IMPLEMENT_DYNAMIC(CDAjustDeclinacao, CDialogEx)

CDAjustDeclinacao::CDAjustDeclinacao(CWnd* pParent,DesPontosGeomView* ppDesGeom)	: CDialogEx(IDD_DIAAJUSTDECLIN, pParent), pDesGeom(ppDesGeom),
                            Rotacionar(true),Transladar(false)
{
}

CDAjustDeclinacao::~CDAjustDeclinacao()
{
}

void CDAjustDeclinacao::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTROTACIONAR, ButRotacionar);
	DDX_Control(pDX, IDC_BUTTRANSLADAR, ButTransladar);
}


BEGIN_MESSAGE_MAP(CDAjustDeclinacao, CDialogEx)
	ON_BN_CLICKED(IDC_BUTROTACIONAR, &CDAjustDeclinacao::OnBnClickedButrotacionar)
	ON_BN_CLICKED(IDC_BUTTRANSLADAR, &CDAjustDeclinacao::OnBnClickedButtransladar)
END_MESSAGE_MAP()

// manipuladores de mensagens de CDAjustDeclinacao

void CDAjustDeclinacao::OnBnClickedButrotacionar()
{
	bool OKRot(false),OkArrast(false),Cancelou(false);
	double NovoAngulo(0.0);
	int Absoluto_Relativo;

	CDRotacionar DRotacionar(this, 0.0, &NovoAngulo, &Absoluto_Relativo);

	while ((!OKRot) && !Cancelou)
	{
		if(!OKRot)
		{
	    if (DRotacionar.DoModal() == IDOK)
	    {
			  pDesGeom->Rotaciona(NovoAngulo, 1);
			  pDesGeom->RedrawWindow();
			  if (AfxMessageBox("Esta rotação esta ok?", MB_YESNO | MB_ICONEXCLAMATION) == IDYES) 
			  {
			    OKRot = true;
					ButRotacionar.EnableWindow(false);
			  }
	    }
		}
	}
}

void CDAjustDeclinacao::OnBnClickedButtransladar()
{

}

