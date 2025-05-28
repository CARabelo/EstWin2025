// typlstvw.cpp : implementation file
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

#include "stdafx.h"
#include "afxtempl.h"
#include "est.h"
#include <sstream>
#include <iomanip>
#include "estdoc.h"
#include <string>
#include <map>
#include "CExcomboBox.h" 
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "typlstvw.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include "carqigualdades.h"
#include "Dialogo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

extern class monologo monolog;
extern class dialogo dialogar;

#define VUE_USRMSG_ROLAR_REGISTRO  WM_APP + 3

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrListView

IMPLEMENT_DYNCREATE(CTypedPtrListView, CFormView)

CTypedPtrListView::CTypedPtrListView(UINT i, int pQuanCampos, int pUltCampo, int pPrimCampo) : CFormView(i), Template(i), Estado(CTypedPtrListView::INSERCAO),
PrimeiroCampo(pPrimCampo), UltimoCampo(pUltCampo), QuanCampos(pQuanCampos+1), TecShift(0), TextExtent(0),PosArqRav(-1)
{
  LimpaCampos();
}																

CTypedPtrListView::~CTypedPtrListView()
{
}	

void CTypedPtrListView::OnInitialUpdate()
{
  CFormView::OnInitialUpdate();

  int Tabs[2] = {50,75};

  ((CListBox *) GetDlgItem(IDC_LIST))->SetTabStops(2,&Tabs[0]);

  // Copy all of the strings from the document's CTypedPtrList
  // to the listbox.

  m_ctlList.ResetContent();
  CMyStructList& mystructList(GetDocument()->m_mystructList);
  POSITION pos(mystructList.GetHeadPosition());
  int C(0);
  while (pos != NULL)
  {
    CMyStruct* pMyStruct(mystructList.GetNext(pos));
    AddMyStructToListBox(pMyStruct);
  }

  if(Template == IDD_CADASTROACITOPOG)
  {
    CFont m_Font;
 	  m_Font.CreatePointFont(140,"Tahoma"); 
    m_ctlList.SetFont(&m_Font);
  }

  ItPrimeiroCampo = ItUltimoCampo = ListaCampos.end();
}

void CTypedPtrListView::DoDataExchange(CDataExchange* pDX)
{
  DoDataExchange(pDX,0);
}

void CTypedPtrListView::DoDataExchange(CDataExchange* pDX,int Arquivo)
{
  static int MaxCarac[30][11] = 
  {{11,11,1,1,1,1,1,1,1,1,1},{15,2,8,5,8,1,1,1,1,1,1},
  {15,2,8,8,8,1,1,1,1,1,1},{15,2,15,2,1,1,9,5,8,6,6},
  {1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1},
  {15,2,7,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1},
  {15,2,7,7,7,7,1,1,1,1,1},{11,2,8,8,8,8,8,8,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1},
  {1,1,1,1,1,1,1,1,1,1,1},{1,1,1,1,1,1,1,1,1,1,1},
  {11,2,1,8,8,8,8,8,8,1,1}};

  CFormView::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LIST, m_ctlList);
  DDX_Text(pDX, IDC_EDIT, m_Campo[0]);
  DDX_Text(pDX, IDC_EDIT1, m_Campo[1]);
  DDX_Text(pDX, IDC_EDIT2, m_Campo[2]);
  DDX_Text(pDX, IDC_EDIT3, m_Campo[3]);
  DDX_Text(pDX, IDC_EDIT4, m_Campo[4]);
  DDX_Text(pDX, IDC_EDIT5, m_Campo[5]);
  DDX_Text(pDX, IDC_EDIT6, m_Campo[6]);
  DDX_Text(pDX, IDC_EDIT7, m_Campo[7]);
  DDX_Text(pDX, IDC_EDIT8, m_Campo[8]);
  DDX_Text(pDX, IDC_EDIT9, m_Campo[9]);
  DDX_Text(pDX, IDC_EDIT10, m_Campo[10]);
  DDX_Text(pDX, IDC_EDIT11, m_Campo[11]);

  if (pDX->m_bSaveAndValidate) 
  {
    /*
    if(_tcschr(m_str, ' ') != NULL)
    {
      AfxMessageBox(IDS_BLANK_CHARS_NOT_ALLOWED);
      pDX->Fail();
    }
  */

  }
}

