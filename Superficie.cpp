// Superficie.cpp : implementation file
//
#pragma warning(disable:4786)

#include "stdafx.h"
#include "afxtempl.h"
#include "est.h"
#include <list>
#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>
#include "cponto.h"
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CLPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include <stack>
#include "CNoTalvegue.h"
#include "Superficie.h"
#include "monologo.h"
#include "dialogo.h"
#include "math.h"
#include "mat.h"
#include <iomanip>
#include <sstream>    
#include <strstream>   
#include "io.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const double M_2_PI = 6.2831853071795864769;
const double M_PI = 3.14159265358979323846;
const double M_PI_2 = 1.57079632679489661923;
const double M_PI_4 = 0.78539816339744830911;
const double M_PI_3_2 = 4.71238898038468985769;

const double DX = 1.0e-12;

#define LER   1
#define GRAVAR 2
#define OK    1
#define NOK   0 

extern monologo monolog;
extern dialogo dialogar;
extern Matem Mat;

typedef std::set<std::string> tysetNomeArquivos;
typedef std::set<std::string>::iterator tyditsetNomeArquivos;

typedef std::list<ittysetArestas> tylstItLpSuperficiesLigadas;
typedef tylstItLpSuperficiesLigadas::iterator ittlstItLpSuperficiesLigadas;

enum { INICIO_INICIO, INICIO_FIM, FIM_INICIO, FIM_FIM, FIM };

/*
tysetint CSuperficie::SetNGeraArestaForcada = { CAciTopografico::TAMPA_BUEIRO,CAciTopografico::BUEIRO,CAciTopografico::PONTE,CAciTopografico::EIXO,CAciTopografico::REDE_ELETRICA_AT,CAciTopografico::REDE_ELETRICA_BT,
CAciTopografico::CANTO_CASA,CAciTopografico::CANTO_CERCA,CAciTopografico::CANTO_MURO,CAciTopografico::POSTE,CAciTopografico::CASA,CAciTopografico::CERCA,
CAciTopografico::MURO,CAciTopografico::MATA,CAciTopografico::FLORESTA,CAciTopografico::PLANTACAO,CAciTopografico::ALINHAMENTO,
CAciTopografico::TALVEGUE,CAciTopografico::ESPIGAO,CAciTopografico::NENHUM_ACITOPOG };
*/

/////////////////////////////////////////////////////////////////////////////
// CSuperficie

CSuperficie::CSuperficie(CString& NomeProj, bool SoRestricoes) : //,bool pSuperfTerrapl) :
  Rotacionada(false),
  AnguloDeRotacao(0.0),
  Seno(0.0),
  Coseno(1.0),
  NomeProjeto(NomeProj),
  LimiteEsq(INFINITO),
  LimiteSup(-INFINITO),
  LimiteDir(-INFINITO),
  LimiteInf(INFINITO),
  pSuperficieAtual(&SetSuperficie[0]),
  pOutraSuperficie(&SetSuperficie[1]),
  Alterada(false),
  AngRotacaoBordos(0.0),
  InicioInterpolacao(false),
  Area(0.0),
  Perimetro(0.0),
  NPontoInsUsuario(0),
  ErroInterpolacao(0),
  bInverteuFronteira(false),
  douDeclinacao(0.0),
  ItTalvFozSIG(ListaRestricoes.end())
{
  Serialize(NomeProj, LER, SoRestricoes, false);
  LeArqPIS(NomeProj);              //--- Pontos Incluidos nas Secoes
  LeArqPontosSoltosDGN();

  RedefineLimites();  //--- não deveria chamar aqui, os limites deveriam ser acertados a medida que os pontos são lidos

  AngRotacaoTopInt = AnguloDeRotacao * 180.0 / Mat.M_PI + .5;
  if (AngRotacaoTopInt > 0) AngRotacaoTopInt = 360 - AngRotacaoTopInt;
}

CSuperficie::~CSuperficie()
{
  //	for(int QElem = size() ; QElem ; QElem--)
  {
    //delete *(this->begin());
    //pop_front();
  }
}

void CSuperficie::CriaSetCurvasNivelNaoCotadasDGN(int SuperfDrones)
{
  for (ItLLPontos itLLCN = LPontosCN.begin(); itLLCN != LPontosCN.end(); itLLCN++)
  {
    int C(0);

    for (register ItLPontos itLCN = itLLCN->begin(); itLCN != itLLCN->end(); itLCN++) 
    {
       //--- O programa que trata as fotos dos drones (AGISOFT) esta gerando curvas de nivel diferentes que passam pelo mesmo ponto (campogolf)
       //--- com também curvas de nível dando laços. Isto inviabiliza o set de curvas de nivel, portanto a solucão foi marcar na superfície os 
       //--- pontos de CN como CN.  <<=== Nao foi confirmado ==> VERIFICAR

       CDeltaSuperficie CDS(*itLCN);

      ItSSuperficie ItSSuperf(pSuperficieAtual->find(CDS));

      if (ItSSuperf != pSuperficieAtual->end())
      {
        int Baliza(Ponto::CURVA_NIVEL);
        if (SuperfDrones)
        {
          Baliza |= !(C++ % 8) ? Ponto::PONTO_MARCADO_CN : 0;
        }
        else
        {
          Baliza |= Ponto::PONTO_MARCADO_CN;
        }

        const_cast<Ponto*>(&(ItSSuperf->PCentral))->Baliza2 = Baliza;
        SetPontosCurvasNivel.emplace(itLCN,&*itLLCN);
      }
    }
  }
}

#include "CConfigProj.h"

int CSuperficie::Serialize(const CString& NomeProjeto, int Tipo, bool SoRestricoes, bool SoCurvasNivel, bool GravarPontosEditados, bool SoBordos,bool ETrecho,bool LerRestricoesAcad)
{
  if (NomeProjeto.IsEmpty()) return !OK;

  CConfigProj ConfigProjeto(NULL, NomeProjeto);
  ConfigProjeto.LeArquivo();

  std::string NomeProjBase(NomeProjeto);
  CString Nom(NomeProjBase.c_str() + CString(".sup"));
  UINT Flags;
  if (Tipo != LER) Flags = CFile::modeCreate | CFile::modeWrite;
  else Flags = CFile::modeRead; // | /*CFile::shareDenyNone |*/ (Tipo != LER ? CFile::modeWrite : 0)); 

  if (!SoRestricoes && !SoCurvasNivel && !LerRestricoesAcad)
  {
    CFile Arq;
    CFileException e;

    if (Arq.Open(Nom, Flags, &e) == TRUE)
    {
      if (Tipo == LER)
      {
        //********* colocar a causa OK
        if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
        {
          CArchive CArcSuperficie(&Arq, CArchive::load);
          size_t QDeltasSup;
          ItSSuperficie itSuperf;

          CArcSuperficie >> AnguloDeRotacao >> douDeclinacao;
          
          Rotacionada = AnguloDeRotacao != 0.0;
          Seno = sin(AnguloDeRotacao);
          Coseno = cos(AnguloDeRotacao);

          for (CArcSuperficie >> QDeltasSup; QDeltasSup; QDeltasSup--)
          {
            try
            {
            bool Acrescentada(false);

            CDeltaSuperficie DeltaSupAtual;

            DeltaSupAtual.Serialize(CArcSuperficie, pSuperficieAtual, SoRestricoes,true);  //--- SaoPontosCN So é usado aqui -(Arquivo de CNS do do IBGE)

            //--- Pontos inserido são os inseridos por delaunay ou gabriel porque são desprezados no cálculo
            //--- Os pontos inseridos pelas seções não são.

            if (DeltaSupAtual.PCentral.Descricao.Find("Ponto Inserido") != -1 && 
                (DeltaSupAtual.PCentral.Descricao.Find("Delaunay") == -1 || DeltaSupAtual.PCentral.Descricao.Find("Gabriel") != -1)) DeltaSupAtual.PCentral.Inserido = true;
            else DeltaSupAtual.PCentral.Inserido = false;

            //--- Se a delta superficie já existe no set é porque ela já foi inserida como um ponto ligado de uma 
            //--- delta superficie anterior. neste caso a sua lista pontos ligados que é atualizada pois como o ponto
            //--- foi inserido como ponto ligado, ela está vazia

            itSuperf = pSuperficieAtual->find(DeltaSupAtual);

            //if(itSuperf != pSuperficieAtual->end()) (*itSuperf).PontosLigados =  DeltaSupAtual.PontosLigados;   set agora é const então tem que remover e iserir de novo
            //else pSuperficieAtual->insert(DeltaSupAtual);

            if (itSuperf != pSuperficieAtual->end())  //--- Parte das arestas já foi lida Acontece raramente não cgal - gabriel
            {
              if (itSuperf->PontosLigados.size() > 0)
              {
                //--- Inclui as arestas faltantes

               //--- pSuperficieAtual->erase(itSuperf);

                *(const_cast<CDeltaSuperficie*> (&*itSuperf)) += DeltaSupAtual;

                Acrescentada = true;
              }
              else pSuperficieAtual->erase(itSuperf);       //--- A superficie foi inserida apenas como um ponto ligado, insere-a novamente
            }

            if (!Acrescentada)
            {
              auto PairSetDeltaSup(pSuperficieAtual->insert(DeltaSupAtual));

              //if (pSuperficieAtual->insert(DeltaSupAtual).second == true)
              if (PairSetDeltaSup.second == true)
              {
                itSuperf = PairSetDeltaSup.first;     //--- Para limpar os pontos ligados duplicados

                DeltaSupAtual.PontosLigados.clear();
                AtualizaLimites(DeltaSupAtual);

                if (DeltaSupAtual.Status == CDeltaSuperficie::INSERIDO_USUARIO)
                {
                  SetPontosInseridos.insert(DeltaSupAtual.PegaPCentral());
                }
                if (DeltaSupAtual.Status == CDeltaSuperficie::DESABILITADO)
                {
                  SetPontosDesabilitados.insert(DeltaSupAtual.PegaPCentral());
                }
              }
              else
              {
                //--- Esta lendo uma segunda parte das arestas geradas pela CGAL

                //monolog.mensagem(38, CString(" (LOC-001) "));
                //return !OK;
              }
            }

            //--- @CARLOS - 30-07-2022
            //--- remove os pontos ligados repetidos
            //--- Começou a acontecer com a inclusão da superficie do autocad porque os 
            //--- azimutes em ltArestaS estão ficando muito diferentes para o mesmo ponto
            //--- Tentei outras soluções mas deu muito efeito colateral
            //--- Investigar estes efeitos para uma solução melhor
            //--- 28092022
            //--- Na importação dos pontos do autocad as coordenadas estavam sendo arredondadas na
            //--- 9a casa decima, talvez fosse este o erro ===> VERIFICAR

            tysetArestasP PontosLigados;

            for (auto it = itSuperf->PontosLigados.begin(); it != itSuperf->PontosLigados.end(); it++)
            {
              PontosLigados.insert(*it);
            }

            auto ItDeltaSuperf((const_cast<CDeltaSuperficie*> (&*itSuperf)));

            ItDeltaSuperf->PontosLigados.clear();

            for (auto it = PontosLigados.begin(); it != PontosLigados.end(); it++)
            {
              ItDeltaSuperf->PontosLigados.insert(*it);
            }

            }//-------------------------------------------------------------------------------
            catch(...)
            {
            }
          }
          //--- Serializa os pontos desabilitados
          /*
          Ponto PAtual;
          size_t QtdPontosDesabilitados(0);

          for( CArcSuperficie >> QtdPontosDesabilitados ; QtdPontosDesabilitados  > 0 ; --QtdPontosDesabilitados)
          {
          PAtual << CArcSuperficie;

          SetPontosDesabilitados.insert(PAtual);
          }
          */
        }
        else
        {
          monolog.mensagem(14, CString(" Superficie: ") + Nom, e.m_cause);
          return !OK;
        }

        //---- Le o arquivo de pontos editados pelo usuário

        SetPontosInseridos.clear();
        SetPontosDesabilitados.clear();
        std::string NomeArquivo(NomeProjBase + "_PontosEditadosUsuario.xyz");

        std::ifstream fstPonEditados(NomeArquivo, std::fstream::in);

        if (fstPonEditados.good())
        {
          std::string strBuffer;

          for (int C = 0; C < 6; ++C) std::getline(fstPonEditados, strBuffer);

          std::string strNumPonto, strx, stry, strz, strDescricao;
          bool PontosDesabilitados(false);
          int ContaLinhas(6);
          std::stringstream strstrMsg;

          do
          {
            if (strBuffer.find("//") == std::string::npos)
            {
              std::stringstream strstrBuffer(strBuffer);
              strstrBuffer >> strNumPonto >> stry >> strx >> strz;
              std::getline(strstrBuffer, strDescricao);

              if (strx.empty() || stry.empty() || strz.empty() || !Mat.ENumero(strx) || !Mat.ENumero(stry) || !Mat.ENumero(strz))
              {
                strstrMsg.str("");
                strstrMsg.seekg(0, strstrMsg.beg);
                strstrMsg << "Erro na leitura do arquivo - " << NomeArquivo << std::endl;
                strstrMsg << "Linha = " << ContaLinhas << std::endl << " Abortando..." << std::endl;
                strstrMsg << "Dados Lidos x =" << strx << " y=" << stry << " z=" << strz << " Obs=" << strDescricao << std::endl;
                strstrMsg << "Veja o arquivo de log" << std::ends;

                AfxMessageBox(strstrMsg.str().c_str());
              }
              else
              {
                if (ConfigProjeto.FormArqPontos != "n N E C O") AcertaCamposLer(ConfigProjeto.FormArqPontos,strNumPonto,strx,stry,strz, strDescricao);

                NPontoInsUsuario = atoi(strNumPonto.c_str());
                SSetPontos& SetAtual(PontosDesabilitados ? SetPontosDesabilitados : SetPontosInseridos);
                Ponto PAtual(atof(strx.c_str()), atof(stry.c_str()), atof(strz.c_str()), INFINITO, INFINITO, INFINITO, CString(strDescricao.c_str()), false, false, CString(strNumPonto.c_str()));

                if (EstaRotacionada()) PAtual.RotacionaPlano(Seno, Coseno,AnguloDeRotacao);
                
                SetAtual.insert(PAtual);
              }
            }

            std::getline(fstPonEditados, strBuffer);
            ContaLinhas++;

            if (strBuffer.find("Pontos Desabilitados") != std::string::npos || (strBuffer.find("Pontos Removidos") != std::string::npos))
            {
              PontosDesabilitados = true;
              std::getline(fstPonEditados, strBuffer);
              ++ContaLinhas;
            }
          } while (fstPonEditados.good());

          if (!fstPonEditados.eof()) AfxMessageBox("Erro na leitura do arquivo de pontos inseridos.");
        }
        /*
        if (EstaRotacionada() == true)
        {
          for (register ItSSetPontos itSP = SetPontosInseridos.begin(); itSP != SetPontosInseridos.end(); ++itSP)
          {
            (const_cast<Ponto*>(&*itSP))->RotacionaPlano(Seno, Coseno, AnguloDeRotacao);
          }
        }
        */
      }
      else
      {
        CArchive CArcSuperficie(&Arq, CArchive::store);
        size_t QDeltasSup(pSuperficieAtual->size());
        ItSSuperficie ItSSupAtual(pSuperficieAtual->begin());

        CArcSuperficie << AnguloDeRotacao << douDeclinacao;

        for (CArcSuperficie << QDeltasSup; QDeltasSup; QDeltasSup--, ItSSupAtual++)
        {
          CDeltaSuperficie* pCDS(const_cast<CDeltaSuperficie*>(&*ItSSupAtual));
          pCDS->Serialize(CArcSuperficie, ((SSuperficie*)this), SoRestricoes);
        }

        //--- Serializa os pontos desabilitados
        /*
        Ponto PAtual;
        size_t QtdPontosDesabilitados(0);

        CArcSuperficie << SetPontosDesabilitados.size();

        for(ItSSetPontos it = SetPontosDesabilitados.begin() ; it != SetPontosDesabilitados.end() ; ++it)
        {
        *it >> CArcSuperficie;
        }

        */

        if (GravarPontosEditados)
        {
          if (EstaRotacionada() == true)
          {
            double Sen(sin(-AnguloDeRotacao)), Cos(cos(-AnguloDeRotacao));

            for (register ItSSetPontos itSP = SetPontosInseridos.begin(); itSP != SetPontosInseridos.end(); ++itSP)
            {
              (const_cast<Ponto*>(&*itSP))->RotacionaPlano(Sen, Cos, -AnguloDeRotacao);
            }
          }

          int Contador(1);

          //--- Grava txt com os pontos editados pelo usuario

          std::fstream fstPonEditados(NomeProjBase + "_PontosEditadosUsuario.xyz", std::fstream::out | std::fstream::trunc);

          fstPonEditados << std::right << std::fixed << std::setfill(' ') << std::setprecision(6);
          fstPonEditados << "//--- Este arquivo contém os pontos editados pelo usuário " << std::endl;
          fstPonEditados << "//==> NÃO EDITAR. A edição deste arquivo produzirá erros no cálculo da superfície." << std::endl;
          fstPonEditados << "//" << std::endl;
          fstPonEditados << "//--- Pontos Inseridos: " << std::endl;
          fstPonEditados << "//" << std::endl;

          for (ItSSetPontos itSP = SetPontosInseridos.begin(); itSP != SetPontosInseridos.end(); ++itSP)
          {
		    //--- Os pontos de drenagem não são incluidos na superfície
		  
            if(itSP->AcidenteTopografico < CAciTopografico::INICIO_EQUI_DRENAGEM || itSP->AcidenteTopografico > CAciTopografico::FIM_EQUI_DRENAGEM)
            {
              *itSP >> fstPonEditados;
              fstPonEditados << std::endl;
            }
          }

          fstPonEditados << "//" << std::endl;
          fstPonEditados << "//--- Pontos Desabilitados: " << std::endl;
          fstPonEditados << "//" << std::endl;

          if (Rotacionada)
          {
            double Sen(sin(-AnguloDeRotacao)), Cos(cos(-AnguloDeRotacao));

            for (ItSSetPontos itSP = SetPontosDesabilitados.begin(); itSP != SetPontosDesabilitados.end(); ++itSP)
            {
              const_cast<Ponto*>(&*itSP)->RotacionaPlano(Sen,Cos, -AnguloDeRotacao);
            }
          }

          Contador = 1;

          for (ItSSetPontos itSP = SetPontosDesabilitados.begin(); itSP != SetPontosDesabilitados.end(); ++itSP)
          {
            fstPonEditados << Contador++;
            *itSP >> fstPonEditados;
            fstPonEditados << std::endl;
          }
        }
      }

      Arq.Close();
    }
    else
    {
      char* pBuffer = new char[102400000];

      if (LePontos(pBuffer) == false)
      {
        AfxMessageBox("Houve erro ao ler os arquivos de terreno");
        return !OK;
      }

      delete[]  pBuffer;
    }
  }

  //*******************************************
  //---  Serializa as Restrições e os bordos
  //*******************************************

  if (!SoCurvasNivel || LerRestricoesAcad)
  {
    CFile Arq;

    for (int i = 0; i < 2; i++)
    {
      CFileException e;

      Nom = NomeProjBase.c_str() + CString(i == 0 ? (LerRestricoesAcad ? ".rea" : ".res") : ".bor");
      if (Tipo != LER) Flags = CFile::modeCreate | CFile::modeWrite;
      else Flags = CFile::modeRead;

      if (Arq.Open(Nom, Flags, &e) == 0)
      {
        if (e.m_cause != CFileException::fileNotFound)
        {
          monolog.mensagem(1, Nom);
          return !OK;
        }
        // else break;                 //--- Arquivo não encontrado
      }
      else
      {
        size_t QRestricoes(0);

        if (Tipo == LER)
        {
          CArchive CArcRestricoes(&Arq, CArchive::load);

          if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
          {
            LLDeltaSup* ListasAtuais(i == 0 ? &ListaRestricoes : &ListaBordos);

            //if(!SoBordos && LerRestricoesAcad) 
            if (LerRestricoesAcad)
            {
              ListasAtuais->clear();
              MMapItLLDeltaSup.clear();
            }

            CArcRestricoes >> AngRotacaoBordos;

            for (CArcRestricoes >> QRestricoes; QRestricoes; QRestricoes--)    //--- Para todas as restrições
            {
              ListasAtuais->push_back(LDeltaSup());

              ItLLDeltaSup ItLLDeltaSupAtual(ListasAtuais->end());
              ItLLDeltaSupAtual--;
              size_t QPontos(0);
              CDeltaSuperficie SuperficieAtual;
              bool PrimeiroPonto(true);
              Ponto PontoAnterior;
              unsigned int Tipo(0);

              CArcRestricoes >> Tipo;

              try
              {
                for (CArcRestricoes >> QPontos; QPontos; QPontos--)       //--- Para todos os pontos da restrição atual
                {
                  //---
                  SuperficieAtual.Serialize(CArcRestricoes, ((SSuperficie*)this), true);

                  SuperficieAtual.PCentral.Inserido = SuperficieAtual.PCentral.Descricao.Find("Ponto Inserido") != -1;
                  SuperficieAtual.ERestricao = !LerRestricoesAcad ? i == 0 : 0;
                  SuperficieAtual.TipoObjeto = Tipo;

                  if (Tipo == CAciTopografico::FRONTEIRA && i == 0)
                  {
                    //--- Quando le calcula a area e o perimetro ===> não precisa. Ler o valor direto, já foi calculado!!!

                    if (PrimeiroPonto)
                    {
                      Area = 0.0;
                      Perimetro = 0.0;
                      PontoAnterior = SuperficieAtual.PegaPCentral();
                      PrimeiroPonto = false;
                    }
                    else
                    {
                      Area += (SuperficieAtual.PCentral.y + PontoAnterior.y) / 2.0 * (SuperficieAtual.PCentral.x - PontoAnterior.x);
                      Perimetro += hypot(SuperficieAtual.PCentral.y - PontoAnterior.y, SuperficieAtual.PCentral.x - PontoAnterior.x);
                      PontoAnterior = SuperficieAtual.PegaPCentral();
                    }
                  }

                  ItLLDeltaSupAtual->push_back(SuperficieAtual);

                  ItSSuperficie itDSupAtual(pSuperficieAtual->find(SuperficieAtual));
                  if (itDSupAtual != pSuperficieAtual->end())
                  {
                    CDeltaSuperficie* pCDS(const_cast<CDeltaSuperficie*>(&*itDSupAtual));

                    if (i == 0) pCDS->ERestricao = true;
                    else pCDS->EBordo = true;
                    if (SuperficieAtual.PCentral.Inserido == true) pCDS->PCentral.Inserido = true;
                    pCDS->PCentral.S = SuperficieAtual.PCentral.S;                                    //--- Nocaso de terraplanagem passa a estaca (usa se for GRADIE)
                    pCDS->TipoObjeto = Tipo;
                  }
                }
              }
              catch (CArchiveException* e)
              {
                if (i == 0) AfxMessageBox("Houve erro na leitura do arquivo de restrições.");
                else  AfxMessageBox("Houve erro na leitura do arquivo de bordos.");

                return !OK;
              }
              if (i == 0)
              {
                if (ItLLDeltaSupAtual->size() == 0 ||
                  (ItLLDeltaSupAtual->size() == 1 &&
                    Tipo != CAciTopografico::ARVORE && Tipo != CAciTopografico::TAMPA_BUEIRO && Tipo != CAciTopografico::CANTO_CASA &&
                    Tipo != CAciTopografico::CANTO_CERCA && Tipo != CAciTopografico::CANTO_MURO && Tipo != CAciTopografico::POSTE &&
                    Tipo != CAciTopografico::PVELETRICA && Tipo != CAciTopografico::PVESGOTO && Tipo != CAciTopografico::PVGAS))
                {
                  // ASSERT(0);
                  ListasAtuais->pop_back();
                }
                else
                {
                  MMapItLLDeltaSup.insert(pairmmapitLLDeltaSup(Tipo, ItLLDeltaSupAtual));
                }
              }
              else if (ItLLDeltaSupAtual->size() < 3) ListasAtuais->pop_back();

            }
          }
        }
        else
        {
          CArchive CArcRestricoes(&Arq, CArchive::store);

          LLDeltaSup* ListasAtuais(i == 0 ? &ListaRestricoes : &ListaBordos);

          CArcRestricoes << AnguloDeRotacao;
          CArcRestricoes << ListasAtuais->size();

          for (register ItLLDeltaSup ItLLDeltaSupAtual = ListasAtuais->begin(); ItLLDeltaSupAtual != ListasAtuais->end(); ItLLDeltaSupAtual++)
          {
            if (ItLLDeltaSupAtual->size() == 0)
            {
              //          assert(FALSE);
            }

            unsigned int TipoAciTopografico(0);

            if (i == 0) TipoAciTopografico = VerfAciTopografico(ItLLDeltaSupAtual);
            else TipoAciTopografico = CAciTopografico::FRONTEIRA;

            CArcRestricoes << TipoAciTopografico;
            CArcRestricoes << (*ItLLDeltaSupAtual).size();

            for (register ItLDeltaSup ItLDeltaSupAtual = ItLLDeltaSupAtual->begin(); ItLDeltaSupAtual != (*ItLLDeltaSupAtual).end(); ItLDeltaSupAtual++)
              ItLDeltaSupAtual->Serialize(CArcRestricoes, ((SSuperficie*)this), true);
          }

          if (i == 1) CArcRestricoes << Area << Perimetro;
        }
        Arq.Close();
      }
    }
  }

  if(LerRestricoesAcad) return OK;
  if (!SoRestricoes)
  {
    CFile Arq,Arq2;
    CFileException e,e2;

    //*******************************************
    //---  Serializa as Curvas de nivel
    //*******************************************

    Nom = NomeProjBase.c_str() + CString(".cun");
	
    if (Tipo != LER) 
    {
      Flags = CFile::modeCreate | CFile::modeWrite;
    }
    else 
    {
      Flags = CFile::modeRead;
    }

    if (Arq.Open(Nom, Flags, &e) == 0)
    {
      if (e.m_cause != CFileException::fileNotFound)
      {
        monolog.mensagem(1, Nom);
        return !OK;
      }
    }

    if (Tipo == LER)
    {
      //--- Que LOKO!!!
      
      CString NomeCNDrone(NomeProjBase.c_str() + CString(".cnd"));

      DWORD dwAttrib(GetFileAttributes(NomeCNDrone.GetBuffer()));
      bool TemCNDrone(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));

      if (TemCNDrone)
      {
        if (Arq2.Open(NomeCNDrone.GetBuffer(), CFile::modeRead, &e2) == 0)
        {
          if (e2.m_cause != CFileException::fileNotFound)
          {
            monolog.mensagem(1, Nom);
            return !OK;
          }
        }
      }

      CArchive CArcCurvasNivel(TemCNDrone ? &Arq2 : &Arq, CArchive::load);
      size_t QtdCurvasNivel(0), QtdPontosCNAtual(0);

      if ((TemCNDrone && e2.m_cause != CFileException::fileNotFound) || (!TemCNDrone && e.m_cause != CFileException::fileNotFound))   //--- Se Existe o arquivo
      {
        LPontosCN.clear();

        CArcCurvasNivel >> AngRotacaoBordos >> EquidisCN;

        for (CArcCurvasNivel >> QtdCurvasNivel; QtdCurvasNivel; QtdCurvasNivel--)
        {
          LPontosCN.push_back(LPontos());
          LPontos& LCurvaNivelAtual(LPontosCN.back());
          size_t QtdPontosCNAtual(0);
          int Cont(0);
          Ponto PAnterior, PAtual;

          for (CArcCurvasNivel >> QtdPontosCNAtual; QtdPontosCNAtual; QtdPontosCNAtual--)    //--- Para todas as CNs
          {
            PAtual << CArcCurvasNivel;

            //--- Interpola um ponto no meio para desenhar as curvas topográficas

            if (!TemCNDrone && Cont > 1 && Cont % 2 == 1)
            {
              Ponto PInterpolado((PAtual.x + PAnterior.x) / 2.0, (PAtual.y + PAnterior.y) / 2.0, PAtual.z);
              PInterpolado.Baliza = true;               //--- Indica que o ponto foi interpolado
              LCurvaNivelAtual.push_back(PInterpolado);
            }

            LCurvaNivelAtual.push_back(PAtual);
            PAnterior = PAtual;
            Cont++;
          }
          if (!TemCNDrone && Cont % 2 == 1)
          {
            ItLPontos itPontoCN(LCurvaNivelAtual.end());
            itPontoCN--;
            if (itPontoCN != LCurvaNivelAtual.begin()) itPontoCN--;

            Ponto PInterpolado((PAtual.x + itPontoCN->x) / 2.0, (PAtual.y + itPontoCN->y) / 2.0, PAtual.z);

            LCurvaNivelAtual.pop_back();
            LCurvaNivelAtual.push_back(PInterpolado);
            LCurvaNivelAtual.push_back(PAtual);
          }
          if (LCurvaNivelAtual.size() == 0)
          {
            LPontosCN.pop_back();
          }
        }
      }
    }
    else
    {
      CArchive CArcCurvasNivel(&Arq, CArchive::store);
      unsigned int QtdCurvasNivel(0);

      if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
      {
        CArcCurvasNivel << AngRotacaoBordos << EquidisCN;
        CArcCurvasNivel << LPontosCN.size();

        ItLLPontos itLLCN(LPontosCN.begin());

        for (ItLLPontos itLLCN = LPontosCN.begin(); itLLCN != LPontosCN.end(); itLLCN++)
        {
          LPontos LPontosCNTemp(itLLCN->begin(), itLLCN->end());
          LITLPontos LITPontosCNRemover;

          for (register ItLPontos itLCNTemp = LPontosCNTemp.begin(); itLCNTemp != LPontosCNTemp.end(); itLCNTemp++)
            if (itLCNTemp->Baliza == true) LITPontosCNRemover.push_back(itLCNTemp);

          for (register ItLITLPontos itLCNRem = LITPontosCNRemover.begin(); itLCNRem != LITPontosCNRemover.end(); itLCNRem++)
            LPontosCNTemp.erase(*itLCNRem);

          CArcCurvasNivel << LPontosCNTemp.size();

          ItLPontos ItPAnterior(LPontosCNTemp.begin());

          for (ItLPontos itLCN = LPontosCNTemp.begin(); itLCN != LPontosCNTemp.end(); itLCN++)
          {
            if (itLCN != LPontosCNTemp.begin())
            {
              itLCN->fi = Mat.CalculaAzimute(*ItPAnterior, *itLCN);
              ItPAnterior = itLCN;
            }
            *itLCN >> CArcCurvasNivel;
          }
        }
      }
    }
  }

  SerializaCNSimbolTerraplanagem(Tipo, std::string(NomeProjeto));

  if(!ETrecho) 
  {
    SerializarBacias(Tipo,std::string(NomeProjeto));
    SerializaBaciasImportadas(Tipo, std::string(NomeProjeto));
    SerializaTalveguesImportados(Tipo, std::string(NomeProjeto));
  }

  return OK;
}

