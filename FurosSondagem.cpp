#include "stdafx.h"
#include <set>
#include "Estaca.h"
#include "CTipoSoloSondagem.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include <string>
#include "SuperCString.h"
#include "CArqFurosSondagem.h"

CFurosSondagem::CFurosSondagem(const std::string& NomArq):strNomeArquivo(NomArq)
{
  CArqFurosSondagem ArqFurosSondagem(NomArq);

  CEstaca EstacaAnterior;
  paitysetFurosSondagem parSetFuros(SetFurosSondagem.end(),false);

  for(POSITION Atual=ArqFurosSondagem.GetHeadPosition() ; Atual != NULL ; ArqFurosSondagem.GetNext(Atual))
  {
    if(ArqFurosSondagem.GetAt(Atual).ListaCampos.size() > 2)
    {
      ittlstwstr It(ArqFurosSondagem.GetAt(Atual).ListaCampos.begin()); 
   
      CEstaca EstacaAtual(It->c_str());
      It++;
      EstacaAtual.Cota = atof(It->c_str());

      parSetFuros = SetFurosSondagem.insert(EstacaAtual);
   
      for( It++ ; It != ArqFurosSondagem.GetAt(Atual).ListaCampos.end() ; It++)
      {
        if(It!= ArqFurosSondagem.GetAt(Atual).ListaCampos.end())
        {
          std::string strTipoSolo(*It++),strProfundidade(*It);

          if(!strProfundidade.empty() && strTipoSolo.size() > 2)
          {
            (const_cast<CFuroSondagem*>(&*parSetFuros.first))->IncluiTipoSolo(CTipoSoloSondagem(strProfundidade.c_str(),strTipoSolo.c_str()));
          }
        }
        //else  logar erro-> Profundidade sem tipo de solo
      }
    }
    // else logar erro faltam as profundidades
  }
}

ittysetFurosSondagem CFurosSondagem::PegaFuroMaisProximo(double Estaca,double ToleranciaEstaca,double Cota, double ToleranciaCota) const
{
  if(SetFurosSondagem.size() == 0) return SetFurosSondagem.end();

  ittysetFurosSondagem itPosterior(SetFurosSondagem.upper_bound(CEstaca(Estaca))),itAnterior(itPosterior),itFuroMaisProximo(SetFurosSondagem.end());
  --itAnterior;

  double d1(itAnterior == SetFurosSondagem.end() ? INFINITO : fabs(Estaca - itAnterior->PegaEstaca().EstVirtual)),
         d2(itPosterior== SetFurosSondagem.end() ? INFINITO : fabs(Estaca - itPosterior->PegaEstaca().EstVirtual));

  if((d1 <= ToleranciaEstaca && d1 <= d2)) itFuroMaisProximo = itAnterior;
  else if(d2 <= ToleranciaEstaca) itFuroMaisProximo = itPosterior;

  if(itFuroMaisProximo == SetFurosSondagem.end()) return SetFurosSondagem.end();

  if(itFuroMaisProximo->PegaEstaca().Cota > (Cota - ToleranciaCota) && itFuroMaisProximo->PegaCotaFundo() < (Cota + ToleranciaCota)) return itFuroMaisProximo;

  return SetFurosSondagem.end();
}

