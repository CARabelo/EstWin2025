#define _AFXDLL 
#include "stdafx.h"
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>

#include <cassert>
#include <iostream>
#include <fstream>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K> Vb;
typedef CGAL::Constrained_triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> TDS;
typedef CGAL::No_intersection_tag Itag; 
typedef CGAL::Constrained_Delaunay_triangulation_2<K,TDS,Itag> CDT;
typedef CDT::Point Point;
typedef CDT::Vertex_handle Vertex_handle;
typedef CDT::Vertex_circulator Vertex_circulator;

double M_PI_2 = 1.57079632679489661923;
double M_PI= 3.14159265358979323846;
double M_2_PI = 6.2831853071795864769;
double DINFINITO = 1.7976931348623158e+308;

struct stPoint3D
{
  double x,y,z;

  stPoint3D(double px,double py,double pz):x(px),y(py),z(pz) {};
};

struct ltPoint_3D_2D 
{ 
  bool operator()(const stPoint3D p1,const stPoint3D p2) const 
  { 
    double dx(p1.x - p2.x ),dy(p1.y - p2.y );

    return dx < -1E-4 ? true :  dx > 1e-4 ? false :
      dy < -1e-4 ? true : false;
  }
};

typedef std::set<stPoint3D,ltPoint_3D_2D>  tystPonto3D;
typedef tystPonto3D::iterator ittystPonto3D;

double AzTrigToTop(double Az,double Deltax,double Deltay)   //--- Passa o azimute de trigonométrico para topográfico (Est::mat.cpp)
{
  Az = M_PI_2 - abs(Az); 

  if(Deltax > 0.0)
  {
    if(Deltay > 0.0) return Az;
    else return M_PI - Az;
  }
  else
  {
    if(Deltay > 0.0) return M_2_PI - Az;
    else return  M_PI + Az;
  }
}