int CSuperficie::Seccionar(const Ponto& P1, const Ponto& P2, const Ponto& CoorEstaca, setPontos& SSetxSecao, double LadoEsq_2, bool bTodas,double xInicial,bool InserirPontas,bool TrimOffSet,bool Talvegue)
{
  //--- Insere os pontos inicial e final

  //Ponto Pinicial(-sqrt(LadoEsq_2), P1.z), PFinal(hypot(P1.x - P2.x, P1.y - P2.y) - sqrt(LadoEsq_2), P2.z);

  //double DistTotal(hypot(P1.x - P2.x, P1.y - P2.y));

  //--- Nos bueiros é preciso inserir os pontos das alas, ele vêem no SSetxSecao

  if (!Talvegue)
  {
    if (SSetxSecao.size() > 0)
    {
      LPontos LPontosXY;

      for (itsetPontos ItPatual = SSetxSecao.begin(); ItPatual != SSetxSecao.end(); )
      {
        //double Dist(hypot(ItPatual->x - P2.x, ItPatual->y - P2.y));
        //double DistAla(Dist -(DistTotal-sqrt(LadoEsq_2)));                 //--- LadoEsq está ao quadrado

        Ponto Ponto_XY(*ItPatual);

        Ponto_XY.x = ItPatual->S;
        Ponto_XY.y = ItPatual->z;
        Ponto_XY.z = INFINITO;

        LPontosXY.push_back(Ponto_XY);

        SSetxSecao.erase(ItPatual++);
      }

      for (ItLPontos itXY = LPontosXY.begin(); itXY != LPontosXY.end(); itXY++)
      {
        SSetxSecao.emplace(*itXY);
      }
    }
  }

    if (InserirPontas)
    {
      //--- Insere os pontos inicial e final

      Ponto Pinicial(-sqrt(LadoEsq_2), P1.z), PFinal(hypot(P1.x - P2.x, P1.y - P2.y) - sqrt(LadoEsq_2), P2.z);

      if (Pinicial.y != INFINITO)  SSetxSecao.emplace(Pinicial);
      if (PFinal.y != INFINITO) SSetxSecao.emplace(PFinal);
    }
 

  ItSSuperficie ItSSupAtual(pSuperficieAtual->begin());     //--- Iterador para o ponto atual da superficie

  //--- Para todos os pontos da superfície 

  for (register size_t QuanPontos = pSuperficieAtual->size(); QuanPontos; QuanPontos--, ItSSupAtual++)
  {
    Ponto PCentral(ItSSupAtual->PegaPCentral()), Intersecao;

    //--- Para todos os vertices que estão ligados ao ponto

    CDeltaSuperficie* pCDS(const_cast<CDeltaSuperficie*>(&*ItSSupAtual));
    pCDS->IniciaPontosLigados();

    //--- ATENÇÃO=> Esta calculando a interseção 2 vezes, como ponto central para ponto ligado e como ponto ligado para ponto central
    //--- Esta até rápido mas no caso de superficie com drones pode melhorar porque são muito mais pontos
    //--- #CORRIGIR

    for (register unsigned int QuanPontosLig = pCDS->PegaQuanPLigados(); QuanPontosLig; QuanPontosLig--, pCDS->itSuperfLigadaAtual++)
    {
      //--- Se achou a interseção insere-a no set de pontos que comporão a seção  
      //--- Caso a interseçao seja repetida não tem importância não vai inserir no set. É melhor 
      //--- calcular duas vezes do que fazer outro set com todos os pontos só para marcar os q já foram feitos.

      if (CLPontos::CalculaIntersecao(P1, P2, PCentral, (*(*pCDS).itSuperfLigadaAtual).PontoLigado, Intersecao))
      {
        //--- Alguns pontos da terraplanagem estão ficando com a cota = infinito, não pode <=== VERIFICAR

        if(CalculaCota(PCentral, (*(*pCDS).itSuperfLigadaAtual).PontoLigado, Intersecao))
        {
          double x = hypot(Intersecao.x - CoorEstaca.x, Intersecao.y - CoorEstaca.y),

          //--- Verifica o lado, se d1 < d2 o ponto está do lado esquerdo

          d1 = pow(P1.x - Intersecao.x, 2.0) + pow(P1.y - Intersecao.y, 2.0);

          Intersecao.x = d1 < LadoEsq_2 ? x * -1.0 : x;
          Intersecao.x += xInicial;
          Intersecao.y = Intersecao.z;
         
          if(PCentral.Descricao.Compare((*(*pCDS).itSuperfLigadaAtual).PontoLigado.Descricao) == 0) 
            Intersecao.Descricao = PCentral.Descricao;
          else
          {
            //--- Caso especial, offset com ponto de passagem
            //--- A descrição do offset pode tem que estar nos  dois 2 pontos
            //--- (É preciso do offset marcado para fazer o trim na seção de terrapplanagem)

            if (PCentral.Descricao.Find("OFFSE") != -1 && (*(*pCDS).itSuperfLigadaAtual).PontoLigado.Descricao.Find("OFFSE") != -1)
            {
              Intersecao.Descricao = PCentral.Descricao;
            }
            else 
            {
              Intersecao.Descricao.Empty();
            }
          }

          pairitboolsetPontos pairISP(SSetxSecao.insert(Intersecao));
        }
      }
    }
  }

  if (TrimOffSet)   //--- O projeto de terraplanagem é aparado no offset
  {
    itsetPontos itini(SSetxSecao.begin()),itfim(SSetxSecao.end());

    //--- apaga do início até o primeiro off-set

    while(itini != SSetxSecao.end() && itini->Descricao.Find("OFFSE") == -1) 
    {
      ++itini;
    }

    //--- não encontrou o offset, então so tem ponto de terreno ( O perfil não cortou  a terraplanagem)

    if (itini == SSetxSecao.end())
    {
      //SSetxSecao.clear();

      //return OK;

      itini = SSetxSecao.begin();
    }
    else
    {
     SSetxSecao.erase(SSetxSecao.begin(), itini);
    }

    itfim = itini;

    itfim++;

      while (itfim != SSetxSecao.end() && itfim->Descricao.Find("OFFSE") == -1)
      {
        ++itfim;
      }

    if(itfim == SSetxSecao.end()) return OK;    //--- A seção acabou no meio da terraplanagem, OK

      //--- Remove do offset direito até o fim da seção

    if(++itfim  != SSetxSecao.end()) SSetxSecao.erase(itfim, SSetxSecao.end()); //--- remove os pontos de terreno do lado direito 
  }

  return OK;
}

int CSuperficie::SeccionarTerrap(const Ponto& P1, const Ponto& P2, const Ponto& CoorEstaca, setPontos& SSetxSecao, double LadoEsq_2,SSuperficie* SuperfTerrap)
{
  ItSSuperficie ItSSupAtual(SuperfTerrap->begin());     //--- Iterador para o ponto atual da superficie

  //--- Para todos os pontos da superfície 

  for (register size_t QuanPontos = SuperfTerrap->size(); QuanPontos; QuanPontos--, ItSSupAtual++)
  {
    Ponto PCentral(ItSSupAtual->PegaPCentral()), Intersecao;

    //--- Para todos os vertices que estão ligados ao ponto

    CDeltaSuperficie* pCDS(const_cast<CDeltaSuperficie*>(&*ItSSupAtual));
    pCDS->IniciaPontosLigados();

    for (register unsigned int QuanPontosLig = pCDS->PegaQuanPLigados(); QuanPontosLig; QuanPontosLig--, pCDS->itSuperfLigadaAtual++)
    {
      //--- Se achou a interseção insere-a no set de pontos que comporão a seção  
      //--- Caso a interseçao seja repetida não tem importância não vai inserir no set. É melhor 
      //--- calcular duas vezes do que fazer outro set com todos os pontos só para marcar os q já foram feitos.

      if (CLPontos::CalculaIntersecao(P1, P2, PCentral, (*(*pCDS).itSuperfLigadaAtual).PontoLigado, Intersecao))
      {
        CalculaCota(PCentral, (*(*pCDS).itSuperfLigadaAtual).PontoLigado, Intersecao);

        double x = hypot(Intersecao.x - CoorEstaca.x, Intersecao.y - CoorEstaca.y),

          //--- Verifica o lado, se d1 < d2 o ponto está do lado esquerdo

        d1 = pow(P1.x - Intersecao.x, 2.0) + pow(P1.y - Intersecao.y, 2.0);

        Intersecao.x = d1 < LadoEsq_2 ? x * -1.0 : x;
        Intersecao.y = Intersecao.z;

        SSetxSecao.insert(Intersecao);
      }
    }
  }

  return OK;
}

bool inline CSuperficie::CalculaCota(const Ponto& P1, const Ponto& P2, Ponto& Intersecao)
{
  if(P1.z != INFINITO && P2.z != INFINITO)
  {
    Intersecao.z = ((P2.z - P1.z) / (fabs(P1.x - P2.x))) * fabs(Intersecao.x - P1.x) + P1.z;

    return true;
  }

  return false;
}

void CSuperficie::PegaNumeroDescricao(const CString& NomeArq)
{
  LeArqPontos(CString("P Y X Z D"), NomeArq);
  LeNumDescPontosInseridos();
}

int CSuperficie::LeArqPontos(const CString& Tipo, const CString& NomeProjeto)
{
  int r(0), n(0), x, y, z, d, nleuXYZ;
  FILE *farquivo = NULL;
  char letra[2];
  CString delimitador, palavra, sn;
  double ponto[6];
  char* Buffer = (char*)malloc(6400000);

  //Verifica o delimitador do arquivo e as posições que devem ter as coordenadas x, y e z.
  delimitador = Tipo[1];

  for (int t = 0; t < 9; t++)
  {
    if (Tipo[t] == delimitador) r++;
    if (Tipo[t] == 'N') n = t - r;
    if (Tipo[t] == 'X') x = t - r;
    if (Tipo[t] == 'Y') y = t - r;
    if (Tipo[t] == 'Z') z = t - r;
    if (Tipo[t] == 'D') d = t - r;
  }

  tysetNomeArquivos setNomeArquivos;

  if (PegaNomesArquivosTerreno(setNomeArquivos, NomeProjeto))
  {
    for (tyditsetNomeArquivos itNomeArqAtual = setNomeArquivos.begin(); itNomeArqAtual != setNomeArquivos.end(); ++itNomeArqAtual)
    {
      bool ArqPontosInseridos(itNomeArqAtual->find("PontosInseridos") != std::string::npos);

      //--- Ponto a ponto encontrado no arquivo coloca a descrição e e numero do ponto na superficie (Set SStuperficie)

      farquivo = fopen(itNomeArqAtual->c_str(), "rt");
      if (farquivo != NULL)
      {
        setvbuf(farquivo, Buffer, _IOFBF, 64000);

        letra[0] = '\x0';

        fread(letra, 1, 1, farquivo);
        do
        {
          nleuXYZ = 0;

          for (int t = 0; t < 5; t++)
          {
            palavra.Empty();
            letra[1] = '\x0';
            while (!feof(farquivo) && (t != 4 ? letra != delimitador : true) && (int(letra[0]) != 13) && (int(letra[0]) != 10))
            {
              palavra += letra;
              fread(&letra, 1, 1, farquivo);
              letra[1] = '\x0';
            }
            ponto[t] = strtod(palavra, '\x0');
            if ((ponto[t] == 0) && ((t == x) || (t == y) || (t == z))) nleuXYZ++;//não era número
            while (!feof(farquivo) && ((letra == delimitador) || (int(letra[0]) == 13) || (int(letra[0]) == 10)))
            {
              fread(&letra, 1, 1, farquivo);
            }
          }

          //Se leu e transformou certo x, y e z.
          if (!nleuXYZ)
          {
            //--- Neste Ponto Temos o ponto com a descrição, basta então procurá-lo no mapa, se for encontrado
            //--- Insere no ponto do mapa a respectiva descrição e o respectivo numero do ponto

            ItSSuperficie DeltaSupSobMouse(PegaPontoMaisProximo(CDeltaSuperficie(ponto[x], ponto[y], 0.00), 0.0005, 0.0005));

            if (DeltaSupSobMouse != pSuperficieAtual->end())
            {
              CDeltaSuperficie* pCDeltaSuperficie(const_cast<CDeltaSuperficie*>(&*DeltaSupSobMouse));

              pCDeltaSuperficie->PCentral.Descricao = palavra;
              pCDeltaSuperficie->PCentral.i = ponto[n];
              if (ArqPontosInseridos) pCDeltaSuperficie->PCentral.Inserido = true;
            }
          }
        } while (!feof(farquivo) && !ferror(farquivo));
      }
      else
      {
        //--- Arquivo Não encontrado

        monolog.mensagem(13, itNomeArqAtual->c_str());
      }

      fclose(farquivo);
    }

    free(Buffer);
  }
  return n > 0;
}

////////////////////////////////////////////////
// CSuperficie message handlers

ItSSuperficie CSuperficie::PegaPontoMaisProximo(CDeltaSuperficie& DSuperficie, double ToleranciaX, double ToleranciaY)
{
  if (pSuperficieAtual->empty()) return pSuperficieAtual->end();

  //--- Verifica o <=

  ItSSuperficie DSupTemp(pSuperficieAtual->lower_bound(DSuperficie));    //--- Pega o <=
  ItSSuperficie DSupUltimo(pSuperficieAtual->end());                     //--- Pega o Ultimo
  DSupUltimo--;

  Ponto Componentes(DSupTemp != pSuperficieAtual->end() ? (*DSupTemp).PegaPCentral() : DSupUltimo->PegaPCentral());

  Componentes -= DSuperficie.PegaPCentral();

  if (fabs(Componentes.x) < ToleranciaX && fabs(Componentes.y) < ToleranciaY)  return DSupTemp != pSuperficieAtual->end() ? DSupTemp : DSupUltimo;

  //--- verifica o anterior

  if (DSupTemp != pSuperficieAtual->end() && DSupTemp != pSuperficieAtual->begin())
  {
    DSupTemp--;
    Componentes = DSupTemp->PegaPCentral();
    Componentes -= DSuperficie.PegaPCentral();

    //--- Verifica o posterior

    if (fabs(Componentes.x) < ToleranciaX && fabs(Componentes.y) < ToleranciaY)
      return DSupTemp;
    else
    {
      DSupTemp++;
      DSupTemp++;
      if (DSupTemp != pSuperficieAtual->end())
      {
        Componentes = (*(DSupTemp)).PegaPCentral();
        Componentes -= DSuperficie.PegaPCentral();

        if (fabs(Componentes.x) < ToleranciaX && fabs(Componentes.y) < ToleranciaY)
          return DSupTemp;
      }
    }
  }
  return pSuperficieAtual->end();      //--- Não achou um ponto dentro da tolerancia
}

int CSuperficie::DefineFronteiraOuPoligono(int Tipo,bool Drones,bool ProjGeoRef,bool ProjACAD)
{
  //---  A LISTA DE BORDOS PODE SER UMA LISTA COMUM, NÃO PRECISA DER DE DELTA SUPERFICIE  ALTERAR!!!!

  ItCLLDeltaSup Inicio, Fim;

  //--- Se precisa pegar so a fronteira ela esta no meio da lista de restrições

  if (Tipo == TIPOFRONTEIRA)
  {
    //itmmapitLLDeltaSup it((Terraplanagem && ProjGeoRef) ? MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA_TERRAP) : MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA));
    
   // itmmapitLLDeltaSup it(Terraplanagem ? MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA_TERRAP) : MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA));

    itmmapitLLDeltaSup it((Drones || ProjACAD) ? MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA_TERRAP) : MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA));

    if (it != MMapItLLDeltaSup.end())
    {
      Inicio = it->second;
      Fim = it->second;
      ++Fim;
    }
    else
    {
      monolog.mensagem(30, "");
      return false;
    }
  }

  //--- No final existe apenas uma lista de bordos, se existir mais que uma a superfície não está fechada, 
  //--- como também se o último ponto não for igual ao primeiro. 

  for (ItCLLDeltaSup itLPontos = Inicio; itLPontos != Fim; ++itLPontos)
  {
   // if (itLPontos->begin()->PCentral != itLPontos->rbegin()->PCentral)
   // if (!(itLPontos->begin()->PCentral == itLPontos->rbegin()->PCentral))
    if(PontoXYIgual(itLPontos->begin()->PCentral, itLPontos->rbegin()->PCentral) == false) //--- Pontos não são iguais
    {
      if (Tipo == TIPOFRONTEIRA)
      {
        std::strstream strTemp;

        const Ponto& PAtual(itLPontos->rbegin()->PCentral);

        strTemp << std::fixed << std::setprecision(3) << "\r\nCoord. N= " << PAtual.x << " Coord. E= " << PAtual.y << std::ends;

        monolog.mensagem(31, strTemp.str());

        return false;
      }
      else continue;    //---- Passa os poligonos abertos
    }

    //--- Desrotaciona os Pontos

    //--- Se for terraplanagem de projeto georeferenciado por drones tem q fazer rotacionado mesmo
    //--- porque estas superfícies são naturalmente rotacionadas. (Declinação)
    //--- na verdade poderia fazer qualquer superficie rotacionada #VERIFICAR

    //--- NAO PRECISA ROTACIONAR PORQUE JÁ ESTÁ ROTACIONADO

    /*
    if (AngRotacaoBordos != 0.0 && !(Tipo == CSuperficie::TIPOFRONTEIRA && Drones && ProjGeoRef))
    {
      double Seno(sin(-AngRotacaoBordos)), Coseno(cos(-AngRotacaoBordos));
//-----aasdasdasdasd
      if(0) for (register ItCLDeltaSup itCDeltaSup = itLPontos->begin(); itCDeltaSup != itLPontos->end(); itCDeltaSup++)
        const_cast<CDeltaSuperficie&>(*itCDeltaSup).RotacionaPcentral(Seno, Coseno);

      // itCDeltaSup->RotacionaPcentral(Seno,Coseno);
    }
    */

    if (itLPontos->size() < 3) continue;      //--- *LBordosAtual é uma lista de listas de bordos

    Area = Perimetro = 0.0;
    ItCLDeltaSup itDeltaSup(itLPontos->begin());
    register const Ponto* PAnterior(&(*itDeltaSup++).PCentral), *PAtual;

    do
    {
      PAtual = &(*itDeltaSup++).PCentral;
      Area += (PAtual->x - PAnterior->x) * ((PAtual->y + PAnterior->y) / 2.0);
      Perimetro += hypot(PAtual->x - PAnterior->x,PAtual->y - PAnterior->y);

      PAnterior = PAtual;

    } while (itDeltaSup != itLPontos->end());

    //--- CORRIGIR ESTE COMENTARIO  <<======
    //--- Se for bordo:
    //--- Retira todos as arestas cujos azimutes forem menores que azimute do lado do bordo, independentemente de 
    //--- ter girado para a direita ou para a esquerda:
    //--- 1- Calcula a área do triangulo formado pelos 3 pontos. Se esta área for
    //---    positiva o ponto esta a direita, caso contrário o ponto está a esquerda.
    //--- 2- Se o bordo girar para a esquerda, retira as arestas a direita
    //---    Se o bordo estiver girando para a direita retira as arestas a esquerda.
    //--- Se for polígono é o contrário

    itDeltaSup = itLPontos->begin();
    PAnterior = &itDeltaSup->PCentral;
    itDeltaSup++;

    do      //--- Enquanto houver pontos 
    {
      PAtual = &itDeltaSup->PCentral;

      //--- Os azimutes estão na superficie, então pe preciso pegar o ponto na superficie.
      //--- Isso não ficou bom, o ideal é q os pontos sejam iteradores para a superficie---===> ALTERAR

      ItCLDeltaSup itLCDS(itDeltaSup);
      itLCDS++;
      const Ponto* PPosterior(&(itLCDS)->PCentral);

      //ItSSuperficie itSuperAtual(pSuperficieAtual->find(itDeltaSup->PCentral));
      ItSSuperficie itSuperAtual(Find(itDeltaSup->PCentral,1e-4));

      if (itSuperAtual == pSuperficieAtual->end())
      {
        std::stringstream strstTemp;

        strstTemp << std::fixed << std::setprecision(3);
        strstTemp << "\r\nCoord N= " << itDeltaSup->PegaPCentral().y << " Coord E= " << itDeltaSup->PegaPCentral().x << " Cota = " << itDeltaSup->PegaPCentral().z << std::ends;
        monolog.mensagem(36, strstTemp.str().c_str());
       
       return NOK;
      }

      ittysetArestas itSuperAnterior(itSuperAtual->PontosLigados.end()),
                     itSuperPosterior(itSuperAtual->PontosLigados.end());

      //--- Pega na superfície os pontos que contém os azimutes

      for (ittysetArestas ItLSL = itSuperAtual->PontosLigados.begin();
           ItLSL != itSuperAtual->PontosLigados.end() && (itSuperAnterior == itSuperAtual->PontosLigados.end() || itSuperPosterior == itSuperAtual->PontosLigados.end());
           ItLSL++)
      {
        if (ItLSL->PontoLigado == PAnterior) itSuperAnterior = ItLSL;
        if (ItLSL->PontoLigado == PPosterior) itSuperPosterior = ItLSL;
      }

      if (itSuperAnterior != itSuperAtual->PontosLigados.end() && itSuperPosterior != itSuperAtual->PontosLigados.end())   //--- so por segurança, tem q achar os 2.
      {
        ittysetArestas ItSuperLigAtual = itSuperAtual->PontosLigados.begin();

        while (ItSuperLigAtual != itSuperAtual->PontosLigados.end())           //--- Enquanto houver pontos ligados ao ponto atual da supeficie
        {
          Ponto PLigado((*ItSuperLigAtual).PontoLigado);

          if (PLigado != *PPosterior && PLigado != *PAnterior)   //--- Estas são arestas do bordo, não devem ser excluídas
          {
            //--- CORRIGIR ESTE COMENTARIO  <<<=====
            //--- Se for fronteira: 
            //--- Se os bordos foram definidos girando para a esquerda e o ponto estiver a direita ou
            //--- os bordos foram definidos para a direita e o ponto estiver a esquerda ele esta fora do polígono que 
            //--- define a superficie
            //--- Se os bordos foram definidos para a esquerda área fica < 0, caso contrario ele fica > 0. Se a  a área do triangulo formado pelos tres pontos é negativa
            //--- o ponto está à esquerda, caso contrario ele está à direita;
            //--- Se for poligono é o contrário

            if ((Area > 0.0 && ((itSuperPosterior->PontoLigado.fi > itSuperAnterior->PontoLigado.fi && PLigado.fi < itSuperPosterior->PontoLigado.fi && PLigado.fi > itSuperAnterior->PontoLigado.fi) ||
                (itSuperPosterior->PontoLigado.fi < itSuperAnterior->PontoLigado.fi && !(PLigado.fi > itSuperPosterior->PontoLigado.fi && PLigado.fi < itSuperAnterior->PontoLigado.fi)))) ||
                (Area < 0.0 && ((itSuperPosterior->PontoLigado.fi < itSuperAnterior->PontoLigado.fi && PLigado.fi > itSuperPosterior->PontoLigado.fi && PLigado.fi < itSuperAnterior->PontoLigado.fi) ||
                (itSuperPosterior->PontoLigado.fi > itSuperAnterior->PontoLigado.fi && !(PLigado.fi < itSuperPosterior->PontoLigado.fi && PLigado.fi > itSuperAnterior->PontoLigado.fi)))))
            {
              if (Tipo == TIPOFRONTEIRA)
              {
                RemoveOutraPonta(ItSuperLigAtual, *PAtual);                         //--- Remove a ligação com o outro ponto

                CDeltaSuperficie* pCDeltaSuperficie(const_cast<CDeltaSuperficie*>(&*itSuperAtual));
                pCDeltaSuperficie->PontosLigados.erase(ItSuperLigAtual++);          //--- Remove a ligação com este ponto 
              }
              else ItSuperLigAtual++;
            }
            else
            {
              if (Tipo != TIPOFRONTEIRA)   //--- Tipo == TIPO_POLIGONO
              {
                RemoveOutraPonta(ItSuperLigAtual, *PAtual);                         //--- Remove a ligação com o outro ponto

                CDeltaSuperficie* pCDeltaSuperficie(const_cast<CDeltaSuperficie*>(&*itSuperAtual));
                pCDeltaSuperficie->PontosLigados.erase(ItSuperLigAtual++);          //--- Remove a ligação com este ponto        
              }
              else ItSuperLigAtual++;        //--- Não Removeu, incrementa a sup ligada
            }
          }
          else ItSuperLigAtual++;     //--- Não Removeu, incrementa a sup ligada    
        }
      }
      else
      {
   //    ASSERT(false);
      }

      PAnterior = PAtual;

    } while (*(++itDeltaSup) != *(itLPontos->rbegin()));
  }
  return OK;
}

