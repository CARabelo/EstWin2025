// MainFrm.h : interface of the CMainFrame class
// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////
#define QARQ 38

//--- Agora, (V4.02) podem estar na memória vários desenhos de greide de vários trechos, então é preciso separar o ponteiro das secões por trecho 

typedef std::pair<void*,void*> tyPairpVoid;

typedef std::map<std::string,tyPairpVoid> tyMapStrPairVoid;     //--- string trecho 1o void* ponteiro para as seções 2o void* ponteiro desenhos secoes
typedef tyMapStrPairVoid::iterator ittyMapStrPairVoid;

//--- Quando pression o fechar a aplicação [X] esta ocorrendo um pb no Close pois não consigui setar o foco ou gravar os dados das janelas abertas
//--- pois a navegação nao passa por todas as janelas e trava o programa quando envia set focos para uma delas (parece que ja foi destruida)
//--- Para resolver criei este std::map com os desenhos alterados para gravá-los no final

typedef std::map<std::string,void*> tyMapStrVoid;                //--- string trecho void* ponteiro para a Cwnd
typedef tyMapStrVoid::iterator ittyMapStrVoid;
typedef std::pair<std::string,void*> tyPairStrVoid;

class CMainFrame : public CMDIFrameWnd
{
  DECLARE_DYNAMIC(CMainFrame)
  void* MouseXY,     //--- Ponteiro para o dialogo do mouse, usado nos desenhos das seções.
      * MouseXYGre,  //--- Ponteiro para o diálogo do mouse, usado nos desenhos do greide.
      * MouseXYGeo;  //--- Ponteiro para o diálogo do mouse, usado nos desenhos geometricos.
  tyMapStrPairVoid MapSecoesTrecho;         //--- Ponteiro para as secoes, usado no desenho do greide, recebe as seções
                                            //--- que já estão construídas na criação dos desenhos das seções.   
  tyMapStrVoid MapDesenhosAlterados;        //--- Desenhos alterados por trecho
  CToolBar* mpCDesTool1,* mpCDesTool2,* mpCDesTool3,* mpCDesTool4,       //--- Ponteiros para os toolbars do desenho da secao
          * mpCDesToolGreide,*mpToolBarPonGeo,*mpCDesToolGeo,            //--- Quando desenhado com o greidde é preciso guardar estes ponteiros 
          * mpToolBoxDesenhar,*mpToolBarPerfGeol,*mpCToolBarTalvegues,   //--- para poder fazer os tollbars aparecem e desaparecerem
          * mpCToolBarEquiDrenagem;

  void* pDialogoInicialView;

  std::string NomeProjPagInicial;

                //--- Tool bar do desenho do greide  

  CMDIChildWnd* BuscaJanela(const CString& Titulo); 
  CDocument* GetMDIActiveDocument();

  bool Silencioso,GeometricoAlterado,GreideAlterado,SecaoAlterada,IniciarDesSuperf;
  UINT TimerChave;
  int ContaTentativas,ContaDias,D0,VerificarChave;

  int VerfInst(bool Inicial = false);
  int VerificaChaveBoole();
  std::string PegaStatus(const std::string& json);
  int PegaChave(std::string& Chave,std::string& pNumcliente);

public:

  CMainFrame();

  // Attributes
public:

  BOOL       Sincronizar,
             DesPontosGeoComSecao, 
             GabaritarProjeto,
             TrianImporTopografia;
  LPSTR      Ext[QARQ];
  UINT       CX[QARQ]; 
  UINT       IDDSubtit[QARQ];
  int        asdfg,
             TipoSuperficie,
             SuperfCalculada,
             ProjetoNovo;
  BOOL       GreideAtivo,    //--- TRUE quando o desenho do greide estiver ativo, serve para 
                             //--- sinalizar para a tarefa de secoes que ele esta em conjunto com
                             //--- o greide.					   
             GeometricoAtivo,
             NovoProj,
             FronteiraEditada,
             VeioPagInicial;
  bool EstadoLicenca;		
 // void* DesSecao;         //--- Ponteiro para o desenho das seções quando o desenho do perfil
                          //--- estiver ativo, os dois precisam conversar.
  void* DesPerfil;        //--- Ponteiro para o desenho das do perfil quando o desenho do perfil
                          //--- for fechado a janela ativa sera a da seção, usada para retornar o tasmanho e 
                          //--- a posicao da janela no fechamento (childfrm).
  void* DesPontosGeo;     //--- Idem
  DesCroquisView* pDesCroquisView2;
  int   QMedADesenhar;    //--- quantidade de mediçoes a desenhar simultaneamente 
//  void* DPonGeom ;        //--- ponteiro para o dialogo do ponto sob o mouse no desenho geometrico       

