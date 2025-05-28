// ListaProjetos.cpp : implementation file
//

#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "cponto.h"
#include "clpontos.h"
#include "math.h"
#include "estaca.h"
#include "perfil.h"
#include "io.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include "supercstring.h"
#include "carqtalcorte.h"
#include "ctalcorte.h"
#include "carqtalaterro.h"
#include "ctalaterro.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqtaludespavim.h"
#include "taludespavim.h"
#include <list>
#include "csecao.h"
#include "clistasecoes.h"
#include "ListaMedicoes.h"
#include <string>
#include <map>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListaProjetos

CListaMedicoes::CListaMedicoes(CString& NomeProj,int EMedicao) : NomeProjeto(NomeProj),QuantMedicoes(((CMainFrame*) AfxGetMainWnd())->QMedADesenhar)
{
  //--- Pega a n últimas mediçoes:
  //--- 1 - Pega todos os nomes das mediçoes menores que a medição atual e os coloca numa lista.
  //--- 2 - Pega n medições a partir do pé da lista.

  if(EMedicao && QuantMedicoes > 0)
  {
    //--- Pega todas as medições menores ou iguais a atual.

    CString Caminho(((CMainFrame*)AfxGetMainWnd())->m_sNomeComp);
    Caminho = Caminho.Left(Caminho.GetLength() - 3) + "???.cab";

    //---  Pega a primeira e a coloca na lista

    struct _finddata_t Arquivo;
    intptr_t Handler;
    CString NomeMedicao;

    if ((Handler = _findfirst(Caminho.GetBuffer(Caminho.GetLength()),&Arquivo)) != -1)
    {
      int NumMed(atoi(NomeProj.Right(3)));

      NomeMedicao = Arquivo.name;
      NomeMedicao = NomeMedicao.Left(NomeMedicao.GetLength()-4);
      Caminho = Caminho.Left(Caminho.ReverseFind('\\')+1);

      CList<CString,CString&> ListaNomesMed; 

      int NumMedLida(atoi(NomeMedicao.Right(3)));

      if (NumMedLida < NumMed) ListaNomesMed.AddTail(NomeMedicao);

      //--- Pega as outras e as coloca na lista

      while(!_findnext(Handler,&Arquivo))
      {
        NomeMedicao = Arquivo.name;
        NomeMedicao = NomeMedicao.Left(NomeMedicao.GetLength()-4);
        NumMedLida = atoi(NomeMedicao.Right(3));

        if (NumMedLida <= NumMed) ListaNomesMed.AddTail(NomeMedicao);
      }

      //--- Pegas os n últimos mnomes da lista, abre a medição com este nome e a inclui na lista de medições
      //--- Lê um a mais, o terreno da última medição

      int i(0);

      while(ListaNomesMed.GetCount()  && i < QuantMedicoes)
      {
        AddTail(new CListaSecoes(Caminho + ListaNomesMed.GetTail()));
        ListaNomesMed.RemoveTail();
        i++;
      }
    }

    //-- Pega o terreno da primeira medição, senão a seção de medição fica no "ar"... 

    if (GetCount() > 0)
    {
      CString NomeTerrMedicao(GetTail()->PegaProjeto() + ".tme");
      TerrPrimMedicao.Constroi(NomeTerrMedicao);
    }
  }
}

CListaMedicoes::~CListaMedicoes()
{
  POSITION Atual(GetHeadPosition());

  while (Atual)	delete GetNext(Atual);
}

/////////////////////////////////////////////////////////////////////////////
// CListaProjetoos message handlers

