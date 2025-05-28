// CalcVolumes.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "estaca.h"
#include "perfil.h"
#include "supercstring.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "clistaperfis.h"
#include "clistasecoes.h"
#include "carqhpavimento.h"
#include "chpavimento.h"
#include <set>
#include "CTipoSolos.h"
#include "CArqTiposSolos.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include "CArqFurosSondagem.h"
#include <algorithm>
#include "CPerfilGeologico.h"
#include "CalcVolumes.h"
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

#define LER 0
#define GRAVAR 1

/////////////////////////////////////////////////////////////////////////////
// CalcVolumes dialog

CalcVolumes::CalcVolumes(CString& NomeProj,CWnd* pParent)	: CDialog(CalcVolumes::IDD, pParent),NomeProjeto(NomeProj),Secoes(NomeProj),
             Empolamentos(NomeProj,CString(".emp")),HLimpeza(NomeProj,CString(".lim")),pDialogo(this),
             PerfilGeologico(std::string(NomeProjeto.GetBuffer()))
{
  //{{AFX_DATA_INIT(CalcVolumes)
  // NOTE: the ClassWizard will add member initialization here
  //}}AFX_DATA_INIT
}

BOOL CalcVolumes::Create(CWnd* pWnd,CDialog* ppDialogo)     //--- para não modal.
{
  pDialogo = ppDialogo;
  return CDialog::Create(IDD,pWnd);
}

