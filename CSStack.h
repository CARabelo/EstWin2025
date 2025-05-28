#if !defined(AFX_CSSTACK_H__86D4FD83_1AA2_11D3_810A_00104BC6689A__INCLUDED_)
#define AFX_CSSTACK_H__86D4FD83_1AA2_11D3_810A_00104BC6689A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CSStack.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCSStack window

class CCSStack 
{
  std::vector<CProjeto> Projetos;

  // Construction
public:
  CCSStack();

  CProjeto* AchaProjeto(CString& Projeto);
  void ExcluiProjeto(CString& Projeto);
  void IncluiProjeto(CString& Nome);

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
  virtual ~CCSStack();

  // Generated message map functions
protected:
  //{{AFX_MSG(CCSStack)
  // NOTE - the ClassWizard will add and remove member functions here.
  //}}AFX_MSG
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSSTACK_H__86D4FD83_1AA2_11D3_810A_00104BC6689A__INCLUDED_)
