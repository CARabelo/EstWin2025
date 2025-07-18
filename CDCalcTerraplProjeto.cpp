// CDCalcTerraplProjeto.cpp : arquivo de implementação
//
#include "stdafx.h"
#include "Est.h"
#include <list>
#include <map>
#include "CPonto.h"
#include <tuple>
#include "CAlinhamento.h"
#include "CAlinhamentos.h"
#include <set>
#include "CSegmento.h"
#include "CDeltaSuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CLpontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "Estaca.h"
#include "CEquipDrenagem.h"
#include "Superficie.h"
#include "CArqHPavimento.h"
#include "CHPavimento.h"
#include "CCalcTerraplProjeto.h"
#include "Perfil.h"
#include "CArqSectipo.h"
#include "CSecTipo.h"
#include "igualdades.h"
#include "Mat.h"
#include "Projeto.h"
#include "CArqCanteiros.h"
#include "CCanteiros.h"
#include "CArqCurHorizontais.h"
#include "CCurhorizontal.h"
#include "Ccurhorizontais.h"
#include "CSecaoAvulsa.h"
#include "CSecoesAvulsas.h"
#include "CArqAlargamentos.h"
#include "CArqAciTopograficos.h"
#include "CAlargamentos.h"
#include "CDCalcTerraplProjeto.h"
#include "afxdialogex.h"
#include <sstream>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "Mainfrm.h"

#define  ESQUERDO 0
#define  DIREITO 1
#define NENHUM_LADO 2
#define GRAVAR 2

extern class Matem Mat;

// caixa de diálogo de CDCalcTerraplProjeto
IMPLEMENT_DYNAMIC(CDCalcTerraplProjeto, CDialogEx)

CDCalcTerraplProjeto::CDCalcTerraplProjeto(CWnd* pParent, const std::string& NomeProj, std::string& ProjBase) : NomeProjeto(NomeProj), CDialogEx(IDD_CALCTERRPROJETO, pParent), NomeProjetoBase(ProjBase),
                      m_uiDensTracos(4)
{
}
CDCalcTerraplProjeto::~CDCalcTerraplProjeto()
{
}
void CDCalcTerraplProjeto::DoDataExchange(CDataExchange* pDX)
{
  CDialogEx::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_PROGRESSO, m_Progresso);
  DDX_Control(pDX, IDC_STATEXTO, m_Texto);
  DDX_Text(pDX, IDC_EDIDESNSTRACOSSIMBOL, m_uiDensTracos);
  DDV_MinMaxUInt(pDX, m_uiDensTracos,1,10);
}
BEGIN_MESSAGE_MAP(CDCalcTerraplProjeto, CDialogEx)
  ON_BN_CLICKED(IDOK, &CDCalcTerraplProjeto::OnBnClickedOk)
  ON_BN_CLICKED(IDC_BUTCALCTERRAPLPROJ, &CDCalcTerraplProjeto::OnBnClickedButcalcterraplproj)
  ON_BN_CLICKED(IDC_BUTLOG, &CDCalcTerraplProjeto::OnBnClickedButlog)
END_MESSAGE_MAP()
void CDCalcTerraplProjeto::OnBnClickedOk()
{
  CDialogEx::OnOK();
}

void CDCalcTerraplProjeto::OnBnClickedButcalcterraplproj()
{
  int TipoProjeto(((CMainFrame*)AfxGetMainWnd())->TipoSuperficie);

  CCalcTerraplProjeto CalcTerrProjeto(NomeProjeto, m_Progresso, m_Texto, NomeProjetoBase,TipoProjeto);

  std::string NomeArquivo(NomeProjeto.substr(0,NomeProjeto.find_last_of('\\')));
  NomeArquivo = NomeArquivo.substr(0,NomeArquivo.find_last_of('\\'));
  std::string NomeProjeto(NomeArquivo.substr(NomeArquivo.find_last_of('\\')));
  NomeArquivo += NomeProjeto + ".ini";

  //--- Verifica se o projeto é georeferenciado
  CString Default;
  char Buffer[1024] = { 0 };

  Default.Format("%lf,%lf,%lf,%lf,%lf,%i,%lf,%lf,%lf,%s",0.0,0.0,0.0,0.0,1.0,0,1.0,0.0,0.0, "");
  ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosMapa"), Default, Buffer, 511, NomeArquivo.c_str());
  std::stringstream strstrBuffer(Buffer);
  char Virgula(' ');
  double Lixo(INFINITO);
  int ProjetoGeoreferenciado(-1);

  strstrBuffer >> Lixo >>Virgula  >> Lixo >> Virgula >> Lixo >> Virgula >> Lixo >> Virgula >> Lixo >> Virgula >> ProjetoGeoreferenciado;

  UpdateData(TRUE);

  if(m_uiDensTracos < 1 || m_uiDensTracos > 10) return;

  if (CalcTerrProjeto.CalculaTerraplProjeto(ProjetoGeoreferenciado, m_uiDensTracos, TipoProjeto) != 0)
  {
    m_Progresso.SetPos(1);
//    m_Texto.SetWindowTextA("Houve erro, o projeto de terraplanagem NÃO foi calculado com êxito,\r\n verifique o arquivo de log");
  }
  else
  {
   //AjustaOffSTerraplanagem();//--- EM DESENVOLVIMENTO #VERFICAR

    m_Progresso.SetPos(10);
    m_Texto.SetWindowTextA("Cálculos Ok, o projeto de terraplanagem foi calculado com êxito");
  }
  GetDlgItem(IDC_BUTLOG)->EnableWindow(true);
}

