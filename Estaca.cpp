#include "stdafx.h"
#include "afxtempl.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include <vector>
#include <iostream>
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MENOR -1
#define MAIOR  1
#define IGUAL  0

double CEstaca::ERRO_MINIMO = 0.004999999;

/////////////////////////////////////////////////////////////////////////////
// Cestaca

//--- Construtores sobrecarregados e complementares, se vier um CString cria o double, se vier 
//--- um double Cria o string. Quando criar sem valor (construtor default)
//--- O construtor Atrasado (<constrói()>) deverá ser chamado para atribuir valores à estaca, a função
//--- <constrói>, tambem está sobrecarregada para aceitar os 2 tipos, double e string.

CEstaca::CEstaca() :CString("+INFINITO"), EstReal(INFINITO),EstVirtual(INFINITO), Cota(INFINITO),Sinalizadores(NENHUM_SINAL) {}

CEstaca::CEstaca(const CString& Estaca,const CString& Ig,const CString& Descr,double pCota):CString(Estaca),Cota(pCota),Descricao(Descr),Igualdade(Ig),Sinalizadores(NENHUM_SINAL)
{
  ParaDouble(Igualdade);
  ParaString();                    //--- Desnecessario ??? verificar pois ao transformar a estaca para string teoricamente 
                                   //--- teria que ficar o mesmo string que veio
}

CEstaca::CEstaca(const CEstaca& Estaca,const CString& Desc)
{
  *this = Estaca;
  Descricao = Desc;
}

CEstaca::CEstaca(const double Estaca,const double CotaEst,const int Ig,const CString& Tipo,unsigned long Sinais):CString(), EstReal(Estaca),EstVirtual(Estaca), Cota(CotaEst),Descricao(Tipo),Sinalizadores(Sinais)
{
  Igualdade.Format("%i",Ig);
  ParaString();
}

void CEstaca::Constroi(const double Estaca,const double cota,const int Ig,const CString& Tipo)
{
  Descricao = Tipo;
  EstReal = EstVirtual = Estaca;  
  Cota = cota;
  Igualdade.Format("%i",Ig);
  ParaString();
}

void CEstaca::Constroi(const char *Estaca,const CString& Ig)
{
  Cota = INFINITO;
  CString::operator=(Estaca);
  Igualdade = Ig;
  ParaDouble(Igualdade);
  ParaString();
}

void CEstaca::Constroi(const CString& Estaca,const CString& PCota,const CString& Ig,const CString& Descr)
{
  Descricao = Descr;
  Cota = atof(PCota);
  CString::operator=(Estaca);
  Igualdade = Ig;
  ParaDouble(Igualdade);
  ParaString();
}

CEstaca::CEstaca(const char *Estaca,const CString& Ig)
{
  Cota = INFINITO;
  CString::operator=(Estaca);
  Igualdade = Ig;
  ParaDouble(Igualdade);
  ParaString();
  Sinalizadores = NENHUM_SINAL;
}

