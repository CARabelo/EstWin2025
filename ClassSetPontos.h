#if !defined(AFX_CCSETPONTOS_H__86D4FD83_1AA2_11D3_810A_00104BC6689A__INCLUDED_)
#define AFX_CCSETPONTOS_H__86D4FD83_1AA2_11D3_810A_00104BC6689A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSetPontos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetPontos window

class CSetPontos 
{
  std::set<Ponto> Pontos;

  // Construction
public:
  CSetPontos();

  Ponto* AchaPonto(Ponto& PPonto);
  Ponto* IncluiPonto(Ponto& PPonto);

  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CCSStack)
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CSetPontos();

  // Generated message map functions
protected:
  //{{AFX_MSG(CCSStack)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CCSETPONTOS_H__86D4FD83_1AA2_11D3_810A_00104BC6689A__INCLUDED_)
