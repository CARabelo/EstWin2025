#include "stdafx.h"
#include <set>
#include "CTipoSoloSondagem.h"
#include "Estaca.h"
#include "CFuroSondagem.h"
#include "FurosSondagem.h"
#include <algorithm>
#include "CPonto.h"
#include "CLPontos.h"
#include "Perfil.h"
#include <list>
#include <map>
#include "CPerfilGeologico.h"
#include <sstream>
#include <iostream>  
#include <iomanip> 
#include "Monologo.h"

#define GRAVAR 1
#define LER 0   

extern class monologo monolog;

typairittyPerfilTSrsetPTS CPerfilGeologico::VerfPontoPerfilDesenho(const CEstaca& pEstaca,double Toleranciaxy_2,ittysetCPerfilTipoSolo* itPTSAtual,double Escalay) 
{
  static typairittyPerfilTSrsetPTS pairResposta;
  ittysetCPerfilTipoSolo itPerfisSolos(PerfisSolos.begin());

  while(itPerfisSolos != PerfisSolos.end()) 
  {
    if(itPTSAtual == NULL || *itPTSAtual == PegaPerfisSolos().end() || itPerfisSolos != *itPTSAtual || itPerfisSolos->PegaPerfil().size() > 1)           //--- Pula o perfil atual e o perfil tem q ter no minimo 2 pontos
    {
      ittyPerfilTipoSolo itProximoPonto,itPontoAnterior;
      static ittyPerfilTipoSolo itPontoMaisProximo;

      itProximoPonto = itPerfisSolos->PegaPerfil().upper_bound(pEstaca);
      itPontoAnterior = itPerfisSolos->PegaPerfil().lower_bound(pEstaca);

      if(itProximoPonto == itPerfisSolos->PegaPerfil().end()) itProximoPonto = --itPerfisSolos->PegaPerfil().end();   //-- Pode Estar proximo do ultimo ponto
      if(itPontoAnterior == itPerfisSolos->PegaPerfil().end()) itPontoAnterior = itPerfisSolos->PegaPerfil().begin();  //--- Pode Estar Proximo do primeiro ponto

 //     double d1(itProximoPonto !=itPerfisSolos->PegaPerfil().end() ?  (pow(itProximoPonto->EstVirtual-pEstaca.EstVirtual,2.0) + pow((itProximoPonto->Cota-pEstaca.Cota)*Escalay,2.0)) : INFINITO),
 //            d2(itPontoAnterior !=itPerfisSolos->PegaPerfil().end() ? (pow(itPontoAnterior->EstVirtual-pEstaca.EstVirtual,2.0) + pow((itPontoAnterior->Cota-pEstaca.Cota)*Escalay,2.0)) : INFINITO),
 //            MenorDistancia(INFINITO);

      double d1(itProximoPonto !=itPerfisSolos->PegaPerfil().end() ?  fabs(itProximoPonto->EstVirtual-pEstaca.EstVirtual) : INFINITO),
             d2(itPontoAnterior !=itPerfisSolos->PegaPerfil().end() ? fabs(itPontoAnterior->EstVirtual-pEstaca.EstVirtual) : INFINITO),
             MenorDistancia(INFINITO);


      if(d1 < d2) 
      {
        if(d1 < Toleranciaxy_2 && fabs(itProximoPonto->Cota - pEstaca.Cota) < (Toleranciaxy_2/10))
        {
          itPontoMaisProximo = itProximoPonto;
          MenorDistancia = d1;
        }
      }
      else 
      {
        if(d2 < Toleranciaxy_2  && fabs(itPontoAnterior->Cota - pEstaca.Cota) < (Toleranciaxy_2/10))
        {
          itPontoMaisProximo = itPontoAnterior;
          MenorDistancia = d2;
        }
      }

      if(MenorDistancia < Toleranciaxy_2)
      {
        pairResposta.first = &itPontoMaisProximo;
        pairResposta.second = itPerfisSolos;

        return pairResposta;
      }
    }

    ++itPerfisSolos;
  }

  pairResposta.first = (ittyPerfilTipoSolo*)NULL;
  return pairResposta;
}

 void CPerfilGeologico::ColocaTipoNoPerfil(const std::string& Tipo,ittysetCPerfilTipoSolo& itPerfil,int pTipoTraco,COLORREF pCor)
 {
   if(Tipo != itPerfil->PegaTipoSolo())
   {
     ittysetCPerfilTipoSolo itPerfilAtual(IncluiPerfilSolo(Tipo,itPerfil->PegaPontoInicial(),pTipoTraco,pCor).first);

     for(register ittyPerfilTipoSolo it = itPerfil->PegaPerfil().begin() ; it != itPerfil->PegaPerfil().end() ; ++it) 
     {
       IncluiPontoPerfil(CEstaca(it->EstVirtual,it->Cota,0,Tipo.c_str(),it->Sinalizadores),itPerfilAtual);
     }

     RemovePerfil(itPerfil);

     itPerfil = itPerfilAtual;
   }
 }

