#define _AFXDLL 
#include "stdafx.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
//#include <CGAL/Delaunay_mesher_2.h>
//#include <CGAL/Delaunay_mesh_face_base_2.h>
//#include <CGAL/Delaunay_mesh_size_criteria_2.h>
#include <CGAL/Triangulation_conformer_2.h>
//#include <CGAL/Constrained_triangulation_2.h>
#include <CGAL/nearest_neighbor_delaunay_2.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>  
#include <strstream>
#include <list>
#include "io.h"
#include "..\CPonto.h"
#include "..\CLPontos.h"
#include "..\CTextoMapaIBGE.h"
#include "..\CCNMapaIGBE.h"
#include "..\CMapaCNIBGE.h"

struct ltpairint
{
  bool operator()(const std::pair<int, int>& pint1, const std::pair<int, int>& pint2)  const
  {
    return pint1.first < pint2.first;
  }
};

struct ltpontres6d
{
  bool operator()(const Ponto& P1,const Ponto& P2)  const
  {
    double deltax(P1.x - P2.x),deltay(P1.y - P2.y);

    return deltax < -1e-6 ? true : deltax > 1e-6 ? false :
           deltay < -1e-6 ? true : false; 
  }
};

typedef std::set<Ponto, ltpontres6d> tysetpon6d;
tysetpon6d::iterator ittysetpon6d;

typedef std::set< std::pair<int, int>, ltpairint> tysetpairint;
typedef tysetpairint::iterator ittysetpairint;

tysetpairint SetAciTtopogFarcarAresta;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> TDS;
//typedef CGAL::Exact_intersections_tag Itag;
typedef CGAL::Exact_predicates_tag Itag;
//typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Constrained_Delaunay_triangulation_2<K,TDS,Itag> CDT;

//typedef CGAL::Constrained_Delaunay_triangulation_2<K> CDT;

typedef CDT::Point Point;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Vertex_circulator Vertex_circulator;
typedef CDT::Face_handle Face_handle;

double M_PI_2 = 1.57079632679489661923;
double M_PI = 3.14159265358979323846;
double M_2_PI = 6.2831853071795864769;
double M_PI_4 = M_PI_2 / 2.0;
double DINFINITO = 1.7976931348623158e+308;

double AngRotacaoBordos[3] = { DINFINITO,DINFINITO,DINFINITO };
std::stringstream strstrMsg;

#include "EstWinCGAL.h"

setPontos SetPontosSemCota;

/*
struct ltPoint_CGAL
{
  bool operator()(const Point & p1, const Point & p2) const
  {
    double dx(p1.x() - p2.x()), dy(p1.y() - p2.y());

    return dx < -1E-4 ? true : dx > 1e-4 ? false :
           dy < -1e-4 ? true : false;
  }
};
*/

bool ForcarDelaunay = false;
bool Gabriel = false;
bool Fragmentar = false;
bool Fragmentando = false;
double LadoFragmento = 30.00;

const int QTD_ACI_NAO_GERA_ARESTA(41);

#define TIPO_DRONES 1                 //--- Tringulacão para drones
#define TIPO_DGN_PONTOS_SOLTOS 2      //--- Triangulação simples Delaunay
#define TIPO_DGN_CN 3                 //--- Superficie DGN  
#define TIPO_TOP_ACAD 4               //--- Triangulação importada Autocad
#define TIPO_SIG 5                    //--- SIG - Sistema de Informações Geográficas (QGIS)

/*
enum TIPOSACITOP {
  NENHUM_ACITOPOG, FRONTEIRA, MURO, CERCA, MEIO_FIO, BORDO, N_A, PONTE, TAMPA_BUEIRO, POSTE, REDE_ELETRICA_BT,              //--- 10
  REDE_ELETRICA_AT, CASA, ROCHA, EIXO, CRISTA, PE, CANTO_MURO, CANTO_CERCA, CANTO_CASA, BUEIRO, N_A_PER, LINHA_FERREA,      //--- 22
  MATA, FLORESTA, PLANTACAO, ALAGADO, ARVORE, INICIO_TERRAP, ALINHAMENTO, OFF_SETS, TERRAPLANAGEM, FRONTEIRA_TERRAP,         //--- 32
  FIM_TERRAP, CRISTA_LIMPEZA, OFFSET_CORTE, OFFSET_ATERRO, ESPIGAO, TALVEGUE, RAVINA, INICIO_EQUI_DRENAGEM, EQUI_BUEIRO,    //--- 41 
  EQUI_VALETA_TERRAPL, EQUI_VALETA_TN, EQUI_BUEIRO_RETANG, EQUI_DESCIDA_DAGUA, EQUI_CAIXA_COLETORA, EQUI_BACIA_DISSIPACAO,  //--- 47 
  EQUI_SARJETA, EQUI_TUBULACAO, EQUI_DRENO_PROFUNDO, EQUI_CANALETA_TN, EQUI_CANALETA_TERRAPL, EQUI_EDITAR_DRENAGEM, FIM_EQUI_DRENAGEM, FIM_ACITOPOG
};
*/

//tysetint setTipoAciTopogNaoGeraAresta;
tymmlstplstPonto3D MMapTipoListaRestricao;

int CalcSuperficie(const std::string& strNomeProj, CProgressCtrl* Progresso, CStatic* pCSTexto, int pConfigSuper, const std::string& Formato, int TipoTriangulacao);
inline bool VerfPontoReta(const CPoint3D& P1, const CPoint3D& P2, const CPoint3D& P3);
inline double AzTrigToTop(double Az, double Deltax, double Deltay);
inline double CalculaAzimute(const CPoint3D& P1, const CPoint3D& P2);
inline void PegaProximoAlfaNum(std::ifstream& Stream);
inline double CalculaCotaPonto(const Point& Ponto, const CDT& cdtOriginal, tysetPonto3D& setP3D, tylstptylstPonto3D* ListaPonteirosListaRestricoes, bool Vertice = true);
inline double CalculaCotaPontoPlano(const ittystPonto3D itP1, const ittystPonto3D itP2, const ittystPonto3D itP3, CPoint3D& PSemCota);
inline void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora = false);
inline bool VerfAlfanum(const std::string& strFrase);
void InserePontosInseridosNasRestricoes(tylstptylstPonto3D* ListaPonteirosListaRestricoes, tyitsetPonto3D& SetitPontosInseridos, std::ofstream& rArqLog, const std::string& strNomeProj, const tymmlstplstPonto3D* pMMapTipoListaRestricao);
int PegaNomesArquivosTerreno(tylstNomeArquivos& lstNomeArquivos, const std::string strNomeProj,int TipoTriangulacao=-1);
int ENumero(std::string strDado);
void inline RotacionaCoordPlano(double& x, double& y, double Sin, double Cos);
unsigned int PegaTipo(const tylstitPonto3D* pitLst, const tymmlstplstPonto3D* pMMapTipoListaRestricao);
bool VerfNaTangenteRestricoes(const Point& PAtual, tylstptylstPonto3D* pListaPonteirosListaRestricoes, ittylpsetPontos3D& itlRestAchou, ittypstPonto3D& itPontoAchou);
//inline bool VerfComentario(const std::string& Buffer1,const std::string& Buffer2,const std::string& Buffer3,const std::string& Buffer4,const std::string& Buffer5);
inline void PulaBrancos(std::ifstream& Stream);
void GetLine(std::ifstream& ifs, std::string& Buffer, const std::string& strDelim);
void GravaPontosEditadosUsuario(const tysetPonto3D& RSetPontosInseridos, const tysetPonto3D& RSetPontosDesabilitados, const std::string& strNomeProj, const std::string& Formato);
inline void ArredondaXY(double& x, double& y, int Casa);
void LimpaMemoria(tylstptylstPonto3D* pListaPonteirosListaRestricoes);
void AcertaCamposLer(const std::string& Formato, std::string& Nome, std::string& x, std::string& y, std::string& z, std::string& Desc);
void AcertaCamposGravar(const std::string& Formato, int NumPonto, ittystPonto3D it, const std::string& Desc, std::stringstream& LinhaArquivo);
int LeArqCN(const std::string& NomeProj, CDT& cdtrian, tysetPonto3D& setPon3D, int TipoTriang);
bool RemovePontosRemovidos(CDT& cdtrian, tylstcponto& LPonRemovidos);
bool LePontosMapaIBGE(const std::string& NomeProj, CDT& cdtrian,tysetPonto3D& setPontos3D);
void RemoveArqProjDGN(const std::string& strNomeProj);
void  LePontosSoltosIBGE(const std::string& NomeProj, CDT& cdtrian, tysetPonto3D& setPontos3D);
int LeSetForcaAresta(const std::string& NomeProjeto, int TipoArq, std::ofstream* ArqLog=nullptr);
void CotaPontosSemCota(const std::string& pstrNomeProj, std::ofstream& ArqLog, const CDT& pcdt, tysetPonto3D& pSetPontos3D);
int RemovePontosDesabilitados(CDT& cdt, tysetPonto3D& SetPonDesabilitados, std::ofstream& ArqLog);
void InserePontosSemCotaNaSuperf(CDT& pcdt, tysetPonto3D& psetPontos3D);

//bool FragmentarSuperficie(CDT& cdt);

inline double AzTrigToTop(double Az, double Deltax, double Deltay)   //--- Passa o azimute de trigonométrico para topográfico (Est::mat.cpp)
{
  Az = M_PI_2 - abs(Az);

  if (Deltax > 0.0)
  {
    if (Deltay > 0.0) return Az;
    else return M_PI - Az;
  }
  else
  {
    if (Deltay > 0.0) return M_2_PI - Az;
    else return  M_PI + Az;
  }
}

inline double CalculaAzimute(const CPoint3D& P1, const CPoint3D& P2)
{
  double Dy(P2.y - P1.y), Dx(P2.x - P1.x), Azxy(atan(Dy / Dx));

  return AzTrigToTop(Azxy, Dx, Dy);
}

