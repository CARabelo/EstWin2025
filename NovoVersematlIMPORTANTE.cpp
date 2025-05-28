INICIO DO ARQUIVO
struct ComppEstaca
{    
  bool operator() (const std::string* p1,const std::string* p2) const    
  {     
    return p1->compare(*p2) < 0;      
  }
};

typedef std::map<const std::string*,CRegPIV*,ComppEstaca> tmapCRegPIV;typedef tmapCRegPIV::iterator ittmapCRegPIV;
typedef tmapCRegPIV::const_iterator cittmapCRegPIV;
typedef std::pair<const std::string*,CRegPIV*> tpairmapCRegPIV;
typedef std::pair<ittmapCRegPIV,bool> tpairtmapCRegPIVINS;
typedef std::list<ittmapCRegPIV> tlstittmapCRegPIV;
typedef tlstittmapCRegPIV::iterator ittlstittmapCRegPIV;
typedef tlstittmapCRegPIV::const_iterator ctlstittmapCRegPIV;


class CCurvasVerticais
{  
  tmapCRegPIV MapCurvasVerticais;  
  tlstittmapCRegPIV LstItMapCurvasVerticais;  
  bool CriarLista;    

public:    

  CCurvasVerticais(bool bCriarLista = false):CriarLista(bCriarLista) {};
  ~CCurvasVerticais(void);    
  const tpairtmapCRegPIVINS InserePIV(CRegPIV* pPIV);
  const cittmapCRegPIV PegaMaisProximo(const std::string* Estaca);
  const ittmapCRegPIV AlteraPIV(const CRegPIV& PIV);    
  bool RemovePIV(CRegPIV& PIV)    
  {      
    ittmapCRegPIV it(MapCurvasVerticais.find(PIV.PegapEstaca()));
    if(it != MapCurvasVerticais.end())       
    {         
      delete(it->second);
      MapCurvasVerticais.erase(it);
      return true;
    }            
    
    return false;    
  };
  
  CArchive& operator>>(CArchive& ar);    
  CArchive& operator<<(CArchive& ar);
};//=======================================================================================// 
CCurverticais.cpp
#include "StdAfx.h"
#include <map>
#include <list>
#include "CRegPIV.h"
#include "CCurvasVerticais.h"
#include <string>

CArchive& operator<<(CArchive& ar,std::string& str) 
{  
   ar << str.size();   

   for(register std::string::iterator it= str.begin() ; it != str.end() ; it++ ) ar << *it;   

   return ar;
}
CArchive& operator>>(CArchive& ar, std::string& str) 
{   
  size_t Tam;   

  ar >> Tam;   

  str.resize(Tam);    
  
  for(register std::string::iterator it= str.begin() ; it != str.end() ; it++ )  ar >> *it;   
  
  return ar;
}

const cittmapCRegPIV CCurvasVerticais::PegaMaisProximo(const std::string* pEstaca)    
{  
  if(MapCurvasVerticais.size() == 0) return MapCurvasVerticais.cend();  //--- tem q ter ao menos 1 elemento    
  
  cittmapCRegPIV citMapGE(MapCurvasVerticais.lower_bound(pEstaca)),   //--- aponta para o >=                
  citMapLT(citMapGE);  citMapLT--;    //--- aponta para o Anterior   
  
  if(citMapGE == MapCurvasVerticais.end()) return citMapLT;    //--- Não tem GE então retorna o anterior  
  if(citMapLT == MapCurvasVerticais.end()) return citMapGE;    //--- se não tiver anterior, retorna o posterior   
  if(*citMapGE->first == *pEstaca) return citMapGE;            //--- Igual  
  
  //--- Verifica a distancia ao posterior e ao anterior.  
  
  if(citMapGE->first < citMapLT->first) return citMapGE;  //-- se a do posterior for menor, retorna o posterior   
  
  return citMapLT;  //--- Se não retorna o anterior

}

const tpairtmapCRegPIVINS CCurvasVerticais::InserePIV(CRegPIV* pPIV)
{  
  tpairtmapCRegPIVINS itRes(MapCurvasVerticais.insert(tpairmapCRegPIV(pPIV->PegapEstaca(),pPIV)));  
  
  if(itRes.second == true && CriarLista == true) LstItMapCurvasVerticais.push_back(itRes.first);  
  
  return itRes;
}

const ittmapCRegPIV CCurvasVerticais::AlteraPIV(const CRegPIV& PIV)
{     
  ittmapCRegPIV it(MapCurvasVerticais.find(PIV.PegapEstaca()));  
  
  if(it != MapCurvasVerticais.end()) *it->second = PIV;    
  
  return it;

}

CArchive& CCurvasVerticais::operator<<(CArchive& ar)
{   
  ar >> CriarLista;   
  size_t C;    
  tpairmapCRegPIV PairMapCRegPIV;   
  
  for ( ar >> C ; C > 0 ; C--)   
  {    
    PairMapCRegPIV.second = new CRegPIV;    
    *PairMapCRegPIV.second << ar;
    /*     CRegPIV PIVTemp;    PIVTemp << ar;    MapCurvasVerticais.insert(tpairmapCRegPIV(PIVTemp.PegaEstaca(),PIVTemp));    */
    PairMapCRegPIV.first = (PairMapCRegPIV.second)->PegapEstaca();    
    MapCurvasVerticais.insert(PairMapCRegPIV);   
  }      
  return ar;
}

CArchive& CCurvasVerticais::operator>>(CArchive& ar)
{  
  ar << CriarLista << MapCurvasVerticais.size();  
  ittmapCRegPIV itMapPIV(MapCurvasVerticais.begin());     
  while(itMapPIV != MapCurvasVerticais.end())  
  {   
    *itMapPIV->second >> ar;   
    ++itMapPIV;  
  }    
  
  return ar;
}

CCurvasVerticais::~CCurvasVerticais(void)
{  
  ittmapCRegPIV itMapPIV(MapCurvasVerticais.begin());     
  
  while(itMapPIV != MapCurvasVerticais.end())  
  {   
    delete itMapPIV->second;   
    ++itMapPIV;  
  }

}//============================================================================================

void CTesteView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{   
  ClientToScreen(&point);   
  
  CCurvasVerticais CVs;   
  
  /*   CVs.InserePIV(CRegPIV("10+0.00","100.234","10","40"));   
       CVs.InserePIV(CRegPIV("20+0.00","105.578","35","10"));   
       CVs.InserePIV(CRegPIV("30+0.00","103.253","20","20"));   
       CVs.InserePIV(CRegPIV("40+0.00","107.380","30","15"));   
       CVs.InserePIV(CRegPIV("50+0.00","105.234","25","40"));      
       CRegPIV P1("35+0.00","107.276","15","20");   
       CVs.InserePIV(P1);   
       CVs.PegaMaisProximo(P1);      
       CFile FTemp(_T("Temp"),CFile::modeCreate|CFile::modeWrite);   
       CArchive ar(&FTemp,CArchive::store);    */   
  
  CFile FTemp(_T("Temp"),CFile::modeRead);   
  CArchive ar(&FTemp,CArchive::load);   
  CVs << ar;   
  const CRegPIV P1("35+0.00","107.276","15","20");   
  cittmapCRegPIV it(CVs.PegaMaisProximo(P1.PegapEstaca()));
  
  //   CVs >> ar;
}//=====================================================================================