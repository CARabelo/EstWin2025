#pragma once
#include "afxdialogex.h"


// caixa de diálogo de CDImportarArqSIG

class CDImportarArqSIG : public CDialogEx
{
	DECLARE_DYNAMIC(CDImportarArqSIG)

	std::string NomeArqCN,NomeArqMapa,NomeProjeto;
	std::string& strArqCN,&strArqMapa,&strArqPCTRL;
	std::string FormatoArqPontos;
	double& EquidisCN;

	int ErroImportacao;

	LLPontos CurvasNivel;

	void* pLstPontosCTRL;

public:

	enum ERROS { SEM_ERROS, CN_SEM_COTA };

	CDImportarArqSIG(std::string& pArqCN,std::string& pArqTalvegues,std::string& pArqMapa,std::string& NomeArqPCTRL,std::string& pNomeProjetoBasico, double& pEquidisCN,void* pLstPontosGeoRef,CWnd* pParent = nullptr);
	virtual ~CDImportarArqSIG();

	bool ImportaCNsSIG(const std::string& NomeArquivo, const std::string& strEntity);

	LLPontos& PegaCurvasNivel() {return CurvasNivel;}
	int PegaErro() {return ErroImportacao;};

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAIMPARQSIG };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:

	CString CSArqCN;
	CString CSArqMapa;
	CString CSArqPontosControle;

	afx_msg void OnBnClickedButpegararqcn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButpegarmapasig();
	afx_msg void OnBnClickedButpegarpontoscontrole();

	void GravaArqCNs();
	bool LeArqPCTRL(std::string& NomArq,void* ListaCoordUTM);
};
