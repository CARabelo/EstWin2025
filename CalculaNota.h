
typedef std::set<CSecao> setCSecao;
typedef setCSecao::iterator itsetCSecao;
typedef setCSecao::const_iterator itcsetCSecao;

typedef std::map<CEstaca, Ponto> MSecaoGeom;

typedef std::list<Ponto> lstPontos;
typedef lstPontos::iterator itlstPontos;
typedef lstPontos::const_iterator citlstPontos;

//--- Mapa para os pontos da seção de projeto com cota 

typedef std::map<Ponto, lstPontos> tymapEstlstPontos;
typedef tymapEstlstPontos::iterator ittymapEstlstPontos;
typedef std::pair<ittymapEstlstPontos, bool> pairtymapEstlstPontos;

class CalculaNota								
{
  enum {ESQUERDO,DIREITO};

  double SE,SL,E,IAnte,IPost,CVsAnteriores[2],EstacaAnterior, CotaAnterior;

  CString& NomeProj;
  CDialog* Dialogo;

  CCurHorizontais CurvasHorizontais;
  CCurVerticais CurvasVerticais;
  CGradiente Gradientes;
  CAlargamento Alargamentos;
  CSecTipo SecoesTipo;
  CEstacasACalcular EstacasACalcular;
  CLPerfis Terreno;
  CTaludesPavim TaludesPavimento;
  CTaludesCorte TaludesCorte;
  CTaludesAterro TaludesAterro;
  CHPavimento HPavimento;
  CCanteiros Canteiros;
  CPistasFixas PistasFixas;
  CCanteiros CanteirosCentrais;
  CListaSecoes Secoes;
  CHPavimento HLimpeza;
  CPerfilGeologico PerfilGeologico;
  MSecaoGeom SecoesGeometricas;
  tymapEstlstPontos PontosTerrplProjeto;
  lstPontos PontosPassagem[2];

  HANDLE EventoFim; 

  bool AlterouTerreno,bSoCotas;

  void AplicaSuperElevacao(Perfil& SecaoAtual,double Gradiente);
  void AplicaSuperLargura(Perfil& SecaoAtual);
  void GravaSecao();
  //void InsereGradiTerreno(Perfil& TerrenoAtual,double XGradiente);              //--- Insere no terreno o ponto onde x = gradiente
  void PegaPontosInseridos(Perfil& PontosInseridos,POSITION POSSecaoAtual);
  double Limpeza,ExessoLateral;
  void AplicaLimpeza(CEstaca& Estaca,Perfil& TerrenoAtual,double Limpeza,double ExessoLateral);
  void RemovePontosEditadosProjeto(CSecao &SecAtual,const Perfil& ProjetoEditado);      //--- Se for para manter a seção tipo editada
                                                                                        //--- Os pontos movidos estão no lugar de origem, então são removidos
//  void GravaPerfilGeologico(lstStDadosPerfGeol& LstPerfGeologicos);
  void RecriaPerfisGeologicos();
  void CalculaPontosTerraplProjeto(CSecao& SecAtual);
  bool AbreArquivoCalcGeomDetalhado(const CString& NomeProjeto);
  bool GravaPontosTerraplProjeto(const CString& NomeProjeto);
  void GravaPontosPassagem();
    
public:
  void MontaSecoes();

  CalculaNota(CString &NomeProjeto,CDialog* Diag,bool bSoCotas,CEvent* Evento,CProjeto* CProj);

  void AcertaLista(const setCSecao& setSecoesCalculadas);
  //void LimpaTerreno(Perfil& TerrenoAtual,double HLimp);
};