void CalcVolumes::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CalcVolumes)
  DDX_Control(pDX, IDC_OK2, m_butOk);
  DDX_Control(pDX, IDC_PAUSA2, m_butPausa);
  DDX_Control(pDX, IDCANCEL3, m_butCancel);
  DDX_Control(pDX, IDC_INICIARCALC, m_butIniciar);
  DDX_Control(pDX, IDC_PROGRESSO, m_Progresso);
  //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CalcVolumes, CDialog)
  //{{AFX_MSG_MAP(CalcVolumes)
  ON_BN_CLICKED(IDC_OK2, OnOk2)
  ON_BN_CLICKED(IDCANCEL3, OnCancel3)
  //}}AFX_MSG_MAP
  ON_BN_CLICKED(IDC_INICIARCALC, OnBnClickedIniciarcalc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CalcVolumes message handlers

BOOL CalcVolumes::OnInitDialog() 
{
  CDialog::OnInitDialog();

  CMainFrame *MainFrame((CMainFrame*)AfxGetMainWnd());
  CString NomeArquivo(NomeProjeto + ".tca"); 

  pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Abrindo o arquivo do terreno atual.");

  TerrenoPrimitivo.Constroi(NomeArquivo);

  CArqTiposSolos ArqTiposSolos(NomeProjeto);
  ArqTiposSolos.CriaSet(SetTiposSolos);
  PerfilGeologico.Serialize(NomeProjeto.GetBuffer(),LER);

  pDialogo->SetDlgItemText(IDC_EXECUTANDO,"");

  if (pDialogo == this)
  {
    m_butOk.EnableWindow(false);
    m_butPausa.EnableWindow(false);
  }

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION: OCX Property Pages should return FALSE
}

bool CalcVolumes::MontaSecoes(int Tipo)
{
  //--- Prepara a barra de progresso.

  pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Preparando as seções para cálculo...");

  //--- Monta as seções para cálculo incluindo os perfís de projeto e de terreno.
  //--- Caso haja apenas um perfil na secão ele é deletado porque não será usado.

  POSITION AtualTerrPrim(TerrenoPrimitivo.GetHeadPosition()),
           AtualProje(Secoes.GetHeadPosition());

  if (!AtualTerrPrim || !AtualProje) return false;

  while(AtualTerrPrim && AtualProje)
  {
    CSecao& SecaoAtual(Secoes.GetAt(AtualProje));
    Perfil& TerrPrimitivo(TerrenoPrimitivo.GetAt(AtualTerrPrim));

    //--- Se os perfís são da mesma estaca inclui uma secao na lista de secoes
    //--- Caso contrário incrementa o menor

    if (SecaoAtual.Projeto.GetCount() > 2 && SecaoAtual.Estaca.EstVirtual == TerrPrimitivo.Estaca.EstVirtual)
    {
      //--- Achou uma seção de terreno numa estaca do projeto, monta a seção e pega as próximas

      SecaoAtual.Terreno = TerrPrimitivo;
      if(Tipo == TERRENO_PRIMITIVO) SecaoAtual.LimpaTerreno(HLimpeza.BuscaH(SecaoAtual.Terreno.Estaca));
      Secoes.GetNext(AtualProje);

      //--- O perfil de terreno foi passado para a seção, remove-o ( Na próxima Versão, passar por ponteiro ...)

      POSITION Temp(AtualTerrPrim);
      TerrenoPrimitivo.GetNext(AtualTerrPrim);
      TerrenoPrimitivo.RemoveAt(Temp);
    }
    else
    {
      if (SecaoAtual.Projeto.GetCount() > 2 && SecaoAtual.Projeto.Estaca.EstVirtual > TerrPrimitivo.Estaca.EstVirtual)
        TerrPrimitivo.GetNext(AtualTerrPrim);
      else
      {
        //--- Remove a seção, ela não tem terreno ou projeto

        POSITION Temp(AtualProje);
        Secoes.GetNext(AtualProje);
      }
    }
  }

  return Secoes.GetCount() > 2; 
}

void CalcVolumes::OnIniciarcalc() 
{
  if (pDialogo == this) 
  {
    m_butPausa.EnableWindow(true);
    m_butIniciar.EnableWindow(false);
  }

  if(Secoes.GetCount() > 1 && MontaSecoes(TERRENO_PRIMITIVO))   //--- Tem q ter no minimo 2 seções
  {  
    CalculaOffSets();
    CalculaAreas();
    CalculaVolumes();
    CalculaCG();

    Secoes.TotalizaVolumes();

    //--- O Terreno foi aparado nos off_sets, portanto não pode ser gravado. grava só o projeto 

    Secoes.Serialize(CListaSecoes::GRAVAR_SO_PROJETO);  

    pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Calculando os volumes dos perfís geológicos...");

    //---  Calcula os volumes dos perfís geológicos --------------------------------------------------------------

    MontaSetPorTipoSolo();

    for(itsetCTipoSolo itTipoSolo = SetTiposSolos.begin() ; itTipoSolo != SetTiposSolos.end() ; ++itTipoSolo)
    {
      std::string strTipoSolo(*itTipoSolo->ListaCampos.begin());

      //--- Monta um terreno primitivo com o tipo de solo atual

      TerrenoPrimitivo.Limpa();
      MontaTerrenoPerfilGeologico(*itTipoSolo->ListaCampos.begin());

      if(Secoes.GetCount() > 1 && MontaSecoes(PERFIL_ECOLOGICO))   //--- Tem q ter no minimo 2 seções
      {
       CalculaAreas(*itTipoSolo->ListaCampos.begin());
      }

      //--- passa as áreas e os volumes para os perfis geologicos

      for(POSITION Atual = Secoes.GetHeadPosition() ; Atual != NULL ; Secoes.GetNext(Atual)) 
      {
        CSecao SecaoAtual(Secoes.GetAt(Atual));

        itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos(strTipoSolo,SecaoAtual.Estaca.EstVirtual)));
        
		    if(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() && itPerfGeo->PegaEstaca() == SecaoAtual.Estaca.EstVirtual && itPerfGeo->PegaNomeTipoSolo() == strTipoSolo)
        {
          CDadosPerfisGeologicos* pDadosPerfEcologico(const_cast<CDadosPerfisGeologicos*>(&*itPerfGeo));
         
          pDadosPerfEcologico->PegaArea(CORTE) = SecaoAtual.AreaTotal[CORTE];
          pDadosPerfEcologico->PegaArea(ATERRO) = SecaoAtual.AreaTotal[ATERRO];
          pDadosPerfEcologico->PegaVolume(CORTE) = SecaoAtual.Volume[CORTE];
          pDadosPerfEcologico->PegaVolume(ATERRO) = SecaoAtual.Volume[ATERRO];
        }
      }
    }

    AcertaAreasPerfGeol();   //--- É Preciso acertar as áreas pois elas estão acumuladas.

    PerfilGeologico.Serialize(NomeProjeto.GetBuffer(),GRAVAR);

    if(pDialogo == this)
    {
      m_butOk.EnableWindow(true);
      m_butCancel.EnableWindow(false);
      m_butPausa.EnableWindow(false);
      m_butIniciar.EnableWindow(false);
    }
  }
  else  
  {
    pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Não há terreno ou projeto para executar os cálculos de volumes.");
    m_butIniciar.EnableWindow(false);
    m_butCancel.EnableWindow(false);
    m_butPausa.EnableWindow(false);

    return;
  }

  pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Ok, os cálculos foram executados com êxito...");
}

