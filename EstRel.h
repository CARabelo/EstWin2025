#pragma once

typedef std::map<CEstaca,Ponto> MSecaoGeom;    //--- Secoes geometricas para acesso randomico
typedef MSecaoGeom::iterator itMSecaoGeom;

typedef std::list<std::string> tylststring;
typedef tylststring::iterator ittylststring;

typedef std::list<Ponto> lstPontos;
typedef lstPontos::iterator itlstPontos;
typedef lstPontos::const_iterator citlstPontos;

typedef std::map<Ponto, lstPontos> tymapEstlstPontos;
typedef tymapEstlstPontos::iterator ittymapEstlstPontos;
typedef std::pair<ittymapEstlstPontos, bool> pairitbooltymapEstlstPontos;
typedef std::pair<Ponto, lstPontos> pairtymapEstlstPontos;

// CEstRel document

class CEstRel : public CDocument
{
  DECLARE_DYNCREATE(CEstRel)

  CListaSecoes* pListaProjeto;
  CCurHorizontais* pCurHorizontais; 
  MSecaoGeom MapaSecoesGeom;          //--- Mapa de secoes geometricas.
  CCurVerticais* pCurvasVerticais;    //--- Dados do Greide de projeto.  
  CLPerfis* pListaSecoesTerreno;
  CLPerfis* pListaSecoesAvulsas;
  tymapEstlstPontos* pMapPontosTerrplProjeto;
  tylstBacias* pBaciasHidrograficas;
  tlstCRegAreaTaludes ListaAreaTaludes;
  tlstCLimpeza ListaLimpeza;
  lstPontos PontosPassagem[2];
  tymapEquipDrenagem* pEquipDrenagem;

  std::stringstream strstrArqLog;

  int IDRelatorio;
  bool Formatar;

public:
  CEstRel();
  virtual ~CEstRel();
  virtual void Serialize(CArchive& ar);   // overridden for document i/o
  void GeraRelatorio(std::strstream& steBuffer,int IDRelatorio,bool TerminarLinha=false);
  void LeBaciasHidrograficas(const std::string& NomeProjBase);
  void LeEquipDrenagem(const std::string& NomeProjBase);
    
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  virtual BOOL OnNewDocument();

  DECLARE_MESSAGE_MAP()
public:
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
  virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
  virtual void OnCloseDocument();
};
