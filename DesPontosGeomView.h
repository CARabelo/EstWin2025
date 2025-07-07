#if !defined(AFX_DESPONTOSGEOMVIEW_H__68799681_1095_11D8_BE61_826A873C8818__INCLUDED_)
#define AFX_DESPONTOSGEOMVIEW_H__68799681_1095_11D8_BE61_826A873C8818__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// 
// header file
//

#define OPR_AMBAS 2

struct ltRelCamadas
{
  bool operator()(const tlstwstr& c1, const tlstwstr& c2) const
  {
    return *c1.begin() < *c2.begin();
  }
};

typedef std::set<tlstwstr, ltRelCamadas> tysetRelCamAcadVUE;
typedef tysetRelCamAcadVUE::iterator ittysetRelCamAcadVUE;

typedef std::vector<POINT> VPOINT;
typedef VPOINT::iterator itVPOINT;

typedef std::list<Ponto> lstPontos;
typedef lstPontos::iterator itlstPontos;

typedef struct Malha
{
  PairDoisPontos Pontos;
  Ponto PontoCota;
  std::string Cota; 
}stMalha;

typedef std::list<stMalha>  listPontosMalha;
typedef listPontosMalha::iterator itlistPontosMalha;

typedef std::set<CLembrete> tysetLembretes;
typedef tysetLembretes::iterator ittysetLembretes;

typedef std::set<ArqAciTopograficos> tysetRegAciTopograficos;             //--- Acidentes topográficos orddenados por (string) código
typedef tysetRegAciTopograficos::iterator ittysetRegAciTopograficos;
typedef std::pair<ittysetRegAciTopograficos,bool> typairittysetRegAciTopograficosbool;

typedef std::map<int,ittysetRegAciTopograficos> tymapItRegAciTopograficos;         //--- Acidentes topográficos ordenados sequencialmente 
typedef tymapItRegAciTopograficos::iterator ittymapItRegAciTopograficos;
typedef std::pair<int,ittysetRegAciTopograficos> pairIntittysetRegAciTopograficos;

typedef std::map<std::string,int> tymapstrint;  //--- Um acidente topográfico pode ter várias chaves, este mapa mapeia a chave para o acidente topográfico
typedef tymapstrint::iterator ittymapstrint;    //--- Na proxima versão fazer um classa para tratar estes dados
typedef std::pair<std::string,int> pairstrint;

struct ltPontox
{
  bool operator()(const Ponto& p1, const Ponto& p2) const
  {
    return p1.x < p2.x;
  }
};

typedef std::set<Ponto,ltPontox> tysetPontos;
typedef tysetPontos::iterator ittysetPontos;

/////////////////////////////////////////////////////////////////////////////
// DesPontosGeomView view

class DesPontosGeomView : public CView
{
  enum{X,Y};
  enum{CORFUNDO,CORPONTOS,CORMALHA,CORBORDOS,CORCRISTASPES,CORARESTAS,CORSECOES,CORTEXTO,CORPROJETO,CORCN};    //--- Tipos de cores
  enum{OBJ_NENHUM,OBJ_PONTO,OBJ_LINHA,OBJ_CURVA,OBJ_SECAO,OBJ_CURNI,OBJ_SECAV,OBJ_CURHR,OBJ_TEXTO,OBJ_PONSECAVULSA,OBJ_PONINSUSUARIO,OBJ_PONINSDELAUNAY,
       OBJ_PONINSGABRIEL,OBJ_PONTERRENO,OBJ_PONDESABILITADO,OBJ_PROJETO,OBJ_IMGBITMAPFLAG,OBJ_LEMBRETE,OBJ_IMGBITMAPSETA,OBJ_RADAR,OBJ_GRADITERRAP,
       OBJ_TERRAPLAN,OBJ_SINPONCTRL,OBJ_ESBOCO,OBJ_PONFOZBACIAHIDROG,OBJ_OAC,OBJ_PONTO_COTA_IBGE,OBJ_PONTO_PROCURADO,OBJ_PONCGBACIAHIDROG};//, OBJ_PONINSSECOES};
  enum {SUPERF_TRADICIONAL,SUPERF_DRONES,SUPERF_GOOGLEEARTH,SUPERF_GERENMAPAS,SUPERF_TOPOG_ACAD,SUPERF_SIG};
  enum {SUPERF_TERRENO_NATURAL,SUPERF_TERRAPLANAGEM};

  DesCroquisView* pDesCroquisView;
  int UltPontoInserido, UltPontoEsbocado;

  LPCTSTR CursorAtual;
  CSuperficie Superficie;                //--- Superficie com os pontos  
  CTrechos Trechos;

  CRadar Radar;
  lstPontos LPSecoes,LPontosMedidos,LPontosProcurados;
  tysetPontos SetCotasVermelhas,SetPontoCotadosSuperf;
  tysetSecoesTerrap SecoesTerraplanagem;
  CCamadaDGN CamadaDGN;
  tysetpairint SetForcarAresta;
  ItSetItLPontos ItPontoCNAtual,ItCNLigar;
  lstPontos BaciaHidrografica;

  CDC* pDC;
  CDC dcMemory;

  Ponto PCTRLAnt[5], PCTRLAtual[4];

  UINT m_nTimer;                        //--- timer para o evento onmousestop

  std::string strTrechoAtual,ProjetoAtual,Projeto,strNomeSecAvulsa,NomeEquiAtual,BufferPontosProcurados;
  