int CalcSuperficie(const std::string& strNomeProj, CProgressCtrl* Progresso, CStatic* pCSTexto, int pConfigSuper, const std::string& Formato, int TipoTriangulacao)
{
  CDT cdt, cdtOriginal;
  tysetPonto3D setPontos3D;             //--- Set de backup, para salvar a cota pois a triangulação é 2D (Verificar necessidade)
  std::string strMsg;
  tylstptylstPonto3D ListaPonteirosListaRestricoes[2];       //--- Lista de ponteiros para as listas das restriçoes  (Bordos e restrições)
  int ContaPontos(0), Passo(200);

  strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  Gabriel = (pConfigSuper & (2)) > 0;
  ForcarDelaunay = (pConfigSuper & 1) > 0;

  std::ofstream ArqLog(strNomeProj + "_CalcSuperficie.log", std::ios_base::app);

  if (ArqLog.good() == false)
  {
    if (pCSTexto) pCSTexto->SetWindowText("Erro ao abrir o arquivo de log. Programa abortado.");
    return 3;
  }

  GravaLog("====> Início do cálculo da superfície: ", ArqLog, true);

  if (TipoTriangulacao != TIPO_DGN_PONTOS_SOLTOS)
  {
    int ResForcarAresta(LeSetForcaAresta(strNomeProj, -1, &ArqLog));

    //--- O arquivo de arestas forçadas é obrigatório exceto no TIPO_DGN_PONTOS_SOLTOS e no TIPO_DGN_CN

    if (TipoTriangulacao != TIPO_DGN_CN && TipoTriangulacao != TIPO_SIG && ResForcarAresta != 0) return ResForcarAresta;

    for (int C = 0; C < 2; C++)     //--- Arquivo de restrições e de bordos
    {
      //    if (Tipo == 0 && C == 1) continue;  //--- Tipo (0) superf tradicional não le o arq de bordos, tipo drones (1) le o arq de bordos 

     //if(TipoTriangulacao == 3) continue;

      if (C == 0) GravaLog("Lendo o arquivo de acidentes topográficos...", ArqLog);
      else GravaLog("Lendo o arquivo de bordos...", ArqLog);

      CFileException e;
      CFile Arq;

      std::string strNomeArq(strNomeProj + (C == 0 ? ".res" : ".bor"));
      unsigned int Flags(CFile::modeRead);

      if (Arq.Open(strNomeArq.c_str(), Flags, &e) == 0)
      {
        if (e.m_cause != CFileException::fileNotFound)
        {
          std::string strTexto("Erro ao abrir o arquivo.\n");
          strTexto += strNomeArq.c_str();
          strTexto += ".\n A superficie não foi gerada ou foi gerada com erros.";
          GravaLog(strTexto, ArqLog);

          if (pCSTexto) pCSTexto->SetWindowText(strTexto.c_str());

          return 4;
        }
      }
      else
      {
        CArchive CArcRestricoes(&Arq, CArchive::load);
        size_t QRestricoes(0);
        register size_t QPontos(0);
        double x1(DINFINITO), y1(DINFINITO),z1(INFINITO), x2(DINFINITO), y2(DINFINITO), z2(DINFINITO), 
                i(DINFINITO), fi(DINFINITO), S(DINFINITO), Sin(0.0), Cos(0.0);

        if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
        {
          CArcRestricoes >> AngRotacaoBordos[C];

          if (AngRotacaoBordos[C] != 0.0)
          {
            Sin = sin(-AngRotacaoBordos[C]);
            Cos = cos(-AngRotacaoBordos[C]);
          }

          CString CSDescricao;
          unsigned int Tipo(0);

          ArqLog << std::right << std::fixed << std::setfill(' ') << std::setprecision(6);

          for (CArcRestricoes >> QRestricoes; QRestricoes; QRestricoes--)    //--- Para todas as restrições
          {
            bool Baliza, ERestricao;
            CString CSNome, CSDescricao;
            int AcidenteTopografico, Baliza2;

            CArcRestricoes >> Tipo;

            CArcRestricoes >> QPontos; //>> CSNome >> CSDescricao >> x1 >> y1 >> z1 >> i >> fi >> S >> Baliza >> Baliza2 >> ERestricao >> AcidenteTopografico;

            bool GeraArestaForcada(false);

            auto ItForcarAresta(SetAciTtopogFarcarAresta.find(std::pair<int, int>(Tipo - 1, -1)));

            GeraArestaForcada = C == 1 || (ItForcarAresta != SetAciTtopogFarcarAresta.end() && (bool)ItForcarAresta->second == true);

            if (AngRotacaoBordos[C] != 0.0)
            {
              RotacionaCoordPlano(x1, y1, Sin, Cos);
              ArredondaXY(x1, y1, 3);
            }
            //--- Com a inclusão da importação da topografia (Autocad - arquivos DXF) Alguns pontos podem vir sem cota
            //--- Estes pontos são guardados e a cota será calculada depois da triangulação
            //--- Definição em andamento 15-070-2022

            ittystPonto3D itSetP3D(setPontos3D.end());
            tylstitPonto3D* pListaRestricaoAtual(new (tylstitPonto3D));

            bool Achou(false);

            //--- No aut

            while (QPontos > 0 && !Achou)  //--- Procura pelo primeiro ponto com cota nesta restrição
            {
              CArcRestricoes >> CSNome >> CSDescricao >> x1 >> y1 >> z1 >> i >> fi >> S >> Baliza >> Baliza2 >> ERestricao >> AcidenteTopografico;
              
              if(z1 == 0.0 || z1 == INFINITO)
              {
                SetPontosSemCota.emplace(x1, y1, z1, i, fi, S, CSDescricao, Baliza, false, CSNome, AcidenteTopografico, Baliza2);

                QPontos--;
              }
              else Achou = true;
            }

            if(QPontos)  //--- Sobrou algum ponto cotado nesta restrição?
            {
              itSetP3D = setPontos3D.insert(CPoint3D(x1, y1, z1, i, fi, S, Tipo, CSDescricao)).first;
              pListaRestricaoAtual->push_back(itSetP3D);
              ListaPonteirosListaRestricoes[C].push_back(pListaRestricaoAtual);

              //---   VERIFICAR AQUI SE O PONTO ESTA DESABILITADO
              //--- Surgiu a necessidade desabilitar um ponto que é uma árvore
              //--- Eu precisaria saber aqui se o ponto está desabilitado ou não
              //--- A principio os pontos que pertencem a acidentes topográficos não podem ser desabilitados
              //--- Mas como como a arvore é um acidente topográfico de apenas um ponto ela pode.
              //--- Então seria  preciso saber aqui se o ponto está desabilitado ou não
              //--- mas a propriedade de desabilitado não faz parte das balizas do ponto (Infelizmente)
              //--- Os pontos desabilitados  são gravados no arquivo de pontos desabilitados
              //--- Este arquivo de ponto desabilitado ainda não foi lido pois os arquivo de acidentes topográficos são lidos primeiro
              //--- então neste ponto eu não tenho como verificar seo ponto esta desabilitado ou não
              //--- Para resolver o pb eu insiro o ponto e quando eu ler o arquivo de pontos desabilitados 
              //--- eu o removo da superficie caso ja exista nela.
              //--- Na proxima versão ler o arquivo de pontos desabilitados primeiro para poder verificar se  o ponto do
              //--- acidente topográfico esta desabilitado ou não.
              //--- ( so pontos de acidentes topograficos de um apenas um ponto podem ser desabilitados (árvores, cantos de muro etc)

              if (!GeraArestaForcada)
              {
                cdt.insert(Point(x1, y1));
              }

              for (--QPontos; QPontos; --QPontos)       //--- Para todos os pontos da restrição atual
              {
                CArcRestricoes >> CSNome >> CSDescricao >> x2 >> y2 >> z2 >> i >> fi >> S >> Baliza >> Baliza2 >> ERestricao >> AcidenteTopografico;

                if (CSDescricao.Find("Ponto Inserido") == -1 || CSDescricao.Find("Usuário") != -1)
                {
                  if (AngRotacaoBordos[C] != 0.0)
                  {
                    RotacionaCoordPlano(x2, y2, Sin, Cos);
                    ArredondaXY(x2, y2, 3);
                  }
                  //  if(CSDescricao.Find("Ponto Inserido") == -1)
                  //{
                  CPoint3D P3DAtual(x2, y2, z2, i, fi, S, Tipo, CSDescricao, CSNome);

                  if (!(z2 == 0.0 || z2 == INFINITO))
                  {
                    pListaRestricaoAtual->push_back((setPontos3D.insert(P3DAtual).first));
                  }

                  //--- Não insere as restrições que não geram arestas forçadas

                  auto ItForcarAresta(SetAciTtopogFarcarAresta.find(std::pair<int, int>(Tipo - 1, -1)));

                  GeraArestaForcada = C == 1 || (ItForcarAresta != SetAciTtopogFarcarAresta.end() && (bool)ItForcarAresta->second == true);

                  if (GeraArestaForcada == true)
                  {
                    if (fabs(x1 - x2) > 1e-4 || fabs(y1 - y2) > 1e-4)                            //--- Pontos iguais??? Aconteceu nos testes
                    {
                      if (z2 == 0.0 || z2 == INFINITO)
                      {
                        SetPontosSemCota.emplace(x2, y2, z2, i, fi, S, CSDescricao, Baliza, false, CSNome, AcidenteTopografico, Baliza2);
                      }
                      else
                      {
                        try
                        {
                          if (!(z1 == 0.0 || z1 == INFINITO))
                          {
                            cdt.insert_constraint(Point(x1, y1), Point(x2, y2));                        //--- insere o segmento na superficie como aresta forçada 
                          }
                        }
                        catch (std::exception& e)
                        {
                          std::stringstream strstrTexto;

                          strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(5)
                            << "Este ponto não pôde ser inserido x= " << x1 << " y= " << y1
                            << "Motivo: " << e.what() << std::endl;

                          GravaLog(strstrMsg.str(), ArqLog);
                        }
                      }

                      x1 = x2;
                      y1 = y2;
                      z1 = z2;

                      if (Progresso != NULL)
                      {
                        if (ContaPontos++ == Passo)
                        {
                          Progresso->StepIt();
                          ContaPontos = 0;
                        }
                      }
                    }
                  }
                  else         //---- Se o ponto não gera aresta forçada inclui-o como ponto simples.
                  {
                    Point PAtual(x2, y2);
                    CDT::Locate_type LT(CDT::VERTEX);
                    int LI(CDT::VERTEX);

                    if (cdt.locate(PAtual) == NULL)
                    {
                      if (z2 == 0.0 || z2 == INFINITO)
                      {
                        SetPontosSemCota.emplace(x2, y2, z2, i, fi, S, CSDescricao, Baliza, false, CSNome, AcidenteTopografico, Baliza2);
                      }
                      else
                      {
                        cdt.insert(PAtual);
                      }

                      if (Progresso != NULL)
                      {
                        if (ContaPontos++ == Passo)
                        {
                          Progresso->StepIt();
                          ContaPontos = 0;
                        }
                      }
                    }
                  }
                }
              }
            }

            if (C == 0)
            {
              //---- Quando não é pra gerar aresta forçada fica o primeiro ponto sobrando na lista, limpa e remove a lista

              if (1 || GeraArestaForcada && pListaRestricaoAtual->size() > 1) MMapTipoListaRestricao.insert(pairmmlstplstPonto3D(Tipo, pListaRestricaoAtual));
              else
              {
                pListaRestricaoAtual->clear();
                ListaPonteirosListaRestricoes[C].pop_back();
              }
            }
          }
        }
      }
    }
  }

  GravaLog("Lendo arquivos de terreno...", ArqLog);

  tyitsetPonto3D SetItPontosInseridos;
  tysetPonto3D SetPontosInseridosUsuario, SetPontosDesabilitados;
  tylstNomeArquivos lstNomeArquivos;
  tylstcponto LPontosRemovidos;
  tyitlstNomeArquivos itArqTriang(lstNomeArquivos.end());  //--- O arquivo de triangulação tem q ser o último a ser lido senão perde a descrição do ponto

  if (TipoTriangulacao == TIPO_SIG || TipoTriangulacao == TIPO_DRONES || TipoTriangulacao == TIPO_DGN_PONTOS_SOLTOS || PegaNomesArquivosTerreno(lstNomeArquivos, strNomeProj,TipoTriangulacao))
  {
    if (TipoTriangulacao != TIPO_DGN_PONTOS_SOLTOS)  //--- Arquivo pontos cotados DGN
    {
      for (tyitlstNomeArquivos itNomeArqAtual = lstNomeArquivos.begin(); itNomeArqAtual != lstNomeArquivos.end(); ++itNomeArqAtual)
      {
        //--- Se o projeto for Autocad não lê o arquivo de triangulação porque 
        //--- está removendo as observações do ponto.
        //--- Todos os pontos da triangulação estão no arquivo de acidentes topográficos (ou deveriam estar)
        //--- a descrição do ponto está no arquivo de acidentes topográficos não no da triangulação

        if (itNomeArqAtual->find("PontosInseridos") == -1 || (itNomeArqAtual->find("Secoes") != -1))    //--- pula o arquivo de pontos inseridos pelo sistema, será calculado novamente(Exceto os pontos incluidos nas secoes)
        {
          std::ifstream ArqPontos(*itNomeArqAtual);

          strstrMsg.str("");
          strstrMsg << "Lendo arquivo de pontos=> " << *itNomeArqAtual << std::ends;
          GravaLog(strstrMsg.str(), ArqLog);

          if (ArqPontos.is_open())     //--- Insere os pontos da superfície
          {
            int ContaLinhas(0);
            static std::string strMarcasComentarios("/!@#$%&*");
            bool ArqPontosEditados(false), PontosDesabilitados(false), PontosInseridos(false), PontosRemovidos(false);

            if (itNomeArqAtual->find("_PontosEditadosUsuario") != -1) ArqPontosEditados = true;

            //  if (Tipo == 1 && !ArqPontosEditados) continue;  //--- Tipo 1 == drones, dos arquivos xyz, so le o arquivo pontos editados

            //--- 27/10/2020
            //--- Nos drones (tipo = 1) não le nenhum arquivo xyz pois os pontos estão nas CNs
            //--- O arquivo de pontos editados contém tb os pontos dos equipamentos de drenagem que não são considerados pois a cota foi calculada na superficie
            //--- Isso vai causar um pb pois se a superficie alterou essas cotas devem ser corrigidas. <<<<<<<<<======= 
            //--- A FAZER --->>> Recalcular estas cotas depois da superficie calculada 

            if (TipoTriangulacao == TIPO_DRONES)/* || ArqPontosEditados)*/ continue;

            while (ArqPontos.good())
            {
              std::string strBuffer, strBufferNome, strBufferx, strBuffery, strBufferz, strBufferObs;

              ++ContaLinhas;
              PulaBrancos(ArqPontos);                        //--- Pula os  brancos no início do arquivo
              GetLine(ArqPontos, strBufferNome, " \t\x0A");       //--- Nome do ponto, descarta

              if (strBufferNome.size() == 0)
              {
                std::getline(ArqPontos, strBufferNome);
                continue;
              }

              if (strMarcasComentarios.find(*strBufferNome.begin()) != std::string::npos)
              {
                std::getline(ArqPontos, strBuffer);
                if (strBuffer.find("Pontos") != std::string::npos || strBuffer.find("Edição") != std::string::npos)
                {
                  //--- Chegou nos pontos inseridos. Os pontos inseridos passam como pontos da superficie pois não sofrem alteração, se a cota estiver com 0.000
                  //--- ela é calculada, pelo mesmo processo dos pontos inseridos por Delaunay ou Gabriel.
                  //--- Os pontos desabilitados ou excluidos simplesmente não são incluídos na superfície.
                  //--- 18-02-2022
                  //--- Nas seções os pontos podem ser incluídos e excluídos aleatoriamente. Os pontos que foram incluidos e excluídos
                  //--- não devem fazer parte da superfície. surgem 2 problemas:
                  //--- Eles são incluidos em um momento e exluídos em outro momento;
                  //--- Os arquivos ende estão as inclusões e a exclusões são lidos aleatoriamente;
                  //--- Para se resolver estes problemas :
                  //--- Lê-se todos os pontos incluídos e os inclui na superfície ( mesmo q ja tenham sido excluídos)
                  //--- Lê-se os pontos excluídos guardando-os em uma lista sem inseri-los na superfície
                  //--- Depois q todos os arquivos foram lidos remove-se da superfície os pontos q estão na lista de pontos excluídos

                  if (strBuffer.find("Inseridos") != std::string::npos)
                  {
                    PontosDesabilitados = PontosRemovidos = false;
                    PontosInseridos = true;
                  }
                  else
                  {
                    if (strBuffer.find("Desabilitados") != std::string::npos)
                    {
                      PontosDesabilitados = true;
                      PontosInseridos = PontosRemovidos = false;
                    }
                    else
                    {
                      if (strBuffer.find("Removidos") != std::string::npos)
                      {
                        PontosDesabilitados = PontosInseridos = false;
                        PontosRemovidos = true;
                      }
                      else
                      {
                        if (strBuffer.find("Edição") != std::string::npos)     //--- Nova edição
                        {
                          PontosDesabilitados = PontosInseridos = PontosRemovidos = false;
                        }
                      }
                    }
                  }
                }
                continue;
              }
              /*
              PegaProximoAlfaNum(ArqPontos);
              GetLine(ArqPontos, strBufferx,"  \t\x0A");   //--- x
              PegaProximoAlfaNum(ArqPontos);
              GetLine(ArqPontos, strBuffery,"  \t\x0A");   //--- y
              PegaProximoAlfaNum(ArqPontos);
              GetLine(ArqPontos, strBufferz,"  \t\x0A");   //--- z
              PegaProximoAlfaNum(ArqPontos);
              std::getline(ArqPontos, strBufferObs);      //--- Obs
              */

              std::getline(ArqPontos, strBuffer);

              std::stringstream strstrBuffer(strBuffer);

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
                      std::getline(strstrBuffer, strBufferObs);
                    }
                  }
                }
              }

              if (Formato != "n N E C O") AcertaCamposLer(Formato, strBufferNome, strBuffery, strBufferx, strBufferz, strBufferObs);

              if (!strBuffery.empty() || !strBufferx.empty() || !strBufferz.empty())
              {
                if (!ENumero(strBufferx) || !ENumero(strBuffery) || !ENumero(strBufferz))
                {
                  strstrMsg.str("");
                  strstrMsg.seekg(0, strstrMsg.beg);
                  strstrMsg << "Erro de leitura no arquivo - " << strBufferNome << " Linha = " << ContaLinhas << std::endl << " Abortando..." << std::endl << "Veja o arquivo de log" << std::ends;

                  AfxMessageBox(strstrMsg.str().c_str());

                  strstrMsg << "Dados Lidos x =" << strBufferx << " y=" << strBuffery << " z=" << strBufferz << " Obs=" << strBufferObs;
                  GravaLog(strstrMsg.str(), ArqLog);

                  return 5;
                }
                else
                {
                  double x(atof(strBufferx.c_str())), y(atof(strBuffery.c_str())), z(atof(strBufferz.c_str()));

                  if (!(TipoTriangulacao == TIPO_TOP_ACAD && (z == 0.0 || z == INFINITO)))
                  {
                    if (!PontosDesabilitados)                                                             //--- //--- ponto desabilitados, desconnsidera
                    {
                      CPoint3D P3DAtual(x, y, z, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_SUPERF, strBufferObs.c_str(), strBufferNome.c_str());

                      if (SetPontosDesabilitados.find(CPoint3D(x, y, z)) == SetPontosDesabilitados.end())   //--- ponto desabilitados, desconnsidera
                      {
                        ittystPonto3D itSP3D(setPontos3D.find(P3DAtual));

                        if (itSP3D != setPontos3D.end())                                                   //--- Se o ponto foi encontrado é porque ele foi incluído como restrição
                        {
                          CPoint3D* pCPoint3D(const_cast<CPoint3D*>(&*itSP3D));   //--- Se o ponto foi inserido como restrição está sem nome e sem descrição, acerta agora
                          pCPoint3D->CSNome = strBufferNome.c_str();
                          pCPoint3D->CSDescricao = strBufferObs.c_str();

                          CDT::Locate_type LT(CDT::VERTEX);
                          int LI(CDT::VERTEX);
                          Point PAtual(x, y);

                          if (PontosInseridos)
                          {
                            SetPontosInseridosUsuario.insert(CPoint3D(x, y, z, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_TEMP, strBufferObs.c_str(), strBufferNome.c_str()));   //--- Os pontos inseridos tem que ser guardados, pode ser preciso calcular a cota 
                          }
                          else
                            //             if(cdt.locate(PAtual,LT,LI) == NULL)        //---- Se o ponto não foi inserido como restrição, insere-o, este if esta redundante....só por segurança
                          {
                            if (PontosRemovidos)
                            {
                              LPontosRemovidos.emplace_back(x, y, z);
                            }
                            else
                            {
                              try
                              { 
                                cdt.insert(Point(x, y));                     //--- Alguns pontos de restrição foram inseridos no set e não foram inseridos na superficie. Se o ponto ja foi inserido ele é desprezado.            
                              }
                              catch(std::exception& e)
                              {
			                          std::stringstream strstrTexto;
								
                                strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(5)
                                          << "Este ponto não pôde ser inserido x= " << x << " y= " << y
                                          << "Motivo: " << e.what() << std::endl;

                                GravaLog(strstrMsg.str(), ArqLog);
                              }

                              if (Progresso != NULL)
                              {
                                if (ContaPontos++ == Passo)
                                {
                                  Progresso->StepIt();
                                  ContaPontos = 0;
                                }
                              }
                            }
                          }
                        }
                        else
                        {
                          // Os pontos inseridos pelo usuário passam como pontos inseridos por Delaunay ou Gabriel, porisso não os insere no set de pontos da superficie
                          // Os insere no set de pontos inseridos pelo usuário.

                          if (PontosInseridos)
                          {
                            SetPontosInseridosUsuario.insert(CPoint3D(x, y, z, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_TEMP, strBufferObs.c_str(), strBufferNome.c_str()));   //--- Os pontos inseridos tem que ser guardados, pode ser preciso calcular a cota 
                          }
                          else                                                                                                                                       //--- e passam como pontos inseridos por Delaunay ou Gabriel, porisso não os insere no set 
                          {                                                                                                                                          //--- Outros pontos: insere-os no set de backup.
                            ittystPonto3D it(setPontos3D.insert(P3DAtual).first);                                                                                    //--- Outros pontos: insere-os no set de backup.
                            try
                            {
                              cdt.insert(Point(x, y));
                            }
                            catch (std::exception& e)
                            {
						                  std::stringstream strstrTexto;

                              strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(5)
                                        << "Este ponto não pôde ser inserido x= " << x << " y= " << y
                                        << "Motivo: " << e.what() << std::endl;

                              GravaLog(strstrMsg.str(), ArqLog);
                            }

                            if (Progresso != NULL)
                            {
                              if (ContaPontos++ == Passo)
                              {
                                Progresso->StepIt();
                                ContaPontos = 0;
                              }
                            }
                            //--- Insere-os na superficie. Obs.: As restriçoes que não forçam arestas não foram 
                          }
                        }

                        if (Progresso && (ContaPontos++ % 500) == 0)
                        {
                          Progresso->StepIt();
                        }
                      }
                    }
                    else 
                    {
                      SetPontosDesabilitados.insert(CPoint3D(x, y, z, DINFINITO, DINFINITO, DINFINITO, 0, CString(strBufferObs.c_str()), CString(strBufferNome.c_str())));
                    }
                  }
                  else
                  {
                    SetPontosSemCota.emplace(x, y, z,INFINITO, INFINITO, INFINITO, CString(strBufferObs.c_str()), 0, false, "", -1, 0);
                  }
                }
              }

              if (ArqPontosEditados && strBufferNome.find("Pontos Desabilitados") != -1) 
              {
                PontosDesabilitados = true;    //--- Chegou nos pontos desabilitados
              }
            }    //--- Inseriu todos os pontos da superficie ou seja, criou a superfície de Delaunay ou de Gabriel na memória.
            if (!ArqPontos.eof())
            {
              strstrMsg.str("");
              strstrMsg << "Erro na leitura do arquivo - Linha => " << ContaLinhas << std::endl << " Abortando..." << std::ends;
              GravaLog(strstrMsg.str(), ArqLog);

              return 6;
            }
          }
        }
      }

      if (Progresso) Progresso->SetPos(100);                               //--- A superfície esta calculada.

      //--- insere os pontos inseridos pelo usuário.

      GravaLog("Lendo arquivo de pontos inseridos pelo usuário...", ArqLog);

      for (ittystPonto3D it = SetPontosInseridosUsuario.begin(); it != SetPontosInseridosUsuario.end(); ++it)
      {
        //--- Calcula a cota do ponto e insere-o na superfície e no set de back up. Agora ele faz parte da superfície
        Point PSemCota(it->x, it->y);
        double Cota(it->z);

        Cota = CalculaCotaPonto(PSemCota, cdt, setPontos3D, NULL, true);

        if (Cota != DINFINITO)
        {
          cdt.insert(Point(it->x, it->y));
          setPontos3D.insert(CPoint3D(it->x, it->y, Cota, DINFINITO, DINFINITO, DINFINITO, 5, it->CSDescricao, it->CSNome));
          const_cast<CPoint3D*>(&*it)->z = Cota;

          if (Progresso != NULL)
          {
            if (ContaPontos++ == Passo)
            {
              Progresso->StepIt();
              ContaPontos = 0;
            }
          }
        }
        else
        {
          std::string strMsg("Erro irrecuperavel - Contacte o suporte - VU&E004-SU");   //--- Não foi possível calular a cora do ponto
          AfxMessageBox(strMsg.c_str());
          GravaLog(strMsg, ArqLog);

          return 7;
        }
      }
    }

    switch (TipoTriangulacao)
    {
      case TIPO_DGN_PONTOS_SOLTOS: LePontosMapaIBGE(strNomeProj, cdt, setPontos3D); break;
      case TIPO_DGN_CN:  LePontosSoltosIBGE(strNomeProj, cdt, setPontos3D);  //--- Tá certo, não tem o break tem q executar o LearqCN tb
      case TIPO_SIG:
      case TIPO_DRONES: LeArqCN(strNomeProj, cdt, setPontos3D, TipoTriangulacao); break;
    }

    RemovePontosRemovidos(cdt, LPontosRemovidos);             //--- Remove os pontos removidos nas seções
    RemovePontosDesabilitados(cdt,SetPontosDesabilitados,ArqLog);

    if (ForcarDelaunay || Gabriel) cdtOriginal = cdt;         //--- Neste ponto a superficie esta com a triangulação original, sem inserções de Delaunay ou Gabriel, 
    //--- é preciso guardá-la para calcular as cotas dos pontos inseridos.
    //--- A superfície está criada na memória, grava o arquivo da superfície.
    //--- Caso haja pontos inseridos nos vertices das restriçoes é preciso
    //--- inclui-los nas devidas listas de restrições

    //  if(Fragmentar) FragmentarSuperficie(cdt);
    if (Progresso) Progresso->SetPos(100);

    std::list<Point> list_of_seeds;
    // list_of_seeds.push_back(Point(626737.0, 9280107.0));
    // CGAL::refine_Delaunay_mesh_2(cdt,list_of_seeds.begin(),list_of_seeds.end(),Criteria());
    // CGAL::refine_Delaunay_mesh_2(cdt,list_of_seeds.begin(),list_of_seeds.end(),Criteria());

   // if (cdt.is_valid(true))   //--- ??? As vezes a CDT não é válida mas serve... verificar 070720123
    if(1)
    {
      CotaPontosSemCota(strNomeProj,ArqLog, cdt, setPontos3D);  //--- Acontece principalmente no AutoCad

      if (ForcarDelaunay) CGAL::make_conforming_Delaunay_2(cdt);
      else if (Gabriel) CGAL::make_conforming_Gabriel_2(cdt);

      std::ofstream ArqPontosInseridos(strNomeProj + "_PontosInseridos.xyz", std::ofstream::out | std::ofstream::trunc);
      if (!ArqPontosInseridos.is_open())  return false;
      ArqPontosInseridos << std::right << std::fixed << std::setfill(' ') << std::setprecision(5);
      ArqPontosInseridos << "//--- Este arquivo contém os pontos inseridos na superficie " << std::endl;
      ArqPontosInseridos << "//==> NÃO EDITAR. A edição deste arquivo produzirá erros no cálculo da superfície." << std::endl;

      CFileException e;
      std::string strNome((strNomeProj + std::string(".sup")).c_str());

      //Le o Angulo de rotação

      double AngRotacao(0.0), Declinacao(0.0);

      if (false)     //--- A FAZER: Corrigir o ang rot inicial
      {
        CFile ArqSuperficieAng;

        if (ArqSuperficieAng.Open(strNome.c_str(), CFile::modeRead | CFile::shareDenyNone, &e) == 0)
        {
          if (e.m_cause != CFileException::fileNotFound)
          {
            std::string strMsg(std::string("Erro ao abrir o arquivo de saida:\n") + strNome.c_str());
            GravaLog(strMsg, ArqLog);

            return 8;
          }
        }

        CArchive CArcSuperficieAng(&ArqSuperficieAng, CArchive::load);
        ///    CArcSuperficieAng >> AngRotacao;   //--- Corrigir : Se a superfície estiver rotacionada, mostra o desenho mostra no local errado no desenho da superficie
        CArcSuperficieAng.Close();
        ArqSuperficieAng.Close();
      }

      GravaLog("Gravando o arquivo de saída...", ArqLog);

      CFile ArqSuperficie;

      if (ArqSuperficie.Open(strNome.c_str(), CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite, &e) == 0)
      {
        if (e.m_cause != CFileException::fileNotFound)
        {
          std::string strMsg(std::string("Erro ao abrir o arquivo de saida:\n") + strNome.c_str());
          GravaLog(strMsg, ArqLog);

          return false;
        }
      }

      CArchive CArcSuperficie(&ArqSuperficie, CArchive::store);

      //--- Salva o Angulo de rotacao inicial  e a quantidade de vertices na superficie

      size_t qtdVertices(cdt.number_of_vertices());
      unsigned int ContaPontosInseridos(0);
      Point PVertice;
      unsigned int ContaPonIns(0);
      std::string Obs;
      std::stringstream LinhaArquivo;

      LinhaArquivo << std::right << std::fixed << std::setfill(' ') << std::setprecision(5);

      CArcSuperficie << AngRotacao << Declinacao << (size_t)cdt.number_of_vertices();

      for (CDT::All_vertices_iterator eit = ++cdt.all_vertices_begin(); eit != cdt.all_vertices_end(); ++eit)   //--- Insere todos os vertices
      {
        CPoint3D PVerticeAtual(eit->point().x(), eit->point().y());
        ittystPonto3D it(setPontos3D.find(PVerticeAtual));
        bool PontoInseridoUsuario(false);

        if (it == setPontos3D.end())    //--- Se true o ponto central é um ponto inserido na triangulação (forçar delaunay ou Gabriel ou delaunay quando cria restriçoes) o e esta sem cota
        {                               //--- é preciso calcular a cota antes de inserí-lo na superfície  
          //--- Calcula a cota no plano
          CPoint3D Ponto(eit->point().x(), eit->point().y(), DINFINITO, (ForcarDelaunay ? CPoint3D::PONTO_INSERIDO_DELAUNAY : CPoint3D::PONTO_INSERIDO_GABRIEL));

          std::stringstream strstrDescPonto;

          ++ContaPontosInseridos;
          strstrDescPonto << ContaPontosInseridos << " Ponto Inserido - " << (ForcarDelaunay ? "Delaunay " : Gabriel ? "Delaunay-Gabriel" : "Delaunay - Restrição") << std::ends;
          Ponto.CSDescricao = strstrDescPonto.str().c_str();

          Ponto.z = CalculaCotaPonto(eit->point(), ForcarDelaunay || Gabriel ? cdtOriginal : cdt, setPontos3D, ListaPonteirosListaRestricoes, !PontoInseridoUsuario);

          if (Ponto.z == DINFINITO)
          {
            if (0)
            {
              strstrMsg.str("");
              strstrMsg << "erro irrecuperavel - Contacte o suporte - VU&E001-SU -  " << std::endl;
              strstrMsg << eit->point() << std::ends;
              AfxMessageBox(strstrMsg.str().c_str());
              GravaLog(strMsg, ArqLog);

              CFile::Remove(strNome.c_str());

              return 9;
            }
          }

          it = setPontos3D.insert(Ponto).first;

          if (SetPontosInseridosUsuario.find(Ponto) == SetPontosInseridosUsuario.end() && it != setPontos3D.end())   //--- Pontos inseridos pelo usuario não entram no arquivo de pontos inseridos por Delaunay ou Gabriel
          {
            Obs = " Ponto Inserido - " + std::string((ForcarDelaunay ? "Delaunay " : "Delaunay-Gabriel"));

            AcertaCamposGravar(Formato, ++ContaPontosInseridos, it, Obs, LinhaArquivo);

            SetItPontosInseridos.insert(it);
          }
        }

        CArcSuperficie << it->CSNome << it->CSDescricao << it->x << it->y << it->z << DINFINITO << DINFINITO << DINFINITO << false << int(0) << false << int(0);  //--- Insere o vertice no arquivo da superficie

        Vertex_circulator vc(cdt.incident_vertices(eit)), Inicio(vc);

        unsigned int ContaVertices(0), ContaVerticesInseridos(0);
        double dx(0.0), dy(0.0), dx1(0.0), dy1(0.0);

        //--- Insere as arestas ligadas ao vértice
        //--- Primeiro conta os vertices

        //--- Erro na CGAL: Esta vindo pontos ligados repetidos
        //--- A solução é coloca-los num set para remover a duplicidade

        tysetpon6d SetPLigados;

        do
        {
          //--- Erro na CGAL, ponto ligado a ele mesmo??
          dx1 = vc->point().x();
          dy1 = vc->point().y();

          dx = it->x - vc->point().x();
          dy = it->y - vc->point().y();

          if (cdt.is_infinite(vc) == false && (fabs(it->x - vc->point().x()) > 0.0001 || fabs(it->y - vc->point().y()) > 0.0001))
          {
            SetPLigados.emplace(vc->point().x(), vc->point().y());
          }

        } while (++vc != Inicio);

        ContaVertices = (int)SetPLigados.size();

        CArcSuperficie << ContaVertices;

        SetPLigados.clear();

        do
        {
          //          PontoInseridoUsuario = false;

          if (cdt.is_infinite(vc) == false)
          {
            if (SetPLigados.emplace(vc->point().x(), vc->point().y()).second == true)
            {
              //--- Calcula o azimute e a area da reta

              double Dy(vc->point().y() - eit->point().y()),
                     Dx(vc->point().x() - eit->point().x()),
                     Azxy(atan((Dy) / (Dx))),
                     S((Dy) / 2.0 * (Dx));                //--- Area de projeção da reta 

              Azxy = AzTrigToTop(Azxy, Dx, Dy);             //--- Azimute  trigonometrico para topografico

              if ((fabs(Dy) > 0.0001 || fabs(Dx) > 0.0001))  //--- Erro na CGAL : pontos ligados a ele mesmos??
              {
                ittystPonto3D it(setPontos3D.find(CPoint3D(vc->point().x(), vc->point().y(), 0.00)));
                {
                  //--- Se forçar Delaunay ou Gabriel o ponto pode não existir ainda pois pode ter sido interpolado
                  //--- Se Delaunay o ponto pode ter sido inserido na interseção de alguma restriçao 
                  //--- Porisso o ponto pode ainda não existir no set, neste caso é preciso calcular a cota do ponto 

                  if (it == setPontos3D.end())
                  {
                    //     if(SetPontosInseridosUsuario.find(CPoint3D(vc->point().x(),vc->point().y())) != SetPontosInseridosUsuario.end()) PontoInseridoUsuario = true;

                    double CotaCalculada(CalculaCotaPonto(vc->point(), ForcarDelaunay || Gabriel ? cdtOriginal : cdt, setPontos3D, ListaPonteirosListaRestricoes, false));

                    if (CotaCalculada == DINFINITO)
                    {
                      if (0)
                      {
                        strstrMsg.str("");
                        strstrMsg << "erro irrecuperavel - Contacte o suporte - VU&E001-SU - " << std::endl;
                        strstrMsg << vc->point() << std::ends;

                        AfxMessageBox(strstrMsg.str().c_str());
                        GravaLog(strstrMsg.str().c_str(), ArqLog);

                        CFile::Remove(strNome.c_str());

                        return 10;
                      }
                    }

                    it = setPontos3D.insert(CPoint3D(vc->point().x(), vc->point().y(), CotaCalculada, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_INSERIDO_DELAUNAY, " Ponto Inserido")).first;

                    //--- Pontos inseridos pelo usuário não fazem parte do grupo de pontos inseridos por Gabriel ou Delaunay

                    if (/*SetPontosInseridosUsuario.find(*it) != SetPontosInseridosUsuario.end() &&*/  SetItPontosInseridos.find(it) == SetItPontosInseridos.end())
                    {
                      SetItPontosInseridos.insert(it);

                      Obs = " Ponto Inserido - " + std::string((ForcarDelaunay ? "Delaunay " : Gabriel ? "Delaunay-Gabriel" : "Delaunay - Restrições"));

                      AcertaCamposGravar(Formato, ++ContaPontosInseridos, it, Obs, LinhaArquivo);

                      ArqPontosInseridos << LinhaArquivo.str() << std::endl;
                    }
                  }

                  if (it != setPontos3D.end())
                  {
                    ContaVerticesInseridos++;
                    CArcSuperficie << it->x << it->y << it->z << Azxy << S;     //--- Insere o ponto da face no arquivo da superfície
                    CArcSuperficie << it->CSDescricao;
                    CArcSuperficie << (unsigned int)0;
                  }
                  else
                  {
                    std::string strMsg("Erro irrecuperavel - Contacte o suporte - VU&E003-SU");   //--- Ponto duplicado
                    AfxMessageBox(strMsg.c_str());
                    GravaLog(strMsg, ArqLog);

                    CFile::Remove(strNome.c_str());

                    return 11;
                  }
                }
              }
            }
          }
        } while (++vc != Inicio);

        CArcSuperficie << false << false << int(0) << int(0);
      }

      ArqPontosInseridos << "//--- Fim do arquivo." << std::endl;

      if(TipoTriangulacao != 5)     //--- SIG (Está colocando tamanho 0 no .res)
      {
        InserePontosInseridosNasRestricoes(ListaPonteirosListaRestricoes, SetItPontosInseridos, ArqLog, strNomeProj, &MMapTipoListaRestricao);
        GravaPontosEditadosUsuario(SetPontosInseridosUsuario, SetPontosDesabilitados, strNomeProj, Formato);   //--- Recalculou(ou não) a cota dos pontos inseridos Pelo usuário, regrava o arquivo                          
        LimpaMemoria(ListaPonteirosListaRestricoes);
      }

      CArcSuperficie.Close();

      {
        std::ofstream ArqTrian(strNomeProj + ".dtr", std::ofstream::trunc);

        ArqTrian << std::right << std::fixed << std::setfill(' ') << std::setprecision(6);

        ArqTrian << cdt;

        ArqTrian.close();
      }

      strMsg = "A superficie foi calculada com exito.";
      if (pCSTexto) pCSTexto->SetWindowText(strMsg.c_str());

      GravaLog(strMsg, ArqLog);
      GravaLog("====> Fim do cálculo da superfície.", ArqLog, true);

      ArqPontosInseridos.close();
      ArqLog.close();

      if (TipoTriangulacao == TIPO_DGN_CN && TipoTriangulacao != TIPO_SIG) RemoveArqProjDGN(strNomeProj);

      return 0;   //--- zero é OK
    }
    else
    {
      
    //  if (SetPontosSemCota.size() > 0)
      if(0)
      {
       // CotaPontosSemCota(ArqLog);  //--- Em desenvolvimento 15-07-2022 , por enquanto esta logando os pontos sem cota
        strMsg = "Há pontos sem cota, a superficie não foi gerada com exito";
      }
      else
      {
        strMsg = "Houve erro. A superfície não foi gerada com exito, verifique o arquivo de log.";
        GravaLog(strMsg.c_str(), ArqLog, true);
      }
    }
  }
  else strMsg = "Erro na abertura do arquivo de pontos (xyz)";

  if (pCSTexto) pCSTexto->SetWindowText(strMsg.c_str());
  GravaLog(strMsg, ArqLog);

  GravaLog("====> Fim do cálculo da superfície.", ArqLog, true);

  return 12;
}

