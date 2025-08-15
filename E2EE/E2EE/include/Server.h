#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"

class
Server {
public:

	Server() = default;
	Server(int port);

	~Server();

	/**
	 * @brief Inicia el servidor en el puerto especificado
	 *
	 * @return true si el servidor se inicializó correctamente
	 * @return false si hubo un error
	 */
	bool 
	Start();

	/**
	 * @brief Espera a que un cliente se conecte e intercambia claves públicas.
	 */
	void 
	WaitForClient();

	/**
	 * @brief Recibe un mensaje cifrado del cliente, lo descifra y lo imprime.
	 */
	void 
	ReceiveEncryptedMessage();


	/**
	 * @brief Inicia un bucle continuo para recibir mensajes cifrados del cliente.	
	 */
	void
	StartReceiveLoop();


	/**
	 * @brief Inicia un bucle para leer mensajes por consola, cifrarlos y enviarlos al cliente.
	 */
	void
	SendEncryptedMessageLoop();


	/**
	 * @brief Inicia el ciclo de chat bidireccional.
	 *
	 * Combina el envío y la recepción de mensajes en paralelo, 
	 * permitiendo comunicación en tiempo real con el cliente.
	 */
	void
	StartChatLoop();

private:
	int m_port;                // Puerto del servidor
	int m_clientSocket;         // Socket del cliente
	SOCKET m_clientSock;
	NetworkHelper m_net;
	CryptoHelper m_crypto;
	std::thread m_rxThread;
	std::atomic<bool> m_running{ false };
};