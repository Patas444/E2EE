#include "Server.h"

Server::Server(int port) : m_port(port),  m_clientSocket(-1) {
	// Socket del cliente(-1) {
	// Generar claves RSA al construir
	m_crypto.GenerateRSAKey();
}

Server::~Server() {
	// Cerrar conexi�n con el cliente si a�n est� activa
	if (m_clientSocket != -1) {
		m_net.close(m_clientSocket);
	}
}


bool Server::Start() {
	std::cout << "[Server] Iniciando servidor en el puerto " << m_port << "...\n";
	return m_net.StartServer(m_port);
}


void Server::WaitForClient() {
	std::cout << "[Server] Esperando conexi�n de un cliente...\n";

	// Aceptar conexi�n entrante
	m_clientSocket = m_net.AcceptClient();
	if (m_clientSocket == INVALID_SOCKET) {
		std::cerr << "[Server] No se pudo aceptar cliente.\n";
		return;
	}
	std::cout << "[Server] Cliente conectado.\n";

	// 1. Enviar clave p�blica del servidor al cliente
	std::string serverPubKey = m_crypto.GetPublicKeyString();
	m_net.SendData(m_clientSocket, serverPubKey);

	// 2. Recibir clave p�blica del cliente
	std::string clientPubKey = m_net.ReceiveData(m_clientSocket);
	m_crypto.LoadPeerPublicKey(clientPubKey);

	// 3. Recibir clave AES cifrada con la p�blica del servidor
	std::vector<unsigned char> encryptedAESKey = m_net.ReceiveDataBinary(m_clientSocket, 256);
	m_crypto.DecryptAESKey(encryptedAESKey);

	std::cout << "[Server] Clave AES intercambiada exitosamente.\n";
}


void Server::ReceiveEncryptedMessage() {
	// 1. Recibir el IV (vector de inicializaci�n)
	std::vector<unsigned char> iv = m_net.ReceiveDataBinary(m_clientSocket, 16);

	// 2. Recibir el mensaje cifrado
	std::vector<unsigned char> encryptedMsg = m_net.ReceiveDataBinary(m_clientSocket, 128);

	// 3. Descifrar el mensaje
	std::string msg = m_crypto.AESDecrypt(encryptedMsg, iv);

	// 4. Mostrar mensaje
	std::cout << "[Server] Mensaje recibido: " << msg << "\n";
}