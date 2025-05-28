#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "monologo.h"
#include "supercstring.h"
#include <math.h>
#include <string>
#include "CPonto.h"
#include "estaca.h"
#include "CLPontos.h"
#include <list>
#include "csecaoavulsa.h"

#define LER 1
#define GRAVAR 2

CSecaoAvulsa::CSecaoAvulsa(const std::string& pNome,
						   const std::string& XIni,
						   const std::string& YIni,
						   const std::string& XFim,
						   const std::string& YFim,
						   const std::string& pAzimute,
	                       const std::string& pDistancia,
                           int pTipo) : 
                           Nome(pNome),
						   PInicial(atof(XIni.c_str()),atof(YIni.c_str())), 
						   PFinal(atof(XFim.c_str()),atof(YFim.c_str())),
						   Azimute(atof(pAzimute.c_str())),
						   Distancia(atof(pDistancia.c_str())),
                           Tipo(pTipo)
{
}

CSecaoAvulsa& CSecaoAvulsa::operator=(CSecaoAvulsa &No)
{
  Nome = No.Nome;
  PInicial = No.PInicial;   
  PFinal = No.PFinal;
  Azimute = No.Azimute;
  Distancia = No.Distancia;
  Tipo = No.Tipo;

  RemoveAll();
  
  for (POSITION Atual = No.GetHeadPosition(); Atual != NULL ; No.GetNext(Atual))
  {
    AddTail(No.GetAt(Atual));
  }

  AcidentesTopograficos.RemoveAll();

  for (POSITION Atual = No.AcidentesTopograficos.GetHeadPosition() ; Atual != NULL ; No.AcidentesTopograficos.GetNext(Atual))
  {
    AcidentesTopograficos.AddTail(No.GetAt(Atual));
  }

  LstPontosProjeto.RemoveAll();

  for (POSITION Atual = No.LstPontosProjeto.GetHeadPosition(); Atual != NULL; No.LstPontosProjeto.GetNext(Atual))
  {
    LstPontosProjeto.AddTail(No.LstPontosProjeto.GetAt(Atual));
  }

  AlinhamentosTerrap.RemoveAll();

  for (POSITION Atual = No.AlinhamentosTerrap.GetHeadPosition(); Atual != NULL; No.AlinhamentosTerrap.GetNext(Atual))
  {
    AlinhamentosTerrap.AddTail(No.AlinhamentosTerrap.GetAt(Atual));
  }

  return *this;
}

CArchive& CSecaoAvulsa::operator>>(CArchive& ar)
{
  ar << Nome;
  PInicial >> ar;
  PFinal >> ar;
  ar << Azimute << Distancia << Tipo <<  GetCount();

	for (POSITION Atual=GetHeadPosition() ; Atual != NULL ; GetNext(Atual))
	{
		GetAt(Atual) >> ar;
	}

  ar << AcidentesTopograficos.GetSize();

  for (POSITION Atual = AcidentesTopograficos.GetHeadPosition(); Atual != NULL; AcidentesTopograficos.GetNext(Atual))
  {
    AcidentesTopograficos.GetAt(Atual) >> ar;
  }
  
  ar << LstPontosProjeto.GetCount();

  for (POSITION Atual = LstPontosProjeto.GetHeadPosition(); Atual != NULL; LstPontosProjeto.GetNext(Atual))
  {
    LstPontosProjeto.GetAt(Atual) >> ar;
  }
  
  ar << AlinhamentosTerrap.GetSize();

  for (POSITION Atual = AlinhamentosTerrap.GetHeadPosition(); Atual != NULL; AlinhamentosTerrap.GetNext(Atual))
  {
    AlinhamentosTerrap.GetAt(Atual) >> ar;
  }

  return ar;
}

