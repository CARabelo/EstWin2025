#define _AFXDLL 
#include "stdafx.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_conformer_2.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>  
#include <strstream>
#include <list>
#include "io.h"
#include "..\CPonto.h"
#include "..\CLPontos.h"

struct ltpairint
{
  bool operator()(const std::pair<int, int>& pint1, const std::pair<int, int>& pint2)  const
  {
    return pint1.first < pint2.first;
  }
};

typedef std::set< std::pair<int, int>, ltpairint> tysetpairint;
typedef tysetpairint::iterator ittysetpairint;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> TDS;
typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag> CDT;
/*
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Constrained_Delaunay_triangulation_2<K> CDT;
*/
typedef CDT::Point Point;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Vertex_circulator Vertex_circulator;
typedef CDT::Face_handle Face_handle;
typedef CDT::Point Point;

extern double DINFINITO;

#include "EstWinCGAL.h"

typedef std::list<Ponto> tylstcponto;
typedef tylstcponto::iterator ittylstcponto;
typedef std::set<Ponto> SSetPontos;                   //--- Set  de um conjunto de pontos
typedef SSetPontos::iterator ItSSetPontos;
typedef std::list<CPoint3D> tylstCPoint3D;
typedef tylstCPoint3D::iterator ittylstCPoint3D;

extern double M_PI_2;
extern double M_PI;
extern double M_2_PI;
extern double M_PI_4;
extern double DINFINITO;
extern tysetpairint SetAciTtopogFarcarAresta;

extern double AngRotacaoBordos[2];
extern bool ForcarDelaunay;
extern bool Gabriel;

extern tymmlstplstPonto3D MMapTipoListaRestricao;
//extern tysetint setTipoAciTopogNaoGeraAresta;
//extern const int QTD_ACI_NAO_GERA_ARESTA(15);

/*enum TIPOSACITOP {
  NENHUM_ACITOPOG, FRONTEIRA, MURO, CERCA, MEIO_FIO, BORDO, N_A, PONTE, TAMPA_BUEIRO, POSTE, REDE_ELETRICA_BT, REDE_ELETRICA_AT, CASA,
  ROCHA, EIXO, CRISTA, PE, CANTO_MURO, CANTO_CERCA, CANTO_CASA, BUEIRO, NA_PER, LINHA_FERREA, FIM_ACITPOG
};
*/

//static TIPOSACITOP iNaoGeraAresta[QTD_ACI_NAO_GERA_ARESTA] = { NENHUM_ACITOPOG,MURO,CERCA,TAMPA_BUEIRO,POSTE,REDE_ELETRICA_BT,REDE_ELETRICA_AT,CASA,EIXO,CANTO_MURO,CANTO_CERCA,CANTO_CASA,BUEIRO,PONTE,FIM_ACITPOG };

struct stmSegment
{
  Point P1, P2;

  stmSegment(const Point& pP1, const Point& pP2) : P1(pP1), P2(pP2) {};
};

struct stltSegment
{
  bool operator()(const stmSegment& s1, const stmSegment& s2) const
  {
    double dx(s1.P1.x() - s2.P1.x()),
      dy(s1.P1.y() - s2.P1.y());

    return dx < -1e-5 ? true : dx > 1e-5 ? false :
           dy < -1e-5 ? true : dy > 1e-5 ? false : false;
  }
};

int CalcSuperficieProjTerrpl(std::string& strNomeProj, CProgressCtrl* Progresso, CStatic* pCSTexto, int pConfigSuper, tylstcponto& LstPontosRemovidos, SSetPontos& SetPontosProblema, double AreaTerrap, tylstcponto& LstFronteira, const std::string& FormatoArqPontos, int GeoRef,int TipoProj);
inline bool VerfPontoReta(const CPoint3D& P1, const CPoint3D& P2, const CPoint3D& P3);
void InserePontosInseridosNasRestricoesTerr(tylstptylstPonto3D* ListaPonteirosListaRestricoes, tyitsetPonto3D& SetitPontosInseridos, std::ofstream& rArqLog, const std::string& strNomeProj, const tymmlstplstPonto3D* pMMapTipoListaRestricao);
extern unsigned int PegaTipo(const tylstitPonto3D* pitLst, const tymmlstplstPonto3D* pMMapTipoListaRestricao);
//void GravaPontosEditadosUsuario(const tysetPonto3D& RSetPontosInseridos, const tysetPonto3D& RSetPontosDesabilitados, const std::string& strNomeProj);
bool PontoDentroTerrap(const Point& PontoVerf, tylstcponto& LstPontosLimTerrpl, double Area);
extern void LimpaMemoria(tylstptylstPonto3D* pListaPonteirosListaRestricoes);
extern inline double AzTrigToTop(double Az, double Deltax, double Deltay);
extern inline double CalculaAzimute(const CPoint3D& P1, const CPoint3D& P2);
extern inline void PegaProximoAlfaNum(std::ifstream& Stream);
extern inline double CalculaCotaPonto(const Point& Ponto, const CDT& cdtOriginal, tysetPonto3D& setP3D, tylstptylstPonto3D* ListaPonteirosListaRestricoes, bool Vertice = true);
extern inline double CalculaCotaPontoPlano(const ittystPonto3D itP1, const ittystPonto3D itP2, const ittystPonto3D itP3, CPoint3D& PSemCota);
extern inline void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora = false);
extern int PegaNomesArquivosTerreno(tylstNomeArquivos& lstNomeArquivos, const std::string strNomeProj,int TipoTriangulacao = -1);
extern int ENumero(std::string strDado);
extern void inline RotacionaCoordPlano(double& x, double& y, double Sin, double Cos);
extern bool VerfNaTangenteRestricoes(const Point& PAtual, tylstptylstPonto3D* pListaPonteirosListaRestricoes, ittylpsetPontos3D& itlRestAchou, ittypstPonto3D& itPontoAchou);
extern inline void PulaBrancos(std::ifstream& Stream);
extern void GetLine(std::ifstream& ifs, std::string& Buffer, const std::string& strDelim);
extern inline void ArredondaXY(double& x, double& y, int Casa);
bool VerfPontoDentroTerraplanagem(tylstcponto& SetPontosRemovidos, const Ponto& Ponto);
extern void AcertaCamposLer(const std::string& Formato, std::string& Nome, std::string& x, std::string& y, std::string& z, std::string& Desc);
extern int LeArqCN_AS(const std::string& NomeProj, tySetPonSuperf& setPontos);
bool FragmentaRestriçoesCruzTerrap(std::string& strNomeProj, tylstcponto& LstFronteiraTerrap, double AreaTerrap);
int GravaRestriçãoFiltrada(CArchive& CArcRestricoes, tylstCPoint3D& ListRestAtual,size_t& ContRestricoes,unsigned int Tipo);
bool VerfSegmentoCruzTerraplanagem(tylstcponto& LstFrontTerrap, Ponto& P1, Ponto& P2);
extern int LeSetForcaAresta(const std::string& NomeProjeto, int TipoArq, std::ofstream* pArqLog=nullptr);

inline double CalculaAzimute(const CPoint3D& P1, const CPoint3D& P2)
{
  double Dy(P2.y - P1.y), Dx(P2.x - P1.x), Azxy(atan(Dy / Dx));

  return AzTrigToTop(Azxy, Dx, Dy);
}

