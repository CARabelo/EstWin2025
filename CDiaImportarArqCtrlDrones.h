//--- Estes sets são usados na rotinas de ligar CNs partidas. É preciso guardar todos os inicios e
//--- fins das listas das CNs para poder emendá-las.

class plstitini
{
  public:

  ittylstlstUtm_Coord_3d itLista;
  Utm_Coord_3d itIni;
  int Usada;

  plstitini::plstitini(ittylstlstUtm_Coord_3d pitLista, Utm_Coord_3d& pIni): itLista(pitLista),itIni(pIni),Usada(0)
  {
  }

  bool operator<(const class plstitini& RHS) const
  {
    return itIni < RHS.itIni;
  }

  const plstitini& operator=(plstitini& RHS)
  {
    itLista = RHS.itLista;
    itIni = RHS.itIni;
    Usada = RHS.Usada;

    return *this;
  }
};

typedef std::multiset<plstitini> msetplstitiniUtm_Coord_3d;
typedef msetplstitiniUtm_Coord_3d::iterator itmsetplstitiniUtm_Coord_3d;
typedef std::pair<itmsetplstitiniUtm_Coord_3d, itmsetplstitiniUtm_Coord_3d> pairitmsetplstitiniUtm_Coord_3d;

class plstitfim
{
public:

  ittylstlstUtm_Coord_3d itLista;
  Utm_Coord_3d itFim;
  int Usada;

  plstitfim::plstitfim(ittylstlstUtm_Coord_3d pitLista, Utm_Coord_3d& ppitFim) : itLista(pitLista), itFim(ppitFim),Usada(0) {}

  const bool operator<(const class plstitfim& RHS) const
  {
    return itFim < RHS.itFim;
  }

  const plstitfim& operator=(plstitfim& RHS)
  {
    itLista = RHS.itLista;
    itFim = RHS.itFim;
    Usada = RHS.Usada;

    return *this;
  }
};

typedef std::multiset<plstitfim> msetplstitfimUtm_Coord_3d;
typedef msetplstitfimUtm_Coord_3d::iterator itmsetplstitfimUtm_Coord_3d;
typedef std::pair<itmsetplstitfimUtm_Coord_3d, itmsetplstitfimUtm_Coord_3d> pairitmsetplstitfimUtm_Coord_3d;

class CInterCNArestaDGN
{
  public:
    ittylstlstUtm_Coord_3d itCNIntersecao;
    Ponto Intersecao;
    bool CNInversora;

    CInterCNArestaDGN(ittylstlstUtm_Coord_3d pItCN,const Ponto& PInters,bool pCNInversora=false) : itCNIntersecao(pItCN) ,Intersecao(PInters),CNInversora(pCNInversora) {};

    bool operator<(const CInterCNArestaDGN& RHS) const
    {
      return Intersecao < RHS.Intersecao;
    }
    bool operator==(const CInterCNArestaDGN& RHS) const
    {
      return Intersecao == RHS.Intersecao;
    }
};

typedef std::set<CInterCNArestaDGN> tysetCInterCNArestaDGN;
typedef tysetCInterCNArestaDGN::iterator ittysetCInterCNArestaDGN;
typedef tysetCInterCNArestaDGN::reverse_iterator rittysetCInterCNArestaDGN;
typedef tysetCInterCNArestaDGN::const_iterator cittysetCInterCNArestaDGN;

typedef std::list<CInterCNArestaDGN> tylstCInterCNArestaDGN;
typedef tylstCInterCNArestaDGN::iterator ittylstCInterCNArestaDGN;
typedef tylstCInterCNArestaDGN::const_iterator cittylstCInterCNArestaDGN;

class CArestaDGN
{
    Ponto MP1,MP2;
    unsigned long MFlags;
    tysetCInterCNArestaDGN setIntersecoes;

  public:
    enum IND{NENHUM_INDICADOR};
    bool TemCNInversora;

