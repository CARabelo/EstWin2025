#include "stdafx.h"
#include <map>
#include "est.h"
#include "CExComboBox.h"

CExComboBox::CExComboBox(void* ppApp,int pContem) : pApp(ppApp),Conteudo(pContem)
{
}

CExComboBox::~CExComboBox()
{
}

BEGIN_MESSAGE_MAP(CExComboBox, CComboBox)
  ON_CONTROL_REFLECT(CBN_SELCHANGE, &CExComboBox::OnCbnSelchange)
END_MESSAGE_MAP()

void CExComboBox::OnCbnSelchange()
{
  char Temp[1024] = {0};
  GetLBText(GetCurSel(),Temp);

  if(Conteudo == ID_PROJETOS)  ((CEstApp*) pApp)->MudouProjetoSelecionado(std::string(Temp));
  else ((CEstApp*)pApp)->MudouRuaSelecionada(std::string(Temp));
}

