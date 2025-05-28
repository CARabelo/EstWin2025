#if !defined(AFX_COMBOPROCURARDESSECAO_H__6D4A49E3_AA6B_11D4_BE0D_00104BC6689A__INCLUDED_)
#define AFX_COMBOPROCURARDESSECAO_H__6D4A49E3_AA6B_11D4_BE0D_00104BC6689A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ComboProcurar.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CComboProcurarDesSecao window

class CComboProcurarDesSecao : public CComboBox
{
  CString Selecao;

  bool PressionouSetaAbaixo,
       PressionouSetaAcima;

  // Construction
public:
  CComboProcurarDesSecao();

  int PegaUltimaTeclaPressionada() 
  { return PressionouSetaAbaixo ? VK_UP : PressionouSetaAcima ? VK_DOWN : -1; ;}

  void Passo(int Direcao)
  {  
    if(Direcao == -1) SetCurSel((GetCurSel()-1));
    else SetCurSel((GetCurSel()+1));
  }

  int Posiciona(const CEstaca& Secao)
  {
   if(GetCount() == 0) return false;

    int SelAtual(-1);

    int Count = GetCount()-1;

    do
    {
      GetLBText(++SelAtual,Selecao);  
      Selecao = Selecao.Left(Selecao.ReverseFind(' '));
    }while (SelAtual < Count && (CEstaca(Selecao).EstVirtual) < Secao.EstVirtual);

    SetCurSel(SelAtual);

    return true;
  }

  int PosicionaTexto(const CString& Texto)
  {
    if (GetCount() == 0) return false;

    int SelAtual(-1);

    int Count = GetCount() - 1;

    do
    {
      GetLBText(++SelAtual, Selecao);
      int Resultado = Selecao.Compare(Texto);
    } while (SelAtual < Count && (Selecao.Compare(Texto) != 0));

    SetCurSel(SelAtual);

    return true;
  }

  // Attributes
public:

  // Operations
public:

  // Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CComboProcurarDesSecao)
  //}}AFX_VIRTUAL

  void SelecaoOK() { OnSelendok(); }

  // Implementation
public:
  virtual ~CComboProcurarDesSecao();

  // Generated message map functions
protected:
  //{{AFX_MSG(CComboProcurarDesSecao)
  afx_msg void OnSelendok();
  //}}AFX_MSG

  DECLARE_MESSAGE_MAP()
public:
  afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
  afx_msg void OnCbnSetfocus();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBOPROCURARDESSECAO_H__6D4A49E3_AA6B_11D4_BE0D_00104BC6689A__INCLUDED_)