int CalcSuperficieProjTerrpl(std::string& strNomeProj, CProgressCtrl* Progresso, CStatic* pCSTexto, int pConfigSuper, tylstcponto& LstFronteiraTerrap, SSetPontos& SetPontosProblema, double AreaTerrap, tylstcponto& LstFronteira, const std::string& FormatoArqPontos, int GeoRef,int TipoProj)
{
  unsigned int ContaPontos(0);
  std::string strMsg;
  std::stringstream strstrMsg;
  std::string strNomeProjOri(strNomeProj);
  double Area(AreaTerrap);

  CDT cdt, cdtOriginal;
  tysetPonto3D setPontos3D;
  tylstptylstPonto3D ListaPonteirosListaRestricoes[3];       //--- Lista de ponteiros para as listas das restriçoes  (Bordos e restrições)
  tySetPonSuperf SetCN;
  std::string NomeArq(strNomeProj);

  for (int C = 0; C < 2; C++) NomeArq = NomeArq.substr(0, NomeArq.find_last_of('\\'));

  NomeArq += NomeArq.substr(NomeArq.find_last_of('\\'));

  LeSetForcaAresta(NomeArq,TipoProj);

  //for (int iTipo = 0; iTipo < QTD_ACI_NAO_GERA_ARESTA; ++iTipo) setTipoAciTopogNaoGeraAresta.insert(iNaoGeraAresta[iTipo]);

  strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  AngRotacaoBordos[0] = AngRotacaoBordos[1] = 0.0;

  Gabriel = (pConfigSuper & (2)) > 0;
  ForcarDelaunay = (pConfigSuper & 1) > 0;

  std::ofstream ArqLog(strNomeProj + "_ProjTerraplanagem.rel", std::ofstream::out | std::ofstream::trunc);

  if (ArqLog.good() == false)
  {
    pCSTexto->SetWindowText("Erro ao abrir o arquivo de log. Programa abortado.");
    return false;
  }

  GravaLog("====> Início do cálculo do projeto: ", ArqLog, true);

  ArqLog << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  for (int C = 0; C < 3; C++)     //--- Arquivo de restrições e de bordos e de restrições
  {
    if (C == 0) GravaLog("Lendo o arquivo de Alinhamentos...", ArqLog);
    else
    {
      if (C == 1) continue;               //--- O arquivo de bordos não esta mais sendo lido pois a fronteira não esta atualizada nele
    }

    CFileException e;
    CFile Arq;

    //--- No arquivo tpr estão os alinhamentos e os bordos
    //--- Na saída tem que salvar este arquivo como .res que 
    //--- é o arquivo de restriçoes da superficie

    std::string strNomeArq(strNomeProj + (C == 0 ? ".tpr" : C == 1 ? ".bor" : ".ref"));
    unsigned int Flags(CFile::modeRead);

    if (C == 2 && !GeoRef && TipoProj != 4)   //--- Georeferenciamentos e ACAD a terraplanagem nõ é integrada a superficie
    {
      //--- Remove das restrições os trechos que estão dentro da terraplanagem
      //--- Le o arquivo na superficie e grava o resultado no trecho

      FragmentaRestriçoesCruzTerrap(strNomeArq, LstFronteiraTerrap, AreaTerrap);
    }

    if (Arq.Open(strNomeArq.c_str(), Flags, &e) == 0)    //--- Abre o arquivo que foi fragmentado 
    {
      if (e.m_cause != CFileException::fileNotFound)
      {
        std::string strTexto("Erro ao abrir o arquivo.\n");
        strTexto += strNomeArq.c_str();
        strTexto += ".\n O Houve erros o projeto não foi gerado.";
        GravaLog(strTexto, ArqLog);

        pCSTexto->SetWindowText(strTexto.c_str());

        return false;
      }
    }
    else
    {
      CArchive CArcAlinhamentos(&Arq, CArchive::load);
      size_t QRestricoes(0);
      double x1(DINFINITO), y1(DINFINITO), x2(DINFINITO), y2(DINFINITO), z1(DINFINITO), z2(DINFINITO), i(DINFINITO), fi(DINFINITO), S(DINFINITO), Sin(0.0), Cos(0.0);

      size_t QuanAlinhamentos(0), QuanPontos(0);
      bool GeraArestaForcada(true);             //--- Todos os alinhamentos geram arestas forçadas,exceto a fronteira por enquanto... 
      unsigned int TipoAciTopog(0);

      if (C == 1 || C == 2)   //--- bordos ou restrições da superfície  Os Arquivos vêem com o angulo de rotação
      {
        CArcAlinhamentos >> AngRotacaoBordos[C];
      }

      for (CArcAlinhamentos >> QuanAlinhamentos; QuanAlinhamentos; --QuanAlinhamentos)
      {
        double Estaca, x;
        std::string Descricao;

        if (C == 0)
        {
          CArcAlinhamentos >> Estaca >> x >> Descricao;
        }
        else
        {
          CArcAlinhamentos >> TipoAciTopog;
        }

        //--- Pega o primeiro ponto

        CString CSNome, CSDescricao;
        int AcidenteTopografico;
        bool Baliza, ERestricao;
        int Baliza2;

        CArcAlinhamentos >> QuanPontos >> CSNome >> CSDescricao >> x1 >> y1 >> z1 >> i >> fi >> S >> Baliza >> Baliza2 >> ERestricao >> AcidenteTopografico;

        if (AngRotacaoBordos[C] != 0.0)
        {
          RotacionaCoordPlano(x1, y1, Sin, Cos);
          ArredondaXY(x1, y1, 3);
        }

        SetPontosProblema.clear();
        SetPontosProblema.insert(Ponto(x1, y1, z1));   //--- Salva sempre o ultimo ponto caso haja uma exceção.

        tylstitPonto3D* pListaRestricaoAtual = new (tylstitPonto3D);

        //--- No projeto de terrapplanagem os pontos sem cota são simplesmente desprezados
        //--- Este erro jã foi informado no cálculo da superfície
        //--- Isto pode causar interseção de arestas forçadas  <<<<<==== VERIFICAR

        if (!(z1 == 0.0 || z1 == INFINITO))
        {
          ittystPonto3D itSetP3D(setPontos3D.insert(CPoint3D(x1, y1, z1, i, fi, S, 0, CSDescricao)).first);
          pListaRestricaoAtual->push_back(itSetP3D);
          ListaPonteirosListaRestricoes[C].push_back(pListaRestricaoAtual);
        }
        for (--QuanPontos; QuanPontos; --QuanPontos)       //--- Para todos os pontos da restrição atual
        {
          CArcAlinhamentos >> CSNome >> CSDescricao >> x2 >> y2 >> z2 >> i >> fi >> S >> Baliza >> Baliza2 >> ERestricao >> AcidenteTopografico;
          CPoint3D P3DAtual(x2, y2, z2, i, fi, S, 0, CSDescricao, CSNome);

          if (!(z2 == 0.0 || z2 == INFINITO))
          {
            pListaRestricaoAtual->push_back((setPontos3D.insert(P3DAtual).first));

            //--- Todos os alinhamentos geram arestas forçadas

            if (GeraArestaForcada == true)
            {
              if (fabs(x1 - x2) > 1e-4 || fabs(y1 - y2) > 1e-4)                            //--- Pontos iguais??? Aconteceu nos testes
              {
                SetPontosProblema.clear();
                SetPontosProblema.insert(Ponto(x1, y1, z1));
                SetPontosProblema.insert(Ponto(x2, y2, z2));                               //--- Salva sempre o ultimo ponto caso haja uma exceção.
                cdt.insert_constraint(Point(x1, y1), Point(x2, y2));                        //--- insere o segmento na superficie como aresta forçada 

                x1 = x2;
                y1 = y2;
                z1 = z2;
              }
            }
            else                                               //---- Se o ponto não gera aresta forçada inclui-o como ponto simples.
            {
              Point PAtual(x2, y2);
              CDT::Locate_type LT(CDT::VERTEX);
              int LI(CDT::VERTEX);

              if (cdt.locate(PAtual) == NULL)
              {
                if (!PontoDentroTerrap(PAtual, LstFronteiraTerrap, AreaTerrap))
                {
                  if (!(z2 == 0.0 || z2 == INFINITO))
                  {
                    SetPontosProblema.clear();
                    SetPontosProblema.insert(Ponto(x1, y1, z1));
                    SetPontosProblema.insert(Ponto(x2, y2, z2));                               //--- Salva sempre o ultimo ponto caso haja uma exceção.

                    cdt.insert(PAtual);
                  }
                }
              }
            }
          }
        }
        if (C == 0)
        {
          //---- Quando não é pra gerar aresta forçada fica o primeiro ponto sobrando na lista, limpa e remove a lista

          if (GeraArestaForcada && pListaRestricaoAtual->size() > 1) MMapTipoListaRestricao.insert(pairmmlstplstPonto3D(AcidenteTopografico, pListaRestricaoAtual));
          else
          {
            pListaRestricaoAtual->clear();
            ListaPonteirosListaRestricoes[C].pop_back();

            cdt.insert(Point(x1, y1));    //--- a lista tem só um ponto insere o ponto como ponto comum
          }
        }
      }
    }
  }

  //--- Insere a fronteira da superficie
  //--- A fronteira de terraplanagem já foi consistida, portanto é so inserir. Ela é inserida como contraints

  if (TipoProj != 4 && GeoRef != 1 && GeoRef != 3)  //--- GeoRef == 1 => Drones 3 IBGE/Mapas
 // if(0) verificar se depende do tipo de superficie
  {
    ittylstcponto it(LstFronteira.begin());   //--- Fronteira da superficie
    Point PAnterior(it->x, it->y);

    ittystPonto3D itSetP3D(setPontos3D.insert(CPoint3D(it->x, it->y, it->z, it->i, it->fi, it->S, 0, it->Descricao)).first);
    tylstitPonto3D* pListaRestricaoAtual = new (tylstitPonto3D);
    pListaRestricaoAtual->push_back(itSetP3D);
    ListaPonteirosListaRestricoes[0].push_back(pListaRestricaoAtual);

    ittylstcponto itAnterior(it);

    SetPontosProblema.clear();
    SetPontosProblema.insert(*it);

    for (it++; it != LstFronteira.end(); it++)
    {
      //--- Se lancar uma exceção no insert_constraint setpontosproblema fica com a aresta que deu problema

      SetPontosProblema.insert(*it);

      Point PAtual(it->x, it->y);
      cdt.insert_constraint(PAnterior, PAtual);        
      pListaRestricaoAtual->push_back(setPontos3D.insert(CPoint3D(it->x, it->y, it->z, it->i, it->fi, it->S, 0, it->Descricao)).first);

      SetPontosProblema.erase(*itAnterior);
      itAnterior = it;

      PAnterior = PAtual;
    }

    MMapTipoListaRestricao.insert(pairmmlstplstPonto3D(17, pListaRestricaoAtual));
  }

  GravaLog("Lendo arquivos de terreno...", ArqLog);

  tyitsetPonto3D SetItPontosInseridos;
  tysetPonto3D SetPontosInseridosUsuario, SetPontosDesabilitados;
  tylstNomeArquivos lstNomeArquivos;
  std::string NomeProjetoBasico(strNomeProjOri.substr(0, strNomeProjOri.find_last_of('\\')));

  if (TipoProj != 4 && GeoRef != 1 && GeoRef != 3)    //--- GeoRef == 1 => Drones 3 => IBGE/Mapas
  {
    if (PegaNomesArquivosTerreno(lstNomeArquivos, NomeProjetoBasico))
    {
      //--- Se for georeferenciado a lista de arquivos de terreno estará vazia, pulará o for

      for (tyitlstNomeArquivos itNomeArqAtual = lstNomeArquivos.begin(); itNomeArqAtual != lstNomeArquivos.end(); ++itNomeArqAtual)
      {
        //--- Os pontos inseridos tem q ser lidos por que os pontos inseridos na fronteira serão usados na ligação das CNs

       // if (1 ||  itNomeArqAtual->find("PontosInseridos") == -1)  //--- pula o arquivo de pontos inseridos pelo sistema, será calculado novamente
        if (itNomeArqAtual->find("PontosInseridos") == -1)  //--- pula o arquivo de pontos inseridos pelo sistema, será calculado novamente
        {
          std::ifstream ArqPontos(*itNomeArqAtual);

          strstrMsg.str("");
          strstrMsg << "Lendo arquivo de pontos=> " << *itNomeArqAtual << std::ends;
          GravaLog(strstrMsg.str(), ArqLog);

          if (ArqPontos.is_open())     //--- Insere os pontos da superfície
          {
            int ContaLinhas(0);
            static std::string strMarcasComentarios("/!@#$%&*");
            bool ArqPontosEditados(false), PontosDesabilitados(false), PontosInseridos(false);

            if (itNomeArqAtual->find("_PontosEditadosUsuario") != -1) ArqPontosEditados = true;

            //--- 27/10/2020
            //--- No cálculo da superficie não esta considerando o arquivo de pontos editados ====>>>> VERIFICAR

            if (ArqPontosEditados) continue;  //--- Verificando

            while (ArqPontos.good())
            {
              std::string strBufferNome, strBufferx, strBuffery, strBufferz, strBufferObs;

              ++ContaLinhas;
              PulaBrancos(ArqPontos);                        //--- Pula os  brancos no início do arquivo
              GetLine(ArqPontos, strBufferNome, " \t\x0A");  //--- Nome do ponto, descarta

              if (strBufferNome.size() == 0)
              {
                std::getline(ArqPontos, strBufferNome);
                continue;
              }

              if (strMarcasComentarios.find(*strBufferNome.begin()) != std::string::npos)
              {
                std::getline(ArqPontos, strBufferNome);       //--- Descarta o resto da linha,se size = 0 é porque e linha em branco
                if (strBufferNome.find("Pontos") != std::string::npos)
                {
                  //--- Chegou nos pontos inseridos. Os pontos inseridos passam como pontos da superficie pois não sofrem alteração, se a cota estiver com 0.000
                  //--- ela é calculada, pelo mesmo processo dos pontos inseridos por Delaunay ou Gabriel.
                  //--- Os pontos desabilitados ou excluidos simplesmente não são incluídos na superfície.

                  if (strBufferNome.find("Inseridos") != std::string::npos) PontosInseridos = true;
                  else
                  {
                    if (strBufferNome.find("Desabilitados") != std::string::npos || strBufferNome.find("Removidos") != std::string::npos)
                    {
                      PontosDesabilitados = true;
                      PontosInseridos = false;
                    }
                  }
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

              if (FormatoArqPontos != "n N E C O") AcertaCamposLer(FormatoArqPontos, strBufferNome, strBuffery, strBufferx, strBufferz, strBufferObs);

              if (!strBuffery.empty() || !strBufferx.empty() || !strBufferz.empty())
              {
                if (!ENumero(strBufferx) || !ENumero(strBuffery) || !ENumero(strBufferz))
                {
                  strstrMsg.str("");
                  strstrMsg.seekg(0, strstrMsg.beg);
                  strstrMsg << "Erro na leitura do arquivo - Ponto: " << strBufferNome << " Linha = " << ContaLinhas << std::endl << " Abortando..." << std::endl << "Veja o arquivo de log" << std::ends;

                  AfxMessageBox(strstrMsg.str().c_str());

                  strstrMsg << "Dados Lidos x =" << strBufferx << " y=" << strBuffery << " z=" << strBufferz << " Obs=" << strBufferObs;
                  GravaLog(strstrMsg.str(), ArqLog);

                  return false;
                }
                else
                {
                  double x(atof(strBufferx.c_str())), y(atof(strBuffery.c_str())), z(atof(strBufferz.c_str()));

                  if (!(z == 0.0 || z == INFINITO))
                  {
                    if (!PontosDesabilitados)                                                             //--- //--- ponto desabilitados, desconnsidera
                    {
                      CPoint3D P3DAtual(x, y, z, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_SUPERF, strBufferObs.c_str(), strBufferNome.c_str());

                      if (SetPontosDesabilitados.find(CPoint3D(x, y, z)) == SetPontosDesabilitados.end())   //--- ponto desabilitados, desconnsidera
                      {
                        ittystPonto3D itSP3D(setPontos3D.find(P3DAtual));

                        if (itSP3D != setPontos3D.end())                                                   //--- Se o ponto foi encontrado é porque ele foi incluído como restrição
                        {
                          CPoint3D* pstPoint3D(const_cast<CPoint3D*>(&*itSP3D));   //--- Se o ponto foi inserido como restrição está sem nome e sem descrição, acerta agora
                          pstPoint3D->CSNome = strBufferNome.c_str();
                          pstPoint3D->CSDescricao = strBufferObs.c_str();

                          CDT::Locate_type LT(CDT::VERTEX);
                          int LI(CDT::VERTEX);
                          Point PAtual(x, y);

                          if (PontosInseridos)
                          {
                            SetPontosInseridosUsuario.insert(CPoint3D(x, y, z, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_TEMP, strBufferObs.c_str(), strBufferNome.c_str()));   //--- Os pontos inseridos tem que ser guardados, pode ser preciso calcular a cota 
                          }
                          else
                          {
                            if (!PontoDentroTerrap(PAtual, LstFronteiraTerrap, AreaTerrap))
                            {
                              SetPontosProblema.clear();
                              SetPontosProblema.insert(Ponto(x, y, z));                               //--- Salva sempre o ultimo ponto caso haja uma exceção.

                              cdt.insert(Point(x, y));                     //--- Alguns pontos de restrição foram inseridos no set e não foram inseridos na superficie 
                                                                            //--- Se o ponto ja foi inserido ele é desprezado.            
                            }
                          }
                        }
                        else
                        {
                          // Os pontos inseridos pelo usuário passam como pontos inseridos por Delaunay ou Gabriel, porisso não os insere no set de pontos da superficie
                          // Os insere no set de pontos inseridos pelo usuário.

                          if (!PontoDentroTerrap(Point(x, y), LstFronteiraTerrap, AreaTerrap))
                          {
                            if (PontosInseridos)
                            {
                              SetPontosInseridosUsuario.insert(CPoint3D(x, y, z, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_TEMP, strBufferObs.c_str(), strBufferNome.c_str()));
                            }
                            else
                            {
                              //--- Remove os pontos do terreno que estiverem dentro da terraplanagem.

                              if (!VerfPontoDentroTerraplanagem(LstFronteiraTerrap, Ponto(y, x, z)))
                              {
                                ittystPonto3D it(setPontos3D.insert(P3DAtual).first);

                                SetPontosProblema.clear();
                                SetPontosProblema.insert(Ponto(x, y, z));                               //--- Salva sempre o ultimo ponto caso haja uma exceção.

                                cdt.insert(Point(x, y));                                //--- insere-os na superficie. Obs.: As restriçoes que não forçam arestas não foram 
                              }
                            }
                          }

                          if (Progresso && (ContaPontos++ % 500) == 0)
                          {
                            Progresso->StepIt();
                            if (Progresso->GetPos() > 99) Progresso->SetPos(0);
                          }
                        }
                      }
                      else SetPontosDesabilitados.insert(CPoint3D(x, y, z, DINFINITO, DINFINITO, DINFINITO, 0, CString(strBufferObs.c_str()), CString(strBufferNome.c_str())));
                    }
                  }
                }

                if (ArqPontosEditados && strBufferNome.find("Pontos Desabilitados") != -1)
                {
                  PontosDesabilitados = true;    //--- Chegou nos pontos desabilitados
                }
              }
            }                               //--- Inseriu todos os pontos da superficie ou seja, criou a superfície de Delaunay ou de Gabriel na memória.
            if (!ArqPontos.eof())
            {
              strstrMsg.str("");
              strstrMsg << "Erro na leitura do arquivo - Linha => " << ContaLinhas << std::endl << " Abortando..." << std::ends;
              GravaLog(strstrMsg.str(), ArqLog);

              return false;
            }
          }
        }
      }
    }
    else
    {
      strMsg = "Erro na abertura do arquivo de pontos (xyz)";

      pCSTexto->SetWindowText(strMsg.c_str());
      GravaLog(strMsg, ArqLog);

      GravaLog("====> Fim do Projeto de terraplanagem.", ArqLog, true);

      return false;
    }
  }
  else     // Se o projeto for georeferenciado os pontos estão nas CN
  {
    std::string NomeArqSuperf(NomeProjetoBasico.substr(0, NomeProjetoBasico.find_last_of('\\'))),
      NomeBasico(NomeArqSuperf.substr(NomeArqSuperf.find_last_of('\\')));

    NomeArqSuperf += NomeBasico;
    SetCN.clear();
    //LeArqCN_AS(NomeArqSuperf, SetCN);   // Por serem cotas inteiras na superficie de drones ocorrem muitas diferencas por arredondamento e a interpolação das cotas fica muito diferente

    if (SetCN.size())
    {
      for (ittySetPonSuperf it = SetCN.begin(); it != SetCN.end(); ++it)
      {
        //--- Os pontos de terreno que estiverem dentro da terraplanagem não são considerados na superfície

        if (!VerfPontoDentroTerraplanagem(LstFronteiraTerrap, Ponto(it->x(), it->y(), 0.0)))
        {
          setPontos3D.insert(*it);
          cdt.insert(Point(it->x(), it->y()));
        }
      }
    }
    else
    {
      if (0)
      {
        strMsg = "Não há curvas de nível ou houve erro na abertura do arquivo das curvas de nível";

        pCSTexto->SetWindowText(strMsg.c_str());
        GravaLog(strMsg, ArqLog);

        GravaLog("====> Fim do Projeto de terraplanagem.", ArqLog, true);

        return false;
      }
    }
  }

  if (Progresso) Progresso->SetPos(100);

  std::list<Point> list_of_seeds;

  ForcarDelaunay = Gabriel = false;   //---- Gabriel: -> Dando erros  no making_conform_gabriel_2=====> Verificar

  //--- 13-050-2020
  //--- Os pontos inseridos por gabriel não estão sendo inseridos na fronteira da terraplanagem (LstFronteiraTerrap)
  //--- Porissso a superficie de terraplanagem não pode ser calculada por gabriel
  //-- este fato foi verificado quando é superf drones deve estar ocorrendo na superf comum tambem
  //--- CORRIGIR!!!!!

  if (TipoProj != 4 && GeoRef != 1 && GeoRef != 3)  Gabriel = false;            //--- GeoRef == 1 => Drones 3 => IBGE/Mapas
  //else Gabriel = true;  //--- <<<< ultima alteração
  if (ForcarDelaunay || Gabriel) cdtOriginal = cdt;                   //--- Neste ponto a superficie esta com a triangulação original, sem inserções de Delaunay ou Gabriel, 

  //--- é preciso guardá-la para calcular as cotas dos pontos inseridos.
  //--- A superfície está criada na memória, grava o arquivo da superfície.
  //--- Caso haja pontos inseridos nos vertices das restriçoes é preciso
  //--- inclui-los nas devidas listas de restrições

  if (ForcarDelaunay) CGAL::make_conforming_Delaunay_2(cdt);
  else if (Gabriel) CGAL::make_conforming_Gabriel_2(cdt);

  if (cdt.is_valid())   //--- Superfície ok?
  {
    if (0)      ///--- Aguardar
    {
      std::ofstream ArqSup(strNomeProj + "ArqSupTerr.log", std::ofstream::out | std::ofstream::trunc);

      ArqSup << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

      ArqSup << cdt;
    }

    /*
    std::ofstream ArqPontosInseridos(strNomeProj + "_PontosInseridos.xyz", std::ofstream::out | std::ofstream::trunc);
    if (!ArqPontosInseridos.is_open())  return false;
    ArqPontosInseridos << std::right << std::fixed << std::setfill(' ') << std::setprecision(5);
    ArqPontosInseridos << "//--- Este arquivo contém os pontos inseridos na superficie " << std::endl;
    ArqPontosInseridos << "//==> NÃO EDITAR. A edição deste arquivo produzirá erros no cálculo da superfície." << std::endl;
    */
    CFileException e;
    std::string strNome((strNomeProjOri + std::string(".sup")).c_str());

    //Le o Angulo de rotação

    double AngRotacao(0.0), Declinacao(0.0);

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
    size_t ContaVerticesGravados(0);

    CArcSuperficie << AngRotacao << Declinacao << (size_t)cdt.number_of_vertices();

    //--- Quando  a superfície é gabriel ou delaunay-gabriel vem alguns pontos dobrados neste for, Este set é para verificar se o ponto ja fou gravado no arquivo
    //--- 25-05-2020 - Na verdade os pontos não vem dobrados, o mesmo ponto  em em partes diferentes do arquivo com algumas arestas dobradas

    tysetPonto3D SetPontosSuperf;

    for (CDT::All_vertices_iterator eit = ++cdt.all_vertices_begin(); eit != cdt.all_vertices_end(); ++eit)   //--- Insere todos os vertices
    {
      CPoint3D PVerticeAtual(eit->point().x(), eit->point().y());
      ittystPonto3D it(setPontos3D.find(PVerticeAtual));
      bool PontoInseridoUsuario(false);

      SetPontosSuperf.insert(PVerticeAtual);    //--- Se já existir automaticamente não insere

      if (it == setPontos3D.end())    //--- Se true o ponto central é um ponto inserido na triangulação (forçar delaunay ou Gabriel ou delaunay quando cria restriçoes) o e esta sem cota
      {                               //--- é preciso calcular a cota antes de inserí-lo na superfície  
        //--- Calcula a cota no plano
        CPoint3D Ponto(eit->point().x(), eit->point().y(), DINFINITO, (ForcarDelaunay ? CPoint3D::PONTO_INSERIDO_DELAUNAY : CPoint3D::PONTO_INSERIDO_GABRIEL));

        //  if(SetPontosInseridosUsuario.find(Ponto) == SetPontosInseridosUsuario.end())
        {
          std::stringstream strstrDescPonto;

          ++ContaPontosInseridos;
          strstrDescPonto << ContaPontosInseridos << " Ponto Inserido - " << (ForcarDelaunay ? "Delaunay " : Gabriel ? "Delaunay-Gabriel" : "Delaunay - Restrição") << std::ends;
          Ponto.CSDescricao = strstrDescPonto.str().c_str();
        }
        //  else PontoInseridoUsuario = true;

        Ponto.z = CalculaCotaPonto(eit->point(), ForcarDelaunay || Gabriel ? cdtOriginal : cdt, setPontos3D, ListaPonteirosListaRestricoes, !PontoInseridoUsuario);

        if (Ponto.z == DINFINITO)
        {
          strstrMsg.str("");
          strstrMsg << "erro irrecuperavel - Contacte o suporte - VU&E004-SU -  " << std::endl;
          strstrMsg << eit->point() << std::ends;
          AfxMessageBox(strstrMsg.str().c_str());
          GravaLog(strMsg, ArqLog);

          CFile::Remove(strNome.c_str());

          return false;
        }

        it = setPontos3D.insert(Ponto).first;

        if (SetPontosInseridosUsuario.find(Ponto) == SetPontosInseridosUsuario.end() && it != setPontos3D.end())   //--- Pontos inseridos pelo usuario não entram no arquivo de pontos inseridos por Delaunay ou Gabriel
        {
          //            ArqPontosInseridos << ++ContaPontosInseridos << ' ' << it->y << ' ' << it->x << ' ' << it->z << " Ponto Inserido - " << (ForcarDelaunay ? "Delaunay " : "Delaunay-Gabriel") << std::endl;

          SetItPontosInseridos.insert(it);
        }
      }

      CArcSuperficie << it->CSNome << it->CSDescricao << it->x << it->y << it->z << DINFINITO << DINFINITO << DINFINITO << false << int(0) << false << int(0);  //--- Insere o vertice no arquivo da superficie

      ContaVerticesGravados++;

      Vertex_circulator vc(cdt.incident_vertices(eit)), Inicio(vc);

      unsigned int ContaVertices(0), ContaVerticesInseridos(0);
      double dx(0.0), dy(0.0), dx1(0.0), dy1(0.0);

      //--- Insere as arestas ligadas ao vértice

      do
      {
        if (cdt.is_infinite(vc) == false)
        {
          //--- Erro na CGAL, ponto ligado a ele mesmo??

          dx1 = vc->point().x();
          dy1 = vc->point().y();

          dx = it->x - vc->point().x();
          dy = it->y - vc->point().y();

          if (fabs(it->x - vc->point().x()) > 0.0001 || fabs(it->y - vc->point().y()) > 0.0001)
            ++ContaVertices;
        }

      } while (++vc != Inicio);   //--- Primeiro conta os vertices

      CArcSuperficie << ContaVertices;

      do
      {
        if (cdt.is_infinite(vc) == false)
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
                double CotaCalculada(CalculaCotaPonto(vc->point(), ForcarDelaunay || Gabriel ? cdtOriginal : cdt, setPontos3D, ListaPonteirosListaRestricoes, false));

                if (CotaCalculada == DINFINITO)
                {
                  //--- 14-06-2023 No debug --> não sei de onde a CGAL tirou estes pontos mas ...." siga la pelota!!!"
                  //--- 16-11-2023 São pontos de interseção entre acidentes topográficos

                  strstrMsg.str("");
                  strstrMsg << " VU&E005-SU - Não foi possível calcular a cota do ponto - " << std::endl;
                  strstrMsg << vc->point() << std::ends;

                  //AfxMessageBox(strstrMsg.str().c_str());
                  GravaLog(strstrMsg.str().c_str(), ArqLog);

                  //CFile::Remove(strNome.c_str());

                  //return false;

                  CotaCalculada = 0.00;
                }

                it = setPontos3D.insert(CPoint3D(vc->point().x(), vc->point().y(), CotaCalculada, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_INSERIDO_DELAUNAY, " Ponto Inserido")).first;

                //--- Pontos inseridos pelo usuário não fazem parte do grupo de pontos inseridos por Gabriel ou Delaunay

                if (/*SetPontosInseridosUsuario.find(*it) != SetPontosInseridosUsuario.end() &&*/  SetItPontosInseridos.find(it) == SetItPontosInseridos.end())
                {
                  SetItPontosInseridos.insert(it);

                  //                ArqPontosInseridos << ++ContaPontosInseridos << ' ' << it->y << ' ' << it->x << ' ' << it->z << " Ponto Inserido - " << (ForcarDelaunay ? "Delaunay " : Gabriel ? "Delaunay-Gabriel" : "Delaunay - Restrições") << std::endl;
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
                std::string strMsg("Erro irrecuperavel - Contacte o suporte - VU&E006-SU");   //--- Ponto duplicado
                AfxMessageBox(strMsg.c_str());
                GravaLog(strMsg, ArqLog);

                CFile::Remove(strNome.c_str());

                return false;
              }
            }
          }
        }
      } while (++vc != Inicio);

      CArcSuperficie << false << false << int(0) << int(0);
    }

    CArcSuperficie.Flush();
    ArqSuperficie.Seek(0, CFile::begin);
    CArcSuperficie << AngRotacao << ContaVerticesGravados; //--- Acerta a quantidade de vertices;

     //ArqPontosInseridos << "//--- Fim do arquivo." << std::endl;

    //--- Insere os pontos inseridos nas restrições: 
    //--- As restrições de terreno são ignoradas pois já estão na superficie e se houver alguma alteração será pouca, perto dos off-setr.
    //--- Os pontos inseridos são inseridos apenas na fronteira que é gravada no arquivo .bor

    //--- 13-050-2020
    //--- Os pontos inseridos por gabriel não estão sendo inseridos na fronteira da terraplanagem (LstFronteiraTerrap)
    //--- Porissso a superficie de terraplanagem não pode ser calculada por gabriel
    //--- CORRIGIR!!!!!

    InserePontosInseridosNasRestricoesTerr(ListaPonteirosListaRestricoes, SetItPontosInseridos, ArqLog, strNomeProjOri, &MMapTipoListaRestricao);
    //      GravaPontosEditadosUsuario(SetPontosInseridosUsuario, SetPontosDesabilitados, strNomeProj);   //--- Recalculou(ou não) a cota dos pontos inseridos Pelo usuário, regrava o arquivo                          
    LimpaMemoria(ListaPonteirosListaRestricoes);

    CArcSuperficie.Close();
    strMsg = "O projeto foi executado com exito.";
    pCSTexto->SetWindowText(strMsg.c_str());

    GravaLog(strMsg, ArqLog);
    GravaLog("====> Fim do cálculo do projeto de terraplanagem.", ArqLog, true);

    return true;
  }
  else strMsg = "houve erro. O projeto não foi executado com exito.";

  pCSTexto->SetWindowText(strMsg.c_str());
  GravaLog(strMsg, ArqLog);

  GravaLog("====> Fim do Projeto de terraplanagem.", ArqLog, true);

  return false;
}