void InterpolaCota(const ittystPonto3D itP1, const ittystPonto3D itP2, CPoint3D& itPInter)
{
  double Dist(sqrt(pow(itP2->x - itP1->x, 2) + pow(itP2->y - itP1->y, 2))),
    i((itP2->z - itP1->z) / Dist),
    Dist2(sqrt(pow(itPInter.x - itP1->x, 2) + pow(itPInter.y - itP1->y, 2)));

  itPInter.z = i * Dist2 + itP1->z;
}

inline double CalculaCotaPontoPlano(const ittystPonto3D itP1, const ittystPonto3D itP2, const ittystPonto3D itP3, CPoint3D& PSemCota)
{
  CPoint3D Origem(*itP1);
  double MaiorCota(itP1->z);

  if (itP2->x < Origem.x) Origem.x = itP2->x;
  if (itP3->x < Origem.x) Origem.x = itP3->x;

  if (itP2->y < Origem.y) Origem.y = itP2->y;
  if (itP3->y < Origem.y) Origem.y = itP3->y;

  if (itP2->z < Origem.z) Origem.z = itP2->z;
  if (itP3->z < Origem.z) Origem.z = itP3->z;

  if (itP2->z > MaiorCota) MaiorCota = itP2->z;
  if (itP3->z > MaiorCota) MaiorCota = itP3->z;

  CPoint3D P1(*itP1), P2(*itP2), P3(*itP3), PontoSemCota(PSemCota);

  Vetor V1(P2.x - P1.x, P2.y - P1.y, P2.z - P1.z),
        V2(P3.x - P1.x, P3.y - P1.y, P3.z - P1.z),
        Vx((V1.y * V2.z) - (V1.z * V2.y), -((V1.x * V2.z) - (V1.z * V2.x)), (V1.x * V2.y) - (V1.y * V2.x));
  double d(Vx.x * P1.x + Vx.y * P1.y + Vx.z * P1.z);

  PSemCota.z =  fabs(Vx.z) < 1e-12 ? INFINITO :  (-(Vx.x * PontoSemCota.x) - (Vx.y * PontoSemCota.y) + d) / Vx.z;

  //--- no caso de drones (pontos muito proximos está calculando errado) tira a média...

  if (PSemCota.z != INFINITO && (PSemCota.z < Origem.z - 0.005 || PSemCota.z > MaiorCota + 0.005))
  {
    PSemCota.z = (itP1->z + itP2->z + itP3->z) / 3.0;
  }

  return PSemCota.z;
}