bool CSuperficie::RemoveOutraPonta(const ittysetArestas ItPontoAtual, const Ponto& PCentral)
{
  //ItSSuperficie ItSOutraDeltaSup = pSuperficieAtual->find(ItPontoAtual->PontoLigado);     //--- TEM Q ACHAR!!!!!
  ItSSuperficie ItSOutraDeltaSup(Find(ItPontoAtual->PontoLigado,1E-3));

  if (ItSOutraDeltaSup == pSuperficieAtual->end())
  {
    AfxMessageBox("Erro irrecuperável - Contacte o suporte - ERRSUP004");
    ASSERT(0);
  }
  //--- Busca o ponto 

  ittysetArestas ItPontoOutroAtual(ItSOutraDeltaSup->PontosLigados.begin());

  if (ItPontoOutroAtual == ItSOutraDeltaSup->PontosLigados.end())
  {
    //--- As vezes não acha, testar como comentario 02072020
   // AfxMessageBox("Erro irrecuperável - Contacte o suporte - ERRSUP005");

   // ASSERT(0);
    
    //return false;
    return true;
  }

  while(!PontoXYIgual(ItPontoOutroAtual->PontoLigado, PCentral))
  //while (!(ItPontoOutroAtual->PontoLigado == PCentral))
  {
    ItPontoOutroAtual++;     //-- TEM Q ACHAR!!!
    if (ItPontoOutroAtual == ItSOutraDeltaSup->PontosLigados.end())
    //  if(!PontoXYIgual(ItPontoOutroAtual->PontoLigado, PCentral))
    {
      //AfxMessageBox("Erro irrecuperável - Contacte o suporte - ERRSUP006");
     
	   // ASSERT(0);

      //return false;

     return true;
    }
  }

  CDeltaSuperficie* pCDeltaSuperficie(const_cast<CDeltaSuperficie*>(&*ItSOutraDeltaSup));
  pCDeltaSuperficie->PontosLigados.erase(ItPontoOutroAtual);      //--- Deleta a aresta no outro ponto, 

  return true;
}

void CSuperficie::ForcarAresta(const Ponto& P1, const Ponto& P2)
{
  //--- força uma aresta entre 2 pontos,excluindo todas as arestas que interceptam esta.

  ItSSuperficie ItSSupAtual(pSuperficieAtual->begin());

  //--- Para todos os pontos da superfície:
  //--- Varre as arestas deste ponto, se alguma delas intercerptar a reta remove-a. 

  for (register size_t QuanPontos = pSuperficieAtual->size(); QuanPontos; QuanPontos--, ItSSupAtual++)
  {
    Ponto PCentral = ItSSupAtual->PegaPCentral();

    if (PCentral == P1 || PCentral == P2) continue;   //--- ponto pertence a aresta a forçar, pula 

    //--- Para todos os vertices que estão ligados ao ponto

    register ittysetArestas ItPontoAtual = ItSSupAtual->PontosLigados.begin();

    while (ItPontoAtual != ItSSupAtual->PontosLigados.end())           //--- Enquanto houver pontos ligados ao ponto atual da supeficie
    {
      //--- Se achou a interseção 

      Ponto PontoLigado = ItPontoAtual->PontoLigado;

      if (PontoLigado != P1 && PontoLigado != P2)    //--- ponto pertence a aresta a forçar, pula 
      {
        if (CLPontos::CalculaIntersecao(P1, P2, PCentral, PontoLigado, Ponto()))
        {
          //-- Achou uma aresta que cruza a aresta dada (P1,P2), então remove-a  

          RemoveOutraPonta(ItPontoAtual, PCentral);                     //--- Remove a ligação com o outro ponto

          CDeltaSuperficie* pCDeltaSuperficieAtual(const_cast<CDeltaSuperficie*>(&*ItSSupAtual));
          pCDeltaSuperficieAtual->PontosLigados.erase(ItPontoAtual++);          //--- Remove a ligação com este ponto 

          Alterada = true;              //--- Indica que a superficie foi alterada
        }
        else ItPontoAtual++;
      }
      else ItPontoAtual++;
    }
  }

  //--- Insere a aresta forçada nos pontos
  //--- Verifica se a aresta já existe, se não existir insere-a.

  CDeltaSuperficie D1(P1), D2(P2);
  ItSSupAtual = pSuperficieAtual->find(D1);

  if (ItSSupAtual != pSuperficieAtual->end())
  {
    ItSSuperficie SuperficiePosterior(pSuperficieAtual->find(D2));

    if (SuperficiePosterior != pSuperficieAtual->end())
    {
      register ittysetArestas ItPontoAtual = (*ItSSupAtual).PontosLigados.begin();

      while (ItPontoAtual != ItSSupAtual->PontosLigados.end())
      {
        Ponto PontoLigado = ItPontoAtual->PontoLigado;

        if (PontoLigado == P2) break;    //--- Achou, aresta já existe

        ItPontoAtual++;
      }

      if (ItPontoAtual == (*ItSSupAtual).PontosLigados.end())         //--- Se não achou a aresta, insere-a
      {
        //  ItSSupAtual->PontosLigados.push_back(&(*SuperficiePosterior));
        CDeltaSuperficie* pCDeltaSuperficieAtual(const_cast<CDeltaSuperficie*>(&*ItSSupAtual));
        pCDeltaSuperficieAtual->PontosLigados.insert(SuperficiePosterior->PegaPCentral());

        //SuperficiePosterior->PontosLigados.push_back(&(*ItSSupAtual));          //--- Se não achou em uma  com certeza não está na outra tb, insere na outra direto, sem verificar
        CDeltaSuperficie* pCDeltaSuperficiePosterior(const_cast<CDeltaSuperficie*>(&*SuperficiePosterior));
        pCDeltaSuperficiePosterior->PontosLigados.insert(ItSSupAtual->PegaPCentral());   //--- Se não achou em uma  om certeza não está na outra tb, insere na outra direto, sem verificar
      }
      else
      {
        ASSERT(0);
      }
    }
  }
}

bool CSuperficie::DefineBordos()
{
  register ItLLDeltaSup itLListaRestricoesAtual(ListaRestricoes.begin());

  //--- Varre a lista dos bordos e retira todas as arestas que interceptam suas arestas.

  while (itLListaRestricoesAtual != ListaRestricoes.end())             //--- enquanto não acabarem as restriçoes
  {
    RetiraArestasInterceptantes(itLListaRestricoesAtual);
    itLListaRestricoesAtual++;
  }

  return Alterada;
}

void CSuperficie::RetiraArestasInterceptantes(const ItLLDeltaSup itLDeltaSuperficie)
{
  register ItLDeltaSup itLDeltaSupAnte(itLDeltaSuperficie->begin()),
    itLDeltaSupAtual(itLDeltaSupAnte);                  //--- Cada acidente topográfico tem no mínimo 2 pontos, (verificado na criação.)

  //--- Verre a lista e retira todas as arestas que interceptam suas arestas. 

  while (++itLDeltaSupAtual != itLDeltaSuperficie->end())
  {
    ForcarAresta(itLDeltaSupAnte->PCentral, itLDeltaSupAtual->PCentral);
    itLDeltaSupAnte = itLDeltaSupAtual;
  }
}

inline double CSuperficie::DefineAzimute(double Deltax, double* Angulo)
{
  if (*Angulo < 0.0)
  {
    if (Deltax > 0.0) *Angulo += M_PI;
    else *Angulo += M_2_PI;
  }
  else if (Deltax < 0.0) *Angulo = fabs(*Angulo + M_PI);

  return *Angulo;
}

bool CSuperficie::RotacionaPlano(double VUx, double VUy, double Angulo)
{
  //--- Verifica se o angulo resultante é menor que 0.99 graus, se for o angulo resultante passa a ser 0. As rotações
  //--- são feitas apenas em angulos inteiros   ---*/

  AnguloDeRotacao += Angulo;

  if (fabs(AnguloDeRotacao - Matem::M_2_PI) < Matem::UmGrau_Rad) AnguloDeRotacao = 0.0;

  AngRotacaoBordos = AnguloDeRotacao;

  if (fabs(AnguloDeRotacao) < .95 * Matem::M_PI_DIV_180) Angulo = -AnguloDeRotacao;

  Rotacionada = AnguloDeRotacao != 0.0;

  AngRotacaoTopInt = AnguloDeRotacao * 180.0 / Mat.M_PI + .5;
  if (AngRotacaoTopInt > 0) AngRotacaoTopInt = 360 - AngRotacaoTopInt;

  /*---   rotaciona os pontos conforme a matriz de rotação :   ----

  x’ = x . cos (Fi)  -  y . sin(Fi )

  y’ = x . sin (Fi)  +  y . cos(Fi )

  O vetor unitário de rotação é : VU = (cos(Fi),Sin(Fi))

  //--- Esta matriz atende a disposição dos eixos do vídeo que é :

  /*
  o-------> x
  |
  |
  |
  |
  V
  y
  */

  CDeltaSuperficie DeltaSupAtualRotacionada;
  ItSSuperficie itDeltaSupAtual, itDeltaRotacionadaInserida;

  //--- Tomou-se todo o cuidado para evitar que seja preciso que os 2 sets, o rotacionado e o não rotacionado fiquem na memória. mesmo que por um instante.
  //--- Os pontos do set não rotacionado são removidos assim que são rotacionados. 
  //--- Para isso é é preciso:
  //--- 1 - rotacionar o ponto e inserí-lo  no set rotacionado. 
  //--- 2 - Passar a lista de pontos ligados no ponto que acabou de ser rotacionado, rotacionando-os.
  //--- 3 - Remover o ponto da superficie atual, incluindo a remoção da lista de pontos ligados

  while (pSuperficieAtual->begin() != pSuperficieAtual->end())
  {
    itDeltaSupAtual = pSuperficieAtual->begin();                                    //--- Pega o ponto na superficie atual (não rotacionada) 
    DeltaSupAtualRotacionada = *itDeltaSupAtual;                                    //--- DeltaSupTemp conterá a superfície rotacionada
    DeltaSupAtualRotacionada.PontosLigados.clear();                                 //--- Limpa os pontos ligados, na nova superficie serão rotacionados
    DeltaSupAtualRotacionada.PegaPCentralNC().RotacionaPlano(VUx, VUy, Angulo);  		//--- Rotaciona o ponto central

    //--- Insere o ponto rotacionado na nova superficie, caso ele não exista, se ja existir pega o iterador
    //--- Para inserir os pontos ligados

    itDeltaRotacionadaInserida = pOutraSuperficie->find(DeltaSupAtualRotacionada);        //--- Verifica se o ponto já existe (já foi inserido) na superficie rotacionada
    if (itDeltaRotacionadaInserida == pOutraSuperficie->end())
    {
      PaircitbooSSuperficie PairITBoolSuperficie(pOutraSuperficie->insert(DeltaSupAtualRotacionada));
      if (PairITBoolSuperficie.second == false)
      {
        AfxMessageBox("Erro irrecuperável - Contacte o suporte - ERRSUP001");
        return false;
      }
      itDeltaRotacionadaInserida = PairITBoolSuperficie.first;
    }
    else   //--- O Ponto já foi inserido como ponto ligado. Como ponto ligado as balizas do ponto estão todas false, atualiza-as
    {
      CDeltaSuperficie* pDeltaRotacionadaInserida(const_cast<CDeltaSuperficie*>(&*itDeltaRotacionadaInserida));

      //--- O ponto foi inserido como ponto ligado, acerta o i,S e o fi bem como outros dados que no ponto ligado não tem

      pDeltaRotacionadaInserida->PCentral.i = DeltaSupAtualRotacionada.PCentral.i;
      pDeltaRotacionadaInserida->PCentral.S = DeltaSupAtualRotacionada.PCentral.S;
      pDeltaRotacionadaInserida->PCentral.fi = DeltaSupAtualRotacionada.PCentral.fi;
      pDeltaRotacionadaInserida->EBordo = DeltaSupAtualRotacionada.EBordo;
      pDeltaRotacionadaInserida->ERestricao = DeltaSupAtualRotacionada.ERestricao;
      pDeltaRotacionadaInserida->Status = DeltaSupAtualRotacionada.Status;
      pDeltaRotacionadaInserida->TipoObjeto = DeltaSupAtualRotacionada.TipoObjeto;
      pDeltaRotacionadaInserida->PCentral.Inserido = DeltaSupAtualRotacionada.PCentral.Inserido;
    }

    //--- rotaciona os pontos ligados

    for (ittysetArestas itpSuperLigada = itDeltaSupAtual->PontosLigados.begin(); itpSuperLigada != itDeltaSupAtual->PontosLigados.end(); itpSuperLigada++)     //--- rotaciona a lista de pontos ligados
    {
      //--- rotaciona o ponto ligado e o insere na lista de pontos ligados do ponto atual

      Ponto PLigadoRotacionado(itpSuperLigada->PontoLigado);
      PLigadoRotacionado.RotacionaPlano(VUx, VUy, Angulo);

      //--- faz a ligação (cria a aresta do ponto atual com o ponto ligado)
      //--- O angulo do ponto ligado reíproco tem que ser ajustado pois o sentido é o invertido 

      CDeltaSuperficie* pCDeltaSuperficieRotacionadaInserida(const_cast<CDeltaSuperficie*>(&*itDeltaRotacionadaInserida));     //--- Para remover o const do set
      if (pCDeltaSuperficieRotacionadaInserida->PontosLigados.insert(PLigadoRotacionado).second == false)       //--- Faz a ligação (cria a aresta com o ponto adjacente)
      {
        //AfxMessageBox("Erro irrecuperável - Contacte o suporte - ERRSUP002");
        //return false;
      }

      //--- Procura o ponto ligado ja rotacionado no novo set (o rotacionado) se não achar insere o ponto.
      //--- ItSuperfRotacionada sai apontando para o ponto ligado rotacionado na superficie rotacionada

      ItSSuperficie ItSuperfRotacionada(pOutraSuperficie->find(PLigadoRotacionado));    //---Se o ponto já existe na nova superficie
      if (ItSuperfRotacionada == pOutraSuperficie->end())
      {
        ItSSuperficie itPligadoSupAtual(pSuperficieAtual->find(itpSuperLigada->PontoLigado));   //--- Para pegar a descrição e o número do ponto         if(itPligadoSupAtual != pSuperficieAtual->end())

        PLigadoRotacionado.i = itPligadoSupAtual->PegaPCentral().i;
        PLigadoRotacionado.Descricao = itPligadoSupAtual->PegaPCentral().Descricao;
        PaircitbooSSuperficie PairItBollSSuperficie(pOutraSuperficie->insert(PLigadoRotacionado));  //--- Insere o ponto no novo set
        if (PairItBollSSuperficie.second == false)
        {
          AfxMessageBox("Erro irrecuperável - Contacte o suporte - ERRSUP022");
          return false;
        }
        ItSuperfRotacionada = PairItBollSSuperficie.first;
      }

      //--- Faz a ligação reciproca (do ponto ligado rotacionado para o ponto central atual)
      //--- Acerta o angulo que agora esta no sentido inverso

      DeltaSupAtualRotacionada.PegaPCentralNC().fi = PLigadoRotacionado.fi + M_PI_2;
      if (DeltaSupAtualRotacionada.PegaPCentral().fi > M_2_PI) DeltaSupAtualRotacionada.PegaPCentralNC().fi -= M_2_PI;

      CDeltaSuperficie* pCDeltaSuperficieRotacionada(const_cast<CDeltaSuperficie*>(&*ItSuperfRotacionada));  //--- Para remover o const do set
      if (pCDeltaSuperficieRotacionada->PontosLigados.insert(DeltaSupAtualRotacionada.PegaPCentral()).second == false)
      {
        std::strstream strMensagem;
        strMensagem << "Erro de inserção 3" << std::ends;

        AfxMessageBox(strMensagem.str());

        ASSERT(0);
      }

      RemoveOutraPonta(itpSuperLigada, itDeltaSupAtual->PegaPCentral());
    }

    pSuperficieAtual->erase(itDeltaSupAtual);      //--- Remove o ponto da superficie atual (ele acabou se ser inserido na outra)  
  }

  //--- troca as superfícies, a atual agora foi rotacionada e está na outra

  if (pSuperficieAtual->size() != 0)
  {
    AfxMessageBox("Erro irrecuperável - Contacte o suporte - ERRSUP003");
    return false;
  }

  SSuperficie* pSuperficieTemp(pSuperficieAtual);
  pSuperficieAtual = pOutraSuperficie;
  pOutraSuperficie = pSuperficieTemp;
   
  AcertaSenoCoseno();

  RotacionarCN(VUx, VUy, Angulo);
  RotacionaEquipDrenagem(VUx, VUy, Angulo);
  RotacionaPontosInserDesabil(VUx, VUy, Angulo);
  
  RedefineLimites();

  return true;
}
//---   O LUGAR DESTE MÉTODO É NA CLASSE SUPERFÍCIE!!!

void CSuperficie::ExcluiDeltaSuperficie(ItSSuperficie itDeltaSupAExcluir)
{
  ItSSuperficie itSuperfAExcluir(pSuperficieAtual->find(*itDeltaSupAExcluir));

  if (itDeltaSupAExcluir != pSuperficieAtual->end())
  {
    //--- Remove as arestas dos pontos ligados a este ponto

    //  if(dialogar.DoModal(5) == IDOK)
    {
      register ittysetArestas ItSuperAtual(itDeltaSupAExcluir->PontosLigados.begin()), itSuperligadaReciproca;

      //--- Remove todas as arestas dos pontos ligados com este ponto
      //--- Para isso varre a lista de pontos ligados do ponto que será excluido (itSuperfAExcluir)
      //--- Acha em cada lista de pontos ligados deste ponto o ponto que sera excluído e exclui-o da lista de pontos ligados. entendeu???

      while (ItSuperAtual != itDeltaSupAExcluir->PontosLigados.end())      //--- Enquanto houver pontos ligados ao ponto atual da supeficie
      {
        //--- Acha no ponto ligado a ligação recíproca (Para o ponto que está sendo excluido) e a exclui

        ItSSuperficie itSuperPonLigado(pSuperficieAtual->find(ItSuperAtual->PontoLigado));

        if (itSuperPonLigado != pSuperficieAtual->end())
        {
          ittysetArestas itReciproca(itSuperPonLigado->PontosLigados.find(itSuperfAExcluir->PCentral));

          if (itReciproca != itSuperPonLigado->PontosLigados.end())
          {
            tysetArestas& ppp(const_cast<tysetArestas&>(itSuperPonLigado->PontosLigados));
            ppp.erase(itReciproca);
          }
          else
          {
            AfxMessageBox("Erro Irrecuperável.Contacte o suporte: ERRSUP-010");
          }
        }
        else
        {
          AfxMessageBox("Erro Irrecuperável.Contacte o suporte: ERRSUP-011");;
        }
      }

      //--- Finalmente, remove o ponto da superficie

      pSuperficieAtual->erase(itDeltaSupAExcluir);
    }
  }
}

void CSuperficie::MoveDeltaSuperficie(ItSSuperficie ItDeltaSupAnterior, const CDeltaSuperficie& DeltaSupAtual)
{
  /*
  if( 0 && ItDeltaSupAnterior != pSuperficieAtual->end())
  {
  register ItLpSuperficiesLigadas ItSuperAtual = ItDeltaSupAnterior->PontosLigados.begin(),itSuperligadaReciproca;

  ItSSuperficie ItDeltaSuperMovida = (pSuperficieAtual->insert(DeltaSupAtual)).first;

  //--- Move todas as arestas dos pontos ligados com este ponto para o novo ponto
  //--- Para isso varre a lista de pontos ligados do ponto que será excluido (itSuperfAExcluir)
  //--- Acha em cada lista de pontos ligados deste ponto o ponto que sera Movido (itSuperfAExcluir),
  //--- substitui esta rentrada da lista pelo novo ponto, que é o ponto movido

  while(ItSuperAtual != ItDeltaSupAnterior->PontosLigados.end())      //--- Enquanto houver pontos ligados ao ponto atual da supeficie
  {
  //	(*ItDeltaSuperMovida).PontosLigados.insert(ItSuperAtual);

  //--- Acha no ponto ligado a ligação recíproca (Para o ponto que está sendo Movido) e a exclui

  ItLpSuperficiesLigadas itSuperligadaReciproca(std::find(((CDeltaSuperficie*)ItSuperAtual->PontoLigado)->PontosLigados.begin(),
  ((CDeltaSuperficie*)ItSuperAtual->PontoLigado)->PontosLigados.end(),
  &*ItDeltaSupAnterior));

  if (itSuperligadaReciproca != ItDeltaSupAnterior->PontosLigados.end())
  *itSuperligadaReciproca = &(*ItDeltaSuperMovida);

  ItSuperAtual++;
  }

  //--- Finalmente, remove o ponto anterior da superficie

  pSuperficieAtual->erase(ItDeltaSupAnterior);
  }
  */
}

ItSSuperficie CSuperficie::PegaPontoMaisProximo(Ponto PontoAPegar, double Tolerancia)
{
  if (EstaRotacionada()) PontoAPegar.RotacionaPlano(Seno, Coseno, AnguloDeRotacao);

  PontoAPegar.x += Tolerancia;

  ItSSuperficie itDSupFinal(pSuperficieAtual->upper_bound(PontoAPegar)), itDSupAtual, itPonMaisProximo;

  PontoAPegar.x -= Tolerancia * 2.0;
  itDSupAtual = pSuperficieAtual->upper_bound(PontoAPegar);         //--- Pega o primeiro na tolerancia    
  PontoAPegar.x += Tolerancia;                                        //--- Restaura o valor original

  if (itDSupFinal != pSuperficieAtual->end()) Ponto PAtual(itDSupFinal->PegaPCentral());
  if (itDSupAtual != pSuperficieAtual->end()) Ponto PAtual(itDSupAtual->PegaPCentral());

  double MenorDistancia(INFINITO);

  //--- Separa o ponto que está mais próximo do mouse 

  while (itDSupAtual != itDSupFinal)
  {
    Ponto PAtual(itDSupAtual->PegaPCentral());
    double DistanciaAtual = hypot(PAtual.x - PontoAPegar.x, PAtual.y - PontoAPegar.y);
    if (DistanciaAtual < MenorDistancia)
    {
      MenorDistancia = DistanciaAtual;
      itPonMaisProximo = itDSupAtual;
    }

    itDSupAtual++;
  }

  //--- Se o ponto separado estiver dentro da tolerancia retorna-o 
  //--- caso contrário não há ponto mais próximo dentro da tolerancia

  return MenorDistancia < Tolerancia ? itPonMaisProximo : pSuperficieAtual->end();
}

bool CSuperficie::InterpolarCotas(ItSSuperficie DeltaSupInicial, ItSSuperficie DeltaSupFinal, CProgressCtrl& Prog,SSetPontos& PontosProblema, double Equidis,int TipoSuperf)
{
  register ItSSuperficie DeltaSupAtual(DeltaSupInicial);
  const Ponto* pPontoAtual;

  InicioInterpolacao = true;

  Prog.SetRange(0, (short)this->pSuperficieAtual->size());

  for (; DeltaSupAtual != DeltaSupFinal; DeltaSupAtual++)
  {
    CDeltaSuperficie* pCDeltaSupAtual(const_cast<CDeltaSuperficie*>(&*DeltaSupAtual));

    for (register ittysetArestas PLigAtual = (*DeltaSupAtual).PontosLigados.begin(); PLigAtual != DeltaSupAtual->PontosLigados.end(); PLigAtual++)
    {
      pPontoAtual = &PLigAtual->PontoLigado;
      if (!pPontoAtual->Baliza)
      {
        InterpolaCotas(*pCDeltaSupAtual, PLigAtual, Equidis);
        if (ErroInterpolacao != 0)
        {
          PontosProblema.insert(pCDeltaSupAtual->PCentral);
              
          return false;
        }
      }
    }

    pCDeltaSupAtual->PegaPCentralNC().Baliza = true;         //--- sinaliza q já foi interpolado

    Prog.StepIt();
  }

  Prog.SetRange(0, 4);
  Prog.SetPos(1);
  Prog.SetStep(1);

  if (LigaCNAbertas(TipoSuperf))
  {
    Prog.StepIt();

    if (LigaCurvasFechadas(TipoSuperf))
    {
      Prog.StepIt();

      //  if(LigaCNRestricoesFechadas())  //--- Aguardar: não esta usando mais so pode ser usado quado não tem triangulação dentro da area de restrição fechada
      //--- A Fazer: É preciso tirar a triangulação de dentro da área    
      {
        Prog.StepIt();
        LimpaBalizasCNs();

        return true;
      }
    }
  }

 // return false;
 return true;
}

void CSuperficie::InterpolaCotas(CDeltaSuperficie& DS1, ittysetArestas itPLigado, double EquiCotas)
{
  Ponto P1(DS1.PegaPCentral()), P2(itPLigado->PontoLigado);

  double Int;
  double Frac(modf(P1.z / EquiCotas, &Int) *  EquiCotas);
  double Resp(EquiCotas - Frac);
  static double DeltaxMaior, DeltaxMenor, DeltaxMinimo;

  if (InicioInterpolacao == true)
  {
    DeltaxMaior = 0.999 * EquiCotas;
    DeltaxMenor = 0.001 * EquiCotas;
    DeltaxMinimo = 0.002 * EquiCotas;

    InicioInterpolacao = false;
  }

  if (Resp > DeltaxMaior) P1.z -= DeltaxMinimo;
  else if (Resp < DeltaxMenor) P1.z += DeltaxMinimo;

  Frac = modf(P2.z / EquiCotas, &Int) *  EquiCotas;
  Resp = EquiCotas - Frac;
  if (Resp > DeltaxMaior) P2.z -= DeltaxMinimo;
  else if (Resp < DeltaxMenor) P2.z += DeltaxMinimo;

  double Deltax(P2.x - P1.x), Deltay(P2.y - P1.y), Deltaz(P2.z - P1.z);

  if (fabs(Deltaz) < 1e-5) return;

  if (0 && Deltaz > 100.00)    //---- Pode ocorrer, não vi muito a fundo....
  {
   ErroInterpolacao = 1;
    return;
  }

  register double Dist(hypot(Deltax, Deltay));
  double Tan((P2.z - P1.z) / (Dist + 1e-6)), CotaAtual((Deltaz > 0.0 ? ((int)(P1.z / EquiCotas)) + 1.0 : (int)(P1.z / EquiCotas)) * EquiCotas);
  register double DistAtual((CotaAtual - P1.z) / Tan);
  double DeltaDist(EquiCotas / Tan);

  if (fabs(DeltaDist) < 0.00001) return;

  Ponto PAtual;
  CAresta*pPLigado(const_cast<CAresta*>(&*itPLigado));
  pPLigado->PontosInterpolacao.clear();

  while (DistAtual < Dist)
  {
    PAtual.x = Deltax * (DistAtual / Dist) + P1.x;
    PAtual.y = Deltay * (DistAtual / Dist) + P1.y;
    PAtual.z = CotaAtual;
    DistAtual += fabs(DeltaDist);
    Tan > 0.0 ? CotaAtual += EquiCotas : CotaAtual -= EquiCotas;

    pPLigado->PontosInterpolacao.insert(PAtual);
  }

  return;
}
bool CSuperficie::LigaCNAbertas(int TipoSuperf)
{
  /*----  Algoritmo de ligação dos pontos das CNs abertas ---

  para todos os pontos do bordo
  enquanto houver cotas neste lado do bordo
  Pega Primeiro ponto da cota
  faça
  inclui na lista da CN o ponto da cota
  Pega ponto saida nos 2 triangulos adjacentes
  enquanto ponto saida não estiver em um bordo
  inclui na lista das CN a lista da CN Atual
  fim enquanto
  fim para

  ------------------------------------------------*/
  LimpaBalizasSuperficie();
  LPontosCN.clear();

  //if(ListaBordos.size() == 0 || ListaBordos.begin()->size() < 3) return false;    //--- quantidade mínima de pontos no bordo
  /*
  itmmapitLLDeltaSup it(MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA));
  if(it != MMapItLLDeltaSup.end())
  {
  Inicio = it->second;
  Fim = it->second;
  ++Fim;
  }
  else Inicio = (ListaPontos.end());
  }
  */

  itmmapitLLDeltaSup it(MMapItLLDeltaSup.find((TipoSuperf == 4 || TipoSuperf == 1 || TipoSuperf == 3) ? CAciTopografico::FRONTEIRA_TERRAP : CAciTopografico::FRONTEIRA));   //--- Se for Drones So interpola as CNs dentro da terraplanagem
  if (it == MMapItLLDeltaSup.end() || it->second->size() < 3 || !it->second->begin()->PCentral.Compara2D(it->second->rbegin()->PCentral))                                  //*it->second->begin() != *it->second->rbegin()) 
  {
    return false;
  }

  ItCLDeltaSup itBordoPosterior(it->second->begin()),
    itBordoAtual(itBordoPosterior);
  //  (ListaBordos.begin())->begin()); 
  //	ItLDeltaSup itBordoPosterior((ListaBordos.begin())->begin()); 
  //	ItLDeltaSup itBordoAtual(itBordoPosterior);

  itBordoPosterior++;

  while (itBordoPosterior != it->second->end())    //--- Para todos os pontos do bordo
  {
    // A lista de bordos é apenas uma lista de pontos,portanto é preciso achar os pontos do bordo na superficie e 
    // trabalhar com as delta-superficies.

//    register ItSSuperficie itSupPosterior(pSuperficieAtual->find(*itBordoPosterior)),  // Acha o ponto de bordo anterior na superficie
  //                         itSupAtual(pSuperficieAtual->find(*itBordoAtual));          // Acha o ponto de bordo atual na superficie.

    ItSSuperficie itSupPosterior(Find(itBordoPosterior->PCentral,1E-3)),  // Acha o ponto de bordo anterior na superficie
    itSupAtual(Find(itBordoAtual->PCentral,1E-3));          // Acha o ponto de bordo atual na superficie.

    if (itSupAtual == pSuperficieAtual->end())
    {
      std::stringstream strstTemp;

      strstTemp << std::fixed << std::setprecision(3);
      strstTemp << "\r\nCoord N= " << itBordoAtual->PegaPCentral().y << " Coord E= " << itBordoAtual->PegaPCentral().x << " Cota = " << itBordoAtual->PegaPCentral().z << std::ends;

      monolog.mensagem(36, strstTemp.str().c_str());       //--- Encontrado ponto de bordo que não pertence a superfície (O Arquivo de bordos é de outra superficie)

      return false;
    }

    //--- Pega o ponto ligado referente ao ponto posterior, pois as cotas estão nos pontos ligados. 

    ittysetArestas itSupLigada(std::find(itSupAtual->PontosLigados.begin(), itSupAtual->PontosLigados.end(), itBordoPosterior->PegaPCentral()));

    if (itSupLigada != itSupAtual->PontosLigados.end())    //--- Tem q achar!!!
    {
      //--- enquanto houver cotas neste lado do bordo, liga a CN

      for (itsetPontosS itPonInter = itSupLigada->PontosInterpolacao.begin(); itPonInter != itSupLigada->PontosInterpolacao.end(); itPonInter++)
      {
        if (itPonInter->Baliza == false) Ligacn(itSupPosterior, itSupAtual, itPonInter);
      }
    }
    itBordoAtual++;
    itBordoPosterior++;
  }

  return true;
}