void CDCalcTerraplProjeto::OnBnClickedButlog()
{
  ((CMainFrame*)AfxGetMainWnd())->asdfg = IDD_REL_LOG_CALCTERRAP;
  ((CEstApp*)AfxGetApp())->AbreArqCriaDoc("_ProjTerraplanagem.rel", false, true);
}

BOOL CDCalcTerraplProjeto::OnInitDialog()
{
  CDialogEx::OnInitDialog();
  GetDlgItem(IDC_BUTLOG)->EnableWindow(false);
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: as páginas de propriedade OCX devem retornar FALSE
}
//---=========================================================================================================

#include "CTrecho.h"
#include "CArqTrechos.h"
#include "CTrechos.h"
#include "CParamSetInterBordos.h"

int CDCalcTerraplProjeto::AjustaOffSTerraplanagem()
{
  //--- EXISTEM MUITOS CASOS A SE ESTUDAR PRINCIPALMENTE  AS CNS DE TERRENO SÃO FECHADAS
  //--- ESTUDAR MAIS ESTES CASOS

  return 0;

  //--- Esta classe contem os dados da curva de nivel de terreno e de terraplanagem envolvidos na interseção
  //--- Usada no Desenhos geométrico, Ajuste curvas de nivel de terraplanagem com o terreno

  CTrechos Trechos(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()).GetBuffer(), true);
  CSuperficie SuperficieTP(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()));         //--- Projeto de Terraplanagem
  CSuperficie SuperficieTN(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoBase()));          //--- Superfície com o terreno natural ==>>> Não proxima versão colocar em 1 só. Não da pois tem 1 proj.terrp. para cada rua/acesso

  SuperficieTP.CalculaArea();
  SuperficieTN.CalculaArea();
  SuperficieTN.LimpaBalizasCNs();

  //--- O sentido do giro das fronteiras tem que ser para o mesmo lado, o da Terraplanagem sempre é para a direita.
  //--- Se o sentido do giro da fronteira da superficie estiver para a esquerda, inverte o giro da fronteira da terraplanagem.

  bool InverteuFronteiraTerrap(false);

  if (SuperficieTN.PegaArea() < 0.0)
  {
    SuperficieTP.InverteFronteira();
    SuperficieTP.InverteCNs();
    InverteuFronteiraTerrap = true;
  }

  LDeltaSup LFronteiraTerrap;

  SuperficieTP.PegaListaFronteira(LFronteiraTerrap, 1);

  if (LFronteiraTerrap.size() < 3)  return 1;   //--- 1-> Erro: Não há fronteira

  ItLDeltaSup itBordoAnterior(LFronteiraTerrap.begin()), itBordoAtual(itBordoAnterior);
  tySetCParamSetCN SetInterCNBordos;
  Ponto Intersecao;
  Intersecao.Baliza = true;       //--- Usado para indicar na lista de CN que este ponto é uma interseção com a linha de off-sets
  Intersecao.Baliza2 = false;     //--- Usado para indicar na lista de CN que este ponto de interseção ja foi usado

  //--- Calcula as interseções das CN de terreno com a linha de offsets e as coloca em um set (SetInterCNBordos)

  for (itBordoAtual++; itBordoAtual != LFronteiraTerrap.end(); itBordoAnterior++, ++itBordoAtual)
  {
    //--- As 2 retas que ligam o lado esquerdo ao lado direito não são consideradas (A descricao dos 2 pontos tem que ser iguais, OFFSE ou OFFSD)

    //if (itBordoAnterior->PegaPCentral().Descricao == itBordoAtual->PegaPCentral().Descricao)
    {
      if (itBordoAnterior->PegaPCentral() != itBordoAtual->PegaPCentral())  //--- O bordo é uma lista fechada (último ponto é igual ao primeiro) neste caso não pega a CN
      {
        for (ItLLPontos itLLCN = SuperficieTN.LPontosCN.begin(); itLLCN != SuperficieTN.LPontosCN.end(); itLLCN++)     //--- Para todas as CN de terreno
        {
          register ItLPontos itLCNAnterior(itLLCN->begin()), itLCNAtual(itLCNAnterior);

          for (itLCNAtual++; itLCNAtual != itLLCN->end(); itLCNAnterior++, itLCNAtual++)    //--- Para todos os pontos da CN de terreno
          {
            if (CLPontos::CalculaIntersecao(itBordoAnterior->PegaPCentral(), itBordoAtual->PegaPCentral(), *itLCNAnterior, *itLCNAtual, Intersecao))
            {
              //--- Set com as interseções das CN de terreno com os offsets

              SetInterCNBordos.emplace(*itLCNAtual, itBordoAtual, itLLCN, itLCNAtual, Intersecao);

              //--- É preciso inserir a interseção na curva de nível, será útil ao ligar para sabermos que o ponto da CN pertence a um bordo
              //--- Para isso, setamos um dos flags do ponto indicando que ele faz parte do offset também.
              //--- O flag do bordo (Intersecao.baliza) vai true indicando que este ponto
              //--- Da CN pertence ao bordo de terraplanagem.

              //Intersecao = { x = 246793.47911110049 y = 4310001.7255864609 z = 1936.0000000000000 ... }

              Intersecao.z = itLCNAnterior->z;
              itLCNAtual = itLLCN->emplace(itLCNAtual, Intersecao);
              itLCNAtual++;
              itLCNAnterior++;
            }
          }
        }
      }
    }
  }

  //--- A partir do ponto da CN de terraplanagem no Offset procura pela interseção da curva de nivel de terreno na mesma cota a um raio de 20m
  //--- (ou 40 na segunda tentativa) e seleciona o mais proximo. OBS: antes, verifica se o lado do ponto está no lado correto da curva de nível:
  //--- esquerdo para o offset esquerdo e direito para o offset direito

  CSuperficie& SuperfTerrap(Trechos.PegaTrechoAtual()->second.SuperficieTerr);
  SuperfTerrap.LimpaBalizasCNs();
  int ContaCurvas = 0;
  bool Final(false);
  tysetItLLPontos SetCNTNUsadas;

  for (ItLLPontos itLLCNTerr = SuperfTerrap.LPontosCN.begin(); itLLCNTerr != SuperfTerrap.LPontosCN.end(); itLLCNTerr++)
  {
   // if (ContaCurvas++ != 1) continue;

    if (*itLLCNTerr->begin() == *itLLCNTerr->rbegin()) continue;  //--- Curva de nivel fechada, não há o que ligar
    if (itLLCNTerr->size() < 2) continue;  //--- ???

    for (int Lado = ESQUERDO; Lado <= DIREITO; Lado++)
    {
      if (Lado == ESQUERDO) continue;
      
      int Tentativa(1);
      bool Achou(false), Fechou(false);

      double Raio(50.0 * Tentativa);
      
      while (Achou == false && Tentativa++ < 2)
      {
        ItLPontos itLCNTerrapAtual(Lado == ESQUERDO ? itLLCNTerr->begin() : --(itLLCNTerr->end()));

        if (itLCNTerrapAtual->Baliza2 == true) continue;   //---- Este ponto de offset já foi usado, provavelmente por uma CN fechada

        tySetCParamSetCN SetTemp;

        ittySetCParamSetCN itSIBE(SetInterCNBordos.upper_bound(Ponto(itLCNTerrapAtual->x - Raio, itLCNTerrapAtual->y, itLCNTerrapAtual->z))),
                           itSIBD(SetInterCNBordos.upper_bound(Ponto(itLCNTerrapAtual->x + Raio, itLCNTerrapAtual->y, itLCNTerrapAtual->z))),
                           itSIBMaisProximo(SetInterCNBordos.end());

        if (itSIBE == SetInterCNBordos.end()) continue;   //--- não há nenhum ponto no raio de de 20 (ou 40) m

        //--- Se a CN for circular pode ter pego as 2 pontas, separa a mais proxima

        double MenorDist(INFINITO);

        for (ittySetCParamSetCN itSIBAtual = itSIBE; itSIBAtual != itSIBD; ++itSIBAtual)
        {
          double Dist(hypot(itLCNTerrapAtual->x - itSIBAtual->Intersecao.x, itLCNTerrapAtual->y - itSIBAtual->Intersecao.y));

          if (itSIBAtual->PontoCN.z == itLCNTerrapAtual->z)
          {
            if (LadoOffs(itSIBAtual->itBordo->PegaPCentral()) == Lado)
            {
              if (Dist < MenorDist)
              {
                //  itSIBMaisProximo = itSIBE;  //--- Não consegui fazer esta atribuição, porisso o SetTemp;
                SetTemp.clear();
                SetTemp.emplace(*itSIBAtual);
                MenorDist = Dist;

              }
            }
          }
        }

        if (SetTemp.size() == 0) continue; //--- Não achou pontos dentro do raio de ação

        itSIBMaisProximo = SetInterCNBordos.find(*SetTemp.begin());

        //--- Separa os pontos de interesse, que estão a menos q <Raio> metros

        tySetInterBordosDouble SetPontosSelecionados;
        {
          if (itSIBMaisProximo->PontoCN.z == itLCNTerrapAtual->z)
          {
            ItLDeltaSup ItBorAnte((const_cast<CParamSetCN*>(&*itSIBMaisProximo))->itBordo);
            --ItBorAnte;

            //--- Varre todos pontos da CN e separa os que estiverem a uma distancia < que raio e no lado correto

            for (ItLPontos ItPCNAtual = itSIBMaisProximo->itLLCN->begin(); ItPCNAtual != itSIBMaisProximo->itLLCN->end(); ItPCNAtual++)
            {
              if (ItPCNAtual->Baliza2 == true) continue;          //--- Este Ponto já foi usado

              if (itSIBMaisProximo->Intersecao.Baliza2 == false)       //--- Verifica se o ponto ainda não foi usado
              {
                double Deltax(ItPCNAtual->x - itSIBMaisProximo->Intersecao.x), Deltay(ItPCNAtual->y - itSIBMaisProximo->Intersecao.y);

                if (fabs(Deltax) < Raio && fabs(Deltay) < Raio)
                {
                  double Dist(hypot(Deltax, Deltay));

                  //--- O ponto do bordo foi incluido na CN porisso é preciso verificar se dist > 0.005
                  //--- Os pontos já estão selecionados por lado

                  if (Dist < Raio && Dist > 0.005)
                  {
                    SetPontosSelecionados.emplace(itSIBMaisProximo->itBordo->PegaPCentral(), ItBorAnte->PCentral, ItPCNAtual, itSIBMaisProximo->itLLCN, Dist);
                  }
                }
              }
            }
          }
        }
        MenorDist = INFINITO;
        ittySetInterBordosDouble itPontoMaisPerto(SetPontosSelecionados.end());
        for (ittySetInterBordosDouble itSPonSel = SetPontosSelecionados.begin(); itSPonSel != SetPontosSelecionados.end(); ++itSPonSel)
        {
          //--- Pega o ponto mais proximo
          //--- Verifica o lado em que o pontos esta em relação a linha de offsets
          //--- O cálculo da área esta girando para a esquerda.
          //--- O ponto tem que estar sempre a direita do lado atual

          double Area((itSPonSel->PontoBordo.y + itSPonSel->PontoBordoAnt.y) * (itSPonSel->PontoBordo.x - itSPonSel->PontoBordoAnt.x) / 2.0 +
                      (itSPonSel->PontoBordo.y + itSPonSel->ItLPontoMaisProxCN->y) * (itSPonSel->ItLPontoMaisProxCN->x - itSPonSel->PontoBordo.x) / 2.0 +
                      (itSPonSel->ItLPontoMaisProxCN->y + itSPonSel->PontoBordoAnt.y) * (itSPonSel->PontoBordoAnt.x - itSPonSel->ItLPontoMaisProxCN->x) / 2.0);

          //--- Se o ponto estiver no lado certo verifica se ele é o mais próximo
          //--- Se for seleciona-o.

          if (Area > 0.0)
          {
            if (MenorDist > itSPonSel->Distancia)
            {
              MenorDist = itSPonSel->Distancia;
              itPontoMaisPerto = itSPonSel;
            }
          }
        }

        //--- Se achou o ponto mais perto do lado certo

        if (itPontoMaisPerto != SetPontosSelecionados.end()) /*&&itPontoMaisPerto->PontoBordo.Baliza2 == false)*/
        {
          //--- Conecta a CN de terreno na de terraplanagem
          //--- Procura na CN o ponto de conexao 

          ItLPontos itLPCNAtual(std::find(itPontoMaisPerto->ItCNPonMaisProx->begin(), itPontoMaisPerto->ItCNPonMaisProx->end(), *itPontoMaisPerto->ItLPontoMaisProxCN));  //--- Ponto anterior a interseção  na CN

          //--- Se achou um mais próximo, do lado correto.

          if (itLPCNAtual != itPontoMaisPerto->ItCNPonMaisProx->end())
          {
            Achou = true;
            Fechou = false;  //--- CN circular
			
			      SetCNTNUsadas.insert(itPontoMaisPerto->ItCNPonMaisProx);             //--- Marca a CN como usada pela terraplanagem          
			
      //      (const_cast<Ponto*>(&itPontoMaisPerto->PontoBordo))->Baliza2 = true;   //--- Marca que este ponto está sendo usado por uma CN
            itLCNTerrapAtual->Baliza2 = true;                                      //--- Iddem na terraplanagem

            //--- Insere os pontos de terreno no inicio (Lado esquerdo) ou fim (Lado direito) da lista da CN de terraplanagem,
            //--- invertendo o sentido se o lado é esquerdo

            if (Lado == ESQUERDO)
            {
              if (itLPCNAtual->Baliza2 == false)
              {
                itLPCNAtual->Baliza2 = true;   //--- Marca que o ponto da CN de terreno foi usado

              //--- A SuperficieTP sempre gira para a direita, sua area será sempre > 0.0

                while (itLPCNAtual != itPontoMaisPerto->ItCNPonMaisProx->begin() && !Fechou)
                {
                  itLLCNTerr->emplace_front(*itLPCNAtual);
                  itLPCNAtual->Baliza2 = true;

                  if (itLPCNAtual->Baliza == true)   //--- se for interseção com o bordo a CN é circular, insere o ultimo ponto da lista, fechando o círculo
                  {
                    itLPCNAtual->Baliza2 = true;
                    itLLCNTerr->rbegin()->Baliza2 = true;
                    itLLCNTerr->emplace_front(*itLLCNTerr->rbegin());  //--- Fecha o círculo
                    Fechou = true;
                  }
                  --itLPCNAtual;
                }
              }
            }
            else
            {
              if (itLPCNAtual->Baliza2 == false)
              {
                itLPCNAtual->Baliza2 = true;      //--- Marca que o ponto da CN de terreno foi usado

                while (itLPCNAtual != itPontoMaisPerto->ItCNPonMaisProx->end() && !Fechou)
                {
                  itLLCNTerr->emplace_back(*itLPCNAtual);
                  itLPCNAtual->Baliza2 = true;

                  if (itLPCNAtual->Baliza == true)   //--- se for interseção com o bordo a CN é circular, insere o ultimo ponto da lista, fechando o círculo
                  {
                    itLPCNAtual->Baliza2 = true;
                    itLLCNTerr->begin()->Baliza2 = true;
                    itLLCNTerr->emplace_back(*itLLCNTerr->begin());  //--- Fecha o círculo
                    Fechou = true;
                  }
                  ++itLPCNAtual;
                }
              }
            }
          }
        }
      }
    }
  }

 // InsereCNTerrenoNaoUsadas(SuperficieTN,SuperfTerrap,SetCNTNUsadas);

  if (0 && SuperfTerrap.InverteuFronteira()) SuperfTerrap.InverteFronteira();

  //--- Apenas a superficie de terraplanagem é gravada.

  SuperfTerrap.Serialize(CString(((CMainFrame*)AfxGetMainWnd())->PegaProjetoAtual()), GRAVAR, false, true, false, false);

    return 0;   //--- 0 -> OK
  }

int CDCalcTerraplProjeto::LadoOffs(const Ponto& PontoOffs)
{
  return PontoOffs.Descricao == "OFFSEE" ? ESQUERDO : PontoOffs.Descricao == "OFFSED" ? DIREITO : NENHUM_LADO;
}

void CDCalcTerraplProjeto::InsereCNTerrenoNaoUsadas(CSuperficie& pSuperficieTN,CSuperficie& pSuperfTerrap, tysetItLLPontos& pSetCNTNUsadas)
{
	//--- Se a CN de terreno não foi usada pela terraplanagem passa-a para a terraplanagem
	
  for (ItLLPontos itLLCN = pSuperficieTN.LPontosCN.begin(); itLLCN != pSuperficieTN.LPontosCN.end(); itLLCN++)     //--- Para todas as CN de terreno
  {		
	  if(pSetCNTNUsadas.find(itLLCN) == pSetCNTNUsadas.end())          
	  {
	    pSuperfTerrap.LPontosCN.insert(pSuperfTerrap.LPontosCN.end(),*itLLCN);
	  }
  }
}
