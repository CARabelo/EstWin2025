// A definiçao desta classe foi baseda na classe collect da MFC - Exemplos
//------------------------------------------------------------------------
// colledoc.h : interface of the CCollectDoc class
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1995 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

class CMyStruct
{
  // Attributes
public:
  std::list<std::string> lstRegAtual;
  std::list<std::string>::iterator itlstRegAtual,itCampoInicio,itCampoFim;
  CString Campos[15];

  // Operations
public:
   void FormatMyStruct(tlstwstr pListaCampos,CString& str, int Template);

   void Arredonda(int NumCampo,int NumCasas)
   {
     if(NumCampo > (int )lstRegAtual.size()) return;

     std::list<std::string>::iterator it = lstRegAtual.begin();

     for (NumCampo-- ; NumCampo != 0 ; it++) 
     {
       NumCampo--;
     }

     std::stringstream strstr;

     strstr <<  std::fixed  << std::setprecision(NumCasas);
     strstr << atof(it->c_str()) << std::endl;

     *it = strstr.str();
   }
};


// Note: in order for the specialization to be accepted by the compiler
//	the prototype for SerializeElements (or any other specialization)
//	must be visible when the template is expanded.	It is best to place
//	these declarations in the header file to avoid any unexpected
//	results.

//void SerializeElements(CArchive& ar, CMyStruct** ppElements, int nCount);
/*
class CMyObject : public CObject
{
public:
CMyObject();
DECLARE_SERIAL(CMyObject)

// Attributes
public:
CString Campos[15];

// Operations
public:
void FormatMyObject(CString& str);

// Implementation
public:
virtual ~CMyObject();
virtual void Serialize(CArchive& ar);
};
*/
typedef CTypedPtrList <CPtrList, CMyStruct*> CMyStructList;

class CEstDoc : public CDocument
{
  void GravaSecaoTriangulada(CArchive& ar,const void* EstacaAtual,const void* ListaAtualPontos);
  unsigned int CEstDoc::PegaQuantidadeSecoes(); 

protected: // create from serialization only

  std::string NomeCompleto;

  CEstDoc();
  int PegaNumArquivo(CArchive &ar);

  DECLARE_DYNCREATE(CEstDoc)

  // Attributes
public:

  tlstlstwstr LstRegAciTopog;     //--- Lista usada na tarefa de Relacionar acidentes topog do autocad
  CMyStructList m_mystructList;

 // enum {GRAVAR = 100};

  // Overrides
  virtual void DeleteContents();
  // ClassWizard generate virtual function overrides
  //{{AFX_VIRTUAL(CEstDoc)
public:
  virtual void SetTitle(LPCTSTR lpszTitle);
  virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);

  POSITION PegaPosOrdenada(std::string EstacaInserir,unsigned long PosArqRav=0);

  const std::string& PegaNomeCompleto() const { return NomeCompleto;};

protected:
  virtual BOOL OnNewDocument();
  //}}AFX_VIRTUAL

  // Implementation
public:
  virtual ~CEstDoc();
  virtual void Serialize(CArchive& ar);   // overridden for document i/o
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;

#endif

  // Generated message map functions
protected:
  //{{AFX_MSG(CEstDoc)
  //}}AFX_MSG
  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnFilePrint();
protected:
  virtual BOOL SaveModified();
};

/////////////////////////////////////////////////////////////////////////////