double CalculaCotaPonto(const Point& Ponto, const CDT& cdtOriginal, tysetPonto3D& setP3D, tylstptylstPonto3D* pListaPonteirosListaRestricoes, bool Vertice)
{
  CDT::Locate_type LT(CDT::VERTEX);
  int LI(CDT::VERTEX);

  Face_handle Face;

  if (Vertice) Face = cdtOriginal.locate(Ponto, LT, LI);      //--- Procura como vértice
  else  Face = cdtOriginal.locate(Ponto);                  //--- Procura como ponto solto
  double Cota(DINFINITO);
  ittystPonto3D itP1(setP3D.end()), itP2(setP3D.end()), itP3(setP3D.end());

  //--- Quando insere por forçar Delaunay ou Gabriel acha a face,
  //--- quando insere por Delaunay - interseção, não acha a face, então é preciso
  //--- procurar nas restriçoes

  Point P1, P2, P3;

  if (Face != NULL)
  {
    if (cdtOriginal.is_infinite((Face->vertex(0))))
    {
      Vertex_handle vMirror(cdtOriginal.mirror_vertex(Face, 0));
      itP1 = setP3D.find(vMirror->point());
    }
    else itP1 = setP3D.find(Face->vertex(0)->point());

    if (cdtOriginal.is_infinite((Face->vertex(1))))
    {
      Vertex_handle vMirror(cdtOriginal.mirror_vertex(Face, 1));
      itP2 = setP3D.find(vMirror->point());
    }
    else itP2 = setP3D.find(Face->vertex(1)->point());

    if (cdtOriginal.is_infinite((Face->vertex(2))))
    {
      Vertex_handle vMirror(cdtOriginal.mirror_vertex(Face, 2));
      itP3 = setP3D.find(vMirror->point());
    }
    else itP3 = setP3D.find(Face->vertex(2)->point());

    if (itP1 != setP3D.end() && itP2 != setP3D.end() && itP3 != setP3D.end())    //--- Achou a face?
    {
      CPoint3D PSemCota(Ponto);
      Cota = CalculaCotaPontoPlano(itP1, itP2, itP3, PSemCota);
    }
    
    if(Cota == DINFINITO && pListaPonteirosListaRestricoes)   //--- Se não achou a face o ponto pode ter sido inserido na interseção de alguma restrição
    {                                                         //--- (Delaunay - Interseções) então procura nas restrições 
      ittylpsetPontos3D itlRestAchou;
      ittypstPonto3D itPontoAchou;



      bool AchouNasRestricoes(VerfNaTangenteRestricoes(Ponto, pListaPonteirosListaRestricoes, itlRestAchou, itPontoAchou));

      if (AchouNasRestricoes)
      {
        ittypstPonto3D itProxPonto(itPontoAchou);
        ++itProxPonto;

        if (itProxPonto != (*itlRestAchou)->end())
        {
          double Dist1(hypot((**itProxPonto).x - (**itPontoAchou).x, (**itProxPonto).y - (**itPontoAchou).y)),
            Dist2(hypot(Ponto.x() - (**itPontoAchou).x, Ponto.y() - (**itPontoAchou).y)),
            i(((**itProxPonto).z - (**itPontoAchou).z) / Dist1);

          Cota = (**itPontoAchou).z + i * Dist2;
        }
        else Cota = DINFINITO;
      }
    }
  }

  return Cota;
}

