// arqnovo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// arqnovo dialog

class arqnovo : public CDialog
{
  // Construction
public:
  arqnovo(CWnd* pParent = NULL);   // standard constructor
  CString Dialoga(int tipo,CString nome);
  CString  Nome;

  // Dialog Data
  //{{AFX_DATA(arqnovo)
  enum { IDD = IDD_ARQNOVO };
  // NOTE: the ClassWizard will add data members here
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(arqnovo)
public:
  virtual INT_PTR DoModal();
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:
  enum {EXT,NOME};
  enum {ARQNOVO,ARQABRIR};

  int      Tipo,NARQ;

  struct arqu
  {
    char *ext[2];
    int  achou;
  }arquivos[22];

  int  PreencheLista();

  // Generated message map functions
  //{{AFX_MSG(arqnovo)
  virtual BOOL OnInitDialog();
  virtual void OnOK();
  afx_msg void OnDblclkArquivos();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};