  int PrimeiroMovimento,         //--- Usado na movimentação de PIs e CCs, a primeira vez não apaga nada.       
    DesenharCirculo,             //--- Parametro : define se a cruz no ponto deve ser desenhada.
    DesenharSecoes,
    DesenharArestas,
    DesenharBordos,
    DesenharPixel,
    DesenharRestricoes,
    DesenharPontos,
    DesenharCruz,         
    MostrarDescricao,
    DesenharMalha,
    DesenharProjeto,
    CircularSecoes,
    Arrastando,            //--- TRUE quando estiver arrastando.
    ArrastandoPI,          //--- TRUE quando estiver arrastando um ponto;
    ArrastandoCC,
    CriandoRestricao,      //--- TRUE se estiver criando um acidente topografico. 
    CriandoBordo,
    EZoom,                 //--- TRUE quando estiver executando um zoom
    ComSecoes,
    SecoesIconizadas,
    TerminarArrasto,
    AlterouTerreno,        //--- TRUE quando a seçao tiver sido alterada, FALSE ao calcular os offsets.              
    AlterouProjeto,
    AlterouSecoesAvulsas,
    AlterouAngulo,
    AlterouSuperficie,
    MostrarToolBarGeom,
    MostrarToolBarSuperf,
    MostrarAciTopogSuperficie,
    CotarMalha,
    VarrerTerreno,
    VarrerProjeto,
    VarrerCN,
    DesenharCN,
    CotarCN,
    Inicializou,
    PegandoPontoSecAvulsa,
    PegouPrimPonto,
    SincronizarSecoes,
    NaoPermitirAlteracoes,
    DesenharObjetos,
    DesenhandoObjetos,
    EsbocandoObjetos,
    ObjetosDrenagem,
    CircularPontoIntersecao,
    FiltrarAciTopog,
    DesenharPontosInseridos,
    PegouPonto,
    InserindoPonto,
    PegouPontoDesabilitado,
    PegouPontoInserido,
    PegouRestricao,
    PegouPontoPerfilAvulso,
    TrocandoObjeto,
    InserindoLembrete,
    MovendoLembrete,
    PermitirArrastar,
    RadarTodosPontos,
    RadarPontosInteresse,
    RadarRealcarPontosInteresse,
    ZerarAzimute,
    VerificouCota,
    SincPontosControle,
    FaltaGeoReferenciamento,
    DesenharMapa,
    ProjetoGeoreferenciado,
    Medindo,
    DesenharCroquis,
    PrimeiraVisualizacao,
    TemArqTriaSuperf,
    IniciouSarjeta,
    RemovendoDrenagem,
    RemovendoEspigao,
    ArrastandoEquipDrenagem,
    DefinindoObjDrenagem,
    DesenharEquipDrenagem,
    DesenharSimbolTerapl,
    DesenharCNTaludes,
    DesenharSecoesAvulsas,
    TemMapa,
    IniciouTranslado,
    Transladando,
    TransladandoPorTeclas,
    CotandoCNDGN,
    RealcarCNsMestras,
    ImportouAciTopogAcadOK,
    DesTodasCN,
    DesCNNaCota,
    DesCNAcimaCota,
    DesCNAbaixoCota,
    DesCNNaFaixa,
    JuntandoEspigoes,
    ProcurarEspigaoDoInicio,
    AlterouBacias,
    DefinindoTalveguePrincipal;

  unsigned int EquidisCN;

  double AnguloRotacaoAnterior,
    AnguloRotacao,
    Seno,
    Coseno,
    Tangente,
    Cotangente,
    SenoM,
    CosenoM,
    AlturaRebatidaYPixels,
    LarguraRebatidaYPixels,
    LarguraRebatidaXPixels,
    AlturaRebatidaXPixels,
    AlturaVideoPixels,
    LarguraVideoPixels,
    AlturaRebatidaXPixel,
    NovaAlturaPixels,
    NovaLarguraPixels,
    ArestaMalha,
    PrimeiraCotaY,
    PrimeiroDeltaY,
    PrimeiraCotaX,
    PrimeiroDeltaX,
    OrigemX,
    RazaoEscalasJPGDes,
    DeltaJPG,
    dNaCota,
    AcimaDaCota,
    AbaixoDaCota,
    CotaInicial,
    CotaFinal; 

  CXYGreide* MouseXYGeo;       //--- Ponteiro para o diálogo das coordenadas do mouse
  CDPopUpPontos DPopUpPonto;
  ItLLDeltaSup ListaAtual;     //--- Lista atual de restrições (bordos ou acidentes topográficos) está sendo editada
  int EquiMalha,               //--- Equidistancia da malha
    EquiCotas,                 //--- Equidistancia das cotas da malha
    bAciTopog,                 //--- Valor do radio dos acidentes topograficos
    bProjeto,                  //--- Valor do radio para projeto  
    Quadrante,                 //--- Quadrante do angulo de rotação (0,1,2,3) 
    DesCNTopograficas,         //--- tipo de CN A desenhar
    DefBordos,                 //---Edita os bordos
    DefinindoLinhaBase,
    SaltarCNs,
    MostrarRadar,
    MostrarTerrap,
    DesenharAlinTerrap,
    DesenharOffSets,
    NumPonCtrl,
    MapaSolto,
    TipoGeoref,
    PegouEquipDrenagem,
    DensidadeSimbologia;
      
  COLORREF Cores[12];           //--- Cores do desenho

  MSecaoGeomPonto MapaSecoesPonto;   //--- Set de secoes orddenadas por ponto, os elementos são ponteiros para o mapa de 
                                     //--- secoes ordenadas por Estaca "MapaSecoesGeom" 
  CArqAciTopograficos ArquivoAciTopograficos;
  LLDeltaSup AlinhamentosTerrapl;
  LDeltaSup ListaAJuntar,TalveguePrincipal,TalvegueMontado;
  LItCLLDeltaSup TalveguesDoTalveguePrincipal;

  tysetRegAciTopograficos SetAciTopograficos;
  tymapItRegAciTopograficos MapAciTopograficos;
  tymapstrint MapaChavesAciTopograficos;
  
  ItMCurHorizontaisTS ItUltimaCurvaAjustadaTS;

  //--- Iteradores para os sets de curvas horizontais por TS, PI e CC.

  ItCMCurHorizontaisTS ItCurHorizSendoArrastadaTS;
  ItCMCurHorizontaisPI* ItCurHorizSendoArrastadaPI;
  ItCMCurHorizontaisCC* ItCurHorizSendoArrastadaCC;
  ItCMCurHorizontaisPI ItCurHorizSendoArrastadaPIAnterior;

  listPontosMalha LPontosCoordX,LPontosCoordY;
  tysetitSSuperficie SetPontosRadarInteresse,SetPontosNoRadar; 
  tylstUtm_Coord_3d LstPontosGeoRef;

  double Escala[2],           //--- Escala em pixels por metro; 
    EscalaDefault[2],         //--- Escalas padrão que permitem a visualização de todo o desenho.
    Deltas[2],                //--- Deslocamento nos eixos.
    DeltasDefault[2],         //--- Coordenadas padrão do canto inferior esquerdo na escala default
    DeltasIniTranslacao[2],   //--- Deltas do início da translacao;   
    CoordInferior,            //--- cota inferior do vídeo;
    AlturaVideo,              //--- Altura do video em metros. (Eixo z)
    LarguraVideo,             //--- Largura do vídeo em metros. (Eixo x)
    Resolucao[2],             //--- Resolucao do vídeo em pixels.
    ResolucaoXDiv2,           //--- Resolucao x / 2, usado nas transformacoes de coordenadas.   
    ResolucaoYDiv2,           //--- Auxiliar para desenho dos pontos
    DeltaYMetro,              //---  Idem, para facilitar os cálculos.
    Toleranciax,              //--- Tolerancias para procurar o ponto na posição 
    Toleranciay,              //--- Atual do mouse.
    RazaoHV,                  //--- Razão entre as escalas H e V.
    DeltasCarta[2],
    DeltasCN[2],
    XCentral,                 //--- coordenadas x do centro do vídeo
    YCentral;                 //--- coordenadas y do centro do vídeo

