#include "stdafx.h"
#include <map>
#include "CArqTrechos.h"
#include "io.h"

CArqTrechos::~CArqTrechos()
{
}

CArqTrechos::CArqTrechos(const std::string& pNomeTrecho,bool bTodos) 
{
  if (bTodos)     //--- Se é para abrir todos os trechos o nome do trecho veio com o nome basico do projeto
  {
    tlstwstr LstNomesTrechos;
    PegaNomesTrechos(LstNomesTrechos, pNomeTrecho);

    for (ittlstwstr it = LstNomesTrechos.begin(); it != LstNomesTrechos.end(); it++)
    {
      MapaNomesTrechos.emplace(*it,-1);
    }
  }
  else            //--- Se não abre apenas o trecho solicitado, o mapa fica com apenas 1 trecho
  {
    std::string NomeTrecho(pNomeTrecho.substr(0, pNomeTrecho.find_last_of(_T('\\')) + 1));


    MapaNomesTrechos.emplace(NomeTrecho,-1);
  }
}

bool CArqTrechos::PegaNomesTrechos(tlstwstr& NomesTrechos, const std::string& pNomeTrecho)
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
      if ((c_fileDir.attrib &_A_SUBDIR) == _A_SUBDIR)
      {
        std::string wsNome(&c_fileDir.name[0], &c_fileDir.name[strlen(c_fileDir.name)]);
        Limpa(wsNome);

        if (wsNome.find('.') == std::string::npos)
        {
          std::string NomeArq(NomeDir.substr(0, NomeDir.find_last_of('\\') + 1));
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
    } while (_findnext(hFileDir, &c_fileDir) == 0);

    _findclose(hFileDir);
  }
  return TRUE;  // return TRUE unless you set the focus to a control
                // EXCEPTION: OCX Property Pages should return FALSE
}
