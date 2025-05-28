#include "stdafx.h"
#include "afxtempl.h"
#include "resource.h"
#include "Est.h"
#include "monologo.h"
#include "math.h"
#include "estaca.h"
#include "supercstring.h"
#include "cponto.h"
#include "clpontos.h"
#include "carqcurhorizontais.h" 
#include "perfil.h"
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include <map>
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurhorizontal.h"
#include "Mat.h"
#include "ccurhorizontais.h"
#include <string>
#include "CExcomboBox.h"
#include "Picture.h"
#include "DesCroquisView.h"
#include "MainFrm.h"
#include "carqcurverticais.h"
#include "ccurverticais.h"
#include "cestacasacalcular.h"
#include <winnetwk.h>
#include <vector>
#include <iostream>
#include "csstack.h"
#include "mat.h"
#include <list>
#include "DSecionar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define INFINITO      1.7976931348623158e+308
#define ESQUERDO      0
#define DIREITO       1
#define ESPIRAL       0
#define CIRCULAR      1
#define MENOR        -1
#define MAIOR         1
#define IGUAL         0
#define TRAS         -1
#define FRENTE        1
#define GRAVAR        2
#define GRAVAR_SO_DIGITADAS  3

extern class monologo monolog;
extern class Matem Mat;

//----------------------------------------------------------------
//--- Implementação do nó da lista.


//----------------------------------------------------------------------------------
//--- Implementação da lista
//--- Neste objeto são criadas duas listas na memória, uma lista com os dados do 
//--- arquivo e outra lista com os dados calculados, como este objeto é genérico,
//--- as duas listas ficam na memoria e devem ser destruídas pelo recipiente 
//--- a medida do necessário. Estas listas são dinâmicas, podem ser mudadas
//--- durante a vida do objeto. Caso se deseje gravar as alteraçòes feitas
//--- deve-se chamar o metodo Serialize().
//-----------------------------------------------------------------------------------

CCurHorizontais::CCurHorizontais(CString& pNomeArquivo,CSecTipo &SecTip) : CArqCurHorizontais(pNomeArquivo),SecoesTipo(SecTip),Canteiros(pNomeArquivo),
                 Erro(0), LimiteE(INFINITO),LimiteI(INFINITO),LimiteS(-INFINITO),LimiteD(-INFINITO),NomeProjeto(pNomeArquivo),AngRotacao(0.0)
{
 // PegaPIAZInicial();

  mapCurHorizontaisTS.clear();

  itAtual = mapCurHorizontaisTS.begin();

  if(Consiste()) return;  //--- Houve erro nos registros ou na abertura do arquivo

  CFile ArqCalcGeo; 
  CFileException e;

  std::string NomeCompArquivo(pNomeArquivo + ".chc");

  if (ArqCalcGeo.Open(NomeCompArquivo.data(),CFile::shareDenyNone | CFile::modeRead,&e) == 0) 
  {
    if (e.m_cause != CFileException::fileNotFound) 
    {
      monolog.mensagem(13,CString(NomeCompArquivo.data()));
      return;
    }
  }
  else
  {
    CArchive CArqCalcGeo(&ArqCalcGeo,CArchive::load);
    CCurHorizontais::operator>>(CArqCalcGeo) ;      //-- Serializa o cálculo geometrico das curvas horizontais
  }

  itAtual = mapCurHorizontaisTS.begin();
}
void CCurHorizontais::RotacionaPlano(double VUx,double VUy,double Angulo)
{
  //--- As coordenadas vão mudar, então os set ordenado por PI e CC ficarão corrompidos. Remove e insere novamente com as novas coordenadas
  //--- As estacas não vão mudar porisso o set ordenado por TS não precisa ser zerado

  mapCurHorizontaisPI.clear();
  mapCurHorizontaisCC.clear();

  for(register ItMCurHorizontaisTS itMCH = mapCurHorizontaisTS.begin() ; itMCH != mapCurHorizontaisTS.end() ; itMCH++)
  {
    itMCH->second.RotacionaPlano(VUx,VUy,Angulo) ;
    mapCurHorizontaisPI.insert(pairpCHPO(&itMCH->second.PI,&itMCH->second));
    mapCurHorizontaisCC.insert(pairpCHPO(&itMCH->second.CC,&itMCH->second));
  }

  AngRotacao += Angulo;

  if(fabs(AngRotacao) < Mat.UmGrau_Rad) AngRotacao = 0.0;
}

int CCurHorizontais::CalculaPontosNotaveis()
{
  ItMCurHorizontaisTS itCHAtual(mapCurHorizontaisTS.begin()),itCHAnterior;

  //--- Cria a lista de curvasHorizontais calculadas. 
  //--- Inicializa a lista criando o primeiro nó, neste nó não há cálculo de I1 e E.

  itCHAtual->second.EstPc = itCHAtual->second.EstPt = itCHAtual->second.EstSt = itCHAtual->second.EstTs = 
  itCHAtual->second.EstCs = itCHAtual->second.EstSc = itCHAtual->second.EstTs;  

  //--- Preenchimento apenas para validação da curva, na primeira curva o ac e raio são = 0.  

  itCHAtual->second.Lado = DIREITO;     //--- Tanto faz....
  itCHAtual->second.Tipo = CIRCULAR;    
  itCHAtual->second.Ac = 0.0;
  itCHAtual->second.Raio = 0.0;
  itCHAtual->second.Se = 0.0;
  itCHAtual->second.Sl = 0.0;

  SecTipo SecaoTipo;
  if (!SecoesTipo.BuscaSecTipo(itCHAtual->second.EstTs, SecaoTipo)) return FALTA_SEC_TIPO;

  itCHAtual->second.m1 = 0.0;
  itCHAtual->second.Ini = itCHAtual->second.EstTs.EstVirtual;
  itCHAtual->second.Fim = itCHAtual->second.EstSt.EstVirtual;
  AtualizaLimites(itCHAtual->second);
  itCHAnterior = itCHAtual;        //-- Primeira curva = curva anterior

  POSITION At(GetHeadPosition());  //--- Atual = Cabeca da lista.
  GetNext(At); 
 
  //--- Para cada um dos nós restantes da lista de Curvas lidas cria um nó na lista
  //--- de curvas calculadas, calculando os dados da curva.

  while (At)
  {
    ++itCHAtual;

    ArqCurHorizontais NoArqAtual;
    NoArqAtual = GetNext(At);

    if(NoArqAtual.ListaCampos.size() == 11)
    {
      ittlstwstr It(NoArqAtual.ListaCampos.begin());

      std::string EstTs(*It);
      It++;
      itCHAtual->second.EstTs.Constroi(EstTs.c_str(),CString(""),It->c_str(),CString("TS")); 
      It++;

      std::string EstSt(*It);
      It++;
      itCHAtual->second.EstSt.Constroi(EstSt.c_str(),CString(""),It->c_str(),CString("ST")); 
      It++;

      *It = ToUpper(*It);
      itCHAtual->second.Lado = (It->size() && *It->begin() == 'E' ) ? ESQUERDO : DIREITO;
      It++;

      std::string Tipo(ToUpper(*It)); 
      It++;

      itCHAtual->second.Ac = Mat.str_gms_rad(*It) * (itCHAtual->second.Lado == ESQUERDO ? -1.0 : 1.0);
      It++;

      itCHAtual->second.Raio = atof(It->c_str());
      It++;

      itCHAtual->second.Lc = atof(It->c_str());
      It++;

      itCHAtual->second.Se = atof(It->c_str()) / 100.0 ;
      It++;

      itCHAtual->second.Sl = atof(It->c_str());
      It++;

      if(itCHAtual->second.EstTs < itCHAnterior->second.EstSt)  
      {
        monolog.mensagem(26,CString("Estaca TS = ") + itCHAtual->second.EstTs);
        return CURVAS_TRANSPASSANDO;
      }

      //--- Se a curva for espiral, constroi o Sc e o Cs, se não, (a curva é 
      //--- circular) tranforma a curva em espiral

      itCHAtual->second.Tipo = Tipo == "E" && itCHAtual->second.Lc > 0.00 ? ESPIRAL : CIRCULAR;

      ConstroiEspiralDaCircular(itCHAtual->second);

      SecTipo SecaoTipo;
      if (!SecoesTipo.BuscaSecTipo(itCHAtual->second.EstTs, SecaoTipo)) return FALTA_SEC_TIPO;

      //--- A principio não há superelevação

      itCHAtual->second.Ini = itCHAtual->second.EstTs.EstVirtual;
      itCHAtual->second.Fim = itCHAtual->second.EstSt.EstVirtual;    
      itCHAtual->second.m1 = itCHAtual->second.m2 = INFINITO;

      //--- Para ter superelevação precisa ter Lc  

      CurHorizontal* CC(&(itCHAtual->second));

      if(itCHAtual->second.Lc > 0.0)
      { 
        double Se(itCHAtual->second.Se);

        // if(itCHAtual->second.Se > 0.00999999)  //--- Superelevação mínima 1%  ... nada disso.....
        {
          double iTangente(BuscaMenorRampaPista(itCHAtual->second.EstSt,SecaoTipo));

          itCHAtual->second.m1 = itCHAtual->second.m2 = Se / (itCHAtual->second.Lc);
          itCHAtual->second.Ini = itCHAtual->second.EstTs.EstVirtual - fabs(iTangente / itCHAtual->second.m1);
          itCHAtual->second.Fim = itCHAtual->second.EstSt.EstVirtual + fabs(iTangente / itCHAtual->second.m2);
        }
      }

      EstudaCaso(itCHAtual->second,itCHAnterior->second);     //-- Estuda o caso de superelevação, caso as curvas estejam muito próximas

      //mapCurHorizontaisTS.insert(*itCHAtual);   //--- Finalmente adiciona o nó na lista.    
      AtualizaLimites(itCHAtual->second);

      itCHAnterior = itCHAtual;
    }
  }

  return CALCULO_OK;
}

bool CCurHorizontais::InsereCurvaNosMapas(CurHorizontal& CurvaAtual)
{
  pairInsMCHTS itMTS(mapCurHorizontaisTS.insert(pairCHCE(CurvaAtual.EstTs,CurvaAtual)));

  if(itMTS.second == true)
  {
    itAtual = itMTS.first;    //--- agora é std::map tem que atualizar o itatual; 

    pairInsMCHPI itMPI(mapCurHorizontaisPI.insert(pairpCHPO(&itMTS.first->second.PI,&itMTS.first->second)));

    if(itMPI.second == true)
    {
      pairInsMCHCC itMCC(mapCurHorizontaisCC.insert(pairpCHPO(&itMTS.first->second.CC,&itMTS.first->second)));

      if(itMCC.second == true)
      {
        AtualizaLimites(CurvaAtual);

        return true;
      }
      else
      {
        mapCurHorizontaisTS.erase(itMTS.first);
        mapCurHorizontaisPI.erase(itMPI.first);
      }
    }
    else mapCurHorizontaisTS.erase(itMTS.first);
  }
  
  return false;
}

bool CCurHorizontais::RemoveCurvaDosMapas(ItCMCurHorizontaisPI ItCCurvaPIRemover,ItMCurHorizontaisTS ItUltimaCurvaAjustadaTS,CEstaca* pNovaEstCurvaAnterior)
{
  ItMCurHorizontaisTS itMapTS(mapCurHorizontaisTS.find(ItCCurvaPIRemover->second->EstTs)),itMapTsAnte(itMapTS);

  if(itMapTS == mapCurHorizontaisTS.end()) return false;

  --itMapTsAnte;
  ItUltimaCurvaAjustadaTS = mapCurHorizontaisTS.end();
  ItUltimaCurvaAjustadaTS = itMapTS;
  ++ItUltimaCurvaAjustadaTS;
  if(itMapTsAnte != mapCurHorizontaisTS.end()) *pNovaEstCurvaAnterior = itMapTsAnte->second.EstTs;
  else pNovaEstCurvaAnterior = NULL;

  mapCurHorizontaisPI.erase(&itMapTS->second.PI);
  mapCurHorizontaisCC.erase(&itMapTS->second.CC);
  mapCurHorizontaisTS.erase(itMapTS);
  
  return true;
}

CArchive& CCurHorizontais::operator<<(CArchive& ar)
{
  ar << AngRotacao;
  ar << mapCurHorizontaisTS.size();

  for(register ItMCurHorizontaisTS itMCH = mapCurHorizontaisTS.begin() ; itMCH != mapCurHorizontaisTS.end() ; itMCH++)
  {
    CurHorizontal CurvaAtual = itMCH->second;
    itMCH->second >> ar;
  }
  return ar; 
}