  CPoint InicioArrast,        //--- Usados nas operações de arrastar.
    FimArrast,                //--- Idem.
    InicioZoom,
    PonMousePixelsAtual,
    FimZoom,
    PAntMouse,                //--- guarda as coordenadas anteriores do ultimo objeto movido para poder apagar
    UltimoPontoPego,
    PontosControleGeoRef[4],
    PCotaDistancia;
	  
  Ponto DeltasReais,
        LimiteYMetros,
        PrimeiroPontoSecAvulsa,
        SegundoPontoSecAvulsa,
        PontoIntersecao,
        PontoSendoInserido,
        PontosLinhaBase[2],
        PontosMedir[2],
        FozSIG;

  unsigned int ObjetoAtual,ObjetoAnterior;

  CRect CRectJanPai;    //--- Crect da janela para, para saber as coordenadas iniciais da janela
  CSize TamLetra;

  tysetLembretes SetLembretes;

  ItSSuperficie DeltaSupSendoArrastada;             //--- Iterador para o ponto que está sendo arrastado 
  CDeltaSuperficie BackDeltaSupSendoArrastada;      //--- Back da deltasuperficie sendo arrastada (para desfazer)
  CurHorizontal BackCurvaSendoArrastada;            //--- Ponteiro para a curva horizontal sendo arrastada
  ItCMCurHorizontaisPI itUltimaCurvaArrastadaPI;    //--- Salva a última curva alterada para fazer o undo;
  ItCMCurHorizontaisCC itUltimaCurvaArrastadaCC;    //--- Salva a última curva alterada para fazer o undo;
  ItCLLDeltaSup ItLLDeltaSupMarcada,                //--- LDeltaSup marcada para remover
                ItLLEspigaoMarcado;                 //--- Espigão marcado para juntar ou editar talvegue principal
  ItSSuperficie	DeltaSupSobMouseAnterior,           //--- Ponto clicado anteriormente.
                DeltaSupSobMousePopup,              //--- Ponto sob o menu popup
                itPonMaisProximo,                   //--- Ponto da superficie mais proximo do mouse;
                ItInicioJuntandoEspigoes,           //--- Esboçando Objetos, marca o ponto inicial juntando espigoes
                ItFimJuntandoEspigoes;              //--- Esboçando Objetos, marca o ponto final.
  CDeltaSuperficie DeltaSupAntesArrasto;            //--- valor da Delta superficie antes de ser arrastada
  ItLLDeltaSup ItListaRestAtual,                    //--- Iterador para Lista com a restrição atual, a última inserida na lista de restrições
               ItListaIniTalvPrincipal;             //--- Iterador para a lista inicial (clicada) do talvegue principal
  ItSSetPontos ItPontoDesabilitado;                 //--- Ponto clicado que está desabilitado.
  ItSSetPontos ItPontoInseridoUsuario;              //--- Ponto clicado que foi inserido pelo usuário.
  ittysetLembretes ItLembrete;                      //--- Lembrete sobre o mouse  
  ItSSuperficie DeltaSupAnterior,                   //--- Salva o ponto anterior
                ItInicioSarjeta,
                ItFimSarjeta;
  ittymapEquipDrenagem ItEquiDrenagemMarcada;
  ittymapEquipDrenagem ItObraDrenagemAtual;
  lstPontos SimbologiaTerrap;
  lstlstPontos AciTopogAcad;
  ittylstBacias ItBaciaMarcada;
  CDeltaSuperficie DSInicioTalvPrincipal;

  CFont CurrierNewVertical;                //--- Fonte para escrita vertical
  CFont CurrierNewHorizontal;              //--- Fonte para escrita horizontal
  CFont CourierNewCN;

  CBitmap BandeiraBMP,BandeiraMaskBMP;

  CSecTipo SecoesTipo;
  CSecoesAvulsas SecoesAvulsas;
  CAlargamento Alargamentos;
  CAciTopografico AciTopogAtual;
  CAcidentesTopograficos AcidentesTopograficos;

  tysetRelCamAcadVUE SetRelCamACADVUE;

  CString NomeMapa,NomeArqPCTRL;

  //--- CPicture - Mapa ------------------------------------------------------

  CPicture	m_cPicture;
 // CxImage  Mapa;

  double m_nRatio;
  double m_nX,m_nY;
  int m_nSizeMode;
  int m_nLoadedImage;

  //-----------------------------------------------------------------
      
  //--- Usadas na inserção de pontos nas arestas dos acidentes topograficos

  ItCLDeltaSup ItCLDS;                   //--- Marca o ponto inicial do segmento da restrição sobre a qual o mouse está
  double Raio;                           //--- guarda a distancia para o ponto inicial do segmento
  itmapSecoesAvulsas ItSecaoAvulsaAtual;  //--- Guarda a seção avulsa sob o mouse, para remoção. 
  