BEGIN_MESSAGE_MAP(CTypedPtrListView, CFormView)
  ON_BN_CLICKED(IDC_ADD, OnAdd)
  ON_BN_CLICKED(IDC_UPDATE, OnUpdate)
  ON_BN_CLICKED(IDC_REMOVE, OnRemove)
  ON_BN_CLICKED(IDC_REMOVE_ALL, OnRemoveAll)
  ON_LBN_SELCHANGE(IDC_LIST, OnSelChangeList)
  ON_BN_CLICKED(IDC_INSERT_BEFORE, OnInsertBefore)
  ON_WM_CLOSE()
  ON_WM_ENDSESSION()
  ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_DIRECT, OnFilePrint)
  ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
  ON_MESSAGE(VUE_USRMSG_ROLAR_REGISTRO, OnKillFocusUltimoCampo)
  ON_COMMAND(IDC_GRAVAR, &CTypedPtrListView::OnGravar)
  ON_COMMAND(IDC_LIMPAR_CAMPOS, &CTypedPtrListView::OnLimparCampos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrListView diagnostics

#ifdef _DEBUG
void CTypedPtrListView::AssertValid() const
{
  CFormView::AssertValid();
}

void CTypedPtrListView::Dump(CDumpContext& dc) const
{
  CFormView::Dump(dc);
}

CEstDoc* CTypedPtrListView::GetDocument() // non-debug version is inline
{
  return STATIC_DOWNCAST(CEstDoc, m_pDocument);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CmystructListView internal implementation

CMyStruct* CTypedPtrListView::FindMyStruct(int& nSel, POSITION& pos)
{
  nSel = m_ctlList.GetCurSel();
  if (nSel == LB_ERR)
  {
    AfxMessageBox(IDS_SELECT_ENTRY);
    return NULL;
  }

  // The CMyStruct pointer was saved as the listbox entry's data item.

  CMyStruct* pMyStruct = (CMyStruct*)m_ctlList.GetItemDataPtr(nSel);

  // Find the CMyStruct pointer in the CTypedPtrList.

  pos = GetDocument()->m_mystructList.Find(pMyStruct);

  // If the CMyStruct is displayed in the listbox, it should also be
  // found in the CTypedPtrList.

  ASSERT(pos != NULL);

  return pMyStruct;
}

void CTypedPtrListView::AddMyStructToListBox(CMyStruct* pMyStruct, int nSel)
{
  // Add new CMyStruct to the listbox.

  CString str;
  size_t TamTexto(0);

  pMyStruct->FormatMyStruct(pMyStruct->lstRegAtual, str, Template);
  if(Template != IDD_FUROSSONDAGEM)
    TamTexto = pMyStruct->lstRegAtual.size() * 53;
  else 
    TamTexto = pMyStruct->lstRegAtual.size() * 110;

  if (TamTexto > TextExtent)
  {
    TextExtent = TamTexto;
    m_ctlList.SetHorizontalExtent((int)TextExtent);
  }
  if (nSel == -1)	
  {
    nSel = m_ctlList.AddString(str.GetBuffer());
  }
  else 
  {
    m_ctlList.InsertString(nSel, str.GetBuffer());
  }

  // Save the CMyStruct pointer as the listbox entry's "data item".

  m_ctlList.SetItemDataPtr(nSel, pMyStruct);

  //--- Rola o listbox

  m_ctlList.SetCurSel(nSel);  	//--- Posiciona no ítem inserido.
  m_ctlList.SetCurSel(-1);  	  //--- Apaga o intem selecionado

  ListaCampos.clear();
  ItUltimoCampo = ItPrimeiroCampo = ListaCampos.end();
  GetDlgItem(IDC_EDIT)->SetFocus();  //--- Vai para o primeiro editbox;
}

/////////////////////////////////////////////////////////////////////////////
// CTypedPtrListView message handlers

void CTypedPtrListView::OnAdd()
{
  UpdateData(TRUE) ;

  TecShift = false;               //--- Simula o killfocus para tras.
  OnKillFocusUltimoCampo(1,0);    //--- Somente para atualizar os campos pendentes na lista de campos.
  ittlstwstr It(ListaCampos.end());
  ittlstwstr ItUltimo(ListaCampos.end());

  //--- Remove os campos vazios no final 

  const std::string& NomeArq(GetDocument()->PegaNomeCompleto()), 
                     Ext(NomeArq.substr(NomeArq.size() - 3));

  if(ListaCampos.size() > 0)
  {
    bool Acabou(false);
    int ContaCampos(0);
    do
    {
      if(!((Ext.compare("OAC") == 0 && ContaCampos == 3) || (Ext.compare("OAE") == 0 && ContaCampos == 5)))    //--- Campos texto, não remove
      { 
        //--- Remove campos vazios ou que só têm brancos

        if(ListaCampos.rbegin()->size() == 0 ||ListaCampos.rbegin()->find_last_not_of(' ') == -1) ListaCampos.pop_back();
        else Acabou = true;
      }
    }while(ListaCampos.size() && !Acabou);
  }
  // Add new CMyStruct to the CTypedPtrList

  CMyStruct* pMyStruct(new CMyStruct);

  for(ittlstwstr It = ListaCampos.begin() ; It!= ListaCampos.end() ; It++)
  {
    pMyStruct->lstRegAtual.push_back(*It);
  }
  
  if(Consiste(pMyStruct->lstRegAtual))
  {
    delete pMyStruct;
    if(ListaCampos.size() <= QuanCampos)        //--- Quando não foi digitado o numero de campos minimos de um registro tem que reiniciar 
    {
      ListaCampos.clear();   
      ItUltimoCampo = ItPrimeiroCampo = ListaCampos.end();
    }

    return;
  }
  
  //--- Adiciona na lista colocando o registro na ordem do primeiro campo
  POSITION PosOrdenada(nullptr);

  if (Ext.compare("RAV") == 0)
  {
    PosOrdenada = GetDocument()->PegaPosOrdenada(*pMyStruct->lstRegAtual.begin(),PosArqRav);
  }
  else
  {
    PosOrdenada = GetDocument()->PegaPosOrdenada(*pMyStruct->lstRegAtual.begin());
  }
  int nSel(-1);

  if(PosOrdenada != NULL)
  {
    CMyStructList& mystructList(GetDocument()->m_mystructList);
    mystructList.InsertBefore(PosOrdenada,pMyStruct);
    CMyStruct* pMyStructPos(mystructList.GetAt(PosOrdenada));

    CString str;

    pMyStruct->FormatMyStruct(pMyStructPos->lstRegAtual, str, Template);
    nSel = m_ctlList.FindString(-1,str);
  }
  else 
  {
    if (Ext.compare("RAV") == 0)
    {
      GetDocument()->m_mystructList.AddTail(pMyStruct);
    }
    else
    {
     // GetDocument()->m_mystructList.InsertBefore(nSel,pMyStruct);
      GetDocument()->m_mystructList.AddTail(pMyStruct);

    }
  }
  
  AddMyStructToListBox(pMyStruct,nSel);
  if(nSel == -1) nSel = m_ctlList.GetCount();
  m_ctlList.SetCurSel(nSel);

  //--- Limpa os edit box.

  LimpaCampos();

  UpdateData(FALSE);

  CMyStructList& mystructList(GetDocument()->m_mystructList);

  GetDocument()->SetModifiedFlag(true);
}

void CTypedPtrListView::OnInsertBefore()
{
  if (UpdateData() != TRUE)	return;

  int nSel;
  POSITION pos;

  // Find the CMyStruct in the CTypedPtrList and listbox.

  if (!FindMyStruct(nSel, pos)) return;

  // Insert new CMyStruct in the CTypedPtrList

  CMyStruct* pMyStruct(new CMyStruct);

  for (int C = 0; C < NUM_MAX_CAMPOS; C++)  pMyStruct->Campos[C] = m_Campo[C];

  if(Consiste(pMyStruct->lstRegAtual))
  {
    delete pMyStruct;
    return;
  }

  //--- Adiciona na lista.

  GetDocument()->m_mystructList.InsertBefore(pos, pMyStruct);

  //---Adiciona no listBox

  AddMyStructToListBox(pMyStruct, nSel);

  GetDocument()->SetModifiedFlag(true);  
}

void CTypedPtrListView::OnUpdate()
{
  if (UpdateData(TRUE) != TRUE)	return;

  TecShift = false;               //--- Simula o killfocus para frente.
  OnKillFocusUltimoCampo(1,0);    //--- Somente para atualizar os campos pendentes na lista de campos.

  int nSel;
  POSITION pos;

  // Find the CMyStruct in the CTypedPtrList and listbox.

  CMyStruct* pMyStruct(FindMyStruct(nSel, pos));
  if (pMyStruct == NULL) return;

  LimpaUltimosCamposBrancos();

  if(ListaCampos.size() < 3) return;
  
  // Replace the value of the CMyStruct in the CTypedPtrList.

  pMyStruct->lstRegAtual.clear();

  for (ittlstwstr It(ListaCampos.begin()); It != ListaCampos.end(); It++)
  {
    pMyStruct->lstRegAtual.push_back(*It);
  }

  if(Consiste(pMyStruct->lstRegAtual)) return;

  // Replace the displayed CMyStruct in the listbox by removing
  // the old listbox entry and adding a new entry.

  m_ctlList.DeleteString(nSel);
  AddMyStructToListBox(pMyStruct, nSel);
  if (nSel == -1) nSel = m_ctlList.GetCount();
  m_ctlList.SetCurSel(nSel);

  //--- Limpa os edit box.

  LimpaCampos();

  UpdateData(FALSE);

  GetDocument()->SetModifiedFlag(true);
}

void CTypedPtrListView::OnRemove()
{
  int nSel;
  POSITION pos;
  // Find the CMyStruct in the CTypedPtrList and listbox.

  CMyStruct* pMyStruct = FindMyStruct(nSel, pos);
  if (pMyStruct == NULL) return;

  // Remove the CMyStruct ptr from the CTypedPtrList.

  GetDocument()->m_mystructList.RemoveAt(pos);

  // Delete the CMyStruct object. (The CTypePtrList only holds the ptr.)

  delete pMyStruct;

  // Remove the corresponding entry from the listbox.

  m_ctlList.DeleteString(nSel);

  nSel = -1;

  Estado = INSERCAO;

  LimpaCampos();

  UpdateData(FALSE);

  GetDocument()->SetModifiedFlag(true);

  m_ctlList.SetCurSel(LB_ERR);
}

void CTypedPtrListView::OnRemoveAll()
{
  if(AfxMessageBox("Tem certeza que deseja remover todos os registros?",MB_YESNO) != IDYES) return; 
  
   CMyStructList& mystructList = GetDocument()->m_mystructList;

  // Delete all of the CMyStruct objects pointed to
  // by the CTypedPtrList. Then remove all of the
  // CMyStruct pointers from the CTypedPtrList, which
  // is faster than removing each individually.

  POSITION pos = mystructList.GetHeadPosition();

  while (pos != NULL)	delete mystructList.GetNext(pos);

  mystructList.RemoveAll();

  // Remove all of the corresponding formatted strings from the listbox.

  m_ctlList.ResetContent();

  GetDocument()->SetModifiedFlag(true);
}

void CTypedPtrListView::OnSelChangeList()
{
  // Update the edit control to reflect the new selection
  // in the listbox.

  int nSel(0);
  POSITION pos;

  // Find the CMyStruct in the CTypedPtrList and listbox.

  CMyStruct* pMyStruct(FindMyStruct(nSel, pos));

  if (nSel == -1)  return;

  ListaCampos.clear();
  ItUltimoCampo = ItPrimeiroCampo = ListaCampos.end();
  LimpaCampos();

  ittlstwstr It = pMyStruct->lstRegAtual.begin();

  for( ; It != pMyStruct->lstRegAtual.end(); It++) ListaCampos.push_back(*It);

  It = pMyStruct->lstRegAtual.begin();

  for (int C = 0; C < NUM_MAX_CAMPOS && It != pMyStruct->lstRegAtual.end(); C++) m_Campo[C] = (*It++).c_str();

  ItPrimeiroCampo = ListaCampos.begin();

  for (unsigned int C = 0 ; C < PrimeiroCampo  && ItPrimeiroCampo != ListaCampos.end() ; C++) ItPrimeiroCampo++;        //--- Posiciona o primeiro campo a rolar

  if(ListaCampos.size() -1 >= UltimoCampo)   
  {
    ItUltimoCampo = ItPrimeiroCampo;

    for (unsigned int C = PrimeiroCampo ; C < UltimoCampo  && ItUltimoCampo != ListaCampos.end(); C++)   //--- Pode ter lista vazia
    {
      ItUltimoCampo++;   //--- Posiciona o último campo a rolar
    }
  }
  else
  {
    //--- Se a quantidade de campos não completa um registro tem q similar como se 
    //--- fosse um novo registro (ItUltimoCampo = end()) senão o último campos fica 
    //--- posicionado fora do campo correto, pois o ultimo campo deve apontar sempre
    //--- para o último campo da lista que está visível na tela  

     ListaCampos.clear();
     ItUltimoCampo = ItPrimeiroCampo = ListaCampos.end();          
                                               
  }
  UpdateData(FALSE);

  //--- Um campo foi selecionado então muda o estado para edicao

  Estado = EDICAO;
}

void CTypedPtrListView::OnFilePrint()
{
}

void CTypedPtrListView::OnDraw(CDC* pDC)
{
  if(pDC->IsPrinting())
  {
    CSize CC = pDC->GetTextExtent("Z");
    CPoint LocalAtual = CPoint(0,CC.cy);
    int NLinha(0);

    pDC->TextOut(0,0,GetDocument()->GetTitle());

    CMyStructList& mystructList = GetDocument()->m_mystructList;
    POSITION pos = mystructList.GetHeadPosition();
    CString str;

    while (pos != NULL)
    {
      CMyStruct* pMyStruct = mystructList.GetNext(pos);
      pMyStruct->FormatMyStruct(pMyStruct->lstRegAtual,str,Template);
      LimpaBrancos(str);

      pDC->TextOut(0,++NLinha*CC.cy,str);
    }
    pDC->TextOut(0,++NLinha*CC.cy,"FIM DO RELATÓRIO.");
  }
}

BOOL CTypedPtrListView::OnPreparePrinting(CPrintInfo* pInfo)
{
  // default preparation
  pInfo->m_bContinuePrinting = TRUE; 
  return DoPreparePrinting(pInfo);
}

void CTypedPtrListView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
  // TODO: Add your specialized code here and/or call the base class

  if(pInfo->m_nCurPage > 1) 
  {
    //pDC->SetViewportOrg(CPoint(100,100));
    CSize gg = pDC->GetViewportExt();
    pDC->SetWindowOrg(CPoint(paper_res.x,0));
  }

  CFormView::OnPrint(pDC, pInfo);

  PagAtual = pInfo->m_nCurPage;
}

void CTypedPtrListView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
  PagAtual = 0;

  paper_res.x = pDC->GetDeviceCaps(HORZRES);
  paper_res.y = pDC->GetDeviceCaps(VERTRES);

  CFormView::OnBeginPrinting(pDC, pInfo);
}