void CalcVolumes::CalculaOffSets()
{
  pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Calculando os off-sets...");
  m_Progresso.SetRange(0,Secoes.GetCount()+1);
  m_Progresso.SetStep(1);

  POSITION Atual(Secoes.GetHeadPosition());

  while(Atual) 
  {
    CSecao &SecaoAtual(Secoes.GetAt(Atual)); 
    m_Progresso.StepIt();

    //--- Se for Medição, calcula os Offsets da seção, se não achar um dos dois, exclui a seção, ela não interessa,
    //--- No excluir a lista é automaticamente incrementada, caso contrario incrementa a lista.
    //--- Se não for medição retira as seções que não tiverem os 2 off-sets (Eles foram calculados no cálculo da nota)

    if (NomeProjeto.Find("med") != -1 || NomeProjeto.Find("MED") != -1)  //--- Medicao tem med no nome
    {
      if (!SecaoAtual.CalculaOffSets(Secoes.TaludesCorte,Secoes.TaludesAterro,TRUE)) 
      {
        POSITION Temp(Atual);
        Secoes.GetNext(Atual);
        Secoes.RemoveAt(Temp);
      }
      else Secoes.GetNext(Atual);
    }
    else
    {
      //--- Não é medição, verifica se a seção tem os 2 offsets

      if (SecaoAtual.Projeto.GetCount() > 0 )
      {
        Ponto& OffSetEsq = SecaoAtual.Projeto.GetHead(),& OffSetDir = SecaoAtual.Projeto.GetTail();

        if (OffSetEsq.Descricao != "OFFSEE" || OffSetDir.Descricao != "OFFSED")
        {
          Secoes.GetNext(Atual);
        }
        else Secoes.GetNext(Atual);
      }
      else Secoes.GetNext(Atual);
    }
  }	
}

void CalcVolumes::CalculaAreas(const std::string& TipoSolo)
{
  m_Progresso.SetPos(0);
  pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Calculando as áreas...");

  //--- Para todas as secoes calcule os volumes. 

  POSITION Atual(Secoes.GetHeadPosition());
  if (!Atual) return;

  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->SetRange(0,Secoes.GetCount()+1);

  while(Atual) 
  {
    ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->StepIt();

    CSecao& SecaoAtual(Secoes.GetAt(Atual)); 

    //--- Se não for perfil geológico calcula a área, se for é preciso primeiro ajustar os off-sets para os da seção no perfil geológico

    if(TipoSolo.size() != 0)           //--- É perfil geológico?
    {
       CSecao& SecaoAtualPerfGeo(Secoes.GetAt(Atual)); 

       if(PegaOffSetsPerfGeo(SecaoAtualPerfGeo.Estaca,TipoSolo,SecaoAtualPerfGeo.OffSets[0],SecaoAtualPerfGeo.OffSets[1]) == 2)      //--- Se calculou os 2 off sets
       {                                                                                                                             //--- Nos perfís geológicos o primeiro e o último ponto de passagem podem ser os off sets
         //--- Acerta o perfil do projeto nos novos off-sets do perfil geológico
         //--- Os primeiro  ultimos pontos dos perfis têm que ser os off sets

         SecaoAtualPerfGeo.Projeto.InsereOrdenada(SecaoAtualPerfGeo.OffSets[0],false);    
         SecaoAtualPerfGeo.Projeto.InsereOrdenada(SecaoAtualPerfGeo.OffSets[1],false);
         SecaoAtualPerfGeo.Projeto.TrimLeft(SecaoAtualPerfGeo.OffSets[0].x-1E-5);
         SecaoAtualPerfGeo.Projeto.TrimRight(SecaoAtualPerfGeo.OffSets[1].x+1E-5);

         SecaoAtualPerfGeo.CalculaArea();
       }
	     else SecaoAtualPerfGeo.AreaTotal[0] =  SecaoAtualPerfGeo.AreaTotal[1] = 0.0;   
       

     }
     else SecaoAtual.CalculaArea();

    Secoes.GetNext(Atual);
  }						
}							