int CCurHorizontais::Serialize(const CString& NomeProj,int Tipo)
{
  if(Tipo != GRAVAR_SO_DIGITADAS)
  {
    CFile ArqCurHoriz;
    CString NomeArquivoX(NomeProj + ".chc");

    //	GravaPIAZInicial();

    CFileException e;

    UINT Modo(Tipo == GRAVAR ? CFile::modeCreate | CFile::modeWrite : CFile::modeRead);

    if (ArqCurHoriz.Open(NomeArquivoX, Modo | CFile::shareDenyNone,&e) == 0)
    {
      if (e.m_cause != CFileException::fileNotFound) 
      {
        monolog.mensagem(1,NomeArquivoX.GetBuffer(1));
        return 1;
      }
    }

    Modo = (Tipo == GRAVAR ? CArchive::store : CArchive::load);

    CArchive arc(&ArqCurHoriz,Modo);

    if (Tipo == GRAVAR) this->operator<<(arc);
    else this->operator>>(arc);

    //--- Serializa o arquivo de curvas horizontais digitadas, acertando as alteraçoes feitas.
  }

  RemoveAll();     //--- Remove todas as curvas digitadas

  //--- Gera as curvas digitadas atraves dos dados do desenho, assim o arquivo de curvas digitais ficara correpondendo ao arquivo 
  //--- do calculo das curvas horizontais. Desta forma, as alterações feitas no desenho serão refletidas no arquivo digitado.

  for(register ItMCurHorizontaisTS itMCH = mapCurHorizontaisTS.begin() ; itMCH != mapCurHorizontaisTS.end() ; itMCH++)
  {
    CurHorizontal& CurvaAtual = itMCH->second;

    ArqCurHorizontais RegCurHhorizontal(CurvaAtual.EstTs,0,CurvaAtual.EstSt,0,CurvaAtual.Lado,CurvaAtual.Tipo,CurvaAtual.Ac,
                                        CurvaAtual.Raio,CurvaAtual.Lc,CurvaAtual.Se,CurvaAtual.Sl);
    AddTail(RegCurHhorizontal);   
  }

  GravarArquivo();

  return 0;
}

void CCurHorizontais::PegaDadosGeometricos(Ponto& PontoACalc, const CEstaca& Estaca)
{
  ItMCurHorizontaisTS itMCH(mapCurHorizontaisTS.upper_bound(Estaca));

  if (itMCH != mapCurHorizontaisTS.begin()) --itMCH;   // aconteceu nos testes

  //--- Upper_bound traz sempre a curva posterior (mesmo que seja depois da última ( traz end()), então pega a anterior.

  CurHorizontal CurvaAtual((itMCH)->second);

  //--- Na primeira curva o azimute esta no TS
  if (itMCH == mapCurHorizontaisTS.begin() && fabs(Estaca.EstVirtual - CurvaAtual.EstSt.EstVirtual) < 0.005)
  {
    PontoACalc = CurvaAtual.ST;
    return;
  }

  //--- Se for ponto notavél , já foi calculado, está nos dados da curva

  if (fabs(Estaca.EstVirtual - CurvaAtual.EstTs.EstVirtual) < 0.005) { PontoACalc = CurvaAtual.TS; return; }
  if (fabs(Estaca.EstVirtual - CurvaAtual.EstSc.EstVirtual) < 0.005) { PontoACalc = CurvaAtual.SC; return; }
  if (fabs(Estaca.EstVirtual - CurvaAtual.EstCs.EstVirtual) < 0.005) { PontoACalc = CurvaAtual.CS; return; }
  if (fabs(Estaca.EstVirtual - CurvaAtual.EstSt.EstVirtual) < 0.005) { PontoACalc = CurvaAtual.ST; return; }

  if (Estaca.EstVirtual > CurvaAtual.EstTs.EstVirtual)
  {
    if (Estaca.EstVirtual < CurvaAtual.EstSt.EstVirtual)
    {
      if (Estaca.EstVirtual < CurvaAtual.EstSc.EstVirtual)
        CalculaEspiral(PRIMEIRO_RAMO, PontoACalc, Estaca, CurvaAtual);
      else if (Estaca.EstVirtual < CurvaAtual.EstCs.EstVirtual) CalculaCircular(PontoACalc, Estaca, CurvaAtual);
      else CalculaEspiral(SEGUNDO_RAMO, PontoACalc, Estaca, CurvaAtual);
    }
    else
    {
//      if (++itMCH != mapCurHorizontaisTS.end()) CurvaAtual = itMCH->second;
      CalculaTangente(PontoACalc, Estaca, CurvaAtual,ANTERIOR);
    }
  }
  else CalculaTangente(PontoACalc, Estaca, CurvaAtual,POSTERIOR);
}

void inline CCurHorizontais::CalculaTangente(Ponto& PontoACalc, const CEstaca& Estaca, const CurHorizontal& CurvaAtual, int CurvaRef)
{
  if (CurvaRef == POSTERIOR)
  {
    double Distancia(CurvaAtual.EstTs.EstVirtual - Estaca.EstVirtual),
      AZ(CurvaAtual.TS.fi + Matem::M_PI);

    PontoACalc.x = sin(AZ) * Distancia + CurvaAtual.TS.x;
    PontoACalc.y = cos(AZ) * Distancia + CurvaAtual.TS.y;
    PontoACalc.fi = CurvaAtual.TS.fi;
  }
  else
  {
    double Distancia(Estaca.EstVirtual - CurvaAtual.EstSt.EstVirtual),
           AZ(CurvaAtual.ST.fi);

    PontoACalc.x = sin(AZ) * Distancia + CurvaAtual.ST.x;
    PontoACalc.y = cos(AZ) * Distancia + CurvaAtual.ST.y;
    PontoACalc.fi = CurvaAtual.ST.fi;
  }
}

bool CCurHorizontais::CalculaEspiral(int RAMO,Ponto& PontoACalc,const CEstaca& Estaca,const CurHorizontal& CurvaAtual)
{		
  double l;

  if (RAMO == PRIMEIRO_RAMO) l = Estaca.EstVirtual - CurvaAtual.EstTs.EstVirtual;
  else l = CurvaAtual.EstSt.EstVirtual - Estaca.EstVirtual;

  if(l <= 0.0) return false;

  CalcxyEsp(CurvaAtual,&PontoACalc.y,&PontoACalc.x,&PontoACalc.fi,l);

  PontoACalc.i = atan(PontoACalc.y / PontoACalc.x) * (CurvaAtual.Lado == ESQUERDO ? -1.0 : 1.0);
  double Azimute(RAMO == PRIMEIRO_RAMO ? CurvaAtual.TS.fi + PontoACalc.i : CurvaAtual.ST.fi + Matem::M_PI - PontoACalc.i);
  double Distancia(hypot(PontoACalc.x,PontoACalc.y)); 

  PontoACalc.x = sin(Azimute) * Distancia;
  PontoACalc.y = cos(Azimute) * Distancia;

  if (RAMO == PRIMEIRO_RAMO)
  {
    PontoACalc.x += CurvaAtual.TS.x;
    PontoACalc.y += CurvaAtual.TS.y;
    PontoACalc.fi = CurvaAtual.TS.fi + PontoACalc.fi;
  }
  else
  {
    PontoACalc.x += CurvaAtual.ST.x;
    PontoACalc.y += CurvaAtual.ST.y;
    PontoACalc.fi = CurvaAtual.ST.fi  - PontoACalc.fi; 
  }

  if (PontoACalc.fi < 0.0) PontoACalc.fi += Matem::M_2_PI;
  else if (PontoACalc.fi > Matem::M_2_PI) PontoACalc.fi -= Matem::M_2_PI;

  return true;
}

int CCurHorizontais::Compara(class CurHorizontal &No1,class CurHorizontal &No2)
{
  //--- Compara 2 nós da lista.

  if (No1.EstTs.EstVirtual < No2.EstTs.EstVirtual) return MENOR;
  else if (No1.EstTs.EstVirtual > No2.EstTs.EstVirtual) return MAIOR;
  else return IGUAL; 
}

void CCurHorizontais::InsOrdenada(class CurHorizontal &No)
{
  mapCurHorizontaisTS.insert(pairCHCE(No.EstTs,No));
  AtualizaLimites(No);
}

int CCurHorizontais::EstudaCaso(CurHorizontal &CurvaAtual,CurHorizontal &CurvaAnterior)
{
  //--- Se o inicio da curva atual esta antes do fim da curva 
  //--- Anterior , então é um caso especial. 

  if (1 || CurvaAtual.Ini > CurvaAnterior.Fim) return CurvaAtual.Caso = NORMAL;   //--- forçado todos os casos para normal ===> verificar posteriormente
  else
  {
    //--- Calcula a interseção das retas da taxa de superelevaçao das curvas,
    //--- coloca o resultado em EstInt.

    double b = (CurvaAtual.EstSc.EstVirtual - CurvaAnterior.EstCs.EstVirtual) * -CurvaAtual.m1 + (CurvaAtual.Se),
      EstInt = (((CurvaAnterior.Se) - b) / (CurvaAtual.m1 + CurvaAnterior.m2)) + CurvaAnterior.EstCs.EstVirtual;

    //--- Se a interseção esta entre o cs da anterior e o sc da atual...

    if (EstInt >= CurvaAnterior.EstCs.EstVirtual && EstInt <= CurvaAtual.EstSc.EstVirtual)
    {
      //--- então cai no caso 1, o fim da anterior e o início da atual vão para a interseçao.

      CurvaAnterior.Fim = CurvaAtual.Ini = EstInt;
      if (CurvaAtual.Lado == CurvaAnterior.Lado) return CurvaAnterior.Caso = CASO1;
      else return CurvaAtual.Caso = CASO1;
    }
    else
    {
      //--- Se não... (a interseçao está no trecho circular da curva)

      if (CurvaAtual.Lado == CurvaAnterior.Lado)
      {
        //--- Se as curvas são concorrentes, recalcula a taxa de super elevação da curva atual
        //--- ligando o cs da anterior e a sc da atual, caso 2.

        CurvaAtual.m1 = (CurvaAtual.Se - CurvaAnterior.Se)/ (CurvaAtual.EstSc.EstVirtual - CurvaAnterior.EstCs.EstVirtual);
        CurvaAnterior.Fim = CurvaAtual.Ini = CurvaAnterior.EstCs.EstVirtual;
        CurvaAnterior.m2 = -CurvaAtual.m1; 
        return CurvaAtual.Caso = CASO2;
      }
      else
      {
        //--- Se não ...( as curvas são opostas....)
        //--- Nesta caso recalcula o fim da curva atual e o início da curva atual, colocando o ponto
        //--- proporcionalmente à distancia entre o Cs da anterior e o Sc da atual e aos valores da 
        //--- superelevaçao.

        EstInt = (CurvaAtual.EstSc - CurvaAnterior.EstCs) * CurvaAtual.Se / (CurvaAtual.Se + CurvaAnterior.Se);
        CurvaAtual.m1 = (CurvaAtual.EstSc.EstVirtual - EstInt) / (CurvaAtual.Se);
        CurvaAnterior.m2 = (EstInt - CurvaAnterior.EstSc.EstVirtual) / (CurvaAtual.Se);
        CurvaAnterior.Fim  = CurvaAtual.Ini = EstInt;
        return CurvaAtual.Caso = CASO3;
      }
    }
  }
}

double CCurHorizontais::BuscaSuperElevacao(CEstaca& Estaca,int& Lado)
{
  if(mapCurHorizontaisTS.size() == 0) return INFINITO;

  PosicionaNoMaisProximo(Estaca);

  if (itAtual == mapCurHorizontaisTS.end()) return INFINITO;

  if(itAtual->second.Ini > Estaca.EstVirtual) itAtual--;

  if (itAtual == mapCurHorizontaisTS.end() || itAtual->second.Lc <= 0.0) return INFINITO;

  Lado = itAtual->second.Lado;   //--- Passa o lado da curva

  //--- Se a estaca estiver dentro da curva...

  if (Estaca > itAtual->second.Ini && Estaca < itAtual->second.Fim)
  {
    //--- Se a estaca estiver no trecho circular retorna a superelevação máxima.

    if (Estaca >= itAtual->second.EstSc && Estaca <= itAtual->second.EstCs) return itAtual->second.Se;

    //--- A estaca só pode estar nos trechos de transiçao, retorna  o valor conforme o caso
    //--- na transiçao anterior ou posterior. 

    if (Estaca < itAtual->second.EstSc) return itAtual->second.Se - (itAtual->second.EstSc - Estaca) * itAtual->second.m1 ;
    else return (itAtual->second.Se) - (Estaca - itAtual->second.EstCs) * itAtual->second.m2;
  }
  else return INFINITO;  //--- A estaca está fora da curva
}

