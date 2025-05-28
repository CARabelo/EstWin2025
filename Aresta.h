#if !defined(AFX_ARESTA_H__ACBEF0EB_BF43_44A8_8BB1_6884A5BAE2A2__INCLUDED_)
#define AFX_ARESTA_H__ACBEF0EB_BF43_44A8_8BB1_6884A5BAE2A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Aresta.h : header file
//

class CDeltaSuperficie;

struct ltPontoZ                                           //--- Ordenação de pontos por cota, usado nas listas de CN
{
  bool operator()(const Ponto P1, const Ponto P2) const 
  { 
    return P1.z < P2.z;
  }
};

typedef std::set<Ponto,ltPontoZ> SSetzPontos;          //--- Set  de um conjunto de pontos, ordenapo por z
typedef SSetzPontos::iterator itSetzPontos;            
typedef SSetzPontos::const_iterator itcSetzPontos; 

/////////////////////////////////////////////////////////////////////////////
// CAresta window

class CAresta 
{

// Construction
public:

  SSetzPontos SetPontosCN;

  CDeltaSuperficie* pDeltaSuperficie;

  CAresta(CDeltaSuperficie* ppDeltaSuper);
  CAresta();
  bool InserePontoCN(const Ponto& P) { return (SetPontosCN.insert(P)).second ;};
  itSetzPontos BuscaPontoCN(double Cota) { SetPontosCN.find(Ponto(0.0,0.0,Cota)) ;};

// Attributes
public:

   bool Feito;

// Operations
public:

  operator=(const CAresta& RHS){};

  bool operator==(const CAresta& RHS) { return true;};


// Implementation
public:
  virtual ~CAresta() {};

	// Generated message map functions
protected:
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(AFX_ARESTA_H__ACBEF0EB_BF43_44A8_8BB1_6884A5BAE2A2__INCLUDED_)