void CSuperficie::Ligacn(ItSSuperficie itSupPosterior, ItSSuperficie itSupAtual, itsetPontosS itPonInter, bool Fechada)
{
  static int count(0);

  LPontosCN.push_back(LPontos(1, *itPonInter));
  LPontos& LCNAtual(LPontosCN.back());
  (const_cast<Ponto*>(&*itPonInter))->Baliza = true;
  ittysetArestas itSA(AchaOutraPonta(itSupPosterior->PCentral, itSupAtual->PCentral));
  itsetPontosS itSPS(itSA->AchaPontoNaCota(itPonInter->z));
  if (itSPS == itSA->PontosInterpolacao.end()) return;
  (const_cast<Ponto*>(&*itSPS))->Baliza = true;

  //	(const_cast<Ponto*>(&*((AchaOutraPonta(itSupPosterior->PCentral,itSupAtual->PCentral))->AchaPontoNaCota(itPonInter->z))))->Baliza = true;

  itsetPontosS iTPontoCota;
  int Achou, Lado;

  ItSSuperficie  itSupAtuRasc(itSupAtual), itSupPostRasc(itSupPosterior);

  do     //--- Faça enquanto não achar a saída da CN, a saida será obrigatoriamente em um bordo
  {
   
    ittysetArestas it(AchaSaidaCN(itSupAtuRasc, itSupPostRasc, itPonInter->z, Achou, Lado, iTPontoCota));  //--- Pega aresta oposta 

    if (Achou == 1)          //--- tem q achar!!
    {
      LCNAtual.push_back(*iTPontoCota);   //--- Tem q achar, se não achar é erro!!

      //--- Passa o lado atual para o lado em que a curva de nível saiu.

      if (Lado == 0)
      {
        itSupAtuRasc = itSupPostRasc;
        itSupPostRasc = pSuperficieAtual->find(it->PontoLigado);
      }
      else itSupPostRasc = pSuperficieAtual->find(it->PontoLigado);
    }
  } while (Achou == 1);

  if (Fechada) LCNAtual.push_back(*itPonInter);
}

bool CSuperficie::LigaCurvasFechadas(int TipoSuperf)
{
  //---  Procura em todos os pontos da superfice por pontos de cota q ainda não foram ligados ( so restaram CNs fechadas)

  int C(0);

  for (register ItSSuperficie itSupAtual = pSuperficieAtual->begin(); itSupAtual != pSuperficieAtual->end(); itSupAtual++)
  {
    //--- Procura em todos os pontos ligados

    for (register ittysetArestas itSupLigadaAtual = itSupAtual->PontosLigados.begin(); itSupLigadaAtual != itSupAtual->PontosLigados.end(); itSupLigadaAtual++)
    {
      //--- Procura em todas as cotas

      for (register itsetPontosS itPontoCota = (*itSupLigadaAtual).PontosInterpolacao.begin(); itPontoCota != (*itSupLigadaAtual).PontosInterpolacao.end(); itPontoCota++)
      {
        //--- Se não foi interpolada interpola-a

        if (itPontoCota->Baliza == false)
        {
          C++;

          ItSSuperficie ItSupLigada = pSuperficieAtual->find(itSupLigadaAtual->PontoLigado);

          Ligacn(ItSupLigada, itSupAtual, itPontoCota, true);
        }
      }
    }
  }

  return true;
}

ittysetArestas CSuperficie::AchaSaidaCN(ItSSuperficie itS1, ItSSuperficie itS2, double Cota, int& Achou, int& LadoSaida, itsetPontosS& ArestaSaida)
{
  //--- Com a inserçao de projetos do autocad começaram a surgir problemas:
  //--- em alguns casos não acha a saida da CN porque como o pontos importados do autocad podem estar muito proximos um do outros
  //--- Muito difícil de rastrear devido a triangulação ser muito complexa por causas dos pontos muitos proximos
  //--- Esperar ocorrer o mesmo erro em uma tringulação mais simples para corrigir
  //--- Porisso a funcionalidade de calcular a superfície com pontos importados do autocad esta suspensa

  ittysetArestas itArestaComum2, itSL(itS1->PontosLigados.begin());
  setPontosS lstPontosArestas;
  double AreaS1S2(CalcAreaReta(itS1->PCentral, itS2->PCentral)), AreaTS1S2(INFINITO);
  Achou = 0;

  //--- Pega todos os pontos com arestas em comum nos 2 triangulos adjacentes
  //--- Tem que achar a saida em um dos triangulos adjacentes

  for (int C = 0; C < 2 && Achou == 0 && itSL != itS1->PontosLigados.end(); C++)   //--- Por segurança ....caso não achar a saida termina com C = 2.
  {
    ittysetArestas itSLAtual;
    do
    {
      lstPontosArestas.clear();
      AreaTS1S2 = INFINITO;

      do
      {
        itSLAtual = itSL;
        itArestaComum2 = std::find(itS2->PontosLigados.begin(), itS2->PontosLigados.end(), *itSL);
        itSL++;

        //-----------------------------------------------------------------------------------------------------
        // - Verifica o caso raro em que existem mais de 1 triangulo oposto por lado:
        // - Separa todas as arestas comums a esta aresta (entre esses 2 pontos) colocando-os num set de pontos
        // - O ponto mais interno é que é o extremo da aresta que deve ser pega.
        // - O ponto mais interno é selecionado pela área do triangulo que ele faz com a aresta atual, quanto menor a área mais interno é o triangulo 

        if (itArestaComum2 != itS2->PontosLigados.end())
        {
          if (AreaTS1S2 == INFINITO)      //--- Primeira vez? salva a área da aresta
          {
            AreaTS1S2 = AreaS1S2 + CalcAreaReta(itS2->PCentral, itArestaComum2->PontoLigado) + CalcAreaReta(itArestaComum2->PontoLigado, itS1->PCentral);
            (const_cast<CAresta*>(&*itArestaComum2))->PontoLigado.i = AreaTS1S2;
          }

          lstPontosArestas.insert(itArestaComum2->PontoLigado);

          while (itSL != itS1->PontosLigados.end())
          {
            ittysetArestas itArestaTemp;

            itArestaTemp = std::find(itS2->PontosLigados.begin(), itS2->PontosLigados.end(), *itSL++);

            if (itArestaTemp != itS2->PontosLigados.end())   //--- Achou mais uma aresta entre estes 2 pontos?
            {
              double AreaTemp = AreaS1S2 + CalcAreaReta(itS2->PCentral, itArestaTemp->PontoLigado) + CalcAreaReta(itArestaTemp->PontoLigado, itS1->PCentral);

              if ((AreaTemp > 0.0 && AreaTS1S2 > 0.0) || (AreaTemp < 0.0 && AreaTS1S2 < 0.0)) //--- se os pontos estão do mesmo lado, insere-o na lista de pontos a escolher
              {
                (const_cast<CAresta*>(&*itArestaTemp))->PontoLigado.i = AreaTemp;
                lstPontosArestas.insert(itArestaTemp->PontoLigado);
              }
            }
          }
        }

      } while (itSL != itS1->PontosLigados.end());

      if (itArestaComum2 != itS2->PontosLigados.end())
      {
        //--- Caso ocorra esta caso raro, o ponto comum válido é o ponto mais interno, pega-o. 

        if (lstPontosArestas.size() > 1)
        {
          itsetPontosS it = PegaMenorAresta(itS1, lstPontosArestas);  //-- Vale a aresta do ponto mais interno, que é a do menor triangulo 
          itSLAtual = std::find(itS1->PontosLigados.begin(), itS1->PontosLigados.end(), *it);

          itArestaComum2 = std::find(itS2->PontosLigados.begin(), itS2->PontosLigados.end(), *it);
        }

        ItSSuperficie itSupAresta(pSuperficieAtual->find(itArestaComum2->PontoLigado));

        itsetPontosS iTPontoCota(itArestaComum2->AchaPontoNaCota(Cota));

        if (iTPontoCota != itArestaComum2->PontosInterpolacao.end())
        {
          if (iTPontoCota->Baliza == false)
          {
            ArestaSaida = iTPontoCota;
            LadoSaida = C;

            //--- Cada Aresta tem sua lista de cotas, é preciso sinalizar nas duas

            (const_cast<Ponto*>(&*iTPontoCota))->Baliza = true;

	          ittysetArestas ISA(AchaOutraPonta(itSLAtual->PontoLigado, itS2->PCentral));

            const CAresta* pOutraAresta((const_cast<CAresta*>(&*((AchaOutraPonta(itSLAtual->PontoLigado, itS2->PCentral))))));
            ItSSetPontos itPontoOutraAresta(pOutraAresta->AchaPontoNaCota(Cota));

            if (itPontoOutraAresta == pOutraAresta->PontosInterpolacao.end())
            {
				      Achou = 1;   //--- Por enquanto mas isto é um erro pois tem q achar, parece um caso especial da triangulação que não foi visto, verificar
				                   //--- NÃO É NÃO ESTA SEM ISSO DESDE 2017 PELO MENOS -- VERIFICAR #CARLOS 
                                   //--- 22-04-2023 É porque As vezes por milésimos de milímetro não acha 
            }
            else
		        {
              (const_cast<Ponto*>(&*itPontoOutraAresta))->Baliza = true;
              Achou = 1;
            }     //--- não tinha antes (não tinha o else)
          }
        }
        else
        {
          itsetPontosS iTPontoCota(itSLAtual->AchaPontoNaCota(Cota));

          if (iTPontoCota != itSLAtual->PontosInterpolacao.end())
          {
            if (iTPontoCota->Baliza == false)
            {
              ArestaSaida = iTPontoCota;

              LadoSaida = C == 0 ? 1 : 0;

              (const_cast<Ponto*>(&*iTPontoCota))->Baliza = true;
              (const_cast<Ponto*>(&*((AchaOutraPonta(itSLAtual->PontoLigado, itS1->PCentral))->AchaPontoNaCota(Cota))))->Baliza = true;
            //  const_cast<Ponto*>(KK)->Baliza = true;

              Achou = 1;
            }
          }
        }
      }

      itSL = itSLAtual;
      itSL++;

    } while (itSL != itS1->PontosLigados.end() && Achou == 0);
  }

  return itArestaComum2;
}

void CSuperficie::RotacionarCN(double Seno, double Cosseno, double Ang)
{
  for(int C = 0 ; C < 2 ; C++)
  {
    LLPontos& LLCN(C == 0 ?LPontosCN : LLPontosCNSimbTerrapl);     

    for (ItLLPontos itLLCN = LLCN.begin(); itLLCN != LLCN.end(); itLLCN++)
    {
      for (register ItLPontos itLCN = itLLCN->begin(); itLCN != itLLCN->end(); itLCN++)
      {
        itLCN->RotacionaPlano(Seno, Cosseno, Ang);
      }
    }
  }
}

itsetPontosS CSuperficie::PegaMenorAresta(ItSSuperficie itSup, setPontosS& pLstArestas)
{
  double MenorArea(INFINITO);
  itsetPontosS itMaisProximo(pLstArestas.end());

  for (itsetPontosS itAtual = pLstArestas.begin(); itAtual != pLstArestas.end(); itAtual++)
  {
    if (fabs(itAtual->i) < fabs(MenorArea))
    {
      itMaisProximo = itAtual;
      MenorArea = itAtual->i;
    }
  }

  return itMaisProximo;
}

/*
const itsetPontosS CSuperficie::PegaMenorAresta(ItSSuperficie itSup,setPontosS& pSetArestas)
{
double MenorDist(INFINITO);
ittysetArestas itPontoMaisProximo;

for(itsetPontosS itAtual = pSetArestas.begin() ; itAtual != pSetArestas.end() ; itAtual++)
{
if(Mat.R_P(itSup->PCentral,itAtual->PontoLigado).y < MenorDist) itPontoMaisProximo = itAtual;
}

return itPontoMaisProximo;
}
*/
//===================================================================================================================================
/*std::map<char,int> mymap;
std::map<char,int>::iterator it,itlow,itup;
std::pair<std::map<char,int>::iterator,std::map<char,int>::iterator> ret;

mymap['a']=50;
mymap['b']=40;
mymap['c']=60;
mymap['d']=80;
mymap['e']=100;

ret = mymap.equal_range(3);

itlow=mymap.lower_bound ('b');  // itlow points to b
itup=mymap.upper_bound ('d');   // itup points to e (not d!)

//  mymap.erase(itlow,itup);        // erases [itlow,itup)

it = mymap.begin();
--it;
*/
/*
//==================================================================================================================================
int CSuperficie::DefineFronteira()
{
//---  A LISTA DE BORDOS PODE SER UMA LISTA COMUM, NÃO PRECISA DER DE DELTA SUPERFICIE  ALTERAR!!!!

//  return OK;

if(ListaBordos.size() == 0)
{
monolog.mensagem(30,"");
return !OK;
}

//--- No final existe apenas uma lista de bordos, se existir mais que uma a superfície não está fechada,
//--- como também se o último ponto não for igual ao primeiro.

if(ListaBordos.size() > 1 || (*(*ListaBordos.begin()).begin()).PCentral != (*(*ListaBordos.begin()).rbegin()).PCentral)
{
std::strstream strTemp;

Ponto& PAtual = (*(*ListaBordos.begin()).rbegin()).PCentral;

strTemp << std::fixed << std::setprecision(3) << "\r\nCoord. N= " << PAtual.x << " Coord. E= " << PAtual.y << std::ends;

monolog.mensagem(31,strTemp.str());

return !OK;
}

//--- Desrotaciona os bordos

if(AngRotacaoBordos != 0.0)
{
for(register ItLLDeltaSup itLBordoAtual = ListaBordos.begin() ; itLBordoAtual != ListaBordos.end() ; itLBordoAtual++)  //--- no final existe apenas uma listq de bordos VERIFICAR!!!!!
{
double Seno(sin(-AngRotacaoBordos)),Coseno(cos(-AngRotacaoBordos));

for(register ItLDeltaSup ItLBordo = itLBordoAtual->begin() ; ItLBordo != itLBordoAtual->end() ;  ItLBordo++)
ItLBordo->RotacionaPcentral(Seno,Coseno);
}
}

for(ItLLDeltaSup itLBordoAtual = ListaBordos.begin() ; itLBordoAtual != ListaBordos.end() ; itLBordoAtual++)  //--- no final existe apenas uma listq de bordos VERIFICAR!!!!!
{
if (itLBordoAtual->size() < 3) continue;      //--- *LBordosAtual é uma lista de listas de bordos

double Area(0.0);
ItLDeltaSup ItLBordo(itLBordoAtual->begin());

register Ponto* PAnterior(&(*ItLBordo++).PCentral),*PAtual;

do
{
PAtual = &(*ItLBordo++).PCentral;

Area += (PAtual->x - PAnterior->x) * ((PAtual->y+PAnterior->y) / 2.0);

PAnterior = PAtual;

}while(ItLBordo != itLBordoAtual->end());

//--- Retira todos as arestas cujos azimutes forem menores que azimute do lado do bordo, independentemente de
//--- ter girado para a direita ou para a esquerda.
//--- Calcula a área do triangulo formado pelos 3 pontos. Se esta área for
//--- positiva o ponto esta a direita, caso contrário o ponto está a esquerda.
//--- Se o bordo girar para a esquerda, retira as arestas a direita
//--- Se o bordo estiver girando para a direita retira as arestas a esquerda.

ItLBordo = itLBordoAtual->begin();
PAnterior = &ItLBordo->PCentral;
ItLBordo++;

do      //--- Enquanto houver pontos de bordo
{
PAtual =  &ItLBordo->PCentral;

//--- Os azimutes estão na superficie, então pe preciso pegar o ponto do bordo na superficie.
//--- Isso não ficou bom, o ideal é q os bordos sejam iteradores para a superficie---===> ALTERAR

ItLDeltaSup gg = ItLBordo;

gg++;

Ponto* PPosterior(&(gg)->PCentral);

ItSSuperficie itSuperAtual(pSuperficieAtual->find(ItLBordo->PCentral));
ittysetArestas itSuperAnterior(itSuperAtual->PontosLigados.end()),
itSuperPosterior(itSuperAtual->PontosLigados.end());

//--- Pega na superfície os pontos que contém os azimutes

for(ittysetArestas ItLSL = itSuperAtual->PontosLigados.begin() ;
ItLSL != itSuperAtual->PontosLigados.end() && (itSuperAnterior == itSuperAtual->PontosLigados.end() || itSuperPosterior == itSuperAtual->PontosLigados.end()) ;
ItLSL++)
{
if (ItLSL->PontoLigado == PAnterior) itSuperAnterior = ItLSL;
if (ItLSL->PontoLigado == PPosterior) itSuperPosterior = ItLSL;
}

if(itSuperAnterior != itSuperAtual->PontosLigados.end() && itSuperPosterior != itSuperAtual->PontosLigados.end())   //--- so por segurança, tem q achar os 2.
{
ittysetArestas ItSuperLigAtual = itSuperAtual->PontosLigados.begin();

while(ItSuperLigAtual != itSuperAtual->PontosLigados.end())           //--- Enquanto houver pontos ligados ao ponto atual da supeficie
{
Ponto PLigado = (*ItSuperLigAtual).PontoLigado;

if (PLigado != *PPosterior && PLigado != *PAnterior)   //--- Estas são arestas do bordo, não devem ser excluídas
{
//--- Se os bordos foram definidos girando para a esquerda e o ponto estiver a direita ou
//--- os bordos foram definidos para a direita e o ponto estiver a esquerda ele esta fora do polígono que
//--- define a superficie
//--- Se os bordos foram definidos para a esquerda área fica < 0, caso contrario ele fica > 0. Se a  a área do triangulo formado pelos tres pontos é negativa
//--- o ponto está à esquerda, caso contrario ele está à direita;

if((Area > 0.0 && PLigado.fi < itSuperPosterior->PontoLigado.fi && PLigado.fi > itSuperAnterior->PontoLigado.fi) ||
(Area < 0.0 && PLigado.fi > itSuperPosterior->PontoLigado.fi && PLigado.fi < itSuperAnterior->PontoLigado.fi))
{
//--- É preciso deletar a aresta no outro ponto antes de deletar deste.
//--- Procura na superficie a deltasuperfície onde este ponto é central.
//--- procura na lista de pontos ligados desta deltasuperficie o ponto central atual.
//--- Exclui este ponto (o ponto central atual) desta lista de pontos ligados

RemoveOutraPonta(ItSuperLigAtual,*PAtual);                         //--- Remove a ligação com o outro ponto

itSuperAtual->PontosLigados.erase(ItSuperLigAtual++);          //--- Remove a ligação com este ponto
}
ItLBordo++;
}
ItSuperLigAtual++;
}
}
PAnterior = PAtual;

}while (*(++ItLBordo) != *(itLBordoAtual->rbegin()));
}
return OK;
}
*/

ittysetArestas CSuperficie::AchaOutraPonta(const Ponto& PontoAtual, const Ponto& PCentral)
{
  //ItSSuperficie ItSOutraDeltaSup(pSuperficieAtual->find(PontoAtual));     //--- TEM Q ACHAR!!!!!

  ItSSuperficie ItSOutraDeltaSup(Find(PontoAtual,1E-3));     //--- TEM Q ACHAR!!!!!

  if (ItSOutraDeltaSup == pSuperficieAtual->end())
  {
    ASSERT(0);
  }
  //--- Faz a busca sequencial===>>> Passar para set

  ittysetArestas ItPontoOutroAtual(ItSOutraDeltaSup->PontosLigados.begin());

  if (ItPontoOutroAtual == ItSOutraDeltaSup->PontosLigados.end())
  {
    ASSERT(0);
  }

  //while (!(ItPontoOutroAtual->PontoLigado == PCentral))
  while (!(ItPontoOutroAtual->PontoLigado.Compara2D(PCentral)))
  {
    ItPontoOutroAtual++;
    if (ItPontoOutroAtual == ItSOutraDeltaSup->PontosLigados.end()) return  ItSOutraDeltaSup->PontosLigados.begin();
  }

  return ItPontoOutroAtual;
}

/*
int CSuperficie::LePontos()
{
tylstNomeArquivos lstNomeArquivos;

if (PegaNomesArquivosTerreno(lstNomeArquivos,NomeProjeto))
{
for(tyitlstNomeArquivos itNomeArqAtual = lstNomeArquivos.begin() ; itNomeArqAtual != lstNomeArquivos.end(); ++itNomeArqAtual)
{
std::ifstream ArqPontos(*itNomeArqAtual);
if(ArqPontos.is_open())
{
unsigned int ContaLinhas(0);
try
{
while(ArqPontos.good())
{
std::string strNome,strBufferx,strBuffery,strBufferz,strDescricao;
static std::string strMarcasComentarios("/!@#$%&*");

++ContaLinhas;
PulaBrancos(ArqPontos);                        //--- Pula os  brancos no início do arquivo
GetLine(ArqPontos, strNome," \t\x0A");       //--- Nome do ponto, descarta

if(strNome.size() == 0)
{
std::getline(ArqPontos,strNome);
continue;
}

if(strMarcasComentarios.find(*strNome.begin()) != std::string::npos)
{
std::getline(ArqPontos,strNome);       //--- Descarta o resto da linha,se size = 0 é porque e linha em branco

continue;
}

PegaProximoAlfaNum(ArqPontos);
GetLine(ArqPontos, strBuffery,"  \t\x0A");   //--- y
PegaProximoAlfaNum(ArqPontos);
GetLine(ArqPontos, strBufferx,"  \t\x0A");   //--- x
PegaProximoAlfaNum(ArqPontos);
GetLine(ArqPontos, strBufferz,"  \t\x0A");   //--- z
PegaProximoAlfaNum(ArqPontos);
std::getline(ArqPontos, strDescricao);      //--- Obs

if(!strBuffery.empty() && !strBufferx.empty() && !strBufferz.empty())
{
double x(atof(strBufferx.c_str())),y(atof(strBuffery.c_str())),z(atof(strBufferz.c_str()));

CDeltaSuperficie CD(atof(strBufferx.c_str()),atof(strBuffery.c_str()),atof(strBufferz.c_str()),INFINITO,INFINITO,INFINITO,CString(strDescricao.c_str()),0,0,CString(strNome.c_str()));

pSuperficieAtual->insert(CD);
}
else
{
std::strstream strMensagem;

strMensagem << "Erro ao ler o arquivo xyz na linha " << ContaLinhas << std::ends;

AfxMessageBox(strMensagem.str());

return false;
}
}

if(!ArqPontos.eof())
{
std::strstream strMensagem;
strMensagem << "Erro ao ler o arquivo xyz na linha " << ContaLinhas << std::ends;
AfxMessageBox(strMensagem.str());

return false;

}
}
catch(...)
{
std::strstream strMensagem;
strMensagem << "Erro ao ler o arquivo xyz na linha " << ContaLinhas << std::ends;
AfxMessageBox(strMensagem.str());

return false;
}
}
else
{
std::strstream strMensagem;
strMensagem << "Erro ao abrir o arquivo " << *itNomeArqAtual << std::ends;
AfxMessageBox(strMensagem.str());

return false;
}
}

return true;
}

std::strstream strMensagem;
strMensagem << "Nenhum arquivo de terreno foi encontrado para o projeto: "  << NomeProjeto << std::ends;
AfxMessageBox(strMensagem.str());

return false;
}
*/
void CSuperficie::LimpaSupBalizasRestricoes()
{
  LLDeltaSup& ListaRestricoes(PegaListaRestricoes());

  for (register ItCLLDeltaSup LDRestricaoAtual = ListaRestricoes.begin(); LDRestricaoAtual != ListaRestricoes.end(); LDRestricaoAtual++)  //--- Para todas as restrições
  {
    ItCLDeltaSup itRestricaoAtual(LDRestricaoAtual->begin()),
      itRestricaoPosterior(itRestricaoAtual);

    ++itRestricaoPosterior;

    while (itRestricaoPosterior != LDRestricaoAtual->end())    //--- Para todos os pontos da restrição 
    {
      ItSSuperficie itSupAtual(pSuperficieAtual->find(*itRestricaoAtual));

      //--- Tem que porcurar a parte porque o set esta ordenado por azimute, aqui queremos por x e y.

      register ittysetArestas itArestaAtual(itSupAtual->PontosLigados.begin());

      for (; itArestaAtual != itSupAtual->PontosLigados.end(); itArestaAtual++)
      {
        if (*itArestaAtual == itRestricaoPosterior->PCentral) break;  //--- Tem que achar!!!
      }

      ASSERT(itArestaAtual != itSupAtual->PontosLigados.end());

      for (register itsetPontosS itPontoInterpAtual = itArestaAtual->PontosInterpolacao.begin(); itPontoInterpAtual != itArestaAtual->PontosInterpolacao.end(); itPontoInterpAtual++)
        (const_cast<Ponto*>(&*itPontoInterpAtual))->Baliza = false;

      ++itRestricaoAtual;
      ++itRestricaoPosterior;
    }
  }
}

