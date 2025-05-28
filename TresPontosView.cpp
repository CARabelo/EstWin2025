// CurHorView.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include "typlstvw.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "carqsectipo.h" 
#include "cponto.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "csectipo.h"
#include "CEditEx.h"
#include "trespontosview.h"
#include "monologo.h"
#include <string>
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

#define IGUAL 0

extern class monologo monolog;

/////////////////////////////////////////////////////////////////////////////
// CTresPontosView

#define PRIMEIRO_CAMPO 2
#define ULTIMO_CAMPO 7

IMPLEMENT_DYNCREATE(CTresPontosView, CTypedPtrListView)

CTresPontosView::CTresPontosView() : CTypedPtrListView(CTresPontosView::IDD,ULTIMO_CAMPO,ULTIMO_CAMPO,PRIMEIRO_CAMPO), 
                                     EdUltimoCampo(ListaCampos.end(),-1),EdPrimeiroCampo(ListaCampos.end(),IDC_EDIT2),						
				                           	 SecoesTipo(CString(((CMainFrame*) AfxGetMainWnd())->PegaProjetoAtual()))
{
}

CTresPontosView::~CTresPontosView()
{
}

void CTresPontosView::DoDataExchange(CDataExchange* pDX)
{
  CTypedPtrListView::DoDataExchange(pDX,9);
  DDX_Control(pDX, IDC_EDIT2, EdPrimeiroCampo);
  DDX_Control(pDX, IDC_EDIT7, EdUltimoCampo);
}

BEGIN_MESSAGE_MAP(CTresPontosView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_COMMAND(ID_FILE_PRINT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, CTypedPtrListView::OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTypedPtrListView::OnFilePrintPreview)
  ON_MESSAGE(VUE_USRMSG_ROLAR_REGISTRO, &CTypedPtrListView::OnKillFocusUltimoCampo)
  ON_BN_CLICKED(IDC_GRAVAR,&CTypedPtrListView::OnGravar)
  ON_BN_CLICKED(IDC_LIMPAR_CAMPOS,&CTypedPtrListView::OnLimparCampos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTresPontosView diagnostics

#ifdef _DEBUG
void CTresPontosView::AssertValid() const
{
  CTypedPtrListView::AssertValid();
}

void CTresPontosView::Dump(CDumpContext& dc) const
{
  CTypedPtrListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTresPontosView message handlers

void CTresPontosView::OnAdd()
{
  CTypedPtrListView::OnAdd();
}

void CTresPontosView::OnUpdate()
{
  CTypedPtrListView::OnUpdate();
}

void CTresPontosView::OnRemove()
{
  CTypedPtrListView::OnRemove();
}

void CTresPontosView::OnRemoveAll()
{
  CTypedPtrListView::OnRemoveAll();
}

void CTresPontosView::OnSelChangeList()
{
  CTypedPtrListView::OnSelChangeList();
}

void CTresPontosView::OnInsertBefore()
{
  CTypedPtrListView::OnInsertBefore();
}

void CTresPontosView::OnInitialUpdate() 
{
  CTypedPtrListView::OnInitialUpdate();

  int Tabs[9] = {10,67,88,128,166,205,244,283,315};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(9,&Tabs[0]);

  //--- Posiciona no último.

  m_ctlList.SetCurSel(m_ctlList.GetCount()-1);
}

int CTresPontosView::Consiste(tlstwstr& LstDados)
{
  const CString& Titulo(GetDocument()->GetTitle());

  ArqCanteiros RegTemp(LstDados); 
  int Erro(RegTemp.Consiste());

  if(Erro) 
  {
    if (Titulo.Find("CANTEIROS") != -1 || Titulo.Find("FIXAS") != -1) monolog.mensagem(24,-1,monolog.ErrosCante[Erro-1]);
    else if (Titulo.Find("TIPO") != -1) monolog.mensagem(24,-1,monolog.ErrosSecTipo[Erro == 10 ? 6 : Erro-1]);
  }
  else
  {
    if (Titulo.Find("CANTEIROS") != -1 || Titulo.Find("FIXAS") != -1)
    {
      ittlstwstr ItRegAtual(RegTemp.LstCampos.begin());
      SecTipo SecaoTipo;

      SecoesTipo.BuscaSecTipo(CEstaca(ItRegAtual->c_str()),SecaoTipo);

      if(SecaoTipo.EstFinal.EstVirtual != CEstaca(ItRegAtual->c_str()).EstVirtual)
      {
        MessageBox("Não há uma seção tipo com esta estaca final");

        return 9;
      }

      ItRegAtual++;

      if(ItRegAtual != RegTemp.LstCampos.end())     //---- Tres pontos pode ter um registro vazio
      {  
        int nPar(0);

        for(ItRegAtual++ ; ItRegAtual != RegTemp.LstCampos.end() ; ItRegAtual++)
        {
          SuperCString Ponto[2] = {(ItRegAtual++)->c_str()};

          if(ItRegAtual!= RegTemp.LstCampos.end()) Ponto[1] = ItRegAtual->c_str();

          //--- Verifica se tem o par

          Ponto[0].Limpa(" ");
          Ponto[1].Limpa(" ");

          if(Ponto[0].GetLength() && Ponto[1].GetLength())
          {
            for (int C = 0 ; C < 2 ; C++)
            {
              if(SecaoTipo.PosicionaNoMaisProximo(atof(Ponto[C])) != IGUAL)   //--- o ponto tem q existir na seção tipo.
              {
                Erro = nPar * 2 + 2;
                monolog.mensagem(24,-1,monolog.ErrosCante[Erro] + ", inexistente na seção tipo.");

                return Erro;
              }
            }
          }
          else   //--- se não tem o par, mostra o erro
          {
            if(Ponto[0].GetLength() || Ponto[1].GetLength()) 
            {
              monolog.mensagem(24,-1,monolog.ErrosCante[8] + CString(nPar%2 == 0 ? " (2)" : nPar%3  == 0  ? " (3)" : " (1)"));

              return 8;
            }
          }
          nPar++;
        }
      }
    }
    else    //--- else é secao tipo 
    {
      //--- Secao tipo tem q ter pelo menos 3 pontos

      if(LstDados.size() < 7) 
      {
        monolog.mensagem(24,-1,monolog.ErrosSecTipo[4]);

        Erro = 5;
      }
      else
      {
        bool Achou(false);
        ittlstwstr ItRegAtual(RegTemp.LstCampos.begin());
        ItRegAtual++;

        //--- ponto (0,0) obrigatório

        for (ItRegAtual++; ItRegAtual != RegTemp.LstCampos.end() && !Achou; ItRegAtual++)
        {
          if (atof(ItRegAtual->c_str()) == 0.0)
          {
            ItRegAtual++;
            if (atof(ItRegAtual->c_str()) == 0.0) Achou = true;
          }
        }
        if (!Achou)
        {
          CString Mensg(monolog.ErrosSecTipo[7]);

          monolog.mensagem(24, Mensg);

          Erro = 6;
        }
      }
    }
  }

  return Erro;
}

void CTresPontosView::Serialize(CArchive& ar) 
{
}
