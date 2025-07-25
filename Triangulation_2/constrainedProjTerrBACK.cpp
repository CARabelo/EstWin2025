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

typedef std::list<Ponto> tylstcponto;
typedef tylstcponto::iterator ittylstcponto;
typedef std::set<Ponto> SSetPontos;                   //--- Set  de um conjunto de pontos
typedef SSetPontos::iterator ItSSetPontos;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Exact_predicates_tag Itag;
typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Constrained_Delaunay_triangulation_2<K> CDT;
typedef CDT::Point Point;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Vertex_circulator Vertex_circulator;
typedef CDT::Face_handle Face_handle;

extern double M_PI_2;
extern double M_PI;
extern double M_2_PI;
extern double M_PI_4;
extern double DINFINITO;

#include "EstWinCGAL.h"

extern double AngRotacaoBordos[2];
extern bool ForcarDelaunay;
extern bool Gabriel;


extern tymmlstplstPonto3D MMapTipoListaRestricao;

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

class CMyPoint : public Point
{
public:
  double z;

  CMyPoint(double px, double py, double pz) : Point(px, py), z(pz) {};
  const bool operator<(const CMyPoint& RHS) const
  {
    double dx(this->x() - RHS.x()), dy(this->y() - RHS.y()), dz(z - RHS.z);

    return dx < -1e-5 ? true : dx > 1e-5 ? false :
      dy < -1e-5 ? true : dy > 1e-5 ? false :
      dz < -1e-5 ? true : dz > 1e-5 ? false : false;
  }
};

struct stltCMyPoint         //--- Compara��o em 2 dimens�es
{
  bool operator()(const CMyPoint& P1, const CMyPoint& P2) const
  {
    double dx(P1.x() - P2.x()),
      dy(P1.y() - P2.y());

    return dx < -1e-5 ? true : dx > 1e-5 ? false :
      dy < -1e-5 ? true : dy > 1e-5 ? false : false;
  }
};

typedef std::set<CMyPoint, stltCMyPoint> tySetPonSuperf;
typedef tySetPonSuperf::iterator ittySetPonSuperf;

  /*: public Point
{
public:
  double z;

  CMyPoint(double px, double py, double pz) : Point(px, py), z(pz) {};
  const bool operator<(const CMyPoint& RHS) const
  {
    double dx(this->x() - RHS.x()), dy(this->y() - RHS.y()), dz(z - RHS.z);

    return dx < -1e-5 ? true : dx > 1e-5 ? false :
      dy < -1e-5 ? true : dy > 1e-5 ? false :
      dz < -1e-5 ? true : dz > 1e-5 ? false : false;
  }
};

*/

int CalcSuperficieProjTerrpl(std::string& strNomeProj, CProgressCtrl* Progresso, CStatic* pCSTexto, int pConfigSuper, tylstcponto& LstPontosRemovidos, SSetPontos& SetPontosProblema, double AreaTerrap, tylstcponto& LstFronteira, const std::string& FormatoArqPontos, int GeoRef);
//int CalcSuperficieProjTerrpl(std::string& strNomeProj, CProgressCtrl* Progresso, CStatic* pCSTexto, int pConfigSuper, tylstcponto& LstPontosRemovidos, double AreaTerrap, tylstcponto& LstFronteira, const std::string& FormatoArqPontos,int GeoRef);
inline bool VerfPontoReta(const CPoint3D& P1, const CPoint3D& P2, const CPoint3D& P3);
void InserePontosInseridosNasRestricoesTerr(tylstptylstPonto3D* ListaPonteirosListaRestricoes, tyitsetPonto3D& SetitPontosInseridos, std::ofstream& rArqLog, const std::string& strNomeProj, const tymmlstplstPonto3D* pMMapTipoListaRestricao);
extern unsigned int PegaTipo(const tylstitPonto3D* pitLst, const tymmlstplstPonto3D* pMMapTipoListaRestricao);
void GravaPontosEditadosUsuario(const tysetPonto3D& RSetPontosInseridos, const tysetPonto3D& RSetPontosDesabilitados, const std::string& strNomeProj);
bool PontoARemover(const Point& PontoVerf, tylstcponto& LstPontosLimTerrpl, double Area);
extern void LimpaMemoria(tylstptylstPonto3D* pListaPonteirosListaRestricoes);
extern inline double AzTrigToTop(double Az, double Deltax, double Deltay);
extern inline double CalculaAzimute(const CPoint3D& P1, const CPoint3D& P2);
extern inline void PegaProximoAlfaNum(std::ifstream& Stream);
extern inline double CalculaCotaPonto(const Point& Ponto, const CDT& cdtOriginal, tysetPonto3D& setP3D, tylstptylstPonto3D* ListaPonteirosListaRestricoes, bool Vertice = true);
extern inline double CalculaCotaPontoPlano(const ittystPonto3D itP1, const ittystPonto3D itP2, const ittystPonto3D itP3, CPoint3D& PSemCota);
extern inline void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora = false);
extern int PegaNomesArquivosTerreno(tylstNomeArquivos& lstNomeArquivos, const std::string strNomeProj);
extern int ENumero(std::string strDado);
extern void inline RotacionaCoordPlano(double &x, double &y, double Sin, double Cos);
extern bool VerfNaTangenteRestricoes(const Point& PAtual, tylstptylstPonto3D* pListaPonteirosListaRestricoes, ittylpsetPontos3D& itlRestAchou, ittypstPonto3D& itPontoAchou);
extern inline void PulaBrancos(std::ifstream& Stream);
extern void GetLine(std::ifstream& ifs, std::string& Buffer, const std::string& strDelim);
extern inline void ArredondaXY(double& x, double& y, int Casa);
bool VerfPontoDentroTerraplanagem(tylstcponto& SetPontosRemovidos, const Ponto& Ponto);
extern void AcertaCamposLer(const std::string& Formato, std::string& Nome, std::string& x, std::string& y, std::string& z, std::string& Desc);
extern int LeArqCN_AS(const std::string& NomeProj, tySetPonSuperf& setPontos);

