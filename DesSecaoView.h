// DesSecaoView.h : header file
//

#define NENHUM_TIMER 0
#define OPGRAVAR   1

typedef std::list<Ponto>  typlstPontos;
typedef typlstPontos::iterator ittyplstPontos;

typedef std::set<std::string> tysetNomeArquivos;
typedef std::set<std::string>::iterator tyditsetNomeArquivos;

struct ltitBacias
{
  bool operator()(const ittylstBacias It1, const ittylstBacias It2) const
  {
    return *It1 < *It2;
  }
};

typedef std::set<ittylstBacias,ltitBacias> tysetitlstBacias;
typedef tysetitlstBacias::iterator ittysetitlstBacias;

/////////////////////////////////////////////////////////////////////////////
// DesSecaoView view

class DesSecaoView : public CView
{
  enum TIPOSPERFIS{PROJETO,TERRENO,PAVIMENTO,LIMPEZA,MEDICAO,TERRPRIMEDICAO,PERFILGEOLOGICO,
                   PERFILGEOLOGICOOFFSETS,ACITOPOGFORASUPERF,ALINHAMENTOSTERRAPL,PARTICAOTALVEGUE,
                   HIPSOGRAMA,PERFILTALVEGUE,ERROPARTICAO,NENHUMTIPO};
  enum BARRAFERRM{BARRA_PARTICIONAR = 7};

  LPCTSTR CursorAtual;
  CString NomeProjeto;
  CSecao* SecAnterior;            //--- Secao desenhada anteriormente.
  CHPavimento AltPavimento,AltLimpeza;
  BOOL ComPerfil;                 //--- Marca se a seção está sendo desenhada com o perfil.
  CTaludesCorte TaludesCorte;     //--- Talude de corte.
  CTaludesAterro TaludesAterro;   //--- Talude de aterro.
  xyMouse* MouseXY;               //--- Ponteiro para o diálogo das coordenadas do mouse
  CListaSecoes Secoes,            //--- Secoes a serem desenhadas.  
               *UltMedicao;       //--- Lista com as seções da ultima medicao (se houver)
  CTaludesPavim TaludesPavimento; //--- Taludes do pavimento , usado nos recálculos 
  CCanteiros CanteirosCentrais;   //--- Canteiros centrais, usados nos recálculos
  CSecTipo SecoesTipo;            //--- Secoes tipo, usadas nos recálculos
  CGradiente Gradientes;          //--- Gradientes, usados nos recalculos
  CAlargamento Alargamentos;      //--- Alargamentos, usados na remontagem da seção 
  CPistasFixas PistasFixas;
  CCurHorizontais CurvasHorizontais;
  CRect PosTool1,PosTool2,PosTool3,PosTool4;     //--- Posições dos Toolbars.
  Ponto PontoAnterior;            //--- contem o valor do ponto antes de ser alterado.
  CListaMedicoes ListaMedicoes;   //--- Lista de medicoes
  CSecao SecaoAnterior,           //--- Seção anterior salva (para restaurar)
         SecaoAvulsaAtual,        //--- Ultima seção avulsa mostrada
         TalvegueAtual,           //--- O perfil do talvegue vai no terreno da seção
         HipsogramaAtual;
  CComboBox* pComboFind;          //--- Ponteiro para o combo do find
  CPerfilGeologico PerfilGeologico;  //--- Perfís geológicos do terreno
  tysetCTipoSolo SetTiposSolos;
  itsetCDadosPerfGeologicos itPerfGeo;
  Perfil PerfOffSetsPerfGeologico;   //--- Temporário, para mostrar as descrições dos offsets dos perfis geologicos
  CSecoesAvulsas SecoesAvulsas;
  CString UltimaSecaoAvulsa,
          UltimoTalvegue,
          UltimoHipsograma;
  tylstBacias BaciasHidrograficas;
  ittylstBacias itBaciaAtual;
  tysetitlstBacias BaciasAlteradas;