  void CotaCN(double fi,Ponto Local,bool Inicio,double dCota,CDC* pDC,const CPen& PenaCotaMestra);
  void inline TransfPixelsXY(CPoint& Ponto,double& Coorx,double& Coory);
  CPoint& TransfXYPixels(double Coorx,double Coory,CPoint& Ponto)
  {
    //--- Mapeia as cordenadas que estão em metros para pixels.

    Ponto.x = ((Coorx - Deltas[X]) * Escala[X]);
    Ponto.y = (AlturaVideo - (Coory - Deltas[Y])) * Escala[Y] ;

    return Ponto;
  }
  void Zoom();
  void CalculaParametros();
  bool VerfPonto(double x,double y,int& TipoPonto);
  void SalvaUltimoPontoResPixels(const ItSSuperficie DeltaSuperficie,bool PontoTerraplanagem=false);
  void AtualizaAcTopog_Projeto(int AcTopog,int Proj) { bAciTopog = AcTopog ; bProjeto = Proj;}
  void DesenharApagar(int TipoObjeto,const CPoint& Local,void* Objeto = NULL);
  void DesenharApagar(int TipoObjeto,const CPoint& LocalAnterior = CPoint(0,0),const CPoint& LocalAtual = CPoint(0,0),void* Objeto = NULL,int OperacaoForcada = OPR_AMBAS,bool PrimeiroMovimento = false);
  void AcertaRestricoes(const CDeltaSuperficie* DeltaSupAAlterar,const CDeltaSuperficie* DeltaSupAlterada = NULL);
  void QuebraLista(LLDeltaSup& ListasAtuais,ItLLDeltaSup& itListaAtual,ItLDeltaSup itPontoRestricao, ItLLDeltaSup* ppItListaQuebrada=nullptr,int CasoEspecial=false);
  void DesenhaEspiral(const CurHorizontal& Curva,CDC* pDC);
  void DesenhaCircular(const CurHorizontal& Curva,CDC* pDC, ittymapCTrechos itT,bool Apagar = false);
  inline void IrradiaPonto(double Angulo,const Ponto& PInicial,double Distancia,Ponto& PCalcular,bool somar=true);
  inline void IrradiaPonto(double Angulo,double x,double y,double s,double Distancia,Ponto& PCalcular);
  inline void DesenharPonto(double x,double y,CDC* pDC,COLORREF Cor);
  void DesenhaProjeto(ItCMCurHorizontaisTS pitMCH,CDC* pDC,int OperacaoForcada = OPR_AMBAS,unsigned int Quantidade = 0XFFFF);
  void DesenhaSuperficie(ItSSuperficie DeltaSupInicial,ItSSuperficie DeltaSupFinal,CDC* pDC,CSuperficie* pSuperfTerrap = NULL); 
  bool VerificaTangente(const Ponto& PAtual,ItCMCurHorizontaisTS& ItCurvaAnterior,std::string* pTrechoAtual=NULL);
  void DefineParametros();
  void CriaFontes(bool DestruirAnteriores = false);
  void CriaFontesCN(bool DestruirAnteriores=false,double Angulo=0.0);
  void CalculaNovasDimensoes();
  void IconizaSecao();
  void IconizarRestaurarSecao(int Modo);
  void EsbocaEspiral(const CurHorizontal& Curva,int Ramo,CDC* pDC);
  void CriaMapaSecoesGeomTemp(const CurHorizontal& Curva,MSecaoGeom& MSecoesGeomTemp);
  void LeCoordSecoes();
  void GravaCoordSecoes();
  void RotacionaBordoseRestricoes(double Sin,double Cos,double Angulo);
  void RotacionaSecoes(double Seno,double Coseno,double AngRotacao);
  void RotacionaBordos(double Sin, double Cos, double Angulo, LLDeltaSup& ListaBordos);
  void RotacionaSecoesAvulsas(double Sin, double Cos, double Angulo);
  void DesenhaMalha(CDC* pDC);
  void OnAlterouAngulo(CDC* pDC);
  void OnAlterouAnguloQ1(CDC* pDC);
  void OnAlterouAnguloQ2(CDC* pDC);
  void OnAlterouAnguloQ3(CDC* pDC);
  void OnAlterouAnguloQ4(CDC* pDC);
  void OnArrastouDesenho();
  void OnArrastouDesenhoQ1();
  void OnArrastouDesenhoQ2();
  void OnArrastouDesenhoQ3();
  void OnArrastouDesenhoQ4();
  void CentralizarTela(const Ponto& PontoCentral,bool DesenharCruz = false);
  void DesenhaNorte(CDC* pDC);
  void DesenhaMalhaQ4(CDC* pDC);
  bool GravaAcidentesTopograficos();
  bool LeAcidentesTopograficos();
  void CriaPenaRestriçao(CPen *PenaObjeto,unsigned int Objeto);
  void LimpaListaRestricao(unsigned int Tipo){ Superficie.RemoveListasDoTipo(Tipo);}
  void SerializaTalveguePrincipal(int Tipo);
  void DesenhaPoste(const Ponto& PontoPoste,CDC* pDC);
  void DesenhaBueiro(const Ponto& PontoBueiro,CPen* Pena,CDC* pDC);
  void DesenhaArvore(const Ponto& PontoArvore,CDC* pDC);
  void DesenhaPVEsgoto(const Ponto& PontoPVEsgoto, CPen* Pena, CDC* pDC);
  void DesenhaPVEletrica(const Ponto& PontoPVEletrica, CPen* Pena, CDC* pDC);
  void DesenhaPVGas(const Ponto& PontoPVGas, CPen* Pena, CDC* pDC);
  void DesenhaSoleira(const Ponto& PontoSoleira, CPen* Pena, CDC* pDC);
  bool LePontosCotadosSuperf();
  void DesenhaPontosCotadosSuperf(CDC* ppDC);
  void DesenhaEspigaoAIncluir(CDC* ppDC);
  void DesenhaTalvegueMontado(CDC* ppDC);
  void DesenhaTalveguePrincipal(CDC* ppDC);
  bool VerfPontoPertenceLinha(tylstpontos& LinhaPontos, Ponto& Ponto);
  void DesenhaEspigaoMarcado(CDC* ppDC);
  void DefineTalveguePrincipalSIG(const Ponto& NascenteFoz);

  bool VerfObjAvulso(unsigned int pObjAtual) const 
  { 
    static tysetint MObjAvulsos = {
          CAciTopografico::POSTE,CAciTopografico::TAMPA_BUEIRO,CAciTopografico::ARVORE,
          CAciTopografico::CANTO_CASA,CAciTopografico::CANTO_CERCA,CAciTopografico::CANTO_MURO,
          CAciTopografico::PVELETRICA,CAciTopografico::PVESGOTO,CAciTopografico::PVGAS, 
          CAciTopografico::SOLEIRA};

        return MObjAvulsos.find(pObjAtual) != MObjAvulsos.end();
  ;};
  ItCLLDeltaSup VerificaNaTangenteRestricoes(ItCLLDeltaSup ItCLLDeltaSupInicial,const Ponto& PAtual,double Toleranciax,ItCLDeltaSup* pItCLDS=NULL,double* pRaio=NULL,CSuperficie* pSuperfTerrap = nullptr);
  void Ima(UINT nFlags,const Ponto& XYPonto);
  bool VerfObjCanto(int pObjAtual) const { return pObjAtual == CAciTopografico::CANTO_CASA || pObjAtual == CAciTopografico::CANTO_MURO || pObjAtual == CAciTopografico::CANTO_CERCA;};
//  bool VerfArestaForaSuperficie(const Ponto& P1,const Ponto& P2);
  void VaiParaPonto(const CPoint& Ponto);
  inline int PegaTipoPonto(const Ponto& Ponto,bool Terrap = false) const
  {
    if (!Terrap)
    {
      if (Ponto.Descricao.Find("Inserido Usuário") != -1) return OBJ_PONINSUSUARIO;
      else if (Ponto.Descricao.Find("Inserido Delaunay") != -1) return OBJ_PONINSDELAUNAY;
      else if (Ponto.Descricao.Find("Inserido Delaunay-Gabriel") != -1) return OBJ_PONINSGABRIEL;
      else if (Ponto.Baliza2 == OBJ_ESBOCO) return OBJ_ESBOCO;
        return OBJ_PONTERRENO;
    }

    if (Ponto.Descricao.Find("GRADIE") != -1) return OBJ_GRADITERRAP;

    return OBJ_TERRAPLAN;
  }
  inline void DesPontosGeomView::CalculaCota(const Ponto& PAnterior,const Ponto& PPosterior,Ponto& PInserir,double Dist)
  {
    double DistTotal(hypot(PPosterior.x - PAnterior.x,PPosterior.y - PAnterior.y));

    PInserir.z = (PPosterior.z-PAnterior.z)/ DistTotal * Dist + PAnterior.z;
  }

