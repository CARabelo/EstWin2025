// est.h : main header file for the EST application
//

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#define QUANTIDADE_TAREFAS 24

/////////////////////////////////////////////////////////////////////////////
// CEstApp:
// See est.cpp for the implementation of this class
//

class CEstApp : public CWinApp
{
  void SalvaListaRuas();
  void* pMainFrame;

  std::string NomeArqMapaGIS,NomeArqPCTRLGIS;
  double EquidisCNQGIS;

  int ErroImportacaoArquivos;
  
 public:

  const std::string& PegaNomeArqPCTRLQGIS() {return NomeArqPCTRLGIS;}
  const std::string& PegaNomeArqMapaQGIS() { return NomeArqMapaGIS;}
  double PegaEquidisCNQGIS() { return EquidisCNQGIS;};

  static std::list<std::string> LstRuas;

  virtual BOOL PreTranslateMessage(MSG* pMsg);
  CEstApp();
  void ChamaHelp(std::string Item = "");

  void AbreArqCriaDoc(char* pNomeArq ,bool Remover=true,bool ELog=false);
  static void LeListaRuas();

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CEstApp)
public:

  void* pDiaProgresso;
  tlstwstr CamadasAcad;

  virtual BOOL InitInstance();
  ~CEstApp();
  void MudouProjetoSelecionado(std::string& NovoProjeto);
  void MudouRuaSelecionada(std::string& NovaRua);
  void GeraSuperficie(void* pTriangulacao, void* pCurvasNivel,const std::string& NomeArquivo,const std::string& NomeArqAciTopog);
  void GeraSuperficieSIG(void* pLLPontosCN);
  void DialogoInicial(bool AbrirProjetoExistente = false);
      
  //}}AFX_VIRTUAL

  // Implementation

  //{{AFX_MSG(CEstApp)
  afx_msg void OnAppAbout();
  afx_msg void OnArqAbrir();
  afx_msg void OnArqNovo();
  afx_msg void OnFileMruFile1();
  afx_msg void OnFileMruFile2();
  afx_msg void OnFileMruFile3();
  afx_msg void OnFileMruFile4();
  afx_msg void OnFileMruFile5();
  afx_msg void OnFileMruFile7();
  afx_msg void OnFileMruFile8();
  afx_msg void OnFileMruFile10();
  afx_msg void OnFileMruFile9();
  afx_msg void OnFileMruFile6();
  afx_msg void OnProcurar();
  afx_msg void OnTestarclassesDesenhodassees();
  afx_msg void OnTestardesenhoperfil();
  afx_msg void OnDesenhosGreide();
  afx_msg void OnDesenhosSecoes();
  afx_msg void OnMedirConfigurar();
  afx_msg void OnEditarPontosGeo();
  afx_msg void OnDesenhosGerardxf();
  afx_msg void OnDesdxfprogeom();
  afx_msg void OnDesdxfgreide();
  afx_msg void OnDesdxfsecoes();
  afx_msg void OnDesdxfperfgeol();
  afx_msg void OnDesdxfprojterrap();
  afx_msg void OnDesdxfBaciasHidrog();

  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  void AbrirArquivoMRU(int Num); 
  UINT PegaRecurso(int NumArq);
public:
  afx_msg void OnRelatNotadeservi();
  afx_msg void OnRelatProjetogeom();
  afx_msg void OnRelatProjetogeomdet();
  afx_msg void OnRelatCurvasverticais();
  afx_msg void OnNotadeserviTerraplanagem();
  afx_msg void OnNotadeserviPavimenta();
  afx_msg void OnRelatTerrenoprimitivo();
  afx_msg void OnFilePrint();
  afx_msg void OnRelatVolumes();
  afx_msg void OnRelatSecavulsas();
  afx_msg void OnRelatAreaTaludes();
  afx_msg void OnRelatAreaLimpeza();
  afx_msg void OnRelatVolTipoSolo();
  afx_msg void OnAjuda();
  afx_msg void OnHelp();
  afx_msg void OnEditarPerfilGeologico();
  afx_msg void OnProjetoInserirRua();
  virtual BOOL SaveAllModified();
  afx_msg void OnDesenhosPerfisAvulsos();
  afx_msg void OnRelatProjTerraplanagem();
  afx_msg void OnRelatBaciasHidrograf();
  afx_msg void OnRelatEquipDrenagem();
  afx_msg void OnImpArqTopografia();
  afx_msg void OnImpArqSIG();

  void AbrirArquivo(int Tipo);
  void ImportaArqTopografia(const std::string& NomeArquivo, const std::string& NomeArqAciTopog, const std::string& pCNMestra, const std::string& pCNInterm);
  void ImportaArqSIG(std::string& pArqCN,std::string& pArqTalvegues,std::string& pArqPCTRL,std::string& pArqMapa);
  void AbreProjExistente(int Num, const std::string& NomeProjeto,CView* pDiaInicial,bool IniciarDessuperf=false);
};