  double Escala[2],    //--- Escala em pixels por metro oficial; 
    EscalaTalv[2],     //--- ... dos talvegues  
    EscalaHipso[2],    //--- ... dos hpsogramas 
    EscalaSecoes[2],   //--- ... das secoes
    CotaInferior,      //--- cota inferior do vídeo;
    AlturaVideo,       //--- Altura do video em metros. (Eixo z)
    LarguraVideo,      //--- Largura do vídeo em metros. (Eixo x)
    Resolucao[2],      //--- Resolucao do vídeo em pixels.
    Deltas[2],         //--- Deslocamento nos eixos do desenho (pode ser seçoes - talvegues ou histograma).
    DeltasTalvegues[2],//--- Deltas dos talvegues             
    DeltasSecoes[2],   //--- Deltas das seçoes    
    DeltasHipso[2],    //--- Deltas dos hipsogramas   
    ResolucaoXDiv2,    //--- Resolucao x / 2, usado nas transformacoes de coordenadas.   
    ResolucaoYDiv2,    //--- Auxiliar para desenho dos pontos
    DeltaYMetro,       //---  Idem, para facilitar os cálculos.
    Toleranciax,       //--- Tolerancias para procurar o ponto na posição 
    Toleranciay,       //--- Atual do mouse.
    RazaoHV,           //--- Razão entre as escalas H e V do desenho
    RazaoTalvHV,       //--- Razão dos talvegues
    RazaoHipsoHV,      //--- Razão dos Hipsogramas   
    xParticaoAtual,    //--- x da partição do perfil do talvegue que está sendo posicionado
    EscalaRazaoTalvegue[2],
    EscalaRazaoHpsometrica[2];
  CPoint InicioArrast, //--- Usados nas operações de arrastar.
    FimArrast,         //--- Idem.
    PonMousePixelsAtual,//--- Ponto em pixels da posição atual do mouse  
    PonToolSec1,PonToolSec2,PonToolSec3,PonToolSec4,PonPosDesSecoes;  //--- Posição dos toolsvars
  bool Arrastando,          //--- TRUE quando estiver attastando.
    ArrastandoPonto,        //--- TRUE quando estiver arrastando um ponto;
    AlterouSecao,           //--- TRUE quando a seçao tiver sido alterada, FALSE ao calcular os offsets.              
    PrimeiraAlteracao,      //--- TRUE quando for feita uma alteração na secao, FALSE ao sair da secao
    AlterouSecoes,          //--- TRUE se alguma seção foi alterada, usada pra gravar ou não as seções na saída. False ao gravar
    TerminarArrasto,        //--- TRUE quando for o segundo click no ponto, indica o fim do arrasto.
    InseriuPontos,
    Propagar,
    Particionando,
    AlterouBacias,
    AlterouPerfilTalvegue,
    EscalaX10;
  POSITION PAtual,          //--- Auxiliar, ponto sendo arrastado.
    SecaoAtual,             //--- Secao atual que esta desenhada 
    POSPonAnterior,         //--- Aponta para o último pontoque o mouse passou ( ou está)  
    PAciTopogAtual,
    PAlinTerrapAtual;
  int Tipo,                 //--- Auxiliar, tipo de perfil, terreno ou projeto;
    EMedicaoD,              //--- Indica se o desenho atual é de medição ou nao
    EquiMalha,
    EquiMalhaTalv,
    EquiMalhaHipso,
    EquiCotas,
    EquiCotasTalv,
    EquiCotasHipso,
    Ferramentas,            //--- controle de mostrar/esconder os dialogos
    Escalas,
    Procurar,
    Coordenadas,
    Volumes,
    ComTitulo,
    ST;

  std::string Trecho;
  CDVolumes DVolumes;          //--- Dialogo dos volumes
  CDPopUpPontos DPopUpPonto;   //--- Dialogo popup dos pontos

  CFont CurrierNewVertical;              //--- Fonte para escrita vertical
  CFont CurrierNewHorizontal;            //--- Fonte para escrita horizontal

  CDC* pDC;                              //--- Ponteiro para o DC exclusivo

  COLORREF Cores[8];

  enum CORES{CORFUNDO,CORMALHA,CORTEXTO,CORTERRENO,CORTERRAPLANAGEM,CORPAVIMENTO}; //--- Array de cores   
  enum FILTROSECOES{COM2OFFSETS=1,COM1OFFSET=2,COMTERRENO=4,COMPROJETOETERRENO=8,TODASASSECOES=16};	 //--- Filtro de seções a serem mostradas

  int DesenharPavimento,DesenharTerraplanagem,DesenharTerreno,DesenharMalha,CotarMalha,Mostrar,
	    PermitirAlterar,CircularPontos,DesenharPerfisGeologicos,DesenharAciTopog,DesenharSecAvulsas,
      DesenharTalvegues,DesenharHistogramas;

