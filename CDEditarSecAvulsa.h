#pragma once


// caixa de diálogo de CDEditarSecAvulsa

class CDEditarSecAvulsa : public CDialogEx
{
	DECLARE_DYNAMIC(CDEditarSecAvulsa)
  Ponto& PIni, &PFim;

public:
	CDEditarSecAvulsa(CWnd* pParent,Ponto& PInicial,Ponto& PFinal);  
	virtual ~CDEditarSecAvulsa();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAEDITARPONTOSSECAV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  double P1Norte;
  double P1Este;
  double P2Norte;
  double P2Este;
  virtual void OnOK();
};
