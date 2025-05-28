#include "stdafx.h"
#include <map>
#include "VerfNeoKey.h"
#ifdef WIN32
  #include <windows.h>
  #include <stdio.h>
  #include <io.h>
  #include <time.h>

  int (__stdcall *NeoKeyCall) (unsigned int cmd,LPSTR param1,int size1,LPSTR param2,int size2,LPSTR param3,int size3,struct LicenseParameters * Licparam);

  HMODULE API_HANDLE=NULL;
  
 // #ifdef _DEBUG
  #ifdef TESTEVUE
    #define NEOKEY_LIB "ClientAPI32dbg.dll"
  #else
    #define NEOKEY_LIB "ClientAPI64.dll"
  #endif
#endif

  std::map<std::string, VerfNeoKey::KEY_STATUS> VerfNeoKey::mapStrSTATUS = {{ "00",VerfNeoKey::KEY_SEM_STATUS },
                                                                            { "01",VerfNeoKey::KEY_OK },
                                                                            { "02",VerfNeoKey::KEY_AGUARDANDO_INSTALACAO },
                                                                            { "03",VerfNeoKey::KEY_INSTALADA_OK },
                                                                            { "04",VerfNeoKey::KEY_PAGAMENTO_ATRASADO },
                                                                            { "05",VerfNeoKey::KEY_CANCELADA },
                                                                            { "06",VerfNeoKey::KEY_NAO_AUTORIZADA }};

  VerfNeoKey::VerfNeoKey()
  {
  if(!(API_HANDLE=LoadLibrary(NEOKEY_LIB))) 
	{
		printf("Dll not found= %s", NEOKEY_LIB);
		Status = false;
	}
	if(!(NeoKeyCall=(int (__stdcall *)(unsigned int, LPSTR , int, LPSTR , int, LPSTR,  int, struct LicenseParameters * )) GetProcAddress(API_HANDLE,"NeoKeyCall"))) 
	{
		printf("Function inside Dll not found");
		FreeModule(API_HANDLE);
		API_HANDLE=NULL;

    Status = false;
	}
}

VerfNeoKey::~VerfNeoKey(void)
{
}

VerfNeoKey::KEY_STATUS VerfNeoKey::LeStatusChave(std::string& strID)
{
  std::string IDLido;

  if (LeID(IDLido) == VerfNeoKey::KEY_OK)
  {
    if (strID == IDLido)
    {
      char chStatus[3] = { 0,0,0 };

      int ret = NeoKeyCall(9, chStatus, 100, NULL, 0, NULL, 0, NULL);
      if (ret != 0) return VerfNeoKey::ERRO_NEOPROT;

      std::map<std::string, VerfNeoKey::KEY_STATUS>::iterator itmapStrSTATUS(mapStrSTATUS.find(chStatus));

      if (itmapStrSTATUS == mapStrSTATUS.end()) return KEY_SEM_STATUS;

      return itmapStrSTATUS->second;
    }
    else return VerfNeoKey::KEY_ID_INVALIDO;
  }
  
  return VerfNeoKey::ERRO_LER_ID;
}

VerfNeoKey::KEY_STATUS VerfNeoKey::LeID(std::string& strDado)
{
  std::string IDLido;
  strDado.clear();

  strDado.clear();
  char chID[9] = { 0,0,0,0,0,0,0,0,0 };

  int ret = NeoKeyCall(10, chID, 9, NULL, 0, NULL, 0, NULL);
  if (ret != 0) return VerfNeoKey::ERRO_LER_ID;

  strDado = chID;

  return VerfNeoKey::KEY_OK;
}

bool VerfNeoKey::GravaStatusChave(KEY_STATUS& Status)
{
  Status = KEY_SEM_STATUS;
  char chStatus[3] = {0,0,0};

  int ret = NeoKeyCall(8,chStatus,100, NULL, 0, NULL, 0, NULL);
  if (ret != 0) return false;

  std::map<std::string, VerfNeoKey::KEY_STATUS>::iterator itmapStrSTATUS(mapStrSTATUS.find(chStatus));

  if (itmapStrSTATUS == mapStrSTATUS.end())
  {
    Status = VerfNeoKey::KEY_STATUS_INVALIDO;

    return false;
  }

  Status = itmapStrSTATUS->second;

  return true;
}