int CCurHorizontais::PosicionaNoMaisProximo(const CEstaca& Estaca,int Direcao)
{
  if (!mapCurHorizontaisTS.size()) return MAIOR;

  CurHorizontal CurTemp;
  CurTemp.EstTs = Estaca;

  //--- Coloca o >= em atual e o < em rasc 

  ItMCurHorizontaisTS itAnterior =  itAtual = mapCurHorizontaisTS.upper_bound(CurTemp.EstTs); 

  if (itAtual == mapCurHorizontaisTS.end()) return MAIOR;
  else itAnterior--;

  //---Verifica qual é o mais proximo, coloca-o no atual e retorna se é = > ou <

  if (itAtual == mapCurHorizontaisTS.end()  ||
    (itAnterior != mapCurHorizontaisTS.end() && fabs(itAtual->second.EstTs.EstVirtual - Estaca.EstVirtual) > (fabs(itAnterior->second.EstTs.EstVirtual - Estaca.EstVirtual)))) itAtual = itAnterior;

  if (itAtual->second.EstTs > Estaca) return MAIOR;
  if (itAtual->second.EstTs < Estaca) return MENOR;

  return IGUAL;
}

void CCurHorizontais::PegaPontosNotaveis(CList<CEstaca,CEstaca&>& PontosNotaveis) 
{
  itAtual = mapCurHorizontaisTS.begin();

  while(itAtual != mapCurHorizontaisTS.end())
  {
    CurHorizontal CC = itAtual->second;

    if (itAtual->second.Raio > 0.0)
    {
      if (itAtual->second.Tipo == ESPIRAL && itAtual->second.Lc >= 5.00)
      {
        PontosNotaveis.AddTail(itAtual->second.EstTs);
        PontosNotaveis.AddTail(itAtual->second.EstSc);
        PontosNotaveis.AddTail(itAtual->second.EstCs);
        PontosNotaveis.AddTail(itAtual->second.EstSt);
      }
      else 
      {
        PontosNotaveis.AddTail(itAtual->second.EstPc);           //--- Iguais ao PC e PT, validas quando raio = 0...
        PontosNotaveis.AddTail(itAtual->second.EstPt);
      }
    }
    else 
    {
      PontosNotaveis.AddTail(itAtual->second.EstPc);
      PontosNotaveis.GetTail().Descricao = "PI";
    }

    itAtual++;
  }
}

void CCurHorizontais::PegaEstacasObrigatorias(CList<CEstaca,CEstaca&>& EstObrig,double Equidis,CProjeto* ProjAtual)
{
  /*
  CMainFrame* MainFrame = (CMainFrame*)AfxGetMainWnd();
  MainFrame->m_ProjetosBox.GetLBText(0,MainFrame->m_sNomeComp);
  CProjeto* ProjAtual = MainFrame->AchaProjetoNoVetor();
  */

  itAtual = mapCurHorizontaisTS.begin();
  CurHorizontal CurvaAtual;

  while(itAtual != mapCurHorizontaisTS.end())
  {
    if (itAtual->second.Raio != 0.0)
    {
      CEstaca Inicio,Fim;
      Inicio = itAtual->second.EstTs;
      Fim = itAtual->second.EstSt;

      for (double Est = Inicio.EstVirtual + Equidis; Est < Fim.EstVirtual ; Est += Equidis)
      {
        int NumIg(0); 
        double Int,EstReal = Est,  //ProjAtual->Igualdades.BuscaEstacaReal(Est,NumIg)+1e-5,
               EstInt = EstReal / Equidis;
        modf(EstInt,&Int);
        EstObrig.AddTail((CEstaca) (Int * Equidis));  
      }
    }
    itAtual++;
  }
}

void CCurHorizontais::CalculaCircular(Ponto& PontoACalc,const CEstaca& Estaca,const CurHorizontal& CurvaAtual)
{
  double l = Estaca.EstVirtual - CurvaAtual.EstSc.EstVirtual;
  PontoACalc.fi = l / CurvaAtual.Raio;

  if (CurvaAtual.Lado == ESQUERDO) PontoACalc.fi *= -1.0;

  double Azimute = CurvaAtual.SC.fi + PontoACalc.fi / 2.0,                //--- Azimote da corda
    Corda = sin(fabs(PontoACalc.fi) / 2.0) * CurvaAtual.Raio * 2.0;

  PontoACalc.x = sin(Azimute) * Corda + CurvaAtual.SC.x;
  PontoACalc.y = cos(Azimute) * Corda + CurvaAtual.SC.y;
  PontoACalc.fi = CurvaAtual.SC.fi + PontoACalc.fi;
}

int CCurHorizontais::CalcGeometrico(CString& Descricao,CProgressCtrl* Progresso,void * pDialogo,double ErroMaximo,CProjeto* CProj,bool SoSeccionar)
{
  SoSeccionar = false;

  CString NomeTrecho(NomeProjeto.Right(NomeProjeto.GetLength() - NomeProjeto.ReverseFind('\\') - 1));

  Descricao = NomeTrecho;
  Descricao += !SoSeccionar ? ": O projeto foi alterado, recalculando o projeto geométrico..." : ": preparando os cálculos...";  

  if(mapCurHorizontaisTS.size() < 2) return ERRO_NAO_HA_CH;

  if(pDialogo) ((CDialog*) pDialogo)->UpdateData(false);

  if(!SoSeccionar)
  {
    CurHorizontal CurvaAnterior(mapCurHorizontaisTS.begin()->second),
    CurvaAtual((++(mapCurHorizontaisTS.begin()))->second);
    double AZInicial(CalculaAzimute(CurvaAtual.PI,CurvaAnterior.PI));

    ItMCurHorizontaisTS itCHAnterior(mapCurHorizontaisTS.begin()),itCHAtual(itCHAnterior),itCHPosterior(++itCHAtual);
    itCHPosterior++;
  
    //--- Calcula oas coordenadas dos pontos das curvas. para tal é preciso usar a curva anterior e a 
    //--- Posterior pra cálculo dos ACs

    CurvaAnterior.PI.fi = AZInicial;
    itCHAnterior->second.PI = itCHAnterior->second.TS = itCHAnterior->second.SC = itCHAnterior->second.CS = itCHAnterior->second.ST = CurvaAnterior.PI;

    Descricao = NomeTrecho + ": Calculando as curvas...";
    if(pDialogo) ((CDialog*) pDialogo)->UpdateData(false);
    if(Progresso) Progresso->SetPos(0);

    while(itCHAtual != mapCurHorizontaisTS.end())
    { 
      CalcxyEsp(itCHAtual->second,&itCHAtual->second.xEsp,&itCHAtual->second.yEsp,&itCHAtual->second.Fi,itCHAtual->second.Lc);
      CalculaDadosComplEspiral(itCHAtual->second);

      int Resp(CalculaDadosCurva(itCHAtual->second,itCHAnterior->second,itCHPosterior == mapCurHorizontaisTS.end() ? NULL : &itCHPosterior->second,false,ErroMaximo));

      if (Resp != CCurHorizontais::CURVA_OK) return Resp;

      itCHAnterior = itCHAtual;
      itCHAtual = itCHPosterior;
    
      if (itCHPosterior != mapCurHorizontaisTS.end()) itCHPosterior++;

      if(Progresso) Progresso->StepIt();
    }

    //---  Serializa o calculo geometrico das curvas

    Descricao =  NomeTrecho + ": Gravando cálculo das curvas horizontais...";

    if(pDialogo) ((CDialog*) pDialogo)->UpdateData(false);

    Serialize(NomeProjeto,GRAVAR);
  }

  CCurVerticais CurvasVerticais(NomeProjeto);

  if(CurvasVerticais.VerfErro() != CALCULO_OK)  return CALCULO_NOK;

  CString NomeProjBase(NomeProjeto.Left(NomeProjeto.ReverseFind('\\')));
  NomeProjBase = NomeProjBase.Left(NomeProjBase.ReverseFind('\\'));

  CEstacasACalcular EstacasACalcular(this,&CurvasVerticais, NomeProjBase,CProj);

  //--- SERIALIZAR A CLASSE CESTACAS A CALULAR,. CODIGO ABAIXO...

  CFile ArqCalcGeo;
  CFileException e;

  if (ArqCalcGeo.Open(NomeArquivo.Left(NomeArquivo.ReverseFind('.')) + CString(".geo"),CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite,&e) == 0)
  {
    monolog.mensagem(1,NomeArquivo.GetBuffer(1));
    return ERRO_CRIAR_ARQUIVO_GEO;
  }

  CArchive CArqCalcGeo(&ArqCalcGeo,CArchive::store);

  POSITION EstAtual(EstacasACalcular.GetHeadPosition());
  Ponto PontoACalc;
  CEstaca EstacaAtual;

  Descricao = NomeTrecho + ": Calculando as coordenadas das estacas...";
  if (Progresso)
  {
    Progresso->SetRange(0, EstacasACalcular.GetCount());
    Progresso->SetPos(0);
  }
  if(pDialogo) ((CDialog*) pDialogo)->UpdateData(false);
  double Seno(sin(AngRotacao)),Coseno(cos(AngRotacao));

  CArqCalcGeo << (int) EstacasACalcular.GetCount();

  while(EstAtual)  //--- Calcula as coordenadas de todas as estacas.
  {
    EstacaAtual = EstacasACalcular.GetNext(EstAtual);  //--- Pega a próxima estaca a calcular.
    PegaDadosGeometricos(PontoACalc,EstacaAtual);
 //   if(EstaRotacionada()) PontoACalc.RotacionaPlano(Seno,Coseno,AngRotacao); 

    EstacaAtual >> CArqCalcGeo;
    PontoACalc >> CArqCalcGeo; 

    if(Progresso) Progresso->StepIt();
  }

  CArqCalcGeo.Close();
  ArqCalcGeo.Close(); 

 // ((CMainFrame*) AfxGetMainWnd())->GeometricoAlterado = false;

  Descricao = NomeTrecho + (!SoSeccionar ? ": Projeto geométrico recalculado..." : ": ");
  if(pDialogo) ((CDialog*) pDialogo)->UpdateData(false);

  return CALCULO_OK;
}

void CCurHorizontais::CalcxyEsp(const CurHorizontal& CurvaAtual,double *x,double *y,double *s,double ls)
{
  if (CurvaAtual.Lc > 0.0 && CurvaAtual.Tipo == ESPIRAL)   
  {
    
    *s = ls / 2.0 / (CurvaAtual.K / ls) ;                                        //--- CurvaAtual.K / ls = Raio
    *x = ls * *s / 3.0 * (1.0 - pow(*s,2.0) / 14.0 + pow(*s,4.0) / 440.0);
    *y = ls * (1.0 - pow(*s,2.0) / 10.0 + pow(*s,4.0) / 216.0);

    if (CurvaAtual.Ac < 0.0) *s *= -1.0;
  }
  else  *s = *x = *y = 0.0;
}

