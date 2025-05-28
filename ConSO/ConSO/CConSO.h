#pragma once
class CConSO
{
public:
  CConSO();
  ~CConSO();

  static __declspec(dllexport) bool ConnectaSoket(SOCKET Socket, int PortNo, char* IPAddress) ;
  static __declspec(dllexport) int EnviaMensagem(SOCKET Socket, std::string& Buffer);
  static __declspec(dllexport) int RecebeMensagem(SOCKET Socket, std::string& Buffer);
  static __declspec(dllexport) void FechaConexao(SOCKET Socket);

protected:

};