inline void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora)
{
  CTime Agora(CTime::GetCurrentTime());
  static CTime GravAnterior(Agora);
  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill(' ') << std::setprecision(2);

  if (ForcarHora || CTimeSpan((Agora - GravAnterior)).GetTotalSeconds() > 30L)
  {
    strstreamTemp << "====> " << Agora.GetDay() << '/' << Agora.GetMonth() << '/' << Agora.GetYear() << " " << Agora.GetHour() << ':' << Agora.GetMinute() << ':' << Agora.GetSecond() << ' ' << std::ends;
    Stream << strstreamTemp.str();
    GravAnterior = Agora;
  }

  Stream << strMsg.c_str() << std::endl;

  Stream.flush();
}

inline bool VerfAlfanum(const std::string& strFrase)
{
  for (register std::string::const_iterator it = strFrase.begin(); it != strFrase.end(); it++)
    if (!iswalnum(*it)) return false;

  return true;
}

inline void PegaProximoAlfaNum(std::ifstream& Stream)
{
  do
  {
    register char Carac(Stream.get());

    if ((iswalnum(Carac) && Carac != ' ') || Carac == '-' || Carac == '+')
    {
      Stream.putback(Carac);
      return;
    }
  } while (!Stream.eof());
}

inline bool VerfPontoReta(const CPoint3D& P1, const CPoint3D& P2, const CPoint3D& P3)
{
  CPoint3D CantoInfEsq(P1), CantoSupDir(P1);

  if (CantoInfEsq.x > P2.x) CantoInfEsq.x = P2.x;
  if (CantoInfEsq.y > P2.y) CantoInfEsq.y = P2.y;

  if (CantoSupDir.x < P2.x) CantoSupDir.x = P2.x;
  if (CantoSupDir.y < P2.y) CantoSupDir.y = P2.y;

  if (P3.x < CantoInfEsq.x || P3.y < CantoInfEsq.y || P3.x > CantoSupDir.x || P3.y >  CantoSupDir.y) return false;

  //--- calcula a área do triangulo

  double S(((P3.y + P1.y) / 2.0 * (P3.x - P1.x)) +
    ((P2.y + P3.y) / 2.0 * (P2.x - P3.x)) +
    ((P1.y + P2.y) / 2.0 * (P1.x - P2.x)));

  return fabs(S) < 0.0005;
}