  void TransfPixelsXY(CPoint& Ponto,double &Coorx, double& Coory);  //--- Transforma as coordenadas de pixels para x e y.
  POSITION VerfPonto(double x,double y,double* xParticao = nullptr, CString& NumMed = CString(""));   //--- Verifica se há ponto sobre o mouse.
  int PegaPontosInseridos(const CSecao& SecaoAtual,CLPontos& PontosInseridos,double Gradiente);       //--- Pega os pontos inseridos no projeto
  std::string PreparaDescricao(int Tipo,const CSecao& SecaoAtual,double* xParticaoTalvegue = nullptr);
  CSecao* PegaSecMedicao(CEstaca& Estaca);  //--- Retorna a seção igual na medicao (se houver)
  void AplicaSuperElevacao(Perfil& SecaoAtual,double Gradiente);
  void AplicaSuperLargura(Perfil& ProjetoAtual);
  void AlteraLimpeza(CSecao& SecAtual,Ponto NovoPonto,POSITION PTerr);
  void RemovePontosEditadosProjeto(CSecao &SecAtual,const Perfil&ProjetoEditado);     //--- Os pontos movidos estão no lugar de origem, são removidos aqui
  void RestauraPontosPerfGeo(const Ponto& PontoAnterior,const Ponto& PontoAtual);
  void PropagaRemoçãoPerfGeologico(double x);
  void PropagaInsercaoPerfGeologico(Ponto& Ponto);
  bool IncluiPontosInsRemNaSuperficie(int tipo);

  bool FiltraMostrar(POSITION SecaoAtual)
  {
    const CSecao& SecAtual(Secoes.GetAt(SecaoAtual)); 

    bool TemTerreno(SecAtual.Terreno.GetCount() > 0),TemProjeto(SecAtual.Projeto.GetCount() > 0),TemProjeTerreno(TemTerreno&TemProjeto), 
         Tem1OffSet(TemProjeto && SecAtual.Projeto.GetHead().Descricao == "OFFSEE" || (TemProjeto && SecAtual.Projeto.GetTail().Descricao == "OFFSED")),
         Tem2OffSets(TemProjeto && SecAtual.Projeto.GetHead().Descricao == "OFFSEE" &&  SecAtual.Projeto.GetTail().Descricao == "OFFSED");

    return !(Mostrar == TODASASSECOES || (Mostrar == COMTERRENO && TemTerreno) || (Mostrar == COMPROJETOETERRENO &&  TemProjeTerreno) ||
            (Mostrar == COM1OFFSET && Tem1OffSet) || (Mostrar == COM2OFFSETS && Tem2OffSets));
  }

  void MostraSecao(POSITION Atual)
  {
    const CSecao& SecAtual(Secoes.GetAt(Atual)); 

    MouseXY->m_STSecao = SecAtual.Projeto.Estaca.EstVirtual != INFINITO ? SecAtual.Projeto.Estaca+" "+SecAtual.Projeto.Estaca.Descricao : (SecAtual.Terreno.Estaca.EstVirtual == INFINITO ? "" : SecAtual.Terreno.Estaca);	  
    if(DesenharSecAvulsas) MouseXY->m_STSecao += " Avulsa";
    MouseXY->UpdateData(FALSE);
  }

  void DesenhaCerca(const Ponto& PInicial,const Ponto& PFinal,CDC* ppDC);
  void DesenhaLinhaFerrea(const Ponto& PInicial,const Ponto& PFinal,CDC* ppDC);
  void DesenhaMuro(const Ponto& PInicial,const Ponto & PFinal,CDC* ppDC);
  void CriaPenaRestriçao(CPen *PenaObjeto,unsigned int Objeto);
  std::string PegaUltimoArquivoSecoes(std::string& NomeAProcurar);
  void PreencheComboProcurar(void* pChildFrame);
  void PreencheComboProcurarSecAvulsas(void* pChildFrame);
  void PreencheComboProcurarTalvegues(void* pChildFrame);
  void LeBaciasHidrograficas(const std::string& NomeProj);
  void GravaBaciasHidrograficas(const std::string& NomeProjBase,int Perguntar = false);
  void SalvaPerfilTalvegue();
  std::string PegaUltimaEdicao(std::string& NomeArquivo);
  void ConverteSecoesTransSecoesAvulsas();
  void ConverteSecoesAvulsasSecoesTrans();

  void Centraliza()
  {
    RECT Coord;
    GetClientRect(&Coord);

    Resolucao[0] = Coord.right;
    Resolucao[1] = Coord.bottom;

    ResolucaoXDiv2 = Resolucao[0] / 2.0;     //--- Usada nas transformaões de coordenadas.
    ResolucaoYDiv2 = Resolucao[1] / 2.0;     //--- Usada nos desenhos dos pontos.   

    CalculaParametros();
  }
  
protected:

  DesSecaoView();           // protected constructor used by dynamic creation

  DECLARE_DYNCREATE(DesSecaoView)

  // Attributes
public:

  int TerrenoProjeto;

  // Operations
public:
  virtual ~DesSecaoView();

