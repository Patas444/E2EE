#include "Client.h"

Client::Client(const std::string& ip, int port)
	: m_ip(ip), m_port(port), m_serverSock(INVALID_SOCKET) {
	m_crypto.GenerateRSAKey();
	m_crypto.GenerateAESKey();
}

Client::~Client() {
	if (m_serverSock != INVALID_SOCKET) {
		m_net.close(m_serverSock);
	}
}

bool Client::Connect() {
	std::cout << "[Client] Conectado al servidor " << m_ip << ":" << m_port << ".. \n";
	bool connected = m_net.ConnectToServer(m_ip, m_port);
	if (connected) {
		m_serverSock = m_net.m_serverSocket;
		std::cout << "[Client] Conexión establecida. \n";
	}
	else {
		std::cerr << "[Client] Error al conectar. \n";
	}
	return connected;
}

void
Client::ExchangeKeys() {
	std::string serverPubKey = m_net.ReceiveData(m_serverSock);
	m_crypto.LoadPeerPublicKey(serverPubKey);
	std::cout << "[Client] Clave pública del servidor recibida.\n";

	std::string clientPubKey = m_crypto.GetPublicKeyString();
	m_net.SendData(m_serverSock, clientPubKey);
	std::cout << "[Client] Clave pública del cliente enviada.\n";
}

void
Client::SendAESKeyEncrypted() {
	std::vector<unsigned char> encryptedAES = m_crypto.EncryptAESKeyWithPeer();
	m_net.SendData(m_serverSock, encryptedAES);
	std::cout << "[Client] Clave AES cifrada y enviada al servidor.\n";
}

void
Client::SendEncryptedMessage(const std::string& message) {
	std::vector<unsigned char> iv;
	std::vector<unsigned char> encryptedMsg = m_crypto.AESEncrypt(message, iv);

	m_net.SendData(m_serverSock, iv);
	m_net.SendData(m_serverSock, encryptedMsg);
	std::cout << "[Client] Mensaje cifrado enviado.\n";
}