int CSuperficie::LigaCNRestricoesFechadas()
{
  //  LimpaSupBalizasRestricoes();

  LLDeltaSup& ListaRestricoes(PegaListaRestricoes());

  for (register ItCLLDeltaSup LDRestricaoAtual = ListaRestricoes.begin(); LDRestricaoAtual != ListaRestricoes.end(); LDRestricaoAtual++)
  {
    LimpaSupBalizasRestricoes();   //--- Não tem importancia limpar tudo, as areas que ja foram feitas não serão mais usadas

    ItCLDeltaSup itRestricaoAtual(LDRestricaoAtual->begin()),
      itRestricaoPosterior(itRestricaoAtual),
      itRestricaoPosteriorDaPosterior(++itRestricaoPosterior);

    ++itRestricaoPosteriorDaPosterior;

    while (itRestricaoPosterior != LDRestricaoAtual->end())    //--- Para todos os pontos da restrição 
    {
      ItSSuperficie itSupAtual(pSuperficieAtual->find(*itRestricaoAtual));

      //--- Tem que porcurar a parte porque o set esta ordenado por azimute, aqui queremos por x e y.

      //ittysetArestas itArestaAtual(itSupAtual->PontosLigados.begin());

      ittysetArestas itArestaAtual(itSupAtual->PegaPontoLigadoXY(itRestricaoPosterior->PCentral));

      /*
      for( ; itArestaAtual != itSupAtual->PontosLigados.end() ; itArestaAtual++)
      {
      if(*itArestaAtual == itRestricaoPosterior->PCentral) break;  //--- Tem que achar!!!
      }
      */

      ASSERT(itArestaAtual != itSupAtual->PontosLigados.end());

      //ittysetArestas itArestaAtual(itSupAtual->PontosLigados.find(CAresta(itRestricaoPosterior->PCentral)));

      for (register itsetPontosS itPontoInterpAtual = itArestaAtual->PontosInterpolacao.begin(); itPontoInterpAtual != itArestaAtual->PontosInterpolacao.end(); itPontoInterpAtual++)
      {
        if (itPontoInterpAtual->Baliza == true) continue;

        //--- As arestas que serão varridas até achar a saida, tem que achar!!!

        register ItCLDeltaSup itRestInicioArestaAtual(itRestricaoPosterior),
          itRestFimArestaAtual(itRestricaoPosteriorDaPosterior);

        bool Achou(false);

        do
        {
          ItSSuperficie itSupPosteriorAtual(pSuperficieAtual->find(*itRestInicioArestaAtual));
          //ittysetArestas itArestaPosterior(itSupPosteriorAtual->PontosLigados.find(itRestFimArestaAtual->PCentral));
          ittysetArestas itArestaPosterior(itSupPosteriorAtual->PegaPontoLigadoXY(itRestFimArestaAtual->PCentral));
          itsetPontosS iTPontoCota(itArestaPosterior->AchaPontoNaCota(itPontoInterpAtual->z, true));

          if (iTPontoCota != itArestaPosterior->PontosInterpolacao.end())
          {
            //--- achou a saida.

            LPontosCN.push_back(LPontos(1, *itPontoInterpAtual));
            LPontos& LCNAtual(LPontosCN.back());
            LCNAtual.push_back(*iTPontoCota);

            //--- Marca como usados

            (const_cast<Ponto*>(&*itPontoInterpAtual))->Baliza = false;
            (const_cast<Ponto*>(&*iTPontoCota))->Baliza = true;
            Achou = true;
          }
          else   //--- Não achou, passa para aproxima aresta da restrição
          {
            ++itRestInicioArestaAtual;
            ++itRestFimArestaAtual;
          }

        } while (!Achou && itRestFimArestaAtual != itRestricaoPosterior);  //--- Tem que achar!!!
      }

      ++itRestricaoAtual;
      ++itRestricaoPosterior;
      ++itRestricaoPosteriorDaPosterior;
      if (itRestricaoPosteriorDaPosterior == LDRestricaoAtual->end()) itRestricaoPosteriorDaPosterior = LDRestricaoAtual->begin();
    }
  }

  return true;
}

/*
int CSuperficie::LigaCNRestricoesFechadas()
{
LLDeltaSup& ListaRestricoes(PegaListaRestricoes());

for(register ItCLLDeltaSup LDRestricaoAtual = ListaRestricoes.begin() ; LDRestricaoAtual != ListaRestricoes.end(); LDRestricaoAtual++)
{
if(*(LDRestricaoAtual->begin()) == *(LDRestricaoAtual->rbegin()))     //--- Esta fechada?
{
ItCLDeltaSup itRestricaoAtual(LDRestricaoAtual->begin()),
itRestricaoPosterior(++(itRestricaoAtual)),
itRestricaoPosteriorDaPosterior(++(itRestricaoPosterior));

while(*(itRestricaoPosterior) != *(LDRestricaoAtual->begin()))    //--- Para todos os pontos da restrição
{
// A lista de restriçoes é apenas uma lista de pontos,portanto é preciso achar os pontos da restriçaõ na superficie e
// trabalhar com as delta-superficies.

// Acha os pontos de restrição atuais na superficie.

ItSSuperficie itSupAtual(pSuperficieAtual->find(*itRestricaoAtual));

ittysetArestas itArestaAtual(itSupAtual->PontosLigados.find(CAresta(itRestricaoPosterior->PCentral)));

for(register itsetPontosS itPontoInterpAtual=itArestaAtual->PontosInterpolacao.begin(); itPontoInterpAtual != itArestaAtual->PontosInterpolacao.end() ; itPontoInterpAtual++)
{
if(itPontoInterpAtual->Baliza == true) continue;

//--- As arestas que serão varridas até se achar a saida, tem que achar!!!

register ItCLDeltaSup itRestInicioArestaAtual(itRestricaoPosterior),
itRestFimArestaAtual(itRestricaoPosteriorDaPosterior);

bool Achou(false);

do
{
ItSSuperficie itSupPosteriorAtual(pSuperficieAtual->find(*itRestInicioArestaAtual));
ittysetArestas itArestaPosterior(itSupPosteriorAtual->PontosLigados.find(itRestFimArestaAtual->PCentral));
itsetPontosS iTPontoCota(itArestaPosterior->AchaPontoNaCota(itPontoInterpAtual->z,true));

if(iTPontoCota != itArestaPosterior->PontosInterpolacao.end())
{
//--- achou a saida.

LPontosCN.push_back(LPontos(1,*itPontoInterpAtual));
LPontos& LCNAtual(LPontosCN.back());
LCNAtual.push_back(*iTPontoCota);

//--- Marca como usados

(const_cast<Ponto*>(&*itPontoInterpAtual))->Baliza = false;
(const_cast<Ponto*>(&*iTPontoCota))->Baliza = true;
Achou = true;
}
else   //--- Não achou, passa para aproxima aresta da restrição
{
++itRestInicioArestaAtual;
++itRestFimArestaAtual;
}

}while (!Achou && itRestFimArestaAtual != itRestricaoPosterior);  //--- Tem que achar!!!
}
++itRestricaoAtual;
++itRestricaoPosterior;
++itRestricaoPosteriorDaPosterior;
}
}
}

return true;
}
*/

int CSuperficie::PegaNomesArquivosTerreno(tysetNomeArquivos& setNomeArquivos, CString NomeProjeto)
{
  struct _finddata_t c_file;
  intptr_t hFile;
  int i(0);

  NomeProjeto = NomeProjeto.Mid(0, NomeProjeto.ReverseFind('\\'));

  setNomeArquivos.clear();

  if ((hFile = _findfirst((char*)LPCTSTR(NomeProjeto + "\\*.xyz"), &c_file)) == -1L) return 0;
  else
  {
    do
    {
      setNomeArquivos.insert((NomeProjeto + "\\" + c_file.name).GetBuffer());
    } while (++i < 500 && _findnext(hFile, &c_file) == 0);

    _findclose(hFile);
  }

  return i;
}

bool CSuperficie::Descarrega(const std::string& strNomeArquivo, bool OutraSuper)
{
  SSuperficie* pSuper(OutraSuper ? pOutraSuperficie : pSuperficieAtual);

  std::ofstream ArqLogPontos(strNomeArquivo);

  if (ArqLogPontos.good())
  {
    ArqLogPontos << std::fixed << std::setprecision(4);

    for (ItSSuperficie it = pSuper->begin(); it != pSuper->end(); it++)
    {
      ArqLogPontos << it->PegaPCentral().x << ' ' << it->PegaPCentral().y << ' ' << it->PegaPCentral().z << " " << it->PegaPCentral().fi << std::endl;

      for (ittysetArestas itpSuperLigada = it->PontosLigados.begin(); itpSuperLigada != it->PontosLigados.end(); ++itpSuperLigada)     //--- rotaciona a lista de pontos ligados
      {
        ArqLogPontos << "      " << itpSuperLigada->PontoLigado.x << ' ' << itpSuperLigada->PontoLigado.y << ' ' << itpSuperLigada->PontoLigado.z << " " << itpSuperLigada->PontoLigado.fi << std::endl;
      }
    }
    ArqLogPontos.flush();
    ArqLogPontos.close();

    return true;
  }
  return false;
}

unsigned int CSuperficie::VerfAciTopografico(ItCLLDeltaSup LDRestricaoAtual, bool Remover)
{
  if (LDRestricaoAtual->size() > 0)
  {
    int TipoObjeto(LDRestricaoAtual->begin()->TipoObjeto);

    if (!Remover) return  TipoObjeto;
    else
    {
      paitititmmapitLLDeltaSup PairItIt(MMapItLLDeltaSup.equal_range(TipoObjeto));

      if(PairItIt.first != MMapItLLDeltaSup.end())
      {
        if(PairItIt.first->first == TipoObjeto)
        {
          citmmapitLLDeltaSup it(PairItIt.first);
      
          while (it != PairItIt.second && it->second != LDRestricaoAtual)
          {
            it++;
          }

          if (it != MMapItLLDeltaSup.end())
          {
            //--- Remove dos pontos inseridos pelo usuario (Se foi inserido pelo usuário)
            //--- Para evitar o find poderia verificar pelo tipo mas assim ficou mais "Limpo" e seguro ou seja
            //--- de qualquer forma ia ter que fazer o find para garantir

            for (auto itDS = it->second->begin(); itDS != it->second->end() ; itDS++)
            {
              ItSSetPontos itIns(SetPontosInseridos.find(itDS->PCentral));
               
              if(itIns != SetPontosInseridos.end())
              {
                SetPontosInseridos.erase(itIns);
              }
            }

            MMapItLLDeltaSup.erase(it);

            return CAciTopografico::NENHUM_ACITOPOG;
          }

          return TipoObjeto;
        }
      }
    }
  }
  return CAciTopografico::NENHUM_ACITOPOG;
}

unsigned int CSuperficie::VerfEsboco(ItCLLDeltaSup LDEsbocoAtual, bool Remover)
{
  citmmapitLLDeltaSup it(MMapItLLDeltaSup.begin());

  if (it == MMapItLLDeltaSup.end()) return CAciTopografico::NENHUM_ACITOPOG;

  while (it != MMapItLLDeltaSup.end() && it->second != LDEsbocoAtual)
  {
    ++it;
  }

  if (it != MMapItLLDeltaSup.end())
  {
    if (Remover)
    {
      MMapItLLDeltaSup.erase(it);
      return CAciTopografico::NENHUM_ACITOPOG;
    }
  }

  return  it->first;
}

//--- 11/07/2023 -> RemoverDaSuperficie - Quando vier de acrescimo de uma lista na outra não pode remover da superficie

void CSuperficie::RemoveListaDoMapa(ItCLLDeltaSup itLAtual, int Tipo,bool RemoverDaSuperficie,bool Forcar)
{
  //--- Se o acidente topografico foi digitalizado (Esboçado) (Talvegues e espigões de bacias por exemplo)
  //--- tem q remover os pontos da superfície tb pois eles foram inseridos na digitalização do acidente topográfico

  if(RemoverDaSuperficie)
  {
    if (itLAtual->begin()->PCentral.Baliza2 & Ponto::ESBOCADO || Forcar) 
    {
      for (ItCLDeltaSup itLDS = itLAtual->begin(); itLDS != itLAtual->end(); itLDS++)
      {
        RemovePonto(itLDS->PegaPCentral());
      }
    }
  }

  if (Tipo != CAciTopografico::NENHUM_ACITOPOG)
  {
    paitititmmapitLLDeltaSup PairItIt(MMapItLLDeltaSup.equal_range(Tipo));

    while (PairItIt.first != PairItIt.second && itLAtual != PairItIt.first->second)  ++PairItIt.first;

    if (PairItIt.first != PairItIt.second) MMapItLLDeltaSup.erase(PairItIt.first);
  }
  else VerfAciTopografico(itLAtual, true);
}

bool CSuperficie::VerificaIntersecaoAcidentesTopog(const Ponto& P1, 
const Ponto& P2, 
Ponto& PontoIntersecao, 
int Tipo, 
bool Sugerir, 
bool PularInseridos, 
itmmapitLLDeltaSup* ItInicio,
ItLLDeltaSup* pItLLRes2,
ItLDeltaSup* pItP1)
{

   return false;

  //--- Se veio com um itInicio começa nele 
  //--- Senão 
  //---   Se o tipo veio como nenhum ou todos, procura em todos (desde o inicio)
  //---   Senão
  //---     Comeca no tipo que veio
  //--- Atenção   CAciTopografico::NENHUM_ACITOPOG esta errado o certo é so usar TODOSACITOPOG   corrigir

  //if(Tipo == CAciTopografico::NENHUM_ACITOPOG || Tipo == CAciTopografico::FIM_ACITOPOG) return false;

  citmmapitLLDeltaSup itAtual(ItInicio != NULL ? *ItInicio : (Tipo == CAciTopografico::NENHUM_ACITOPOG || Tipo == CAciTopografico::FIM_ACITOPOG) ? MMapItLLDeltaSup.begin() : MMapItLLDeltaSup.lower_bound(Tipo)),

    //--- Se veio para procurar apenas em um tipo procura apenas nas restrições daquele tipo, caso contrário o procura em todas as restrições

  itFinal((Tipo == CAciTopografico::NENHUM_ACITOPOG || Tipo == CAciTopografico::FIM_ACITOPOG) ? MMapItLLDeltaSup.end() : MMapItLLDeltaSup.upper_bound(Tipo));

  //  while (it != (MMapItLLDeltaSup.end()) && (Tipo == CAciTopografico::NENHUM_ACITOPOG ? true : it->first == Tipo))
  if (itAtual != MMapItLLDeltaSup.end())
  {
    while (itAtual != itFinal)
    {
      if (itAtual->first < CAciTopografico::INICIO_EQUI_DRENAGEM && VerfGeraArestaForcada(itAtual->first) == true)       
      {
        ItCLLDeltaSup ItLDRestricaoAtual(itAtual->second);
        register ItCLDeltaSup ItPontoAtual(ItLDRestricaoAtual->begin()), IpPontoAnterior(ItPontoAtual);

        while (++ItPontoAtual != ItLDRestricaoAtual->end())
        {
          bool PontoInserido(ItPontoAtual->PegaPCentral().Descricao.Find("Ponto Inserido") != -1),
               PontoInseridoUsuario(ItPontoAtual->PegaPCentral().Descricao.Find("Usuário") != -1);

          if ((PontoInserido && PontoInseridoUsuario) || !PontoInserido)  //-- Pontos inseridos por Delaunay ou Gabriel não são considerados
          {
            //--- As retas não podem ter pontos em comum senão é falso positivo

            if (!P1.Compara2D(IpPontoAnterior->PCentral,1E-3) && !P1.Compara2D(ItPontoAtual->PCentral, 1E-3) &&
                !P2.Compara2D(IpPontoAnterior->PCentral, 1E-3) && !P2.Compara2D(ItPontoAtual->PCentral, 1E-3))
            {
              if (CLPontos::CalculaIntersecao(P1, P2, IpPontoAnterior->PCentral, ItPontoAtual->PCentral, PontoIntersecao, true) == TRUE)
              {
                //--- Exclui da comparação os pontos extremos da reta como tambem se o segmento a comparar for igual ao segmento comparado

                if (!PontoIntersecao.Compara2D(IpPontoAnterior->PCentral) && !PontoIntersecao.Compara2D(ItPontoAtual->PCentral) && !PontoIntersecao.Compara2D(P1) && !PontoIntersecao.Compara2D(P2))
                {
                  Sugerir = false;   //--- Aguardar

               //   if (Sugerir && !InserirPontoIntersecao(P1, P2, ItLDRestricaoAtual, IpPontoAnterior, ItPontoAtual, PontoIntersecao, pItLLRes2, &(*pItLLRes2)->end()))

                  //--- não precisa mais impedir o acidente topográfico ou calcular a intersecao entre eles
                  //--- A triangulação da CGAL cuida disso 
                  //--- Usando typedef CGAL::Exact_predicates_tag Itag; vewja constraint.cpp


                  //--- OK a triangulação da CGAL resolve a interseção entre arestas forçadas
                  //--- mas aumenta a coméxidade da triangulação (principalmente a de Delaunay-gabriel)
                  //--- Porisso mantem (em teste) <<<---VERIFICAR 

                  //--- Manter a regra de não acidentes topograficos que geram arestar forçadas
                  //--- não poderem se interceptarem: 
                  //--- 1 - Quando remove qualquer um dos acidentes topograficos tem q remover a insterseção tb, não esta removendo
                  //--- 2 - existem acidentes topograficos que não podem se interceptar de jeito nenhum, por exeplo
                  //---     cristas e bordos de rios
                  //--- ANALIZAR MELHOR - DEIXAR PARA UMA OUTRA VERSÃO <<=== VERIFICAR

                  if(1)
                  {
                    return true;   //--- return true emite um erro dizendo que há interseçáo entre acidentes topograficos
					               //--- Apesar da CGAL suportar este caso não o permite diminuir a complexidade
                  }
                  else
                  {
                    //--- Este ponto tem que ser removido caso qualquer um dos acidentes topográgicos seja removido
                    //--- (não está sendo removido)

                    PontoIntersecao.Nome.Format("Inters");
                    PontoIntersecao.Baliza2 = Ponto::INTER_ACI_TOPOG;
                    PontoIntersecao.Descricao = "Intersecao Acidentes Topográficos";
                    PontoIntersecao.AcidenteTopografico = CAciTopografico::NENHUM_ACITOPOG;

                    if (EstaRotacionada())   //--- se estiver desrotaciona
                    {
                      PontoIntersecao.RotacionaPlano(sin(-PegaAnguloRotacao()), cos(-PegaAnguloRotacao()), PegaAnguloRotacao());
                    }

                    InsereDeltaSuperficie(CDeltaSuperficie(PontoIntersecao, PontoIntersecao.AcidenteTopografico));
                  }
                 
                  return false;
                }
              }
            }
            IpPontoAnterior = ItPontoAtual;
          }
        }
      }
      ++itAtual;
    }
  }
  return false;    //--- não ha inteerceções ou todas foram calculadas
}

void CSuperficie::CriaListaBordos()  //--- Para compatibilidade com versões anteriores
{
  ListaBordos.clear();

  paitititmmapitLLDeltaSup PairItIt(MMapItLLDeltaSup.equal_range(CAciTopografico::FRONTEIRA));

  while (PairItIt.first != PairItIt.second)
  {
    ItCLDeltaSup itCLDS(PairItIt.first->second->begin());

    ListaBordos.push_back(LDeltaSup(1, *PairItIt.first->second->begin()));

    for (register ItCLDeltaSup itCLDS = ++(PairItIt.first->second->begin()); itCLDS != PairItIt.first->second->end(); ++itCLDS)
    {
      ListaBordos.back().push_back(*itCLDS);
    }

    ++PairItIt.first;
  }
}

bool CSuperficie::VerfArestaForaSuperficie(const Ponto& P1, const Ponto& P2)
{
//--- 

  return false;  //--- essa verificação é necessaria?

  //--- -4-07-2022 Verifica se uma aresta a partir de um ponto da fronteira esta indo para fora ou para dentro da superficie

  itmmapitLLDeltaSup it(MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA));

  if (it != MMapItLLDeltaSup.end())
  {
    if (it->second->size() > 2 && *it->second->begin() == *it->second->rbegin())
    {
      double AZP1P2(Mat.CalculaAzimute(P1, P2)), AZP2P1(Mat.CalculaAzimute(P2, P1));

      for (register ItCLDeltaSup ItPontoRestAtual = ++it->second->begin(); ItPontoRestAtual != it->second->end(); ++ItPontoRestAtual)
      {
        if (*ItPontoRestAtual == P1 || *ItPontoRestAtual == P2)
        {
          ItCLDeltaSup ItProximo(ItPontoRestAtual), ItAnterior(ItPontoRestAtual);

          //--- Listas circulares.
          //--- Pula os pontos inseridos

          if (ItAnterior == it->second->begin()) ItAnterior = --(it->second->end());
          else --ItAnterior;
          while (ItAnterior->PegaPCentral().Descricao.Find("Ponto Inserido") != -1 && ItAnterior->PegaPCentral().Descricao.Find("Usuário") == -1)
          {
            if (ItAnterior == it->second->begin()) ItAnterior = --(it->second->end());
            else --ItAnterior;
          }

          ++ItProximo;
          if (ItProximo == it->second->end()) ItProximo = it->second->begin();
          while (ItProximo->PegaPCentral().Descricao.Find("Ponto Inserido") != -1 && ItProximo->PegaPCentral().Descricao.Find("Usuário") == -1)
          {
            ++ItProximo;
            if (ItProximo == it->second->end()) ItProximo = it->second->begin();
          }

          //--- Verifica se a aresta da restrição está em cima de uma aresta da fronteira, se estiver tudo bem, não precisa verificar a interseção
          //--- A fronteira pode estar em um sentido e a restrição em outro, porisso é preciso verificar nos 2 pontos.

          if (!(ItProximo->PegaPCentral() == P1 || ItProximo->PegaPCentral() == P2 || ItAnterior->PegaPCentral() == P1 || ItAnterior->PegaPCentral() == P2))
          {
            //--- A fronteira pode estar em um sentido e a restrição em outro, porisso é preciso verificar com os 2.
            //--- Calcular o fi no cálculo da fronteira, assim não será preciso calcular o azimute aqui

            double AZPosterior(Mat.CalculaAzimute(ItPontoRestAtual->PegaPCentral(), ItProximo->PegaPCentral())),
                   AZAnterior(Mat.CalculaAzimute(ItPontoRestAtual->PegaPCentral(), ItAnterior->PegaPCentral())),
                   AZRestricao(P1 == ItPontoRestAtual->PegaPCentral() ? AZP1P2 : AZP2P1);

          //--- Este seria o az anterior correto mas esta pegando sequencia de pontos defazados  na fronteira
          //--- Verificar #Carlos 17072022

            AZPosterior = AZAnterior + Matem::M_PI;

            if(AZPosterior > Matem::M_2_PI) AZPosterior -= Matem::M_2_PI;


            //--- Se o segmento for um segmento de fronteira ou estiver em cima de um segmento de fronteira nao é preciso comparar e se comparar ocorrem erros de arredondamento
            //--- causando falso true ou falso false.
            //--- Verifica comparando os azimutes.

            double DeltaAzPosterior(fabs(AZPosterior - AZRestricao)), DeltaAzAnterior(fabs(AZAnterior - AZRestricao));

            //-- É preciso verificar nos 2 sentidos.

            if ((DeltaAzPosterior > 1e-10 && fabs((DeltaAzPosterior - Matem::M_PI)) > 1e-10) || (DeltaAzAnterior > 1e-10 && fabs((DeltaAzAnterior - Matem::M_PI)) > 1e-10))
            {
              double DeltaAzRestAzPost(fabs(AZRestricao - AZPosterior)),
                     DeltaAzRestAzAnte(fabs(AZRestricao - AZAnterior));
                      
              if ((Area < 0.0 && ((AZPosterior > AZAnterior && !(DeltaAzPosterior < -1E-7 && DeltaAzAnterior >  1E-7)) || (AZPosterior < AZAnterior && (DeltaAzAnterior < -1E-7 && DeltaAzPosterior >  1E-7)))) ||
                  (Area > 0.0 && ((AZPosterior < AZAnterior && !(DeltaAzPosterior >  1E-7 && DeltaAzAnterior < -1E-7)) || (AZPosterior > AZAnterior && (DeltaAzAnterior >  1E-7 && DeltaAzPosterior < -1E-7)))))

/*            if ((Area < 0.0 && ((AZPosterior > AZAnterior && !(AZRestricao < AZPosterior && AZRestricao > AZAnterior)) || (AZPosterior < AZAnterior && (AZRestricao < AZAnterior && AZRestricao > AZPosterior)))) ||
                  (Area > 0.0 && ((AZPosterior < AZAnterior && !(AZRestricao > AZPosterior && AZRestricao < AZAnterior)) || (AZPosterior > AZAnterior && (AZRestricao > AZAnterior && AZRestricao < AZPosterior)))))*/
              {
                return true;
                //return false;  //--- essa verificação é necessaria?
              }
            }
          }
        }
      }
    }
  }
  return false;
}

bool CSuperficie::VerfAciTopogForaSuperficie(SSetPontos& PontosProblema, ItCLLDeltaSup* ItMarcada)
{
  citmmapitLLDeltaSup it(MMapItLLDeltaSup.begin());

  while (it != (MMapItLLDeltaSup.end()))
  {
    switch (it->first)
    {
    case CAciTopografico::FRONTEIRA:
    case CAciTopografico::POSTE:
    case CAciTopografico::TAMPA_BUEIRO:
    case CAciTopografico::BUEIRO:
    case CAciTopografico::EIXO:
    case CAciTopografico::REDE_ELETRICA_AT:
    case CAciTopografico::REDE_ELETRICA_BT:
    case CAciTopografico::CANTO_CASA:
    case CAciTopografico::CANTO_CERCA:
    case CAciTopografico::CANTO_MURO:
    case CAciTopografico::CERCA:
    case CAciTopografico::MURO:
    case CAciTopografico::CASA:
    case CAciTopografico::MATA:
    case CAciTopografico::FLORESTA:
    case CAciTopografico::PLANTACAO:
    case CAciTopografico::ARVORE:
    case CAciTopografico::ALINHAMENTO:
    case CAciTopografico::PONTE:
    case CAciTopografico::NENHUM_ACITOPOG: break;
    default:
    {
      ItCLLDeltaSup ItLDRestricaoAtual(it->second);
      register ItCLDeltaSup ItPontoAtual = ItLDRestricaoAtual->begin(), IpPontoAnterior(ItPontoAtual);

      while (++ItPontoAtual != ItLDRestricaoAtual->end())
      {
        if (ItPontoAtual->PegaPCentral().Descricao.Find("Ponto Inserido") == -1 || ItPontoAtual->PegaPCentral().Descricao.Find("Usuário") != -1)
        {
          if (VerfArestaForaSuperficie(IpPontoAnterior->PCentral, ItPontoAtual->PCentral) == true)
          {
            if (ItMarcada) *ItMarcada = it->second;
            PontosProblema.insert(ItPontoAtual->PCentral);
          }

          IpPontoAnterior = ItPontoAtual;
        }
      }
    }
    }
    ++it;
  }

  return PontosProblema.size() == 0;
}

//--- Agora esta verificando cruzamento de todos os acientes topográficos  CAciTopografico::TODOS_ACITOPOG