void CalcVolumes::CalculaVolumes()
{
  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->SetPos(0);
   pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Calculando os Volumes...");

  //--- Para todas as secoes calcule os volumes. 

  POSITION Atual(Secoes.GetHeadPosition());
  if (!Atual) return;

  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->SetRange(0,Secoes.GetCount());
  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->SetStep(1);

  CSecao* SecaoAnterior(&Secoes.GetAt(Atual));
  Secoes.GetNext(Atual);
  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->StepIt();

  while(Atual) 
  {
    ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->StepIt();
    CSecao &SecaoAtual(Secoes.GetAt(Atual));
    SecaoAtual.LimpaVolumes();
    if(SecaoAtual.CalculaVolumes(*SecaoAnterior,Empolamentos.BuscaH(SecaoAtual.Terreno.Estaca,NULL))) {} ;	
    SecaoAnterior = &SecaoAtual;
    Secoes.GetNext(Atual);
  }	
}

void CalcVolumes::CalculaCG()
{
  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->SetPos(0);
  pDialogo->SetDlgItemText(IDC_EXECUTANDO,"Calculando os Centros de Gravidade...");

  //--- Para todas as secoes calcule os CGs. 

  POSITION Atual(Secoes.GetHeadPosition());
  if (!Atual) return;

  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->SetRange(0,Secoes.GetCount());
  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->SetStep(1);

  CSecao* SecaoAnterior(&Secoes.GetAt(Atual));
  Secoes.GetNext(Atual);
  ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->StepIt();

  double SomaNum[2] = { 0.0,0.0 }, SomaDen[2] = { 0.0,0.0 }, CgCTAnterior[2] = { SecaoAnterior->Estaca.EstVirtual,SecaoAnterior->Estaca.EstVirtual };

  while(Atual) 
  {
    ((CProgressCtrl*)(pDialogo->GetDlgItem(IDC_PROGRESSO)))->StepIt();
    CSecao* SecaoAtual(&Secoes.GetAt(Atual));

    //---------------------------------------------------------------------------------------

    double SemiDist((SecaoAtual->Estaca.EstVirtual - SecaoAnterior->Estaca.EstVirtual) / 2.0);

    for (int Tipo = CORTE ; Tipo <= ATERRO ; Tipo++)
    {/*
     SecaoAtual->CGs[Tipo] = 0.0;
     double Denom(SecaoAtual->Volume[Tipo] - SecaoAnterior->Volume[Tipo] / 2.0 * SemiDist);

     if(Denom != 0.0)
     {
     SomaNum[Tipo] += (SecaoAtual->Volume[Tipo] - SecaoAnterior->Volume[Tipo]) / ( 1.0 * (SecaoAtual->Volume[Tipo] + SecaoAnterior->Volume[Tipo])) * SecaoAtual->Estaca.EstVirtual  * Denom;
     SomaDen[Tipo] += Denom;
     SecaoAtual->CGs[Tipo] = SomaNum[Tipo] / SomaDen[Tipo];
     }

     if(SecaoAtual->Volume[Tipo] == 0.0)  SomaNum[Tipo] = 0.0;
     */

      SecaoAtual->CGs[Tipo] = INFINITO;

      if(SecaoAtual->Volume[Tipo] != 0.0)
      {
        SomaNum[Tipo] += (SecaoAtual->Volume[Tipo] * SecaoAtual->Estaca.EstVirtual);
        SomaDen[Tipo] += SecaoAtual->Volume[Tipo];
        SecaoAtual->CGs[Tipo] = SomaNum[Tipo] / SomaDen[Tipo];// +CgCTAnterior[Tipo];
      }
      else
      {
        SomaNum[Tipo] = 0.0;
        CgCTAnterior[Tipo] = SecaoAtual->Estaca.EstVirtual;
      }
    }

    //---------------------------------------------------------------------------------------

    Secoes.GetNext(Atual);
  }	
}

void CalcVolumes::OnBnClickedIniciarcalc()
{ 
  OnIniciarcalc(); 
}