ittysetCPerfilTipoSolo CPerfilGeologico::VerfPontoPerfil(const CEstaca& Estaca,double Toleranciax,double Toleranciay) 
{
  for (ittysetCPerfilTipoSolo it = PerfisSolos.begin() ; it != PerfisSolos.end() ; ++it)
  {
    bool Achou(false);
    ittyPerfilTipoSolo itTSAnterior(it->PegaPerfil().begin()),itTSPosterior(itTSAnterior);

    while(!Achou && ++itTSPosterior != it->PegaPerfil().end())
    {
      if((itTSAnterior->EstVirtual - Toleranciax) < Estaca.EstVirtual && (itTSPosterior->EstVirtual + Toleranciax) > Estaca.EstVirtual)
      {
        double Cota((((itTSPosterior->Cota - itTSAnterior->Cota) / (itTSPosterior->EstVirtual- itTSAnterior->EstVirtual)) * 
                     (Estaca.EstVirtual - itTSAnterior->EstVirtual)) + itTSAnterior->Cota);

        if(fabs(Cota -Estaca.Cota) < Toleranciay)    
        {
          return it;
        }
      }
      ++itTSAnterior;
    }
  }
  return PerfisSolos.end();
}
  
void CPerfilGeologico::PegaTrecho(ittysetCPerfilTipoSolo itPerfil,const CEstaca& Ponto,ittyPerfilTipoSolo* ppitLB,ittyPerfilTipoSolo* ppitUB) const
{
  static ittyPerfilTipoSolo itLB,itUB;

  itLB = itPerfil->PegaPerfil().upper_bound(Ponto);
  itUB = itLB;

  if(itLB == itPerfil->PegaPerfil().begin() || itLB == itPerfil->PegaPerfil().end())                  //--- fora do trecho
  {
    ppitLB = ppitUB = NULL;
    return;    
  }

  --itLB;

  ppitLB = &itLB;
  ppitUB = &itUB;
}