void InserePontosInseridosNasRestricoes(tylstptylstPonto3D* ListaPonteirosListaRestricoes, tyitsetPonto3D& SetitPontosInseridos, std::ofstream& rArqLog, const std::string& strNomeProj, const tymmlstplstPonto3D* pMMapTipoListaRestricao)
{
  //--- Insere os pontos inseridos nas restriçoes nas listas de restrições

  for (ittyitsetPonto3D itSetitPonIns = SetitPontosInseridos.begin(); itSetitPonIns != SetitPontosInseridos.end(); ++itSetitPonIns)
  {
    //int TipoRestricao((*itSetitPonIns)->Tipo - CPoint3D::PONTO_RESTRICAO);


    //if (TipoRestricao == 2) --TipoRestricao;
    //TipoRestricao = 0;

    int TipoRestricao((*itSetitPonIns)->Tipo+1);

    /*if (C == 0) TipoRestricao = PegaTipo(*itl, pMMapTipoListaRestricao);
    else TipoRestricao = 17;
    */

    bool Achou(false);

    TipoRestricao = 0;

    for (ittylpsetPontos3D itLisPonLisRes = (ListaPonteirosListaRestricoes + TipoRestricao)->begin(); !Achou && itLisPonLisRes != (ListaPonteirosListaRestricoes + TipoRestricao)->end(); ++itLisPonLisRes)
    {
      if ((*itLisPonLisRes)->size() > 2)
      {
        ittypstPonto3D itPRest1((*itLisPonLisRes)->begin()), itPRest2(itPRest1);

        for (++itPRest2; !Achou && itPRest2 != (*itLisPonLisRes)->end(); ++itPRest2)
        {
          if (VerfPontoReta(**itPRest1, **itPRest2, **itSetitPonIns) == true)
          {
            (*(*itLisPonLisRes)).insert(itPRest2, *itSetitPonIns);
            //  if(ForcarDelaunay || Gabriel) Achou = true;      //--- se for Delaunay puro o ponto pode ter sido inserido no cruzamento de 2 restriçoes
          }                                                    //--- tem que verificar em todas as restrições    

          itPRest1 = itPRest2;
        }
      }
    }
  }

  //--- Grava as restrições com os novos pontos
  //--- Caso não tenha ocorrido pontos inseridos as restriçoes voltam a ser o que eram antes, sem os pontos inseridos

  for (int C = 0; C < 2; ++C)     //--- Arquivo de restrições e de bordos
  {
    if (C == 0) GravaLog("Gravando arquivo de áreas especiais...", rArqLog);
    else continue;//GravaLog("Gravando o arquivo de bordos...",rArqLog);

    CFileException e;
    CFile Arq;

    std::string strNomeArq(strNomeProj + (C == 0 ? ".res" : ".bor"));
    unsigned int Flags(CFile::modeWrite);

    if (Arq.Open(strNomeArq.c_str(), Flags, &e) == 0)
    {
      if (e.m_cause != CFileException::fileNotFound)
      {
        std::string strTexto("Erro ao abrir o arquivo.\n");
        strTexto += strNomeArq.c_str();
        strTexto += ".\n A superficie não foi gerada ou foi gerada com erros.";
        GravaLog(strTexto, rArqLog);

        return;
      }
    }
    else
    {
      CArchive CArcRestricoes(&Arq, CArchive::store);
      size_t QRestricoes(0);
      register size_t QPontos(0);
      double x1(DINFINITO), y1(DINFINITO), x2(DINFINITO), y2(DINFINITO), z(DINFINITO), i(DINFINITO), fi(DINFINITO), S(DINFINITO);

      if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
      {
        CArcRestricoes << AngRotacaoBordos[C];

        size_t ContaListas((ListaPonteirosListaRestricoes + C)->size());
        CArcRestricoes << ContaListas;

        for (ittylpsetPontos3D itl = (ListaPonteirosListaRestricoes + C)->begin(); itl != (ListaPonteirosListaRestricoes + C)->end(); ++itl)    //--- Para todas as restrições
        {
          unsigned int TipoRestricao(0);

          if (C == 0) TipoRestricao = PegaTipo(*itl, pMMapTipoListaRestricao);
          else TipoRestricao = 17;                                           //--- CAciTopografico::FRONTEIRA 

          CArcRestricoes << TipoRestricao << (size_t)((*itl)->size());

          for (ittypstPonto3D it = (*itl)->begin(); it != (*itl)->end(); ++it)    //--- Grava todos os pontos da restrição
          {
            CArcRestricoes << (*it)->CSNome << (*it)->CSDescricao << (*it)->x << (*it)->y << (*it)->z << 0.0 << 0.0 << 0.0 << false << (int)0 << false << int(0); //(*it)->i << (*it)->fi << (*it)->->S;
          }
        }
        if (C == 1) CArcRestricoes << (double)0.0;   //--- area
      }
    }
  }
}

bool FragmentarSuperficie(CDT& cdt)
{
  /*
  tysetPonto3D SetPontosInseridos;
  typedef tysetPonto3D::iterator ittystPonto3D;

  for (register CDT::All_faces_iterator fit = ++cdt.all_faces_begin(); fit != cdt.all_faces_end(); ++fit)   //--- Insere todos os vertices
  {
  if (fit->Length() > LadoFragmento)
  {
  int Partes(fit->Length() / 30.00 + 0.5)
  {
  //--- não precisa calcular a cota, vai rodar antes de calcular a cota

  double Fragmento(fit->Length() / Partes), Deltax(Fragmento * sin(fit->Azimute())), Deltay(Fragmento * sin(fit->Azimute())),
  CompXAtual(Deltax), CompYAtual(Deltay);

  for (int C = 0 ; C < Partes; ++C)
  {
  tysetPonto3D PAtual(fit->x() + CompXAtual,fit->y() + CompYAtual, 0.0);
  SetPontosInseridos.insert(PAtual);

  CompXAtual += Deltax;
  CompYAtual += Deltay;
  }
  }
  }
  }

  for (register ittysetPonto3D it = SetPontosInseridos.begin() ; it != SetPontosInseridos.end(); ++it)
  {
  cdt.insert(point(*it));    //;--- Tem que ser antes de calcular a cota
  }
  */
  return true;
}

int PegaNomesArquivosTerreno(tylstNomeArquivos& lstNomeArquivos, std::string strNomeProj,int TipoTriangulacao)
{
  struct _finddata_t c_file;
  intptr_t hFile;
  int i(0);

  tyitlstNomeArquivos itArqPontosInseridosUsuario(lstNomeArquivos.end()),
                      itArqTriangulacao(lstNomeArquivos.end()) ;

  lstNomeArquivos.clear();

  strNomeProj = strNomeProj.substr(0, strNomeProj.find_last_of('\\'));
  strNomeProj += "\\*.xyz";

  if ((hFile = _findfirst((char*)LPCTSTR(strNomeProj.c_str()), &c_file)) == -1L)
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

      if (TipoTriangulacao == TIPO_TOP_ACAD && lstNomeArquivos.rbegin()->find("Triang.") != std::string::npos)
      {
        itArqTriangulacao = --(lstNomeArquivos.end());
      }

    } while (++i < 1000 && _findnext(hFile, &c_file) == 0);

    //--- Coloca o arquivo de pontos inseridos pelo usuario como o primeiro da lista
    //--- Este arquivo tem q ser o primeiro da lista por causa da verificação dos pontos desabilitados (que estão neste aquivo)
    //--- Se o pontos for lido no arquivo de pontos da superfície e ele estiver dasabilitado ou removido ele não entra na superficie

    if (itArqPontosInseridosUsuario != lstNomeArquivos.end())
    {
      std::string strNomeArquivo(*itArqPontosInseridosUsuario);
      lstNomeArquivos.erase(itArqPontosInseridosUsuario);
      lstNomeArquivos.push_front(strNomeArquivo);
      itArqPontosInseridosUsuario = lstNomeArquivos.begin();
    }

    // Se for triangulação Autocad coloca o arquivo de triangulação como o segundo da lista (senão perde a descrição do acidente topográfico

    if (itArqTriangulacao != lstNomeArquivos.end())
    {
      std::string strNomeArquivo(*itArqTriangulacao);
      lstNomeArquivos.erase(itArqTriangulacao);
        
      if (itArqPontosInseridosUsuario == lstNomeArquivos.end())
      {
        lstNomeArquivos.push_front(strNomeArquivo);
      }
      else
      {
        lstNomeArquivos.insert(++itArqPontosInseridosUsuario,strNomeArquivo);
      }
    }

    _findclose(hFile);
  }

  return i;
}

int ENumero(std::string strDado)
{
  static std::string Algarismos("0123456789");
  static std::string Sinais("+-");

  //--- Remove os brancos

  std::remove(strDado.begin(), strDado.end(), ' ');

  if (strDado.size() == 0) return 0;   //--- Vazio  não é numero

  //-- se o primeiro algarismo não for algarismo nem sinal, retorna FALSE.

  if (Algarismos.find(*strDado.begin()) == std::string::npos && Sinais.find(*strDado.begin()) == std::string::npos) return false;

  //--- remove o ponto e os sinais, tem q ter só um de cada

  int i(0);
  size_t Tam(strDado.size());

  std::remove(strDado.begin(), strDado.end(), '.');
  if (Tam > (strDado.size() + 1)) return false;
  Tam = strDado.size();
  std::remove(strDado.begin(), strDado.end(), '+');
  if (Tam > (strDado.size() + 1)) return false;
  Tam = strDado.size();
  std::remove(strDado.begin(), strDado.end(), '-');
  if (Tam > (strDado.size() + 1)) return false;

  //--- o q sobrou tem q ser algarismo

  for (std::string::iterator it = strDado.begin(); it != strDado.end(); ++it)
    if (Algarismos.find(*it) == -1) return false;

  return true;
}


