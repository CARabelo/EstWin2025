// DesPerfilView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DesPerfilView view

struct ltEstComCota 
{ 
    bool operator()(const CEstaca& e1, const CEstaca& e2) const 
    { 
      return e1.EstVirtual < e2.EstVirtual ? true :
             e1.EstVirtual > e2.EstVirtual ? false :
             e1.Cota < e2.Cota ? true : false;
    }
};

typedef std::set<CEstaca> tysetEstaca;
typedef tysetEstaca::iterator ittysetEstaca;

#define X        0
#define Y        1

class DesPerfilGeoView : public CView
{
  CArqOAC  OAC;
  CArqOAE  OAE;

  CFile ArqSecTria;

  LPCTSTR CursorAtual;

  enum TIPOSPONTOS{NENHUMTIPO,PONTOFUROSONDAGEM,PONTOTERRENO,PONTOTIPOSOLO,PONTOPERFILSONDAGEM,TODOSTIPOS,PONTOPERFILGEOLNOTERRPRIM,FIMTIPOPONTOS};
  enum OPERACOES{NENHUMA_OPERACAO,OPERACAO_APAGAR,OPERACAO_DESENHAR,OPERACAO_AMBAS,FIM_OPERACOES};
  enum CORESPERFIS{CORFUNDO,CORMALHA,CORTEXTO,CORTERRENO,CORPERFILGEOLOGICO,TOTALPENAS}; //--- Array de cores   
  enum OBJETOS{NENHUM_OBJETO,OBJETO_PONTO,OBJETO_LINHA,OBJETO_PERFIL_TERRENO,OBJETO_PERFIL_GEOLOGICO,OBJETO_TRECHO_PERFIL,FIM_OBJETOS};
  enum ERROSPERFIL{NENHUM_ERRO,ERRO_PASSOUULTIMOFURO,ERRO_CORTOUFURO,FIMERROSPERFIL};
  enum TIPPONSETPERFIL{NAO_PERTENCE,PRIMEIROPONTO,ULTIMOPONTO,MEIOPERFIL};

  CPerfilGeologico PerfilGeologico;
  ittyPerfilTipoSolo itInicioTrecho,itFimTrecho;
  ittysetCPerfilTipoSolo itPerfilAtual,itPerfilAnterior,itPerfilMarcado,itPerfilSelecionado;
  tysetEstaca setEstacasTerreno;
  ittysetEstaca itPosAnterior,itPosAtual,itInicioTrechoTerreno,itFimTrechoTerreno;
  CEstaca* pEstSendoMovida,EstSendoMovidaAnterior,PontoOriginal;

  tysetCTipoSolo SetTiposSolos;

  CXYGreide* MouseXYGre;     //--- Ponteiro para o diálogo das coordenadas do mouse
  double Escala[2],          //--- Escala em pixels por metro; 
         EscalaDefault[2],   //--- Escalas padrão que permitem a visualização de todo o desenho.
         Deltas[2],          //--- Deslocamento nos eixos.
         AlturaVideo,        //--- Altura do video em metros.
         LarguraVideo,       //--- Largura do vídeo em metros. (Eixo x)
         Resolucao[2],       //--- Resolucao do vídeo em pixels.
         DeltaYMetro,        //--- Idem, paRedesenharra facilitar os cálculos.
         Toleranciax,        //--- Tolerancias para procurar o ponto na posição 
         Toleranciay,        //--- Atual do mouse.
         Toleranciaxy_2,     //--- Distancia ao quadrado da tolerancia
         RazaoHV,            //--- Razão entre as escalas H e V.
         LimiteEsq,          //--- Limites do terreno
         LimiteDir,
         LimiteSup,
         LimiteInf,
         LimitesMoverEstaca[2];
  CPoint InicioArrast,      //--- Usados nas operações de arrastar.
         InicioZoom,
         FimZoom,
         PonMousePixelsAtual,      
         PonMousePixelsAnterior,   
         PonMouseClicado,
         FimArrast;            
  bool Arrastando,             
       ArrastandoPonto,        
       AlterouPerfil,                
       TerminarArrasto,        
       InserindoPonto,
       TrechoTerrenoCopiado,
       CopiarApenasTrecho,
       TrechoPerfilGeolCopiado,
       MarcandoTrecho,
       TrechoMarcado,
       PegouPrimeiroPonto;
                            
  POSITION PAtual;            //--- Auxiliar, ponto sendo arrastado.
  int DesenharTerreno,
	  DesenharMalha,
	  MostrarDesc,
	  MostrarPonto,
	  MostrarTitulo,
	  VarrerTerreno,
	  CircularPontos,
	  PermitirArrastar,
	  EZoom,
	  EquiMalhaX,
	  EquiMalhaY,
	  EquiCotas,
	  Cotar,
	  MoverApenasEixoy;

  CFont CurrierNewVertical;              //--- Fonte para escrita vertical
  CFont CurrierNewHorizontal;            //--- Fonte para escrita horizontal

