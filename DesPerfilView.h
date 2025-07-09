// DesPerfilView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DesPerfilView view

class DesPerfilView : public CView
{
  enum TIPOSPERFIS{PROJETO,TERRENO,PAVIMENTO,MEDICAO,TERRAPLANAGEM,LIMPEZA,TOTALPENAS,PONTOPERFILSONDAGEM,PONTOOAC,PONTOOAEINICIAL,PONTOOAEFINAL};

  LPCTSTR CursorAtual;

  CCurVerticais CurvasVerticais,      //--- Dados do Greide de projeto.  
                CurvasVerAnteriores;  //--- Greide Salvo, para restaurar
  CListaMedicoes& ListaMedicoes;
  COAC  OACs;
  COAE  OAEs;
  CPerfilGeologico PerfilGeologico;
  tysetCTipoSolo SetTiposSolos;

  CHPavimento HPavimento,HLimpeza;
  DesSecaoView& DesenhoSecao;     //--- Referencia para o desenho das secoes. para sincronismo

  typairittyPerfilTSrsetPTS pairResp;   //--- Resposta para o verfponto no perfil geológico

  std::string Trecho,TrechoAnterior,Projeto;

  tylstpontos GreideSimplificado;

  CXYGreide* MouseXYGre;     //--- Ponteiro para o diálogo das coordenadas do mouse
  CListaSecoes& Secoes;      //--- Referencia às Secoes, para pegar as cotas dos eixos.  
  double Escala[2],          //--- Escala em pixels por metro; 
         EscalaDefault[2],   //--- Escalas padrão que permitem a visualização de todo o desenho.
         Deltas[2],          //--- Deslocamento nos eixos.
         AlturaVideo,        //--- Altura do video em metros.
         LarguraVideo,       //--- Largura do vídeo em metros. (Eixo x)
         Resolucao[2],       //--- Resolucao do vídeo em pixels.
         DeltaYMetro,        //--- Idem, para facilitar os cálculos.
         Toleranciax,        //--- Tolerancias para procurar o ponto na posição 
         Toleranciay,        //--- Atual do mouse.
         Toleranciaxy_2,     //--- tolerancias em perfis geologicos (não ficou bom, remover)
         RazaoHV,            //--- Razão entre as escalas H e V.
         LimiteEsq,          //--- Limites do terreno
         LimiteDir,
         LimiteSup,
         LimiteInf,
         ParametroEnsaio1,
         ParametroEnsaio2;
  CPoint InicioArrast,  //--- Usados nas operações de arrastar.
         InicioZoom,
         FimZoom,
         PonMousePixelAtual,   //--- Ponto atual do mouse
         FimArrast;            //--- Idem.
  bool Arrastando,        //--- TRUE quando estiver attastando.
       ArrastandoPonto,      //--- TRUE quando estiver arrastando um ponto;
       Alterou,              //--- TRUE quando a seçao tiver sido alterada.
       AlterouGreide,        //--- TRUE quando houve algula alteraçao no greide
       TerminarArrasto;      //--- TRUE quando for o segundo click quando estiver arrastando um ponto para 
      
 
 CurVertical CurvaOriginal; //--- Sava a curva que está sendo arrastada, para poder restaurar

  POSITION PAtual;           //--- Auxiliar, ponto sendo arrastado.
  int TipoPonto,       //--- Tipo do ponto que esta sendo arrastado, pcv,ptv,piv ou terreno
      Tipo,              //--- tipo do ponto projeto ou terreno
      EMedicao;          //--- Indica se esta desenhando o perfil de uma medicao
  int DesenharPixel,
      DesenharCirculo,
      DesenharCruz,
      GabaritarProjeto,
      DesenharProjeto,
      DesenharTerreno,
      DesenharMalha,
      MostrarDesc,
      MostrarToolCoordenadas,
      MostrarPonto,
      SincronizarSecoes,
      AtualizarSecoes,
      MostrarTitulo,
      VarrerTerreno,
      VarrerProjeto,
      AlterarGreide,
      CircularPontos,
      DesenharPerfGeol,
	  SecoesIconizadas,
	  PermitirArrastar,
      EZoom;
  int EquiMalhaX,
      EquiMalhaY,
      EquiCotas,
      Cotar,
      MetodoEnsaio;