bool CSuperficie::VerfAciTopogCruzFronteira(ItCLLDeltaSup& ItLLDeltaSupMarcada, SSetPontos& PontosProblema)
{
  citmmapitLLDeltaSup it(MMapItLLDeltaSup.upper_bound(CAciTopografico::FRONTEIRA)); //---- A FRONTEIRA tem que ser o numero 1 no enum do CAciTopografico

  while (it != (MMapItLLDeltaSup.end()))
  {
    if (it->first < CAciTopografico::INICIO_EQUI_DRENAGEM && VerfGeraArestaForcada(it->first) == true)  //---  Equipamentos de drenagem estão nos acidentes mas não geram arestas 
    {
      ItCLLDeltaSup ItLDRestricaoAtual(it->second);
      register ItCLDeltaSup ItPontoAtual(ItLDRestricaoAtual->begin()), IpPontoAnterior(ItPontoAtual);

      Ponto PontoIntersecao;

      while (++ItPontoAtual != ItLDRestricaoAtual->end())
      {
        //--- Pula os pontos inseridos (exetos os que foram inseridos pelo usuário)

        while (ItPontoAtual->PCentral.Descricao.Find("Ponto Inserido") != -1 && ItPontoAtual->PCentral.Descricao.Find("Usuário") == -1) ++ItPontoAtual;

        //     if(ItPontoAtual->PCentral.Descricao.Find("Ponto Inserido") == -1 || ItPontoAtual->PCentral.Descricao.Find("Usuário") != -1)
        {
          if (SetSegmentosRestricoes.find(CSegmento(IpPontoAnterior->PegaPCentral(), ItPontoAtual->PegaPCentral())) == SetSegmentosRestricoes.end() &&
              SetSegmentosRestricoes.find(CSegmento(ItPontoAtual->PegaPCentral(), IpPontoAnterior->PegaPCentral())) == SetSegmentosRestricoes.end())   //--- Verifica se não é segmento da fronteira
          {
            if (VerificaIntersecaoAcidentesTopog(IpPontoAnterior->PCentral, ItPontoAtual->PCentral, PontoIntersecao, CAciTopografico::FIM_ACITOPOG, false, true) == true)
            {
              ItLLDeltaSupMarcada = it->second;

              if (Rotacionada)
              {
                PontosProblema.insert((ItPontoAtual->PegaPCentral().RotacionaPlano(sin(-AnguloDeRotacao), cos(-AnguloDeRotacao), -AnguloDeRotacao, true)));
                PontosProblema.insert((IpPontoAnterior->PegaPCentral().RotacionaPlano(sin(-AnguloDeRotacao), cos(-AnguloDeRotacao), -AnguloDeRotacao, true)));
                PontosProblema.insert((PontoIntersecao.RotacionaPlano(sin(-AnguloDeRotacao), cos(-AnguloDeRotacao), -AnguloDeRotacao, true)));
              }
              else
              {
                PontosProblema.insert(ItPontoAtual->PCentral);
                PontosProblema.insert(IpPontoAnterior->PCentral);
                PontosProblema.insert(PontoIntersecao);
              }
            }
          }
          IpPontoAnterior = ItPontoAtual;
        }
      }
    }
    ++it;
  }
  return PontosProblema.size() != 0;
}

void CSuperficie::LeNumDescPontosInseridos()
{
  for (register ItSSetPontos itSPI = SetPontosInseridos.begin(); itSPI != SetPontosInseridos.end(); ++itSPI)
  {
    ItSSuperficie ItSUP(PegaPontoMaisProximo(CDeltaSuperficie(itSPI->x, itSPI->y, 0.00), 0.0005, 0.0005));

    if (ItSUP != pSuperficieAtual->end())
    {
      CDeltaSuperficie* pCDeltaSuperficie(const_cast<CDeltaSuperficie*>(&*ItSUP));
      pCDeltaSuperficie->PegaPCentralNC().Descricao = itSPI->Descricao;
      pCDeltaSuperficie->PegaPCentralNC().i = itSPI->i;
      pCDeltaSuperficie->PegaPCentralNC().Inserido = true;
    }
  }
}

bool CSuperficie::MudaTipoAciTopografico(ItCLLDeltaSup LDRestricaoAtual, unsigned int TipoAtual, unsigned int NovoTipo)
{
  citmmapitLLDeltaSup it(MMapItLLDeltaSup.find(TipoAtual));

  while (it != MMapItLLDeltaSup.end() && it->first == TipoAtual && it->second != LDRestricaoAtual)
  {
    ++it;
  }

  if (it != MMapItLLDeltaSup.end() && it->first == TipoAtual && it->second == LDRestricaoAtual)
  {
    for (ItCLDeltaSup it = LDRestricaoAtual->begin(); it != LDRestricaoAtual->end(); it++)
    {
      (const_cast<CDeltaSuperficie*>(&*it))->TipoObjeto = NovoTipo;
    }
    MMapItLLDeltaSup.erase(it);
    MMapItLLDeltaSup.insert(pairmmapitLLDeltaSup(NovoTipo, LDRestricaoAtual));

    return true;
  }

return false;
}

int CSuperficie::VerfPontoForaSuperficie(const Ponto& PontoVerificar, bool MostrarMensagem)
{
  citmmapitLLDeltaSup itLLFronteira(MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA));

  if (itLLFronteira == MMapItLLDeltaSup.end() || itLLFronteira->second->size() < 3) return 2;   //--- A fronteira ainda não esta definida, retorna como se o
                                                                                                //---  ponto estivesse dentro da superfície.     
  register ItCLDeltaSup itPAnterior(itLLFronteira->second->begin());
  register ItCLDeltaSup itPAtual(itPAnterior);
  unsigned int Contador(0);
  double dy(1e-5);

  if (PontoVerificar == itPAtual->PCentral) return false;                //--- Ponto Pertence a fronteira tb

  for (++itPAtual; itPAtual != itLLFronteira->second->end(); ++itPAtual)
  {
    if (PontoVerificar != itPAtual->PCentral)
    {
      //--- Se o ponto estiver no y do ponto a verificar conta 2 x, nesta reta e na proxima
      //--- Esse if evita essa contagem dobrada.

      if (fabs(itPAtual->PCentral.y - PontoVerificar.y) < dy) continue;

      //--- @Verificar a necessidade de considerar a area

      if ((Area < 0.0 && itPAnterior->PCentral.x > PontoVerificar.x && itPAtual->PCentral.x > PontoVerificar.x) ||
        (Area > 0.0 && itPAnterior->PCentral.x < PontoVerificar.x && itPAtual->PCentral.x < PontoVerificar.x))
      {
        if ((PontoVerificar.y >= (itPAnterior->PCentral.y - dy) && PontoVerificar.y <= (itPAtual->PCentral.y + dy)) || (PontoVerificar.y <= (itPAnterior->PCentral.y + dy) && PontoVerificar.y >= (itPAtual->PCentral.y - dy)))
        {
          ++Contador;
        }
      }
      else
      {
        if ((itPAnterior->PCentral.x <= PontoVerificar.x && itPAtual->PCentral.x >= PontoVerificar.x) || (itPAnterior->PCentral.x >= PontoVerificar.x && itPAtual->PCentral.x <= PontoVerificar.x))
        {
          if ((itPAnterior->PCentral.y >= (PontoVerificar.y - dy) && itPAtual->PCentral.y <= (PontoVerificar.y + dy)) || (itPAnterior->PCentral.y <= (PontoVerificar.y + dy) && itPAtual->PCentral.y >= (PontoVerificar.y - dy)))
          {
            Ponto PontoIntersecao;

            if (CLPontos::CalculaIntersecao(itPAnterior->PCentral, itPAtual->PCentral, PontoVerificar, Ponto(PontoVerificar.x + 1000.0, PontoVerificar.y), PontoIntersecao, false) == TRUE)
            {
              if ((Area < 0.0 && PontoIntersecao.x >= (PontoVerificar.x - dy)) || (Area > 0.0 && PontoIntersecao.x <= (PontoVerificar.x + dy)))
              {
                ++Contador;
              }
            }
          }
        }
      }
      itPAnterior = itPAtual;
    }
    else return false;         //--- Ponto pertence a fronteira tb
  }

  return (Contador % 2) == 0;
}

void CSuperficie::CriaSetSegmentosRestricoes(int Tipo)
{
  if (MMapItLLDeltaSup.size() == 0) return;

  paitititmmapitLLDeltaSup pairInicioFim(MMapItLLDeltaSup.begin(), MMapItLLDeltaSup.end());

  if (Tipo != CAciTopografico::NENHUM_ACITOPOG) pairInicioFim = MMapItLLDeltaSup.equal_range(Tipo);

  for (citmmapitLLDeltaSup it = pairInicioFim.first; it != pairInicioFim.second; ++it)
  {
    ItCLDeltaSup ItPontoAnterior(it->second->begin());
    register ItCLDeltaSup ItPontoAtual(ItPontoAnterior);

    for (++ItPontoAtual; ItPontoAtual != it->second->end(); ++ItPontoAtual)
    {
      SetSegmentosRestricoes.emplace(CSegmento(ItPontoAnterior->PCentral, ItPontoAtual->PCentral));
      // SetSegmentosRestricoes.insert(CSegmento(ItPontoAnterior->PCentral,ItPontoAtual->PCentral));
      ItPontoAnterior = ItPontoAtual;
    }
  }
}

bool CSuperficie::VerfPontosForaSuperficie(SSetPontos& PontosProblema, int* pResposta)
{
  CriaSetPontosFronteira();

  *pResposta = 0;

  for (register ItSSuperficie itDeltaSupAtual = pSuperficieAtual->begin(); itDeltaSupAtual != pSuperficieAtual->end() && PontosProblema.size() < 201; itDeltaSupAtual++)
  {
    //--- Pontos inseridos serão recriados && Pontos da fronteira não precisam ser verificados && Se o ponto pertencer a fronteira também não precisa ser verificado

    if (SetPontosDesabilitados.find(itDeltaSupAtual->PegaPCentral()) == SetPontosDesabilitados.end())
    {
      if (itDeltaSupAtual->PegaPCentral().Nome.Find("PDES") == -1)  //--- Pontos dos equipamentos de drenagem não são considerados
      {
        if (itDeltaSupAtual->PegaPCentral().Inserido == false && itDeltaSupAtual->TipoObjeto != CAciTopografico::FRONTEIRA && EFronteira(itDeltaSupAtual->PegaPCentral()) == false)
        {
          int Resp(VerfPontoForaSuperficie(itDeltaSupAtual->PegaPCentral(), false));

          if (Resp != 0)
          {
            PontosProblema.insert(itDeltaSupAtual->PegaPCentral());
          
            *pResposta = Resp;
          }
        }
      }
    }
  }

  //--- Verifica os pontos que foram inseridos nas seções

  if(*pResposta == false)
  {
    for (ItCSSetPontos it = SetPonInsSecoes().begin(); it != SetPonInsSecoes().end(); it++)
    {
      int Resp(VerfPontoForaSuperficie(*it, false));

      if (Resp != 0)
      {
        PontosProblema.insert(*it);

        *pResposta = Resp;
      }
    }

    LimpaSetPontosFronteira();
  }

  return *pResposta == 0;
}

int CSuperficie::PegaNomesArquivosTerreno(tylstNomeArquivos& lstNomeArquivos, std::string strNomeProj)
{
  struct _finddata_t c_file;
  intptr_t hFile;
  int i(0);

  tyitlstNomeArquivos itArqPontosInseridosUsuario(lstNomeArquivos.end());

  lstNomeArquivos.clear();

  strNomeProj = strNomeProj.substr(0, strNomeProj.find_last_of('\\'));
  strNomeProj += "\\*.xyz";

  if ((hFile = _findfirst((char *)LPCTSTR(strNomeProj.c_str()), &c_file)) == -1L)
  {
    return 0;
  }
  else
  {
    strNomeProj = strNomeProj.substr(0, strNomeProj.find_last_of('\\') + 1);
    do
    {
      lstNomeArquivos.push_back(strNomeProj + c_file.name);

      if ((lstNomeArquivos.rbegin())->find("_PontosEditadosUsuario") != -1)    //-- tem que ser o 1o arquivo da lista
      {
        itArqPontosInseridosUsuario = --(lstNomeArquivos.end());
      }

    } while (++i < 1000 && _findnext(hFile, &c_file) == 0);

    //---Coloca o arquivo de pontos inseridos pelo usuaario como o primeiro da lista

    if (itArqPontosInseridosUsuario != lstNomeArquivos.end())
    {
      std::string strNomeArquivo(*itArqPontosInseridosUsuario);
      lstNomeArquivos.erase(itArqPontosInseridosUsuario);
      lstNomeArquivos.push_front(strNomeArquivo);
    }

    _findclose(hFile);
  }

  return i;
}

bool CSuperficie::InserirPontoIntersecao(const Ponto& P1, const Ponto& P2, ItCLLDeltaSup ItLDRestricao1, ItCLDeltaSup ItRestricao2P1, ItCLDeltaSup ItRestricao2P2, Ponto& Intersecao, ItLLDeltaSup* pItLDRestricao2,ItLDeltaSup* pItP1)
{
  Ponto PCota1(Intersecao),PCota2(Intersecao);

  if (CalculaCota(P1,P2, PCota1) && CalculaCota(ItRestricao2P1->PCentral, ItRestricao2P2->PCentral, PCota2))
  {
    Intersecao.z = PCota1.z = PCota2.z = (PCota1.z + PCota2.z) /2.0;
   (const_cast<LDeltaSup*>(&*ItLDRestricao1))->insert(ItRestricao2P2,PCota1);
   (const_cast<LDeltaSup*>(&*(*pItLDRestricao2)))->insert(*pItP1,PCota2);

    return true;
  }
  return false;
}

bool CSuperficie::VerfPontoRestricao(const Ponto& Ponto, ItLLDeltaSup& ItLLDS, ItLDeltaSup& itPonto)
{
  for (ItLLDeltaSup ItLRestricao = ListaRestricoes.begin(); ItLRestricao != PegaListaRestricoes().end(); ++ItLRestricao)
  {
    itPonto = std::find(ItLRestricao->begin(), ItLRestricao->end(), Ponto);

    if (itPonto != ItLRestricao->end())
    {
      if(ItLRestricao->size() == 2)
      {
        RemoveListaDoMapa(ItLRestricao, CAciTopografico::NENHUM_ACITOPOG);
        ListaRestricoes.remove(*ItLRestricao);
        return false;
      }

      ItLLDS = ItLRestricao;
      return true;
    }
  }

  ItLLDS = ListaRestricoes.end();

  return false;
}

bool CSuperficie::VerfFronteira(SSetPontos& PontosProblema, bool MostrarMensagem, int* pResposta)
{
  size_t QtdFronteiras(MMapItLLDeltaSup.count(CAciTopografico::FRONTEIRA));
  paitititmmapitLLDeltaSup itLLFaixaFronteira(MMapItLLDeltaSup.equal_range(CAciTopografico::FRONTEIRA));

  if (QtdFronteiras != 1)
  {
    if (QtdFronteiras != 0)
    {
      for (itmmapitLLDeltaSup itmapLLF = itLLFaixaFronteira.first; itmapLLF != itLLFaixaFronteira.second; ++itmapLLF)
      {
        PontosProblema.insert(itmapLLF->second->begin()->PCentral);
        PontosProblema.insert(itmapLLF->second->rbegin()->PCentral);
      }
    }

    if (MostrarMensagem)
    {
      if (QtdFronteiras == 0) AfxMessageBox("A fronteira não está definida.");
      else AfxMessageBox("Há mais de um segmento de pontos determinando a fronteira.");
    }

    if (pResposta != NULL) *pResposta = (int) QtdFronteiras;

    return false;
  }

  citmmapitLLDeltaSup itLLFronteira(MMapItLLDeltaSup.find(CAciTopografico::FRONTEIRA));

 // if (itLLFronteira->second->size() < 3 || *itLLFronteira->second->begin() != *itLLFronteira->second->rbegin())
  if (itLLFronteira->second->size() < 3 || 
  !itLLFronteira->second->begin()->PegaPCentral().Compara2D(itLLFronteira->second->rbegin()->PegaPCentral()))

  {
    if (MostrarMensagem) AfxMessageBox("A fronteira não está Fechada.");

    PontosProblema.insert(itLLFronteira->second->begin()->PCentral);
    PontosProblema.insert(itLLFronteira->second->rbegin()->PCentral);

    if (pResposta != NULL) *pResposta = 1;

    return false;
  }

  return true;
}

/*
void CSuperficie::CriaListaBordos()  //--- Para compatibilidade com versões anteriores
{
ListaBordos.clear();

paitititmmapitLLDeltaSup PairItIt(MMapItLLDeltaSup.equal_range(CAciTopografico::FRONTEIRA));

while(PairItIt.first != PairItIt.second)
{
ItCLDeltaSup itCLDS(PairItIt.first->second->begin());

ListaBordos.push_back(LDeltaSup(1,*PairItIt.first->second->begin()));

for (register ItCLDeltaSup itCLDS = ++(PairItIt.first->second->begin()) ; itCLDS !=  PairItIt.first->second->end() ; ++itCLDS)
{
ListaBordos.back().push_back(*itCLDS);
}

++PairItIt.first;
}
}
*/

bool CSuperficie::VerfInsercaoSgmento(Ponto P1, Ponto P2, int Tipo)
{
  paitititmmapitLLDeltaSup PairItIt(MMapItLLDeltaSup.equal_range(Tipo));

  while (PairItIt.first != PairItIt.second)
  {
    ItCLDeltaSup itCLDS(std::find(PairItIt.first->second->begin(), PairItIt.first->second->end(), P1));

    if (itCLDS != PairItIt.first->second->end())
    {
      ItCLDeltaSup itAnterior(itCLDS);

      if ((itAnterior != PairItIt.first->second->begin() && *--itAnterior == P2) || (++itCLDS != PairItIt.first->second->end() && *itCLDS == P2))
      {
        AfxMessageBox("Este segmento já existe neste acidente topográfico.");

        return false;
      }
    }
    ++PairItIt.first;
  }
  return true;
}

int CSuperficie::LePontos(char* pBufferArq)
{
  tylstNomeArquivos lstNomeArquivos;

  MaiorCota = -INFINITO;

  if (PegaNomesArquivosTerreno(lstNomeArquivos, NomeProjeto))
  {
    CConfigProj ConfigProjeto(NULL, NomeProjeto.c_str());
    ConfigProjeto.LeArquivo();
    int Erro = false;

    for (tyitlstNomeArquivos itNomeArqAtual = lstNomeArquivos.begin(); !Erro && itNomeArqAtual != lstNomeArquivos.end(); ++itNomeArqAtual)
    {
      std::ifstream ArqPontos(*itNomeArqAtual);
      ArqPontos.rdbuf()->pubsetbuf(pBufferArq, 102400000);
      if (ArqPontos.is_open())
      {
        unsigned int ContaLinhas(0);
        try
        {
          bool ArqPontosEditados(itNomeArqAtual->find("_PontosEditadosUsuario") != -1),
               PontosDesabilitados(false);

          while (ArqPontos.good())
          {
            std::string strLinha, strNome, strBufferx, strBuffery, strBufferz, strDescricao;
            static std::string strMarcasComentarios("/!@#$%&*");

            ++ContaLinhas;

            std::getline(ArqPontos, strLinha); 

           // ArqPontos.read(BufferKK, 100024000);

            //continue;

            if (0 && ContaLinhas > 30000)   //--- A leitura deste arquivo é fake.... :)
            {
              ArqPontos.setstate(std::ios_base::eofbit);
              break;
            }

            if (!FindAnyCharS1S2(strLinha, strMarcasComentarios))
            {
              std::stringstream strstrBuffer(strLinha);

              if (!strstrBuffer.eof())
              {
                strstrBuffer >> strNome;

                if (!strstrBuffer.eof())
                {
                  strstrBuffer >> strBuffery;

                  if (!strstrBuffer.eof())
                  {
                    strstrBuffer >> strBufferx;

                    if (!strstrBuffer.eof())
                    {
                      strstrBuffer >> strBufferz;

                      if (!strstrBuffer.eof())
                      {
                        strstrBuffer >> strDescricao;
                      }
                    }
                  }
                }
              }

            //  strstrBuffer >> strNome >> strBufferx >> strBuffery >> strBufferz;
            //  if(!strstrBuffer.eof()) strstrBuffer >> strDescricao;

              if(!PontosDesabilitados) 
              {
                if (ConfigProjeto.FormArqPontos != "n N E C O") AcertaCamposLer(ConfigProjeto.FormArqPontos, strNome, strBuffery, strBufferx, strBufferz, strDescricao);

                if (!strBuffery.empty() && !strBufferx.empty() && !strBufferz.empty())    //--- linha em branco
                {
                  if (Mat.ENumero(strBuffery) && Mat.ENumero(strBufferx) && Mat.ENumero(strBufferz))
                  {
                    double x(atof(strBufferx.c_str())), y(atof(strBuffery.c_str())), z(atof(strBufferz.c_str()));

                    CDeltaSuperficie CD(atof(strBufferx.c_str()), atof(strBuffery.c_str()), atof(strBufferz.c_str()), INFINITO, INFINITO, INFINITO, CString(strDescricao.c_str()), 0, 0, CString(strNome.c_str()));

                    pSuperficieAtual->insert(CD);

                    if (MaiorCota < z) MaiorCota = z;
                  }
                  else
                  {
                    if (!ArqPontos.eof())
                    {
                      std::strstream strMensagem;

                      strMensagem << "Erro ao ler o arquivo " << *itNomeArqAtual << " na linha " << ContaLinhas << std::endl
                        << "Os valores  não puderam ser convertidos para números." << std::endl
                        << "Verifique o separador das casas decimais, dever ser . (Ponto)" << std::ends;

                      AfxMessageBox(strMensagem.str());

                      Erro = true;
                    }
                  }
                }
                else
                {
                  if (!strBuffery.empty() || !strBufferx.empty() || !strBufferz.empty())
                  {
                    std::strstream strMensagem;

                    strMensagem << "Erro ao ler o arquivo " << *itNomeArqAtual << " na linha " << ContaLinhas << std::endl
                                << "Falta algum campo obrigatório." << std::ends;

                    AfxMessageBox(strMensagem.str());

                    Erro = true;
                  }
                }
              }
            }
            else
            {
              if (ArqPontosEditados || strDescricao.find("Desabilitado") != std::string::npos)
              {
                PontosDesabilitados = true;  //--- pontos desabilitados são descartados (São os ultimos do arquivo)
              }
            }
          }

          if (!ArqPontos.eof())
          {
            std::strstream strMensagem;
            strMensagem << "Erro ao ler o arquivo " << *itNomeArqAtual << " na linha " << ContaLinhas << std::ends;
            AfxMessageBox(strMensagem.str());

            Erro = true;

          }
        }
        catch (...)
        {
          std::strstream strMensagem;
          strMensagem << "Erro ao ler o arquivo " << *itNomeArqAtual << " na linha " << ContaLinhas << std::ends;
          AfxMessageBox(strMensagem.str());

          Erro = true;
        }
      }
      else
      {
        std::strstream strMensagem;
        strMensagem << "Erro ao abrir o arquivo " << *itNomeArqAtual << std::ends;
        AfxMessageBox(strMensagem.str());

        Erro = true;
      }
    }

    return !Erro;
  }

  /*
  std::strstream strMensagem;
  strMensagem << "Nenhum arquivo de terreno foi encontrado para o projeto: " << NomeProjeto << std::ends;
  AfxMessageBox(strMensagem.str());
  */

  return true;
}

void CSuperficie::MontaChaves()
{
  for (ItSSuperficie it = pSuperficieAtual->begin(); it != pSuperficieAtual->end(); ++it)
  {
    CDeltaSuperficie DS(*it);
    DS.MontaChave();
    pOutraSuperficie->insert(DS);
  }

  SSuperficie * pTemp(pSuperficieAtual);

  pSuperficieAtual->clear();
  pSuperficieAtual = pOutraSuperficie;
  pOutraSuperficie = pTemp;
}

void CSuperficie::Constroi()
{
  Serialize(NomeProjeto.c_str(), LER,false, false);
  
  RedefineLimites();  //--- não deveria chamar aqui, os limites deveriam ser acertados a medida que os pontos são lidos

  AngRotacaoTopInt = AnguloDeRotacao * 180.0 / Mat.M_PI + .5;
  if (AngRotacaoTopInt > 0) AngRotacaoTopInt = 360 - AngRotacaoTopInt;
}

double CSuperficie::CalculaArea()
{
  Area = 0.0;

  for (ItCLLDeltaSup LDRestricaoAtual = PegaListaRestricoes().begin(); LDRestricaoAtual != PegaListaRestricoes().end(); ++LDRestricaoAtual)
  {
    unsigned int TipoRestricao(VerfAciTopografico(LDRestricaoAtual));

    if (TipoRestricao == CAciTopografico::FRONTEIRA || TipoRestricao == CAciTopografico::FRONTEIRA_TERRAP)
    {
      register ItCLDeltaSup ItLRestricaoAnterior((*LDRestricaoAtual).begin()), ItLRestricaoAtual(ItLRestricaoAnterior);

      for (++ItLRestricaoAtual; ItLRestricaoAtual != (*LDRestricaoAtual).end(); ++ItLRestricaoAnterior, ++ItLRestricaoAtual)
      {
        Area += (ItLRestricaoAnterior->PCentral.y + ItLRestricaoAtual->PCentral.y) * (ItLRestricaoAtual->PCentral.x - ItLRestricaoAnterior->PCentral.x) / 2.0;
      }
      break;
    }
  }

  return Area;
}

double CSuperficie::CalculaPerimetro()
{
  Perimetro = 0.0;

  for (ItCLLDeltaSup LDRestricaoAtual = PegaListaRestricoes().begin(); LDRestricaoAtual != PegaListaRestricoes().end(); ++LDRestricaoAtual)
  {
    unsigned int TipoRestricao(VerfAciTopografico(LDRestricaoAtual));

    if (TipoRestricao == CAciTopografico::FRONTEIRA || TipoRestricao == CAciTopografico::FRONTEIRA_TERRAP)
    {
      register ItCLDeltaSup ItLRestricaoAnterior((*LDRestricaoAtual).begin()), ItLRestricaoAtual(ItLRestricaoAnterior);

      for (++ItLRestricaoAtual; ItLRestricaoAtual != (*LDRestricaoAtual).end(); ++ItLRestricaoAnterior, ++ItLRestricaoAtual)
      {
        Perimetro += hypot(ItLRestricaoAnterior->PCentral.y - ItLRestricaoAtual->PCentral.y,ItLRestricaoAtual->PCentral.x - ItLRestricaoAnterior->PCentral.x);
      }
      break;
    }
  }

  return Perimetro;
}

void CSuperficie::InverteFronteira(int TipoFronteira)
{
  for (ItLLDeltaSup LDRestricaoAtual = PegaListaRestricoes().begin(); LDRestricaoAtual != PegaListaRestricoes().end(); ++LDRestricaoAtual)
  {
    unsigned int TipoRestricao(VerfAciTopografico(LDRestricaoAtual));

  //  if (TipoRestricao == CAciTopografico::FRONTEIRA || TipoRestricao == CAciTopografico::FRONTEIRA_TERRAP)
    if (TipoRestricao == TipoFronteira)
    {
      std::reverse(LDRestricaoAtual->begin(), LDRestricaoAtual->end());
      Area *= -1.0;
      bInverteuFronteira = true;
      break;
    }
  }
}

/*
void CSuperficie::ReleSuperficie()
{
  ListaRestricoes;
  &ListaBordos
}
*/

int CSuperficie::PegaNomesArquivosPIS(tysetNomeArquivos& setNomeArquivos, CString NomeProjeto)
{
  struct _finddata_t c_file;
  intptr_t hFile;
  int i(0);

  CString NomeArquivoBase(NomeProjeto.Mid(0, NomeProjeto.ReverseFind('\\') + 1));

  setNomeArquivos.clear();

  if ((hFile = _findfirst((char*)LPCTSTR(NomeArquivoBase + "*PonInsSec*.xyz"), &c_file)) == -1L) return 0;  //--- Arquivos PIS
  else
  {
    do
    {
      setNomeArquivos.insert((NomeArquivoBase + c_file.name).GetBuffer());
    } while (++i < 1000 && _findnext(hFile, &c_file) == 0);

    _findclose(hFile);
  }

  return i;
}