  CDPopUpPontos DPopUpPonto;             //--- Dialogo popup dos pontos

  COLORREF Cores[8];

  int Tipo;
  const void* pPontoMouse;
  const void* pPosAnterior;

  std::string Trecho,TrechoAnterior;

  //CArqTiposSolos ArqTiposSolos;

  std::string PreparaDescricao(const void* pPonMouse,int Tipo,const std::string* strTipoSolo=NULL);
  void VisualizarPonto();
  void DefineParametros();
  void Zoom();
  void GravarProjeto(int Tipo,bool Forcar,bool Perguntar);    
  bool LeTerreno(const CString& NomeProjeto);
  const void* PosicionaMaisProximo(double x,double y,double Toleranciax,double Toleranciay,int& TipoPonto);
  void DesenhaFuroSondagem(CMMemDC* pDC);
  void DesenhaPerfisGeologicos(CMMemDC* pDC,CPen* pPena);
  double InterpolaCotaTerreno(const CEstaca& Estaca);
  void CalculaCotasFurosSondagem();
  void DesenharApagar(int TipoObjeto,const CPoint& LocalAnterior,const CPoint& LocalAtual,void* Objeto=NULL,int OperacaoForcada=OPERACAO_AMBAS,bool PrimeiroMovimento=false);
  void SelecionaPerfil(char* pTipoPerfil);
  void PegaTrechoPerfGeol(const CEstaca& Estaca,ittyPerfilTipoSolo& itInicioTrecho,ittyPerfilTipoSolo& itFimTrecho);
  bool VerfInsercaoPerfilGeologico(const tyPerfilTipoSolo& PerfilTipoSolo);
  bool VerPerfilAcimaTerrenoPrimitivo(const tyPerfilTipoSolo& PerfilTipoSolo,double Deltay);
  void CalculaIsTerrenoPrimitivo();
  void RemovePerfisNoTerreno();
  
  void ApagaElastico(int Objeto) 
  {
     DesenharApagar(Objeto,PonMouseClicado,PonMousePixelsAnterior,NULL,OPERACAO_APAGAR);
     PonMousePixelsAtual = PonMousePixelsAnterior = PonMouseClicado;
  }

  void PiscarPerfil(ittysetCPerfilTipoSolo ItPerfil)
  {
    for(int C = 0 ; C < 12 ; C++)
    {
      if(C%2 == 0)  itPerfilMarcado = ItPerfil;
      else itPerfilMarcado = PerfilGeologico.PegaPerfisSolos().end();
      RedrawWindow();
      WaitForSingleObject(CEvent(),100);
    }
  }

  void VerfIniFimTrecho()
  {
    if(itFimTrecho->EstVirtual < itInicioTrecho->EstVirtual)
    {
      ittyPerfilTipoSolo itTemp(itFimTrecho);
      itFimTrecho = itInicioTrecho;
      itInicioTrecho = itTemp;
    }
  }

  int CortouFuroSondagem(double x);
  int CortouFuroSondagem2(double xOriginal,double xAtual);
  typairittyPerfilTSrsetPTS* VerificaTangente(const Ponto& PAtual,bool& NaTangente);
  bool VerificaInsercao(const CEstaca& PontoAtual,const CEstaca& PontoAnterior,bool MovPerfil = false);
  bool CruzouTerreno(const CEstaca& PontoInserir);
  void AbrePerfil(typairittyPerfilTSrsetPTS* pairPontoSet);
  void RemoveTrechoQualquer(ittyPerfilTipoSolo itInicio,ittyPerfilTipoSolo itFim,CPerfilTipoSolo* PerfilTSAtual); 

  bool ETerreno(const CEstaca& PontoInserir) 
  {  
    ittysetEstaca it(setEstacasTerreno.find(PontoInserir));
    return (it != setEstacasTerreno.end() && fabs(it->Cota - PontoInserir.Cota) < .005);   //--- Verificar a raz~]ao desta diferença
  };

  TIPPONSETPERFIL VerfPontoPertencePerfil(const CEstaca& PontoInserir)
  {
    for(ittysetCPerfilTipoSolo itPerfilSolo=PerfilGeologico.PegaPerfisSolos().begin() ; itPerfilSolo != PerfilGeologico.PegaPerfisSolos().end() ; ++itPerfilSolo)
    {
      ittyPerfilTipoSolo it(itPerfilSolo->PegaPerfil().find(PontoInserir));

      if(it == itPerfilSolo->PegaPerfil().end()) return NAO_PERTENCE;
      if(it == itPerfilSolo->PegaPerfil().begin()) return PRIMEIROPONTO;
      if(*it == *itPerfilSolo->PegaPerfil().rbegin()) return ULTIMOPONTO;
    }
     return MEIOPERFIL;;
  };
  
