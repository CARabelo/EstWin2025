// CurVerView.cpp : implementation file
//
#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "estdoc.h"
#include "typlstvw.h"
#include "CMListBox.h"
#include "CDialogoInicialView.h"
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "Mainfrm.h"
#include "Estaca.h"
#include "cponto.h" 
#include "clpontos.h" 
#include "perfil.h"
#include "supercstring.h"
#include "xygreide.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "xymouse.h"
#include "mytoolbar.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "comboprocdessecao.h"
#include "CArqOAC.h"
#include "CArqOAE.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CPerfilGeologico.h"
#include "CMemDC.h"
#include "CTipoSolos.h"
#include "ddeslizantes.h"
#include "deslizantesgeom.h"
#include "palette.h"
#include "childfrm.h"
#include "dialogo.h"
#include "monologo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class dialogo dialogar;
extern class monologo monolog;

/////////////////////////////////////////////////////////////////////////////
// CCurVerView

IMPLEMENT_DYNCREATE(CDialogoInicialView, CFormView)

CDialogoInicialView::CDialogoInicialView(int* pTipoProj, CWnd* pParent, CString* ppLstProjRec):CFormView(CDialogoInicialView::IDD), pTipoProjeto(pTipoProj), CLstProjExistente(this),pLstProjRecentes(ppLstProjRec),ProjExistente(false), BrushWhite(RGB(255,255,255))
, CheIniDesSuperf(TRUE)
{
}

CDialogoInicialView::~CDialogoInicialView()
{
}

void CDialogoInicialView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISPROJEXISTENTE, CLstProjExistente);
	DDX_Check(pDX, IDC_CHEINIDESSUPERF, CheIniDesSuperf);
}

BEGIN_MESSAGE_MAP(CDialogoInicialView, CFormView)
	ON_BN_CLICKED(IDC_BUTTOPDRONES, &CDialogoInicialView::OnBnClickedButtopdrones)
	ON_BN_CLICKED(IDC_BUTTOPTRADICIONAL, &CDialogoInicialView::OnBnClickedButtoptradicional)
	ON_BN_CLICKED(IDC_BUTTOPMAPASIBGE, &CDialogoInicialView::OnBnClickedButtopmapasibge)
	ON_BN_CLICKED(IDC_BUTTOPGOOGLE, &CDialogoInicialView::OnBnClickedButtopgoogle)
	ON_BN_CLICKED(IDC_BUTMAISTOPTRADI, &CDialogoInicialView::OnBnClickedButmaistoptradi)
	ON_BN_CLICKED(IDC_BUTTOPACAD, &CDialogoInicialView::OnBnClickedButtopacad)
	ON_BN_CLICKED(IDC_BUTORDENARPROJ, &CDialogoInicialView::OnBnClickedButordenarproj)
	ON_LBN_DBLCLK(IDC_LISPROJEXISTENTE, &CDialogoInicialView::OnLbnDblclkLisprojexistente)
	ON_BN_CLICKED(IDC_BUTPROCURARPROJ, &CDialogoInicialView::OnBnClickedButprocurarproj)
	ON_BN_CLICKED(IDOK, &CDialogoInicialView::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogoInicialView::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTMAISGOOGLE, &CDialogoInicialView::OnBnClickedButmaisgoogle)
	ON_BN_CLICKED(IDC_BUTMAISIBGE, &CDialogoInicialView::OnBnClickedButmaisibge)
	ON_BN_CLICKED(IDC_BUTMAISDRONES, &CDialogoInicialView::OnBnClickedButmaisdrones)
	ON_UPDATE_COMMAND_UI(IDOK, &CDialogoInicialView::OnUpdateButOK)  
	ON_BN_CLICKED(IDC_BUTTOPSIG, &CDialogoInicialView::OnBnClickedButtopsig)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// manipuladores de mensagens de CDialogoInicialView

void CDialogoInicialView::OnBnClickedButtoptradicional()
{
	((CEstApp*)AfxGetApp())->AbreProjExistente(1, ProjSelecionado,this);
}

void CDialogoInicialView::OnBnClickedButtopdrones()
{
	((CEstApp*)AfxGetApp())->AbreProjExistente(2, ProjSelecionado, this);
}

void CDialogoInicialView::OnBnClickedButtopmapasibge()
{
	((CEstApp*)AfxGetApp())->AbreProjExistente(3, ProjSelecionado, this);
}

void CDialogoInicialView::OnBnClickedButtopacad()
{
	((CEstApp*)AfxGetApp())->AbreProjExistente(4, ProjSelecionado, this);
}

void CDialogoInicialView::OnBnClickedButtopgoogle()
{
	((CEstApp*)AfxGetApp())->AbreProjExistente(5, ProjSelecionado, this);
}

void CDialogoInicialView::OnBnClickedButtopsig()
{
	((CEstApp*)AfxGetApp())->AbreProjExistente(6, ProjSelecionado, this);
}

void CDialogoInicialView::OnBnClickedButmaistoptradi()
{
	ShowWindow(SW_HIDE);
	((CEstApp*)AfxGetApp())->ChamaHelp("ImportarArquivoxyz");
	ShowWindow(SW_SHOW);
}