int CCurHorizontais::CalculaDadosCurva(CurHorizontal& CurvaAtual,CurHorizontal& CurvaAnterior,CurHorizontal* CurvaPosterior,bool ComAc,double ErroMaximo)
{
  double AZPosterior(0.0);

  //--- Calcula os dados da espiral e o AC da circular
  //--- Se veio o ponto posterior caclula o ac a partir dos azimutes posterior e anterior,
  //--- Caso contrário usa o ac que veio na curva
  //--- Esta rotina só setá chamada quando existirem as 3 curvas anterior, Atual e posterior, então não é preciso fazer a 
  //--- verificação se a curva existe ou não.

  //--- Se o calculo é para ser feito a partir do arquivo de curva horizontais pega o ac digitado, caso contrários
  //--- Se for uma alteração de PI calcula o ac a partir dos azimutes anterior e posterior;

 if (!ComAc && CurvaPosterior)
  {
    AZPosterior = CalculaAzimute(CurvaPosterior->PI,CurvaAtual.PI);
    CurvaAtual.Ac = AZPosterior - CurvaAtual.TS.fi;

    //--- O Dominio do AC é de -PI a PI

    if (CurvaAtual.Ac > Matem::M_PI) CurvaAtual.Ac -= Matem::M_2_PI;
    else if (CurvaAtual.Ac < -Matem::M_PI) CurvaAtual.Ac += Matem::M_2_PI;  

    if (CurvaAtual.Ac >  -1.4544e-4 && CurvaAtual.Ac < 1.4544e-4) return AC_MENOR_PERMITIDO;

    CurvaAtual.Lado = CurvaAtual.Ac < 0.0 ? ESQUERDO : DIREITO;
  }
  else if (!CurvaPosterior) CurvaAtual.Ac = 0.0;        //--- Se for a última curva o ac = 0;

  //--- O Dominio do AC é [-PI,PI]

  if (CurvaAtual.Ac > Matem::M_PI) CurvaAtual.Ac -= Matem::M_2_PI;
  else if (CurvaAtual.Ac < -Matem::M_PI) CurvaAtual.Ac += Matem::M_2_PI;  

  double Erro(fabs(CurvaAtual.EstCs - CurvaAtual.EstSc - fabs((CurvaAtual.AcCirc - (2.0 * CurvaAtual.Fi)) * CurvaAtual.Raio)));

  /*
  if(Erro > ErroMaximo)
  {
    // --- Erro na curva, "desenvolvimento da circular nao confere com  o ac" ou Sc/Cs reentrantes.
    if(CurvaAtual.Tipo == CIRCULAR) monolog.mensagem(18,CString("Estaca PC = ") + CurvaAtual.EstPc);
    else monolog.mensagem(19,CString("Estaca TS = ") + CurvaAtual.EstTs);

    return DESENV_NAOCONFERE;
  }
*/
  //--- Verifica as distancias com as curvas posterior e enterior

  double Distancia(0.0);  

  if (!ComAc) Distancia = hypot(CurvaAtual.PI.x - CurvaAnterior.PI.x,CurvaAtual.PI.y - CurvaAnterior.PI.y) - CurvaAnterior.Ts - CurvaAtual.Ts;
  else Distancia = CurvaAtual.EstTs - CurvaAnterior.EstSt;  

  if(Distancia > 0.001)
  {
 //   if (!ComAc) Distancia = hypot(CurvaAtual.PI.x - CurvaPosterior->PI.x,CurvaAtual.PI.y - CurvaPosterior->PI.y) - CurvaPosterior->Ts - CurvaAtual.Ts;
 //   else Distancia = CurvaPosterior->EstTs - CurvaAtual.EstSt;  
  }

  //--- Se a curva é circular e tem Lc o Ts é fictício, tem q corrigir

  //  if(CurvaAtual.Tipo == CIRCULAR && CurvaAtual.Lc > 0.0) Distancia += CurvaAtual.Lc;
  //  if(CurvaAnterior.Tipo == CIRCULAR && CurvaAnterior.Lc > 0.0) Distancia += CurvaAnterior.Lc;

  //--- Se a distancia é menor que a soma dos Ts a alteração não pode ser feita

  if (Distancia < 0.0005) 
  {
  //  Calcula o AC que falta para zerar a circular
  //  Calcula novo PI aumentando o AZ2 e calculando a interseção.

    return CURVAS_TRANSPASSANDO;
  }

  CurvaAtual.TS.x = CurvaAnterior.ST.x + sin(CurvaAnterior.ST.fi) * Distancia;
  CurvaAtual.TS.y = CurvaAnterior.ST.y + cos(CurvaAnterior.ST.fi) * Distancia;
  CurvaAtual.TS.fi = CurvaAnterior.ST.fi;

  //-- Calcula as coordenadas do SC. 

  double Corda[2],AngEsp,AzTemp;

  Corda[ESPIRAL] = hypot(CurvaAtual.xEsp,CurvaAtual.yEsp);
  if(CurvaAtual.yEsp != 0.0) AngEsp = atan(CurvaAtual.xEsp / CurvaAtual.yEsp);
  else AngEsp = 0.0;
  if (CurvaAtual.Lado ==  ESQUERDO) AngEsp *= -1.0;
  AzTemp = AngEsp + CurvaAtual.TS.fi;
  CurvaAtual.SC.x = CurvaAtual.TS.x + (sin(AzTemp) * Corda[ESPIRAL]);
  CurvaAtual.SC.y = CurvaAtual.TS.y + (cos(AzTemp) * Corda[ESPIRAL]);
  CurvaAtual.SC.fi = CurvaAtual.TS.fi + CurvaAtual.Fi;
  AcertaAngulo(&CurvaAtual.SC.fi);

  //-- Calcula as coordenadas do CS. 

  Corda[CIRCULAR] = sin(fabs(CurvaAtual.AcCirc) / 2.0) * CurvaAtual.Raio * 2.0;
  AzTemp = CurvaAtual.SC.fi + CurvaAtual.AcCirc / 2.0;
  CurvaAtual.CS.x = CurvaAtual.SC.x + (sin(AzTemp) * Corda[CIRCULAR]);
  CurvaAtual.CS.y = CurvaAtual.SC.y + (cos(AzTemp) * Corda[CIRCULAR]);
  CurvaAtual.CS.fi = CurvaAtual.SC.fi + CurvaAtual.AcCirc;
  AcertaAngulo(&CurvaAtual.CS.fi);

  //--- Calcula as coordenadas do ST

  AzTemp = CurvaAtual.CS.fi + CurvaAtual.Fi - AngEsp;
  CurvaAtual.ST.x = CurvaAtual.CS.x + sin(AzTemp) * Corda[ESPIRAL];
  CurvaAtual.ST.y = CurvaAtual.CS.y + cos(AzTemp) * Corda[ESPIRAL];
  CurvaAtual.ST.fi = CurvaAtual.CS.fi + CurvaAtual.Fi;

  //--- As coordenadas do PI não são calculadas nunca, sempre vem fixas
  //--- Calcula as coordenadas do PI

 // CurvaAtual.PI.x = CurvaAtual.TS.x + CurvaAtual.Ts * sin(CurvaAtual.TS.fi); 
 // CurvaAtual.PI.y = CurvaAtual.TS.y + CurvaAtual.Ts * cos(CurvaAtual.TS.fi);

  //--- Calcula as coordenadas do centro da circular

  double AZCentro = CurvaAtual.Lado == DIREITO ? CurvaAtual.SC.fi + Matem::M_PI_2 :CurvaAtual.SC.fi - Matem::M_PI_2;

  CurvaAtual.CC.x = CurvaAtual.SC.x + CurvaAtual.Raio * sin(AZCentro); 
  CurvaAtual.CC.y = CurvaAtual.SC.y + CurvaAtual.Raio * cos(AZCentro); 

  //--- Acerta o azimute do ST

  AcertaAngulo(&CurvaAtual.ST.fi);

  return CURVA_OK;
}

void CCurHorizontais::ConstroiEspiralDaCircular(CurHorizontal& CurvaAtual)
{
  //--- Tansforma a estaca circular em espiral conformes normas do DER - MG

  if (CurvaAtual.Tipo ==  ESPIRAL && CurvaAtual.Lc > 0.0)
  {
    CurvaAtual.EstSc.Constroi(CurvaAtual.EstTs.EstVirtual + CurvaAtual.Lc,INFINITO,0,CString("SC")); 
    CurvaAtual.EstCs.Constroi(CurvaAtual.EstSt.EstVirtual - CurvaAtual.Lc,INFINITO,0,CString("CS")); 
  }
  else
  {
    CurvaAtual.EstPc = CurvaAtual.EstSc = CurvaAtual.EstTs;
    CurvaAtual.EstPc.Descricao = "PC";
    CurvaAtual.EstPt = CurvaAtual.EstCs = CurvaAtual.EstSt;
    CurvaAtual.EstPt.Descricao = "PT";

    //--- Transforma a curva circular em espiral
    //--- *** Abandonado aé 2a ordem ***
    /*
    CurvaAtual.EstSc.Constroi(CurvaAtual.EstTs.EstVirtual + CurvaAtual.Lc * 0.4,0);
    CurvaAtual.EstCs.Constroi(CurvaAtual.EstSt.EstVirtual - CurvaAtual.Lc * 0.4,0);
    CurvaAtual.EstSt.Constroi(CurvaAtual.EstSt.EstVirtual + CurvaAtual.Lc * 0.6,0);
    CurvaAtual.EstTs.Constroi(CurvaAtual.EstTs.EstVirtual - CurvaAtual.Lc * 0.6,0);
    */
  }
}