void CTypedPtrListView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
  if(pInfo && pInfo->m_nCurPage > 1) pDC->SetWindowOrg(CPoint(100,100));

  CFormView::OnPrepareDC(pDC, pInfo);
}

inline void  CTypedPtrListView::LimpaBrancos(CString& CSDado)   //-- Deixa só um espaço em branco.
{
  CSDado.Replace('\t',' ');

  register int i(0);

  while((i= CSDado.Find(' ',i)) != -1)
  {
    register int z(i);
    register int Count(0);

    while (CSDado[++z] == ' ') Count++;

    CSDado.Delete(i,Count);

    i+= ++Count;
  }
}

int CTypedPtrListView::Consiste(tlstwstr& ListaCampos)
{
  //--- As consistências estão nas classes filhas

  return 0;
}

void CTypedPtrListView::OnBnClickedGravar()
{
}

LRESULT CTypedPtrListView::OnKillFocusUltimoCampo(WPARAM WP, LPARAM LP)  //--- Rola o registro sendo editado, se necessário
{
  unsigned long CtrlAtual(::GetDlgCtrlID(::GetFocus()));

  UpdateData(TRUE);

  //--- Shift não esta pressionado? rola pra frente
  //--- Quando a tarefa não rola campos (UltimoCampo == ListaCampos.end())
  //--- tem que simular o rolar para frente para passar os campos dos editbox 
  //--- para a para a lista  

  if (TecShift  == false)           
  {
    if((LP & 2) != 2)       //--- É o último campo?, se sim rola pra frente (LP = 2 é o primeiro campo) 
    {
      if (ListaCampos.size() == 0)   //--- A lista ainda esta vazia? insere os primeiros campos na lista
      {
        for (unsigned int C = 0; C <= QuanCampos; C++)
          ListaCampos.push_back(m_Campo[C].GetBuffer());

        ItUltimoCampo = ListaCampos.end();
        ItPrimeiroCampo = ListaCampos.begin();

        for (unsigned int C = 0; C < PrimeiroCampo ; C++) ItPrimeiroCampo++;

        if(WP != 1 && ItPrimeiroCampo != ListaCampos.end()) ItPrimeiroCampo++;   //--- WP == 1 veio do enter, não rola o registro
      }
      else    //--- A lista não esta vazia           
      {
        unsigned int C(0);
        ittlstwstr It(ListaCampos.begin());

        for(C = 0 ; C < PrimeiroCampo; C++)                                   //--- Atualiza os campos antes dos campos a rolar
        {
           if(It == ListaCampos.end()) 
             ListaCampos.push_back(m_Campo[C].GetBuffer());                   //--- O registro pode estar incompleto, (veio do enter)
           else  *It++ = m_Campo[C];   
        }

        //--- Atualiza os dados da lista com os valores dos campos

      //  ItUltimoCampo--;

        for (It = ItPrimeiroCampo ; It != ItUltimoCampo ; It++)  *It = m_Campo[C++];

        if(It != ListaCampos.end()) *It = m_Campo[C];  //-- Passa o ultimo campo
        else if(m_Campo[C].GetLength()) ListaCampos.push_back(m_Campo[C].GetBuffer());
        
        ittlstwstr ItUltTemp(ItUltimoCampo);

        if ((ItUltTemp) == ListaCampos.end())       //--- Se não há mais dados na lista está inserindo o último campo  
        {  
          if(WP != 1) ListaCampos.push_back(m_Campo[UltimoCampo].GetBuffer());
          ItUltimoCampo = ListaCampos.end();
          ItUltimoCampo--;
        }
        else
          m_Campo[C] = ItUltTemp->c_str();           //--- Senão esta navegando na lista (pressionou tab) então atualiza o último campo 

        if (WP != 1)     //--- se WP == 1 veio do Adicionar, não rola o registro
        {
          ItUltimoCampo++;
          ItPrimeiroCampo++;
        }
      }
    }
    else   //--- É o primeiro campo e shift não esta pressionado (passa para o proximo campo)
    {
      GetNextDlgTabItem(GetDlgItem(CtrlAtual))->SetFocus();
      ((CEdit*)GetNextDlgTabItem(GetDlgItem(CtrlAtual)))->SetSel(0,-1,false);

      return true;
    }
  }
  else        //--- Shift está pressionado
  {
    ittlstwstr ItTemp(ListaCampos.begin());

    if (ItTemp != ListaCampos.end())
    {
      ittlstwstr It(ItPrimeiroCampo);

      if (It == ListaCampos.end())
      {
        //--- Ajusta o ultimo campo

        if(ItUltimoCampo == ListaCampos.end()) ItUltimoCampo--;  //--- Retrocede para ficar no último
        else ItUltimoCampo++;                                    //--- Incrementa para ficar no último 
      }

      for (unsigned int C = 0; C < PrimeiroCampo; C++)  ItTemp++;    //--- Posiciona no 1o campo a rolar (não pode andar pra tras do primeiro campo a rolar)
    }

    if (ItPrimeiroCampo != ItTemp) ItPrimeiroCampo--;        //--- Rola pra tras o primeiro campo       
    else
    {
     ((CEdit*)GetNextDlgTabItem(GetDlgItem(CtrlAtual), true))->SetFocus();    //--- Acerta o foco, tem q  ficar no primeiro campo

     for (unsigned int C = PrimeiroCampo ; C < UltimoCampo ; C++) ItTemp++;   //--- não pode andar pra tras do primeiro campo a rolar

     ItUltimoCampo = ItTemp;

     return true;
    }
  }

  if (ItPrimeiroCampo != ListaCampos.end())    //--- O primeiro campo pode ser o último
  {
    ///if (WP != 1 && TecShift) UltimoCampo++;

    if(WP != 1) MostraRegistroSendoEditado();              //--- Se não veio do enter rola o registro para a direita ou para a esquerda

    GetDlgItem(CtrlAtual)->SetFocus();
    ((CEdit*)GetNextDlgTabItem(GetDlgItem(CtrlAtual)))->SetSel(0,-1,false);
  }
  else GetNextDlgTabItem(GetDlgItem(CtrlAtual))->SetFocus();   //--- Rola o registro mas fica no ultimo campo

  return true;
}