bool CSuperficie::LeArqPIS(CString& NomeProjBase)    //--- Pontos Incluidos seções
{
  //--- Se foram incluidos pontos nas seções e a superfície ainda não foi recalculada eses pontos não estariam visiveis pois ainda não fazem parte da superfície
  //--- Le este arquivo e coloca os pontos numa lista para que sejam visualizados.

  tysetNomeArquivos setNomeArquivosPIS;   //--- PIS: Pontos Inseridos nas Seções

  if (PegaNomesArquivosPIS(setNomeArquivosPIS, NomeProjBase))
  {
    for (tyditsetNomeArquivos itNomeArqAtual = setNomeArquivosPIS.begin(); itNomeArqAtual != setNomeArquivosPIS.end(); ++itNomeArqAtual)
    {
      std::ifstream ArqPontos(itNomeArqAtual->c_str());
      std::stringstream strstrMsg;

      if (ArqPontos.is_open())
      {
        int ContaLinhas(0);
        static std::string strMarcasComentarios("/!@#$%&*");
        bool ArqPontosEditados(false), PontosDesabilitados(false), PontosInseridos(false), PontosRemovidos(false);

        while (ArqPontos.good())
        {
          std::string strBufferNome, strBufferx, strBuffery, strBufferz, strBufferObs;

          ++ContaLinhas;
          PulaBrancos(ArqPontos);                        //--- Pula os  brancos no início do arquivo
          GetLine(ArqPontos, strBufferNome, " \t\x0A");  //--- Nome do ponto

          if (strBufferNome.size() == 0) 
          {
            if (!ArqPontos.eof()) 
            {
              std::getline(ArqPontos, strBufferNome);
            }
            continue;
          }

          if (strMarcasComentarios.find(*strBufferNome.begin()) != std::string::npos)
          {
            std::getline(ArqPontos, strBufferNome);

            if(strBufferNome.find("Removidos") != std::string::npos)
            {
              PontosRemovidos = true; 
            }
            if (strBufferNome.find("Edição") != std::string::npos)    //--- Reinicia 
            {
              PontosRemovidos = false;
            }

            continue;
          }

          PegaProximoAlfaNum(ArqPontos);
          GetLine(ArqPontos, strBuffery, "  \t\x0A");   //--- y
          PegaProximoAlfaNum(ArqPontos);
          GetLine(ArqPontos, strBufferx, "  \t\x0A");   //--- x
          PegaProximoAlfaNum(ArqPontos);
          GetLine(ArqPontos, strBufferz, "  \t\x0A");   //--- z
          PegaProximoAlfaNum(ArqPontos);
          std::getline(ArqPontos, strBufferObs);      //--- Obs

          if (!strBuffery.empty() || !strBufferx.empty() || !strBufferz.empty())
          {
            if (!Mat.ENumero(strBufferx) || !Mat.ENumero(strBuffery) || !Mat.ENumero(strBufferz))
            {
              strstrMsg.seekg(0, strstrMsg.beg);
              strstrMsg << "Erro na leitura do arquivo - " << strBufferNome << " Linha = " << ContaLinhas << std::endl << " Abortando..." << std::endl << "Veja o arquivo de log" << std::ends;
              strstrMsg << "Dados Lidos x =" << strBufferx << " y=" << strBuffery << " z=" << strBufferz << " Obs=" << strBufferObs;
              AfxMessageBox(strstrMsg.str().c_str());

              return false;
            }
            else
            {
              double x(atof(strBufferx.c_str())), y(atof(strBuffery.c_str())), z(atof(strBufferz.c_str()));

              if(!PontosRemovidos) 
              {
                SetPontosInsSecoes.emplace(x, y, z, INFINITO, INFINITO, INFINITO, CString(strBufferObs.c_str()), false, true, CString(strBufferNome.c_str()), -1);
              }
              else LPontosRemSecoes.emplace_back(x, y, z, INFINITO, INFINITO, INFINITO, CString(strBufferObs.c_str()), false, true, CString(strBufferNome.c_str()), -1);
            }
          }
        }

        if (0 && !ArqPontos.eof())   //--- eof nao funciona!!!
        {
          strstrMsg.str("");
          strstrMsg << "Erro na leitura do arquivo - Linha => " << ContaLinhas << std::endl << std::ends;

          return false;
        }
      }
    }

    //--- Pontos removidos nas seções não são mostrados, 
    //--- só serão removidos efetivamente no proximo recálculo da superficie e
    //--- podem nem terem sido incluidos na superfície ainda
    //--- Geralmente o ponto é incluido em uma edição e excluido em outra, 
    //--- portanto estão em arquivos diferentes.

    if (!LPontosRemSecoes.empty())
    {
      for (itlstPontos it = LPontosRemSecoes.begin(); it != LPontosRemSecoes.end(); it++)
      {
        SetPontosInsSecoes.erase(*it);
        RemovePonto(*it);              //--- Se o ponto ja foi inserido na superfície
      }
    }
  }

  return true;
}

int CSuperficie::SerializarBacias(int Tipo,const std::string& NomeProjBase)
{
  CString Nom(NomeProjBase.c_str() + CString(".bah"));
  UINT Flags;
  if (Tipo != LER) Flags = CFile::modeCreate | CFile::modeWrite;
  else Flags = CFile::modeRead;  

  CFile Arq;
  CFileException e;

  if (Arq.Open(Nom, Flags, &e) == TRUE)
  {
    if(Tipo == GRAVAR)
    {
      CArchive CArcBacias(&Arq, CArchive::store);
      ItSSuperficie ItSSupAtual(pSuperficieAtual->begin());

      CArcBacias << AnguloDeRotacao;

      CArcBacias << BaciasHidrograficas.size();

      for (ittylstBacias it = BaciasHidrograficas.begin(); it != BaciasHidrograficas.end(); it++)
      {
        *it >> CArcBacias;
      }
    }
    else
    {
      BaciasHidrograficas.clear();

      size_t QuanBacias(0);
      CArchive CArcBacias(&Arq, CArchive::load);
      ItSSuperficie ItSSupAtual(pSuperficieAtual->begin());

      CArcBacias >> AnguloDeRotacao;

      for (CArcBacias >> QuanBacias ; QuanBacias > 0 ; --QuanBacias)
      {
        BaciasHidrograficas.push_back(CBacia());
        CBacia& BaciaAtual(*BaciasHidrograficas.rbegin());
      
        BaciaAtual << CArcBacias;
      }
    }
  }

  return 0;  
}

int CSuperficie::SerializarEquipDrenagem(int Tipo,const std::string& NomeProjBase)
{
  CString Nom(NomeProjBase.c_str() + CString(".edr"));
  UINT Flags;
  if (Tipo != LER) Flags = CFile::modeCreate | CFile::modeWrite;
  else Flags = CFile::modeRead;

  CFile Arq;
  CFileException e;

  if (Arq.Open(Nom, Flags, &e) == TRUE)
  {
    if (Tipo == GRAVAR)
    {
      CArchive CArcEquipDrenagem(&Arq, CArchive::store);

      CArcEquipDrenagem << EquipamentosDrenagem.size();

      for (ittymapEquipDrenagem It = EquipamentosDrenagem.begin(); It != EquipamentosDrenagem.end(); ++It)
      {
        CArcEquipDrenagem << It->first;
        It->second >> CArcEquipDrenagem;
      }
    }
    else
    {
      CArchive CArcEquipDrenagem(&Arq, CArchive::load);

      size_t Tam(0);
      std::string Nome;

      for (CArcEquipDrenagem >> Tam ; Tam ; --Tam)
      {
        CEquipDrenagem EquipAtual;

        CArcEquipDrenagem >> Nome;
        EquipAtual << CArcEquipDrenagem;
        EquipamentosDrenagem.emplace(Nome, EquipAtual);
      }
    }
  }

  return 0;
}

void CSuperficie::RotacionaEquipDrenagem(double Seno, double Cosseno, double Ang)
{
  for (auto& rEquipAtual : EquipamentosDrenagem)
  {
    rEquipAtual.second.RotacionaPlano(Seno,Cosseno,Ang);
  }
}

ItCLLDeltaSup CSuperficie::VerificaNaTangenteRestricoes(ItCLLDeltaSup ItCLLDeltaSupInicial, const Ponto& PAtual, 
                            double Toleranciax, ItCLDeltaSup* pItCLDS, double* pRaio, CSuperficie* pSuperfTerrap,
                            unsigned int ObjAtual,bool FiltrarAciTopog,bool VerifBordos)
{
  if (pItCLDS != NULL) *pItCLDS = ItCLLDeltaSupInicial->end();
  if (pRaio != NULL) *pRaio = 0.0;

  CSuperficie* pSuperfAtual(pSuperfTerrap ? pSuperfTerrap : this);

  const LLDeltaSup& ListaRestAtual(pSuperfAtual->PegaListaRestricoes());
  ItCLLDeltaSup itLRestricaoAtual(ListaRestAtual.end());

  for (itLRestricaoAtual = ItCLLDeltaSupInicial; itLRestricaoAtual != ListaRestAtual.end(); ++itLRestricaoAtual)
  {
    if (itLRestricaoAtual->size() > 1)
    {
      unsigned int TipoRestricao(pSuperfAtual->VerfAciTopografico(itLRestricaoAtual));
      bool EBordo(VerifBordos && TipoRestricao == 31 && itLRestricaoAtual->begin()->PegaPCentral().Descricao.Find("BORTE") != -1);

      if (!FiltrarAciTopog || TipoRestricao == ObjAtual || EBordo)
      {
        register ItCLDeltaSup itPontoRestricaoAtual(itLRestricaoAtual->begin());
        ItCLDeltaSup itPontoRestricaoAnterior(itPontoRestricaoAtual);

        bool NaTangente(false);

        while (!NaTangente && ++itPontoRestricaoAtual != itLRestricaoAtual->end())
        {
          double Raio(hypot(itPontoRestricaoAnterior->PegaPCentral().x - PAtual.x, itPontoRestricaoAnterior->PegaPCentral().y - PAtual.y));

          if (Raio < Toleranciax)  NaTangente = true;
          else
          {
            double DistPontos(hypot(itPontoRestricaoAtual->PegaPCentral().x - itPontoRestricaoAnterior->PegaPCentral().x, itPontoRestricaoAtual->PegaPCentral().y - itPontoRestricaoAnterior->PegaPCentral().y));

            if (Raio < (DistPontos + Toleranciax))
            {
              double AzReta(Mat.CalculaAzimute(itPontoRestricaoAtual->PegaPCentral(), itPontoRestricaoAnterior->PegaPCentral())),
                     AzPonto(Mat.CalculaAzimute(PAtual, itPontoRestricaoAnterior->PegaPCentral()));

              if (fabs(AzReta - AzPonto) < Matem::M_PI_2 / 4.0)
              {
                double Distancia(fabs(sin(AzPonto - AzReta) * Raio));
                NaTangente = Distancia < Toleranciax;
              }
            }
          }

          if (NaTangente)
          {
            if (pItCLDS != NULL) *pItCLDS = itPontoRestricaoAnterior;
            if (pRaio != NULL) *pRaio = Raio;
          }
          itPontoRestricaoAnterior = itPontoRestricaoAtual;
        }

        if (NaTangente) 
        {
          return itLRestricaoAtual;
        }
      }
    }
    else
    {
      double Raio(hypot(itLRestricaoAtual->begin()->PegaPCentral().x - PAtual.x, itLRestricaoAtual->begin()->PegaPCentral().y - PAtual.y));

      if (Raio < Toleranciax) return itLRestricaoAtual;
    }
  }

  return pSuperfAtual->PegaListaRestricoes().end();
}


void CSuperficie::SerializaCNSimbolTerraplanagem(int Tipo, const std::string& NomeProjBase)
{
  CFile Arq, Arq2;
  CFileException e,e2;
  unsigned long Flags;

  std::string Nom = NomeProjBase.c_str() + CString(".cns");
  if (Tipo != LER) Flags = CFile::modeCreate | CFile::modeWrite;
  else Flags = CFile::modeRead;

  if (Arq.Open(Nom.c_str(), Flags, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      monolog.mensagem(1,Nom.c_str());
      return;
    }
  }

  if (Tipo == LER)
  {
    CString NomeCNDrone(NomeProjBase.c_str() + CString(".cnd"));

    DWORD dwAttrib(GetFileAttributes(NomeCNDrone.GetBuffer()));
    bool TemCNDrone(dwAttrib != INVALID_FILE_ATTRIBUTES && (dwAttrib & FILE_ATTRIBUTE_ARCHIVE));

    if (TemCNDrone)
    {
      if (Arq2.Open(NomeCNDrone.GetBuffer(), CFile::modeRead, &e2) == 0)
      {
        if (e2.m_cause != CFileException::fileNotFound)
        {
          monolog.mensagem(1, Nom.c_str());
          return;
        }
      }
    }

    CArchive CArcCurvasNivel(TemCNDrone ? &Arq2 : &Arq, CArchive::load);
    size_t QtdCurvasNivel(0), QtdPontosCNAtual(0);

    if ((TemCNDrone && e2.m_cause != CFileException::fileNotFound) || (!TemCNDrone && e.m_cause != CFileException::fileNotFound))   //--- Se Existe o arquivo
    {
      LLPontosCNSimbTerrapl.clear();

      CArcCurvasNivel >> AngRotacaoBordos >> EquidisCN;

      for (CArcCurvasNivel >> QtdCurvasNivel; QtdCurvasNivel; QtdCurvasNivel--)
      {
        LLPontosCNSimbTerrapl.push_back(LPontos());
        LPontos& LCurvaNivelAtual(LLPontosCNSimbTerrapl.back());
        size_t QtdPontosCNAtual(0);
        int Cont(0);
        Ponto PAnterior, PAtual;

        for (CArcCurvasNivel >> QtdPontosCNAtual; QtdPontosCNAtual; QtdPontosCNAtual--)    //--- Para todas as CNs
        {
          PAtual << CArcCurvasNivel;

          //--- Interpola um ponto no meio para desenhar as curvas topográficas

          if (!TemCNDrone && Cont > 1 && Cont % 2 == 1)
          {
            Ponto PInterpolado((PAtual.x + PAnterior.x) / 2.0, (PAtual.y + PAnterior.y) / 2.0, PAtual.z);
            PInterpolado.Baliza = true;               //--- Indica que o ponto foi interpolado
            LCurvaNivelAtual.push_back(PInterpolado);
          }

          LCurvaNivelAtual.push_back(PAtual);
          PAnterior = PAtual;
          Cont++;
        }
        if (!TemCNDrone && Cont % 2 == 1)
        {
          ItLPontos itPontoCN(LCurvaNivelAtual.end());
          itPontoCN--;
          if (itPontoCN != LCurvaNivelAtual.begin()) itPontoCN--;

          Ponto PInterpolado((PAtual.x + itPontoCN->x) / 2.0, (PAtual.y + itPontoCN->y) / 2.0, PAtual.z);

          LCurvaNivelAtual.pop_back();
          LCurvaNivelAtual.push_back(PInterpolado);
          LCurvaNivelAtual.push_back(PAtual);
        }
        if (LCurvaNivelAtual.size() == 0)
        {
          LLPontosCNSimbTerrapl.pop_back();
        }
      }
    }
  }
  else
  {
    CArchive CArcCurvasNivel(&Arq, CArchive::store);
    unsigned int QtdCurvasNivel(0);

    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      CArcCurvasNivel << AngRotacaoBordos << EquidisCN;
      CArcCurvasNivel << LLPontosCNSimbTerrapl.size();

      ItLLPontos itLLCN = LLPontosCNSimbTerrapl.begin();

      for (ItLLPontos itLLCN = LLPontosCNSimbTerrapl.begin(); itLLCN != LLPontosCNSimbTerrapl.end(); itLLCN++)
      {
        LPontos LPontosCNTemp(itLLCN->begin(), itLLCN->end());
        LITLPontos LITPontosCNRemover;

        for (register ItLPontos itLCNTemp = LPontosCNTemp.begin(); itLCNTemp != LPontosCNTemp.end(); itLCNTemp++)
          if (itLCNTemp->Baliza == true) LITPontosCNRemover.push_back(itLCNTemp);

        for (register ItLITLPontos itLCNRem = LITPontosCNRemover.begin(); itLCNRem != LITPontosCNRemover.end(); itLCNRem++)
          LPontosCNTemp.erase(*itLCNRem);

        CArcCurvasNivel << LPontosCNTemp.size();

        ItLPontos ItPAnterior(LPontosCNTemp.begin());

        for (ItLPontos itLCN = LPontosCNTemp.begin(); itLCN != LPontosCNTemp.end(); itLCN++)
        {
          if (itLCN != LPontosCNTemp.begin())
          {
            itLCN->fi = Mat.CalculaAzimute(*ItPAnterior, *itLCN);
            ItPAnterior = itLCN;
          }
          *itLCN >> CArcCurvasNivel;
        }
      }
    }
  }
}

void CSuperficie::SeccionaPelaCN(const Ponto& PIni, const Ponto& PCentral,const Ponto& PFim, setPontos& SetSecao,bool EixoX, std::ofstream& ArqLog)
{
  SetSecao.clear();
  tylstpontos LPontosInter;

  
  ArqLog << "PINI: " << PIni.x << " " << PIni.y << " " << PIni.z << std::endl;
  ArqLog << "PCENTRAL: " << PCentral.x << " " << PCentral.y << " " << PCentral.z << std::endl;
  ArqLog << "PFIM: " << PFim.x << " " << PFim.y << " " << PFim.z << std::endl;


  ArqLog << "Pontos do Intersecao: " << std::endl;

  //--- Calcula as interseções pela CN

  for (ItLLPontos itLLCN = LPontosCN.begin(); itLLCN != LPontosCN.end(); itLLCN++)
  {
    if(itLLCN->size() > 1)
    {
      Ponto PInters;
        
      ItLPontos itLPontoAnt(itLLCN->begin()), itLPontoAtual(itLPontoAnt);

      for (++itLPontoAtual ; itLPontoAtual != itLLCN->end() ; ++itLPontoAnt,++itLPontoAtual)
      {
        if (CLPontos::CalculaIntersecao(PIni, PFim, *itLPontoAnt, *itLPontoAtual,PInters))
        {
          PInters.z = itLPontoAnt->z;       //--- Cota da CN
          LPontosInter.push_back(PInters);

          ArqLog << PInters.x << " " << PInters.y << " " << PInters.z << std::endl;
        }
      }
    }
  }

  //--- Ordena os pontos por distancia

  Ponto PRIniFim(Mat.R_P(PIni, PFim));

  for(ittylstpontos It = LPontosInter.begin() ; It != LPontosInter.end() ; ++It)
  {
    Ponto RP(Mat.R_P(PCentral,*It));

    //--- Verifica se o ponto está do lado esquerdo.

    if(EixoX && It->x < PCentral.x)  RP.y *= -1.0;
    if(!EixoX && It->y < PCentral.y)  RP.y *= -1.0;

    //if(!(fabs(RP.x - PRIniFim.x) < 1E-4)) RP.y *= -1.0; 

    //--- Os ponto são ordenados por xy

    It->S = It->x;
    It->x = RP.y; 

    SetSecao.insert(*It);
  } 
}

bool CSuperficie::LeArqPontosSoltosDGN()
{
  std::string NomeArq(NomeProjeto);

  NomeArq += ".pos";

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
  {
    return false;
  }
  else
  {
    CArchive CArcPontosSoltos(&Arq, CArchive::load);

    if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
    {
      double AngRotacao;
      size_t QtdPontos;
      Ponto PSolto;

      SetPontosSoltosDGN.clear();

      CArcPontosSoltos  >> AngRotacao;                 //--- Angulo rotacao 

      for (CArcPontosSoltos >> QtdPontos ; QtdPontos != 0 ; QtdPontos--)
      {
        PSolto << CArcPontosSoltos;
        if(PSolto.AcidenteTopografico & Ponto::PONTO_COTADO)
          SetPontosSoltosDGN.emplace(PSolto);
      }
    }
  }
  return true;
}

/*
void CSuperficie::LeSetForcarAresta()
{
  std::string NomeArq(NomeProjeto);
  NomeArq += ".rav";

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
  {
    std::string NomeArq(NomeProjeto);

    NomeArq += ".ato";

    if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
    {
      //--- Arquivo de acidentes topog não existe
      return;
    }
  }

  CArchive CArcAciTopog(&Arq, CArchive::load);
  {
    SetForcarAresta.emplace(0, 0);
    unsigned int QuanReg;

    for(CArcAciTopog >> QuanReg ; QuanReg ; QuanReg--) 
    {
       std::string Campo,CampoAnterior;
       size_t QuanCampos;
       int C(0);

       for (CArcAciTopog >> QuanCampos ; QuanCampos ; QuanCampos--,C++)
       {
         if(C == 2) CArcAciTopog >> CampoAnterior;
         else CArcAciTopog >> Campo;
       }

       int AciTopog(atoi(CampoAnterior.c_str())),
           ForcaAresta(atoi(Campo.c_str()));

       AciTopog++;    //--- 0 É NENHUM_ACI_TOPOG

      SetForcarAresta.emplace(AciTopog, ForcaAresta);
    }
  }
}
*/

void CSuperficie::LeSetForcarAresta()
{
  std::string NomeArq(NomeProjeto);
  NomeArq += ".rav";

  //void DesPontosGeomView::LeSetForcaAresta(const std::string& NomeProjeto)
  //{
  bool ArqAto(false);

  //std::string NomeArq(NomeProjeto);
  //NomeArq += ".rav";

  CFile Arq;
  CFileException e;

  if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
  {
    ArqAto = true;

    std::string NomeArq(NomeProjeto);

    NomeArq += ".ato";

    if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
    {
      //--- Arquivo de acidentes topog não existe
      return;
    }
  }

  CArchive CArcAciTopog(&Arq, CArchive::load);
  {
    unsigned int QuanReg;

    for (CArcAciTopog >> QuanReg; QuanReg; QuanReg--)
    {
      std::string Campo, CampoAnterior;
      size_t QuanCampos;
      int C(0);

      for (CArcAciTopog >> QuanCampos; QuanCampos; QuanCampos--, C++)
      {
        if (ArqAto)
        {
          if (C == 0) CArcAciTopog >> CampoAnterior;
          else CArcAciTopog >> Campo;
  }
        else
        {
          if (C == 2) CArcAciTopog >> CampoAnterior;
          else CArcAciTopog >> Campo;
        }
      }

      int AciTopog(atoi(CampoAnterior.c_str())),
          ForcaAresta(atoi(Campo.c_str()));

      SetForcarAresta.emplace(++AciTopog, ForcaAresta);
    }
  }
  /* Nova Versão - AGUARDAR TESTES  09-10-2023
  
   SetForcarAresta.clear();

  std::set<int> SetTrue = {3,7,9,10,17,19,20,22,27,29,30,31};

  std::pair<int, int> PairAtual;

  for (int C = 1; C < 38; C++)
  {
   PairAtual.first = C;
   PairAtual.second = SetTrue.find(C) != SetTrue.end();;
   
  }

  SetForcarAresta.erase(std::pair<int, int>(8, 0));
  SetForcarAresta.insert(std::pair<int, int>(8, 100));

  SetForcarAresta.erase(std::pair<int, int>(11, 0));
  SetForcarAresta.insert(std::pair<int, int>(11, 100));
  */
  
  
}

//--- Esta rotina deve ser usada quando o ponto a procurar não foi obtido da mesma 
//--- fonte de pontos da superficie, o que causa pequenas diferenças nos valores de x e y
//--- e faz com que o ponto não seja encontrado na superfície

ItSSuperficie CSuperficie::Find(Ponto PontoProcurar,double MenorDistancia)
{
 // return pSuperficieAtual->end();

  ItSSuperficie itDSupFinal(pSuperficieAtual->end()), itDSupAtual(itDSupFinal),
                ItPontoMaisProximo(pSuperficieAtual->end());
  double MenorDistanciaAtual(INFINITO);

  if (Rotacionada)
  {
   // PontoProcurar.RotacionaPlano(sin(-AnguloDeRotacao), cos(-AnguloDeRotacao), -AnguloDeRotacao);
  }

  //--- Procura nos pontos da superficie

  PontoProcurar.x -= MenorDistancia;
  itDSupAtual = pSuperficieAtual->upper_bound(PontoProcurar);
  PontoProcurar.x += MenorDistancia * 2.0;
  itDSupFinal = pSuperficieAtual->upper_bound(PontoProcurar);
  PontoProcurar.x -= MenorDistancia;

  //--- Separa o ponto que está mais próximo de PontoProcurar
  //-- Se colocar este if dá erro.. porque??? VERIFICAR 21042023 - TesteDrone3

  if(itDSupAtual != pSuperficieAtual->end())
  {
    do
    {
      Ponto PAtual((*itDSupAtual).PegaPCentral());

      if(fabs(PAtual.y- PontoProcurar.y) < MenorDistancia)
      {
        double DistanciaAtual(hypot(PAtual.x - PontoProcurar.x, PAtual.y - PontoProcurar.y));

        if (DistanciaAtual < MenorDistanciaAtual)
        {
          MenorDistanciaAtual = DistanciaAtual;
          ItPontoMaisProximo = itDSupAtual;
        }
      }
	  //--- itDSupAtual não poderia ficar null sem itDSupFinal ser null
	  //--- Mas aconteceu nos testes
    } while (++itDSupAtual != itDSupFinal && itDSupAtual != pSuperficieAtual->end());  
  }

  return  ItPontoMaisProximo;
}
void CSuperficie::AjustaPontosInseridosUsuario()
{
  //--- Os pontos inseridos passaram a ser usados nos acidentes topograficos digitalizados
  //--- como eles são arredondados para gravação no arquivo quando são lidos 
  //--- vão para o setpontosinseridosusuario com valores ligeiramente diferentes.
  //--- Devido a esta diferença do arredontamento não são mais encontrados na superficie
  //--- Esta rotina iguala os pontos do set com os pontos da superfície

  tylstitSSuperficie  LstItSSuperficie;
  lstPontos  LstPSuperfNaoEncontrado;

  for (auto& PontoAtual : SetPontosInseridos)
  {
    auto ItPSuperf(Find(PontoAtual,2e-3));

    if(ItPSuperf != pSuperficieAtual->end())
    {
      LstItSSuperficie.emplace_back(ItPSuperf);
    }
    else
    {
      LstPSuperfNaoEncontrado.emplace_back(PontoAtual);
    }
  }

  SetPontosInseridos.clear();

  for (auto& PLstItSuperf : LstItSSuperficie)
  {
    SetPontosInseridos.emplace(PLstItSuperf->PCentral);
  }

  for (auto& PLstPontoNaoEncontrado : LstPSuperfNaoEncontrado)
  {
    SetPontosInseridos.emplace(PLstPontoNaoEncontrado);
  }
}

void CSuperficie::PegaRestricoesNoPonto(const Ponto& pProcurar, lstItCLLDeltaSup& LstRestricoesNoPonto)
{
  paitititmmapitLLDeltaSup PairItIt(MMapItLLDeltaSup.equal_range(pProcurar.AcidenteTopografico));

  if(PairItIt.first == MMapItLLDeltaSup.end()) return;

  if(PairItIt.first->first != pProcurar.AcidenteTopografico) return;

  do
  {
    for (ItCLDeltaSup It = PairItIt.first->second->begin(); It != PairItIt.first->second->end(); It++)
    {
      if(It->PCentral.Compara2D(pProcurar))
      {
        LstRestricoesNoPonto.push_back(PairItIt.first->second);
      }
    }
  }while(++PairItIt.first != PairItIt.second);
}

void CSuperficie::IncluiTalveguesSISSuperficie()
{
  for (LDeltaSup& RestAtual : ListaRestricoes)
  {
    for (CDeltaSuperficie& DeltaSupAtual : RestAtual)
    {
      if (DeltaSupAtual.TipoObjeto == CAciTopografico::TALVEGUE)
      {
        DeltaSupAtual.PCentral.Descricao = "Talvegue";
        DeltaSupAtual.PCentral.Baliza2 = Ponto::ESBOCADO;

        InsereDeltaSuperficie(DeltaSupAtual);
      }
    }
  }
}

void CSuperficie::SerializaBaciasImportadas(int Tipo, const std::string& NomeProjBase)
{
  CString Nom(NomeProjBase.c_str() + CString(".bai"));
  UINT Flags;
  if (Tipo != LER) Flags = CFile::modeCreate | CFile::modeWrite;
  else Flags = CFile::modeRead;

  CFile Arq;
  CFileException e;

  if (Arq.Open(Nom, Flags, &e) == TRUE)
  {
    if (Tipo == GRAVAR)
    {
      CArchive CArcBaciasImp(&Arq, CArchive::store);

      CArcBaciasImp << AnguloDeRotacao;

      CArcBaciasImp << BaciasImportadas.size();

      for (Ponto& PAtual : BaciasImportadas)
      {
        PAtual >> CArcBaciasImp;
      }
    }
    else
    {
      BaciasImportadas.clear();

      size_t QuanBacias(0);
      Ponto BaciaImportada;

      CArchive CArcBaciasImp(&Arq, CArchive::load);

      CArcBaciasImp >> AnguloDeRotacao;

      for (CArcBaciasImp >> QuanBacias ; QuanBacias > 0 ; --QuanBacias)
      {      
        BaciaImportada << CArcBaciasImp;
        BaciasImportadas.emplace_back(BaciaImportada);
      }
    }
  }
}

