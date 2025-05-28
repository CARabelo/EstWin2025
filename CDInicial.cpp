// CDInicial.cpp : arquivo de implementação
//
#include "stdafx.h"
#include "Est.h"
#include "CMListBox.h"
#include "CDInicial.h"
#include "afxdialogex.h"

// caixa de diálogo de CDInicial

IMPLEMENT_DYNAMIC(CDInicial, CDialogEx)

CDInicial::CDInicial(int& pTipoProjeto,CWnd* pParent, CString* ppLstProjRec,bool pProjExistente)	: CDialogEx(IDD_DIAINICIAL, pParent),TipoProjeto(pTipoProjeto), CLstProjExistente(this),ProjExistente(pProjExistente)
{
}

CDInicial::~CDInicial()
{
}

void CDInicial::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISPROJEXISTENTE, CLstProjExistente);
}

BEGIN_MESSAGE_MAP(CDInicial, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTOPDRONES, &CDInicial::OnBnClickedButtopdrones)
	ON_BN_CLICKED(IDC_BUTTOPTRADICIONAL, &CDInicial::OnBnClickedButtoptradicional)
	ON_BN_CLICKED(IDC_BUTTOPMAPASIBGE, &CDInicial::OnBnClickedButtopmapasibge)
	ON_BN_CLICKED(IDC_BUTTOPGOOGLE, &CDInicial::OnBnClickedButtopgoogle)
	ON_BN_CLICKED(IDC_BUTMAISTOPTRADI, &CDInicial::OnBnClickedButmaistoptradi)
	ON_BN_CLICKED(IDC_BUTMAISDRONES, &CDInicial::OnBnClickedButmaisdrones)
	ON_BN_CLICKED(IDC_BUTMAISIBGE, &CDInicial::OnBnClickedButmaisibge)
	ON_BN_CLICKED(IDC_BUTMAISGOOGLE, &CDInicial::OnBnClickedButmaisgoogle)
	ON_BN_CLICKED(IDC_BUTTOPACAD, &CDInicial::OnBnClickedButtopacad)
	ON_BN_CLICKED(IDC_BUTMAISTOPAUTOCAD, &CDInicial::OnBnClickedButmaistopautocad)
	ON_BN_CLICKED(IDC_BUTORDENARPROJ, &CDInicial::OnBnClickedButordenarproj)
	ON_LBN_DBLCLK(IDC_LISPROJEXISTENTE, &CDInicial::OnLbnDblclkLisprojexistente)
	ON_BN_CLICKED(IDC_BUTPROCURARPROJ, &CDInicial::OnBnClickedButprocurarproj)
	ON_BN_CLICKED(IDOK, &CDInicial::OnBnClickedOk)
	ON_WM_CREATE()
	ON_WM_RBUTTONUP()
	ON_BN_CLICKED(IDCANCEL, &CDInicial::OnBnClickedCancel)
	ON_WM_KEYUP()
END_MESSAGE_MAP()

// manipuladores de mensagens de CDInicial

void CDInicial::OnBnClickedButtoptradicional()
{
  TipoProjeto = 1;
	CDialogEx::OnOK();
}

void CDInicial::OnBnClickedButtopdrones()
{
	TipoProjeto = 2;
	CDialogEx::OnOK();
}

void CDInicial::OnBnClickedButtopmapasibge()
{
	TipoProjeto = 3;
	CDialogEx::OnOK();
}

void CDInicial::OnBnClickedButtopacad()
{
	TipoProjeto = 4;
	CDialogEx::OnOK();
}

void CDInicial::OnBnClickedButtopgoogle()
{
	TipoProjeto = 5;
	CDialogEx::OnOK();
}

void CDInicial::OnBnClickedButmaistoptradi()
{
  ShowWindow(SW_HIDE);
	((CEstApp*)AfxGetApp())->ChamaHelp("InstalacaoDesinstalacao");
	ShowWindow(SW_SHOW);
}

void CDInicial::OnBnClickedButmaisdrones()
{
	// TODO: Adicione seu código de manipulador de notificações de controle aqui
}

void CDInicial::OnBnClickedButmaisibge()
{
	// TODO: Adicione seu código de manipulador de notificações de controle aqui
}

void CDInicial::OnBnClickedButmaisgoogle()
{
	// TODO: Adicione seu código de manipulador de notificações de controle aqui
}

