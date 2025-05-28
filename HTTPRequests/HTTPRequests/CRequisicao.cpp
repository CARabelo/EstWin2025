#include "stdafx.h"
#include <string>
#include "CRequisicao.h"
#include <iostream>
#include <map>
#include "curl_easy.h"
#include "curl_form.h"
#include "curl_ios.h"
#include "curl_exception.h"

CRequisicao::CRequisicao()
{
}

std::string CRequisicao::get_response(std::string url) 
{
  std::stringstream strstr;
  curl::curl_ios<std::stringstream> writer(strstr);

  curl::curl_easy easy(writer);

  easy.add<CURLOPT_URL>(url.data());
  easy.add<CURLOPT_FOLLOWLOCATION>(1L);
  easy.add<CURLOPT_VERBOSE>(1L);    //--- Para debug

  try
  {
    try
    {
      char ErrBuf[1500] = {0};

      curl_easy_setopt(&easy, CURLOPT_ERRORBUFFER, ErrBuf);

      easy.perform();
    }
    catch (...)
    {
      return strstr.str();
    }
  }
  catch (curl::curl_easy_exception error)   //--- Sem internet não cai nest catch ??? Verificar
  {
    curl::curlcpp_traceback erros(error.get_traceback());
    error.print_traceback();
  }

  return strstr.str();
}

CRequisicao::~CRequisicao()
{
}

std::string CRequisicao::EnviaRequisicaoPegaResp(const std::string& url)
{
  return get_response(url);
}
