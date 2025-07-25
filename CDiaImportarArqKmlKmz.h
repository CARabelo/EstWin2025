#pragma once

// caixa de diálogo de CDiaImportarArqKmlKmz

class CDiaImportarArqKmlKmz : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaImportarArqKmlKmz)

  std::string NomeArq,NomeProjeto,NomeArqPonTerr;
  bool bPontosControle;
  tylstUtm_Coord_3d* pLstPontosControle;
  int TipoArqSaida;

public:
	CDiaImportarArqKmlKmz(CWnd* pParent = nullptr,const std::string& NomeProj = "",bool bPontosControle = false, tylstUtm_Coord_3d* LstPontosCtrl = nullptr,int TipoArqSaida=0);   // construtor padrão
	virtual ~CDiaImportarArqKmlKmz();
  bool PegaCotas(const std::string& json, ittylstUtm_Coord_3d& itCoordUTM);
  bool GravaArquivoXYZ(const tylstUtm_Coord_3d& LstCoorUTM);
  bool PegaStatus(const std::string& json);
  
// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAIMPORTARKMLKMZ };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  CString CEdCaminho,CEdCaminhoArqTerr;
  CProgressCtrl CProProgresso;
  afx_msg void OnBnClickedButtestar();
  afx_msg void OnBnClickedButpegararq();
  afx_msg void OnBnClickedButpegararqterr();
  CString Datum;
  CEdit StrMeridianoCentral;
  CEdit StrElipsoide;
  CString StrObs;
  CComboBox ComboDatum;
  virtual BOOL OnInitDialog();
  BOOL bSemCotas;
  afx_msg void OnRadsemcotas();
  CString StaObservacoes;
};