void CDialogoInicialView::OnBnClickedButordenarproj()
{
	tySetString SetProjetos;

	for (int C = 0; C < CLstProjExistente.GetCount(); C++)
	{
		CString ProjAtual;
		CLstProjExistente.GetText(C, ProjAtual);
		SetProjetos.emplace(ProjAtual.GetString());
	}

	CLstProjExistente.ResetContent();
	CLstProjExistente.ModifyStyle(0, LBS_SORT);

	for (auto& rNomeProj : SetProjetos)
	{
		CLstProjExistente.AddString(rNomeProj.c_str());
	}

	CLstProjExistente.RedrawWindow();
}

void CDialogoInicialView::OnLbnDblclkLisprojexistente()
{
	if (CLstProjExistente.GetCurSel() != CB_ERR) OnBnClickedOk();
}

void CDialogoInicialView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	int TabStop(50);

	HFONT hFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	if (hFont == NULL) hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);

	CLstProjExistente.SetTabStops(1, &TabStop);
	CLstProjExistente.SetHorizontalExtent(1000);
	CLstProjExistente.EnableToolTips();     //--- Não funciona!!!

	pLstProjRecentes = ((CMainFrame*)AfxGetMainWnd())->PegaProjRecentes();

	if (pLstProjRecentes)
	{
	  int ContaProjetos(0);
		for (int C = 0; C < 50; C++)
		{
			if (pLstProjRecentes[C].GetLength() != 0)
			{
				ContaProjetos++;
				std::string NomeProj(pLstProjRecentes[C].Mid(pLstProjRecentes[C].ReverseFind('\\') + 1));

				if (pLstProjRecentes[C].Find('\t') == -1) NomeProj += "\t ";
				else NomeProj.clear();

				std::string Linha(NomeProj.c_str() + pLstProjRecentes[C]);

				CLstProjExistente.AddString(Linha.c_str());
			}
		}
		if (ContaProjetos == 0)
		{
			monolog.mensagem(-1, "\r\nOs Projetos de exemplos encontram-se em:\r\n\r\n ..\\Meus Documentos\\ViasUrbanas & Estradas\\Exemplos\\");
		}
	}

	bool ApagarBotoes(true);

  #ifdef PACOTE_AVANCADO
    ApagarBotoes = false;
  #endif

	if(ApagarBotoes)
	{
	  GetDlgItem(IDC_BUTTOPMAPASIBGE)->EnableWindow(false);
	  GetDlgItem(IDC_BUTTOPGOOGLE)->EnableWindow(false);
	  GetDlgItem(IDC_BUTMAISTOPTRADI)->EnableWindow(false);
	  GetDlgItem(IDC_BUTTOPACAD)->EnableWindow(false);
	  GetDlgItem(IDC_BUTORDENARPROJ)->EnableWindow(false);
	  GetDlgItem(IDC_BUTTOPDRONES)->EnableWindow(false);
	  GetDlgItem(IDC_BUTTOPDRONES)->EnableWindow(false);
	  GetDlgItem(IDC_BUTTOPSIG)->EnableWindow(false);
      #ifdef DEMOVER
	    GetDlgItem(IDC_BUTTOPTRADICIONAL)->EnableWindow(false);
	  #endif
  }

	CheIniDesSuperf = ((CMainFrame*)AfxGetMainWnd())->IniciarDesSuperficie();

	UpdateData(false);
}

int CDialogoInicialView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDialogoInicialView::OnRButtonUp(UINT nFlags, CPoint point)
{
	if ((point.x & 0xFFFF) == WM_BUTUP_LISTBOX_PROJETOS)
	{
		int ProjSel(CLstProjExistente.GetCurSel());

		//if (ProjSel != CB_ERR)
		if (0)
		{
			if (dialogar.DoModal(-1,"Deseja realmente remover este projeto da lista de projetos existentes") != IDOK) return;

			CLstProjExistente.DeleteString(ProjSel);
		}
	}

	CFormView::OnRButtonUp(nFlags, point);
}

void CDialogoInicialView::OnBnClickedButprocurarproj()
{
	CString filtro = "Cabeçalho (*.prj) | *.prj||", NomeProj;

	CFileDialog diaabrearq(TRUE, NULL, NULL, OFN_EXPLORER | OFN_FILEMUSTEXIST, filtro, NULL);

	diaabrearq.m_ofn.lpstrTitle = "Vias Urbanas & Estradas - Abrir projeto :";

	if (diaabrearq.DoModal() == IDOK)
	{
		CString PathProj(diaabrearq.GetPathName());

		if (!PathProj.IsEmpty())
		{
			PathProj = PathProj.Left(PathProj.GetLength() - 4);
			std::string NomeProj(PathProj.Mid(PathProj.ReverseFind('\\') + 1));
			NomeProj += "\t ";

			std::string Linha(NomeProj.c_str() + PathProj);

			if (CLstProjExistente.FindString(0, Linha.c_str()) == CB_ERR)
				CLstProjExistente.AddString(Linha.c_str());

			CLstProjExistente.SetCurSel(CLstProjExistente.FindString(0, Linha.c_str()));
		}
	}
}