void CTypedPtrListView::MostraRegistroSendoEditado()
{
  unsigned int NumCampoAtual(PrimeiroCampo);

  int UltCampo(UltimoCampo);

  if (TeclaShift() == 1) UltCampo++;

  for (ittlstwstr ItCampoAtual = ItPrimeiroCampo; NumCampoAtual < UltCampo && ItCampoAtual != ItUltimoCampo; ItCampoAtual++, NumCampoAtual++)
  {
    m_Campo[NumCampoAtual] = ItCampoAtual->c_str();
  }

  if (ItUltimoCampo != ListaCampos.end()) m_Campo[NumCampoAtual] = ItUltimoCampo->c_str();
  else  m_Campo[NumCampoAtual].Empty();

  for (NumCampoAtual++ ; NumCampoAtual < NUM_MAX_CAMPOS-1 ; NumCampoAtual++)
  {
    m_Campo[NumCampoAtual] = m_Campo[NumCampoAtual + 1];
  }

  m_Campo[NUM_MAX_CAMPOS-1] = "";

  UpdateData(FALSE);
}

BOOL CTypedPtrListView::PreTranslateMessage(MSG* pMsg)
{
  if (pMsg->message == WM_KEYUP)
  {
    if (pMsg->wParam == 16)
    {
      TeclaShift(false);
    }
  }
  else
  {
    if (pMsg->message == WM_KEYDOWN)
    {
      if (pMsg->wParam == 16)
      {
        TeclaShift(true);
      }
    }
  }

  return CFormView::PreTranslateMessage(pMsg);
}

void CTypedPtrListView::OnGravar()
{
  if(dialogar.DoModal(11) == IDOK) 
  {
    GetDocument()->DoFileSave();
  }
}

void CTypedPtrListView::OnLimparCampos()
{
  ListaCampos.clear();
  ItPrimeiroCampo = ItUltimoCampo = ListaCampos.end();

  LimpaCampos();

  UpdateData(false);
}

void CTypedPtrListView::LimpaUltimosCamposBrancos()
{
  const std::string& NomeArq(GetDocument()->PegaNomeCompleto()), 
                     Ext(NomeArq.substr(NomeArq.size() - 3));

  if(ListaCampos.size() > 0)
  {
    bool Acabou(false);
    int ContaCampos(0);

    if(!((Ext.compare("OAC") == 0 && ContaCampos == 3) || (Ext.compare("OAE") == 0 && ContaCampos == 5)))    //--- Campos texto, não remove
    { 
      do
      {
          //--- Remove campos vazios ou que só têm brancos

        if(ListaCampos.rbegin()->size() == 0 ||ListaCampos.rbegin()->find_last_not_of(' ') == -1) ListaCampos.pop_back();
        else Acabou = true;
      }while(ListaCampos.size() && !Acabou);
    }
  }
}