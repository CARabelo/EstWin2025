// CurHorView.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "ImageListBox.h"
#include "IconComboBox.h"
#include "CEditEx.h"
#include "CTiposSolosView.h"
#include "supercstring.h"
#include "carqfurossondagem.h"
#include "monologo.h"
#include <set>
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CFurosSondagemView.h"
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;

#define PRIMEIRO_CAMPO  2
#define ULTIMO_CAMPO 9

/////////////////////////////////////////////////////////////////////////////
// CTiposSolosView

IMPLEMENT_DYNCREATE(CFurosSondagemView, CTypedPtrListView)

CFurosSondagemView::CFurosSondagemView() : CTypedPtrListView(CFurosSondagemView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO),EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT), EdUltimoCampo(ListaCampos.end(),-1)
{
}

CFurosSondagemView::~CFurosSondagemView()
{
}

void CFurosSondagemView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,8);

  DDX_Control(pDX, IDC_COMTIPOSOLO1, m_ComTiposSolos1);
  DDX_Control(pDX, IDC_COMTIPOSOLO2, m_ComTiposSolos2);
  DDX_Control(pDX, IDC_COMTIPOSOLO3, m_ComTiposSolos3);
  DDX_Control(pDX, IDC_COMTIPOSOLO4, m_ComTiposSolos4);
  DDX_Control(pDX, IDC_EDIT2, EdPrimeiroCampo);
  DDX_Control(pDX, IDC_EDIT9, EdUltimoCampo);
}

BEGIN_MESSAGE_MAP(CFurosSondagemView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
  ON_MESSAGE(VUE_USRMSG_ROLAR_REGISTRO, &CFurosSondagemView::OnKillFocusUltimoCampo)
  ON_BN_CLICKED(IDC_GRAVAR,&CTypedPtrListView::OnGravar)
  ON_BN_CLICKED(IDC_LIMPAR_CAMPOS,&CTypedPtrListView::OnLimparCampos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTiposSolosView diagnostics

#ifdef _DEBUG
void CFurosSondagemView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CFurosSondagemView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTiposSolosView message handlers

void CFurosSondagemView::OnAdd()
{
  PegaCamposCombos();

  CTypedPtrListView::OnAdd();
}

void CFurosSondagemView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CFurosSondagemView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CFurosSondagemView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CFurosSondagemView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();

   PassaCamposCombos();
}

void CFurosSondagemView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CFurosSondagemView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[13] = {6,76,134,250,286,402,437,551,586,699,760,873,934};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(13,Tabs);

  CString NomeProjComp(((CMainFrame*)AfxGetMainWnd())->m_sNomeComp);

  tysetCTipoSolo SetTiposSolos;
  CArqTiposSolos ArqTiposSolos(NomeProjComp.Left(NomeProjComp.ReverseFind('.')));
  ArqTiposSolos.CriaSet(SetTiposSolos);

  for(itsetCTipoSolo it = SetTiposSolos.begin() ; it != SetTiposSolos.end() ; it++)
  {
    m_ComTiposSolos1.AddString(it->ListaCampos.begin()->c_str());
    m_ComTiposSolos2.AddString(it->ListaCampos.begin()->c_str());
    m_ComTiposSolos3.AddString(it->ListaCampos.begin()->c_str());
    m_ComTiposSolos4.AddString(it->ListaCampos.begin()->c_str());
  }

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CFurosSondagemView::Consiste(tlstwstr& ListaCampos)
{
  ArqFurosSondagem RegTemp(ListaCampos); 

  int Erro(RegTemp.Consiste());

  if(Erro != -1)
  {
    if(Erro < 13)
    {
      int Resto(Erro % 3),Dividendo((Erro - Resto) / 3);

      std::stringstream strstrComplemento;

      strstrComplemento << (Resto == 0 ? "Codigo de Solo " : " h ") << (Dividendo+1) << " - " << monolog.ErrosTiposSolos[Resto] << std::ends;

      monolog.mensagem(24,strstrComplemento.str().c_str());
    }
    else
    {
      Erro-= 9;
       monolog.mensagem(24,-1,monolog.ErrosTiposSolos[Erro]);
    }
  }

  return Erro != -1;
}

void CFurosSondagemView::Serialize(CArchive& ar) 
{
}

void CFurosSondagemView::OnBnClickedGravar()
{
  CTypedPtrListView::OnBnClickedGravar();
}

void CFurosSondagemView::PegaCamposCombos()
{
  CString CStrTemp;

  if(m_ComTiposSolos1.GetCurSel() != -1) 
  {
    m_ComTiposSolos1.GetLBText(m_ComTiposSolos1.GetCurSel(),CStrTemp);
    SetDlgItemText(IDC_EDIT2,CStrTemp.GetBuffer());
    if(m_ComTiposSolos2.GetCurSel() != -1) 
    {
      m_ComTiposSolos2.GetLBText(m_ComTiposSolos2.GetCurSel(),CStrTemp);
      SetDlgItemText(IDC_EDIT4,CStrTemp.GetBuffer());

      if(m_ComTiposSolos3.GetCurSel() != -1) 
      {
        m_ComTiposSolos3.GetLBText(m_ComTiposSolos3.GetCurSel(),CStrTemp);
        SetDlgItemText(IDC_EDIT6,CStrTemp.GetBuffer());

        if(m_ComTiposSolos4.GetCurSel() != -1) 
        {
          m_ComTiposSolos4.GetLBText(m_ComTiposSolos4.GetCurSel(),CStrTemp);
          SetDlgItemText(IDC_EDIT8,CStrTemp.GetBuffer());
        }
      }
    }
  }
}

void CFurosSondagemView::PassaCamposCombos()
{
  CString CStrTemp;
  
  GetDlgItemText(IDC_EDIT2,CStrTemp);
  m_ComTiposSolos1.SetCurSel(m_ComTiposSolos1.FindString(-1,CStrTemp.GetBuffer()));
  GetDlgItemText(IDC_EDIT4,CStrTemp);
  m_ComTiposSolos2.SetCurSel(m_ComTiposSolos2.FindString(-1,CStrTemp.GetBuffer()));
  GetDlgItemText(IDC_EDIT6,CStrTemp);
  m_ComTiposSolos3.SetCurSel(m_ComTiposSolos3.FindString(-1,CStrTemp.GetBuffer()));
  GetDlgItemText(IDC_EDIT8,CStrTemp);
  m_ComTiposSolos4.SetCurSel(m_ComTiposSolos4.FindString(-1,CStrTemp.GetBuffer()));
}

LRESULT CFurosSondagemView::OnKillFocusUltimoCampo(WPARAM WP, LPARAM LP)
{
  PegaCamposCombos();

  CTypedPtrListView::OnKillFocusUltimoCampo(0,LP);
  CTypedPtrListView::OnKillFocusUltimoCampo(0,LP);

  PassaCamposCombos();
 
  return true;
}

