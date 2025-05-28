// xyMouse.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// xyMouse dialog

class xyMouse : public CDialog
{
  // Construction
public:
  xyMouse(CWnd* pParent = NULL);   // standard constructor
  double m_DOCoorX, m_DOCoorY;     //--- Usada pelos recipientes para armazenar as coordenadas.

  // Dialog Data
  //{{AFX_DATA(xyMouse)
  enum { IDD = IDD_DESGREIDETOOL };
  CButton	m_RATerreno;
  CButton	m_RAProjeto;
  CButton	m_RAMedicao;

  CString m_EDCoorX;
  CString m_EDCoorY;
  CString	m_STSecao;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(xyMouse)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(xyMouse)
  virtual void OnOK();
  afx_msg void OnRaterreno();
  afx_msg void OnRaprojeto();
  afx_msg void OnRamedicao();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