/*
void InterpolaCota(const ittystPonto3D itP1,const ittystPonto3D itP2,stPoint3D& itPInter)
{
  double Dist(sqrt(pow(itP2->x-itP1->x,2)+pow(itP2->y-itP1->y,2))),
    i((itP2->z - itP1->z) / Dist),
    Dist2(sqrt(pow(itPInter.x-itP1->x,2)+pow(itPInter.y-itP1->y,2)));

  itPInter.z = i * Dist2 + itP1->z;
}
*/
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

  PSemCota.z = (-(Vx.x * PontoSemCota.x) - (Vx.y * PontoSemCota.y) + d) / Vx.z;

  if (PSemCota.z > Origem.z - 0.005 || PSemCota.z < MaiorCota + 0.005) return PSemCota.z;

  exit(1);

  return DINFINITO;
}
\
inline double CalculaCotaPonto(const Point& Ponto, const CDT& cdtOriginal, tysetPonto3D& setP3D, tylstptylstPonto3D* pListaPonteirosListaRestricoes, bool Vertice)
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
    else               //--- Se não achou a face o ponto pode ter sido inserido na interseção de alguma restrição
    {                  //--- (Delaunay - Interseções) então procura nas restrições 
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

void InserePontosInseridosNasRestricoesTerr(tylstptylstPonto3D* ListaPonteirosListaRestricoes, tyitsetPonto3D& SetitPontosInseridos, std::ofstream& rArqLog, const std::string& strNomeProj, const tymmlstplstPonto3D* pMMapTipoListaRestricao)
{
  //--- Insere os pontos inseridos nas restriçoes nas listas de restrições

  for (ittyitsetPonto3D itSetitPonIns = SetitPontosInseridos.begin(); itSetitPonIns != SetitPontosInseridos.end(); ++itSetitPonIns)
  {
    int TipoRestricao((*itSetitPonIns)->Tipo - CPoint3D::PONTO_RESTRICAO);

    if (TipoRestricao == 2) --TipoRestricao;

    TipoRestricao = 0;

    bool Achou(false);

    if (TipoRestricao > 1) TipoRestricao = 1;

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

  CFileException e;
  CFile Arq;

  std::string strNomeArq(strNomeProj + ".res");
  unsigned int Flags(CFile::modeWrite | CFile::modeCreate);

  if (Arq.Open(strNomeArq.c_str(), Flags, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      std::string strTexto("Erro ao abrir o arquivo.\n");
      strTexto += strNomeArq.c_str();
      strTexto += ".\n A superficie não foi gerada ou foi gerada com erros.";
      GravaLog(strTexto, rArqLog);

      //   pCSTexto->SetWindowText(strTexto.c_str());

      return;
    }
  }
  else
  {
    CArchive CArcRestricoes(&Arq, CArchive::store);
    size_t QRestricoes(0);
    register size_t QPontos(0);
    double x1(DINFINITO), y1(DINFINITO), x2(DINFINITO), y2(DINFINITO), z(DINFINITO), i(DINFINITO), fi(DINFINITO), S(DINFINITO);

    CArcRestricoes << AngRotacaoBordos[0];

    size_t ContaListas(ListaPonteirosListaRestricoes->size() + (ListaPonteirosListaRestricoes + 1)->size());
    CArcRestricoes << ContaListas;

    for (int C = 0; C < 2; C++)
    {
      for (ittylpsetPontos3D itl = (ListaPonteirosListaRestricoes + C)->begin(); itl != (ListaPonteirosListaRestricoes + C)->end(); ++itl)    //--- Para todas as restrições
      {
        unsigned int TipoRestricao(0);

        if (C == 0)
        {
          TipoRestricao = PegaTipo(*itl, pMMapTipoListaRestricao);
        }
        else 
        {
          TipoRestricao = 17;                                           //--- CAciTopografico::FRONTEIRA_TERRAP
        }

        CArcRestricoes << TipoRestricao << (size_t)((*itl)->size());

        for (ittypstPonto3D it = (*itl)->begin(); it != (*itl)->end(); ++it)    //--- Grava todos os pontos da restrição
        {
          CArcRestricoes << (*it)->CSNome << (*it)->CSDescricao << (*it)->x << (*it)->y << (*it)->z << (*it)->i << (*it)->fi << (*it)->S << false
                         << (int) 0 << ((*it)->CSDescricao.Find("Ponto Inserido") != -1) << int(0); //(*it)->i << (*it)->fi << (*it)->->S;
        }
      }
      //  CArcRestricoes << (double) 0.0;   //--- area
    }
  }
}

bool PontoDentroTerrap(const Point& PontoVerf, tylstcponto& LstPontosLimTerrpl, double Area)
{
  Ponto PontoVerificar(PontoVerf.x(), PontoVerf.y());
  unsigned int Contador(0);

  ittylstcponto itAnterior(LstPontosLimTerrpl.begin()), itAtual(itAnterior);

  for (++itAtual; itAtual != LstPontosLimTerrpl.end(); ++itAtual)
  {
    if ((Area > 0.0 && itAnterior->x < PontoVerificar.x && itAtual->x < PontoVerificar.x) ||
        (Area < 0.0 && itAnterior->x > PontoVerificar.x && itAtual->x > PontoVerificar.x))
    {
      if ((PontoVerificar.y > itAnterior->y && PontoVerificar.y < itAtual->y) ||
          (PontoVerificar.y < itAnterior->y && PontoVerificar.y > itAtual->y))
      {
        ++Contador;
      }
    }
    else
    {
      if ((itAnterior->x < PontoVerificar.x && itAtual->x > PontoVerificar.x) || (itAnterior->x > PontoVerificar.x && itAtual->x < PontoVerificar.x))
      {
        if ((itAnterior->y > PontoVerificar.y && itAtual->y < PontoVerificar.y) || (itAnterior->y < PontoVerificar.y && itAtual->y > PontoVerificar.y))
        {
          Ponto PontoIntersecao;

          if (CLPontos::CalculaIntersecao(*itAnterior, *itAtual, PontoVerificar, Ponto(PontoVerificar.x + 1000.0, PontoVerificar.y), PontoIntersecao, false) == TRUE)
          {
            if ((Area > 0.0 && PontoIntersecao.x < PontoVerificar.x) || (Area < 0.0 && PontoIntersecao.x > PontoVerificar.x))
            {
              ++Contador;
            }
          }
        }
      }
    }

    itAnterior = itAtual;
  }

  return (Contador % 2 == 1);
}

bool VerfPontoDentroTerraplanagem(tylstcponto& FronteiraTerraplanagem, const Ponto& PontoVerificar)
{
  //--- Remove do terreno os pontos do terreno que estão entro da terraplanagem

  if (FronteiraTerraplanagem.size() < 4) return false;

  if (*FronteiraTerraplanagem.begin() != *FronteiraTerraplanagem.rbegin()) return false;

  unsigned int Contador(0);
  int Area(1);               //--- A área é sempre maior que zero pois girou para a direita, fica aqui o lembrete
  ittylstcponto itFrontAnterior(FronteiraTerraplanagem.begin()), itFrontAtual(itFrontAnterior);

  for (++itFrontAtual; itFrontAtual != FronteiraTerraplanagem.end(); ++itFrontAtual)
  {
    if ((Area > 0.0 && itFrontAnterior->x < PontoVerificar.x && itFrontAtual->x < PontoVerificar.x) ||
        (Area < 0.0 && itFrontAnterior->x > PontoVerificar.x && itFrontAtual->x > PontoVerificar.x))          //--- Se girar para a esquerda...
    {
      if ((PontoVerificar.y > itFrontAnterior->y && PontoVerificar.y < itFrontAtual->y) ||
        (PontoVerificar.y < itFrontAnterior->y && PontoVerificar.y > itFrontAtual->y))
      {
        ++Contador;
      }
    }
    else
    {
      if ((itFrontAnterior->x < PontoVerificar.x && itFrontAtual->x > PontoVerificar.x) || (itFrontAnterior->x > PontoVerificar.x && itFrontAtual->x < PontoVerificar.x))
      {
        if ((itFrontAnterior->y > PontoVerificar.y && itFrontAtual->y < PontoVerificar.y) || (itFrontAnterior->y < PontoVerificar.y && itFrontAtual->y > PontoVerificar.y))
        {
          Ponto PontoIntersecao;

          if (CLPontos::CalculaIntersecao(*itFrontAnterior, *itFrontAtual, PontoVerificar, Ponto(PontoVerificar.x + 1000000.0, PontoVerificar.y), PontoIntersecao, false) == TRUE)
          {
            if ((Area > 0.0 && PontoIntersecao.x < PontoVerificar.x) || (Area < 0.0 && PontoIntersecao.x > PontoVerificar.x))
            {
              ++Contador;
            }
          }
        }
      }
    }

    itFrontAnterior = itFrontAtual;
  }

  return (Contador % 2 != 0);
}

bool FragmentaRestriçoesCruzTerrap(std::string& strNomeProj, tylstcponto& LstFronteiraTerrap, double AreaTerrap)
{
  //--- Não pode haver pontos da superfície do TN dentro da terraplanagem como tb as arestas dos acidentes topograficos
  //--- não podem cruzar a fronteira da terraplanagem. caso aconteça é preciso fragmentar o acidente topográfico
  //--- dividindo-o.

  //--- O arquivo é lido da superficie e gravado no trecho atual

  //--- 21/11/2023 Estas regra servem para quando a terraplanagem esta integrada a superficie.
  //--- Quando o projeto de terraplanagem está isolado como nos drones e nos projetos do autocad 
  //--- Esta regra não é usada pois os acidentes topográficos da superficie nem são lidos

  tylstptylstPonto3D ListaPonteirosListaRestricoes;       //--- Lista de ponteiros para as listas das restriçoes  (Bordos, restrições e acidentes topográficos da superficie)

  CFileException e;
  CFile Arq,ArqS;

  unsigned int Flags(CFile::modeRead);

  std::string strNomeArq(strNomeProj);
  strNomeArq = strNomeArq.substr(0, strNomeArq.find_last_of('\\'));
  strNomeArq = strNomeArq.substr(0, strNomeArq.find_last_of('\\'));

  std::string NomeProjeto(strNomeArq.substr(strNomeArq.find_last_of('\\')));

  strNomeArq += NomeProjeto + ".res";

  if (Arq.Open(strNomeArq.c_str(), Flags, &e) == 0)     //--- Arquivo de entrada .res da superficie
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      std::string strTexto("Erro ao abrir o arquivo.\n");
      strTexto += strNomeArq.c_str();
      strTexto += ".\n O Houve erros o projeto não foi gerado.";
     // GravaLog(strTexto, ArqLog);
     // pCSTexto->SetWindowText(strTexto.c_str());

      return false;
    }
  }
  else
  {
    strNomeArq = strNomeProj;
    strNomeArq = strNomeArq.substr(0,strNomeArq.find_last_of('.'));
    strNomeArq += ".ref";

    Flags = CFile::modeWrite | CFile::modeCreate;

    if (ArqS.Open(strNomeArq.c_str(), Flags, &e) == 0)     //--- Arquivo de Saida .ref do trecho atual (ref = restrições filtradas)
    {                                                     //--- no .res do trecho estão os alinhamentos      
      if (e.m_cause != CFileException::fileNotFound)
      {
        std::string strTexto("Erro ao abrir o arquivo.\n");
        strTexto += strNomeArq.c_str();
        strTexto += ".\n O Houve erros o projeto não foi gerado.";
        //GravaLog(strTexto, ArqLog);
        // pCSTexto->SetWindowText(strTexto.c_str());

        return false;
      }
    }

     double Sin(0.0), Cos(0.0), AngRotacaoBordos(0.0);

    size_t  QuanRestricoes(0), QuanAlinhamentos(0), QuanPontos(0),ContaRestricoes(0);
    unsigned int TipoAciTopog(0);
    tylstCPoint3D ListaRestricaoAtual;

    CArchive CArcRestricoesSuperf(&Arq, CArchive::load), CArcRestricoesTrecho(&ArqS, CArchive::store);

    CArcRestricoesSuperf >> AngRotacaoBordos >> QuanRestricoes;

    CArcRestricoesTrecho << AngRotacaoBordos << (size_t) 0;    //--- o numero de restricões pode mudar...

    for ( ; QuanRestricoes; --QuanRestricoes)
    {
      CArcRestricoesSuperf >> TipoAciTopog;

      bool PrimeiroPonto(true);

      //--- Nas restrições da superfície algumas não geram arestas forçadas, é preciso verificar
      //--- as restriçoes que não geram arestas forçadas não são inseridas na triangulação

      auto ItForcarAresta(SetAciTtopogFarcarAresta.find(std::pair<int, int>(TipoAciTopog, -1)));

      bool GeraArestaForcada(ItForcarAresta != SetAciTtopogFarcarAresta.end() && (bool)(ItForcarAresta->second) == true);

//      bool GeraArestaForcada = setTipoAciTopogNaoGeraAresta.find(TipoAciTopog) == setTipoAciTopogNaoGeraAresta.end();

      //--- Pega o primeiro ponto

      CString CSNome, CSDescricao;
      bool Baliza, ERestricao;
      int Baliza2(0);
      CPoint3D PontoAntRestricao, PontoAtualRestricao;

      CArcRestricoesSuperf >> QuanPontos >> PontoAntRestricao.CSNome >> PontoAntRestricao.CSDescricao >> PontoAntRestricao.x >> PontoAntRestricao.y 
                           >> PontoAntRestricao.z >> PontoAntRestricao.i >> PontoAntRestricao.fi >> PontoAntRestricao.S >> Baliza >> Baliza2>> ERestricao >> PontoAntRestricao.Tipo;

      //--- Os Pontos dentro da terraplanagem são removidos

      if (!GeraArestaForcada || PontoDentroTerrap(Point(PontoAntRestricao.x, PontoAntRestricao.y), LstFronteiraTerrap, AreaTerrap))
      {
        PontoAntRestricao.x = PontoAntRestricao.y = DINFINITO;  //--- ponto da restrição dentro da terraplanagem, anula
      }

      for (--QuanPontos; QuanPontos; --QuanPontos)       //--- Para todos os pontos da restrição atual
      {
        CArcRestricoesSuperf >> PontoAtualRestricao.CSNome >> PontoAtualRestricao.CSDescricao >> PontoAtualRestricao.x >> PontoAtualRestricao.y
                             >> PontoAtualRestricao.z >> PontoAtualRestricao.i >> PontoAtualRestricao.fi >> PontoAtualRestricao.S >> Baliza >> Baliza2 >> ERestricao >> PontoAtualRestricao.Tipo;

        if (!GeraArestaForcada || TipoAciTopog == 1) continue;   //--- A fronteira já foi lida

        //--- Se qualquer um dos pontos do segmento atual estiver dentro da terraplanagem, anula o segmento.
        //--- Verifica o segundo ponto

        if (PontoDentroTerrap(Point(PontoAtualRestricao.x, PontoAtualRestricao.y), LstFronteiraTerrap, AreaTerrap) ||
            VerfSegmentoCruzTerraplanagem(LstFronteiraTerrap, Ponto(PontoAntRestricao.x, PontoAntRestricao.y), Ponto(PontoAtualRestricao.x, PontoAtualRestricao.y)))
        {
          PontoAntRestricao.x = PontoAntRestricao.y = DINFINITO;     //--- O acidente topografico entrou na área da terraplanagem, anula o segmento

          if (!PrimeiroPonto)          //--- Grava a restrição atual, dividindo-a
          {
            GravaRestriçãoFiltrada(CArcRestricoesTrecho, ListaRestricaoAtual, ContaRestricoes, TipoAciTopog);
            ListaRestricaoAtual.clear();
          }

          PrimeiroPonto = true;       //--- Reinicia uma restrição (A partir do proximo desta restrição ponto que estiver fora da terraplanagem)

          continue;
        }
        else
        {
          if (PontoAntRestricao.x == DINFINITO)      //--- o ponto anterior esta dentro da terraplanagem?
          {
            PontoAntRestricao = PontoAtualRestricao;
            continue;
          }

          //--- Se chegar aqui os 2 pontos do segmento estão fora da terraplanagem
          //--- Resta então verificar se o segmento cruza a terraplanagem

          if (VerfSegmentoCruzTerraplanagem(LstFronteiraTerrap, Ponto(PontoAntRestricao.x, PontoAntRestricao.y), Ponto(PontoAtualRestricao.x, PontoAtualRestricao.y)))
          {
            PontoAntRestricao.x = PontoAntRestricao.y = PontoAtualRestricao.x = PontoAtualRestricao.y = DINFINITO;

            continue;
          }
        }
        if (PrimeiroPonto)
        {
          ListaRestricaoAtual.clear();
          ListaRestricaoAtual.push_back(PontoAntRestricao);

          PrimeiroPonto = false;
        }

        ListaRestricaoAtual.push_back(PontoAtualRestricao);
      }

      GravaRestriçãoFiltrada(CArcRestricoesTrecho, ListaRestricaoAtual,ContaRestricoes, TipoAciTopog);

      ListaRestricaoAtual.clear();
    }

    //--- Acerta a quantidade de restrições

    ArqS.Seek(0, CFile::begin);

    CArcRestricoesTrecho << AngRotacaoBordos << ContaRestricoes;

    CArcRestricoesTrecho.Flush();
  }

  return true;
}

