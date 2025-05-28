#include "stdafx.h"
#include "CPonto.h"
#include "Mat.h"
#include "CTextoMapaIBGE.h"
#include "Monologo.h"
#include "CCNMapaIGBE.h"
#include "CMapaCNIBGE.h"

extern monologo monolog;
extern Matem Mat;

int CMapaCNIBGE::LeArquivoDXF(const std::string& NomeArq)
{
  std::ifstream ArqDXF(NomeArq);
  std::stringstream strstrMsg;

  if (!ArqDXF.is_open() || !ArqDXF.good())
  {
    monolog.mensagem(13, CString(NomeArq.c_str()) + " (lendo os dados)");
    return false;
  }

  std::string Entitiy;
  int C(0);

  do
  {
    C++;
    std::getline(ArqDXF, Entitiy);
    Limpa(Entitiy, "\t ");
  } while (Entitiy != "ENTITIES");

  std::string Label, Valor;
  int ContaCN(0);
  tylstCTextos LTextos;

  do
  {
    C++;

    std::getline(ArqDXF, Entitiy);
    Limpa(Entitiy, "\t ");

    if (Entitiy == "3DFACE")
    {
      ContaCN++;

      LCN.push_back(CCNMapaIGBE());
      rittylstCN itCNAtual(LCN.rbegin());
      lstPontos& LCNAtual(itCNAtual->LstPontos());
      LCNAtual.push_back(Ponto());

      do
      {
        C++;
        std::getline(ArqDXF, Label);
        Limpa(Label, "\t ");

        if (Label == "0") break;

        C++;
        std::getline(ArqDXF, Valor);

        Limpa(Valor, "\t ");

        if (Label == "10" || Label == "20" || Label == "30" ||
            Label == "11" || Label == "21" || Label == "31" ||
            Label == "12" || Label == "22" || Label == "32")
        {
          if (Label == "10") LCNAtual.rbegin()->x = atof(Valor.c_str());
          else
          {
            if (Label == "20")
            {
              LCNAtual.rbegin()->y = atof(Valor.c_str());
              LCNAtual.rbegin()->Baliza2 = Ponto::CURVA_NIVEL;

              LCNAtual.push_back(Ponto());
            }
          }
        }
      } while (true);    //--- sai no break

      LCNAtual.pop_back();   //--- O ultimo ponto fica sobrando...
    }
  } while (Entitiy != "ENDSEC");

  return true;
};

int CMapaCNIBGE::LeArquivoDXF_DGN(const std::string& NomeArq,CProgressCtrl* pProgresso)
{
  std::ifstream ArqDXF(NomeArq);
  std::stringstream strstrMsg;

  if (!ArqDXF.is_open() || !ArqDXF.good())
  {
      monolog.mensagem(13, CString(NomeArq.c_str()) + " (lendo os dados)");
      return false;
  }

  std::string Entitiy;
  int C(0),ContaTextos(0);

  do
  {
    C++;
    std::getline(ArqDXF, Entitiy);
    Limpa(Entitiy, "\t ");
  }
  while(Entitiy != "ENTITIES");

  std::string Label, Valor;
  int ContaCN(0);

  do
  {
    C++;

    std::getline(ArqDXF, Entitiy);
    Limpa(Entitiy, "\t ");

    if (Entitiy == "TEXT" || Entitiy == "MTEXT")
    {
      LTextos.push_back(CTextoMapaIBGE());
      rittylstCTextos itTextoAtual(LTextos.rbegin());

      if (pProgresso && (LTextos.size() % 10) == 0)
      {
        pProgresso->StepIt();
      }

      do
      {
        C++;
        std::getline(ArqDXF, Label);
        Limpa(Label, "\t ");

        if (Label == "0") break;

        C++;
        std::getline(ArqDXF, Valor);
        Limpa(Valor, "\t ");

        if (Label == "10") itTextoAtual->x = atof(Valor.c_str());
        else if (Label == "20") itTextoAtual->y = atof(Valor.c_str());
        else if (Label == "11") itTextoAtual->xCentro = atof(Valor.c_str());
        else if (Label == "21") itTextoAtual->yCentro = atof(Valor.c_str());
        else  if (Label == "1") 
        {
          itTextoAtual->Texto = Valor;
        }

      } while (true);    //--- Sai no break

      if (itTextoAtual->xCentro != INFINITO || itTextoAtual->yCentro != INFINITO)
      {
          itTextoAtual->Tipo = CTextoMapaIBGE::PONTO_CN_MESTRA;
      }
      else
      {
        if (Mat.ENumero(itTextoAtual->Texto))
        {
          itTextoAtual->Tipo = CTextoMapaIBGE::PONTO_COTADO;
        }
        else itTextoAtual->Tipo = CTextoMapaIBGE::PONTO_TEXTO;
      }
    }
    else
    {
      if (Entitiy == "LWPOLYLINE")
      {
        ContaCN++;

        if (pProgresso && (ContaCN % 100) == 0)
        {
          pProgresso->StepIt();
        }

        LCN.push_back(CCNMapaIGBE());
        rittylstCN itCNAtual(LCN.rbegin());
        lstPontos& LCNAtual(itCNAtual->LstPontos());
        LCNAtual.push_back(Ponto());

        do
        {
          C++;
          std::getline(ArqDXF, Label);
          Limpa(Label, "\t ");

          if (Label == "0") break;

          C++;
          std::getline(ArqDXF, Valor);

          Limpa(Valor, "\t ");

          if (Label == "10" || Label == "20" || Label == "70")
          {
            if (Label == "10") LCNAtual.rbegin()->x = atof(Valor.c_str());
            else
            {
              if (Label == "20")
              {
                LCNAtual.rbegin()->y = atof(Valor.c_str());
                LCNAtual.rbegin()->Baliza2 = Ponto::CURVA_NIVEL;

                LCNAtual.push_back(Ponto());
              }
            }
          }
        } while (true);    //--- sai no break

        LCNAtual.pop_back();   //--- O ultimo ponto fica sobrando...
      }
    }
  } while (Entitiy != "ENDSEC");

  return true;
};