  void CalculaVolumes();      //--- Chamado pelo menu Calcular volumes (Atualiza os arquivos antes de calcular)
  void GravarSecoes(bool Perguntar=true) ;
  bool VerfAlterouSecoes() { return AlterouSecoes ;};
  POSITION PegaSecAtual() { return SecaoAtual; };  //--- Retorna a secao que esta desenhada, udada nos desenho do perfil
  CListaMedicoes& PegaListaMedicoes() {return ListaMedicoes;};
  void MudarRazao(double Fator);
  void MudarEscala(int Fator);                                    //--- Muda a escala do desenho.
  void Posiciona(double x = INFINITO,double Cota = INFINITO,bool VeioDoComboProcurar=false,int Tipo = 0,std::string* pNomeSecAvulas=NULL);     //--- Posiciona a lista de soções em uma determinada seção, chamada pelo desenho do perfil
  void Reinicializa();                                            //--- Reinicializa od dados, chamada no recálculo dos volumes
  void CalculaParametros();
  void MostrarVolumes(bool Mostrar) 
  { 
    if (Volumes)
    { 
     if(Mostrar) DVolumes.ShowWindow(SW_SHOW);
     else DVolumes.ShowWindow(SW_HIDE);
    }
    
  };

  void OnNcLButtonDown2(UINT nHitTest, CPoint point);
  void Fechar(); 
  void ReleSecoes();     //--- Le novamente as seções, para recalcular os volumes
  int PegaVolumes() const {return Volumes;} 
  bool PegaPermitirAlterar() const { return PermitirAlterar > 0;}
  int PegaEstadoToolBars() 
  {
    return (((Coordenadas > 0) << 3) + ((Escalas > 0) << 2) + ((Procurar > 0) << 1) + (Ferramentas > 0));
  }

  double PegaEstacaAtual()
  {
    if(SecaoAtual) return Secoes.GetAt(SecaoAtual).Estaca.EstVirtual;
    else return INFINITO;
  }

  const std::string& PegaRua() const { return Trecho;}
  const CString& PegaNomeProjeto() const { return NomeProjeto; }

  POSITION PegaSecaoAvulsa(const CString& NomeSevAvulsa)
  {
    CList<CSecaoAvulsa, CSecaoAvulsa&>& LSecAvulsas(SecoesAvulsas.PegaLSecoesAvulsas());

    register POSITION PSecAtual(LSecAvulsas.GetHeadPosition());

    for (; PSecAtual && LSecAvulsas.GetAt(PSecAtual).Nome.compare(NomeSevAvulsa) != 0; LSecAvulsas.GetNext(PSecAtual));

    return PSecAtual;
  }

  bool DesenhandoSecAvulsa()
  {
    return DesenharSecAvulsas;
  }

  bool DesenhandoTalvegue()
  {
    return DesenharTalvegues;
  }
  
  bool DesenhandoHistograma()
  {
    return DesenharHistogramas;
  }

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
  afx_msg void OnUpdateCalcOffSets(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButSecAvulsas(CCmdUI* pCmdUI);
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
  afx_msg void OnConfigtoolsec();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnCalcoffsets();
  afx_msg void OnRestaurarsecao();
 // afx_msg void OnCalcvolumes();
  afx_msg void OnUpdateCalcvolumes(CCmdUI* pCmdUI);
  afx_msg void OnGravarSecoes();
  afx_msg void OnUpdateGravar(CCmdUI* pCmdUI);
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnAlternartitulo();
  afx_msg void OnButvisualizartodasecao();
  afx_msg void OnButmostsecavulsas();
  afx_msg void OnUpdateRestaurarsecao(CCmdUI* pCmdUI);
  afx_msg void OnButTrancarSecao();
  afx_msg void OnButFechar();
  afx_msg void OnUpdateFechar(CCmdUI* pCmdUI);
  afx_msg LRESULT FechouToolBar(WPARAM wParam, LPARAM lParam);
  afx_msg void OnPropagar();
  afx_msg void OnUpdatePropagar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButTalvegues(CCmdUI* pCmdUI);


  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
  virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
public:
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnBnClickedRaterreno();
  afx_msg void OnBnClickedRaprojeto();
  afx_msg void OnCalcvolumes();
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  afx_msg void OnClose();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnSetFocus(CWnd* pOldWnd);
 // afx_msg void OnUpdateButdesacitopog(CCmdUI *pCmdUI);
  afx_msg void OnButacitopog();
  afx_msg void OnButSecAvulsas();
  afx_msg void OnButTalvegues();
  afx_msg void OnButParticionar();
  afx_msg void OnButHistograma();
  afx_msg void OnButX10();

};

/////////////////////////////////////////////////////////////////////////////