void CEstaca::ParaString()
{
  CString Estacastr;
  bool EstNegativa(EstReal < 0.00);


  //--- Faixa permitida para a estaca -1000000.0 a 1000000.0.

  //  if (EstReal < 1000000.0 || 1)
  //{
  //if (EstReal > - 1000000.0 || 1)

  if(EstReal != INFINITO)
  {
    if(EstReal != -INFINITO)
    {
      char Temp[50];

      if (EstNegativa) EstReal = fabs(EstReal);

      //--- Separa as partes inteira e fracionária do numero, tranforma para string 
      //--- e concatena as duas partes no buffer.

      int EstacaInt = (int) floor((EstReal / 20.0) + 1e-10);
      double EstacaFrac = EstReal - EstacaInt * 20.0;

      sprintf((char *) &Temp,EstNegativa ? "-(" :"");
      Estacastr = Temp;
      sprintf((char *) &Temp,"%5d+",EstacaInt);
      Estacastr += Temp;
      sprintf((char *) &Temp,"%5.2lf",EstacaFrac);
      Estacastr += Temp;
      sprintf((char *) &Temp,EstNegativa ? ")" :"");
      Estacastr += Temp;

      if (EstNegativa)EstReal = - EstReal ;

      if (atoi(Igualdade) > 0)
      { 
      //  CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
      //  MainFrame->m_ProjetosBox.GetLBText(0,MainFrame->m_sNomeComp);// ===>>>>> Verificar!!!!!
       // CProjeto* ProjAtual = MainFrame->AchaProjetoNoVetor();
    
    //    if (ProjAtual)  EstVirtual += ProjAtual->Igualdades.BuscaDifIgualdade(atoi(Igualdade));
      }
    }
    else Estacastr = "-INFINITO";
  }
  else Estacastr = "+INFINITO";

  //--- Passa a estaca para o buffer.  

  CString::operator=(Estacastr);
}

void CEstaca::ParaDouble(const CString& Ig)
{
  //--- Passa o string para double 

  SuperCString Frac;

  SuperCString Int = Left(Find('+'));

  Int.TrimLeft();
  Int.TrimRight();

  Int.Limpa("( ");
  bool EstNegativa(Int.IsEmpty() ? false : Int.GetAt(0) =='-');

  if (Int.IsEmpty()) Int = (SuperCString) Right(GetLength());
  else Frac = (SuperCString) Right(GetLength()-Find('+')-1);

  Frac.TrimLeft();
  Frac.TrimRight();
  Frac.Limpa(") ");

  int Tam(Frac.Find(' '));
  Frac.Left(Tam == -1 ? Frac.GetLength() : Tam);

  if (Int.IsEmpty() || !e_numeroInt(Int) || !Frac.ENumero())
  {
    EstReal = EstVirtual = INFINITO;
    return;
  }

  EstReal = EstVirtual = atof(Int) * 20.0 + (EstNegativa ? -atof(Frac) : atof(Frac));

  if (atoi(Ig) > 0) 
  {
   // CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
   // CProjeto* ProjAtual = MainFrame->AchaProjetoNoVetor();
   // EstVirtual += ProjAtual->Igualdades.BuscaDifIgualdade(atoi(Ig));
  }
}

//--- Operadores (not in line)

const double &CEstaca::operator=(const double& Valor)
{
  EstReal = EstVirtual = Valor;
  Igualdade = "0";
  ParaString();
  Sinalizadores = 0;
  return EstReal;
}

const CEstaca &CEstaca::operator=(const CEstaca& Estaca)
{
  CString::operator=(Estaca);
  EstReal = Estaca.EstReal; 
  EstVirtual = Estaca.EstVirtual;
  Cota = Estaca.Cota; 
  Descricao = Estaca.Descricao;
  Igualdade = Estaca.Igualdade;
  Sinalizadores = Estaca.Sinalizadores;

  return *this;
}

CArchive& CEstaca::operator>>(CArchive& ar) 
{
  CString Estacastr(*this);

  ar << Estacastr << EstVirtual << EstReal << Igualdade << Cota << Descricao << Sinalizadores;

  return ar;
}

CArchive& CEstaca::operator<<(CArchive& ar)
{
  CString Desc,Estacastr;

  ar >> Estacastr >> EstVirtual >> EstReal >> Igualdade >> Cota >> Descricao >> Sinalizadores; 

  CString::operator=(Estacastr);

  return ar;
}

int CEstaca::Compara(const CEstaca& Est)
{
  return EstVirtual > Est.EstVirtual ? MAIOR : EstVirtual < Est.EstVirtual ? MENOR : IGUAL; 
}

void CEstaca::AlteraValor(double Valor,double PCota,int Ig)
{
  Cota = PCota;
  EstVirtual = EstReal = Valor;
  Igualdade.Format("%i",Ig);

  ParaString();
}