void CDInicial::OnBnClickedButmaistopautocad()
{
	// TODO: Adicione seu código de manipulador de notificações de controle aqui
}

void CDInicial::OnBnClickedButordenarproj()
{
  tySetString SetProjetos;

	for (int C = 0 ; C < CLstProjExistente.GetCount() ; C++)
	{
	  CString ProjAtual;
		CLstProjExistente.GetText(C, ProjAtual);
		SetProjetos.emplace(ProjAtual.GetString());
	}

	CLstProjExistente.ResetContent();
	CLstProjExistente.ModifyStyle(0,LBS_SORT);

	for (auto& rNomeProj : SetProjetos)
	{
		CLstProjExistente.AddString(rNomeProj.c_str());
	}

	CLstProjExistente.RedrawWindow();
}

void CDInicial::OnLbnDblclkLisprojexistente()
{
	if (CLstProjExistente.GetCurSel() != CB_ERR) OnBnClickedOk();
}

BOOL CDInicial::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int TabStop(50);

	HFONT hFont((HFONT)GetStockObject(DEFAULT_GUI_FONT));
	if (hFont == NULL) hFont = (HFONT)GetStockObject(ANSI_VAR_FONT);

	CLstProjExistente.SetTabStops(1,&TabStop);
	CLstProjExistente.SetHorizontalExtent(1000);
	CLstProjExistente.EnableToolTips();     //--- Não funciona!!!

	if (pLstProjRecentes)
	{
		for (int C = 0; C < 50 ; C++)
		{
			if(pLstProjRecentes[C].GetLength() != 0)
			{
			  std::string NomeProj(pLstProjRecentes[C].Mid(pLstProjRecentes[C].ReverseFind('\\') +1));
			  
			  if (pLstProjRecentes[C] .Find('\t') == -1) NomeProj += "\t ";
			  else NomeProj.clear();

			  std::string Linha(NomeProj.c_str() + pLstProjRecentes[C]);

			  CLstProjExistente.AddString(Linha.c_str());
			}
		}
	}

	return TRUE;                 // return TRUE unless you set the focus to a control
								// EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}

int CDInicial::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CDInicial::OnRButtonUp(UINT nFlags, CPoint point)
{
	if((point.x & 0xFFFF) == WM_BUTUP_LISTBOX_PROJETOS)
	{
	  int ProjSel(CLstProjExistente.GetCurSel());

		//if (ProjSel != CB_ERR)
		if(0)
		{
			if (AfxMessageBox("Deseja realmente remover este projeto da lista de projetos existentes",MB_YESNO | MB_ICONEXCLAMATION) != IDYES) return;
			 
			CLstProjExistente.DeleteString(ProjSel);
		}
	}

	CDialogEx::OnRButtonUp(nFlags, point);
}

void CDInicial::OnBnClickedButprocurarproj()
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

			if(CLstProjExistente.FindString(0,Linha.c_str()) == CB_ERR)
			  CLstProjExistente.AddString(Linha.c_str());

			CLstProjExistente.SetCurSel(CLstProjExistente.FindString(0,Linha.c_str()));
		}
  }
}

void CDInicial::OnBnClickedOk()
{
  int ProjSel(CLstProjExistente.GetCurSel());
	CString CSProjSel;

	AtualizaListaProjRecentes();

	ProjSelecionado.clear();

	if (ProjSel != -1)
	{
		CLstProjExistente.GetText(ProjSel,CSProjSel);
		ProjSelecionado = CSProjSel.GetBuffer();

		ProjSelecionado.erase(0, ProjSelecionado.find('\t')+2);
	}

	CDialogEx::OnOK();
}

void CDInicial::AtualizaListaProjRecentes()
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

void CDInicial::OnBnClickedCancel()
{
	AtualizaListaProjRecentes();

	CDialogEx::OnCancel();
}


void CDInicial::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nRepCnt == WM_BUTUP_LISTBOX_PROJETOS)
	{
		int ProjSel(CLstProjExistente.GetCurSel());

		if (ProjSel != CB_ERR)
		{
			if (AfxMessageBox("Deseja realmente remover este projeto da lista de projetos existentes", MB_YESNO | MB_ICONEXCLAMATION) != IDYES) return;

			CLstProjExistente.DeleteString(ProjSel);
		}
	}

	CDialogEx::OnKeyUp(nChar, nRepCnt, nFlags);
}
