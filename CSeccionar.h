struct ltPerfil 
{ 
  bool operator()(const Perfil* p1, const Perfil* p2) const 
  { 
    return p1->Estaca.EstVirtual < p2->Estaca.EstVirtual;
  }
};

typedef std::set<Perfil*,ltPerfil> tysetpPerfis;
typedef tysetpPerfis::iterator ittysetpPerfis;

class CSeccionar
{
  CString NomeProjeto;
  CDialog* pDialogo;
  HANDLE EventoFim;
  CString Observacao;
  CSuperficie Superficie,                                 //--- Para Restrições
              SuperficieTerrap;                           //--- Para perfil projeto das secoes avulas   
  CSecoesAvulsas PerfisAvulsos;
  CString NomeTrecho;

  bool Otimizar,TodosTrechos,TemArqTriaSuperf;
  double douDistMinima,douIncMinima;

  void CalculaPontosExtremos(const Ponto& PontoEixo,Ponto& PEsq,Ponto& PDir);
  void Interpola(setPontos& SSetxSecao,Ponto& PCalc);      //--- Insere o ponto em x na lista
  //void CriaSetTerrenoAnterior();
  void CriaSetTerrenoAnterior(CLPerfis& TerrenoAnterior, tysetpPerfis& SetTerrenoAnterior);
  void PassaTerrenoAnteriorSet(ittysetpPerfis,setPontos& SSetxSecao);
  bool LeAcidentesTopograficos();
  void DefinirPerfisAvulsos(int& ContaPerfAvulsos,bool Forcar);
  void InsereAcidentesTopograficos(CSuperficie& pSuperFicie,CDialog* pDialogo,Ponto& pEsq,Ponto& pDir,CLPontos& AciTopograficos,int TipoSecao,Ponto* pEixo);
  void GravaLog(const std::string& strMsg, std::ostream& Stream, bool ForcarHora=false);
  void AcertaOffSets(CSecaoAvulsa& SecAvAtual);
  void LimpaSecao(setPontos& SetXPontos);
  void CalcCotaSuperf(Ponto& Ponto);
  
public:
	CSeccionar(CString &NomeProj,CDialog* Diag,CEvent* Evento=NULL,bool pOtimizar=false,double pDistMinima=INFINITO,double pInclMinima=INFINITO,bool pTodosTRechos = false);

	virtual ~CSeccionar();

  bool BackupArquivos();
  bool VoltaBackupArquivos();
  bool RemoveBackupArquivos();
  bool Iniciar(bool bTodas);
};


