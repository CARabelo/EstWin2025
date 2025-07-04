// ConSO.cpp: define as rotinas de inicialização para a DLL.
//

#include "stdafx.h"
#include <winsock.h>
#include "ConSO.h"
#include <string>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: Se esta DLL é conectada dinamicamente contra as MFC DLLs,
//		qualquer função exportada desta DLL que chama a
//		MFC deve possuir o macro AFX_MANAGE_STATE adicionado no
//		começo desta função.
//
//		Por exemplo:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			//corpo normal da função aqui
//		}
//
//		É muito importante que essa macro apareça em todas
//		as funções, antes de qualquer chamada a MFC.  Isto significa que
//		it deve aparecer como a primeira instrução dentro de
//		função, antes mesmo da declaração de qualquer objeto variável
//		uma vez que seus construtores podem gerar chamadas ao MFC
//		DLL.
//
//		Consulte Notas Técnicas MFC 33 e 58 para informações
//		adicionais.
//

// CConSOApp

BEGIN_MESSAGE_MAP(CConSOApp, CWinApp)
END_MESSAGE_MAP()


// construção de CConSOApp

CConSOApp::CConSOApp()
{
	// TODO: adicione código de construção aqui,
	// Coloque todas as inicializações significativas em InitInstance
}


// O único objeto de CConSOApp

CConSOApp theApp;

// inicialização de CConSOApp

BOOL CConSOApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	return TRUE;
}
/*
// CONNECT TO REMOTE HOST(CLIENT APPLICATION)
//Include the needed header files.
//Don't forget to link libws2_32.a to your program as well
#include <winsock.h>

//SOCKET Socket; //Socket handle

bool ConnectToHost(SOCKET Socket,int PortNo, char* IPAddress)
{
  //Start up Winsock…
  WSADATA wsadata;

  int error = WSAStartup(0x0202, &wsadata);

  //Did something happen?
  if (error)
    return false;

  //Did we get the right Winsock version?
  if (wsadata.wVersion != 0x0202)
  {
    WSACleanup(); //Clean up Winsock
    return false;
  }

  //Fill out the information needed to initialize a socket…
  SOCKADDR_IN target;               //Socket address information

  target.sin_family = AF_INET;      // address family Internet
  target.sin_port = htons(PortNo); //Port to connect on
  target.sin_addr.s_addr = inet_addr(IPAddress); //Target IP

  Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
  if (Socket == INVALID_SOCKET)
  {
    return false; //Couldn't create the socket
  }

  //Try connecting...

  if (connect(Socket, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
  {
    return false;                 //Couldn't connect
  }
  else
    return true;                  //Success
}

//CLOSECONNECTION – shuts down the socket and closes any connection on it
void CloseConnection(SOCKET Socket)
{
  //Close the socket if it exists
  if (Socket)
    closesocket(Socket);

  WSACleanup();                     //Clean up Winsock
}

int SendMessage(SOCKET Socket, std::string& Buffer)
{
  send(Socket, Buffer.c_str(), (int)Buffer.length(), 0);
}

int RecvMessage(SOCKET Socket, std::string& Buffer)
{
  char* chBuffer[512] = { 0 };

  Buffer.reserve(512);

  int Tam(recv(Socket, chBuffer[0], 511, 0));

  chBuffer[Tam] = 0;

  Buffer = chBuffer[0];
}
*/
/*
int WSAAPI send(
  SOCKET     s,
  const char *buf,
  int        len,
  int        flags
);
*/