  BOOL DirectoryExists(LPCTSTR path);
  BOOL FileExists(LPCTSTR path);
  BOOL RemoveArqProjeto(std::string& Extensao);

  void* DialogoInicialView(int op=0,void* p=nullptr) 
  {
    if(op == 0) pDialogoInicialView = p;
    return pDialogoInicialView;
  };
  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CMainFrame)
public:
  virtual CFrameWnd* GetActiveFrame();
  virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
  //}}AFX_VIRTUAL
  // Implementation
public:

  int CasasDecimaisRelatorios;

  virtual ~CMainFrame();
  class CProjeto* AchaProjetoNoVetor();
  int InsLstProjetos(CString Nome);
  void AdProjrecentes(CString Nome); //---
  void* PegaJanMouse(){return MouseXY;};  //--- Retorna  o ponteiro para a janela do mouse
  bool VerfMedicao();
  void CalculouSuperficie(bool Estado) {CalculouSuperf = Estado;};
  bool CalculouSuperficie(){ return CalculouSuperf;};
  bool VerificaAberturaTarefa(CString& NomeProj);
  CToolBar* PegaBarra(int Num);    //--- Retorna o ToolBar correspondenta a Num   
  const char* PegaProjetoAtual();  //--- Retorna o projeto atual no combobox   
 // DesCroquisView* PegaDesenhoCroquis() { return pDesCroquisView; };
  DesCroquisView* PegaDesenhoCroquis() { return NULL; };

  const char * CMainFrame::PegaProjetoBase(char* pBuffer=NULL)
  {
	  static char Temp[1024] = { 0 };

	  m_ProjetosBox.GetLBText(m_ProjetosBox.GetCurSel(), pBuffer ? pBuffer : Temp);

	  //  if ((strstr(Temp,"MED") || strstr(Temp,"med")) && strlen(Temp) > 6)
	  //    *(Temp + (strlen(Temp) -6)) = 0;

	  return pBuffer ? pBuffer : Temp;
  }                 

  const char* PegaRuaAtual();      
  void RemoveRua(const std::string& Rua); 
  int SelecionaRua(const std::string& RuaSel) 
  { 
    return m_RuasProjeto.SetCurSel(m_RuasProjeto.FindStringExact(-1, RuaSel.c_str())); 
  }
  void TrocaRua(const std::string& Rua);
  void InsereSecoes(void* pSecoesAtuais,void* pDesSecoes) { MapSecoesTrecho.insert(std::pair<std::string,tyPairpVoid>(std::string(PegaRuaAtual()),tyPairpVoid(pSecoesAtuais,pDesSecoes)));}   //--- Se ja existir não insere, OK. 
  void* PegaSecoes() 
  {
     std::string ruaAtual(PegaRuaAtual());
     ittyMapStrPairVoid it(MapSecoesTrecho.find(std::string(PegaRuaAtual())));
     return  it == MapSecoesTrecho.end() ? NULL : it->second.first;
  } 
  
  void* PegaDesenhoSecoes() 
  {
     ittyMapStrPairVoid it(MapSecoesTrecho.find(std::string(PegaRuaAtual())));
     return  it == MapSecoesTrecho.end() ? NULL : it->second.second;
  }

  void AnulaSecao();
  void AnulaPerfil();

  void InsereDesenhoAlterado(std::string& strTrecho,std::string& strNomeDesenho,void* pCWnd)
  {
    MapDesenhosAlterados.insert(tyPairStrVoid(strTrecho+strNomeDesenho,pCWnd));
  }

  void RemoveDesenhoAlterado(const std::string& strTrecho,const std::string& strNomeDesenho)
  {
    MapDesenhosAlterados.erase((strTrecho+strNomeDesenho));
  }
  bool& IniciarDesSuperficie() { return IniciarDesSuperf;};
  std::string strArquivoRec;
  CComboBox m_SearchBox;
  CExComboBox m_ProjetosBox, m_RuasProjeto;
  CString m_sNomeComp,               //--- Usado para passar o nome para o construtor da MDIChildFrame
          NomeProjetoAtual,          //--- Contem o nme do projeto selecionado no combobox
          m_sMedicao,                //--- numero da medicao selecionada.
          m_sMedicaoAnterior,        //--- Núumero da medicao Anterior;
          DirRua,
          RuaAtual,
         *pListaArqRecentes;
  CDialog m_wndProgress;             //--- Dialogo de progresso  
  CString m_strArqAtivo;
  CDialog* DVolumes;          //--- Volumes da seção
  bool EMedicao,CalculouSuperf;          //--- True se o projeto atual for medicão 

