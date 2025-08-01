#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"

class
Server {
public:

	Server() = default;
	Server(int port);

	~Server();

	bool Start();
	void WaitForClient();
	void ReceiveEncryptedMessage();

private:
	int m_port;                // Puerto del servidor
	int m_clientSocket;         // Socket del cliente
	NetworkHelper m_net;
	CryptoHelper m_crypto;
};