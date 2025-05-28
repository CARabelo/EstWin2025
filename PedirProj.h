// PedirProj.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PedirProj dialog

class PedirProj : public CDialog
{
  // Construction
public:
  PedirProj(CWnd* pParent = NULL);   // standard constructor

  CString Projeto;    //--- Nome do projeto digitado.

  // Dialog Data
  //{{AFX_DATA(PedirProj)
  enum { IDD = IDD_PROJETO };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(PedirProj)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(PedirProj)
  virtual void OnOK();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