int CCurHorizontais::AlteraCurvaPI(ItCMCurHorizontaisPI& ItCurSendoAlteradaPI,Ponto& NovoPI,ItMCurHorizontaisTS& ItUltimaCurvaAjustadaTS,CEstaca* pEstTsCurvaAnterior)
{   
  CEstaca Estaca(ItCurSendoAlteradaPI->second->EstTs);

 // itAtual = mapCurHorizontaisTS.find(ItCurSendoAlteradaPI->second->EstTs);   //--- Quando o mouse move-se muito rápido não acha a curva....
  itAtual = mapCurHorizontaisTS.find(Estaca);
  if(itAtual == mapCurHorizontaisTS.end()) 
  {
    ASSERT(0);
    return false;
  }

  ItMCurHorizontaisTS PCurvas[QTD_TOTAL_CURVAS]= {mapCurHorizontaisTS.end(),mapCurHorizontaisTS.end(),itAtual,mapCurHorizontaisTS.end(),mapCurHorizontaisTS.end()},Rasc;
  CurHorizontal curvas[QTD_TOTAL_CURVAS],CurvaSalva,UltimaCurvaAjustada;
  int PrimeiraCurvaAlterada(-1);    

  Rasc = itAtual;

  curvas[ATUAL] = itAtual->second;
  curvas[ATUAL].PI = NovoPI;        //--- Altera o PI para o ponto onde o mouse está

  //--- 1- Acerta o mapa TS

  //--- Recalcula os ACs das curvas
  //--- A anterior da anterior a a proxima da proxima não são alteradas. 
  //--- A Anterior da anterior é chamada para calcular a estaca do ts da 
  //--- curva a ser calculada e a posterior da posterior é usada apenas para 
  //--- se calcular o AC da posterior

  //--- Com estas alterações os sets ficam corrompidos pois o TS e o PI Mudaram. Deste modo
  //--- o programa faz as alterações sem requisitar nenhuma busca e depois deleta os elementos e 
  //--- os insere novamente, nesta nova inserção ficarão na ordem correta.

  if(Rasc != mapCurHorizontaisTS.end() && Rasc != mapCurHorizontaisTS.begin())
  {
    PCurvas[ANTERIOR] = --Rasc;                          //--- curvaAnterior
    if (PCurvas[ANTERIOR] != mapCurHorizontaisTS.end()) 
    {
      PrimeiraCurvaAlterada = ANTERIOR;
      curvas[ANTERIOR] = PCurvas[ANTERIOR]->second;
    }
  }

  if(Rasc != mapCurHorizontaisTS.end() && Rasc != mapCurHorizontaisTS.begin())
  {
    PCurvas[ANTERIOR_A_ANTERIOR] = --Rasc;                        //--- Anterior da Anterior
    if (PCurvas[ANTERIOR_A_ANTERIOR] != mapCurHorizontaisTS.end()) curvas[ANTERIOR_A_ANTERIOR] = PCurvas[0]->second; 
  }

  Rasc= itAtual;
  PCurvas[POSTERIOR] = ++Rasc;                          //--- Proxima curva
  if (PCurvas[POSTERIOR] != mapCurHorizontaisTS.end()) 
  {
    curvas[POSTERIOR] = PCurvas[POSTERIOR]->second; 
    CurvaSalva = curvas[POSTERIOR];
  }

  if (Rasc != mapCurHorizontaisTS.end())
  {
    PCurvas[POSTERIOR_A_POSTERIOR] = ++Rasc;                        //--- Proxima da proxima.
    if (PCurvas[POSTERIOR_A_POSTERIOR] != mapCurHorizontaisTS.end()) curvas[POSTERIOR_A_POSTERIOR] = PCurvas[POSTERIOR_A_POSTERIOR]->second; 
  }

  double UltimoSt(0.0);
  int i(0);
  CEstaca EstCurvaAtualNovoTS;
  bool MovendoPrimeiroPI(false),MovendoUltimoPI(false);

  for (i = ANTERIOR ; i < POSTERIOR_A_POSTERIOR ; i++)
  {
    if (PCurvas[i] == mapCurHorizontaisTS.end()) continue;    //--- Curva não existe, faz as outras
  
    //--- Salva a última curva que sofreu alteração, sera usada no restaqueamento. 
    //--- O resto eixo será restaqueado a partir ddo ST desta curva

    ItUltimaCurvaAjustadaTS = PCurvas[i]; 
    UltimaCurvaAjustada = PCurvas[i]->second; 

    CurHorizontal& CurvaPosterior = curvas[i+1],& CurvaAtual = curvas[i],& CurvaAnterior = curvas[i-1];

    CurvaAtual.ST.fi = CurvaPosterior.TS.fi = CalculaAzimute(CurvaPosterior.PI,CurvaAtual.PI);

    if(mapCurHorizontaisTS.begin() != PCurvas[i] && --mapCurHorizontaisTS.end() != PCurvas[i])   //--- Se não for a primeira ou última curva recalcula os dados da curva
    {                                           
      //--- Acerta o AC da curva

      CurvaAtual.Ac = CurvaAtual.ST.fi - CurvaAtual.TS.fi;

      //-- Se menor que 30 segundos a alteração não pode ser feita pois  o raio teria q ser zerado

   //   if (fabs(CurvaAtual.Ac) < 1.4544e-4)  return AC_MENOR_PERMITIDO;   Está corrompendo o set, verificar

      double AcSemCircular(fabs(CurvaAtual.Fi) * 2.0);

      if (CurvaAtual.Tipo == ESPIRAL && fabs(CurvaAtual.Ac) < ((fabs(CurvaAtual.Fi) * 2.0) + 1.4544e-7))  return LACOS_TRANSPASSANDO;
      
	  //--- O Dominio do AC é de -PI a PI

      if (CurvaAtual.Ac > Matem::M_PI) CurvaAtual.Ac -= Matem::M_2_PI;
      else if (CurvaAtual.Ac < -Matem::M_PI) CurvaAtual.Ac += Matem::M_2_PI;  

      CurvaAtual.Lado = CurvaAtual.Ac < 0.0 ? ESQUERDO : DIREITO;

      //--- Recalcula o estaqueamento da curva

      CalcxyEsp(CurvaAtual,&CurvaAtual.xEsp,&CurvaAtual.yEsp,&CurvaAtual.Fi,CurvaAtual.Lc);
      CalculaDadosComplEspiral(CurvaAtual);

      if (fabs(CurvaAtual.Ac) < fabs(CurvaAtual.Fi * 2.0)) 
        return CCurHorizontais::LACOS_TRANSPASSANDO;     //--- Ramos da espiral reentrantes

      double DistanciaAnterior(hypot(CurvaAtual.PI.x - (curvas[i-1]).PI.x, CurvaAtual.PI.y - (curvas[i-1]).PI.y) - (curvas[i-1]).Ts - CurvaAtual.Ts);

      CurvaAtual.EstTs = CEstaca(CEstaca((curvas[i-1]).EstSt.EstVirtual + DistanciaAnterior),"TS");
      CurvaAtual.AcCirc = (fabs(CurvaAtual.Ac) - fabs(CurvaAtual.Fi * 2.0)) * (CurvaAtual.Lado == ESQUERDO ? -1.0 : 1.0);
      CurvaAtual.EstSc = CEstaca(CEstaca(CurvaAtual.EstTs.EstVirtual + CurvaAtual.Lc),"SC");
      CurvaAtual.EstCs = CEstaca(CEstaca(fabs(CurvaAtual.AcCirc) * CurvaAtual.Raio +  CurvaAtual.EstSc.EstVirtual),"CS");
      CurvaAtual.EstSt = CEstaca(CEstaca(CurvaAtual.EstCs.EstVirtual + CurvaAtual.Lc),"ST");

      if(i == ATUAL) EstCurvaAtualNovoTS = CurvaAtual.EstTs; 

      UltimoSt = CurvaAtual.EstSt.EstReal;      //--- Salva o valor do ultimo St, será usado no restaqueamento do restante do projeto.

      if (CurvaAtual.Tipo == CIRCULAR)
      {
        CurvaAtual.EstPc = CurvaAtual.EstTs;
        CurvaAtual.EstPc.Descricao= "PC ";
        CurvaAtual.EstPt = CurvaAtual.EstSt;
        CurvaAtual.EstPt.Descricao = "PT ";
      }

      //--- Recalcula as coordenadas da Curva Se tiver uma curva anterior

      if (PCurvas[i-1] != mapCurHorizontaisTS.end()) 
      {
        //--- A Alteração nao pode ser feita, uma curva esta entrando na outra.

        if (!CalculaDadosCurva(curvas[i],curvas[i-1],&curvas[i+1])) return CCurHorizontais::CURVAS_TRANSPASSANDO;                                              

        //--- Pode ocorre também dependendo da velocidade dos saltos do mouse que a 
        //--- curva atual esteja antes da curva anterior

        if (CurvaAtual.EstTs.EstVirtual < CurvaAnterior.EstSt. EstVirtual) return CCurHorizontais::CURVAS_TRANSPASSANDO;
      }
    } 
    else  
    {
      if(i == ATUAL)
      {
        if(mapCurHorizontaisTS.begin() == PCurvas[i]) MovendoPrimeiroPI = true;
        else MovendoUltimoPI = true;
      }

      CurvaAtual.CC.x = CurvaAtual.TS.x = CurvaAtual.ST.x = CurvaAtual.PI.x;     //--- Ateração apenas no Pi Inicial ou final
      CurvaAtual.CC.y = CurvaAtual.TS.y = CurvaAtual.ST.y = CurvaAtual.PI.y; 

      CurvaAtual.Ac = CurvaAtual.Ts = 0.0;

      if(PCurvas[i] == mapCurHorizontaisTS.begin())    //--- Primeira curva?
        CurvaAtual.EstTs = CurvaAtual.EstSc = CurvaAtual.EstCs = CurvaAtual.EstSt = CurvaAtual.EstPc = CurvaAtual.EstPt = mapCurHorizontaisTS.begin()->second.EstTs;

      if(--mapCurHorizontaisTS.end() == PCurvas[i] && mapCurHorizontaisTS.size() > 1)   //--- Se for a último PI e tiver mais que 1 PIV 
      {
        CurvaAtual.EstTs = CurvaAtual.EstSc = CurvaAtual.EstCs =CurvaAtual.EstSt = CurvaAtual.EstPc = CurvaAtual.EstPt =  CurvaAnterior.EstSt + hypot(CurvaAtual.PI.x - (curvas[i-1]).PI.x, CurvaAtual.PI.y - (curvas[i-1]).PI.y) - (curvas[i-1]).Ts;					

        if(PCurvas[i-1]->second.EstSt > CurvaAtual.EstTs) 
        {
         // ASSERT(0);
          return false; //--- Transpassou o último PI?? retona false
        }

        ItUltimaCurvaAjustadaTS = PCurvas[i]; 
        UltimoSt = CurvaAtual.EstTs.EstVirtual;
      } 
    }
  }

  ReEstaqueia(ItUltimaCurvaAjustadaTS,UltimoSt,ItCurSendoAlteradaPI);   //--- Reestaqueia as curvas posteriores

  //---Muda no set TS as 3 (ou 2) curvas que sofreram alteração

  std::list<ItMCurHorizontaisTS> lstItMCurHorTS;
  std::list<ItMCurHorizontaisTS>::iterator itlstItMCurHorTS;

  int Comecou(ANTERIOR_A_ANTERIOR);     // Alteração no meio
  
  ItMCurHorizontaisTS itAtualBack = itAtual = mapCurHorizontaisTS.find(curvas[ANTERIOR_A_ANTERIOR].EstTs);   

  if(itAtual == mapCurHorizontaisTS.end()) 
  {
    itAtual = itAtualBack = mapCurHorizontaisTS.find(curvas[ANTERIOR].EstTs);  
    Comecou = ANTERIOR;               //--- Alteração no 2o PI
  }

  if(itAtual == mapCurHorizontaisTS.end()) 
  {
    itAtual = itAtualBack = mapCurHorizontaisTS.find(curvas[ATUAL].EstTs);  
    Comecou = ATUAL;                  //--- Alteração no 1o PI
  }

  if(Comecou == ANTERIOR_A_ANTERIOR) ++itAtual;

  //--- Salva os iteradores para as curvas que serão removidas

  for (i = Comecou == ATUAL ? ATUAL : ANTERIOR ; i < POSTERIOR_A_POSTERIOR && itAtual != mapCurHorizontaisTS.end() ; i++) 
  {
    lstItMCurHorTS.push_back(itAtual++);
  }
  
  //--- Remove as curvas antigas

  for (itlstItMCurHorTS = lstItMCurHorTS.begin() ; itlstItMCurHorTS != lstItMCurHorTS.end() ; itlstItMCurHorTS++) 
  {
    mapCurHorizontaisTS.erase(*itlstItMCurHorTS);
  }

  //--- Insere as novas curvas

  itlstItMCurHorTS = lstItMCurHorTS.begin();   //--- Apenas para achar o último a inserir

  for(i = Comecou == ATUAL ? ATUAL : ANTERIOR ; i < POSTERIOR_A_POSTERIOR &&  itlstItMCurHorTS != lstItMCurHorTS.end() ; i++,itlstItMCurHorTS++) 
  {
    curvas[i].mapSecoesDesenho.clear();
    mapCurHorizontaisTS.insert(pairCHCE(curvas[i].EstTs,curvas[i]));
  }

  //--- 2- Acerta os outros sets
  //--- Os sets de PI e CC São de ponteiros, então, pra facilitar, remove tudo e insere de novo

  mapCurHorizontaisPI.clear();
  mapCurHorizontaisCC.clear();

  for (ItMCurHorizontaisTS itMapTSAtual = mapCurHorizontaisTS.begin() ; itMapTSAtual != mapCurHorizontaisTS.end() ; itMapTSAtual++)
  {
    mapCurHorizontaisPI.insert(pairpCHPO(&itMapTSAtual->second.PI,&itMapTSAtual->second));
    mapCurHorizontaisCC.insert(pairpCHPO(&itMapTSAtual->second.CC,&itMapTSAtual->second));
  }

  if(MovendoPrimeiroPI) EstCurvaAtualNovoTS = mapCurHorizontaisTS.begin()->second.EstTs;
  if(MovendoUltimoPI) EstCurvaAtualNovoTS = mapCurHorizontaisTS.rbegin()->second.EstTs;

  ItMCurHorizontaisTS itTemp(mapCurHorizontaisTS.find(EstCurvaAtualNovoTS));
  ItCurSendoAlteradaPI = mapCurHorizontaisPI.find(&itTemp->second.PI);                //--- Passa a curva sendo alterada para o desenho

//  if(pEstTsCurvaAnterior) *pEstTsCurvaAnterior = PCurvas[ANTERIOR] == mapCurHorizontaisTS.end() ? INFINITO : curvas[ANTERIOR].EstTs;
 
  if(pEstTsCurvaAnterior)
  {
    if(curvas[ANTERIOR].EstTs.EstReal != INFINITO) 
    {
      *pEstTsCurvaAnterior = curvas[ANTERIOR].EstTs;     
    }
    else
    {
      *pEstTsCurvaAnterior = INFINITO;
    }

  }

  return CCurHorizontais::CURVA_OK;
}