void CDialogoInicialView::OnBnClickedOk()
{
  int ProjSel(CLstProjExistente.GetCurSel());

  if (ProjSel != -1)
  {
    CString CSProjSel;

    AtualizaListaProjRecentes();

    ProjSelecionado.clear();
    CLstProjExistente.GetText(ProjSel, CSProjSel);
    ProjSelecionado = CSProjSel.GetBuffer();
    ProjSelecionado.erase(0, ProjSelecionado.find('\t') + 2);

    UpdateData(true);

    if (VerfTipoProjeto(ProjSelecionado))
    {
			((CMainFrame*)AfxGetMainWnd())->GravaProjetosRecentes();
      ((CMainFrame*)AfxGetMainWnd())->IniciarDesSuperficie() = CheIniDesSuperf;

      ((CEstApp*)AfxGetApp())->AbreProjExistente(-1, ProjSelecionado, this, CheIniDesSuperf);
    }
		else
		{
			monolog.mensagem(-1, "Projetos georeferenciados não podem ser abertos neste tipo de pacote\r\n "
				                 "adquira o pacote avançado ");
		}
  }
}

void CDialogoInicialView::AtualizaListaProjRecentes()
{
	int C(0);

	for (; C < CLstProjExistente.GetCount(); C++)
	{
		CString ProjAtual;

		CLstProjExistente.GetText(C, ProjAtual);

		pLstProjRecentes[C] = ProjAtual;
	}

	for (; C < 50; C++) pLstProjRecentes[C].Empty();
}

void CDialogoInicialView::OnBnClickedCancel()
{
	AtualizaListaProjRecentes();

	//CFormView::OnCancel();
}

void CDialogoInicialView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nRepCnt == WM_BUTUP_LISTBOX_PROJETOS)
	{
		int ProjSel(CLstProjExistente.GetCurSel());

		if (ProjSel != CB_ERR)
		{
			if (dialogar.DoModal(-1,"Deseja realmente remover este projeto da lista de projetos existentes") != IDOK) return;

			CLstProjExistente.DeleteString(ProjSel);
		}
	}

	CFormView::OnKeyUp(nChar, nRepCnt, nFlags);
}

HBRUSH CDialogoInicialView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hBrush(NULL);

	switch (nCtlColor)
	{
	case CTLCOLOR_DLG:
		// just return a not NULL brush handle
		hBrush = (HBRUSH)BrushWhite;
		break;
	case CTLCOLOR_STATIC:
	{
		// set text color, transparent back node then 
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkMode(TRANSPARENT);
		// return a not NULL brush handle
		hBrush = (HBRUSH)BrushWhite;
	}
	break;
	default:
		// do the default processing
		hBrush = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
		break;
	}
	return hBrush;
}

BOOL CDialogoInicialView::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClip;
	pDC->GetClipBox(rcClip);
	pDC->FillRect(rcClip, &BrushWhite);
	return TRUE; 
}

void CDialogoInicialView::OnBnClickedButmaisgoogle()
{
	ShowWindow(SW_HIDE);
	((CEstApp*)AfxGetApp())->ChamaHelp("ImportarArquivoKMLGoogleEarth");
	ShowWindow(SW_SHOW);
}

void CDialogoInicialView::OnBnClickedButmaisibge()
{
	ShowWindow(SW_HIDE);
	((CEstApp*)AfxGetApp())->ChamaHelp("EditoresdeMapas");
	ShowWindow(SW_SHOW);
}

void CDialogoInicialView::OnBnClickedButmaisdrones()
{
	ShowWindow(SW_HIDE);
	((CEstApp*)AfxGetApp())->ChamaHelp("Drones");
	ShowWindow(SW_SHOW);
}
void CDialogoInicialView::OnUpdateButOK(CCmdUI* pCmdUI)
{
  pCmdUI->Enable(false);
}

int CDialogoInicialView::VerfTipoProjeto(std::string& Projeto)
{
  //--- Verifica se o tipo do pacote pode abrir este tipo de projeto

	char Buffer[1024] = { 0 };
	CString Default;

	std::string NomeArquivo(Projeto  + std::string(".ini"));

	Default.Format("%lf,%lf,%lf,%lf,%lf,%i,%lf,%lf,%lf,%s", 0.0, 0.0, 1.0, 0, 1.0, 0.0, 0.0, 0.0, 0.0, "");
	::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosMapa"), Default, Buffer, 511, NomeArquivo.c_str());

	std::stringstream strstrBuffer(Buffer);
	char Virgula;
	double Lixo;
	int ProjGeoreferenciado(-1);

	strstrBuffer >> Lixo >> Virgula >> Lixo >> Virgula >> Lixo >> Virgula
		         >> Lixo >> Virgula >> Lixo >> Virgula >> ProjGeoreferenciado;

  bool Permitir(true);

  #ifndef PACOTE_AVANCADO
    #ifndef DEMOVER
	  Permitir = false;
	#endif
  #endif

  if(ProjGeoreferenciado)
  {
   return Permitir;
  }

  return true;
}