  CFont CurrierNewVertical;              //--- Fonte para escrita vertical
  CFont CurrierNewHorizontal;            //--- Fonte para escrita horizontal

  CDPopUpPontos DPopUpPonto;             //--- Dialogo popup dos pontos

  COLORREF Cores[8];

  enum CORESPERFIS{CORFUNDO,CORMALHA,CORTEXTO,CORTERRENO,CORTERRAPLANAGEM,CORPAVIMENTO}; //--- Array de cores   

  void DesenhaParabola(CurVertical* PIV,CDC* pDC,double hPavim,bool Terraplan);           //--- Metodo para desenhar a parábola.
  void DesenhaPLine(CListaSecoes& ListaSecoes,CDC* pDC);     //--- Desenha uma PLine dada uma lista de secçoes
  void AcerDesSecao(); 
  std::string PreparaDescricao(POSITION Pos,int Tipo); 
  void RedesenharJanela(); 
  void IconizaSecao();
  void IconizarRestaurarSecao(int Modo);
  void VisualizarPonto();
  void DefineParametros();
  void Zoom();
  void DesenhaPerfisGeologicos(CMMemDC* pDC);
  LRESULT EnsaiarGreide(WPARAM WP, LPARAM LP);
  void AdotarEnsaioAtual();

  CDC* pDC;                

protected:
  DesPerfilView(int Medicao = FALSE);           // protected constructor used by dynamic creation

  DECLARE_DYNCREATE(DesPerfilView)

  // Attributes
public:

  // Operations
public:
  virtual ~DesPerfilView();
  void MudarEscala(int Fator);
  void MudarRazao(double Fator);
  void CalculaParametros();
  void CalculaCotaEPosiciona(CEstaca& Estaca);
  void GravarProjeto(int Tipo = 0,bool Forcar = false,bool Perguntar = true);               //--- Chamado pelo menu Calcular volumes (Atualiza os arquivos antes de calcular)
  bool PegaSecoesIconizadas() {return  SecoesIconizadas;};
  bool AlterouProjeto() {return AlterouGreide;}; 
  void CalculaCota(CEstaca& Estaca) {CurvasVerticais.CalculaCota(Estaca);};
  void MostrarApagarSecao(bool Mostrar);
  const std::string& PegaRua() const { return Trecho; }
  
  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(DesPerfilView)
public:
  virtual void OnInitialUpdate();
  virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL

  // Implementation
protected:
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif
  void TransfPixelsXY(CPoint& Ponto,double& Coorx,double& Coory);
  // Generated message map functions
protected:
  //{{AFX_MSG(DesPerfilView)
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnDesgrevoltar();
  afx_msg void OnUpdateDesgrevoltar(CCmdUI* pCmdUI);
  afx_msg void OnGravarperfil();
  afx_msg void OnUpdateGravarperfil(CCmdUI* pCmdUI);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnConfDesGreide();
  afx_msg void OnSecsemprevisIconizar();
  afx_msg void OnIrparaponto();
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnEnquadrar();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnAlternartitulogreide();
  afx_msg void OnButFechar();
  afx_msg void OnUpdateGravar(CCmdUI* pCmdUI);
  afx_msg void OnSincronizarSecoes();
  afx_msg void OnTrancarGreide();
  afx_msg void OnCalcVolumes();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  POSITION VerfPonto(double x,double y,int& Tipo,int& TipoPonto,CString& NumMed = CString(""));
public:
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  afx_msg void OnUpdateCalcvolumes(CCmdUI *pCmdUI);
  afx_msg void OnButarrastar();
  afx_msg void OnClose();
  afx_msg void OnButdesperfgeo();
  afx_msg void OnButensaiargreide();
  afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////