 void InserePontoNaRestricaoMarcada(ItCLLDeltaSup ItCLLDeltaSupMarcada,Ponto& PInserir,bool CalcularCota);
 bool RemovePontoRestricoes(const CDeltaSuperficie& DSRemover);
 void SalvaLembretes();
 void LeLembretes();
 bool RemovePontoRestricao(ItLLDeltaSup& ItLRestricao,ItLDeltaSup ItPonto);
 void RotacionarLinhaBase();
 void DesenharCruzPonto(const Ponto& PCentral,CDC* ppDC=NULL,int TipoPontoPixels = 0);
 ItCLLDeltaSup VerfClickRestricao(Ponto& PAtual, CSuperficie* pSuperfTerrap = NULL);
 void DesenhaCerca(const Ponto& PInicial,const Ponto& PFinal,CDC* ppDC);
 void DesenhaMuro(const Ponto& PInicial,const Ponto& PFinal,CDC* ppDC);
 void DesenhaLinhaFerrea(const Ponto& PInicial,const Ponto& PFinal,CDC* ppDC);
 bool VerfRemocaoPontoRestricao(ItLLDeltaSup& ItLRestricao,ItLDeltaSup ItPonto);
 double CalculaAreaSuperficie();
 bool VerfMudancaTipoAcitopog(ItCLLDeltaSup& LLDS,int Tipo=CAciTopografico::NENHUM_ACITOPOG);
 void VisualizarLogFronteira();
 inline void SeparaPontosInteresse(double Px,double Py);
 //void OnTimer(UINT_PTR nIDEvent);
 void CriaMapaChaves();
 tlstwstr& PegaTokens(CString& Descricao);
 ItSSuperficie* PegaPontoMaisProximoTerrap(Ponto PMouse);
 int AcertaMapa();
 int AjustaOffSTerraplanagem();
 void onLButtondownEsbocandoObj(UINT nFlags,Ponto PMouse);

 void RemovePerfilAvulso(itmapSecoesAvulsas ItSecRemover)
 {
   if(AfxMessageBox("Confirma a remoção deste perfil avulso?", MB_YESNO | MB_ICONEXCLAMATION) != IDYES) return;
   
   SecoesAvulsas.RemoveSecaoAvulsaMapa(ItSecRemover);
   AlterouSecoesAvulsas = AlterouProjeto = true;
 };

