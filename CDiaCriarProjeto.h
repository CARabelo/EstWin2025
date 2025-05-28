#pragma once


// CDiaCriarProjeto dialog

class CDiaCriarProjeto : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaCriarProjeto)

  int ProjAcad;

public:
  
  bool OK;

	CDiaCriarProjeto(CWnd* pParent,const CString& CStrDir,int pProjACAD=false);   // standard constructor
	virtual ~CDiaCriarProjeto();
  int GravaProjeto();
  
// Dialog Data
	enum { IDD = IDD_DIANOVOPROJETO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
  CString CStrNomeProjeto;
  CString CStrDiretorio;
  afx_msg void OnClickedButdiretorio();
  afx_msg void OnBnClickedOk();
};
