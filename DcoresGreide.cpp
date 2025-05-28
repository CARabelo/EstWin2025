// DcoresGreide.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "meleditbox.h"
#include "DcoresGreide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDcoresGreide dialog


CDcoresGreide::CDcoresGreide(int PTipo,CWnd* pParent,COLORREF* pCores) : Tipo(PTipo), CDialog(CDcoresGreide::IDD, pParent),pCoresRef(pCores),m_EDCorFundo(*pCores),
m_EDCorMalha(*(pCores+1)),m_EDCorTexto(*(pCores+2)),m_EDCorTerreno(*(pCores+3)),
m_EDCorTerraplanagem(*(pCores+4)),m_EDCorPavimento(*(pCores+5))

{
  //{{AFX_DATA_INIT(CDcoresGreide)
  //}}AFX_DATA_INIT
}

void CDcoresGreide::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CDcoresGreide)
  DDX_Control(pDX, IDC_CORTEXTO, m_EDCorTexto);
  DDX_Control(pDX, IDC_CORTERRENO, m_EDCorTerreno);
  DDX_Control(pDX, IDC_CORTERRAPLANAGEM, m_EDCorTerraplanagem);
  DDX_Control(pDX, IDC_CORPAVIMENTO, m_EDCorPavimento);
  DDX_Control(pDX, IDC_CORMALHA, m_EDCorMalha);
  DDX_Control(pDX, IDC_CORFUNDO, m_EDCorFundo);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDcoresGreide, CDialog)
  //{{AFX_MSG_MAP(CDcoresGreide)
  ON_BN_CLICKED(IDC_RADIOFUNDO, OnRadiofundo)
  ON_BN_CLICKED(IDC_RADIOMALHA, OnRadiomalha)
  ON_BN_CLICKED(IDC_RADIOPAVIM, OnRadiopavim)
  ON_BN_CLICKED(IDC_RADIOTERRAPLANAGEM, OnRadioterraplanagem)
  ON_BN_CLICKED(IDC_RADIOTERRENO, OnRadioterreno)
  ON_BN_CLICKED(IDC_RADIOTEXTO, OnRadiotexto)
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDcoresGreide message handlers

void CDcoresGreide::MudaCor(CMelEditBox* EditBox) 
{
  CColorDialog DCor;

  if (DCor.DoModal() == IDOK)
  {
    EditBox->MudaCorFundo(DCor.GetColor());
    CDC* pDC = EditBox->GetDC();

    if(!pDC) return;     //--- Segurança, aconteçeu nos testes


    CRect rc;                                         
    EditBox->GetClientRect(&rc); 
    pDC->FillSolidRect( rc, DCor.GetColor());		//--- Pìnta o fundo

    EditBox->ReleaseDC(pDC);

  }
}

void CDcoresGreide::OnRadiofundo() 
{
  MudaCor(&m_EDCorFundo);
}

void CDcoresGreide::OnRadiomalha() 
{
  MudaCor(&m_EDCorMalha);	
}

void CDcoresGreide::OnRadiopavim() 
{
  MudaCor(&m_EDCorPavimento);
}

void CDcoresGreide::OnRadioterraplanagem() 
{
  MudaCor(&m_EDCorTerraplanagem);
}

void CDcoresGreide::OnRadioterreno() 
{
  MudaCor(&m_EDCorTerreno);
}

void CDcoresGreide::OnRadiotexto() 
{
  MudaCor(&m_EDCorTexto);	
}

void CDcoresGreide::OnOK() 
{
  *pCoresRef = m_EDCorFundo.PegaCorFundo();
  *(pCoresRef+1) = m_EDCorMalha.PegaCorFundo();
  *(pCoresRef+2) = m_EDCorTexto.PegaCorFundo();
  *(pCoresRef+3) = m_EDCorTerreno.PegaCorFundo();
  *(pCoresRef+4) = m_EDCorTerraplanagem.PegaCorFundo();
  *(pCoresRef+5) = m_EDCorPavimento.PegaCorFundo();

  CDialog::OnOK();
}

BOOL CDcoresGreide::OnInitDialog() 
{
  CDialog::OnInitDialog();
  /*
  m_EDCorFundo.MudaCorFundo(*pCoresRef);
  m_EDCorMalha.MudaCorFundo(*(pCoresRef+1));
  m_EDCorTexto.TrocaCor(*(pCoresRef+2));
  m_EDCorTerreno.TrocaCor(*(pCoresRef+3));
  m_EDCorTerraplanagem.TrocaCor(*(pCoresRef+4));
  m_EDCorPavimento.TrocaCor(*(pCoresRef+5));
  */

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}
