#if !defined(AFX_CALCVOLUMES_H__A37AF0C1_684E_11D7_BE61_D4C73E6B3A19__INCLUDED_)
#define AFX_CALCVOLUMES_H__A37AF0C1_684E_11D7_BE61_D4C73E6B3A19__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define TERRENO_PRIMITIVO 0
#define PERFIL_ECOLOGICO 1

/////////////////////////////////////////////////////////////////////////////
// CalcVolumes dialog

class CalcVolumes : public CDialog
{
  enum{CORTE,ATERRO}; 
  enum{ESQUERDO,DIREITO};
  enum{X,Y};

  CListaSecoes Secoes;
  CLPerfis TerrenoPrimitivo;
  CString NomeProjeto,NomeProjetoBase;
  CDialog* pDialogo;
  CHPavimento Empolamentos,HLimpeza;

  CPerfilGeologico PerfilGeologico; 
  tysetCTipoSolo SetTiposSolos;      
  setitCDadosPerfGeologicos SetItDadosPerfisPorTipo;

 // CHPavimento AltPavimento;

  bool MontaSecoes(int Tipo = TERRENO_PRIMITIVO);
  void CalculaVolumes();
  void CalculaOffSets();
  void CalculaAreas(const std::string& TipoSolo = "");
  void CalculaCG();

  void MontaTerrenoPerfilGeologico(const std::string& TipoSolo);
  
  void MontaSetPorTipoSolo()
  {
    for(itsetCDadosPerfGeologicos itPerfGeo = PerfilGeologico.PegaSetSecaoPerfGeo().begin() ; itPerfGeo != PerfilGeologico.PegaSetSecaoPerfGeo().end() ; ++itPerfGeo) 
    {
      SetItDadosPerfisPorTipo.insert(itPerfGeo);
    }
  }

  int PegaOffSetsPerfGeo(const CEstaca& Estaca,const std::string& TipoSolo,Ponto& OffSetEsq,Ponto& OffSetDir);
  void AcertaAreasPerfGeol();

  // Construction
public:
  CalcVolumes(CString& NomProj, CWnd* pParent = NULL);   // standard constructor
  BOOL Create(CWnd* pWnd,CDialog* ppDialogo);             //--- para não modal.
  void CalcularDireto() { OnIniciarcalc();};

  // Dialog Data
  //{{AFX_DATA(CalcVolumes)
  enum { IDD = IDD_CALCVOLUMES };
  CButton	m_butOk;
  CButton	m_butPausa;
  CButton	m_butCancel;
  CButton	m_butIniciar;
  CProgressCtrl	m_Progresso;
  //}}AFX_DATA


  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CalcVolumes)
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
  //}}AFX_VIRTUAL

  // Implementation
protected:

  // Generated message map functions
  //{{AFX_MSG(CalcVolumes)
  virtual BOOL OnInitDialog();
  afx_msg void OnIniciarcalc();
  afx_msg void OnOk2();
  afx_msg void OnCancel3();
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnBnClickedIniciarcalc3();
  afx_msg void OnBnClickedIniciarcalc();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CALCVOLUMES_H__A37AF0C1_684E_11D7_BE61_D4C73E6B3A19__INCLUDED_)
