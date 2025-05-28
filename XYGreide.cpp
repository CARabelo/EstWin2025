// XYGreide.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "xygreide.h"
#include "math.h"
#include "estaca.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXYGreide dialog

CXYGreide::CXYGreide(CWnd* pParent)	: CDialog(CXYGreide::IDD, pParent),Geometrico(false),VUx(0.0),VUy(0.0),m_DOCoorX(0),m_DOCoorY(0),
PontosRotacionados(false)
{
  //{{AFX_DATA_INIT(CXYGreide)
  m_CSCoorX = _T("");
  m_CSCoorY = _T("");
  //}}AFX_DATA_INIT
}

void CXYGreide::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);

  if (!Geometrico)  
  {
    m_CSCoorX = CEstaca(m_DOCoorX);
    m_CSCoorY.Format("%8.3f",m_DOCoorY);
  }
  else 
  {
    if (PontosRotacionados)
    {
      //--- Desrotaciona o ponto para mostrar as coordenadas corretas.
      /*
      x’ = x . cos (fi)  -  y . sin(fi)

      y’ = x . sin (fi)  +  y . cos(fi)
      */

      double yTemp(m_DOCoorX), xTemp(m_DOCoorY);   

      m_CSCoorX.Format("%8.3f",xTemp * VUy - yTemp * VUx);
      m_CSCoorY.Format("%8.3f",xTemp * VUx + yTemp * VUy);
    }
    else
    {
      m_CSCoorX.Format("%8.3f",m_DOCoorY);
      m_CSCoorY.Format("%8.3f",m_DOCoorX);
    }
  }

  //{{AFX_DATA_MAP(CXYGreide)
  DDX_Text(pDX, IDC_COORX, m_CSCoorX);
  DDX_Text(pDX, IDC_COORY, m_CSCoorY);
  //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CXYGreide, CDialog)
  //{{AFX_MSG_MAP(CXYGreide)
  // NOTE: the ClassWizard will add message map macros here
  //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXYGreide message handlers

void CXYGreide::OnOK(){}    //--- Anula o OnOK

