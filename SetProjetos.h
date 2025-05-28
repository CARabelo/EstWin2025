#if !defined(AFX_SETPROJETOS_H__94743ED3_2E0D_11D7_84CC_0004ACE3FAF3__INCLUDED_)
#define AFX_SETPROJETOS_H__94743ED3_2E0D_11D7_84CC_0004ACE3FAF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetProjetos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetProjetos window
/*
   #include <string>
       #include <map>
       #include <iostream.h>

      typedef map<string, int, less<string> > months_type;

       // Print out a pair
      template <class First, class Second>
      ostream& operator<<(ostream& out,
                          const pair<First,Second> & p)
       {

                                    - 9 -       Formatted:  January 24, 2003

 map(3C++)                   Rogue Wave Software                   map(3C++)
 -                                                                         -

                                 20 Mar 1996

        cout << p.first << " has " << p.second << " days";
        return out;            
*/

//typedef map<CString, int> months_type;


//typedef map<CString,CListaSecoes> mapSecoes;

//typedef mapSecoes::iterator itmapSecoes;

class CSetProjetos : public CWnd
{
// Construction
public:
	CSetProjetos();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetProjetos)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSetProjetos();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSetProjetos)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPROJETOS_H__94743ED3_2E0D_11D7_84CC_0004ACE3FAF3__INCLUDED_)
