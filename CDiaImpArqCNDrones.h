#pragma once


// caixa de diálogo de CDiaImpArqCNDrones

class CDiaImpArqCNDrones : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaImpArqCNDrones)

  std::string NomeArq, NomeArqCN, NomeProjeto, FormatoArqPontos;
  int TipoArquivo;

public:
	CDiaImpArqCNDrones(CWnd* pParent = nullptr,const std::string& NomeProjeto="",int pTipoArquivo=1);   // construtor padrão
	virtual ~CDiaImpArqCNDrones();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DIAARQCNDRONES};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV
  bool LeArqCN(std::string& NomArqAtual, tylstlstUtm_Coord_3d& ListaListaCoordGdc);

	DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnClickedButpegararqcn();
  CEdit CEDCaminhoArqCNDrones;
  CString CStrCaminhoArqCNDrones;
  CProgressCtrl CProgresso;
  CStatic CStaObs;
};
