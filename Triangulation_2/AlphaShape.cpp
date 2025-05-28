#define _AFXDLL 
#include "stdafx.h"
#define NOMINMAX 
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/algorithm.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Alpha_shape_2.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::FT FT;
typedef K::Point_2  Point;
typedef K::Segment_2  Segment;
typedef CGAL::Alpha_shape_vertex_base_2<K> Vb;
typedef CGAL::Alpha_shape_face_base_2<K>  Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds> Triangulation_2;
typedef CGAL::Alpha_shape_2<Triangulation_2>  Alpha_shape_2;
typedef Alpha_shape_2::Alpha_shape_edges_iterator Alpha_shape_edges_iterator;

#include <cassert>
#include <iostream>
#include <fstream>
#include <iomanip>  
#include <strstream>
#include "io.h"

extern double M_PI_2;
extern double M_PI;
extern double M_2_PI;
extern double DINFINITO;

typedef std::set<std::string> tysetNomeArquivos;
typedef std::set<std::string>::iterator tyditsetNomeArquivos;

struct stPoint3D
{
  enum ENUTIPOPONTO { PONTO_SUPERF, PONTO_BORDO, PONTO_RESTRICAO, PONTO_INSERIDO_DELAUNAY, PONTO_INSERIDO_GABRIEL, PONTO_TEMP };

  CString CSDescricao, CSNome;
  double x, y, z;
  int Tipo;

  stPoint3D(double px, double py, double pz = 0.0, int pTipo = PONTO_TEMP, const CString& pCstrDescricao = CString(""), const CString& pCstrNome = CString("")) :x(px), y(py), z(pz), Tipo(pTipo), CSDescricao(pCstrDescricao), CSNome(pCstrNome) {};
  stPoint3D(const Point& pP) :x(pP.x()), y(pP.y()), z(DINFINITO), Tipo(PONTO_TEMP) {};
  stPoint3D(const stPoint3D& pP) :x(pP.x), y(pP.y), z(pP.z), Tipo(pP.Tipo), CSDescricao(pP.CSDescricao), CSNome(pP.CSNome) {};

  inline stPoint3D operator-(const stPoint3D& RHS) const
  {
    return stPoint3D(x - RHS.x, y - RHS.y, z - RHS.z);
  }
};

typedef std::list<std::string> tylstNomeArquivos;                      //--- Lista de strings com os nomes dos arquivos de terreno.    
typedef tylstNomeArquivos::iterator tyitlstNomeArquivos;

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

typedef std::set<stmSegment, stltSegment> setmSegment;
typedef setmSegment::iterator itsetmSegment;
typedef std::list<itsetmSegment>  lstitpPoint;
typedef lstitpPoint::iterator itlstitpPoint;
typedef std::list<lstitpPoint> lstlstitpPoint;
typedef lstlstitpPoint::iterator itlstlstitpPoint;

struct stltCMyPoint         //--- Comparação em 2 dimensões
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

inline void PegaProximoAlfaNum(std::ifstream& Stream);
int GravaBordos(const std::string& strNomeProj, Alpha_shape_2& pA, std::ofstream& ArqLog, CStatic* pCSTexto, tySetPonSuperf& pSetCMyPoint);
inline void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora = false);
extern int PegaNomesArquivosTerreno(tylstNomeArquivos& lsttNomeArquivos, std::string strNomeProj,int TipoTriangulacao = -1);
extern inline void PulaBrancos(std::ifstream& Stream);
extern void GetLine(std::ifstream& ifs, std::string& Buffer, const std::string& strDelim);
extern int ENumero(std::string strDado);
extern void AcertaCamposLer(const std::string& Formato, std::string& Nome, std::string& x, std::string& y, std::string& z,std::string& Desc);
int LeArqCN_AS(const std::string& NomeProj, tySetPonSuperf& setPontos);

bool EST_MODO_DEBUG = false;