int GravaRestriçãoFiltrada(CArchive& CArcRestricoes,tylstCPoint3D& ListRestAtual,size_t& ContRestricoes,unsigned int Tipo)
{
  if (ListRestAtual.size() > 1)
  {
    ContRestricoes++;

    CArcRestricoes << Tipo << ListRestAtual.size();

    for (ittylstCPoint3D itP3D = ListRestAtual.begin(); itP3D != ListRestAtual.end(); ++itP3D)
    {
     // unsigned int  size_t CString CString double double double double double double int int unsigned int

      /*CString CSNome, CSDescricao;
      int AcidenteTopografico;
      bool Baliza, ERestricao;
      */

      CArcRestricoes << itP3D->CSNome << itP3D->CSDescricao << itP3D->x << itP3D->y << itP3D->z
                     << itP3D->i << itP3D->fi << itP3D->S << (bool) 0 << (int) 0 << (bool) 1 << itP3D->Tipo;   //--- 0-> baliza 1-> é restrição
    }

    CArcRestricoes.Flush();
  }

  return true;
}

bool VerfSegmentoCruzTerraplanagem(tylstcponto& LstFrontTerrap, Ponto& P1, Ponto& P2)
{
  ittylstcponto itPAnterior(LstFrontTerrap.begin()), itPAtual(itPAnterior);

  ++itPAtual;

  while (itPAtual != (LstFrontTerrap.end()))
  {
    Ponto PontoIntersecao;

    if (CLPontos::CalculaIntersecao(*itPAnterior, *itPAtual, P1, P2, PontoIntersecao) == TRUE)
    {
      return true;
    }

    ++itPAnterior++;
    ++itPAtual;
  }

  return false;
}
/*
void LeSetForcaAresta(const std::string& NomeProjeto)
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
    unsigned int QuanReg;

    for (CArcAciTopog >> QuanReg; QuanReg; QuanReg--)
    {
      std::string Campo, CampoAnterior;
      size_t QuanCampos;
      int C(0);

      for (CArcAciTopog >> QuanCampos; QuanCampos; QuanCampos--, C++)
      {
        if (C == 0) CArcAciTopog >> CampoAnterior;
        else CArcAciTopog >> Campo;
      }

      int AciTopog(atoi(CampoAnterior.c_str())),
        ForcaAresta(atoi(Campo.c_str()));

      SetAciTtopogFarcarAresta.emplace(AciTopog, ForcaAresta);
    }
  }
}
*/

