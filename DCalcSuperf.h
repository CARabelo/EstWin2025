#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CDCalcSuperf dialog

class CDCalcSuperf : public CDialogEx
{
	DECLARE_DYNAMIC(CDCalcSuperf)

  CEvent Evento;

  bool ReadWindowPlacement();
  void WriteWindowPlacement();

  int* pTriangTopografia,TrImpTopog;
 
public:
	CDCalcSuperf(const CString& pCSProjeto,CWnd* pParent = NULL,int* pTrianTopografia = nullptr);   // standard constructor
	virtual ~CDCalcSuperf();

  int Terminou;

// Dialog Data
	enum { IDD = IDD_DIACALCSUPERF };

  CWinThread* T;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  CString CSProjeto;

	DECLARE_MESSAGE_MAP()
public:
  CProgressCtrl ProCalcSuperf;
  CStatic StaMsgCalcSuperf;
  afx_msg void OnBnClickedButcalcsuperf();
  CButton m_ButGerar;
  CButton m_ButPausar;
  CButton m_ButOK;
  CButton m_ButCancelar;
  double m_doEquidisCN;
  bool bDelaunay;
  bool bForcarDelaunay;
  bool bGabriel;
 
  
  afx_msg void OnBnClickedButpausar();
  afx_msg void OnBnClickedCancel();

  CEvent* PegaEventoFim() { return &Evento;};
  afx_msg void OnBnClickedRadio1();
  afx_msg void OnBnClickedCheforcardelaunay();
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedRadgabriel();
  afx_msg void OnBnClickedRaddelaunay();
  afx_msg void OnClickedButlog();
  afx_msg void OnBnClickedRadimptriatopog();
  afx_msg void OnBnClickedOK();
};
