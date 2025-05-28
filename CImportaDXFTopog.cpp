#include "stdafx.h"
#include "CPonto.h"
#include "CTriangulo.h"
#include "CTriangulacao.h"
#include "CImportaDXFTopog.h"
#include "Monologo.h"
#include "Mat.h"

#define  IMP_SOMENTE 1
#define  IMP_EXCETO 2

extern monologo monolog;
extern Matem Mat;

void CImportaDXFTopografia::ImportarArquivo()
{
  for (ittlstwstr itNomeArquivo = NomesArquivos.begin(); itNomeArquivo != NomesArquivos.end(); itNomeArquivo++)
  {
  //  ImportaArquivo(*itNomeArquivo);
  }
}

bool CImportaDXFTopografia::ImportaArquivo(const std::string& NomeArquivo, const std::string& NomeArqAciTopog)
{
  std::ifstream ArqDXFTop(NomeArquivo);
  std::stringstream strstrMsg;

  if (!ArqDXFTop.is_open() || !ArqDXFTop.good())
  {
    monolog.mensagem(13, CString(NomeArquivo.c_str()) + " (lendo os dados)");
    return false;
  }

  int C(0);
  std::string Entitiy;

  do
  {
    C++;
    std::getline(ArqDXFTop, Entitiy);
    Limpa(Entitiy, "\t ");
  } while (Entitiy != "ENTITIES");

  std::string Label, Valor;
  int ContaCN(0);

  do
  {
    C++;

    std::getline(ArqDXFTop, Entitiy);
    Limpa(Entitiy, "\t ");

    if (Entitiy == "3DFACE")
    {
      Ponto P1,P2,P3;

      do
      {
        C++;
        std::getline(ArqDXFTop, Label);
        Limpa(Label, "\t ");

        if (Label == "0") break;

        C++;
        std::getline(ArqDXFTop, Valor);
        Limpa(Valor, "\t ");

        if (Label == "0") break;

        if (Label == "10") P1.x = atof(Valor.c_str());
        else if (Label == "20") P1.y = atof(Valor.c_str());
        else if (Label == "30") P1.z = atof(Valor.c_str());

        else if (Label == "11") P2.x = atof(Valor.c_str());
        else if (Label == "21") P2.y = atof(Valor.c_str());
        else if (Label == "31") P2.z = atof(Valor.c_str());

        else if (Label == "12") P3.x = atof(Valor.c_str());
        else if (Label == "22") P3.y = atof(Valor.c_str());
        else 
        if (Label == "32")
        {
          P3.z = atof(Valor.c_str());
          Triangulacao.InsereTriangulo(P1,P2,P3);
        }
      } while (true);    //--- Sai no break
    }
  } while (Entitiy != "ENDSEC");

  return true;
}

bool CImportaDXFTopografia::ImportaCNs(const std::string& NomeArquivo, const std::string& CNMestra, const std::string& CNInterm)
{
  std::ifstream ArqDXFTop(NomeArquivo);
  std::stringstream strstrMsg;

  if (!ArqDXFTop.is_open() || !ArqDXFTop.good())
  {
    monolog.mensagem(13, CString(NomeArquivo.c_str()) + " (lendo os dados)");
    return false;
  }

  std::string Tabelas;
  int C(0);

  do
  {
    C++;
    std::getline(ArqDXFTop, Tabelas);
    Limpa(Tabelas, "\t ");
  } while (Tabelas != "AcDbLayerTableRecord");

  Camadas.clear();

  do
  {
    C++;
    std::getline(ArqDXFTop, Tabelas);
    Limpa(Tabelas, "\t ");

    if (Tabelas == "2")
    {
      C++;
      std::getline(ArqDXFTop, Tabelas);
      Limpa(Tabelas, "\t ");

      {
        Camadas.emplace_back(Tabelas);
      }

      do
      {
        C++;
        std::getline(ArqDXFTop, Tabelas);
        Limpa(Tabelas, "\t ");
      } while (Tabelas != "AcDbLayerTableRecord" && Tabelas != "ENDSEC");
    }
  } while (Tabelas != "ENDSEC");

  SalvaArquivoCamadas(NomeArquivo);

  CurvasNivel.clear();
  std::string Entitiy;

  do
  {
    C++;
    std::getline(ArqDXFTop, Entitiy);
    Limpa(Entitiy, "\t ");
  } while (Entitiy != "ENTITIES");

  std::string Label, Valor;
  int ContaCN(0);

  do
  {
    C++;

    std::getline(ArqDXFTop, Entitiy);
    Limpa(Entitiy, "\t ");

    if (Entitiy == CNMestra || Entitiy == CNInterm)
    {
      Ponto PAtual;
      double CotaAtual(INFINITO);

      CurvasNivel.emplace_back(LPontos());

      LPontos& CNAtual(*CurvasNivel.rbegin());

      do
      {
        C++;
        std::getline(ArqDXFTop, Label);
        Limpa(Label, "\t ");

        if (Label == "0") break;

        C++;
        std::getline(ArqDXFTop, Valor);
        Limpa(Valor, "\t ");

        if (Label == "0") break;

        if(Label == "38") CotaAtual = atof(Valor.c_str());
        else if (Label == "10") PAtual.x = atof(Valor.c_str());
        else if (Label == "20")
        {
          PAtual.y = atof(Valor.c_str());
          PAtual.z = CotaAtual;

          CNAtual.emplace_back(PAtual);
        }
      } while (true);    //--- Sai no break
    }
  } while (Entitiy != "ENDSEC");

  for (ItLLPontos ItCN = CurvasNivel.begin(); ItCN != CurvasNivel.end() ; )
  {
    if(ItCN->size() < 2) CurvasNivel.erase(ItCN++);
    else  ItCN++;
  }

  return true;
}