int CCurHorizontais::AlteraCurvaCC(ItCMCurHorizontaisCC& ItCurSendoAlteradaCC,Ponto& NovoCC,ItMCurHorizontaisTS& ItUltimaCurvaAjustadaTS,double NovoLc,double NovoRaio)
{
  itAtual = mapCurHorizontaisTS.find(ItCurSendoAlteradaCC->second->EstTs);

  ItMCurHorizontaisTS PCurvas[QTD_TOTAL_CURVAS]= {mapCurHorizontaisTS.end(),mapCurHorizontaisTS.end(),itAtual,mapCurHorizontaisTS.end(),mapCurHorizontaisTS.end()},Rasc(itAtual);
  CurHorizontal curvas[QTD_TOTAL_CURVAS];

  curvas[ATUAL] = itAtual->second;
  if(NovoRaio != INFINITO) curvas[ATUAL].Raio = NovoRaio;
  else curvas[ATUAL].Raio = hypot(itAtual->second.SC.x - NovoCC.x,itAtual->second.SC.y - NovoCC.y);       //--- Muda o raio, o novo valor é igual distância do mouse ao TS

  if(NovoLc != INFINITO)
  {
    curvas[ATUAL].Lc = NovoLc;
    curvas[ATUAL].Tipo = ESPIRAL;
  }

  //--- A anterior da anterior a a proxima da proxima não são alteradas. 
  //--- A Anterior da anterior é chamada para calcular a estaca do ts da 
  //--- curva a ser calculada e a posterior da posterior é usada apenas para 
  //--- se calcular o AC da posterior. Neste caso o AC da posterior não muda mas a rotina 
  //--- que calcula as coordenadas (CalculaDadosCurva) exige este parametro

  //--- Com estas alterações os sets ficam corrompidos pois o TS e o PI Mudaram. Deste modo
  //--- o programa faz as alterações sem requisitar nenhuma busca e depois deleta os elementos e 
  //--- os insere novamente, nesta nova inserção ficarão na ordem correta.

  PCurvas[ANTERIOR] = --Rasc;                          //--- curvaAnterior
  curvas[ANTERIOR] = PCurvas[ANTERIOR]->second;

  if (Rasc != mapCurHorizontaisTS.begin()) 
  {
    PCurvas[ANTERIOR_A_ANTERIOR] = --Rasc;                        //--- Anterior da Anterior
    if (PCurvas[ANTERIOR_A_ANTERIOR] != mapCurHorizontaisTS.end()) curvas[ANTERIOR_A_ANTERIOR] = PCurvas[ANTERIOR_A_ANTERIOR]->second; 
  }

  Rasc= itAtual;
  PCurvas[POSTERIOR] = ++Rasc;                       
  curvas[POSTERIOR] = PCurvas[POSTERIOR]->second; 

  if (Rasc != mapCurHorizontaisTS.end())
  {
    PCurvas[POSTERIOR_A_POSTERIOR] = ++Rasc;                       
    if (PCurvas[POSTERIOR_A_POSTERIOR] != mapCurHorizontaisTS.end()) curvas[POSTERIOR_A_POSTERIOR] = PCurvas[POSTERIOR_A_POSTERIOR]->second; 
  }

  for (int i = ANTERIOR ; i < POSTERIOR_A_POSTERIOR ; i++)
  {
    //--- Salva a última curva que sofreu alteração, sera usada no restaqueamento. 
    //--- O resto eixo será restaqueado a partir do ST desta curva

    ItUltimaCurvaAjustadaTS = PCurvas[i]; 

    if (PCurvas[i] != mapCurHorizontaisTS.end() /*&& PCurvas[i+1] != mapCurHorizontaisTS.end()*/)
    {
      CurHorizontal& CurvaPosterior = curvas[i+1],& CurvaAtual = curvas[i],& CurvaAnterior = curvas[i-1];

      if (CurvaAtual.Tipo == ESPIRAL) 
      {
        CurvaAtual.AcCirc =  fabs(CurvaAtual.Ac) - (CurvaAtual.Lc / CurvaAtual.Raio);
        if(CurvaAtual.AcCirc < 0.0) return LACOS_TRANSPASSANDO;
        if(CurvaAtual.Ac < 0.0) CurvaAtual.AcCirc *= -1.0;
      }

      CalcxyEsp(CurvaAtual,&CurvaAtual.xEsp,&CurvaAtual.yEsp,&CurvaAtual.Fi,CurvaAtual.Lc);
      CalculaDadosComplEspiral(CurvaAtual);

      if(PCurvas[i-1] != mapCurHorizontaisTS.end())
      {
        double DistanciaAnterior = 	hypot(CurvaAtual.PI.x - PCurvas[i-1]->second.PI.x, CurvaAtual.PI.y - PCurvas[i-1]->second.PI.y) - (PCurvas[i-1])->second.Ts - CurvaAtual.Ts;
        CurvaAtual.EstTs = CEstaca(CEstaca((PCurvas[i-1])->second.EstSt.EstVirtual + DistanciaAnterior),"TS");
      }

      CurvaAtual.EstSc = CEstaca(CEstaca(CurvaAtual.EstTs.EstVirtual + CurvaAtual.Lc),"SC");
      CurvaAtual.EstCs = CEstaca(CEstaca(fabs(CurvaAtual.AcCirc) * CurvaAtual.Raio +  CurvaAtual.EstSc.EstVirtual),"CS");
      CurvaAtual.EstSt = CEstaca(CEstaca(CurvaAtual.EstCs.EstVirtual + CurvaAtual.Lc),"ST");

      if (CurvaAtual.Tipo == CIRCULAR)
      {
        CurvaAtual.EstPc = CurvaAtual.EstSc;
        CurvaAtual.EstPc.Descricao = "PC ";
        CurvaAtual.EstPt = CurvaAtual.EstCs;
        CurvaAtual.EstPt.Descricao = "PT ";
      }

      //--- Recalcula as coordenadas da Curva Se tiver uma curva anterior

      if (PCurvas[i-1] != mapCurHorizontaisTS.end()) 
      {
        //--- A Alteração nao pode ser feita, uma curva esta entrando na outra.

        if(CalculaDadosCurva(curvas[i],curvas[i-1],&curvas[i+1]) != CCurHorizontais::CURVA_OK) 
          return LACOS_TRANSPASSANDO;   
      }
    }
    else
    {
    }
  } 

  //--- Remove as curvas anteriores dos sets 

  ItMCurHorizontaisPI itCurvaNoSetPI(mapCurHorizontaisPI.find(&itAtual->second.PI));        //--- Salva o iterador para esta curva no set ordenado por PI
  ItMCurHorizontaisCC itCurvaNoSetCC(mapCurHorizontaisCC.find(&itAtual->second.CC));        //--- Salva o iterador para esta curva no set ordenado por CC

  mapCurHorizontaisTS.erase(itAtual);                      
  mapCurHorizontaisPI.erase(itCurvaNoSetPI);               
  mapCurHorizontaisCC.erase(itCurvaNoSetCC);               

  //--- Insere a curva alterada nos dois sets e retorna o iterador para a curva inserida.

  curvas[2].mapSecoesDesenho.clear(); 

  if(InsereCurvaNosMapas(curvas[2]) == true)
  {
    ItCurSendoAlteradaCC = mapCurHorizontaisCC.find(&itAtual->second.CC);
    AtualizaLimites(curvas[2]);

    return CURVA_OK;
  }

  return ERRO_INSERIR_MAPA;
}

int CCurHorizontais::RemoveCurva(ItCMCurHorizontaisPI ItConstCurRemHorizPI,ItMCurHorizontaisTS ItUltimaCurvaAjustadaTS,CEstaca* pNovaEstCurvaAnterior)
{
  //--- Os tres sets andam juntos, removeu de um tem q remover do outro.

  ItMCurHorizontaisPI itCurRemHorizPI(mapCurHorizontaisPI.find(&ItConstCurRemHorizPI->second->PI));    //--- somente para tirar a constante do iterador...
  ItMCurHorizontaisCC itCurRemHorizCC(mapCurHorizontaisCC.find(&ItConstCurRemHorizPI->second->CC));
  ItMCurHorizontaisTS itCurRemHorizTS(mapCurHorizontaisTS.find(ItConstCurRemHorizPI->second->EstTs));

  //--- Se não for a primeira ou a ultima curva que estao sendo removidas é preciso fazer ajustes nas curvas adjacentes.

  ItMCurHorizontaisTS itCurRemAnteriorTS(itCurRemHorizTS),itCurRemPosteriorTS(itCurRemHorizTS);

  if (mapCurHorizontaisTS.size() > 1)
  {
    itCurRemAnteriorTS--;          //--- Posiciona no anterior
    itCurRemPosteriorTS++;         //--- Posiciona no posterior

    //--- Ajusta as curvas adjacentes, manda com o mesmo pi para não altera-lo
    //--- Tudo  se passa como se o pi da curva posterior tivesse sido movido.
    //--- Porisso manda a curva posterior com o mesmo. Deste modo serão feitos os 
    //--- ajustes nas curvas adjacentes.

    if (itCurRemHorizTS != mapCurHorizontaisTS.begin() && itCurRemHorizTS != --mapCurHorizontaisTS.end())  //--- Remoção no meio
    {
      CurHorizontal CurvaRemovida(itCurRemHorizTS->second);

      mapCurHorizontaisPI.erase(itCurRemHorizPI);          
      mapCurHorizontaisCC.erase(itCurRemHorizCC);              
      mapCurHorizontaisTS.erase(itCurRemHorizTS); 

      int Resp(AlteraCurvaPI(mapCurHorizontaisPI.find(&itCurRemPosteriorTS->second.PI),Ponto(itCurRemPosteriorTS->second.PI),ItUltimaCurvaAjustadaTS,pNovaEstCurvaAnterior));    

      if(Resp != CCurHorizontais::CURVA_OK)    //--- A remoção da curva provocou alguma situação inválida? 
      {
        InsereCurvaNosMapas(CurvaRemovida);
         return Resp;
      }
    }
    else   
    { 
      //--- Neste ponto é a primeira ou a última curva do set q esta sendo removida.
      //--- Neste caso é preciso zerar os raios da curva posterior ou da 
      //---- anterior, respectivamente.
    
      bool APAGOUPRIMEIRA(itCurRemHorizTS == mapCurHorizontaisTS.begin());

      ItMCurHorizontaisTS itMapTSCurAzerarRaio(APAGOUPRIMEIRA ? itCurRemPosteriorTS : itCurRemAnteriorTS);

      //--- Zera o raio da curva posterior (ou anterior). Se for a ultima curva do set que estiver tendo o raio zerado 
      //--- itCurAzerarRaio fica = end(), então é preciso verificar antes de zerar.
      
      if (itMapTSCurAzerarRaio != mapCurHorizontaisTS.end())
      {
        double AzTS(itMapTSCurAzerarRaio->second.TS.fi),AzST(itMapTSCurAzerarRaio->second.ST.fi);                   //--- Salva os azimutes, um deles será usado      

        itMapTSCurAzerarRaio->second.Raio = itMapTSCurAzerarRaio->second.Lc = itMapTSCurAzerarRaio->second.Ts = itMapTSCurAzerarRaio->second.AcCirc = itMapTSCurAzerarRaio->second.Ac = 0.0;  
        itMapTSCurAzerarRaio->second.TS = itMapTSCurAzerarRaio->second.SC = itMapTSCurAzerarRaio->second.CS = itMapTSCurAzerarRaio->second.ST = itMapTSCurAzerarRaio->second.PI;  //--- Não acerta o CC, vai precisar para remover a curva do set por CC
        itMapTSCurAzerarRaio->second.TS.fi = itMapTSCurAzerarRaio->second.ST.fi =  APAGOUPRIMEIRA ? AzST : AzTS;    //--- Acerta os azimutes do TS e do ST
        itMapTSCurAzerarRaio->second.EstTs = itMapTSCurAzerarRaio->second.EstPc = itMapTSCurAzerarRaio->second.EstSc = itMapTSCurAzerarRaio->second.EstCs = itMapTSCurAzerarRaio->second.EstPt = itMapTSCurAzerarRaio->second.EstSt = APAGOUPRIMEIRA ? mapCurHorizontaisTS.begin()->second.EstTs : mapCurHorizontaisTS.rbegin()->second.EstTs ;

        //--- Remove a curva deletada pelo usuário

        mapCurHorizontaisPI.erase(itCurRemHorizPI);          
        mapCurHorizontaisCC.erase(itCurRemHorizCC);              
        mapCurHorizontaisTS.erase(itCurRemHorizTS); 

        //--- O Ts da proxima curva (ou da anterior) mudou (o Raio foi zerado), então tem q remover a curva antiga e inserir a nova no mapa de TS o que provaca a remoção e inserção nos outros sets

        CurHorizontal CurHorizTemp(itMapTSCurAzerarRaio->second);  //--- Salva a curva , ela ja foi alterada, falta só acertar o CC

        itCurRemHorizPI = mapCurHorizontaisPI.find(&itMapTSCurAzerarRaio->second.PI);    //--- pega os iteradores mas os mapas, o do ts já esta em itMapTSCurAzerarRaio
        itCurRemHorizCC = mapCurHorizontaisCC.find(&itMapTSCurAzerarRaio->second.CC);

        mapCurHorizontaisPI.erase(itCurRemHorizPI);          
        mapCurHorizontaisCC.erase(itCurRemHorizCC);              
        mapCurHorizontaisTS.erase(itMapTSCurAzerarRaio->first);

        CurHorizTemp.CC = CurHorizTemp.PI;    //--- Removeu, agora Acerta as coordenadas do centro da curva ( O raio foi zerado)

        if(InsereCurvaNosMapas(CurHorizTemp) == false)
        {     
          AfxMessageBox("Erro na remoção da curva, contact o suporte : VU&E-005");
          return CCurHorizontais::CURVA_NOK;
        }
    
        if (mapCurHorizontaisTS.size() > 1) AlteraCurvaPI(mapCurHorizontaisPI.find(&CurHorizTemp.PI),mapCurHorizontaisPI.find(&CurHorizTemp.PI)->second->PI,ItUltimaCurvaAjustadaTS,NULL);    //--- Acerta a curva, zerando o raio   
      }
    }
  }
  else
  {
    mapCurHorizontaisPI.erase(itCurRemHorizPI);     //--- Deixar com o iterador só para testar a integridade dos mapas     
    mapCurHorizontaisCC.erase(itCurRemHorizCC);     //--- IDEM         
    mapCurHorizontaisTS.clear();                    //--- A estaca do TS nao bate porque é a última curva, porisso não se pode usar o erase; (só tem uma curva mesmo)
  }

  return CCurHorizontais::CURVA_OK;
}