void RotacionaCoordPlano(double& x, double& y, double Sin, double Cos)
{
  //                   Matriz de rotação


  //  x’ = x . cos (fi)  -  y . sin(fi)

  //  y’ = x . sin (fi)  +  y . cos(fi)


  double xTemp(x * Cos - y * Sin), yTemp(x * Sin + y * Cos);

  x = xTemp;
  y = yTemp;
}

unsigned int PegaTipo(const tylstitPonto3D* pitLst, const tymmlstplstPonto3D* pMMapTipoListaRestricao)
{
  cittymmlstplstPonto3D it(pMMapTipoListaRestricao->begin());

  while (it != pMMapTipoListaRestricao->end() && it->second != pitLst) ++it;

  if (it == pMMapTipoListaRestricao->end()) return 0;
  else return it->first;
}

bool VerfNaTangenteRestricoes(const Point& PAtual, tylstptylstPonto3D* pListaPonteirosListaRestricoes, ittylpsetPontos3D& itlRestAchou, ittypstPonto3D& itPontoAchou)
{
  for (ittylpsetPontos3D itLisPonLisRes = pListaPonteirosListaRestricoes->begin(); itLisPonLisRes != pListaPonteirosListaRestricoes->end(); ++itLisPonLisRes)
  {
    if ((*itLisPonLisRes)->size() > 1)
    {
      ittypstPonto3D itPRest1((*itLisPonLisRes)->begin()), itPRest2(itPRest1);

      int TipoRestricao(PegaTipo(*itLisPonLisRes, &MMapTipoListaRestricao));

      for (++itPRest2; itPRest2 != (*itLisPonLisRes)->end(); ++itPRest2)
      {
        double Raio(hypot((*itPRest2)->x - PAtual.x(), ((*itPRest2)->y - PAtual.y()))),
               DistPontos(hypot((*itPRest2)->x - (*itPRest1)->x, (*itPRest2)->y - (*itPRest1)->y));

        if (Raio < DistPontos)
        {
          double AzReta(CalculaAzimute(**itPRest1, **itPRest2)),
            AzPonto(CalculaAzimute(PAtual, **itPRest2));

          if (fabs(AzReta - AzPonto) < M_PI_4)
          {
            double Distancia(fabs(sin(AzPonto - AzReta) * Raio));

            if (Distancia < 0.005)
            {
              itlRestAchou = itLisPonLisRes;
              itPontoAchou = itPRest1;

              return true;
            }
          }
        }
        itPRest1 = itPRest2;
      }
    }
  }
  return false;
}

void GetLine(std::ifstream& ifs, std::string& Buffer, const std::string& strDelim)
{
  Buffer.clear();
  char Carac;

  if (!ifs.eof())
  {
    do
    {
      Carac = ifs.get();

      if (Carac == -1)    //--- não esta setando eof
      {
        //--- ifs.eof não funciona!!! porisso estas 3 linhas---  MSVC2017 na msvc2010 funciona

        std::ios::iostate iost(ifs.rdstate());
        iost |= std::ios::eofbit;
        ifs.setstate(iost);
      }

      Buffer.push_back(Carac);

    } while (strDelim.find(*Buffer.rbegin()) == std::string::npos && !ifs.eof());  //---- O windows não esta setando eof, carac vem com -1 neste caso

    if (!ifs.eof()) ifs.putback(*Buffer.rbegin());
    Buffer.pop_back();
  }
}

inline void PulaBrancos(std::ifstream& Stream)
{
  register char Carac(0);
  do
  {
    Carac = Stream.get();
  } while ((Carac == ' ' && Carac != '\x10') && !Stream.eof());

  if (Carac != ' ')   Stream.putback(Carac);
}

typedef std::set<CPoint3D, ltPoint_3D_2D>  tysetPonto3D;      //--- Set de pontos da superficie
typedef tysetPonto3D::iterator ittystPonto3D;
typedef std::pair< ittystPonto3D,bool> pairittystPonto3Dbool;

void GravaPontosEditadosUsuario(const tysetPonto3D& RSetPontosInseridos, const tysetPonto3D& RSetPontosDesabilitados, const std::string& strNomeProj, const std::string& Formato)
{
  std::fstream fstPonEditados(strNomeProj + "_PontosEditadosUsuario.xyz", std::fstream::out | std::fstream::trunc);

  fstPonEditados << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);
  fstPonEditados << "//--- Este arquivo contém os pontos editados pelo usuário " << std::endl;
  fstPonEditados << "//==> NÃO EDITAR. A edição deste arquivo produzirá erros no cálculo da superfície." << std::endl;
  fstPonEditados << "//" << std::endl;
  fstPonEditados << "//--- Pontos Inseridos: " << std::endl;
  fstPonEditados << "//" << std::endl;

  int ContaPontos(0);
  std::stringstream LinhaArquivo;
  std::string Obs;

  LinhaArquivo << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  for (ittystPonto3D itSPI = RSetPontosInseridos.begin(); itSPI != RSetPontosInseridos.end(); ++itSPI)
  {
    if (Formato != "n N E C O")
    {
      AcertaCamposGravar(Formato, ++ContaPontos, itSPI, std::string((const_cast<CPoint3D*>(&*itSPI))->CSDescricao.GetBuffer()), LinhaArquivo);
      fstPonEditados << LinhaArquivo.str();
    }
    else fstPonEditados << ++ContaPontos << ' ' << itSPI->y << ' ' << itSPI->x << ' ' << itSPI->z << ' ' << itSPI->CSDescricao << std::endl;     //--- Os arquivos de pontos estão com o y primeiro.
  }

  fstPonEditados << "//" << std::endl;
  fstPonEditados << "//--- Pontos Desabilitados: " << std::endl;
  fstPonEditados << "//" << std::endl;

  //--- Os pontos desabilitados não foram alterados.

  ContaPontos = 0;

  for (ittystPonto3D itSPI = RSetPontosDesabilitados.begin(); itSPI != RSetPontosDesabilitados.end(); ++itSPI)
  {
    if (Formato != "n N E C O")
    {
      AcertaCamposGravar(Formato, ++ContaPontos, itSPI, std::string((const_cast<CPoint3D*>(&*itSPI))->CSDescricao.GetBuffer()), LinhaArquivo);
      fstPonEditados << LinhaArquivo.str();
    }
    else fstPonEditados << itSPI->CSNome << ' ' << itSPI->y << ' ' << itSPI->x << ' ' << itSPI->z << ' ' << itSPI->CSDescricao << std::endl;     //--- Os arquivos de pontos estão com o y primeiro.
  }
}

inline void ArredondaXY(double& x, double& y, int Decimais)
{
  double Fator(pow(10.0, Decimais));

  x = floor(x * Fator + .5) / Fator;
  y = floor(y * Fator + .5) / Fator;
}

void LimpaMemoria(tylstptylstPonto3D* pListaPonteirosListaRestricoes)
{
  for (int C = 0; C < 2; C++)
  {
    for (ittylpsetPontos3D itl = (pListaPonteirosListaRestricoes + C)->begin(); itl != (pListaPonteirosListaRestricoes + C)->end(); ++itl)
    {
      delete* itl;
    }
    (pListaPonteirosListaRestricoes + C)->clear();
  }
}

void AcertaCamposLer(const std::string& Formato, std::string& Nome, std::string& x, std::string& y, std::string& z, std::string& Desc)
{
  std::string CopiaNome(Nome), Copiax(x), Copiay(y), Copiaz(z), CopiaDesc(Desc);
  std::string* pCampos[5] = { &CopiaNome,&Copiax,&Copiay,&Copiaz,&CopiaDesc };

  for (int C = 0; C < 9; C++, C++)
  {
    int i(C / 2);

    switch (Formato[C])
    {
    case 'n': Nome = *pCampos[i]; break;
    case 'E': x = *pCampos[i]; break;
    case 'N': y = *pCampos[i]; break;
    case 'C': z = *pCampos[i]; break;
    case 'O': Desc = *pCampos[i]; break;
    default:assert(0);
    }
  }
}

void AcertaCamposGravar(const std::string& Formato, int NumPonto, ittystPonto3D it, const std::string& Desc, std::stringstream& LinhaArquivo)
{
  LinhaArquivo.str("");

  for (int C = 0; C < 9; C++, C++)
  {
    switch (Formato[C])
    {
    case 'n': LinhaArquivo << NumPonto << ' '; break;
    case 'E': LinhaArquivo << it->x << ' '; break;
    case 'N': LinhaArquivo << it->y << ' '; break;
    case 'C': LinhaArquivo << it->z << ' '; break;
    case 'O': LinhaArquivo << Desc << ' '; break;
    default:assert(0);
    }
  }
}

int LeArqCN(const std::string& NomeProj, CDT& cdtrian, tysetPonto3D& setPon3D,int TipoTriang)
{
  CFile Arq;
  CFileException e;

  CString NomeCNDrone(NomeProj.c_str() + (TipoTriang == TIPO_SIG ? CString(".cun") : CString(".cnd")));

  if (Arq.Open(NomeCNDrone, CFile::modeRead, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      strstrMsg.str("");
      strstrMsg.seekg(0, strstrMsg.beg);
      strstrMsg << "Erro na leitura do arquivo - " << NomeCNDrone << std::ends;

      AfxMessageBox(strstrMsg.str().c_str());
      return false;
    }
  }

  CArchive CArcCurvasNivel(&Arq, CArchive::load);
  size_t QtdCurvasNivel(0), QtdPontosCNAtual(0);

  if (e.m_cause != CFileException::fileNotFound)
  {
    double EquidisCN;

    CArcCurvasNivel >> AngRotacaoBordos[0] >> EquidisCN;

    for (CArcCurvasNivel >> QtdCurvasNivel; QtdCurvasNivel; QtdCurvasNivel--)
    {
      Ponto PAtual;

      for (CArcCurvasNivel >> QtdPontosCNAtual; QtdPontosCNAtual; QtdPontosCNAtual--)
      {
        PAtual << CArcCurvasNivel;

        cdtrian.insert(Point(PAtual.x, PAtual.y));
        setPon3D.emplace(CPoint3D(PAtual.x, PAtual.y, PAtual.z));
      }
    }
  }

  return true;
}

bool RemovePontosRemovidos(CDT& cdtrian, tylstcponto& LPonRemovidos)
{
  bool Removeu(false);

  for (ittylstcponto it = LPonRemovidos.begin(); it != LPonRemovidos.end(); it++)
  {
    Removeu = false;

    Point PontoRem(it->x, it->y);
    Face_handle Face(cdtrian.locate(PontoRem));

    if (Face != NULL)
    {
      int C(0);

      while (C < 3 && !Removeu)
      {
        double dx(fabs(Face->vertex(C)->point().x() - it->x)),
               dy(fabs(Face->vertex(C)->point().y() - it->y));

        if (dx < 1e10 && dy < 1e10)
        {
          if (cdtrian.is_infinite((Face->vertex(C))))
          {
            Vertex_handle vMirror(cdtrian.mirror_vertex(Face, C));
            cdtrian.remove(vMirror);
          }
          else cdtrian.remove(Face->vertex(C));

          Removeu = true;
        }

        C++;
      }
    }
  }
  return Removeu;
}

