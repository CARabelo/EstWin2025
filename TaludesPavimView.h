// TaludesPavimView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTaludesPavimView form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CTaludesPavimView : public CTypedPtrListView
{
protected:
  CTaludesPavimView();           // protected constructor used by dynamic creation
  DECLARE_DYNCREATE(CTaludesPavimView)

  // Form Data
public:
  enum { IDD = IDD_TALUDESPAVIM};
  // Attributes
public:
   CEditEx EdUltimoCampo,EdPrimeiroCampo;

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides

public:
  virtual void OnInitialUpdate();
  virtual void Serialize(CArchive& ar);
protected:
  virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

  // Implementation
protected:
  virtual ~CTaludesPavimView();
#ifdef _DEBUG
  virtual void AssertValid() const;
  virtual void Dump(CDumpContext& dc) const;
#endif

  int Consiste(tlstwstr& LstCampos);

  // Generated message map functions
  afx_msg void OnAdd();
  afx_msg void OnRemove();
  afx_msg void OnUpdate();
  afx_msg void OnRemoveAll();
  afx_msg void OnInsertBefore();
  afx_msg void OnSelChangeList();
  DECLARE_MESSAGE_MAP()
  virtual void OnDraw(CDC* /*pDC*/);
};

/////////////////////////////////////////////////////////////////////////////
