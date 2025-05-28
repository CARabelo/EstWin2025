#include "stdafx.h"
#include "est.h"
#include "afxtempl.h"
#include "carqterreno.h"
#include "cponto.h"
#include <string>
#include "mat.h"
#include "estaca.h"
#include "clpontos.h"
#include "perfil.h"
#include "carqperfis.h"
#include "clistaperfis.h"
#include <list>
#include <set>
#include <vector>
#include <algorithm>
#include "cdeltasuperficie.h"
#include "CRetangulo.h"
#include "CAciTopografico.h"
#include <map>
#include "CAcidentesTopograficos.h"
#include "CSegmento.h"
#include "CDadosBacias.h"
#include "SetxPontos.h"
#include "CCotasAreas.h"
#include "CBacia.h"
#include "CEquipDrenagem.h"
#include "superficie.h"
#include "xygreide.h"
#include "meleditbox.h"
#include "cdpopuppontos.h"
#include "supercstring.h"
#include "carqcurhorizontais.h" 
#include "carqcanteiros.h"
#include "ccanteiros.h"
#include "carqsectipo.h"
#include "csectipo.h"
#include "carqigualdades.h"
#include "cigualdade.h"
#include "igualdades.h"
#include "projeto.h"
#include "ccurhorizontal.h"
#include "ccurhorizontais.h"
#include "carqsecoesavulsas.h"
#include "csecaoavulsa.h"
#include "csecoesavulsas.h"
#include "carqalargamentos.h"
#include "calargamentos.h"
#include "CLembrete.h"
#include "CRadar.h"
#include "CArqAciTopograficos.h"
#include "CTrecho.h"
#include "CArqTrechos.h"
#include "meleditbox.h"
#include "CTrechos.h"
#include "io.h"
#include <sstream>

CTrechos::CTrechos(const std::string& pNomeTrecho,bool bTodos) : CArqTrechos(pNomeTrecho,bTodos),itTrechoAtual(MapaTrechos.end())
{
  if (bTodos)     //--- Se é para abrir todos os trechos o nome do trecho veio com o nome basico do projeto
  {
    for (ittymapNomesTrechos it = MapaNomesTrechos.begin() ; it != MapaNomesTrechos.end() ; it++)
    {
      MapaTrechos.emplace(it->first,it->first);
    }
  }
  else            //--- Se não abre apenas o trecho solicitado, o mapa fica com apenas 1 trecho
  {
    MapaTrechos.emplace(pNomeTrecho, pNomeTrecho);
  }

  itTrechoAtual = MapaTrechos.begin();
}

CTrechos::~CTrechos()
{
}

bool CTrechos::PegaNomesTrechos(tlstwstr& NomesTrechos, const std::string& pNomeTrecho)
{
  struct _finddata_t c_fileDir, c_fileArq;
  intptr_t hFileDir;
  std::string Nome(pNomeTrecho.substr(0, pNomeTrecho.find_last_of(_T('\\')) + 1));

  //--- Verre todos os diretórios filho e verifica se há um trecho nele.

  Nome += _T("*.*");

  std::string NomeDir(Nome.begin(), Nome.end());

  if ((hFileDir = _findfirst(NomeDir.c_str(), &c_fileDir)) == -1L)
  {
    //monolog.mensagem(6, Nome);
    return FALSE;
  }
  else
  {
    NomesTrechos.clear();

    do
    {
      if (c_fileDir.attrib == _A_SUBDIR)
      {
        std::string wsNome(&c_fileDir.name[0], &c_fileDir.name[strlen(c_fileDir.name)]);
        Limpa(wsNome);

        if (wsNome.find('.') == std::string::npos)
        {
          std::string NomeArq(NomeDir.substr(0, NomeDir.find_last_of('\\')+1));
          NomeArq += wsNome + "\\" + wsNome + ".cat";

          intptr_t hFileArq;

          if ((hFileArq = _findfirst(NomeArq.c_str(), &c_fileArq)) != -1L)
          {
            //--- é trecho

            NomesTrechos.emplace_back(NomeArq.substr(0, NomeArq.find_last_of('.')));
          }
          _findclose(hFileArq);
        }
      }
    }while (_findnext(hFileDir, &c_fileDir) == 0);

    _findclose(hFileDir);
  }
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}