//--- Neste caso opcionalmente pode-se passa a lista de pontos lidos
//--- Testar um pouco assim, se complicar passar na chamada a lista de pontos lidos

bool LePontosMapaIBGE(const std::string& NomeProj, CDT& cdtrian, tysetPonto3D& setPontos3D)
{
  CMapaCNIBGE MapaIBGE;

  MapaIBGE.LeArquivoDXF_DGN(NomeProj + ".dxf");

  setPontos3D.clear();

  for (ittylstCTextos it = MapaIBGE.Textos().begin(); it != MapaIBGE.Textos().end(); it++)
  {
    if(it->Tipo == 4 && ENumero(it->Texto))    //--- Só pega cotas de pontos cotados 4= pontos cotados
    {
      //--- SetPontos3D guarda os pontos com cota ja que a triangulação é 2D

      pairittystPonto3Dbool ItSet(setPontos3D.emplace(it->x*1000.0, it->y*1000.0, atof(it->Texto.c_str())));

      if(ItSet.second == true) cdtrian.insert(Point(ItSet.first->x, ItSet.first->y));
    }
  }

  return true;
}

void RemoveArqProjDGN(const std::string& strNomeProj)
{
  //--- Remove os arquivos de log etc desnecessários nona triangulacao de pontos do arqDGN

  DeleteFile((strNomeProj + "_PontosInseridos.xyz").c_str());
  DeleteFile((strNomeProj + "_CalcSuperficie.log").c_str());
  DeleteFile((strNomeProj + "_PontosEditadosUsuario.xyz").c_str());
}
void LePontosSoltosIBGE(const std::string& NomeProj, CDT& cdtrian, tysetPonto3D& setPontos3D)
{
  CFile Arq;
  CFileException e;
  std::string NomeArq(NomeProj + ".pos");
  if (Arq.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
  {
    return;
  }
  else
  {
    double AngRotacao;
    size_t Tam;
    Ponto PAtual;
    CArchive CArcPontosSoltos(&Arq, CArchive::load);
    CArcPontosSoltos >> AngRotacao;
    for(CArcPontosSoltos >> Tam ; Tam != 0 ; Tam--)
    { 
      PAtual << CArcPontosSoltos;
      setPontos3D.emplace(PAtual.x,PAtual.y,PAtual.z);
      cdtrian.insert(Point(PAtual.x, PAtual.y));
    }
  }
}

int LeSetForcaAresta(const std::string& NomeProjeto,int TipoArq, std::ofstream* ArqLog)
{
  std::string NomeArq1(NomeProjeto),NomeArq2(NomeProjeto);
  NomeArq1 += ".rav";
  NomeArq2 += ".ato";

  CFile Arq1,Arq2;
  CFileException e;

  for(int C = 0 ; C < 2 ; C++)
  {
    if(C == 0)
    {
      if (Arq1.Open(NomeArq1.c_str(), CFile::modeRead, &e) == 0)
      {
        if(TipoArq == 4)  //--- Proj AutoCad? tem q ler o .rav
        {
          if(ArqLog)
          {
            GravaLog("O Arquivo que relaciona as camadas do autocad com os acidentes topográficos \r\n \
                      do Vias Urbanas & Estradas (.rav) não foi encontrado", *ArqLog);
          }

          return 1;            
        }
      }
      else
      {
        continue;   //--- Se não é AutoCad não tem que abrir o .rav
      }
    }
    else
    {
      if (Arq2.Open(NomeArq2.c_str(), CFile::modeRead, &e) == 0)
      {
        
        if(ArqLog)
        {
          GravaLog("O Arquivo de acidentes topográficos \r\n \
                    do Vias Urbanas & Estradas (.ato) não foi encontrado", *ArqLog);
        }
        return 2;
      }
    }


    if(C == 0 && TipoArq != 4) continue;   //-- Apenas o projetos Autocad tem arquivo .rav

    CArchive CArcAciTopog(C==0 ? &Arq1 : & Arq2, CArchive::load);
    unsigned int QuanReg;

    for (CArcAciTopog >> QuanReg; QuanReg; QuanReg--)
    {
      std::string Campo, CampoAnterior;
      size_t QuanCampos;
      int NumCampo(0);

      for (CArcAciTopog >> QuanCampos; QuanCampos; QuanCampos--, NumCampo++)
      {
        //--- Os arquivos são um pouco diferentes

        if(C == 1)
        {
          if (NumCampo == 0) CArcAciTopog >> CampoAnterior;
          else CArcAciTopog >> Campo;
        }
        else
        {
          if (NumCampo == 2) CArcAciTopog >> CampoAnterior;
          else CArcAciTopog >> Campo;
        }
      }

      int AciTopog(atoi(CampoAnterior.c_str())),
          ForcaAresta(atoi(Campo.c_str()));

      SetAciTtopogFarcarAresta.emplace(AciTopog, ForcaAresta);
    }
  }
  return 0;  //--- 0 ok
}

void CotaPontosSemCota(const std::string& pstrNomeProj,std::ofstream& ArqLog, const CDT& pcdt, tysetPonto3D& pSetPontos3D)
{
  int C(1);

  if(SetPontosSemCota.size() > 0)
  {
    GravaLog("Pontos sem cota (A cota foi definida na superficie calculada):", ArqLog);

    for (const Ponto& PSemCota : SetPontosSemCota)
    { 
      const_cast<Ponto*>(&PSemCota)->z = CalculaCotaPonto(Point(PSemCota.x, PSemCota.y), pcdt, pSetPontos3D, NULL, true);

      strstrMsg.str("");
      strstrMsg << C++ << " " << PSemCota.x << " " << PSemCota.y << " " << PSemCota.z << " " << std::ends;
      GravaLog(strstrMsg.str(), ArqLog);
    }

    ArqLog.flush();

    CFile ArqPSemCota;
    CFileException e;
    std::string strNome((pstrNomeProj + std::string(".pcs")).c_str());

    if (ArqPSemCota.Open(strNome.c_str(), CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite, &e) == 0)
    {
      if (e.m_cause != CFileException::fileNotFound)
      {
        std::string strMsg(std::string("Erro ao abrir o arquivo de saida:\n") + strNome.c_str());
        GravaLog(strMsg, ArqLog);

        return ;
      }
    }

    CArchive CArcPSemCota(&ArqPSemCota, CArchive::store);

    CArcPSemCota << SetPontosSemCota.size();

    for (const Ponto& PSemCota : SetPontosSemCota)
    {
      PSemCota >> CArcPSemCota;
    }
  }
}

void InserePontosSemCotaNaSuperf(CDT& pcdt, tysetPonto3D& psetPontos3D)
{
  //--- Este foi um teste mas não ficou bom
  //--- Pontos sem cota acontecem proncipalmente no AutoCad são pontos de apoio que os projetistas por engano
  //--- colocam na camada de pontos topográficos, deste modo estes pontos são importados como pontos de 
  //--- topografia mas não são. Parece que o melhor mesmo é desprezar estes pontos
  //---
  //--- Fica o código de backup. 
  //--- Estes pontos são gravados no arquivo .psc e lidos no desenho da superfície mas nada
  //--- esta sendo feito com eles, estão apenas sendo informados no log do cálculo da superfície

  return;

  for (const Ponto& PSemCota : SetPontosSemCota)
  {
    if (PSemCota.z != 0.0 && PSemCota.z != DINFINITO)
    {
      pcdt.insert(Point(PSemCota.x, PSemCota.y));
      psetPontos3D.insert(CPoint3D(PSemCota.x, PSemCota.y, PSemCota.z, PSemCota.i, PSemCota.fi, PSemCota.S, PSemCota.AcidenteTopografico, PSemCota.Descricao));
    }
  }
}

/*
void LeSetForcaArestaAnt(const std::string& NomeProjeto)
{
  bool ArqAto(false);

  std::string NomeArq(NomeProjeto);
  NomeArq += ".rav";

  CFile ArqAT;
  CFileException e;

  if (ArqAT.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
  {
    ArqAto = true;

    std::string NomeArq(NomeProjeto);

    NomeArq += ".ato";

    if (ArqAT.Open(NomeArq.c_str(), CFile::modeRead, &e) == 0)
    {
      //--- Arquivo de acidentes topog não existe
      return;
    }
  }
  CArchive CArqAcitopog(&ArqAT, CArchive::load);

  tlstwstr LstCampos;
  unsigned int QuanReg(0);

  for (CArqAcitopog >> QuanReg; QuanReg; QuanReg--)
  {
    size_t QuanCampos(0);
    std::string Campo;
    tlstwstr LstCampos;

    for (CArqAcitopog >> QuanCampos; QuanCampos; QuanCampos--)
    {
      CArqAcitopog >> Campo;
      LstCampos.emplace_back(Campo);
    }
  }

  if (0)
  {
    int Relacao2[] = {
    -1,1,2,3,12,17,18,19,5,4,6,21,13,7,8,9,10,11,1,14,15,16,20,22,24,25,23,27,26,39,38,37,-1,41,42,40,43,44,45,46
    };

    int C = 2;

    for (auto it = LstRegAciTopog.begin(); it != LstRegAciTopog.end(); it++, C++)
    {
      std::stringstream strNum;

      strNum << Relacao2[C];

      *it->rbegin() = strNum.str();
    }

    CFileException e;
    CFile ArqAT2;

    if (ArqAT2.Open(NomeArq.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == 0)   //--- Pontos Terraplanagem Projeto
    {
      monolog.mensagem(1, NomeArq.c_str());
      return false;
    }

    CArchive CArqAcitopog(&ArqAT2, CArchive::store);

    CArqAcitopog << (unsigned int)LstRegAciTopog.size();

    for (auto it = LstRegAciTopog.begin(); it != LstRegAciTopog.end(); it++)
    {
      CArqAcitopog << it->size();

      for (auto itc = it->begin(); itc != it->end(); itc++)
      {
        CArqAcitopog << *itc;
      }
    }
  }
}
*/

int RemovePontosDesabilitados(CDT& cdt, tysetPonto3D& SetPonDesabilitados, std::ofstream& ArqLog)
{
  for (auto& RPonto : SetPonDesabilitados)
  {
    Point PonRemover(RPonto.x, RPonto.y);

    auto Face(cdt.locate(PonRemover));

    if (Face != NULL)
    {
      try
      {
        auto Tam(cdt.number_of_vertices());
        auto PontoARemover(cdt.insert(Point(RPonto.x, RPonto.y)));

        if(cdt.number_of_vertices() != Tam)  //--- Inseriu o ponto??? então tem um ponto muito proximo
        {
          auto PontoMuitoProximo(CGAL::nearest_neighbor(cdt, PontoARemover));

          if(hypot((RPonto.x - PontoMuitoProximo->point().x()), (RPonto.y - PontoMuitoProximo->point().x()) < 1E-3))
          {
            cdt.remove(PontoMuitoProximo);
          }
          cdt.remove(PontoARemover);
        }
        else
        {
          cdt.remove(PontoARemover);
        }
      }
      catch (std::exception& e)
      {
        strstrMsg.str("");

        strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(5)
                  << "Este ponto não pôde ser removido: x= " << RPonto.x << " y= " << RPonto.y;

        GravaLog(strstrMsg.str(), ArqLog);
      }
    }
  }

  return 1;
}