void CalcVolumes::OnOk2() 
{
  CDialog::OnOK();
}

void CalcVolumes::OnCancel3() 
{
  EndDialog(true);	
}

void CalcVolumes::MontaTerrenoPerfilGeologico(const std::string& TipoSolo)
{
  itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().begin());    

  while(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() &&  itPerfGeo->PegaNomeTipoSolo().compare(TipoSolo) != 0) ++itPerfGeo;    //--- Pega a primeira ocorrencia na ordem de estacas.

  if(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end())          //-- Se achou pega as secoes pela ordem de tipo de solo
  {
    itsetitCDadosPerfGeologicos itPerfGeoTipoSolo(SetItDadosPerfisPorTipo.find(itPerfGeo));

    while(itPerfGeoTipoSolo != SetItDadosPerfisPorTipo.end() && (*itPerfGeoTipoSolo)->PegaNomeTipoSolo() == TipoSolo)
    {
      TerrenoPrimitivo.AddTail(*(const_cast<Perfil*>(&(*itPerfGeoTipoSolo)->PegaPerfilGeologico())));
      ++itPerfGeoTipoSolo;
    }
  }
}

int CalcVolumes::PegaOffSetsPerfGeo(const CEstaca& Estaca,const std::string& TipoSolo,Ponto& OffSetEsq,Ponto& OffSetDir)
{
  if(PerfilGeologico.PegaSetSecaoPerfGeo().size() == 0) return false;

  OffSetEsq.x =  OffSetDir.x = INFINITO;
  int Resp(0);

 //--- Neste ponto não sabemos o deltah então não da pra pegar o igual.

  itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().upper_bound(CDadosPerfisGeologicos(TipoSolo,Estaca.EstVirtual)));

  if(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() && itPerfGeo->PegaEstaca() == Estaca.EstVirtual && itPerfGeo->PegaNomeTipoSolo() == TipoSolo)
  {
    OffSetEsq = itPerfGeo->PegaOffSet(ESQUERDO);
    OffSetDir = itPerfGeo->PegaOffSet(DIREITO);
  }

  if(OffSetEsq.x != INFINITO) Resp++;
  if(OffSetDir.x != INFINITO) Resp++;

  return Resp;
}

struct ltporAreaInv
{
    bool operator()(const CDadosPerfisGeologicos* pPerf1,const CDadosPerfisGeologicos* pPerf2) const 
    { 
       return pPerf1->PegaValorArea(0) >  pPerf2->PegaValorArea(0) ? true :
              pPerf1->PegaValorArea(0) <  pPerf2->PegaValorArea(0) ? false :
              pPerf1->PegaNomeTipoSolo().compare(pPerf2->PegaNomeTipoSolo()) == -1 ? true : false;
    }
};

typedef std::set<const CDadosPerfisGeologicos*,ltporAreaInv> setCDadosPerfGeologicosArea;
typedef setCDadosPerfGeologicosArea::iterator itsetCDadosPerfGeologicosArea;

void CalcVolumes::AcertaAreasPerfGeol()             //--- Acerta as áreas dos perfís geológicos pois elas estão acumuladas. 
{
  setCDadosPerfGeologicosArea SetPerfilEcolArea; 

  int contador(0);

  itsetCDadosPerfGeologicos itPerfGeo(PerfilGeologico.PegaSetSecaoPerfGeo().begin());

  while(itPerfGeo !=PerfilGeologico.PegaSetSecaoPerfGeo().end())
  {
    double EstacaAtual(itPerfGeo->PegaEstaca());
    SetPerfilEcolArea.clear();

    while(itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() && itPerfGeo->PegaEstaca() == EstacaAtual)
    {
      SetPerfilEcolArea.insert(&*itPerfGeo);
      ++itPerfGeo;
    }

    itsetCDadosPerfGeologicosArea itAtual(SetPerfilEcolArea.begin());
    double AreaAcum((*itAtual)->PegaValorArea(0));

    ++itAtual;

    while(itAtual != SetPerfilEcolArea.end())
    {
      double AreaAtual((*itAtual)->PegaValorArea(0));
      const_cast<CDadosPerfisGeologicos*>(*itAtual)->PegaArea(0) -= AreaAcum;
      AreaAcum = AreaAtual;

      ++itAtual;
    }
  }
}