void  CCurHorizontais::ReEstaqueia(ItMCurHorizontaisTS itMCHInicial,double UltimoSt,ItCMCurHorizontaisPI& ItCSendoAlteradaPI)
{
  // Este mapa esta ordenado por estaca do TS, então é preciso remover e inserir.

  MCurHorizontaisTS MapTsTemp;
  ItMCurHorizontaisTS itMCHIniBack(itMCHInicial);

  double Diferenca(UltimoSt - itMCHInicial->second.EstSt.EstReal);

  ++itMCHInicial;

  //--- soma a diferenca em todas as estacas remanescentes a partir da estaca inicial

  while(itMCHInicial != mapCurHorizontaisTS.end())
  {
    CurHorizontal CurTemp(itMCHInicial->second); 

    CurTemp.EstTs += Diferenca;
    CurTemp.EstSc += Diferenca;
    CurTemp.EstCs += Diferenca;
    CurTemp.EstSt += Diferenca;
    CurTemp.EstPc += Diferenca;
    CurTemp.EstPt += Diferenca;

    MapTsTemp.insert(pairCHCE(CurTemp.EstTs,CurTemp));     //--- Insere no set temporario

    itMCHInicial++;
  }

  mapCurHorizontaisTS.erase(++itMCHIniBack,mapCurHorizontaisTS.end());   //--- remove as curvas anteriores

  mapCurHorizontaisTS.insert(MapTsTemp.begin(),MapTsTemp.end());       //--- Insere as curvas recalculadas no set ordenado por TS
}


void CCurHorizontais::CalculaDadosComplEspiral(CurHorizontal& CurvaAtual)
{
  if (CurvaAtual.Tipo == ESPIRAL && CurvaAtual.Lc > 0.0)
  {
    CurvaAtual.p = CurvaAtual.xEsp - (CurvaAtual.Raio * (1.0 - cos(fabs(CurvaAtual.Fi))));
    CurvaAtual.q = CurvaAtual.yEsp - (CurvaAtual.Raio * sin(fabs(CurvaAtual.Fi)));
    CurvaAtual.K = CurvaAtual.Raio * CurvaAtual.Lc;
  }
  else  CurvaAtual.p = CurvaAtual.q = 0.0;

  CurvaAtual.Ts = CurvaAtual.q + (CurvaAtual.Raio + CurvaAtual.p) * tan(fabs(CurvaAtual.Ac) / 2.0); 
  CurvaAtual.AcCirc = (fabs(CurvaAtual.Ac) - fabs(CurvaAtual.Fi) * 2.0) * (CurvaAtual.Lado == ESQUERDO ? -1.0 : 1.0);
  CurvaAtual.ms = CurvaAtual.Se / CurvaAtual.Lc;
}


double CCurHorizontais::BuscaSuperLargura(const CEstaca& Estaca)
{
  if (itAtual == mapCurHorizontaisTS.end()) itAtual = mapCurHorizontaisTS.begin();  

  if (itAtual == mapCurHorizontaisTS.end()) return 0.0;

  if (Estaca < itAtual->second.EstTs) PosicionaNoMaisProximo(Estaca,FRENTE);
  else if (Estaca > itAtual->second.EstSt) PosicionaNoMaisProximo(Estaca,TRAS);

  //--- Se a estaca a calcular a superlargura estiver fora do trecho retorna 0.0

  if (itAtual == mapCurHorizontaisTS.end() || itAtual->second.Lc <= 0.0 ) return 0.0;

  //--- Se a estaca estiver dentro da curva...

  if (Estaca > itAtual->second.EstTs && Estaca < itAtual->second.EstSt)
  {
    //--- Se a estaca estiver no trecho circular retorna a superlargura máxima.

    if (Estaca >= itAtual->second.EstSc && Estaca <= itAtual->second.EstCs) return itAtual->second.Sl;

    //--- A estaca só pode estar nos trechos de transiçao, retorna  o valor conforme o caso
    //--- na transiçao anterior ou posterior. 
    //--- O calculo é feito proporcionalmente à superelevação para aproveitar o m1 ou m2

    if(itAtual->second.Sl > 0.0)   
    {
      if (Estaca < itAtual->second.EstSc) return (itAtual->second.Se - (itAtual->second.EstSc - Estaca) * itAtual->second.m1) / itAtual->second.Se *  itAtual->second.Sl ;
      else return (itAtual->second.Se - ( Estaca.EstVirtual - itAtual->second.EstCs.EstVirtual) * itAtual->second.m2) / itAtual->second.Se *  itAtual->second.Sl;
    }
  }
  
  return 0.0;  //--- A estaca está fora da curva ou SL =0.0  
}  

bool CCurHorizontais::Consiste()
{
  POSITION Atual = GetHeadPosition();
  int i(0);

  Erro = 0;

  if(!Atual) return true;

  while(Atual && Erro == 0)
  {
    i++;
    Erro = GetNext(Atual).Consiste();

    if (Erro)
    {
      CString Mensg;

      Mensg.Format("Erro no arquivo de curvas horizontais: o registro %d está invalido: ",i);
      Mensg += monolog.ErrosCurHor[Erro-1];

      monolog.mensagem(15,Mensg);
    }
  }
  return Erro > 0;
}

/*
bool CCurHorizontais::CalculaNovoCC(CurHorizontal& Curva,double RaioAnterior)
{
//--- Calcula as coordenadas do novo centro

if (Curva.Raio < 10.0) return false;

double Az((Mat.Matem::M_PI_2 - Curva.Ac) / 2.0 + 
Curva.TS.fi);
double Distancia(Curva.Raio - RaioAnterior);

Ponto RAtual(Curva.CC);

Curva.CC.x = RAtual.x + sin(Az) * Distancia;
Curva.CC.y = RAtual.y + cos(Az) * Distancia;




return true;  
}
*/

const CEstaca& CCurHorizontais::CalculaIntersecaoEixo(const Ponto& PAnt,const Ponto& PPost,Ponto& Intersecao)
{
  static CEstaca EstR;
  int AchouTangente(false),AchouCircular(false);
  double  DistPonto;

  ItMCurHorizontaisTS ItMCurHorizAnt(mapCurHorizontaisTS.begin()),ItMCurHorizPost(mapCurHorizontaisTS.begin());
  ItMCurHorizPost++;

  do
  {
    double Dist(INFINITO);

    if(CLPontos::CalculaIntersecao(PAnt,PPost,ItMCurHorizAnt->second.PI,ItMCurHorizPost->second.PI,Intersecao))
    {
      double Dist(ItMCurHorizPost->second.EstTs -  ItMCurHorizAnt->second.EstSt + ItMCurHorizAnt->second.Ts);
      DistPonto = hypot((Intersecao.x -ItMCurHorizAnt->second.PI.x),(Intersecao.y - ItMCurHorizAnt->second.PI.y));

      if(DistPonto <  Dist && DistPonto > ItMCurHorizAnt->second.Ts)  AchouTangente = true;
    }

    if(!AchouTangente)
    { 
      //Calcula na circular da frente;
 
      Ponto S1,S2;
 
      if( Mat.CalcInterRetaCurva(PAnt,PPost,ItMCurHorizPost->second.CC,ItMCurHorizPost->second.Raio,S1,S2))
      {
        const Ponto& PXAnt(PAnt.x < PPost.x ? PAnt : PPost),PXPost(PAnt.x > PPost.x ? PAnt : PPost);

        if(S1.x > PXAnt.x && S1.x <  PXPost.x)  Intersecao = S1;
        else Intersecao = S2;

        AchouCircular = true;
      }
      else
      {
        ItMCurHorizAnt++;
        ItMCurHorizPost++;
      }
    }
  }while(!AchouTangente && ! AchouCircular && ItMCurHorizPost != mapCurHorizontaisTS.end());
  

  if(ItMCurHorizPost != mapCurHorizontaisTS.end())    //--- Se Achou alguma coisa.
  {
    if(AchouTangente)  EstR = ItMCurHorizAnt->second.EstTs +  DistPonto - ItMCurHorizAnt->second.Ts;
    else 
    {
      if(AchouCircular)
      {
        Intersecao.fi += ItMCurHorizPost->second.Ac > 0.0 ? + Matem::M_PI_2 : -Matem::M_PI_2;
        double Desenv(fabs(Intersecao.fi - ItMCurHorizPost->second.TS.fi) * ItMCurHorizPost->second.Raio);
        EstR = ItMCurHorizPost->second.EstTs + Desenv;     
      }
    }

    double Lado(hypot(Intersecao.x - PAnt.x, Intersecao.y - PAnt.y));

    //--- calcula o lado esquerdo guarda o lado esquerdo em z e o direito em i

    if(Mat.CalculaAreaTriangulo(ItMCurHorizAnt->second.PI,ItMCurHorizPost->second.PI,PAnt) < 0.0)  Intersecao.z = Lado;
    else Intersecao.i = Lado;

    //-- Calcula o lado direito 

    Lado = hypot(Intersecao.x - PPost.x, Intersecao.y - PPost.y);

    if(Intersecao.z == INFINITO) Intersecao.z = Lado;
    else Intersecao.i = Lado;
  }

  return EstR;
}
double CCurHorizontais::BuscaMenorRampaPista(CEstaca& Estaca,SecTipo& Secao)
{
  if (Secao.GetCount() < 2) return INFINITO;

  double IMenor(INFINITO); 
  int IniFim(-1);
  Ponto PontoAtual;

  for(POSITION AtualSec = Secao.GetHeadPosition() ; AtualSec ; Secao.GetNext(AtualSec))
  {
    PontoAtual = Secao.GetAt(AtualSec);
    if(!Canteiros.ECanteiro(Estaca,PontoAtual.x,IniFim) && PontoAtual.i < IMenor) IMenor = PontoAtual.i;
  }
  return IMenor;
}

CArchive& CCurHorizontais::operator>>(CArchive& ar)
{
  size_t QRegistros;
  CurHorizontal CurvaAtual;

  ar >> AngRotacao;

  for(ar >> QRegistros ; QRegistros ; QRegistros--)
  {  
    CurvaAtual << ar;

    if(InsereCurvaNosMapas(CurvaAtual) == false) monolog.mensagem(38,"");
  }
  return ar;
}



/*
ItMCurHorizontaisPI itIniAlteracaoPI;

  double Diferenca = 0.00;

  int PrimcurAlterada(ANTERIOR_A_ANTERIOR);

  if(PCurvas[ANTERIOR_A_ANTERIOR] != mapCurHorizontaisTS.end())
    itIniAlteracaoPI = mapCurHorizontaisPI.find(&(PCurvas[ANTERIOR_A_ANTERIOR]->second.PI));   
  else 
    if(PCurvas[ANTERIOR] != mapCurHorizontaisTS.end())
    {
      itIniAlteracaoPI = mapCurHorizontaisPI.find(&(PCurvas[ANTERIOR]->second.PI));
      PrimcurAlterada = ANTERIOR; 
    }
    else 
      if(PCurvas[ATUAL] != mapCurHorizontaisTS.end())
      {
        itIniAlteracaoPI = mapCurHorizontaisPI.find(&(PCurvas[ATUAL]->second.PI));
        PrimcurAlterada = ATUAL; 
      }

  for(int i = PrimcurAlterada i != POSTERIOR_A_POSTERIOR ; i++)  
  {
    itIniAlteracaoPI = 

  }



  for(ItMCurHorizontaisPI itAtual = itIniAlteracaoPI ; itAtual != mapCurHorizontaisPI.end() ; itAtual++)  
  {
    itAtual->second->EstTs += Diferenca;
    itAtual->second->EstSc += Diferenca;
    itAtual->second->EstCs += Diferenca;
    itAtual->second->EstPc += Diferenca;
    itAtual->second->EstPt += Diferenca;
  }

   ItMCurHorizontaisCC itIniAlteracaoCC;

  if(PCurvas[ANTERIOR_A_ANTERIOR] != mapCurHorizontaisTS.end())
    itIniAlteracaoCC = mapCurHorizontaisCC.find(&(PCurvas[ANTERIOR_A_ANTERIOR]->second.CC));   
  else if(PCurvas[ANTERIOR] != mapCurHorizontaisTS.end())
         itIniAlteracaoCC = mapCurHorizontaisCC.find(&(PCurvas[ANTERIOR]->second.CC));
       else if(PCurvas[ATUAL] != mapCurHorizontaisTS.end())
         itIniAlteracaoCC = mapCurHorizontaisCC.find(&(PCurvas[ATUAL]->second.CC));
 
  for(ItMCurHorizontaisCC itAtual = itIniAlteracaoCC ; itAtual != mapCurHorizontaisCC.end() ; itAtual++)  
  {
    itAtual->second->EstTs += Diferenca;
    itAtual->second->EstSc += Diferenca;
    itAtual->second->EstCs += Diferenca;
    itAtual->second->EstPc += Diferenca;
    itAtual->second->EstPt += Diferenca;
  }
    
    */

