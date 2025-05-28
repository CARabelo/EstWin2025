// DGerarDXFPerfGeol.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDGerarDXFPerfGeol dialog

typedef std::list<std::string> tylststring;
typedef tylststring::iterator ittylststring;

typedef std::set<std::string> tysetstring;
typedef tysetstring::iterator ittysetstring;

typedef std::set<CEstaca> tysetEstaca;
typedef tysetEstaca::iterator ittysetEstaca;

class CDGerarDXFPerfGeol : public CDialog
{
  std::string NomeProjeto;
  std::string DxfFileName;

  tylststring LstNomesTiposSolos;
  tysetEstaca setEstacasTerreno;

  double Deltas[2],
         Escala[2];
		 
  double AlturaCotas,DistMinCotas,DistEntreCotas;

  int DirecaoAtual;

  bool OK;          //--- Se inicializou ok, fica true

  Ponto Ppixels,PAtual;

  CDrawing drawing;
  tysetxCotas SetxCotas;

  void ConvEstRealPVertex(REALPOINT& Vertex,const CEstaca& Estaca)   //--- converte coordenadas reais para coordenadas do vídeo
  {
    Vertex.x = Estaca.EstVirtual;
    Vertex.y = Estaca.Cota * 10.0;
  };

  LAYER* PegaLayer(LAYER* Layers,const std::string& NomeLayer,unsigned int QtdLayers)
  {
    for(unsigned int C = 0 ; C < QtdLayers ; C++)
    {
      if(strcmp((Layers+C)->Name,NomeLayer.c_str()) == 0) return (Layers+C);
    }

    return NULL;
  }

  void DistribuiCotas(double& XCota,tysetxCotas& SetxCotas);

  std::string& RemoveBrancos(const std::string& String)
  {
    static std::string Resposta;

    Resposta.clear();

    for(register std::string::const_iterator it = String.begin() ; it != String.end() ; ++it) 
    {
      if(*it != ' ') Resposta += *it;
    }

    return Resposta;
  }

  void CalculaCotasFurosSondagem(const void* vpPerfilGeologico);
  double PegaCotaTerreno(const CEstaca& Estaca,const void* vpPerfilGeologico);

  enum { VU_E_LADOESQUERDO, VU_E_LADODIREITO, VU_E_ACABOUESPACO };

  // Construction
public:
  CDGerarDXFPerfGeol(CWnd* pParent = NULL,CString& NomeProj = CString(""));   // standard constructor
  ~CDGerarDXFPerfGeol();

  // Dialog Data
  //{{AFX_DATA(CDGerarDXFPerfGeol)
  enum { IDD = IDD_GERARDXFPERFGEOL};
  CButton	m_BUGerar;
  CButton	m_BUSair;
  CProgressCtrl	m_Progresso;
  double	m_CStrRazaoHV;
  CString	m_CStrMensagem;
  //}}AFX_DATA

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CDGerarDXFPerfGeol)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CDGerarDXFPerfGeol)
  afx_msg void OnGerar();
  afx_msg void OnSair();
  virtual BOOL OnInitDialog();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

