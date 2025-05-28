#include "stdafx.h"
#include <string>
#include <sstream>
#include "CHTTPRequests.h"
#include "CRequisicao.h"

CHTTPRequests::CHTTPRequests()
{
}

CHTTPRequests::~CHTTPRequests()
{
}

std::string CHTTPRequests::EnviaRequisicaoPegaResposta(std::string& pRequisicao)
{
  CRequisicao Requisicao;
  
  return Requisicao.get_response(pRequisicao);
}
