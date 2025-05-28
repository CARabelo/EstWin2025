// Instalar.cpp : implementation file
//

#include "stdafx.h"
#include "teste.h"
#include "Instalar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Instalar dialog


Instalar::Instalar(CWnd* pParent /*=NULL*/)
	: CDialog(Instalar::IDD, pParent)
{
	//{{AFX_DATA_INIT(Instalar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void Instalar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Instalar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Instalar, CDialog)
	//{{AFX_MSG_MAP(Instalar)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Instalar message handlers

BOOL Instalar::OnInitDialog() 
{
	CDialog::OnInitDialog();

    SetDlgItem(IDC_ORIGEM,"A:\\");
    SetDlgItem(IDC_DESTINO,"C:\ESTRADAS");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void Instalar::OnOK() 
{
	CString Dir_Origem = GetDlgItem(IDC_ORIGEM),
		    Dir_Destino = GetDlgItem(IDC_DESTINO);
 
	if (!_chdir(Dir_Origem))
	{
	  //erro "drive a não está pronto"
	}
	else
	{
	  if (_chdir(Dir_Destino))
	  {
		  // Mensagem: - Diretório de destiono já existe sobrescrever?
	      // caso positivo continuar a inatalação
	  }
	  else
	  {
	    if (!_mkdir(Dir_Destino))
		{
		  // diretório de destino não pode ser criado!

		}
		else
		{
			continuar a instalação
		}
	  }
	}
  }

  CDialog::OnOK();
}