    CArestaDGN(const Ponto& pMP1, const Ponto& pMP2, unsigned long pMFlags = 0,bool pTemCNInversora=false) : MP1(pMP1), MP2(pMP2), MFlags(pMFlags),TemCNInversora(pTemCNInversora) {};

    const unsigned long Flags() const { return MFlags;}
    const Ponto& P1() const { return MP1;}
    const Ponto& P2() const { return MP2; }
    void LimpaIntersecoes() { setIntersecoes.clear();}
    void InsereIntersecao(const ittylstlstUtm_Coord_3d itCN,const Ponto& pPonto) { setIntersecoes.emplace(itCN,pPonto);}
    void RemoveIntersecao(const ittysetCInterCNArestaDGN it) { setIntersecoes.erase(it); }	
    const tysetCInterCNArestaDGN& SetIntersecoes() const { return setIntersecoes;}

    bool operator <(const CArestaDGN& RHS) const
    {
      return MP1 < RHS.P1() ? true : MP1 > RHS.P1() ? false :
             MP2 < RHS.P2() ? true : false;
    }

    CArestaDGN& operator=(const CArestaDGN& RHS)
    {
      MP1 = RHS.P1();
      MP2 = RHS.P2();
      MFlags = RHS.Flags();
    }
};

typedef std::set<CArestaDGN>  tysetPontosSoltosCArestaDGN;
typedef tysetPontosSoltosCArestaDGN::iterator ittysetPontosSoltosCArestaDGN;
typedef tysetPontosSoltosCArestaDGN::reverse_iterator rittysetPontosSoltosCArestaDGN;

class ParitCNDGN
{
public:

  ittylstUtm_Coord_3d itInicioCN,itPontoCN;

  ParitCNDGN(ittylstUtm_Coord_3d pitIni, ittylstUtm_Coord_3d pitPonto):itInicioCN(pitIni), itPontoCN(pitPonto) {};
  
  bool operator<(const ParitCNDGN& RHS) const
  {
    return *itPontoCN < *RHS.itPontoCN;
  }
};

typedef std::set<ParitCNDGN> tysetParitCNDGN;
typedef tysetParitCNDGN::iterator ittysetParitCNDGN;

//----------------------------------------------------------------------------------------------------

// caixa de diálogo de CDiaImportarArqCtrlDrones

class CDiaImportarArqCtrlDrones : public CDialogEx
{
	DECLARE_DYNAMIC(CDiaImportarArqCtrlDrones)

  std::string NomeArq,NomeArqNuvem,NomeProjeto,FormatoArqPontos;
  Ponto PontosControle[4];
  tylstUtm_Coord_3d* pLstPontosControle, pLstPontosNuvem;
  CString Datum;
  bool TemPontosControle;
  int TipoArquivo;
  setPontos SetPontosSoltos;

  tysetParitCNDGN SetCotasCoord_3d;