void CPerfilGeologico::CriaPerfisNosTrechos(ittysetEstaca itInicioTrecho,ittysetEstaca itFimTrecho,Ponto Deltas)
{
  ittysetFurosSondagem itFuroSondagemAtual(FurosSondagem.PegaSetFurosSondagem().begin());

  ittysetEstaca itPontoAtual(itInicioTrecho); 

  while(itFuroSondagemAtual != FurosSondagem.PegaSetFurosSondagem().end() && itFuroSondagemAtual->PegaEstaca().EstVirtual < (itInicioTrecho->EstVirtual- 5.0)) ++itFuroSondagemAtual;

  ittysetCPerfilTipoSolo itPerfilAtual(IncluiPerfilSolo("Desconhecido",CEstaca(itPontoAtual->EstVirtual-Deltas.x,itPontoAtual->Cota-Deltas.y,0,"Desconhecido"),0,255).first);

  ++itPontoAtual;

  do
  {
    ++itPontoAtual;

    if(itFuroSondagemAtual != FurosSondagem.PegaSetFurosSondagem().end() && itPontoAtual->EstVirtual > (itFuroSondagemAtual->PegaEstaca().EstVirtual - 5.0)) 
    {
      while( itPontoAtual != itFimTrecho && itPontoAtual->EstVirtual < (itFuroSondagemAtual->PegaEstaca().EstVirtual + 5.0)) ++itPontoAtual; 

      if(itPontoAtual != itFimTrecho)
      {
        if(itPerfilAtual->PegaPerfil().size() < 2) RemovePerfil(itPerfilAtual);

        itPerfilAtual = IncluiPerfilSolo("Desconhecido",CEstaca(itPontoAtual->EstVirtual-Deltas.x,itPontoAtual->Cota-Deltas.y,0,"Desconhecido"),0,255).first;
      }

      ++itFuroSondagemAtual;
    }
    else
    {
      (const_cast<CPerfilTipoSolo*>( &*itPerfilAtual))->IncluiPonto(CEstaca(itPontoAtual->EstVirtual - Deltas.x,itPontoAtual->Cota - Deltas.y,0,"Desconhecido"));
    }
  }while( itPontoAtual != itFimTrecho);
}

bool CPerfilGeologico::FundePerfis(ittysetCPerfilTipoSolo itPerfilAtual,ittysetCPerfilTipoSolo PerfilaInserir)
{
  if(itPerfilAtual->PegaTipoSolo().compare(PerfilaInserir->PegaTipoSolo()) != 0)
  {
    if(itPerfilAtual->PegaTipoSolo().compare("Desconhecido") != 0)
    {
      for(register ittyPerfilTipoSolo itPontoPerfAtual = PerfilaInserir->PegaPerfil().begin() ; itPontoPerfAtual !=  PerfilaInserir->PegaPerfil().end() ; ++itPontoPerfAtual)
      {
        CPerfilTipoSolo* pPerfTipoSolo((const_cast<CPerfilTipoSolo*>(&*itPerfilAtual)));

        CEstaca PontoAtualPerfil(*itPontoPerfAtual);
        PontoAtualPerfil.Descricao = itPerfilAtual->PegaTipoSolo().c_str();   //--- Acerta os nomes do tipo de solos dos pontos
        pPerfTipoSolo->IncluiPonto(PontoAtualPerfil);
      //  pPerfTipoSolo->PegaPerfil().rbegin()->Descricao = itPerfilAtual->PegaTipoSolo().c_str();
 //      (const_cast<CPerfilTipoSolo*>(&*itPerfilAtual))->IncluiPonto(*itPontoPerfAtual);
      }

      PerfisSolos.erase(PerfilaInserir);  
    }
    else   //--- Vai Mudar a chave do set, tem que excluir o velho e inserir o novo
    {
      //--- Cria o novo perfil com o tipo do perfil a inserir (RHS) (LHS é do tipo desconhecido)

      CPerfilTipoSolo NovoPerfil(PerfilaInserir->PegaTipoSolo(),itPerfilAtual->PegaPontoInicial(),PerfilaInserir->PegaTraco(),PerfilaInserir->PegaCor()); 
      pairtysetCPerfilTipoSoloItBool pairNovoPerfil(PerfisSolos.insert(NovoPerfil));

      if(pairNovoPerfil.second == true)
      {
        //--- Insere os pontos já existentes perfil atual  (LHS) (Tipo Desconhecido)

        for(register ittyPerfilTipoSolo itPontoPerfAtual = itPerfilAtual->PegaPerfil().begin() ; itPontoPerfAtual !=  itPerfilAtual->PegaPerfil().end() ; ++itPontoPerfAtual)
        {
          (const_cast<CPerfilTipoSolo*>(&*pairNovoPerfil.first))->IncluiPonto(*itPontoPerfAtual);
        }

        //--- Insere os pontos do perfi a inserir (RHS)

        for(register ittyPerfilTipoSolo itPontoPerfAtual = PerfilaInserir->PegaPerfil().begin() ; itPontoPerfAtual !=  PerfilaInserir->PegaPerfil().end() ; ++itPontoPerfAtual)
        {
          (const_cast<CPerfilTipoSolo*>(&*pairNovoPerfil.first))->IncluiPonto(*itPontoPerfAtual);
        }

        PerfisSolos.erase(PerfilaInserir);          //--- Exclui o velho
        PerfisSolos.erase(itPerfilAtual);           //--- Exclui o novo com o tipo velho (Tipo velho = Desconhecido)
      }
    }
  }
  else    //--- Os 2 perfís têm o mesmo nome, insere o segundo no primeiro e remove o segundo
  {
    for(register ittyPerfilTipoSolo itPontoPerfInserir = PerfilaInserir->PegaPerfil().begin() ; itPontoPerfInserir !=  PerfilaInserir->PegaPerfil().end() ; ++itPontoPerfInserir)
    {
      (const_cast<CPerfilTipoSolo*>(&*itPerfilAtual))->IncluiPonto(*itPontoPerfInserir);
    }

    PerfisSolos.erase(PerfilaInserir);
  }

  return true;
}