void CSuperficie::SerializaTalveguesImportados(int Tipo, const std::string& NomeProjBase)
{
  CString Nom(NomeProjBase.c_str() + CString(".tai"));
  UINT Flags;
  if (Tipo != LER) Flags = CFile::modeCreate | CFile::modeWrite;
  else Flags = CFile::modeRead;

  CFile Arq;
  CFileException e;

  if (Arq.Open(Nom, Flags, &e) == TRUE)
  {
    if (Tipo == GRAVAR)
    {
      CArchive CArcTalvImp(&Arq, CArchive::store);

      CArcTalvImp << AnguloDeRotacao;

      CArcTalvImp << TalveguesImportados.size();

      for (Ponto& PAtual : TalveguesImportados)
      {
        PAtual >> CArcTalvImp;
      }
    }
    else
    {
      TalveguesImportados.clear();

      size_t QuanTalvegues(0);
      Ponto TalvegueImportado;

      CArchive CArcTalvImp(&Arq, CArchive::load);

      CArcTalvImp >> AnguloDeRotacao;

      for (CArcTalvImp >> QuanTalvegues; QuanTalvegues > 0; --QuanTalvegues)
      {
        TalvegueImportado << CArcTalvImp;
        TalveguesImportados.emplace_back(TalvegueImportado);
      }
    }
  }
}
#include <stack>

void CSuperficie::NormalizaTalvegues()
{
  tysetNosTalvegues SetNosTalveguesIniIni, SetNosTalveguesFimFim, SetNosTalveguesIniFim, SetNosTalveguesFimIni;
  tymsetItLDEltaSup SetItTalvegues;

  //--- Existem 4 casos de combinação
  //--- Inicio com Inicio (II)
  //--- Inicio com fim (IF)
  //--- Fim com Inicio (FI)
  //--- Fim com Fim (FF)
  //---
  //--- Cria um set com os pontos iniciais dos Talvegues
  //--- Cria o set com os nos:
  //--- Para Todos os talvegues:
  //--- Pega o proximo Talvegue
  //--- CORRIGIR ESTE ALGORITIMO
  //--- Existe Algum outro ponto no set de inicio que contem este ponto?
  //--- SIM:
  //---   Já existe algum talvegue neste neste ponto?
  //---   NAO:
  //---     Cria um no com este ponto e insere as 2 listas dos tamvegues neste no
  //---   SENAO
  //---     insere o talvegue no no
  //---   FIM SENAO
  //--- FIM SE
  //---  Funde os nos que tem so 2 listas criando uma nova lista de talvegues 
  //---   e removendo o talvegue que foi fundido
  /*
  Ponto PRaiz(100,150,300);

  CNoArvoreN NoRaiz(PRaiz),No1((110,160,310)), No2((120,170,320)), No3((130,180,330));
  NoRaiz.IncluiFilho(No1);
  NoRaiz.IncluiFilho(No2);
  NoRaiz.IncluiFilho(No3);

  CArvoreN Arvore(NoRaiz);
  */
  std::multiset<Ponto> MSetPontosNo;   //--- Set para contar o numero de pontos em cada no
  do
  {
    for (ItLLDeltaSup itMsetPontos = ListaRestricoes.begin(); itMsetPontos != ListaRestricoes.end(); itMsetPontos++)
    {
      if (itMsetPontos->size() < 2) continue;

      if (itMsetPontos->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
      {
        MSetPontosNo.insert(itMsetPontos->begin()->PCentral);
        MSetPontosNo.insert(itMsetPontos->rbegin()->PCentral);
      }
    }

    LimpaTalveguesZerados();

    bool Fim(false), FimGeral(true);

    do
    {
      //--- Cria os 4 sets com os nos (Inicio inicio, fim fim ,inicio fim e fim inicio):

      FimGeral = true;

      SetNosTalveguesIniIni.clear();
      SetNosTalveguesIniFim.clear();
      SetNosTalveguesFimIni.clear();
      SetNosTalveguesFimFim.clear();

      for (int C = INICIO_INICIO; C < FIM; C++)
      {
        switch (C)
        {
          case INICIO_INICIO:
          {
            bool Fim(false);

            while (!Fim)     //--- II (Inicio com Inicio)
            {
              for (ItLLDeltaSup it = ListaRestricoes.begin(); it != ListaRestricoes.end(); it++)
              {
                if (it->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
                {
                  auto Faixa(MSetPontosNo.equal_range(it->begin()->PCentral));

                  if (std::distance(Faixa.first, Faixa.second) != 2) continue;

                  //--- Cria o Set de início com início

                  CNoTalvegue NoTalvIniIniAtual(it, CNoTalvegue::INICIO);

                  ittysetNosTalvegues ItTalvAtual(SetNosTalveguesIniIni.find(NoTalvIniIniAtual));

                  if (ItTalvAtual != SetNosTalveguesIniIni.end())
                  {
                    const_cast<CNoTalvegue*>(&*ItTalvAtual)->IncluiPontoNo(it, CNoTalvegue::INICIO);  //--- Não corrompe o set 
                  }
                  else
                  {
                    SetNosTalveguesIniIni.insert(NoTalvIniIniAtual);
                  }
                }
              }

              size_t Contador(0);

              for (auto& ItNoTalvegueAtual : SetNosTalveguesIniIni)
              {
                if (ItNoTalvegueAtual.PegaListaPontosNo().size() == 2)
                {
                  auto LPontosNo(&ItNoTalvegueAtual.PegaListaPontosNo());

                  (*ItNoTalvegueAtual.PegaListaPontosNo().begin())->reverse();
                  (*(ItNoTalvegueAtual.PegaListaPontosNo().rbegin()))->pop_front();    //--- Ponto repetido nas 2  lists
                  (*(LPontosNo->begin()))->splice((*ItNoTalvegueAtual.PegaListaPontosNo().begin())->end(), *(*(ItNoTalvegueAtual.PegaListaPontosNo().rbegin())));

                  Contador++;
                  FimGeral = false;
                }
              }
              LimpaTalveguesZerados();         //--- Quando soma as 2 listas a somada fica com 1 elemento apenas e tem q ser excluída
              SetNosTalveguesIniIni.clear();

              Fim = Contador == 0;
            }
          }
          break;
          case INICIO_FIM:
          {
            Fim = false;

            while (!Fim)     //--- IF (Inicio com fim)   Cada elemento do set é um Inicio com Fim no mesmo ponto
            {
              for (ItLLDeltaSup it = ListaRestricoes.begin(); it != ListaRestricoes.end(); it++)
              {
                if (it->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
                {
                  //--- Cria o Set de início com fim
                  //--- Neste caso (inicio com fim) Tem q verificar as 2 extremidades

                  auto Faixa(MSetPontosNo.equal_range(it->begin()->PCentral));

                  if (std::distance(Faixa.first, Faixa.second) == 2)
                  {
                    CNoTalvegue NoTalvIniFimAtualIni(it, CNoTalvegue::INICIO);   //--- O primeiro nó é um início de lista

                    ittysetNosTalvegues ItTalvAtual(SetNosTalveguesIniFim.find(NoTalvIniFimAtualIni));

                    if (ItTalvAtual == SetNosTalveguesIniFim.end())
                    {
                      SetNosTalveguesIniFim.insert(NoTalvIniFimAtualIni);
                    }
                  }
                  else
                  {
                    //--- Verifica a outra extremidade

                    Faixa = MSetPontosNo.equal_range(it->rbegin()->PCentral);
                    if (std::distance(Faixa.first, Faixa.second) != 2) continue;

                    CNoTalvegue NoTalvIniFimAtualFim(it, CNoTalvegue::FIM);

                    ittysetNosTalvegues ItTalvAtual(SetNosTalveguesIniFim.find(NoTalvIniFimAtualFim));

                    if (ItTalvAtual != SetNosTalveguesIniFim.end())
                    {
                      if (ItTalvAtual->PegaFlagsPontosNo() == CNoTalvegue::INICIO)
                      {
                        const_cast<CNoTalvegue*>(&*ItTalvAtual)->IncluiPontoNo(it, CNoTalvegue::FIM);  //--- Não corrompe o set 
                      }
                    }
                    else
                    {
                      SetNosTalveguesIniFim.insert(NoTalvIniFimAtualFim);
                    }
                  }
                }
              }

              size_t Contador(0);

              for (ittysetNosTalvegues ItNoTalvegueAtual = SetNosTalveguesIniFim.begin(); ItNoTalvegueAtual != SetNosTalveguesIniFim.end(); ItNoTalvegueAtual++)
              {
                if (ItNoTalvegueAtual->PegaListaPontosNo().size() == 2)
                {
                  auto LPontosNo(&ItNoTalvegueAtual->PegaListaPontosNo());

                  auto& L1(*(*(ItNoTalvegueAtual->PegaListaPontosNo().begin())));
                  auto& L2(*(*(ItNoTalvegueAtual->PegaListaPontosNo().rbegin())));
                  int Caso(-1);

                  if (ItNoTalvegueAtual->PegaFlagsPontosNo() & CNoTalvegue::FIM)
                  {

                    L2.pop_front();                 //--- Ponto repetido nas 2 listas
                    L1.splice(L1.end(), L2);
                    Caso = 2;
                  }
                  else
                  {
                    L1.pop_front();                 //--- Ponto repetido nas 2 listas
                    L2.splice(L2.end(), L1);
                    Caso = 1;
                  }

                  //--- Neste caso, INICIO_FIM (e FIM_INICIO) pode ser que a reversão de uma das listas altere
                  //--- outro nó (O no onde estava o ponto que foi revertido). Por isso é preciso verificar se a lista
                  //--- que foi somada esta em outro nó. Se estiver é preciso acertar o iterador para a lista neste
                  //--- nó, pois ele mudou, é outra lista 

                  if (Caso == 1)
                  {
                    CNoTalvegue NoTalvegue(*ItNoTalvegueAtual->PegaListaPontosNo().rbegin(), CNoTalvegue::INICIO);

                    ittysetNosTalvegues NoAntigo(SetNosTalveguesIniFim.find(NoTalvegue));

                    if (NoAntigo != SetNosTalveguesIniFim.end() && NoAntigo->PegaListaPontosNo().size() == 2)
                    {
                      CNoTalvegue* NoAntigoNC = const_cast<CNoTalvegue*>(&*NoAntigo);

                      if (!(NoAntigoNC->PegaFlagsPontosNo() & (CNoTalvegue::FIM)))    //--- O fim é o primeiro ponto da lista de pontos no nó //--- ou é 01 ou 10 
                      {
                        //--- O primeiro ponto da lista de pontos no nó  é o fim da lista antiga, troca para o novo fim (que é o da lista que foi somada)

                        NoAntigoNC->TrocaInicio(*NoTalvegue.PegaListaPontosNo().rbegin());
                      }
                      else
                      {
                        //--- O segundo ponto da lista de pontos no nó  é o fim da lista antiga, troca para o novo fim (que é o da lista que foi somada)

                        NoAntigoNC->TrocaFim(*NoTalvegue.PegaListaPontosNo().rbegin());
                      }
                    }
                  }
                  else
                  {
                    CNoTalvegue NoTalvegue(*ItNoTalvegueAtual->PegaListaPontosNo().begin(), CNoTalvegue::FIM);

                    ittysetNosTalvegues NoAntigo(SetNosTalveguesIniFim.find(NoTalvegue));

                    if (NoAntigo != SetNosTalveguesIniFim.end() && NoAntigo->PegaListaPontosNo().size() == 2)
                    {
                      CNoTalvegue* NoAntigoNC = const_cast<CNoTalvegue*>(&*NoAntigo);

                      if (NoAntigoNC->PegaFlagsPontosNo() & (CNoTalvegue::FIM))    //--- O fim é o primeiro ponto da lista de pontos no nó //--- ou é 01 ou 10 
                      {
                        //--- O primeiro ponto da lista de pontos no nó  é o fim da lista antiga, troca para o novo fim (que é o da lista que foi somada)

                        NoAntigoNC->TrocaInicio(*NoTalvegue.PegaListaPontosNo().rbegin());
                      }
                      else
                      {
                        //--- O segundo ponto da lista de pontos no nó  é o fim da lista antiga, troca para o novo fim (que é o da lista que foi somada)

                        NoAntigoNC->TrocaFim(*NoTalvegue.PegaListaPontosNo().rbegin());
                      }
                    }
                  }

                  Contador++;
                  FimGeral = false;   //--- Como neste caso um dos nos foi invertido, é preciso passas de novo
                }
              }

              SetNosTalveguesIniFim.clear();
              LimpaTalveguesZerados();

              Fim = Contador == 0;
            }
          }
          break;
          case FIM_INICIO:
          {
            Fim = false;

            while (!Fim)     //--- IF (Inicio com fim)   Cada elemento do set é um Inicio com Fim no mesmo ponto
            {
              for (ItLLDeltaSup it = ListaRestricoes.begin(); it != ListaRestricoes.end(); it++)
              {
                if (it->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
                {
                  //--- Cria o Set de início com fim
                  //--- Neste caso (inicio com fim) Tem q verificar as 2 extremidades

                  auto Faixa(MSetPontosNo.equal_range(it->rbegin()->PCentral));

                  if (std::distance(Faixa.first, Faixa.second) == 2)
                  {
                    CNoTalvegue NoTalvFimIniAtualFim(it, CNoTalvegue::FIM);   //--- O primeiro nó é um início de lista

                    ittysetNosTalvegues ItTalvAtual(SetNosTalveguesFimIni.find(NoTalvFimIniAtualFim));

                    if (ItTalvAtual == SetNosTalveguesFimIni.end())
                    {
                      SetNosTalveguesFimIni.insert(NoTalvFimIniAtualFim);   //--- Neste caso o primeiro ponto tem q ser um FIM
                    }
                  }

                  //--- Verifica a outra extremidade

                  Faixa = MSetPontosNo.equal_range(it->begin()->PCentral);
                  if (std::distance(Faixa.first, Faixa.second) != 2) continue;

                  CNoTalvegue NoTalvFimIniAtualIni(it, CNoTalvegue::INICIO);

                  ittysetNosTalvegues ItTalvAtual(SetNosTalveguesFimIni.find(NoTalvFimIniAtualIni));

                  if (ItTalvAtual != SetNosTalveguesFimIni.end())
                  {
                    if (ItTalvAtual->PegaFlagsPontosNo() == CNoTalvegue::FIM)
                    {
                      const_cast<CNoTalvegue*>(&*ItTalvAtual)->IncluiPontoNo(it, CNoTalvegue::INICIO);  //--- Não corrompe o set 
                    }
                  }
                }
              }

              size_t Contador(0);

              for (ittysetNosTalvegues ItNoTalvegueAtual = SetNosTalveguesFimIni.begin(); ItNoTalvegueAtual != SetNosTalveguesFimIni.end(); ItNoTalvegueAtual++)
              {
                if (ItNoTalvegueAtual->PegaListaPontosNo().size() == 2)
                {
                  auto LPontosNo(&ItNoTalvegueAtual->PegaListaPontosNo());

                  auto& L1(*(*(ItNoTalvegueAtual->PegaListaPontosNo().begin())));
                  auto& L2(*(*(ItNoTalvegueAtual->PegaListaPontosNo().rbegin())));
                  int Caso(-1);

                  if (ItNoTalvegueAtual->PegaFlagsPontosNo() & CNoTalvegue::FIM)
                  {
                    L2.pop_front();
                    L1.splice(L1.end(), L2);
                    Caso = 1;
                  }
                  else
                  {
                    L1.pop_front();
                    L2.splice(L2.end(), L1);
                    Caso = 2;
                  }

                  //--- Neste caso, INICIO_FIM (e FIM_INICIO) pode ser que a reversão de uma das listas altere
                  //--- outros nós. Por isso é preciso verificar se a lista que foi somada esta em outro nó.
                  //--- Se estiver é preciso acertar o iterador para a lista neste nó, pois ele mudou, é outra lista 

                  if (Caso == 2)
                  {
                    CNoTalvegue NoTalvegue(*ItNoTalvegueAtual->PegaListaPontosNo().rbegin(), CNoTalvegue::FIM);

                    ittysetNosTalvegues NoAntigo(SetNosTalveguesFimIni.find(NoTalvegue));

                    if (NoAntigo != SetNosTalveguesFimIni.end() && NoAntigo->PegaListaPontosNo().size() == 2)
                    {
                      CNoTalvegue* NoAntigoNC(const_cast<CNoTalvegue*>(&*NoAntigo));

                      if (!(NoAntigoNC->PegaFlagsPontosNo() & (CNoTalvegue::FIM)))    //--- O fim é o primeiro ponto da lista de pontos no nó //--- ou é 01 ou 10 
                      {
                        //--- O primeiro ponto da lista de pontos no nó  é o fim da lista antiga, troca para o novo fim (que é o da lista que foi somada)

                        NoAntigoNC->TrocaFim(*NoTalvegue.PegaListaPontosNo().begin());
                      }
                      else
                      {
                        //--- O segundo ponto da lista de pontos no nó  é o fim da lista antiga, troca para o novo fim (que é o da lista que foi somada)

                        NoAntigoNC->TrocaInicio(*NoTalvegue.PegaListaPontosNo().rbegin());
                      }
                    }
                  }
                  else
                  {
                    CNoTalvegue NoTalvegue(*ItNoTalvegueAtual->PegaListaPontosNo().begin(), CNoTalvegue::INICIO);

                    ittysetNosTalvegues NoAntigo(SetNosTalveguesFimIni.find(NoTalvegue));

                    if (NoAntigo != SetNosTalveguesFimIni.end() && NoAntigo->PegaListaPontosNo().size() == 2)
                    {
                      CNoTalvegue* NoAntigoNC(const_cast<CNoTalvegue*>(&*NoAntigo));

                      if (NoAntigoNC->PegaFlagsPontosNo() & (CNoTalvegue::FIM))    //--- O fim é o primeiro ponto da lista de pontos no nó //--- ou é 01 ou 10 
                      {
                        //--- O primeiro ponto da lista de pontos no nó  é o fim da lista antiga, troca para o novo fim (que é o da lista que foi somada)

                        NoAntigoNC->TrocaFim(*NoTalvegue.PegaListaPontosNo().rbegin());
                      }
                      else
                      {
                        //--- O segundo ponto da lista de pontos no nó  é o fim da lista antiga, troca para o novo fim (que é o da lista que foi somada)

                        NoAntigoNC->TrocaInicio(*NoTalvegue.PegaListaPontosNo().rbegin());
                      }
                    }
                  }

                  Contador++;
                  FimGeral = false;  //--- Como neste caso um dos nos foi invertido, é preciso passas de novo
                }
              }

              SetNosTalveguesFimIni.clear();
              LimpaTalveguesZerados();

              Fim = Contador == 0;
            }
          }
          break;
          case FIM_FIM:
          {
            bool Fim(false);

            while (!Fim)    
            {
              for (ItLLDeltaSup it = ListaRestricoes.begin(); it != ListaRestricoes.end(); it++)
              {
                if (it->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
                {
                  auto Faixa(MSetPontosNo.equal_range(it->rbegin()->PCentral));

                  if (std::distance(Faixa.first, Faixa.second) != 2) continue;

                  //--- Cria o Set de início com início

                  CNoTalvegue NoTalvFimFimAtual(it, CNoTalvegue::FIM);

                  ittysetNosTalvegues ItTalvAtual(SetNosTalveguesFimFim.find(NoTalvFimFimAtual));

                  if (ItTalvAtual != SetNosTalveguesFimFim.end())
                  {
                    const_cast<CNoTalvegue*>(&*ItTalvAtual)->IncluiPontoNo(it, CNoTalvegue::FIM);  //--- Não corrompe o set 
                  }
                  else
                  {
                    SetNosTalveguesFimFim.insert(NoTalvFimFimAtual);
                  }
                }
              }

              size_t Contador(0);

              for (auto& ItNoTalvegueAtual : SetNosTalveguesFimFim)
              {
                if (ItNoTalvegueAtual.PegaListaPontosNo().size() == 2)
                {
                  auto LPontosNo(&ItNoTalvegueAtual.PegaListaPontosNo());

                  (*ItNoTalvegueAtual.PegaListaPontosNo().begin())->reverse();
                  (*(ItNoTalvegueAtual.PegaListaPontosNo().begin()))->pop_front();    //--- Ponto repetido nas 2  lists
                  (*(LPontosNo->rbegin()))->splice((*ItNoTalvegueAtual.PegaListaPontosNo().rbegin())->end(), *(*(ItNoTalvegueAtual.PegaListaPontosNo().begin())));

                  Contador++;
                  FimGeral = false;
                }
              }
              LimpaTalveguesZerados();         //--- Quando soma as 2 listas a somada fica com 1 elemento apenas e tem q ser excluída
              SetNosTalveguesFimFim.clear();

              Fim = Contador == 0;
            }
          }
          break;
        }
      }
    } while (!FimGeral);
  } while(LigaTalveguesAlinhados());
}      

int CSuperficie::LigaTalveguesAlinhados()
{
  //--- Liga os talvegues que estiverem alinhados no nó
  //--- 02-03-2024:
  //--- Nos testes aconteceu que um talvegue que foi ligado reto não poderia ter sido ligado porque era menor
  //--- ou porque o caminho do talvegue principal não era reto.
  //--- Então esta rotina não pode ser usada como esta pois impediria o traçado do talvegue principal.
  //--- SOLUÇÃO: (A FAZER)
  //--- Na definição do talvegue principal não procurar a ligação apenas no início e no fim do talvegue clicado
  //--- procurar no meio também, se achar ligar o talvegue principal a partir deste ponto

  return 0;

  tysetNosTalvegues SetNosTalveguesTodos;

  int Locais[2] = { CNoTalvegue::INICIO,CNoTalvegue::FIM };
  int FezUm(false);

  for (ItLLDeltaSup it = ListaRestricoes.begin(); it != ListaRestricoes.end(); it++)
  {
    if (it->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
    {
      for (int C = 0; C < 2; C++)
      {
        CNoTalvegue NoTalvTodosIniAtual(it, Locais[C]);

        ittysetNosTalvegues ItTalvAtual(SetNosTalveguesTodos.find(NoTalvTodosIniAtual));

        if (ItTalvAtual != SetNosTalveguesTodos.end())
        {
          const_cast<CNoTalvegue*>(&*ItTalvAtual)->IncluiPontoNo(it, Locais[C]);  //--- Não corrompe o set 
        }
        else
        {
          SetNosTalveguesTodos.insert(NoTalvTodosIniAtual);
        }
      }
    }
  }
  
  for (ittysetNosTalvegues ItTalvTodos = SetNosTalveguesTodos.begin(); ItTalvTodos != SetNosTalveguesTodos.end(); ItTalvTodos++)
  {
    auto& LTalvegues(ItTalvTodos->PegaListaPontosNo());

    //--- Calcula o azimute de partida de todos os pontos do nó
    //--- Como so a direção importa não precisa saber o sentido.

    if (LTalvegues.size() > 2)    //--- 1 ponto -> folha; 2 pontos -> ja foram fundidos, so pode haver alinhamento em nós com mais q 2 pontos.
    {
      for (auto ItPontosNo = LTalvegues.begin(); ItPontosNo != LTalvegues.end(); ItPontosNo++)
      {
        Ponto CoordPolarInicio(Mat.R_P((*ItPontosNo)->begin()->PCentral, (++(*ItPontosNo)->begin())->PCentral));

        (*ItPontosNo)->begin()->PCentral.fi = CoordPolarInicio.x;
        (*ItPontosNo)->begin()->PCentral.S = CoordPolarInicio.y;
        (*ItPontosNo)->begin()->PCentral.Baliza = CNoTalvegue::INICIO;

        Ponto CoordPolarFim(Mat.R_P((*ItPontosNo)->rbegin()->PCentral, (++(*ItPontosNo)->rbegin())->PCentral));

        (*ItPontosNo)->rbegin()->PCentral.fi = CoordPolarFim.x;
        (*ItPontosNo)->rbegin()->PCentral.S = CoordPolarFim.y;
        (*ItPontosNo)->rbegin()->PCentral.Baliza = CNoTalvegue::FIM;
      }
    }
  }

  for (ItLLDeltaSup it = ListaRestricoes.begin(); it != ListaRestricoes.end(); it++)
  {
    if(it->size() == 0) continue;  //--- Se limpar no ponto certo esse if não seria necessário

    if (it->begin()->TipoObjeto == CAciTopografico::TALVEGUE)
    {
      CNoTalvegue NoTalvegueAtual(it, CNoTalvegue::INICIO);    //--- Inicio ou fim, não importa

      ittysetNosTalvegues NoAtual(SetNosTalveguesTodos.find(NoTalvegueAtual));

      if (NoAtual != SetNosTalveguesTodos.end())
      {
        if (NoAtual != SetNosTalveguesTodos.end() && NoAtual->PegaListaPontosNo().size() < 3) continue;   //--- Nos com 2 pontos form obrigatoriamente ligados

        auto LPontosNoNo(NoAtual->PegaListaPontosNo());
        char N1(0), N2(0);        //--- Numero de ordem dos pontos no nó 

        //--- Verifica os azimutes, caso estejam alinhados funde as listas

        for (auto ItPontosNo = LPontosNoNo.begin(); ItPontosNo != LPontosNoNo.end(); ItPontosNo++, N1++)
        {
          if ((*ItPontosNo)->size() == 0) continue;

          auto ItRestantes(ItPontosNo), itPontoAtual(ItPontosNo);
          double DezGrausRad(Matem::UmGrau_Rad * 10.0);
          bool Achou(false);

          int Bit0(((NoAtual->PegaFlagsPontosNo() & (1 << N1)) > 0));   //--- Sinaliza a posição do ponto na lista: se ele está no início ou no fim da lista (Bit1 tb)

          Ponto& P1(Bit0 == CNoTalvegue::INICIO ? (*itPontoAtual)->begin()->PCentral : (*itPontoAtual)->rbegin()->PCentral);

          N2 = N1;

          for (ItRestantes++, N2++; !Achou && ItRestantes != LPontosNoNo.end(); ItRestantes++, N2++)
          {
            if ((*ItRestantes)->size() == 0) continue;

            int Bit1((NoAtual->PegaFlagsPontosNo() & (1 << N2)) > 0);  //--- Para pegar o início ou o fim da lista

            Ponto& P2(Bit1 == CNoTalvegue::INICIO ? (*ItRestantes)->begin()->PCentral : (*ItRestantes)->rbegin()->PCentral);

            double AngDirecao(fabs(P1.fi - P2.fi));

            if (AngDirecao < DezGrausRad || fabs(AngDirecao - Matem::M_PI) < DezGrausRad || fabs(AngDirecao - Matem::M_2_PI) < DezGrausRad)
            {
              int Flags((Bit1 << 1) | Bit0);   //-- Sinaliza a combinação dos 2 pontos

              FezUm = true;

              switch (Flags)
              {
                case INICIO_INICIO:
                {
                  (*ItRestantes)->pop_front();   //--- Ponto existente nas 2 listas

                  (*itPontoAtual)->reverse();

                  (*itPontoAtual)->splice((*itPontoAtual)->end(), *(*ItRestantes));
                }
                break;
                case INICIO_FIM:
                {
                  (*ItRestantes)->splice((*ItRestantes)->end(), *(*itPontoAtual));
                }
                break;
                case FIM_INICIO:
                {
                  (*ItRestantes)->splice((*ItRestantes)->end(), *(*itPontoAtual));
                }
                break;
                case FIM_FIM:
                {
                  ItLDeltaSup itLista((*ItRestantes)->begin());

                  (*ItRestantes)->reverse();

                  (*itPontoAtual)->splice((*itPontoAtual)->end(), *(*ItRestantes));
                }
                break;
              }
              Achou = true;
            }
          }
        }
      }
    }
  }

  return FezUm;
}

#include "CNoArvoreN.h"
#include "CArvoreN.h"

bool CSuperficie::DefineTalveguePrincipal(const Ponto& pFoz,const Ponto& pNascente)
{
  CArvoreN ArvoreTalvegues(pFoz);

  return false;
}
