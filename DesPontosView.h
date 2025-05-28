// DesSecaoView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView view

class DesSecaoView : public CView
{
  enum{PROJETO,TERRENO,PAVIMENTO,MEDICAO,TERRPRIMEDICAO};

  CString NomeProjeto;

  CDPonto DPonto;                 //--- Dialogo do ponto.
  xyMouse* MouseXY;               //--- Ponteiro para o diálogo das coordenadas do mouse
	CRect PosTool1,PosTool2,PosTool3,PosTool4;     //--- Posições dos Toolbars.
  Ponto PontoAnterior;            //--- contem o valor do ponto antes de ser alterado.
	
	double   Escala[2],         //--- Escala em pixels por metro; 
	         AlturaVideo,       //--- Altura do video em metros. (Eixo z)
				   LarguraVideo,      //--- Largura do vídeo em metros. (Eixo x)
           Resolucao[2],      //--- Resolucao do vídeo em pixels.
				   Deltas[2],         //--- Deslocamento nos eixos.
				   ResolucaoXDiv2,    //--- Resolucao x / 2, usado nas transformacoes de coordenadas.   
					 ResolucaoYDiv2,    //--- Auxiliar para desenho dos pontos
				   DeltaYMetro,       //---  Idem, para facilitar os cálculos.
				   Toleranciax,       //--- Tolerancias para procurar o ponto na posição 
				   Toleranciay,       //--- Atual do mouse.
	         RazaoHV;           //--- Razão entre as escalas H e V.
  CPoint   InicioArrast,      //--- Usados nas operações de arrastar.
	         FimArrast,         //--- Idem.
	         PonToolSec1,PonToolSec2,PonToolSec3,PonToolSec4,PonPosDesSecoes;  //--- Posição dos toolsvars
  BOOL     Arrastando,        //--- TRUE quando estiver attastando.
           ArrastandoPonto,   //--- TRUE quando estiver arrastando um ponto;
	         Alterou,           //--- TRUE quando a seçao tiver sido alterada, FALSE ao calcular os offsets.              
					 PrimeiraAlteracao, //--- TRUE quando for feita uma alteração na secao, FALSE ao sair da secao
					 TerminarArrasto;   //--- TRUE quando for o segundo click no ponto, indica o fim do arrasto.
  POSITION PAtual,            //--- Auxiliar, ponto sendo arrastado.
  int      Tipo,              //--- Auxiliar, tipo de perfil, terreno ou projeto;
		     
  void TransfPixelsXY(CPoint& Ponto,double &Coorx, double& Coory);  //--- Transforma as coordenadas de pixels para x e y.
  void MostraPonto(int Tipo,CSecao& SecaoAtual,CString& NumMed = CString(""));   //--- Mostra o ponto que está sobre o mouse.
  POSITION VerfPonto(double x,double y,CString & NumMed = CString(""));   //--- Verifica se há ponto sobre o mouse.
CSecao* PegaSecMedicao(CEstaca& Estaca);  //--- Retorna a seção igual na medicao (se houver)

  protected:
	  DesSecaoView();           // protected constructor used by dynamic creation


		DECLARE_DYNCREATE(DesSecaoView)

	// Attributes
  public:

  // Operations
  public:
	  virtual ~DesSecaoView();

	void Gravar();         //--- Grava o estado atual das seções.

	void MudarRazao(double Fator);
 	void MudarEscala(int Fator);                                    //--- Muda a escala do desenho.
	void DesenhaSecao(CEstaca& Estaca);                             //--- Desenha uma determinada seção.
	void Posiciona(double x = INFINITO,double Cota = INFINITO);     //--- Posiciona a lista de soções em uma determinada seção, chamada pelo desenho do perfil
	void Reinicializa();                                            //--- Reinicializa od dados, chamada no recálculo dos volumes
  void CalculaParametros();

  // Overrides
	  // ClassWizard generated virtual function overrides
	  //{{AFX_VIRTUAL(DesSecaoView)
	public:
	virtual void OnInitialUpdate();
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

	// Generated message map functions
  protected:
	  //{{AFX_MSG(DesSecaoView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnButproxima();
	afx_msg void OnButanterior();
	afx_msg void OnPrimeirasecao();
	afx_msg void OnUltimasecao();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdatePrimeirasecao(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUltimasecao(CCmdUI* pCmdUI);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnConfigtoolsec();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCalcoffsets();
	afx_msg void OnRestaurarsecao();
	afx_msg void OnUpdateRestaurarsecao(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCalcoffsets(CCmdUI* pCmdUI);
	afx_msg void OnGravar();
	afx_msg void OnCalculoVolumes();
	afx_msg void OnCalcvolumes();
	afx_msg void OnUpdateCalcvolumes(CCmdUI* pCmdUI);
	afx_msg void OnUpdateGravar(CCmdUI* pCmdUI);
	//}}AFX_MSG
	  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
