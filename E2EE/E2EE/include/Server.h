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
	bool Start();

	/**
	 * @brief Espera a que un cliente se conecte e intercambia claves públicas.
	 */
	void WaitForClient();

	/**
	 * @brief Recibe un mensaje cifrado del cliente, lo descifra y lo imprime.
	 */
	void ReceiveEncryptedMessage();

private:
	int m_port;                // Puerto del servidor
	int m_clientSocket;         // Socket del cliente
	NetworkHelper m_net;
	CryptoHelper m_crypto;
};