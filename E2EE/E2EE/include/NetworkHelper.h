#pragma once
#include "Prerequisites.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")


class
	NetworkHelper {
public:

	NetworkHelper();
	~NetworkHelper();

	// Modo Server
	bool
	StartServer(int port);

	SOCKET
	AcceptClient();

	// Modo Cliente
	bool
	ConnectToServer(const std::string& ip, int port);

	// Enviar y recibir datos
	bool
	SendData(SOCKET socket, const std::string& data);

	bool
	SendData(SOCKET socket, const std::vector<unsigned char>& data);

	std::string
	ReceiveData(SOCKET socket);

	std::vector <unsigned char>
	ReceiveDataBinary(SOCKET socket, int size = 0);

	void
	close(SOCKET socket);

	bool
	SendAll(SOCKET s, const unsigned char* data, int len);

	bool
	ReceiveExact(SOCKET s, unsigned char* out, int len);

public:
	SOCKET m_serverSocket = -1;
	

private:
	bool m_initialized = 0;
};