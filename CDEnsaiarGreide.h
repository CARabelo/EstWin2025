#pragma once
#include "afxdialogex.h"

// caixa de diálogo de CDEnsaiarGreide

class CDEnsaiarGreide : public CDialogEx
{
	DECLARE_DYNAMIC(CDEnsaiarGreide)

	int& Metodo;
	double& Parametro1,&Parametro2;
	std::string NomeProjeto;
	double ParamAtuais[8][2];

	public:

  enum Ensaios{ENESIMO_PONTO,DISTANCIA_RADIAL,DISTANCIA_PERPENDICULAR,REUMANN_WITKAM,OPHEIM,
	             LANG,DOUGLAS_PEUCKER,DOUGLAS_PEUCKER_VARIANTE,CARLOS_RABELO,TERMINAR_ENSAIOS,REMOVER_ENSAIO_ATUAL,
							 ADOTAR_ENSAIO_ATUAL};

	CDEnsaiarGreide(CWnd* pParent,std::string& pNomeProj, int& pMetodo, double& pParam1,double& pParam2);
	virtual ~CDEnsaiarGreide();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAENSAIARGREIDE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CComboBox EnsaiarGreide;
	CString cstParametro1,cstParametro2;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButexecutar();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeCommetodo();
	afx_msg void OnBnClickedButpadrao();
	afx_msg void OnBnClickedButadotar();
	afx_msg void OnBnClickedButremover();
};