  void EstabeleceLimitesMovimento(ittyPerfilTipoSolo* it)
  {
    ittyPerfilTipoSolo itAnterior(*it),itPosterior(*it);

    if(itAnterior != itPerfilAtual->PegaPerfil().begin()) --itAnterior;
    if(itPosterior != itPerfilAtual->PegaPerfil().end()) ++itPosterior;

    LimitesMoverEstaca[0] = itAnterior == itPerfilAtual->PegaPerfil().begin() ? -INFINITO : itAnterior->EstVirtual+0.1;
    LimitesMoverEstaca[1] = itPosterior == itPerfilAtual->PegaPerfil().end() ? INFINITO : itPosterior->EstVirtual-0.1;
  }

  ittysetEstaca VerfPontonoTerreno(const CEstaca& Ponto);
  bool VerificaInsercaoPonto(ittyPerfilTipoSolo ItPerfilSolo,ittysetCPerfilTipoSolo ItSetPerfTipoSolo,CEstaca& EstPonto);

  CPoint& TransfCoordReaisParaVideo(const CEstaca& Ponto) const
  {
    static CPoint Resposta;
   
    Resposta.x = (int) (Deltas[X] + (Ponto.EstVirtual * Escala[X]));
    Resposta.y = (int) (Deltas[Y] + (Resolucao[Y] - (Ponto.Cota) * Escala[Y]));

    return Resposta;
  }

  void PegaTrechoTerreno(const CEstaca& Ponto,ittysetEstaca& itSELB,ittysetEstaca& itSEUB);
  const void* VerificaTipoPerfilOrfao(ittysetCPerfilTipoSolo itPerfil,int& TipoPonto);

  bool DesPerfilGeoView::VerfLigacaoFurosSondagem()
  {
    if(itPerfilMarcado->PegaPerfil().size() < 2) return false;

    const tysetFurosSondagem SetFurosSondagem(PerfilGeologico.PegaFurosSondagem().PegaSetFurosSondagem());

    for(itctysetFurosSondagem itFuroSondagem = SetFurosSondagem.begin() ; itFuroSondagem != SetFurosSondagem.end() ; ++itFuroSondagem)
    {
      ittyPerfilTipoSolo itUB(itPerfilMarcado->PegaPerfil().upper_bound(itFuroSondagem->PegaEstaca()));

      if(itUB != itPerfilMarcado->PegaPerfil().begin()) --itUB; 

      if(*itUB == itFuroSondagem->PegaEstaca()) return true;
    }

    return false;
  }

 
 CDC* pDC;                

protected:
  DesPerfilGeoView();           // protected constructor used by dynamic creation

  DECLARE_DYNCREATE(DesPerfilGeoView)

  // Attributes
public:

  // Operations
public:
  virtual ~DesPerfilGeoView();
  void MudarEscala(int Fator);
  void MudarRazao(double Fator);
  void CalculaParametros();
  void CriaPerfisNosTrechos(ittyPerfilTipoSolo pitInicioTrecho,ittyPerfilTipoSolo pitFimTrecho,Ponto Deltas);

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
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnGravarperfil();
  afx_msg void OnUpdateGravarperfil(CCmdUI* pCmdUI);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnConfDesGeologico();
  afx_msg void OnIrparaponto();
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnEnquadrar();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnAlternartitulogreide();
  afx_msg void OnButFechar();
  afx_msg void OnUpdateGravar(CCmdUI* pCmdUI);
  afx_msg void OnTrancarGreide();
  afx_msg void OnCorPerfAtual();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
private:
  const void* VerfPonto(double x,double y,int& Tipo);
public:
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  afx_msg void OnButarrastar();
  afx_msg void OnClose();
  afx_msg void OnPopupRemoverTodoPerfil();
  afx_msg void OnPopupPerfilSair();
  afx_msg void OnPopupRemoverTrechoPerfil();
  afx_msg void OnTerrenoCopiartodooperfil();
  afx_msg void OnTerrenoCopiarapenasestetrecho();
  afx_msg void OnTerrenoSair();
  afx_msg void OnButmovesoy();
  afx_msg void OnUpdateButmovesoy(CCmdUI *pCmdUI);
  afx_msg void OnPopupPerfilSelecionar();
  afx_msg void OnPerfilCopiartodoperfil();
  afx_msg void OnPerfilCopiarapenasestetrecho();
  afx_msg void OnPopupMudarTipo();
  afx_msg void OnQualquertrechoRemover();
  afx_msg void OnQualquertrechoCopiar();
  afx_msg void OnQualquertrechoSair();
  afx_msg void OnRestTodoTerreno();
  afx_msg void OnRestEsteTrechoTerreno();
  afx_msg void OnPopupVisualizarperfilselecionado();
  afx_msg void OnButdesperfgeo();
  afx_msg void OnButensaiargreide();
  afx_msg void OnSetFocus(CWnd* pOldWnd);
};

/////////////////////////////////////////////////////////////////////////////