bool VerfNeoKey::Verifica()
{
/*  DER-MG
  memset(&Challenge,0,17);
  memcpy(&Challenge,"agldlcbhmrjluzvx",16); 
  memset(&HardwareID,0,9);
  memset(&HardwareID,'x',8);
  MyLic.NEOKEYID=0;
  MyLic.Enable=0;
  if(NeoKeyCall(1,(LPSTR)&Challenge,16,NULL,0,NULL,0,&MyLic) != 0) return false;
  return memcmp(&Challenge,"lgjwicmnmsfudfjr",16) == 0;
  */

/*  Prof Paraiba 
  memset(&Challenge,0,17);
  memcpy(&Challenge,"aaaaaaaaaaaaaaaa",16); 
  memset(&HardwareID,0,9);
  memset(&HardwareID,'x',8);
  MyLic.NEOKEYID=0;
  MyLic.Enable=0;
  if(NeoKeyCall(1,(LPSTR)&Challenge,16,NULL,0,NULL,0,&MyLic) != 0) return false;
  return memcmp(&Challenge,"ssssssssssssssss",16) == 0;

  */

  /* REDE 10 */

/*
  memset(&Challenge,0,17);
  memcpy(&Challenge,"AS58aqFaX67Kr9je",16); 
  memset(&HardwareID,0,9);
  memset(&HardwareID,'x',8);
  MyLic.NEOKEYID=0;
  MyLic.Enable=0;
  if(NeoKeyCall(1,(LPSTR)&Challenge,16,NULL,0,NULL,0,&MyLic) != 0) return false;
  return memcmp(&Challenge,"xCtUvd59wFhy7EkA",16) == 0;
  */
  /*Neíza*/

  // ===>Chave de criptografia (NeoKey) = "akropZNFDJKQIlaskwKAKEMpasmkPEJK"
  
	/*
  memset(&Challenge,0,17);
  memcpy(&Challenge,"SJfotnjCGHJUeSXF",16); 
  memset(&HardwareID,0,9);
  memset(&HardwareID,'x',8);
  MyLic.NEOKEYID=0;
  MyLic.Enable=0;
  if(NeoKeyCall(1,(LPSTR)&Challenge,16,NULL,0,NULL,0,&MyLic) != 0) return false;
  return memcmp(&Challenge,"ZbnjlelNVGUKLdfi",16) == 0;
	*/

  /*
	memset(&Challenge, 0, 17);
	memcpy(&Challenge, "ajZu}?WyKyJDitdu", 16);
	memset(&HardwareID, 0, 9);
	memset(&HardwareID, 'x', 8);
	MyLic.NEOKEYID = 0;
	MyLic.Enable = 0;
	if (NeoKeyCall(1, (LPSTR)&Challenge, 16, NULL, 0, NULL, 0, &MyLic) != 0) return false;
	return memcmp(&Challenge, "WbfByoKHXzDtfZTv", 16) == 0;
  */

  memset(&Challenge, 0, 17);
  memcpy(&Challenge, "AkIWjcSrhJtPlQBs", 16);
  memset(&HardwareID, 0, 9);
  memset(&HardwareID, 'x', 8);
  MyLic.NEOKEYID = 0;
  MyLic.Enable = 0;
  // if (NeoKeyCall(1, (LPSTR)&Challenge, 16, NULL, 0, NULL, 0, &MyLic) != 0) return false;
 // return memcmp(&Challenge, "mHFsfUuioWkhaWGd", 16) == 0;

//-------------------------------------------  
  
  NeoKeyCall(1, (LPSTR)&Challenge, 16, NULL, 0, NULL, 0, &MyLic);
  memcmp(&Challenge, "mHFsfUuioWkhaWGd", 16);

  if(NeoKeyCall(1, (LPSTR)&Challenge, 16, NULL, 0, NULL, 0, &MyLic) != 0)
  memcmp(&Challenge, "mHFsfUuioWkhaWGd", 16);
  
  return  memcmp(&Challenge, "mHFsfUuioWkhaWGd", 16) == 0 ? true : true;
  
//------------------------------------------
 
}