 bool OnMouseMoveTerrap(Ponto& PSobMouse,POINT& PonScreenCoor,CPoint& point);
 bool OnLButtonDownTerr(CPoint& point);
 void LeArqTPR(CString& NomeProjetoAtual);
 void DesenhaAlinhamentosTerrapl(CDC* ppDC);
 void PulaBrancos(std::ifstream& Stream);
 inline void PegaProximoAlfaNum(std::ifstream& Stream);
 bool LeCotasVermelhas();
 void DesenhaCN(CSuperficie* pSuperficie, CDC* pDC,bool simbologia = true);
 void DesenhaRestricoes(CSuperficie* pSuperficieAtual, CDC* pDC, CPen* pPenaRestricoes,int TipoSuperf,bool ParaTerraplanagem = false);
 int CalculaPixelspRef();
 int CalculaUTMpRef(int Origem = 0);
 void AcertaCotasPontosMedidos();
 int DefineFronteiraDireto(int TipoSuperf = SUPERF_TRADICIONAL);
 int CalculaSuperficie(CProgressCtrl* pProgress = nullptr,int TipoSuperf = SUPERF_TRADICIONAL);
 void IrParaPonto(CPoint& Ponto);
 void CalculaCotaPonto(Ponto& pPonto,bool Desrotacionar=false);
 void DefineEquipDrenAlinhamento();
 void GeraSecoesTerraplEstaca();
 void DesenhaEquiDrenagem(CDC* pDC, ittymapEquipDrenagem* pEquipADesenhar=nullptr);
 void DefineAlinEquipDrenagem(CEquipDrenagem& EquipDrenagem);
 bool VerificaEquipDrenagem(CPoint point, Ponto PAtual, CRect Rect, CMenu* pmenu=nullptr);
 bool VerificaNaTangenteEquipDrenagem(ittymapEquipDrenagem& It, const Ponto& pAtual, double ToleranciaX, itsetPontos** ppItPontoPego=nullptr);
 void CalculaPoligonalTraco(citlstitsetPontos it, tylstpontos& LstPontosDesenho, double Afast, citlstitsetPontos itFim,double Escon=0.0);
 void CalculaPontosBacias(citlstitsetPontos it, tylstpontos& LstPontosDesenho, double Afast, citlstitsetPontos itFim,double AlturaBueiro,double Escon=0.0,double AngAla=Matem::M_PI_4);
 void CalculaCotasAlinhamento(lstPontos& Alinhamento);
 void DefineCanaleta(ItLLDeltaSup ItLstPontos, std::string& NomeCanaleta);
 void DefineSarjetaTN(ItLLDeltaSup ItLstPontos,std::string& NomeSarjeta);
 void DefineBueiro(ItLLDeltaSup ItLstPontos, std::string& NomeBueiro);
 void DefineBueiroRetangular(ItLLDeltaSup ItLstPontos, std::string& NomeBueiro);
 void DefineDescidaDagua(ItLLDeltaSup ItLstPontos, std::string& NomeDescida);
 void DefineCaixaColetora(std::string& NomeDescida,ItSSuperficie& ObjetoLigado, ittymapEquipDrenagem ItAgregado,ItLLDeltaSup& pListaAtual);
 void DefineBaciaAmortecimento(std::string& NomeBacia, ItSSuperficie& ObjetoLigado, ittymapEquipDrenagem ItAgregado, ItLLDeltaSup& pListaAtual);
 void EditarSarjeta();
 void DefineTubulacao(ItLLDeltaSup ItLstPontos, std::string& NomeSarjeta);
 void RotacionaLPontos(itlstPontos& itInicio, citlstPontos& itFim, double Angulo, lstPontos& LRotacionado);
 void TransladaLPontos(itlstPontos& itInicio, citlstPontos& itFim,const Ponto& De,const Ponto& Para);
 std::pair<double,bool> VerfEsconsidade(const Ponto& PInicio,const Ponto& PFim, double& EstacaEixo,Ponto& CoordEixo);
 bool InterRetaEspiralAproxNewton(const CurHorizontal& CurvaAtual, const Ponto& PReta1, const Ponto& PReta2, double LimiteInf, double LimiteSup, Ponto& Inters,int RamoEspiral);
 void CalculaPontoEspiral(const CurHorizontal& CurvaAtual, double Dist, Ponto& PCalculado, int RamoEspiral);
 double CalculaEsconsidadeCurva(Ponto PInicio, Ponto PFim, ItCMCurHorizontaisTS pitMCH);
 bool PegaPontoMaisProximoTangente(Ponto& P1, Ponto* P2);
 void RedefineEquipDrenagem(ittymapEquipDrenagem ItBueiro,Ponto& NovoLocal);
 void TrocaPontoEquipDrenagem(ittymapEquipDrenagem ItEquipAtual,const Ponto& NovoLocal);
 bool VerificaPontoDrenagem(const Ponto& Pon, ittymapEquipDrenagem* ItAgregado);
 bool VerfPontoOutroEquipamento(const Ponto& Pon);
 ittylstBacias VerfClicouBaciaHidrografica(ItCLLDeltaSup ItEspigao);
 ittylstBacias VerfClicouTalvegueBacia(ItCLLDeltaSup ItEspigao);
 void DesenhaAciDrenagemMarcada(CDC* pDC);
 void DefineSecAvulsaBueiro(CEquipDrenagem& Bueiro,bool Invertido);
 void forca90GrausBueiro(ittymapEquipDrenagem ItBueiro);
 bool CalculaCotaPontoCN(Ponto& pPonto);
 bool GravaArquivoCN(tylstlstUtm_Coord_3d& LstLstCoorCN);
 int AjustaDeclinacao();
 void AjustaTranslacao();
 void FinalizaTranslacao(bool ViaTeclaEnter=false);
 int LeRelCamAcadVUE(const std::string& NomeProjeto);
 int LeAciTopogAcad(const std::string& NomeArquivo, std::string& NomeArqTRian, std::string& NomeArqAciTopog);
 int ImportaAciTopogAcad(const std::string& NomeArquivo,CProgressCtrl& Progresso);
 int GravaArqResAcad();
 ItLDeltaSup FindListas(LDeltaSup& ListaProcurar, const Ponto& pPonto, double Precisao=1e-3);
 void LeSetForcarAresta();
 bool CalculaCotaPontoSuperf(Ponto& PontoSemCota);
 bool VerfPontoOutroEspigao(const Ponto& Pon);
 void MontaTalveguePrincipal();
 void DesenhaTalveguePrincipalBacia(CBacia& Bacia, CDC* ppDC);

 bool DesPontosGeomView::PontoXYIgual(const Ponto& P1,const  Ponto& P2, double Precisao = 1e-3)
 {
   return (fabs(P1.x - P2.x) < Precisao && fabs(P1.y - P2.y) < Precisao);
 }

 void CotaPontosSemCota();

 void PassaDescAciTopogAcadSuperf();
 void DesenhaPontosProcurados(CDC* ppDC);
 bool LeArqPCTRL(std::string& NomArq, void* ppLstPCT);

protected:

  DesPontosGeomView(const std::string& pTrecho="");           // protected constructor used by dynamic creation
  ItSSuperficie PegaPontoMaisProximo(Ponto PMouse, ItSSetPontos* pItPontoDesabMaisProximo=nullptr, ItSSetPontos* pItPontoInsMaisProximo=nullptr,Ponto** pfozBacia=nullptr, ittymapEquipDrenagem* pOAC=nullptr, ItSSuperficie* pItPTerrap=nullptr, ItSSetPontos* pItPontoSolto=nullptr, itlstPontos* ItPontoProcurado=nullptr,int Forcar=false);//,ItSSetPontos* pItPontoPISMaisProximo=NULL);
  ItCMCurHorizontaisPI* PegaPIMaisProximo(CurHorizontal& CurvaHorizontal,std::string& Trecho);
  ItCMCurHorizontaisCC* PegaCCMaisProximo(CurHorizontal& CurvaHorizontal,std::string& Trecho);
  const ittysetLembretes PegaLembreteMaisProximo(const Ponto& PontoLembrete,double Toleranciax);
  void  GetLine(std::ifstream& ifs, std::string& Buffer, const std::string& strDelim);
  void CalculaCotasTalveguesSIG();
  void SuavizaEspigao(lstPontos* plstEspigao);
      
  DECLARE_DYNCREATE(DesPontosGeomView)

  // Attributes
public:

  CMapaCNIBGE MapaIBGE;
  tysetPontosSoltosCArestaDGN SetArestas;

  // Operations
public:

  bool Destruindo;

  void MudarEscala(double Fator,double zDelta = 0.0);
  void MudarRazao(double Fator);
  std::string PreparaDescricao(const void* pObjeto,bool ArrastandoPonto,int OBJETO,std::string* pRua = NULL); 
  bool PegaSecoesIconizadas() {return  SecoesIconizadas ;};
  bool SuperfAlterada() { return AlterouProjeto || AlterouTerreno || AlterouSecoesAvulsas ;};
  bool GravarSuperficie();    
  void GravaLog(const std::string& strMsg,std::ostream& Stream,bool ForcarHora=false);
  void PassaFronteiraParaListaPropria();  //--- Para compatibilidade com versões anteriores
  void Calcula();
  bool is_balanced(std::string expression); 
  void Bits();
  void LeArquivosBordos();
  double PegaAngRotacaoSuperf() { return Superficie.PegaAnguloRotacao(); };
  void Desrotacionar() 
  { 
    AnguloRotacaoAnterior = Superficie.PegaAnguloRotacao();
    ZerarAzimute = true;
    OnButrotacionar();
  };
  void Rotacionar(double Angulo);
  void RotacionaSecoesTerrap(double VUx, double VUy, double Angulo)
  {
    tysetSecoesTerrap SecoesTerrrapAnt(SecoesTerraplanagem);
    SecoesTerraplanagem.clear();

    for (ittysetSecoesTerrap It = SecoesTerrrapAnt.begin() ; It != SecoesTerrrapAnt.end() ; ++It)
    {
      CSecaoTerrap ST(*It);

      ST.Rotaciona(VUx, VUy,Angulo);

      SecoesTerraplanagem.emplace(ST);
    }
   }

