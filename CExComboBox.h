#include "afxwin.h"

class CExComboBox :  public CComboBox
{
  void * pApp;
  int Conteudo;
  
  public:

    enum  {ID_PROJETOS,ID_RUAS};

    CExComboBox(void* pMF=NULL, int pID=ID_PROJETOS);
    ~CExComboBox();

    void Contem(int oQue) { Conteudo = oQue; }

    DECLARE_MESSAGE_MAP()
    afx_msg void OnCbnSelchange();
};