/*

  UltimaCurvaAjustada.EstTs += Diferenca;

  ItMCurHorizontaisTS itLixo =  mapCurHorizontaisTS.find(UltimaCurvaAjustada.EstTs);

  if(itLixo == mapCurHorizontaisTS.end())
  {
    ASSERT(0);
  }
  
  //--- Como a curva mudou é preciso excluir a curva anterior e inserir a nova pois a ordem foi alterada
  //--- e os sets estão corrompidos. Esta deleção e inclusão acertará a ordem dos elementos no set.
  //--- Isto ocorre somente nos sets de PI e CC pois no set ordenado por Ts a alteração das estacas 
  //--- não corrompe o set ordem vai continuar a ser a mesma pois nos caso de um Ts tranpor um St a 
  //--- alteração é abortada e a função retorna false.

  //--- O programa original foi feito com set, mas na versão 10 do visual C++ os sets são const e tive que mudar para map
  //--- Teoricamente esse ajuste não sera necessario pos as curvas já deveriam estar na ordem correta no mapa sem a necessidade de remoção 
  //--- das anteriores e inserção das atuais mas se tirar esse ajuste fica uma bagunça .... ===>>> VERIFICAR

  for (i = ANTERIOR ; i < POSTERIOR_A_POSTERIOR ; i++)
  {
    if (PCurvas[i] == mapCurHorizontaisTS.end()) continue;    //--- Curva não existe, faz as outras

    CEstaca EstTsAtual(curvas[i].EstTs + Diferenca);
    ItMCurHorizontaisTS itCurAtual(mapCurHorizontaisTS.find(EstTsAtual));

    if(itCurAtual == mapCurHorizontaisTS.end())
    {
      ASSERT(0);
    }

 

    //--- Salva os iteradores para as curvas com os valores antigos nos sets ordenados por PI e CC 

    Ponto * p1 = &itCurAtual->second.PI,*p2 = &itCurAtual->second.CC;
        
    ItMCurHorizontaisTS itCurvaNoSetTS(mapCurHorizontaisTS.find(itCurAtual->first));        
    ItMCurHorizontaisPI itCurvaNoSetPI(mapCurHorizontaisPI.find(&(itCurAtual->second.PI)));        
    ItMCurHorizontaisCC itCurvaNoSetCC(mapCurHorizontaisCC.find(&(itCurAtual->second.CC))); 

    //--- Tudo ok?, então remove s curva anterior dos (antigos sets) mapas ordenados por PI e CC, (agora foi preciso incluir o por TS tb, por causa da chave) os sets passam a ficar apenas com a 
    //--- curva alterada
  
    if(itCurvaNoSetTS == mapCurHorizontaisTS.end())  ASSERT(0);
    else mapCurHorizontaisTS.erase(itCurvaNoSetTS);
    if(itCurvaNoSetPI == mapCurHorizontaisPI.end())  ASSERT(0);
    else mapCurHorizontaisPI.erase(itCurvaNoSetPI);
    if(itCurvaNoSetCC == mapCurHorizontaisCC.end())  ASSERT(0);
    else mapCurHorizontaisCC.erase(itCurvaNoSetCC);

//    if(InsereCurvaNosMapas(curvas[i]) == true) if (i == ATUAL) ItCurSendoAlteradaPI = mapCurHorizontaisPI.find(&curvas[i].PI);;    
//    return CCurHorizontais::ERRO_INSERIR_MAPA;

    

    //--- Atualiza a curva atual no set ordenado por Ts  (PCurvas[i] aponta para a curva no set ordenado por Ts)

  //  PCurvas[i]->second = curvas[i];                      

    //--- Insere nos sets de PI e CC a curva com os novos valores (de PI e de CC), neste momento
    //--- estes dois sets ficam com a curva anterior e a nova

    pairInsMCHTS PairSCurHorizTSInserida(mapCurHorizontaisTS.insert(pairCHCE(curvas[i].EstTs,curvas[i]))); 
    if(PairSCurHorizTSInserida.second == true)
    {
 //     pairInsMCHPI PairSCurHorizPIInserida(mapCurHorizontaisPI.insert(pairpCHPO(&PCurvas[i]->second.PI,&PCurvas[i]->second)));

      //pairpCHPO PInsere(&PairSCurHorizTSInserida.first->second.PI,&PairSCurHorizTSInserida.first->second);

      pairInsMCHPI PairSCurHorizPIInserida(mapCurHorizontaisPI.insert(pairpCHPO(&PairSCurHorizTSInserida.first->second.PI,&PairSCurHorizTSInserida.first->second)));
      if(PairSCurHorizPIInserida.second == true)
      {
        //--- Atualiza o valor do novo iterador para a curva sendo alterada (com as modificações)

        if (i == ATUAL) ItCurSendoAlteradaPI = PairSCurHorizPIInserida.first;     

        pairInsMCHCC PairMCurHorizCCnserida(mapCurHorizontaisCC.insert(pairpCHPO(&PairSCurHorizTSInserida.first->second.CC,&PairSCurHorizTSInserida.first->second)));
        if(PairMCurHorizCCnserida.second != true) 
          ASSERT(0);
      }
      else 
        ASSERT(0);
    }
    else 
      ASSERT(0);
  }
*/

/*
  switch(Comecou)
  {
    case ANTERIOR_A_ANTERIOR:
    {
      for (i = ANTERIOR , itAtual++ ; i < POSTERIOR_A_POSTERIOR ; i++) lstItMCurHorTS.push_back(itAtual++);
      itAtual = itAtualBack;
      for (std::list<ItMCurHorizontaisTS> ::iterator itlstItMCurHorTS = lstItMCurHorTS.begin() ; itlstItMCurHorTS != lstItMCurHorTS.end() ; itlstItMCurHorTS++)
        mapCurHorizontaisTS.erase(*itlstItMCurHorTS);
      for(i = ANTERIOR ; i < POSTERIOR_A_POSTERIOR ; i++) mapCurHorizontaisTS.insert(pairCHCE(curvas[i].EstTs,curvas[i]));
    }
    break;
    case ANTERIOR:
    {
      for (i = ANTERIOR ; i < POSTERIOR_A_POSTERIOR ; i++) lstItMCurHorTS.push_back(itAtual++);
      itAtual = itAtualBack;
      for (std::list<ItMCurHorizontaisTS> ::iterator itlstItMCurHorTS = lstItMCurHorTS.begin() ; itlstItMCurHorTS != lstItMCurHorTS.end() ; itlstItMCurHorTS++)
        mapCurHorizontaisTS.erase(*itlstItMCurHorTS);
      for(i = ANTERIOR ; i < POSTERIOR_A_POSTERIOR ; i++) mapCurHorizontaisTS.insert(pairCHCE(curvas[i].EstTs,curvas[i]));
    }
    break;
    case ATUAL:
    {
      for (i = ATUAL ; i < POSTERIOR_A_POSTERIOR ; i++) lstItMCurHorTS.push_back(itAtual++);
      itAtual = itAtualBack;
      for (std::list<ItMCurHorizontaisTS> ::iterator itlstItMCurHorTS = lstItMCurHorTS.begin() ; itlstItMCurHorTS != lstItMCurHorTS.end() ; itlstItMCurHorTS++)
        mapCurHorizontaisTS.erase(*itlstItMCurHorTS);
      for(i = ATUAL ; i < POSTERIOR_A_POSTERIOR ; i++) mapCurHorizontaisTS.insert(pairCHCE(curvas[i].EstTs,curvas[i]));
    }
    break;
  }
  */

bool CCurHorizontais::BackupArquivos()
{
  if(NomeArquivo.GetLength())
  {
    std::string strNomeArquivo(NomeArquivo.GetBuffer());
    strNomeArquivo=strNomeArquivo.substr(0,strNomeArquivo.rfind('.'));

    if(strNomeArquivo.size())
    {
       CopyFile(NomeArquivo+".chc",NomeArquivo+".chcbak",false);
       CopyFile(NomeArquivo+".geo",NomeArquivo+".geobak",false);
    }
  }

  return true;
}

bool CCurHorizontais::VoltaBackupArquivos()
{
  if(NomeArquivo.GetLength())
  {
    std::string strNomeArquivo(NomeArquivo.GetBuffer());
    strNomeArquivo=strNomeArquivo.substr(0,strNomeArquivo.rfind('.'));

    if(strNomeArquivo.size())
    {
       CopyFile(NomeArquivo+".chcbak",NomeArquivo+".chc",false);
       CopyFile(NomeArquivo+".geobak",NomeArquivo+".geo",false);
    }
  }

  return true;
}

int CCurHorizontais::CalculaDadosComplementares()
{
  if (mapCurHorizontaisTS.size() == 0) return CALCULO_OK;

  ItMCurHorizontaisTS itCHAtual(mapCurHorizontaisTS.begin()),itCHAnterior;

  //--- Cria a lista de curvasHorizontais calculadas. 
  //--- Inicializa a lista criando o primeiro nó, neste nó não há cálculo de I1 e E.

  itCHAtual->second.EstPc = itCHAtual->second.EstPt = itCHAtual->second.EstSt = itCHAtual->second.EstTs = 
  itCHAtual->second.EstCs = itCHAtual->second.EstSc = itCHAtual->second.EstTs;  

  //--- Preenchimento apenas para validação da curva, na primeira curva o ac e raio são = 0.  

  itCHAtual->second.Lado = DIREITO;     //--- Tanto faz....
  itCHAtual->second.Tipo = CIRCULAR;    
  itCHAtual->second.Ac = 0.0;
  itCHAtual->second.Raio = 0.0;
  itCHAtual->second.Se = 0.0;
  itCHAtual->second.Sl = 0.0;

  SecTipo SecaoTipo;
  if (SecoesTipo.BuscaSecTipo(itCHAtual->second.EstTs, SecaoTipo) == FALTA_SEC_TIPO)
  {
    std::string strMsg("Falta seção tipo : Estaca-> ");
    strMsg += itCHAtual->second.EstTs;

    AfxMessageBox(strMsg.c_str(), MB_ICONERROR);

    return CALCULO_NOK;
  }

  itCHAtual->second.m1 = 0.0;
  itCHAtual->second.Ini = itCHAtual->second.EstTs.EstVirtual;
  itCHAtual->second.Fim = itCHAtual->second.EstSt.EstVirtual;
  AtualizaLimites(itCHAtual->second);
  itCHAnterior = itCHAtual;        //-- Primeira curva = curva anterior

  //--- Para cada um dos nós restantes da lista de Curvas lidas cria um nó na lista
  //--- de curvas calculadas, calculando os dados da curva.

  while (++itCHAtual != mapCurHorizontaisTS.end())
  {
    //--- Se a curva for espiral, constroi o Sc e o Cs, se não, (a curva é 
    //--- circular col lc > 0) tranforma a curva em espiral

    ConstroiEspiralDaCircular(itCHAtual->second);

    SecTipo SecaoTipo;
    if (SecoesTipo.BuscaSecTipo(itCHAtual->second.EstTs, SecaoTipo) == FALTA_SEC_TIPO) return CALCULO_NOK;

    //--- A principio não há superelevação

    itCHAtual->second.Ini = itCHAtual->second.EstTs.EstVirtual;
    itCHAtual->second.Fim = itCHAtual->second.EstSt.EstVirtual;    
    itCHAtual->second.m1 = itCHAtual->second.m2 = INFINITO;

    //--- Para ter superelevação precisa ter Lc  

    CurHorizontal* CC(&(itCHAtual->second));

    if(itCHAtual->second.Lc > 0.0)
    { 
      double Se(itCHAtual->second.Se),
             iTangente(BuscaMenorRampaPista(itCHAtual->second.EstSt,SecaoTipo));

      itCHAtual->second.m1 = itCHAtual->second.m2 = Se / (itCHAtual->second.Lc);
      itCHAtual->second.Ini = itCHAtual->second.EstTs.EstVirtual - fabs(iTangente / itCHAtual->second.m1);
      itCHAtual->second.Fim = itCHAtual->second.EstSt.EstVirtual + fabs(iTangente / itCHAtual->second.m2);
    }

    EstudaCaso(itCHAtual->second,itCHAnterior->second);     //-- Estuda o caso de superelevação, caso as curvas estejam muito próximas

    //mapCurHorizontaisTS.insert(*itCHAtual);   //--- Finalmente adiciona o nó na lista.    
    AtualizaLimites(itCHAtual->second);

    itCHAnterior = itCHAtual;
  }

  return CALCULO_OK;
}