bool CImportaDXFTopografia::ImportaCNsSIG(const std::string& NomeArquivo, const std::string& strEntity)
{
  std::ifstream ArqDXFCNSIG(NomeArquivo);
  std::stringstream strstrMsg;

  if (!ArqDXFCNSIG.is_open() || !ArqDXFCNSIG.good())
  {
    monolog.mensagem(13, CString(NomeArquivo.c_str()) + " (lendo os dados)");
    return false;
  }

  unsigned int C(0);
  CurvasNivel.clear();
  std::string Entitiy;

  do
  {
    C++;
    std::getline(ArqDXFCNSIG, Entitiy);
    Limpa(Entitiy, "\t ");
  } while (Entitiy != "ENTITIES");

  std::string Label, Valor;
  int ContaCN(0);

  do
  {
    C++;

    std::getline(ArqDXFCNSIG, Entitiy);
    Limpa(Entitiy, "\t ");

    if (Entitiy == strEntity)
    {
      Ponto PAtual;
      double CotaAtual(INFINITO);

      if (CurvasNivel.size() > 1 && CurvasNivel.rbegin()->size() < 2)
      {
        CurvasNivel.pop_back();
      }

      CurvasNivel.emplace_back(LPontos());

      LPontos& CNAtual(*CurvasNivel.rbegin());

      do
      {
        C++;
        std::getline(ArqDXFCNSIG, Label);
        Limpa(Label, "\t ");

        if (Label == "0") break;

        std::getline(ArqDXFCNSIG, Valor);
        Limpa(Valor, "\t ");

        if (Label == "0") break;

        if (Label == "38") CotaAtual = atof(Valor.c_str());
        else if (Label == "10") PAtual.x = atof(Valor.c_str());
        else if (Label == "20")
        {
          PAtual.y = atof(Valor.c_str());
          PAtual.z = CotaAtual;

          CNAtual.emplace_back(PAtual);
        }
      } while (true);    //--- Sai no break
    }
  } while (Entitiy != "ENDSEC");

  return true;
}
bool CImportaDXFTopografia::ImportaTalveguesSIG(const std::string& NomeArquivo, const std::string& strEntity)
{
  std::ifstream ArqDXFCNSIG(NomeArquivo);
  std::stringstream strstrMsg;

  if (!ArqDXFCNSIG.is_open() || !ArqDXFCNSIG.good())
  {
    monolog.mensagem(13, CString(NomeArquivo.c_str()) + " (lendo os dados)");
    return false;
  }

  unsigned int C(0);
  Talvegues.clear();
  std::string Entitiy;

  do
  {
    C++;

    std::getline(ArqDXFCNSIG, Entitiy);
    Limpa(Entitiy, "\t ");
  } while (Entitiy != "ENTITIES");

  std::string Label, Valor;
  int ContaCN(0);

  do
  {
    C++;

    std::getline(ArqDXFCNSIG, Entitiy);
    Limpa(Entitiy, "\t ");

    if (Entitiy == strEntity)
    {
      Ponto PAtual;
      double CotaAtual(INFINITO);

      if (Talvegues.size() > 1 && Talvegues.rbegin()->size() < 2)
      {
        Talvegues.pop_back();
      }

      Talvegues.emplace_back(LPontos());

      LPontos& CNAtual(*Talvegues.rbegin());

      do
      {
        C++;
        std::getline(ArqDXFCNSIG, Label);
        Limpa(Label, "\t ");

        if (Label == "0") break;

        std::getline(ArqDXFCNSIG, Valor);
        Limpa(Valor, "\t ");

        if (Label == "0") break;

        else if (Label == "10") PAtual.x = atof(Valor.c_str());
        else if (Label == "20")
        {
          PAtual.y = atof(Valor.c_str());

          CNAtual.emplace_back(PAtual);
        }
      } while (true);    //--- Sai no break
    }
  } while (Entitiy != "ENDSEC");

  return true;

}


bool CImportaDXFTopografia::SalvaArquivoCamadas(const std::string& NomeCompleto)
{
  std::string NomeCompArquivo(CaminhoProjeto + ".cam");

  CFile ArqCamada;
  CFileException e;

  if (ArqCamada.Open(NomeCompArquivo.c_str(), CFile::modeCreate | CFile::modeWrite, &e) == 0)
  {
    monolog.mensagem(13, CString(NomeCompArquivo.c_str()) + " (Gravando os dados)");
    throw e.m_cause;
  }
  else
  {
    CArchive arCamadas(&ArqCamada, CArchive::store);

    arCamadas << Camadas.size();

    for (ittlstwstr It = Camadas.begin() ; It != Camadas.end() ; It++)
    {
      arCamadas << *It;
    }
  }
}
int CImportaDXFTopografia::LeCamadas(std::ifstream& ArqDXFTopog)
{
  std::string Tabelas;
  int C(0);

  do
  {
    C++;
    std::getline(ArqDXFTopog, Tabelas);
    Limpa(Tabelas, "\t ");
  } while (Tabelas != "AcDbLayerTableRecord");

  Camadas.clear();

  do
  {
    C++;
    std::getline(ArqDXFTopog, Tabelas);
    Limpa(Tabelas, "\t ");

    if (Tabelas == "2")
    {
      C++;
      std::getline(ArqDXFTopog, Tabelas);
      Limpa(Tabelas, "\t ");

      Camadas.emplace_back(Tabelas);

      do
      {
        C++;
        std::getline(ArqDXFTopog, Tabelas);
        Limpa(Tabelas, "\t ");
      }while (Tabelas != "AcDbLayerTableRecord" && Tabelas != "ENDSEC");
    }
  } while (Tabelas != "ENDSEC");

  return true;
}