int CalcAlphaShape(const std::string& strNomeProj, CProgressCtrl* pProgresso, CStatic* pCSTexto, unsigned long DMP,const std::string& FormatoArqPontos,int Tipo)
{
  tySetPonSuperf SetCMyPoint,SetPontosDesabilitados;

  Alpha_shape_2 AlphaSape;
  std::string strMsg;

  std::ofstream ArqLog(strNomeProj + "_DefFronteira.log");

  if (ArqLog.good() == false)
  {
    if(pCSTexto) pCSTexto->SetWindowText("Erro ao abrir o arquivo de log. Programa abortado.");

    return false;
  }

  GravaLog("====> Inicio da definição da fronteira ", ArqLog, true);

  bool retorno(false);
  tylstNomeArquivos lstNomeArquivos;
  tyitlstNomeArquivos itNomeArqAtual(lstNomeArquivos.end());
  std::stringstream strstrMsg;

  strstrMsg << std::right << std::fixed << std::setfill(' ') << std::setprecision(3);

  if (Tipo == 1 || Tipo == 5 || PegaNomesArquivosTerreno(lstNomeArquivos, strNomeProj))
  {
    if (Tipo == 0 || Tipo == 2 || Tipo == 4)
    {
      static std::string strMarcasComentarios("/!@#$%&*");

      for (itNomeArqAtual = lstNomeArquivos.begin(); itNomeArqAtual != lstNomeArquivos.end(); ++itNomeArqAtual)
      {
        bool PontosDesabilitados(false), PontosEditados(false),PontosRemovidos(false);

        if (itNomeArqAtual->find("PontosInseridos") == std::string::npos || itNomeArqAtual->find("Secoes") != std::string::npos)
        {
          std::ifstream ArqPontos(*itNomeArqAtual);

          GravaLog("Lendo arquivo de pontos-> " + *itNomeArqAtual, ArqLog);

          if (itNomeArqAtual->find("PontosEditados") != std::string::npos)
          {
            PontosEditados = true;
          }

          if (ArqPontos.is_open())     //--- Insere os pontos da superfície
          {
            int ContaLinhas(0);
            unsigned int ContaPontos(0);
            while (ArqPontos.good())
            {
              std::string strBufferNome, strBufferx, strBuffery, strBufferz, strBufferObs;

              PulaBrancos(ArqPontos);                        //--- Pula os  brancos no início do arquivo
              GetLine(ArqPontos, strBufferNome, " \t\x0A");       //--- Nome do ponto, descarta

              if (strBufferNome.size() == 0)
              {
                std::getline(ArqPontos, strBufferNome);
                continue;
              }

              /*
              if(strBufferNome.find("Pontos") != std::string::npos)  //--- Pontos removidos
              {
                PontosRemovidos = true;
                std::getline(ArqPontos, strBufferNome);

                continue;
              }
              */

              if (strMarcasComentarios.find(*strBufferNome.begin()) != std::string::npos)
              {
                std::getline(ArqPontos, strBufferNome);       //--- Descarta o resto da linha,se size = 0 é porque e linha em branco

                if (strBufferNome.find("Pontos Desabilitados") != std::string::npos)
                {
                  PontosDesabilitados = true;
                }
                else
                {
                  if (strBufferNome.find("Pontos Removidos") != std::string::npos)
                  {
                   PontosRemovidos = true;
                  }
                  else
                  {
                    if (strBufferNome.find("Edição") != std::string::npos)   //--- nova edição de pontos, reinicia
                    {
                      PontosRemovidos = PontosDesabilitados = false;
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

              if (!strBuffery.empty() || !strBufferx.empty() || !strBufferz.empty())
              {
                if (FormatoArqPontos != "n N E C O") AcertaCamposLer(FormatoArqPontos, strBufferNome, strBuffery, strBufferx, strBufferz, strBufferObs);

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

                  if (z != DINFINITO)    //--- Pontos sem cota não podem fazer parte da fronteira
                  {
                    CMyPoint MyPoint(x, y, z);

                    if (PontosDesabilitados || PontosRemovidos)
                    {
                      SetPontosDesabilitados.insert(MyPoint);                     
                    }
                    else
                    {
                      SetCMyPoint.insert(MyPoint);                                  //--- Insere o ponto.
                    }

                    if (pProgresso && (ContaPontos++ % 500) == 0)
                    {
                      pProgresso->StepIt();

                      if (pProgresso->GetPos() > 99) pProgresso->SetPos(0);
                    }
                  }
                }
              }
            }

            //--- Inseriu todos os pontos da superficie.

            if (!ArqPontos.eof())
            {
              strstrMsg.seekg(0, strstrMsg.beg);
              strstrMsg << "Erro na leitura do arquivo de pontos (.xyz): " << itNomeArqAtual->c_str() << std::endl << std::ends;
              GravaLog(strstrMsg.str(), ArqLog);
            }
          }
        }
        else
        {
          strstrMsg.seekg(0, strstrMsg.beg);
          strstrMsg << "Erro na abertura do arquivo de pontos (.xyz): " << itNomeArqAtual->c_str() << std::endl << std::ends;
          GravaLog(strstrMsg.str(), ArqLog);
        }
      }
    }
    else
    {
      LeArqCN_AS(strNomeProj,SetCMyPoint);
    }

    //--- Remove os pontos desabilitados

    for (ittySetPonSuperf it = SetPontosDesabilitados.begin(); it != SetPontosDesabilitados.end(); ++it)
    {
      SetCMyPoint.erase(*it);
    }

    //--- Define a fronteira.

    Alpha_shape_2 AS(SetCMyPoint.begin(), SetCMyPoint.end(), /*FT(10000)*/DMP, Alpha_shape_2::GENERAL);

    if (AS.alpha_shape_edges_begin() != AS.alpha_shape_edges_end())
    {
      if (GravaBordos(strNomeProj, AS, ArqLog, pCSTexto, SetCMyPoint) != 0)
      {
        strMsg = "Houve ERRO, a fronteira NÃO foi gerada com exito.";
        retorno = false;
      }
      else
      {
        strMsg = "A fronteira foi gerada com exito.";
        retorno = true;
      }
    }
    else strMsg = "Não foram encontrados segmentos com estes parametros.\nA distancia média entre os pontos esta inapropriada para este conjunto de pontos.";
  }     
  else 
  {
  std::strstream strstrMsg2;
    strstrMsg2 << "Não há arquivos sw pontos (.xyz) no diretorio: " << strNomeProj.substr(0,strNomeProj.find_last_of('\\')) 
               << std::endl << "A fronteira não foi gerada." << std::ends;
    GravaLog(strstrMsg2.str(), ArqLog);

    strMsg = "Ou não há arquivos de terreno ou houve erro na abertura de um dos arquivos de pontos (.xyz)";
  }

  if(pCSTexto) pCSTexto->SetWindowText(strMsg.c_str());
  GravaLog(strMsg,ArqLog);
  
  GravaLog("====> Fim da definição da fronteira",ArqLog,true);
  
  return retorno;
}

inline void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora)
{
  CTime Agora(CTime::GetCurrentTime());
  static CTime GravAnterior(Agora);
  std::strstream strstreamTemp;

  strstreamTemp << std::right << std::fixed << std::setfill('0') << std::setprecision(2);

  if (ForcarHora || CTimeSpan((Agora - GravAnterior)).GetTotalSeconds() > 30L)
  {
    strstreamTemp << "====> " << Agora.GetDay() << '/' << Agora.GetMonth() << '/' << Agora.GetYear() << ' ' << Agora.GetHour() << ':' << Agora.GetMinute() << ':' << Agora.GetSecond() << ' ' << std::ends;
    Stream << strstreamTemp.str();
    GravAnterior = Agora;
  }

  Stream << strMsg.c_str() << std::endl;

  Stream.flush();
}

int GravaBordos(const std::string& strNomeProj, Alpha_shape_2& pAS, std::ofstream& ArqLog, CStatic* pCSTexto, tySetPonSuperf& pSetCMyPoint)
{
  std::string Texto("Gravando o arquivo de bordos...");
  if(pCSTexto) pCSTexto->SetWindowText(Texto.c_str());
  GravaLog(Texto.c_str(),ArqLog);

  size_t QRestricoes(0);
  register size_t QPontos(0);
  double x1(DINFINITO), y1(DINFINITO), x2(DINFINITO), y2(DINFINITO), z(DINFINITO), i(DINFINITO), fi(DINFINITO), S(DINFINITO);
  setmSegment SetmSegment;

  ArqLog << std::fixed << std::setprecision(5);
  unsigned long yy(0);

  for (register Alpha_shape_edges_iterator it = pAS.alpha_shape_edges_begin(); it != pAS.alpha_shape_edges_end(); ++it)
  {
    Segment SegAtual(pAS.segment(*it));
    SetmSegment.insert(stmSegment(SegAtual.point(0), SegAtual.point(1)));
    if (EST_MODO_DEBUG == true) ArqLog << pAS.segment(*it) << std::endl;
  }

  //--- Cria na memoria as listas com a fronteira, se a fronteira não estiver fechada pode existir mais de uma lista

  lstlstitpPoint LstLstpPointBordos;                 //--- Lista de lista de segmentos de bordos
  lstitpPoint LstpPointBordosAtual;                  //--- Lista de segmentos de bordo 

  LstLstpPointBordos.push_back(LstpPointBordosAtual);
  itlstlstitpPoint ItLstLstpPointBordosAtual(LstLstpPointBordos.begin());

  itsetmSegment itSetPrimeiroSegmento(SetmSegment.begin());
  register itsetmSegment itSetSegmentoAtual(itSetPrimeiroSegmento), itSetSegmentoAnterior(itSetPrimeiroSegmento);

  ItLstLstpPointBordosAtual->push_back(itSetSegmentoAtual);    //--- Primeiro segmento
  unsigned long ContaSeg(0);

  do
  {
    ++ContaSeg;
    itSetSegmentoAnterior = itSetSegmentoAtual;
    if ((itSetSegmentoAtual = SetmSegment.find(stmSegment((itSetSegmentoAtual)->P2, (itSetSegmentoAtual)->P1))) != SetmSegment.end())
    {
      ItLstLstpPointBordosAtual->push_back(itSetSegmentoAtual);   //--- Mas  um segmento nesta seção
    }
    else    //--- Senão o conjunto de segmentos está quebrado (a fronteira não está fechada pois tem mais de uma lista) 
    {
      itSetSegmentoAtual = ++itSetSegmentoAnterior;
      if (itSetSegmentoAtual != SetmSegment.end())      //--- Se não acabaram os segmentos
      {
        //--- Inicia outro conjunto de segmentos (outra lista) 

        LstpPointBordosAtual.clear();
        LstLstpPointBordos.push_front(LstpPointBordosAtual);
        ItLstLstpPointBordosAtual = LstLstpPointBordos.begin();
        ItLstLstpPointBordosAtual->push_back(itSetSegmentoAtual);
      }
    }
  } while (ContaSeg != SetmSegment.size() && itSetSegmentoAtual != SetmSegment.end() && itSetSegmentoAtual != itSetPrimeiroSegmento);


  if (LstLstpPointBordos.size() > 200)
  {
    if (pCSTexto)
    {
      std::stringstream strstrMsg;

      strstrMsg << "A fronteira está muito fragmentada, está com " << LstLstpPointBordos.size() << " fragmentos. Este valor não pode ultrapassar a 200. "
                << "A distância média entre os pontos não está apropriada para este conjunto de pontos. "
                << "Aumente a distância média entre os pontos e calcule a fronteira novamente. A fronteira final deve ser uniforme possuir apenas 1 fragmento.";

      AfxMessageBox(strstrMsg.str().c_str());
      GravaLog(strstrMsg.str(), ArqLog);
    }

    return 3;
  }

  //--- Neste pontos LstLstpPointBordos contem todos os conjuntos de segmentos, 1 só se a fronteira não estiver quebrada
  //--- Grava o arquivo de bordos

  CFileException e;
  CFile Arq;

  std::string strNomeArq(strNomeProj + ".bor");
  unsigned int Flags(CFile::modeCreate | CFile::modeWrite);

  if (Arq.Open(strNomeArq.c_str(), Flags, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      std::string strTexto("Erro ao abrir o arquivo.\n");
      strTexto += strNomeArq.c_str();
      strTexto += ".\n A fronteira não foi gerada ou foi gerada com erros.";
      GravaLog(strTexto, ArqLog);

      if(pCSTexto) pCSTexto->SetWindowText(strTexto.c_str());

      return 2;
    }
  }
  else
  {
    //--- Remove as listas com menos de 3 pontos

    itlstlstitpPoint it(LstLstpPointBordos.begin());

    while (it != LstLstpPointBordos.end())
    {
      if (it->size() < 3) LstLstpPointBordos.erase(it++);
      else it++;
    }

    //--- Grava o arquivo.

    size_t QRestricoes(0);
    register size_t QPontos(0);
    double x1(DINFINITO), y1(DINFINITO), x2(DINFINITO), y2(DINFINITO), z(DINFINITO), i(DINFINITO), fi(DINFINITO), S(DINFINITO);
    CArchive CArcRestricoes(&Arq, CArchive::store);
    unsigned int PontosRepetidos(0);

    CArcRestricoes << (double) 0.0 << (size_t)LstLstpPointBordos.size();

    for (ItLstLstpPointBordosAtual = LstLstpPointBordos.begin(); ItLstLstpPointBordosAtual != LstLstpPointBordos.end(); ++ItLstLstpPointBordosAtual)
    {
      CArcRestricoes << (unsigned int) 17;                                //--- CAciTopografico::FRONTEIRA
      CArcRestricoes.Flush();

      ULONGLONG PosQtdReg(Arq.GetPosition());             //--- Se houver pontos repetidos vao precisar regravar a qtd de pontos

      CArcRestricoes << (size_t)(ItLstLstpPointBordosAtual->size());

      ittySetPonSuperf itSetPonSuperf(pSetCMyPoint.find(CMyPoint((*(ItLstLstpPointBordosAtual->begin()))->P1.x(), (*(ItLstLstpPointBordosAtual->begin()))->P1.y(), 0.0)));

      if (itSetPonSuperf != pSetCMyPoint.end())
      {
        PontosRepetidos = 0;

        CArcRestricoes << CString("") << CString("") << itSetPonSuperf->x() << itSetPonSuperf->y() << itSetPonSuperf->z << 0.0 << 0.0 << 0.0 << true << (int)0 << true << (int)1;     //--- 1 = Restricao

        for (itlstitpPoint itlstitpPointAtual = ++ItLstLstpPointBordosAtual->begin(); itlstitpPointAtual != ItLstLstpPointBordosAtual->end(); ++itlstitpPointAtual)
        {
          itSetPonSuperf = pSetCMyPoint.find(CMyPoint((*itlstitpPointAtual)->P1.x(), ((*itlstitpPointAtual))->P1.y(), 0.0));

          if (itSetPonSuperf != pSetCMyPoint.end())
          {
            CArcRestricoes << CString("") << CString("") << itSetPonSuperf->x() << itSetPonSuperf->y() << itSetPonSuperf->z << 0.0 << 0.0 << 0.0 << true << (int)0 << true << (int)0;
          }
          else
          {
           /// std::string StrMsg("Erro irrecuperavel, contacte o suporte. VU&E_AS001");
            std::string StrMsg("Ponto Existente, será ignorado.");
            GravaLog(StrMsg, ArqLog, true);
            ++PontosRepetidos;
          }
        }

        if (PontosRepetidos != 0)
        {
          CArcRestricoes.Flush();
          Arq.Seek(PosQtdReg, CFile::begin);
          CArcRestricoes << (size_t) (ItLstLstpPointBordosAtual->size() - PontosRepetidos);
          CArcRestricoes.Flush();

          Arq.SeekToEnd();
        }
      }
      else
      {
        if (pCSTexto)
        {
          std::string StrMsg("Erro irrecuperavel, contacte o suporte. VU&E_AS002");
          pCSTexto->SetWindowText(StrMsg.c_str());
          GravaLog(StrMsg, ArqLog, true);
        }

        CFile::Remove(strNomeArq.c_str());

        return 1;
      }
    }

    CArcRestricoes << (double) 0.0;      //--- Área 
  }

  Arq.Close();

  return 0;
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

#include "..\CPonto.h"

int LeArqCN_AS(const std::string& NomeProj, tySetPonSuperf& setPontos)
{
  CFile Arq;
  CFileException e;

  CString NomeCNDrone(NomeProj.c_str() + CString(".cnd"));

  if (Arq.Open(NomeCNDrone, CFile::modeRead, &e) == 0)
  {
    if (e.m_cause != CFileException::fileNotFound)
    {
      return false;
    }
  }

  CArchive CArcCurvasNivel(&Arq, CArchive::load);
  size_t QtdCurvasNivel(0), QtdPontosCNAtual(0);

  if (e.m_cause != CFileException::fileNotFound)
  {
    double EquidisCN, AngRotacaoBordos;

    CArcCurvasNivel >> AngRotacaoBordos >> EquidisCN;

    for (CArcCurvasNivel >> QtdCurvasNivel; QtdCurvasNivel; QtdCurvasNivel--)
    {
      Ponto PAtual;

      for (CArcCurvasNivel >> QtdPontosCNAtual; QtdPontosCNAtual; QtdPontosCNAtual--)
      {
        PAtual << CArcCurvasNivel;

        setPontos.emplace(CMyPoint(PAtual.x, PAtual.y, PAtual.z));
      }
    }
  }

  return true;
}