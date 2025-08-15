#pragma once
#include "NetworkHelper.h"
#include "CryptoHelper.h"
#include "Prerequisites.h"

class Client {
public:
	Client() = default;
	Client(const std::string& ip, int port);
	~Client();

	/**
	 * @brief Establece conexi�n con el servidor.
	 *
	 * @return true si la conexi�n fue exitosa.
	 * @return false si ocurri� un error.
	 */
	bool 
	Connect();
	
	/**
	 * @brief Intercambia claves p�blicas con el servidor.
	 */
	void 
	ExchangeKeys();

	/**
	 * @brief Cifra la clave AES con la p�blica del servidor y la env�a.
	 */
	void 
	SendAESKeyEncrypted();
	
	/**
	 * @brief Cifra un mensaje con AES y lo env�a al servidor.
	 *
	 * @param message Texto plano a cifrar y enviar.
	 */
	void 
	SendEncryptedMessage(const std::string& message);


	/**
	 * @brief Inicia un bucle que solicita mensajes por consola,
	 *        los cifra y los envía al servidor.
	 *
	 * Este método se usa para enviar mensajes de manera continua
	 * mientras dure la conexión.
	 */
	void
	SendEncryptedMessageLoop();


	/**
	 * @brief Inicia el ciclo principal de chat.
	 *
	 * Combina el envío y recepción de mensajes en paralelo,
	 * permitiendo comunicación en tiempo real con el servidor.
	 */
	void
	StartChatLoop();


	void
	StartReceiveLoop(); // Recibir y mostrar mensajes del servidor

private:
	std::string m_ip; // Dirección IP del servidor.
	int m_port; // Puerto de conexión.
	SOCKET m_serverSock; // Socket para la comunicación con el servidor.
	NetworkHelper m_net; // Ayudante para operaciones de red.
	CryptoHelper m_crypto; // Ayudante para operaciones criptográficas.
};