#include "stdafx.h"
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <string>
#include "CConSO.h"

CConSO::CConSO()
{
}

CConSO::~CConSO()
{
}

bool CConSO::ConnectaSoket(SOCKET Socket, int PortNo, char* IPAddress)
{
  WSADATA wsadata;

  int error = WSAStartup(0x0202, &wsadata);

  if (error) return false;

  if (wsadata.wVersion != 0x0202)
  {
    WSACleanup(); 
    return false;
  }

  SOCKADDR_IN target;             

  target.sin_family = AF_INET;     
  target.sin_port = htons(PortNo); 
  target.sin_addr.s_addr = inet_addr(IPAddress); 

  Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); 
  if (Socket == INVALID_SOCKET) return false; 

  return (connect(Socket, (SOCKADDR *)&target, sizeof(target)) != SOCKET_ERROR);        
}

int CConSO::EnviaMensagem(SOCKET Socket, std::string& Buffer)
{
  return send(Socket, Buffer.c_str(), (int) Buffer.length(), 0);
}

int CConSO::RecebeMensagem(SOCKET Socket, std::string& Buffer)
{
  char* chBuffer[512] = { 0 };

  int Tam(recv(Socket, chBuffer[0], 511, 0));

  chBuffer[Tam] = 0;

  Buffer = chBuffer[0];

  return Tam;
}

void CConSO::FechaConexao(SOCKET Socket)
{
  if (Socket) closesocket(Socket);

  WSACleanup();      
}