bool CPerfilGeologico::Serialize(const std::string& NomeProjeto,int Tipo)
{
  CFile ArqPerfil;
  CFileException e;

  std::string NomeArquivo(NomeProjeto);
  NomeArquivo.append(".prg");

  if(Tipo == GRAVAR)
  {
    if (ArqPerfil.Open(NomeArquivo.c_str(),CFile::modeCreate | CFile::modeWrite | CFile::shareDenyNone,&e) == 0)
    {
       std::stringstream strstrTexto;
      strstrTexto << "Erro ao abrir o arquivo: " << NomeArquivo << std::ends;

      AfxMessageBox(strstrTexto.str().c_str());

      return false;
    }
    else
    {	
      CArchive ar(&ArqPerfil,CArchive::store);

      this->operator>>(ar);
    }
  }
  else
  {
    if (ArqPerfil.Open(NomeArquivo.c_str(),CFile::modeRead,&e) == 0)
    {
      if(e.m_cause != 2)
      {
        std::stringstream strstrTexto;
        strstrTexto << "Erro ao abrir o arquivo: " << NomeArquivo << std::ends;

        AfxMessageBox(strstrTexto.str().c_str());

        return false;
      }
    }
    else
    {	
      CArchive ar(&ArqPerfil,CArchive::load);

      this->operator<<(ar);
    }
  }
  
  return true;
}

