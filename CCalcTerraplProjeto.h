typedef std::list<Ponto> lstPontos;
typedef lstPontos::iterator itlstPontos;
typedef lstPontos::const_iterator citlstPontos;

//--- Mapa para os pontos da seção de projeto com cota 

typedef std::map<Ponto, lstPontos> tymapEstlstPontos;
typedef tymapEstlstPontos::iterator ittymapEstlstPontos;
typedef std::pair<ittymapEstlstPontos, bool> pairittymapEstlstPontosBool;
typedef std::pair<Ponto,lstPontos>  pairittymapEstlstPontos;

typedef std::list<Ponto> tylistcponto;
typedef tylistcponto::iterator ittysetcponto;

typedef std::list<cittymapCAlinhamentos> tysetcittymapCAlinhamentos;
typedef tysetcittymapCAlinhamentos::iterator ittysetcittymapCAlinhamentos;

struct ltCString
{
  bool operator()(const CString S1, const CString S2) const
  {
    return S1.Compare(S2) < 0;
  }
};

class CCalcTerraplProjeto
{
  tymapEstlstPontos MapaPontosTerrlProjeto;
  std::string NomeProjeto, NomeProjetoBasico;
  LDeltaSup LFronteiraSuperf;
  lstPontos PontosPassagem[2];
  CHPavimento Limpeza;
  double AnguloRotacao;
  int TipoProjeto,Erro;
 
  bool LePontosTerraplProjeto(const std::string& pNomeProjeto);
  std::set<std::string> SetAlinhamentosVariaveis;

  ittymapAlinhamentosAtivos PegaAlinhamentoPorDescricao(const std::string& pDescricao);
  int VerfProjTerrap(CSuperficie& SuperfTerrpl, SSetPontos& PontosProblema, std::ofstream& ArqLog,double AreaTerrap);
  int VerfFrontDentroTerraplanagem(const LDeltaSup& FronteiraSuperficie, const tylstpontos& FronteiraTerraplanagem, SSetPontos& PontosProblema,double Area);
  int VerfFrontCruzTerraplanagem(const LDeltaSup& FronteiraSuperficie, const tylstpontos& FronteiraTerraplanagem, SSetPontos& PontosProblema, CSuperficie& SuperficieTerrap);
  int VerificaOffSets(SSetPontos& PontosProblema);
  void GravaLogPontosProblema(SSetPontos& PonProblema, std::ostream& Stream, CSuperficie& Superf, bool ForcarHora=false,bool Estaca=false);
  void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora = false);
  void InserePontosPassagemBordos();
  void MudaTipoListaFronteira(tylstpontos& LstFronteira);
  int AjustaOffSTerraplanagem();  //--- EM DESENVOLVIMENTO
  void DefineSimbologiaTerrap(const CAlinhamentos& Alinhamentos,int Densidade);  //--- EM DESENVOLVIMENTO
  void DefineTracosSimbologia(const Ponto& PAnteL1, const Ponto& PProxL1, const Ponto& PAnteL2, const Ponto& PProxL2, lstPontos& SimbolTerrap, int Tipo,int Lado,bool Reiniciar,int Densidade);
  bool GravaArqSimbologia(lstPontos& SimbologiaTerrap);
  cittymapCAlinhamentos ProcuraProxAlinhamento(cittyplstPontos itEstacaAtual, cittymapCAlinhamentos AlinAtual,int pLado);
  cittymapCAlinhamentos VerfEntrouAlinhamento(cittyplstPontos itEstacaAtual, cittyplstPontos citPAtualL1, cittymapCAlinhamentos AlinAtual,bool Pe);
  void CalculaCNSimbologia(CSuperficie& pSuperfTerrap);
  void FechaExtremosTerrap(CSuperficie& SuperficieTerrap);

  CProgressCtrl& CtrlProgresso;
  CStatic& CstaTexto; 

public:

  CAlinhamentos Alinhamentos;
  
  CCalcTerraplProjeto(const std::string& NomeProjeto,CProgressCtrl& Progresso,CStatic& Texto,std::string& NomeProjetoBase,int TipoProjeto = -1);
  ~CCalcTerraplProjeto();
  int CalculaTerraplProjeto(int ProjGeoReferenciado,int Densidadesimbologia,int TipoProjeto);
  int DefineFronteiraTerr(double& AreaTerrap);
  int Gravar();
  int LePontosPassagem();

};

