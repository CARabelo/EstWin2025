#include <iostream>
#include <ostream>
#include <string>
#include <string_view>
#include <map>
#include "curl_easy.h"
#include "curl_form.h"
#include "curl_ios.h"
#include "curl_exception.h"

std::stringstream get_response(std::string_view url)
{
   std::stringstream str;
   curl::curl_ios<std::stringstream> writer(str);

   curl::curl_easy easy(writer);

   easy.add<CURLOPT_URL>(url.data());
   easy.add<CURLOPT_FOLLOWLOCATION>(1L);
 //---  easy.add<CURLOPT_VERBOSE>(1L);    //--- Para debug

   try
   {
     easy.perform();
   }
   catch (curl::curl_easy_exception error)
   {
     curl::curlcpp_traceback erros(error.get_traceback());
     error.print_traceback();
   }

   return str;
}

int main()
{
   std::string url("https://maps.googleapis.com/maps/api/elevation/json?locations=-23.64737184441803,-46.624&key=AIzaSyBMRoEyMFkcrDPCYzwxxTMLiS4lOwH7jD4");
   std::stringstream json(get_response(url));

 //  std::cout << json.str() << std::endl;

   return 0;
}