int CalcSuperficie(const std::string& strNomeProj,CProgressCtrl* pProgresso,CStatic* pCSTexto)
{
  CDT cdt;
  tystPonto3D setPontos3D;
  unsigned int ContaPontos(0);

  pCSTexto->SetWindowText("Triangulando a superfície...");

  for(int i = 0 ; i < 2 ; i++)
  {
    CFileException e;
    CFile Arq;

    std::string strNomeArq(strNomeProj + (i == 0 ? ".res" : ".bor"));
    unsigned int Flags(CFile::modeRead); 

    if (Arq.Open(strNomeArq.c_str(),Flags,&e) == 0)
    {
      if (e.m_cause != CFileException::fileNotFound) 
      {
        std::string strTexto("Erro ao abrir o arquivo.\n"); 
        strTexto += strNomeArq.c_str();
        strTexto += ".\n A superficie não foi gerada ou foi gerada com erros.";

        pCSTexto->SetWindowText(strTexto.c_str());

        return false;
      }
    }
    else
    {  
      CArchive CArcRestricoes(&Arq,CArchive::load);
      size_t QRestricoes(0);

      if (e.m_cause != CFileException::fileNotFound)   //--- Se Existe o arquivo
      {
        double AngRotacaoBordos;

        CArcRestricoes >> AngRotacaoBordos;

        for (CArcRestricoes >> QRestricoes ; QRestricoes ; QRestricoes--)    //--- Para todas as restrições
        {
          register size_t QPontos(0);
          double x1,y1,x2,y2,z,i,fi,S;
          CString CSDescricao;

          CArcRestricoes >> QPontos;

          CArcRestricoes >> CSDescricao >> x1 >> y1 >> z >> i >> fi >> S;
          setPontos3D.insert(stPoint3D(x1,y1,z));

          for(QPontos-- ; QPontos ; QPontos--)       //--- Para todos os pontos da restrição atual
          {
            CArcRestricoes >> CSDescricao >> x2 >> y2 >> z >> i >> fi >> S;  

            stPoint3D P3DAtual(x2,y2,z);

            if( 0 && setPontos3D.find(P3DAtual) != setPontos3D.end())   //=== Vamos deixar inserir e ver no que que dá
            {
              //Ponto de bordo ou restrição já existente
            }
            else 
            {
              setPontos3D.insert(P3DAtual);
              cdt.insert_constraint(Point(x1,y1),Point(x2,y2));

              x1 = x2;
              y1 = y2;
            }
          }      
        }
      }
    }
  }

  std::string strNomeArqEntrada(strNomeProj + std::string(".xyz"));

  std::ifstream ArqPontos(strNomeArqEntrada);

  if(ArqPontos.is_open())
  {
    while(ArqPontos.good())
    {
      std::string strBufferx,strBuffery,strBufferz;

      std::getline(ArqPontos,strBuffery,' ');   //--- Nome do ponto
      std::getline(ArqPontos,strBuffery,' ');   //--- y
      std::getline(ArqPontos,strBufferx,' ');   //--- x
      std::getline(ArqPontos,strBufferz,' ');   //--- z

      if(!strBuffery.empty() || !strBufferx.empty() || !strBufferz.empty())
      {   
        double x(atof(strBufferx.c_str())),y(atof(strBuffery.c_str())),z(atof(strBufferz.c_str()));

        stPoint3D P3DAtual(x,y,z);

        if(setPontos3D.find(P3DAtual) != setPontos3D.end())
        {
          //--- Ponto já existe não sera considerado
        }
        else
        {
          setPontos3D.insert(stPoint3D(x,y,z));
          cdt.insert(Point(x,y));
        }

        std::getline(ArqPontos,strBuffery);       //--- Resto da linha, descarta.

        if(pProgresso && (ContaPontos++ % 500) == 0) 
        {
          pProgresso->StepIt();

          if(pProgresso->GetPos() > 99) pProgresso->SetPos(0);
        }
      }
    }

    if(pProgresso) pProgresso->SetPos(100);

    if(ArqPontos.eof())
    { 
      if(cdt.is_valid())
      {        
        CFileException e;
        CFile ArqSuperficie;
        std::string strNome((strNomeProj + std::string(".sup")).c_str());

        if (ArqSuperficie.Open(strNome.c_str(),CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite,&e) == 0)
        {
          std::string strMsg(std::string("Erro ao abrir o arquivo de saida:\n") + strNome.c_str());

          return false;
        }

        CArchive CArcSuperficie(&ArqSuperficie,CArchive::store);

        //--- Salva o Angulo de rotacao inicial  e a quantidade de vertices na superficie

        size_t qtdVertices =  cdt.number_of_vertices();

        CArcSuperficie << (double) 0.0 << (size_t) cdt.number_of_vertices();

        for (CDT::All_vertices_iterator eit = ++cdt.all_vertices_begin(); eit != cdt.all_vertices_end() ; ++eit)
        {
          ittystPonto3D it(setPontos3D.find(stPoint3D(eit->point().x(),eit->point().y(),0.00)));

          if(it != setPontos3D.end())
          {
            CArcSuperficie << CString("") << it->x << it->y << it->z << DINFINITO << DINFINITO << DINFINITO;

            if(fabs(it->y - 9286359.915) < .001 && fabs(it->x -618697.825) < 0.001 )
            {
              double x = eit->point().x(),y=eit->point().y();

              int xx = 0;
            }

            std::list<Vertex_handle> lh;
            std::back_insert_iterator< std::list<Vertex_handle>  > back_it(lh);	
            Vertex_circulator vc(cdt.incident_vertices(eit)),Inicio(vc);

            unsigned int ContaVertices(0);

            do 
            { 
              //--- Erro na CGAL, ponto ligado a ele mesmo.

              if(cdt.is_infinite(vc) == false && (fabs(it->x - vc->point().x()) > 0.0001 || fabs(it->y - vc->point().y()) > 0.0001)) 
                ++ContaVertices;

            }while(++vc != Inicio); 

            CArcSuperficie << ContaVertices; 

            do 
            { 
              if(cdt.is_infinite(vc) == false)
              {
                //--- Calcula o azimute e a area da reta

                double Dy(vc->point().y() - eit->point().y()),
                       Dx(vc->point().x() - eit->point().x()),
                       Azxy(atan((Dy) / (Dx))),
                       S=(Dy) / 2.0 * (Dx);                //--- Area de projeção da reta 
                       Azxy = AzTrigToTop(Azxy,Dx,Dy);     //--- Azimute  trigonometrico para topografico

                if((fabs(Dy) > 0.0001 || fabs(Dx) > 0.0001))  //--- Erro na CGAL : pontos ligados a ele mesmos.
                { 
                  ittystPonto3D it(setPontos3D.find(stPoint3D(vc->point().x(),vc->point().y(),0.00)));

                  if(it == setPontos3D.end())
                  {
                    AfxMessageBox("erro irrecuperavel - Contacte o suporte - VU&E005-SU");

                    return false;
                  }

                  CArcSuperficie << it->x << it->y << it->z  << Azxy << S;
                  CArcSuperficie << (unsigned long) 0;     //--- Pontos interpolados
                }
              }
            }while(++vc != Inicio);
          }
          else
          {
            AfxMessageBox("erro irrecuperavel - Contacte o suporte - VU&E005-SU");

            return false;
          }
        }

        CArcSuperficie.Close();

        pCSTexto->SetWindowText("A superficie foi gerada com exito.");

        return true;
      }
      else pCSTexto->SetWindowText("A superficie não foi gerada com exito.");
    }
    else pCSTexto->SetWindowText("Erro na leitura do arquivo de pontos (xyz)");
  }
  else pCSTexto->SetWindowText("Erro na abertura do arquivo de pontos (xyz)");

  return false;
}