#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

  CStatusBar m_wndStatusBar;
  CToolBar m_wndToolBar,m_wndToolBar2;
  CString m_strProjRec[51];   //--- Pilhas de projetos recentes
  UINT IDENTIF[6];           //--- IDs da pilha de projetos recentes
  int I,                     //--- Ponteiro da pilha. 
      IP;                    //--- Ponteiro para o projeto atual.

  void LeProjetosRecentes();
  void PoeTopo(int Num,CString Nome);
  void IncluiProjetoNoVetor(CString Nome);
  void ExcluiProjetoNoVetor(CString Projeto);
  void LeRuasProjeto();

  // Generated message map functions

protected:

  //{{AFX_MSG(CMainFrame)
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnProjAbrir();
  afx_msg void OnProjClose();
  afx_msg void OnConfigProj();
  afx_msg void OnUpdateProjClose(CCmdUI* pCmdUI);
  afx_msg void OnUpdateConfigProj(CCmdUI* pCmdUI);
  afx_msg void OnUpdateArqAbrir(CCmdUI* pCmdUI);
  afx_msg void OnUpdateArqNovo(CCmdUI* pCmdUI);
  afx_msg void OnProjrec1();
  afx_msg void OnProjrec2();
  afx_msg void OnProjrec3();
  afx_msg void OnProjrec4();
  afx_msg void OnProjrec5();
  afx_msg void OnProjrec6();
  afx_msg void OnDestroy();
  afx_msg void OnJanelaCentrar();
  afx_msg void OnUpdateJanelaCentrar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateProxJanela(CCmdUI* pCmdUI);
  afx_msg void OnProxJanela();
  afx_msg void OnUpdateCalcterreno(CCmdUI* pCmdUI);
  afx_msg void OnCalcterreno();
  afx_msg void OnMedioAbrir();
  afx_msg void OnMedioCalcularamedio();
  afx_msg void OnUpdateMedioCalcularamedio(CCmdUI* pCmdUI);
  afx_msg void OnUpdateMedioAbrir(CCmdUI* pCmdUI);
  afx_msg void OnMedioNova();
  afx_msg void OnUpdateMedioNova(CCmdUI* pCmdUI);
  afx_msg void OnCalculaNotaDeServico();
  afx_msg void OnSeccionar();
  afx_msg void OnUpdateClculosCalculatanotadeservio(CCmdUI* pCmdUI);
  afx_msg void OnCalculoVolumes();
  afx_msg void OnSuperfcieGerar();
  afx_msg void OnTerrenoSeccionar();
  afx_msg void OnCalcularGeometrico();
  afx_msg void nCalcSecoesAvulsas();
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnTerrenoAcidentestopogrficos();
  afx_msg void OnUpdateSuperfcieGerar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateTerrenoSeccionar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCalculoVolumes(CCmdUI* pCmdUI);
  afx_msg void OnUpdateClculosCalcularosdadosgeomtricos(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDesenhosSecoes(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEditarPontosGeo(CCmdUI* pCmdUI);
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
  afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
  afx_msg void OnClose();
  afx_msg void OnUpdateButFechar(CCmdUI* pCmdUI);
  afx_msg void OnDesenhosGerardxf();
  afx_msg void OnUpdateDesenhosGreide(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDesdxfprogeom(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDesdxfgreide(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDesdxfsecoes(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCalcAreaTaludes(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCalcLimpeza(CCmdUI* pCmdUI);
  afx_msg void OnUpdatePerfilGeol(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDXFProjGeom(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDEXGreide(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDXFSecoes(CCmdUI* pCmdUI);
  afx_msg void OnUpdateDXFPerfGeo(CCmdUI* pCmdUI);
  afx_msg void OnUpdateNSTerraplanagem(CCmdUI* pCmdUI);
  afx_msg void OnUpdateNSPavimentacao(CCmdUI* pCmdUI);
  afx_msg void OnUpdateAreaTaludes(CCmdUI* pCmdUI);
  afx_msg void OnUpdateVolumesTipoSolo(CCmdUI* pCmdUI);

  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnRelatNotadeserv();
  bool PegaSilencioso(){ return Silencioso;};
  void LigaSilencioso() { Silencioso = true;}; 
  void DesligaSilencioso() {Silencioso = false;};
  CString GetUserHomeDir();
  void ProjNovoCriaDir(const std::string& Dir);
  void MudouProjeto(std::string ProjetoAtual)
  {
    m_RuasProjeto.ResetContent();
    LeRuasProjeto();

    if (m_RuasProjeto.GetCount() == 0)
    {
      AfxMessageBox("Ainda não foram criados trechou ou ruas neste projeto, não há nada para ser feito ou visualizado nele. Crie uma rua ou um trecho para poder dar início a execução de um projeto.");
    }

    int Lixo(0);
    char Buffer[512] = { 0 };
    std::string NomeArquivo(PegaProjetoBase() + std::string(".ini"));

    ::GetPrivateProfileString(_T("DadosDesenhos"), _T("DadosDesPontosGeoGeoRef"), _T("0,0,0,0,0"), Buffer, 511, NomeArquivo.c_str());
    sscanf(Buffer, "%i,%i,%i,%i,%i", &Lixo, &Lixo, &TipoSuperficie, &Lixo, &Lixo);
  };

  void MudouRua(const std::string& RuaAtual);
  int CriaRua(const std::string& pDirNovaRua,const std::string& pNovaRua);
  void InsereRuaDesGeo(const std::string& RuaAtual);
  void InicializaCroquisDesGeo();
  CString* PegaProjRecentes() { return &m_strProjRec[0];}
  void RemoveProjRecente(int Ret);
  void AbreProjRec(int Num, CString* pNomeProj = nullptr);
  void AbreProj(const std::string& pNomeProj,bool AbrirDesSuperficie = false);
  void SalvaEstadoDiaInicial(WINDOWPLACEMENT* pwp, int NumTar);
  void LeEstadoDiaInicial(WINDOWPLACEMENT* pwp, int NumTar);
  void RemoveLista(int Num);
  void RemoveProjetosRecentes() 
  { 
    for (int i = 0; i < 50; i++) m_strProjRec[i].Empty();
  } //--- Limpa a pilha de PR.

  void GravaProjetosRecentes();

  afx_msg void OnUpdateRelatCurvasverticais(CCmdUI *pCmdUI);
  afx_msg void OnUpdateRelatProjetogeom(CCmdUI *pCmdUI);
  afx_msg void OnUpdateRelatProjetogeom33017(CCmdUI *pCmdUI);
  afx_msg void OnUpdateRelatTerrenoprimitivo(CCmdUI *pCmdUI);
  afx_msg void OnUpdateRelatVolumes(CCmdUI *pCmdUI);
  afx_msg void OnUpdateNotadeserviPavimenta(CCmdUI *pCmdUI);
  afx_msg void OnUpdateNotadeserviTerraplanagem(CCmdUI *pCmdUI);
  afx_msg void OnTerrenoGerarCn();
  afx_msg void OnUpdateTerrenoGerarCn(CCmdUI *pCmdUI);
  afx_msg void OnCalcSecTranversais();
  afx_msg void OnCalcSecAvulsas();
  afx_msg void OnCalcAreaTaludes();
  afx_msg void OnCalcAreaLimpeza();
  afx_msg void OnRelatAreaLimpeza();
  afx_msg void OnRelatVolTipoSolo();
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  afx_msg void OnFileSave();
  afx_msg void OnUpdateFileSave(CCmdUI *pCmdUI);
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnProjNovo();
  afx_msg void OnProjetoRemoverrua();
  afx_msg void OnUpdateProjetoInserirrua(CCmdUI *pCmdUI);
  afx_msg void OnUpdateProjetoRemoverrua(CCmdUI *pCmdUI);
  afx_msg void OnDesenhosPerfisAvulsos();
  afx_msg void OnMenuCalcProjTerrapl();
  afx_msg void OnUpdateMenuProjterrapl(CCmdUI *pCmdUI);
  afx_msg void OnUpdateDesdxfproterrap(CCmdUI *pCmdUI);
  afx_msg void OnUpdateRelProjTerraplanagem(CCmdUI *pCmdUI);
  afx_msg void OnImportarArqkml();
  afx_msg void OnUpdateImportarArquivokml(CCmdUI *pCmdUI);
  afx_msg void OnGeorefImportarPontosControle();
  afx_msg void OnGeorefImportarMapa();
  afx_msg void OnDronesImportarnuvemdepontos();
  afx_msg void OnDesinstalar();
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
  afx_msg void OnUpdateProjImparqtopog(CCmdUI* pCmdUI);
};

/////////////////////////////////////////////////////////////////////////////