CArchive& CSecaoAvulsa::operator<<(CArchive& ar)
{
  ar >> Nome;
  PInicial << ar;
  PFinal << ar;
  ar >> Azimute >> Distancia >> Tipo;

  size_t QtdPontos;
  RemoveAll();

  for (ar >> QtdPontos; QtdPontos != 0; --QtdPontos)
  {
 	Ponto PontoAtual;
	PontoAtual << ar;

   AddTail(PontoAtual);
  }

  AcidentesTopograficos.RemoveAll();

  for (ar >> QtdPontos; QtdPontos != 0; --QtdPontos)
  {
    Ponto PontoAtual;
    PontoAtual << ar;

    AcidentesTopograficos.AddTail(PontoAtual);
  }
  
  LstPontosProjeto.RemoveAll();

  for (ar >> QtdPontos; QtdPontos != 0; --QtdPontos)
  {
    Ponto PontoAtual;
    PontoAtual << ar;

    LstPontosProjeto.AddTail(PontoAtual);
  }
  
  AlinhamentosTerrap.RemoveAll();

  for (ar >> QtdPontos; QtdPontos != 0; --QtdPontos)
  {
    Ponto PontoAtual;
    PontoAtual << ar;

    AlinhamentosTerrap.AddTail(PontoAtual);
  }

  return ar;
}

void CSecaoAvulsa::RotacionaPlano(double Sin,double Cos,double Angulo)
{
  PInicial.RotacionaPlano(Sin,Cos,Angulo);
  PFinal.RotacionaPlano(Sin,Cos,Angulo);
  Azimute += Angulo;
}

void CSecaoAvulsa::Serialize(CArchive & ar)
{
  if(ar.IsStoring()) this->operator>>(ar);
  else this->operator<<(ar);
}

#define ESQUERDO 0
#define DIREITO 1 

void CSecaoAvulsa::AcertaOffSets()
{
  if (LstPontosProjeto.GetCount() > 1 && GetCount() > 1)
  {
    Ponto OffSet;

    //---  Lado Esquerdo

    POSITION POSPROJ(LstPontosProjeto.GetHeadPosition());
    Ponto& PProjAtualEsq(LstPontosProjeto.GetNext(POSPROJ)), PProjPostEsq(LstPontosProjeto.GetAt(POSPROJ));

    POSITION PTerrAtual(GetHeadPosition()),PTerrAnterior(PTerrAtual);
    bool Achou(false);

    GetNext(PTerrAtual);

      while (PTerrAtual != NULL && !Achou)
      {
        Ponto& PAnterior(GetAt(PTerrAnterior)), & PAtual(GetAt(PTerrAtual));

        //--- Aqui  o ponto tem que estar no terreno não necessariamente no projeto, porisso o terreno vai no lugar do projeto e vice versa (ultimo parametro = true)

        Achou = CLPontos::CalculaIntersecao(PProjAtualEsq, PProjPostEsq, PAtual, PAnterior, OffSet, false);

        //--- O Ponto que que estar no terreno não obrigatoriamente no projeto

        Achou &= (OffSet.x > (PAnterior.x - 1E-6) && OffSet.x < (PAtual.x + 1E-6));

        if (!Achou)
        {
          GetNext(PTerrAtual);
          GetNext(PTerrAnterior);
        }
      }

      if (Achou)
      {
        //--- Acerta o offset esquerdo

        PProjAtualEsq.x = OffSet.x;
        PProjAtualEsq.y = OffSet.y;
      }

      POSPROJ = LstPontosProjeto.GetTailPosition();
    Ponto& PProjPostDir(LstPontosProjeto.GetPrev(POSPROJ)),& PProjAntDir(LstPontosProjeto.GetAt(POSPROJ));

      PTerrAtual = GetHeadPosition(), PTerrAnterior = PTerrAtual;

      Achou = false;

    GetNext(PTerrAtual);

    while (PTerrAtual != NULL && !Achou)
    {
      Ponto& PAnterior(GetAt(PTerrAnterior)), & PAtual(GetAt(PTerrAtual));

      //--- Aqui  o ponto tem que estar no terreno não necessariamente no projeto, porisso o terreno vai no lugar do projeto e vice versa (ultimo parametro = true)

      Achou = CLPontos::CalculaIntersecao(PProjAntDir, PProjPostDir, PAnterior, PAtual, OffSet, false);

      //--- O Ponto que que estar no terreno não obrigatoriamente no projeto

      Achou &= ( OffSet.x > (PAnterior.x - 1E-6) && OffSet.x < (PAtual.x + 1E-6));    

      if (!Achou)
      {
        GetNext(PTerrAtual);
        GetNext(PTerrAnterior);
      }
    }

    if (Achou)
    {
      //--- Acerta o offset direito

      PProjPostDir.x = OffSet.x;
      PProjPostDir.y = OffSet.y;
    }
  }
}