void CPerfilGeologico::GravaDadosNotaServico(std::string& NomeProjeto)
{
  CFile ArqPerfGeoCalc;
  CFileException e;

  std::string NomeCompArquivo(NomeProjeto + ".pgc");

  if (ArqPerfGeoCalc.Open(NomeCompArquivo.data(),CFile::modeCreate | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(13,CString(NomeCompArquivo.data()) + " (Gravando os dados)");

    throw e.m_cause;
  }
  else
  {	
    CArchive ar(&ArqPerfGeoCalc,CArchive::store);

    ar <<  SetDadosPerfis.size(); 

    for(itsetCDadosPerfGeologicos it = SetDadosPerfis.begin() ; it != SetDadosPerfis.end() ; ++it)
	  {
	    const_cast<CDadosPerfisGeologicos*>(&*it)->Serialize(ar);
	  }

    ar.Close();
    ArqPerfGeoCalc.Close();
  }
}

void CPerfilGeologico::LeDadosNotaServico(const std::string& NomeProjeto)
{
  CFile ArqPerfGeoCalc;
  CFileException e;

  std::string NomeCompArquivo(NomeProjeto + ".pgc");

  if (ArqPerfGeoCalc.Open(NomeCompArquivo.data(),CFile::modeRead,&e) == 0)
  {
    monolog.mensagem(13,CString(NomeCompArquivo.data()) + " (Lendo os dados)");

    throw e.m_cause;
  }
  else
  {	
    CArchive ar(&ArqPerfGeoCalc,CArchive::load);

	SetDadosPerfis.clear();

	std::size_t QtdPerfis(0);
    
    for(ar >>  QtdPerfis ; QtdPerfis > 0 ; --QtdPerfis)
	{
	  CDadosPerfisGeologicos PerfilAtual;
	  PerfilAtual.Serialize(ar);
	  SetDadosPerfis.insert(PerfilAtual);
	}

    ar.Close();
    ArqPerfGeoCalc.Close();
  }
}

void CPerfilGeologico::ReleDados(const std::string& NomeArquivo)
{
  SetDadosPerfis.clear();
  Serialize(NomeArquivo,LER);
}

void CPerfilGeologico::CriaPerfisGeologicosNosTrechos(ittyPerfilTipoSolo itInicioTrecho,ittyPerfilTipoSolo itFimTrecho,Ponto Deltas)
{
  ittysetFurosSondagem itFuroSondagemAtual(FurosSondagem.PegaSetFurosSondagem().begin());

  ittyPerfilTipoSolo itPontoAtual(itInicioTrecho); 

  while(itFuroSondagemAtual != FurosSondagem.PegaSetFurosSondagem().end() && itFuroSondagemAtual->PegaEstaca().EstVirtual < (itInicioTrecho->EstVirtual- 5.0)) ++itFuroSondagemAtual;

  ittysetCPerfilTipoSolo itPerfilAtual(IncluiPerfilSolo("Desconhecido",CEstaca(itPontoAtual->EstVirtual-Deltas.x,itPontoAtual->Cota-Deltas.y,0,"Desconhecido"),0,255).first);

  ++itPontoAtual;

  do
  {
    ++itPontoAtual;

    if(itFuroSondagemAtual != FurosSondagem.PegaSetFurosSondagem().end() && itPontoAtual->EstVirtual > (itFuroSondagemAtual->PegaEstaca().EstVirtual - 5.0)) 
    {
      while( itPontoAtual != itFimTrecho && itPontoAtual->EstVirtual < (itFuroSondagemAtual->PegaEstaca().EstVirtual + 5.0)) ++itPontoAtual; 

      if(itPontoAtual != itFimTrecho)
      {
        if(itPerfilAtual->PegaPerfil().size() < 2) RemovePerfil(itPerfilAtual);

        itPerfilAtual = IncluiPerfilSolo("Desconhecido",CEstaca(itPontoAtual->EstVirtual-Deltas.x,itPontoAtual->Cota-Deltas.y,0,"Desconhecido"),0,255).first;
      }

      ++itFuroSondagemAtual;
    }
    else
    {
      (const_cast<CPerfilTipoSolo*>( &*itPerfilAtual))->IncluiPonto(CEstaca(itPontoAtual->EstVirtual - Deltas.x,itPontoAtual->Cota - Deltas.y,0,"Desconhecido"));
    }
  }while( itPontoAtual != itFimTrecho);
}
/*
bool CPerfilGeologico::PegaTrechoEntreFurosSondagem(CEstaca& Local,ittysetCPerfilTipoSolo itPerfil,ittyPerfilTipoSolo& itInicioTrecho,ittyPerfilTipoSolo& itFimTrecho)
{
  if(FurosSondagem.PegaSetFurosSondagem().size() == 0) return false;

  itInicioTrecho = itFimTrecho = itPerfil->PegaPerfil().end();

  ittysetFurosSondagem itFuroSondagemAnterior(FurosSondagem.PegaSetFurosSondagem().upper_bound(Local)),itFuroSondagemPosterior(itFuroSondagemAnterior);

  if(itFuroSondagemAnterior != FurosSondagem.PegaSetFurosSondagem().end()) --itFuroSondagemAnterior;       

  --itFuroSondagemAnterior;

  itInicioTrecho = itPerfil->PegaPerfil().upper_bound(itFuroSondagemAnterior->PegaEstaca());

  if(itFuroSondagemPosterior == FurosSondagem.PegaSetFurosSondagem().end()) itFimTrecho = itPerfil->PegaPerfil().end();
  else itFimTrecho = itPerfil->PegaPerfil().upper_bound(*itFimTrecho);

  --itFimTrecho;

  return true;
}
*/


 const Ponto& CPerfilGeologico::PegaIniFimLimpeza(const Perfil& SecaoLimpeza,Perfil& SecaoPerfilGeologico,double hLimpeza)
 {
   static Ponto IniFim;
   
   IniFim.x = -INFINITO;
   IniFim.y = INFINITO;

   if(SecaoLimpeza.GetCount() > 1)
   {
     IniFim.x = SecaoLimpeza.GetHead().x;
     IniFim.y = SecaoLimpeza.GetTail().x;
   }

   Ponto PontoInterpolado,IniFimLimpeza;

   if (IniFim.x != INFINITO) 
   {
     PontoInterpolado = SecaoPerfilGeologico.Interpola(IniFim.x,CString("LIM"));
     PontoInterpolado.x += 5e-4;
     SecaoPerfilGeologico.InsereOrdenada(PontoInterpolado);

     IniFimLimpeza.x = PontoInterpolado.x;

   }
   if (IniFim.y != INFINITO) 
   {
     PontoInterpolado = SecaoPerfilGeologico.Interpola(IniFim.y,CString("LIM"));
     PontoInterpolado.x -= 5e-4;
     SecaoPerfilGeologico.InsereOrdenada(PontoInterpolado);

     IniFimLimpeza.y = PontoInterpolado.x;
   }

   IniFimLimpeza.z = hLimpeza;

   SecaoPerfilGeologico.AlteraFaixaY(IniFimLimpeza);   //-- Desconta a limpeza

     /*
  



   POSITION Atual(SecaoTerrenoPrimitivo.GetHeadPosition());



   while(Atual && SecaoTerrenoPrimitivo.GetAt(Atual).Descricao != "LIM") SecaoTerrenoPrimitivo.GetNext(Atual);

   if(Atual)
   {
     SecaoPerfilGeologico.InsereOrdenada(Ponto(SecaoTerrenoPrimitivo.GetAt(Atual).x,SecaoTerrenoPrimitivo.GetAt(Atual).y-hLimpeza),true);
     IniFim.x = SecaoTerrenoPrimitivo.GetNext(Atual).x;

     SecaoTerrenoPrimitivo.GetNext(Atual);

     while(Atual && SecaoTerrenoPrimitivo.GetAt(Atual).Descricao != "LIM") 
     {
       SecaoTerrenoPrimitivo.GetNext(Atual);
     }

     if(Atual != NULL) 
     {
       SecaoPerfilGeologico.InsereOrdenada(Ponto(SecaoTerrenoPrimitivo.GetAt(Atual).x+0.001,SecaoTerrenoPrimitivo.GetAt(Atual).y),true);
       IniFim.y = SecaoTerrenoPrimitivo.GetAt(Atual).x;
     }
   }
   */
   return IniFim;
 }
 
void CPerfilGeologico::AjustaLimpeza(const Perfil& SecaoLimpeza,Perfil& SecaoPerfilGeologico)
{
  if(SecaoLimpeza.GetCount() > 1)
  {
    Ponto IniFimLimpeza(SecaoLimpeza.GetHead().x,SecaoLimpeza.GetTail().x);

    SecaoPerfilGeologico.RemoveFaixa2(SecaoLimpeza.GetHead().x-0.001,SecaoLimpeza.GetTail().x+0.001);

    for(POSITION Atual = SecaoLimpeza.GetHeadPosition() ; Atual ; SecaoLimpeza.GetNext(Atual)) 
    {
      SecaoPerfilGeologico.InsereOrdenada(SecaoLimpeza.GetAt(Atual),false,true);
    }
  }
}