  void RotacionaSimbologia(double VUx, double VUy, double Angulo)
  {
    for (itlstPontos it = SimbologiaTerrap.begin(); it != SimbologiaTerrap.end(); ++it)
    {
      it->RotacionaPlano(VUx, VUy, Angulo);
    }
  }

  void MudouRua(const std::string& NovaRua);

  void InsereRuaRecemCriada(const std::string& ProjBase, const std::string& NovaRua);
  bool GeoReferenciado() { return ProjetoGeoreferenciado; }
  void InicializaCroquis();
  int PegaNomesArquivosPIS(tysetNomeArquivos& setNomeArquivos, CString NomeProjeto);
  void AlternaDesenharCroquis() { DesenharCroquis = !DesenharCroquis; }
  void ConfinaCursorAreaCliente()
  {
    CRect WinRect(0, 0, 0, 0);
    GetWindowRect(&WinRect);
    WinRect.DeflateRect(2, 2);
    ClipCursor(WinRect);   //--- confina o cursor na área cliente (-2 pixels)
  }
  void RemoveRua(const std::string& NomeRua);
  bool LeArqSimbologia(lstPontos& SimbologiaTerrap, const std::string& NomeProjeto);
  CCamadaDGN& PegaCamadaDGN() {return this->CamadaDGN;};
  void Rotaciona(double pNovoAngulo,bool pAbsoluto_Relativo);
  
  bool VerfAciUmPonto(int AciTopog)
  {
    return (AciTopog == CAciTopografico::POSTE || AciTopog == CAciTopografico::TAMPA_BUEIRO || AciTopog == CAciTopografico::ARVORE ||
            AciTopog == CAciTopografico::PVELETRICA || AciTopog == CAciTopografico::PVESGOTO || AciTopog == CAciTopografico::PVGAS ||
            AciTopog == CAciTopografico::SOLEIRA);
  }

  bool IniciaJuntandoEspigoes_2(ItCLLDeltaSup ItListaProcurar);
  bool FinalizaJuntandoEspigoes_2(lstItCLLDeltaSup LstItListaProcurar, ItSSuperficie pDeltaSupsobMouse);
  void ImportaArqTalvegues(std::string& NomeProjeto);
  bool PegaFozSIG(LDeltaSup& LstEspigao,ItLLDeltaSup ItTalvegueFoz,Ponto& pFozSIG);

public:
  virtual void OnInitialUpdate();
protected:
  virtual void OnDraw(CDC* pDC);      // overridden to draw this view
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
  //}}AFX_VIRTUAL