  bool GravaArquivoXYZ(tylstUtm_Coord_3d& LstCoorUTM,std::string& FormArqPontos);
  bool GravaArquivoCN(tylstlstUtm_Coord_3d& LstLstCoorCN, std::string& FormatoArqPontos,std::string& NomeProj);
  int LeArquivoXYZ();
 // bool LeArqNuvem(std::string& NomArqAtual, tylstGdc_Coord_3d& ListaCoordGdc);
  bool LeArqPCTRL(std::string& NomArq, tylstUtm_Coord_3d& ListaCoordUTM,int NumDatum);
  bool LeArqCN(std::string& NomArqAtual, tylstGdc_Coord_3d& ListaCoordGdcCN);
  //void AcertaNo(ittylstUtm_Coord_3d itSegVerificar, ittylstUtm_Coord_3d itSegAtual);
  void DesembaracaCNs(tylstlstUtm_Coord_3d** pLstLstCoorCN);
  void LigaCNsPartidas(tylstlstUtm_Coord_3d& LstLstCNUTM);  //-- Nas arquivos DGN do IBGE as cns vêem partidas
  int TriangulaPontosCotaDGN(tysetPontosSoltosCArestaDGN& SetArestasPontosCotadosDGN,tylstlstUtm_Coord_3d& LstLstCNUTM);
  int CotaCNsDGN(tysetPontosSoltosCArestaDGN& SetArestasPontosCotadosDGN,tylstlstUtm_Coord_3d& LstLstCNUTM);
  int CotaCurvasNivelDGN(tysetPontosSoltosCArestaDGN& SetArestasPontosCotadosDGN,tylstlstUtm_Coord_3d& LstLstCNUTM);
  int TentaCotar(const tylstlstUtm_Coord_3d& pLstLstCNUTM, ittysetPontosSoltosCArestaDGN pitAresta, unsigned int pEquidisCN,int& pCotouAlguma,unsigned int& pContaCNsCotadas);
  int TentaCotarInvertido(const tylstlstUtm_Coord_3d& pLstLstCNUTM, ittysetPontosSoltosCArestaDGN pitAresta, unsigned int pEquidisCN, int& pCotouAlguma,unsigned int& pContaCNsCotadas );
  void SimplificaCNs(tylstlstUtm_Coord_3d& LstLstCNUTM);  
  bool GravaArquivoPontosSoltos(setPontos& SetPontosSoltos, std::string& FormatoArqPontos, std::string& NomeProj);
  bool GravaArquivoCNCoorxyz(lstlstPontos& pCNs, std::string& FormatoArqPontos, std::string& NomeProj);
  void TransferePontosParaCoord3d(lstlstPontos& pCNs, tylstlstUtm_Coord_3d& LSTLstCoordGdc);
  void CotaCNsIBGE(tylstlstUtm_Coord_3d& LstLstCNUTM, setPontos& PontosCotados);
  void CalculaAZdistanciaCNs(tylstlstUtm_Coord_3d& LstLstCNUTM);
  void CotaCNsRotuladas(tylstlstUtm_Coord_3d& LstLstCNUTM, tylstUtm_Coord_3d& PontosCotados);
  bool VerificaCotaCN(ittylstlstUtm_Coord_3d& LstLstCNUTM, ittylstUtm_Coord_3d& PontosCotados);
  void AcertaCNsRepetidas(tysetPontosSoltosCArestaDGN& ppSetArestasPontosCotadosDGN, tylstlstUtm_Coord_3d& LstLstCNUTM);
  void CotaCNsInversoras(tysetPontosSoltosCArestaDGN& ppSetArestasPontosCotadosDGN, tylstlstUtm_Coord_3d& LstLstCNUTM);

public:

  CDiaImportarArqCtrlDrones(setPontos& pSetPontosSoltos, CWnd* pParent, const std::string& NomeProj, bool bPontosControle, tylstUtm_Coord_3d* pLstPonCtrl, int pTipoArq,unsigned int& EquidisCN);
	virtual ~CDiaImportarArqCtrlDrones();

// Janela de Dados
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIAIMPORTARARQCTRLDRONES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Suporte DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  CString CEdCaminhoPCT,CEdCaminhoNuvem,CEdCaminhoCN;

  void AtualizaDialogo(std::string* pstrMsg, int ValorProgresso);
  afx_msg void OnBnClickedButtestar();
  afx_msg void OnBnClickedButpegararq();
  afx_msg void OnBnClickedButpegararqnuvem();
  afx_msg void OnClickedButpegararqcn();

  CString StrObs;
  virtual BOOL OnInitDialog();
  CComboBox ComboDatum;
  afx_msg void OnEnSetfocusEdcaminhocn();
  afx_msg void OnBnKillfocusButpegararq();
  unsigned int& EquidisCN;
  CStatic CStaObserv;
  CProgressCtrl Progress;
  afx_msg void OnEnKillfocusEdiEquisCn();
};