inline double CalculaAzimute(const CPoint3D& P1, const CPoint3D& P2)
{
  double Dy(P2.y - P1.y), Dx(P2.x - P1.x), Azxy(atan(Dy / Dx));

  return AzTrigToTop(Azxy, Dx, Dy);
}

int CalcSuperficieProjTerrpl(std::string& strNomeProj, CProgressCtrl* Progresso, CStatic* pCSTexto, int pConfigSuper, tylstcponto& LstPontosRemovidos, SSetPontos& SetPontosProblema, double AreaTerrap, tylstcponto& LstFronteira, const std::string& FormatoArqPontos,int GeoRef)
{
  CDT cdt, cdtOriginal;
  tysetPonto3D setPontos3D;                                  //--- Set de backup, para salvar o z (Verificar necessidade)
  unsigned int ContaPontos(0);
  std::string strMsg;
  tylstptylstPonto3D ListaPonteirosListaRestricoes[2];       //--- Lista de ponteiros para as listas das restri�oes  (Bordos e restri��es)
  std::stringstream strstrMsg;
  std::string strNomeProjOri(strNomeProj);
  double Area(AreaTerrap);

  strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  AngRotacaoBordos[0] = AngRotacaoBordos[1] = 0.0;

  Gabriel = (pConfigSuper & (2)) > 0;
  ForcarDelaunay = (pConfigSuper & 1) > 0;

  std::ofstream ArqLog(strNomeProj + "_ProjTerraplanagem.rel", std::ofstream::out | std::ofstream::app);

  if (ArqLog.good() == false)
  {
    pCSTexto->SetWindowText("Erro ao abrir o arquivo de log. Programa abortado.");
    return false;
  }

  GravaLog("===> In�cio do c�lculo do projeto: ", ArqLog, true);

  for (int C = 0; C < 2; C++)     //--- Arquivo de restri��es e de bordos
  {
    if (C == 0) GravaLog("Lendo o arquivo de Alinhamentos...", ArqLog);
    else
    {
      continue;               //--- O arquivo de bordos n�o esta mais sendo lido pois a fronteira n�o esta atualizada nele
      /*
      std::string strNomeProjBasico(strNomeProj.substr(0, strNomeProj.find_last_of('\\')));
      strNomeProjBasico = strNomeProjBasico.substr(0, strNomeProjBasico.find_last_of('\\'));
      strNomeProj = strNomeProjBasico + strNomeProjBasico.substr(strNomeProjBasico.find_last_of('\\', std::string::npos));

      GravaLog("Lendo o arquivo de bordos...", ArqLog);
      */
    }

    CFileException e;
    CFile Arq;

    //--- No arquivo tpr est�o os alinhamentos e os bordos
    //--- Na sa�da tem que salvar este arquivo como .res que 
    //--- � o arquivo de restri�oes da superficie

    std::string strNomeArq(strNomeProj + (C == 0 ? ".tpr" : ".bor"));
    unsigned int Flags(CFile::modeRead);

    if (Arq.Open(strNomeArq.c_str(), Flags, &e) == 0)
    {
      if (e.m_cause != CFileException::fileNotFound)
      {
        std::string strTexto("Erro ao abrir o arquivo.\n");
        strTexto += strNomeArq.c_str();
        strTexto += ".\n O Houve erros o projeto n�o foi gerado.";
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
      bool GeraArestaForcada(true);             //--- Todos os alinhamentos geram arestas for�adas,exceto a fronteira por enquanto... 
      unsigned int TipoAciTopog(0);

      if (C == 1)   //--- bordos??  o Arquivo de bordos � um pouco diferente
      {
        CArcAlinhamentos >> AngRotacaoBordos[0];
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

        CArcAlinhamentos >> QuanPontos >> CSNome >> CSDescricao >> x1 >> y1 >> z1 >> i >> fi >> S >> Baliza >> ERestricao >> AcidenteTopografico;

        if (AngRotacaoBordos[C] != 0.0)
        {
          RotacionaCoordPlano(x1, y1, Sin, Cos);
          ArredondaXY(x1, y1, 3);
        }

        SetPontosProblema.clear();
        SetPontosProblema.insert(Ponto(x1, y1, z1));                               //--- Salva sempre o ultimo ponto caso haja uma exce��o.

        ittystPonto3D itSetP3D(setPontos3D.insert(CPoint3D(x1, y1, z1, i, fi, S, 0, CSDescricao)).first);
        tylstitPonto3D* pListaRestricaoAtual = new (tylstitPonto3D);
        pListaRestricaoAtual->push_back(itSetP3D);
        ListaPonteirosListaRestricoes[C].push_back(pListaRestricaoAtual);

        for (--QuanPontos; QuanPontos; --QuanPontos)       //--- Para todos os pontos da restri��o atual
        {
          CArcAlinhamentos >> CSNome >> CSDescricao >> x2 >> y2 >> z2 >> i >> fi >> S >> Baliza >> ERestricao >> AcidenteTopografico;
          CPoint3D P3DAtual(x2, y2, z2, i, fi, S, 0, CSDescricao, CSNome);

          pListaRestricaoAtual->push_back((setPontos3D.insert(P3DAtual).first));

          //--- Todos os alinhamentos geram arestas for�adas

          if (GeraArestaForcada == true)
          {
            if (fabs(x1 - x2) > 1e-4 || fabs(y1 - y2) > 1e-4)                            //--- Pontos iguais??? Aconteceu nos testes
            {
              SetPontosProblema.clear();
              SetPontosProblema.insert(Ponto(x1, y1, z1));
              SetPontosProblema.insert(Ponto(x2, y2, z2));                               //--- Salva sempre o ultimo ponto caso haja uma exce��o.
              cdt.insert_constraint(Point(x1, y1), Point(x2, y2));                        //--- insere o segmento na superficie como aresta for�ada 

              x1 = x2;
              y1 = y2;
              z1 = z2;
            }
          }
          else                                               //---- Se o ponto n�o gera aresta for�ada inclui-o como ponto simples.
          {
            Point PAtual(x2, y2);
            CDT::Locate_type LT(CDT::VERTEX);
            int LI(CDT::VERTEX);

            if (cdt.locate(PAtual) == NULL)
            {
              if (!PontoARemover(PAtual, LstPontosRemovidos, AreaTerrap))
              {
                SetPontosProblema.clear();
                SetPontosProblema.insert(Ponto(x1, y1, z1));
                SetPontosProblema.insert(Ponto(x2, y2, z2));                               //--- Salva sempre o ultimo ponto caso haja uma exce��o.

                cdt.insert(PAtual);
              }
            }
          }
        }
        if (C == 0)
        {
          //---- Quando n�o � pra gerar aresta for�ada fica o primeiro ponto sobrando na lista, limpa e remove a lista

          if (GeraArestaForcada && pListaRestricaoAtual->size() > 1) MMapTipoListaRestricao.insert(pairmmlstplstPonto3D(AcidenteTopografico, pListaRestricaoAtual));
          else
          {
            pListaRestricaoAtual->clear();
            ListaPonteirosListaRestricoes[C].pop_back();

            cdt.insert(Point(x1,y1));    //--- a lista tem s� um ponto insere o ponto como ponto comum
          }
        }
      }
    }
  }

  //--- Insere a fronteira da superficie
  //--- A fronteira j� foi consistida, portanto � so inserir

  ittylstcponto it(LstFronteira.begin());
  Point PAnterior(it->x, it->y);

  ittystPonto3D itSetP3D(setPontos3D.insert(CPoint3D(it->x, it->y, it->z, it->i, it->fi, it->S, 0, it->Descricao)).first);
  tylstitPonto3D* pListaRestricaoAtual = new (tylstitPonto3D);
  pListaRestricaoAtual->push_back(itSetP3D);
  ListaPonteirosListaRestricoes[0].push_back(pListaRestricaoAtual);

  for (it++; it != LstFronteira.end(); it++)
  {
    Point PAtual(it->x, it->y);
    cdt.insert_constraint(PAnterior, PAtual);
    pListaRestricaoAtual->push_back(setPontos3D.insert(CPoint3D(it->x, it->y, it->z, it->i, it->fi, it->S, 0, it->Descricao)).first);

    PAnterior = PAtual;
  }

  MMapTipoListaRestricao.insert(pairmmlstplstPonto3D(1, pListaRestricaoAtual));

  GravaLog("Lendo arquivos de terreno...", ArqLog);

  tyitsetPonto3D SetItPontosInseridos;
  tysetPonto3D SetPontosInseridosUsuario, SetPontosDesabilitados;
  tylstNomeArquivos lstNomeArquivos;
  std::string NomeProjetoBasico(strNomeProjOri.substr(0, strNomeProjOri.find_last_of('\\')));

  if (GeoRef)     // Se o projeto for georeferenciado os pontos est�o nas CN
  {
    std::string NomeArqSuperf(NomeProjetoBasico.substr(0, NomeProjetoBasico.find_last_of('\\'))),
                NomeBasico(NomeArqSuperf.substr(NomeArqSuperf.find_last_of('\\')));

    NomeArqSuperf += NomeBasico;
    tySetPonSuperf SetCN;
    LeArqCN_AS(NomeArqSuperf, SetCN);

    ittySetPonSuperf it = SetCN.begin();

    for ( ; it != SetCN.end(); ++it)
    {
      cdt.insert(Point(it->x(),it->y()));
    }
  }
  else
  if (GeoRef || PegaNomesArquivosTerreno(lstNomeArquivos, NomeProjetoBasico))
  {
    for (tyitlstNomeArquivos itNomeArqAtual = lstNomeArquivos.begin(); itNomeArqAtual != lstNomeArquivos.end(); ++itNomeArqAtual)
    {
      if (itNomeArqAtual->find("PontosInseridos") == -1)  //--- pula o arquivo de pontos inseridos pelo sistema, ser� calculado novamente
      {
        std::ifstream ArqPontos(*itNomeArqAtual);

        strstrMsg.str("");
        strstrMsg << "Lendo arquivo de pontos=> " << *itNomeArqAtual << std::ends;
        GravaLog(strstrMsg.str(), ArqLog);

        if (ArqPontos.is_open())     //--- Insere os pontos da superf�cie
        {
          int ContaLinhas(0);
          static std::string strMarcasComentarios("/!@#$%&*");
          bool ArqPontosEditados(false), PontosDesabilitados(false), PontosInseridos(false);

          if (itNomeArqAtual->find("_PontosEditadosUsuario") != -1) ArqPontosEditados = true;

          while (ArqPontos.good())
          {
            std::string strBufferNome, strBufferx, strBuffery, strBufferz, strBufferObs;

            ++ContaLinhas;
            PulaBrancos(ArqPontos);                        //--- Pula os  brancos no in�cio do arquivo
            GetLine(ArqPontos, strBufferNome, " \t\x0A");       //--- Nome do ponto, descarta

            if (strBufferNome.size() == 0)
            {
              std::getline(ArqPontos, strBufferNome);
              continue;
            }

            if (strMarcasComentarios.find(*strBufferNome.begin()) != std::string::npos)
            {
              std::getline(ArqPontos, strBufferNome);       //--- Descarta o resto da linha,se size = 0 � porque e linha em branco
              if (strBufferNome.find("Pontos") != std::string::npos)
              {
                //--- Chegou nos pontos inseridos. Os pontos inseridos passam como pontos da superficie pois n�o sofrem altera��o, se a cota estiver com 0.000
                //--- ela � calculada, pelo mesmo processo dos pontos inseridos por Delaunay ou Gabriel.
                //--- Os pontos desabilitados simplesmente n�o s�o inclu�dos na superf�cie.

                if (strBufferNome.find("Inseridos") != std::string::npos) PontosInseridos = true;
                else
                {
                  if (strBufferNome.find("Desabilitados") != std::string::npos)
                  {
                    PontosDesabilitados = true;
                    PontosInseridos = false;
                  }
                }
              }
              continue;
            }

            PegaProximoAlfaNum(ArqPontos);
            GetLine(ArqPontos, strBuffery, "  \t\x0A");   //--- x
            PegaProximoAlfaNum(ArqPontos);
            GetLine(ArqPontos, strBuffery, "  \t\x0A");   //--- y
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
                strstrMsg << "Erro na leitura do arquivo - " << strBufferNome << " Linha = " << ContaLinhas << std::endl << " Abortando..." << std::endl << "Veja o arquivo de log" << std::ends;

                AfxMessageBox(strstrMsg.str().c_str());

                strstrMsg << "Dados Lidos x =" << strBufferx << " y=" << strBuffery << " z=" << strBufferz << " Obs=" << strBufferObs;
                GravaLog(strstrMsg.str(), ArqLog);

                return false;
              }
              else
              {
                double x(atof(strBufferx.c_str())), y(atof(strBuffery.c_str())), z(atof(strBufferz.c_str()));

                if (!PontosDesabilitados)                                                             //--- //--- ponto desabilitados, desconnsidera
                {
                  CPoint3D P3DAtual(x, y, z, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_SUPERF, strBufferObs.c_str(), strBufferNome.c_str());

                  if (SetPontosDesabilitados.find(CPoint3D(x, y, z)) == SetPontosDesabilitados.end())   //--- ponto desabilitados, desconnsidera
                  {
                    ittystPonto3D itSP3D(setPontos3D.find(P3DAtual));

                    if (itSP3D != setPontos3D.end())                                                   //--- Se o ponto foi encontrado � porque ele foi inclu�do como restri��o
                    {
                      CPoint3D* pstPoint3D(const_cast<CPoint3D*>(&*itSP3D));   //--- Se o ponto foi inserido como restri��o est� sem nome e sem descri��o, acerta agora
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
                        if (!PontoARemover(PAtual, LstPontosRemovidos, AreaTerrap))
                        {
                          SetPontosProblema.clear();
                          SetPontosProblema.insert(Ponto(x, y, z));                               //--- Salva sempre o ultimo ponto caso haja uma exce��o.

                          cdt.insert(Point(x, y));                     //--- Alguns pontos de restri��o foram inseridos no set e n�o foram inseridos na superficie 
                                                                       //--- Se o ponto ja foi inserido ele � desprezado.            
                        }
                      }
                    }
                    else
                    {
                      // Os pontos inseridos pelo usu�rio passam como pontos inseridos por Delaunay ou Gabriel, porisso n�o os insere no set de pontos da superficie
                      // Os insere no set de pontos inseridos pelo usu�rio.

                      if (!PontoARemover(Point(x, y), LstPontosRemovidos, AreaTerrap))
                      {
                        if (PontosInseridos)
                        {
                          SetPontosInseridosUsuario.insert(CPoint3D(x, y, z, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_TEMP, strBufferObs.c_str(), strBufferNome.c_str()));
                        }
                        else
                        {
                          //--- Os pontos inseridos tem que ser guardados, pode ser preciso calcular a cota       
                          //--- e passam como pontos inseridos por Delaunay ou Gabriel, porisso n�o os insere no set 
                          //--- Outros pontos: insere-os no set de backup.

                          if (!VerfPontoDentroTerraplanagem(LstPontosRemovidos, Ponto(x, y, z)))
                          {
                            ittystPonto3D it(setPontos3D.insert(P3DAtual).first);

                            SetPontosProblema.clear();
                            SetPontosProblema.insert(Ponto(x, y, z));                               //--- Salva sempre o ultimo ponto caso haja uma exce��o.

                            cdt.insert(Point(x, y));                                //--- insere-os na superficie. Obs.: As restri�oes que n�o for�am arestas n�o foram 
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

              if (ArqPontosEditados && strBufferNome.find("Pontos Desabilitados") != -1) PontosDesabilitados = true;    //--- Chegou nos pontos desabilitados
            }
          }                               //--- Inseriu todos os pontos da superficie ou seja, criou a superf�cie de Delaunay ou de Gabriel na mem�ria.
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

    if (Progresso) Progresso->SetPos(100);

    std::list<Point> list_of_seeds;

    ForcarDelaunay = Gabriel = false;   //---- Dando erros =====> Verificar

    if (ForcarDelaunay || Gabriel) cdtOriginal = cdt;                   //--- Neste ponto a superficie esta com a triangula��o original, sem inser��es de Delaunay ou Gabriel, 
    //--- � preciso guard�-la para calcular as cotas dos pontos inseridos.
    //--- A superf�cie est� criada na mem�ria, grava o arquivo da superf�cie.
    //--- Caso haja pontos inseridos nos vertices das restri�oes � preciso
    //--- inclui-los nas devidas listas de restri��es

    if (ForcarDelaunay) CGAL::make_conforming_Delaunay_2(cdt);
    else if (Gabriel) CGAL::make_conforming_Gabriel_2(cdt);

    if (cdt.is_valid())   //--- Superf�cie ok?
    {
      /*
      std::ofstream ArqPontosInseridos(strNomeProj + "_PontosInseridos.xyz", std::ofstream::out | std::ofstream::trunc);
      if (!ArqPontosInseridos.is_open())  return false;
      ArqPontosInseridos << std::right << std::fixed << std::setfill(' ') << std::setprecision(5);
      ArqPontosInseridos << "//--- Este arquivo cont�m os pontos inseridos na superficie " << std::endl;
      ArqPontosInseridos << "//==> N�O EDITAR. A edi��o deste arquivo produzir� erros no c�lculo da superf�cie." << std::endl;
      */
      CFileException e;
      std::string strNome((strNomeProjOri + std::string(".sup")).c_str());

      //Le o Angulo de rota��o

      double AngRotacao(0.0);

      GravaLog("Gravando o arquivo de sa�da...", ArqLog);

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

      CArcSuperficie << AngRotacao << (size_t)cdt.number_of_vertices();

      for (CDT::All_vertices_iterator eit = ++cdt.all_vertices_begin(); eit != cdt.all_vertices_end(); ++eit)   //--- Insere todos os vertices
      {
        CPoint3D PVerticeAtual(eit->point().x(), eit->point().y());
        ittystPonto3D it(setPontos3D.find(PVerticeAtual));
        bool PontoInseridoUsuario(false);

        if (it == setPontos3D.end())    //--- Se true o ponto central � um ponto inserido na triangula��o (for�ar delaunay ou Gabriel ou delaunay quando cria restri�oes) o e esta sem cota
        {                               //--- � preciso calcular a cota antes de inser�-lo na superf�cie  
          //--- Calcula a cota no plano
          CPoint3D Ponto(eit->point().x(), eit->point().y(), DINFINITO, (ForcarDelaunay ? CPoint3D::PONTO_INSERIDO_DELAUNAY : CPoint3D::PONTO_INSERIDO_GABRIEL));

          //  if(SetPontosInseridosUsuario.find(Ponto) == SetPontosInseridosUsuario.end())
          {
            std::stringstream strstrDescPonto;

            ++ContaPontosInseridos;
            strstrDescPonto << ContaPontosInseridos << " Ponto Inserido - " << (ForcarDelaunay ? "Delaunay " : Gabriel ? "Delaunay-Gabriel" : "Delaunay - Restri��o") << std::ends;
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

          if (SetPontosInseridosUsuario.find(Ponto) == SetPontosInseridosUsuario.end() && it != setPontos3D.end())   //--- Pontos inseridos pelo usuario n�o entram no arquivo de pontos inseridos por Delaunay ou Gabriel
          {
            //            ArqPontosInseridos << ++ContaPontosInseridos << ' ' << it->y << ' ' << it->x << ' ' << it->z << " Ponto Inserido - " << (ForcarDelaunay ? "Delaunay " : "Delaunay-Gabriel") << std::endl;

            SetItPontosInseridos.insert(it);
          }
        }

        CArcSuperficie << it->CSNome << it->CSDescricao << it->x << it->y << it->z << DINFINITO << DINFINITO << DINFINITO << false << false << int(0);  //--- Insere o vertice no arquivo da superficie

        Vertex_circulator vc(cdt.incident_vertices(eit)), Inicio(vc);

        unsigned int ContaVertices(0), ContaVerticesInseridos(0);
        double dx(0.0), dy(0.0), dx1(0.0), dy1(0.0);

        //--- Insere as arestas ligadas ao v�rtice

        do
        {
          //--- Erro na CGAL, ponto ligado a ele mesmo??
          dx1 = vc->point().x();
          dy1 = vc->point().y();


          dx = it->x - vc->point().x();
          dy = it->y - vc->point().y();

          if (cdt.is_infinite(vc) == false && (fabs(it->x - vc->point().x()) > 0.0001 || fabs(it->y - vc->point().y()) > 0.0001))
            ++ContaVertices;

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
              S = (Dy) / 2.0 * (Dx);                //--- Area de proje��o da reta 

            Azxy = AzTrigToTop(Azxy, Dx, Dy);             //--- Azimute  trigonometrico para topografico

            if ((fabs(Dy) > 0.0001 || fabs(Dx) > 0.0001))  //--- Erro na CGAL : pontos ligados a ele mesmos??
            {
              ittystPonto3D it(setPontos3D.find(CPoint3D(vc->point().x(), vc->point().y(), 0.00)));
              {
                //--- Se for�ar Delaunay ou Gabriel o ponto pode n�o existir ainda pois pode ter sido interpolado
                //--- Se Delaunay o ponto pode ter sido inserido na interse��o de alguma restri�ao 
                //--- Porisso o ponto pode ainda n�o existir no set, neste caso � preciso calcular a cota do ponto 

                if (it == setPontos3D.end())
                {
                  double CotaCalculada(CalculaCotaPonto(vc->point(), ForcarDelaunay || Gabriel ? cdtOriginal : cdt, setPontos3D, ListaPonteirosListaRestricoes, false));

                  if (CotaCalculada == DINFINITO)
                  {
                    strstrMsg.str("");
                    strstrMsg << "erro irrecuperavel - Contacte o suporte - VU&E005-SU - " << std::endl;
                    strstrMsg << vc->point() << std::ends;

                    AfxMessageBox(strstrMsg.str().c_str());
                    GravaLog(strstrMsg.str().c_str(), ArqLog);

                    CFile::Remove(strNome.c_str());

                    return false;
                  }

                  it = setPontos3D.insert(CPoint3D(vc->point().x(), vc->point().y(), CotaCalculada, DINFINITO, DINFINITO, DINFINITO, CPoint3D::PONTO_INSERIDO_DELAUNAY, " Ponto Inserido")).first;

                  //--- Pontos inseridos pelo usu�rio n�o fazem parte do grupo de pontos inseridos por Gabriel ou Delaunay

                  if (/*SetPontosInseridosUsuario.find(*it) != SetPontosInseridosUsuario.end() &&*/  SetItPontosInseridos.find(it) == SetItPontosInseridos.end())
                  {
                    SetItPontosInseridos.insert(it);

                    //                ArqPontosInseridos << ++ContaPontosInseridos << ' ' << it->y << ' ' << it->x << ' ' << it->z << " Ponto Inserido - " << (ForcarDelaunay ? "Delaunay " : Gabriel ? "Delaunay-Gabriel" : "Delaunay - Restri��es") << std::endl;
                  }
                }

                if (it != setPontos3D.end())
                {
                  ContaVerticesInseridos++;
                  CArcSuperficie << it->x << it->y << it->z << Azxy << S;     //--- Insere o ponto da face no arquivo da superf�cie
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

      //    ArqPontosInseridos << "//--- Fim do arquivo." << std::endl;

          //--- Insere os pontos inseridos nas restri��es: 
          //--- As restri��es de terreno s�o ignoradas pois j� est�o na superficie e se houver alguma altera��o ser� pouca, perto dos off-setr.
          //--- Os pontos inseridos s�o inseridos apenas na fronteira que � gravada no arquivo .bor

      InserePontosInseridosNasRestricoesTerr(ListaPonteirosListaRestricoes, SetItPontosInseridos, ArqLog, strNomeProjOri, &MMapTipoListaRestricao);
      //      GravaPontosEditadosUsuario(SetPontosInseridosUsuario, SetPontosDesabilitados, strNomeProj);   //--- Recalculou(ou n�o) a cota dos pontos inseridos Pelo usu�rio, regrava o arquivo                          
      LimpaMemoria(ListaPonteirosListaRestricoes);

      CArcSuperficie.Close();
      strMsg = "O projeto foi executado com exito.";
      pCSTexto->SetWindowText(strMsg.c_str());

      GravaLog(strMsg, ArqLog);
      GravaLog("===> Fim do c�lculo do projeto de terraplanagem.", ArqLog, true);

      return true;
    }
    else strMsg = "houve erro. O projeto n�o foi executado com exito.";
  }
  else strMsg = "Erro na abertura do arquivo de pontos (xyz)";

  pCSTexto->SetWindowText(strMsg.c_str());
  GravaLog(strMsg, ArqLog);

  GravaLog("===> Fim do Projeto de terraplanagem. <=====================", ArqLog, true);

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

  if (Vertice) Face = cdtOriginal.locate(Ponto, LT, LI);      //--- Procura como v�rtice
  else  Face = cdtOriginal.locate(Ponto);                  //--- Procura como ponto solto
  double Cota(DINFINITO);
  ittystPonto3D itP1(setP3D.end()), itP2(setP3D.end()), itP3(setP3D.end());

  //--- Quando insere por for�ar Delaunay ou Gabriel acha a face,
  //--- quando insere por Delaunay - interse��o, n�o acha a face, ent�o � preciso
  //--- procurar nas restri�oes

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
    else               //--- Se n�o achou a face o ponto pode ter sido inserido na interse��o de alguma restri��o
    {                  //--- (Delaunay - Interse��es) ent�o procura nas restri��es 
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

  //--- calcula a �rea do triangulo

  double S(((P3.y + P1.y) / 2.0 * (P3.x - P1.x)) +
    ((P2.y + P3.y) / 2.0 * (P2.x - P3.x)) +
    ((P1.y + P2.y) / 2.0 * (P1.x - P2.x)));

  return fabs(S) < 0.0005;
}

void InserePontosInseridosNasRestricoesTerr(tylstptylstPonto3D* ListaPonteirosListaRestricoes, tyitsetPonto3D& SetitPontosInseridos, std::ofstream& rArqLog, const std::string& strNomeProj, const tymmlstplstPonto3D* pMMapTipoListaRestricao)
{
  //--- Insere os pontos inseridos nas restri�oes nas listas de restri��es

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
            //  if(ForcarDelaunay || Gabriel) Achou = true;      //--- se for Delaunay puro o ponto pode ter sido inserido no cruzamento de 2 restri�oes
          }                                                    //--- tem que verificar em todas as restri��es

          itPRest1 = itPRest2;
        }
      }
    }
  }

  //--- Grava as restri��es com os novos pontos
  //--- Caso n�o tenha ocorrido pontos inseridos as restri�oes voltam a ser o que eram antes, sem os pontos inseridos

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
      strTexto += ".\n A superficie n�o foi gerada ou foi gerada com erros.";
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
      for (ittylpsetPontos3D itl = (ListaPonteirosListaRestricoes + C)->begin(); itl != (ListaPonteirosListaRestricoes + C)->end(); ++itl)    //--- Para todas as restri��es
      {
        unsigned int TipoRestricao(0);

        if (C == 0) TipoRestricao = PegaTipo(*itl, pMMapTipoListaRestricao);
        else TipoRestricao = 1;                                           //--- CAciTopografico::FRONTEIRA_TERRAP

        CArcRestricoes << TipoRestricao << (size_t)((*itl)->size());

        for (ittypstPonto3D it = (*itl)->begin(); it != (*itl)->end(); ++it)    //--- Grava todos os pontos da restri��o
        {
          CArcRestricoes << (*it)->CSNome << (*it)->CSDescricao << (*it)->x << (*it)->y << (*it)->z << (*it)->i << (*it)->fi << (*it)->S << false
            << ((*it)->CSDescricao.Find("Ponto Inserido") != -1) << int(0); //(*it)->i << (*it)->fi << (*it)->->S;
        }
      }
      //  CArcRestricoes << (double) 0.0;   //--- area
    }
  }
}

bool PontoARemover(const Point& PontoVerf, tylstcponto& LstPontosLimTerrpl, double Area)
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
  //--- Remove do terremo os pontos do terreno que est�o entro da terraplanagem

  if (FronteiraTerraplanagem.size() < 4) return false;

  if (*FronteiraTerraplanagem.begin() != *FronteiraTerraplanagem.rbegin()) return false;

  unsigned int Contador(0);
  int Area(1);               //--- A �rea � sempre maior que zero pois girou para a direita, fica aqui o lembrete
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