  // Implementation
protected:
  virtual ~DesPontosGeomView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(DesPontosGeomView)
  afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnEnquadrar();
  afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
  afx_msg void OnRedesenhar();
  afx_msg void OnUpdateGravar(CCmdUI* pCmdUI);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
  afx_msg void OnButrotacionar();
  afx_msg void OnUpdateButrotacionar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateRedesenhar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateEnquadrar(CCmdUI* pCmdUI);
  afx_msg void OnDefinebordos();
  afx_msg void OnSetFocus(CWnd* pOldWnd);
  afx_msg void OnAcitopog();
  afx_msg void OnDefAciTopog(); 
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
  afx_msg void OnConfdes();
  afx_msg void OnSecsemprevisIconizar();
  afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
  afx_msg void OnGravar();
  afx_msg void OnIrparaponto();
  afx_msg void OnAlternartituloDesGeom();
  afx_msg void OnButFechar();
  afx_msg void OnCalcgeometrico();
  afx_msg void OnSecaoAvulsa();
  afx_msg void OnTipoCN();
  afx_msg void OnBordos();
  afx_msg void OnNaoPermitirAlteracoes(); 
  afx_msg void OnSincronizarSecoes();
  afx_msg void OnUpdateButSincro(CCmdUI* pCmdUI);
 //afx_msg void OnUpdateHabilitar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButProjTerrap(CCmdUI* pCmdUI);
  afx_msg LRESULT FechouToolBar(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT AmpliarMapa(WPARAM wParam, LPARAM lParam);
  afx_msg LRESULT MostraListaPontos(WPARAM WP, LPARAM LP);
  void Teste();
  afx_msg void OnSincPontosControle();
  afx_msg void OnUpdateButSinPontosCtrl(CCmdUI* pCmdUI);
  afx_msg void OnBaixarMapa();
  afx_msg void OnUpdateBaixarMapa(CCmdUI* pCmdUI);
  afx_msg void OnButAmpliarMapa();
  afx_msg void OnUpdateButAmpliarMapa(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButRemoverGeoRef(CCmdUI *pCmdUI);
  afx_msg void OnButMedir();
  afx_msg void OnButVisCroquis();
  afx_msg void OnUpdateButMedir(CCmdUI *pCmdUI);
  afx_msg void OnUpdateButVisCroquis(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButradar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButPonIns(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButSecAvulsa(CCmdUI* pCmdUI);
  afx_msg void OnUpdateImportarAciAutoCad(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButAciTopog(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButEsbocar(CCmdUI* pCmdUI);
  afx_msg void OnUpdateButFronteira(CCmdUI* pCmdUI);
  afx_msg void OnUpdateTalveguePrincipal(CCmdUI* pCmdUI);
  afx_msg void OnUpdateImportarBaciasHidrog(CCmdUI* pCmdUI);
  afx_msg void OnUpdateImportarTalvegues(CCmdUI* pCmdUI);
  afx_msg void OnTalveguePrincipal();

  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
  virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
  afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
  virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
  afx_msg void OnButfronteira();
  afx_msg void OnDesMuro();
  afx_msg void OnDesenhar();
  afx_msg void OnEsbocar();
  afx_msg void OnDrenagem();
  // afx_msg void OnCantocasa();
  afx_msg void OnNa();
  afx_msg void OnNaPeriodico();
  //afx_msg void OnRocha();
  afx_msg void OnDesMeiofio();
  afx_msg void OnDesNa();
  afx_msg void OnDesRocha();
  afx_msg void OnDesBueiro();
  afx_msg void OnDesRedeeletricaAt();
  afx_msg void OnDesRedeeletricaBt();
  afx_msg void OnDesBordo();
  afx_msg void OnDesCerca();
  afx_msg void OnDesPonte();
  afx_msg void OnDesPoste();
  afx_msg void OnDesFronteira();
  afx_msg void OnDesFiltrar();
  afx_msg void OnButponins();
  afx_msg void OnDeseixo();
  afx_msg void OnDesCrista();
  afx_msg void OnDesPe();
  afx_msg void OnDesCantoCasa();
  afx_msg void OnDesCantoCerca();
  afx_msg void OnDesCantoMuro();
  afx_msg void OnDesCasa();
  afx_msg void OnDesPVEletrica();
  afx_msg void OnDesPVEsgoto();
  afx_msg void OnDesPVGas();
  afx_msg void OnDesRedeGas();
  afx_msg void OnDesRedeEsgoto();
  afx_msg void OnDesSoleira();
  afx_msg void OnDesRedeEletrica();
  afx_msg void OnUpdateDesFronteira(CCmdUI *pCmdUI);
  afx_msg void OnDesTampaBueiro();
  afx_msg void OnDesLinhaFerrea();
  afx_msg void OnPopupDesabilitar();
  afx_msg void OnPopupHabilitar();
  afx_msg void OnPopupIncluirPonto();
  afx_msg void OnPopupSair();
  afx_msg void OnUpdatePopupDesabilitar(CCmdUI *pCmdUI);
  afx_msg void OnUpdatePopupHabilitar(CCmdUI *pCmdUI);
  afx_msg void OnUpdatePopupIncluirPonto(CCmdUI *pCmdUI);
  afx_msg void OnPopupRemoverPonto();
  afx_msg void OnUpdatePopupRemoverPonto(CCmdUI *pCmdUI);
  afx_msg void OnButLembrete();
  afx_msg void OnUpdateButtrocaracitopog(CCmdUI *pCmdUI);
  afx_msg void OnPopupRemover();
  afx_msg void OnPopupTrocar();
  afx_msg void OnPopupEditar();
  afx_msg void OnUpdatePopupEditar(CCmdUI *pCmdUI);
  afx_msg void OnButarrastar();
  afx_msg void OnButRadar();
  afx_msg void OnUpdateCalcgeometrico(CCmdUI *pCmdUI);
  afx_msg void OnDesAlagado();
  afx_msg void OnDesAlinhamento();
  afx_msg void OnDesArvore();
  afx_msg void OnDesFloresta();
  afx_msg void OnDesMata();
  afx_msg void OnDesPlantacao();
  afx_msg void OnDesRavina();
  afx_msg void OnDesEquiBueiro();
  afx_msg void OnDesEquiBueiroRetangular();
  afx_msg void OnDesEquiSarjetaOff();
  afx_msg void OnDesEquiSarjetaTN();
  afx_msg void OnDesDescidaDagua();
  afx_msg void OnDesCaixaColetora();
  afx_msg void OnDesBaciaAmortecimento();
  afx_msg void OnDesEquiSarjeta();
  afx_msg void OnDesEquiTubulacao();
  afx_msg void OnDesEquiDrenoProfundo();
  afx_msg void OnDesEquiCanaleta();
  afx_msg void OnDesEquiCanaletaOff();
  afx_msg void OnDesMovimentarEquiDrenagem();
  afx_msg void OnTimer(UINT_PTR nIDEvent);
  afx_msg void OnButProjTerrap();
  afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnRemovergeoreferenciamento();
  afx_msg void OnBnClickedButpegararqnuvem();
  afx_msg void OnBnClickedButpegararq();
  afx_msg void OnCotarCNs();
  afx_msg LRESULT OnClicouCroquis(WPARAM WP, LPARAM LP);
  //afx_msg void OnPopupAreaPerim();
  //afx_msg void OnPopupAreaPerimetro();
  afx_msg void OnPopupInserirPonto();
  afx_msg void OnUpdatePopupInserirPonto(CCmdUI* pCmdUI);
  afx_msg void OnPopupAreaPerimetro();
  afx_msg void OnDesTalvegue();
  afx_msg void OnDesEspigao();
  afx_msg void OnPopupAreaPerimetroAci();
  afx_msg void OnUpdatePopupAreaPerimAci(CCmdUI* pCmdUI);
  afx_msg void OnPopupDefinirBacia();
  afx_msg void OnUpdatePopupDefinirBacia(CCmdUI* pCmdUI);
  afx_msg void OnPopupEditarDrenagem();
  afx_msg void OnPopupRemoverDrenagem();
  afx_msg void OnPopupDrenagemSair();
  afx_msg void OnUpdatePopupRemover(CCmdUI* pCmdUI);
  afx_msg void OnUpdateCotarCNs(CCmdUI* pCmdUI);
  afx_msg void OnUpdateMenucotarcn(CCmdUI* pCmdUI);
  afx_msg void OnPopupCotarcn();
  afx_msg void OnPopupSaircn();
  afx_msg void OnPopupPontoSair();
  afx_msg void OnPopupDescPonto();
  afx_msg void OnImportarAciAutoCad();
  afx_msg void OnUpdateOnDrenagem(CCmdUI* pCmdUI);
  afx_msg void OnSelecionarCNs();
  afx_msg void OnMenuremoverbaciahidrol();
  afx_msg void OnMenueditarbaciahidrol();
  afx_msg void OnMenuremoverespigao();
  afx_msg void OnUpdateMenuremoverbaciahidrol(CCmdUI* pCmdUI);
  afx_msg void OnMenubaciasair();
  afx_msg void OnImportarBaciasHidrog();
  afx_msg void OnPopupIncTalvPrincipal();
  afx_msg void OnUpdatePopupIncTalvPrincipal(CCmdUI* pCmdUI);
  afx_msg void OnPopupExcTalvPrincipal();
  afx_msg void OnUpdatePopupExcTalvPrincipal(CCmdUI* pCmdUI);
  afx_msg void OnPopupCriarTalvPrincipal();
  afx_msg void OnUpdatePopupCriarTalvPrincipal(CCmdUI* pCmdUI);
  afx_msg void OnPopupFinTalvPrincipal();
  afx_msg void OnImportarTalvegues();


};
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DESPONTOSGEOMVIEW_H__68799681_1095_11D8_BE61_826A873C8818__INCLUDED_)
