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

	/**
	 * @brief Inicia un socket servidor en el puerto indicado y lo deja en modo escucha.
	 *
	 * @param port Puerto TCP que se usar� para escuchar conexiones entrantes.
	 * @return true Si el servidor se inicia correctamente.
	 * @return false Si ocurre un error en cualquier paso.
	 */
	bool
	StartServer(int port);


	/**
	 * @brief Espera y acepta un cliente entrante.
	 *
	 * @return SOCKET Socket del cliente aceptado, o INVALID_SOCKET si falla.
	 */
	SOCKET
	AcceptClient();


	// Modo cliente

	/**
	 * @brief Conecta al servidor especificado por IP y puerto.
	 *
	 * @param ip Direcci�n IP del servidor.
	 * @param port Puerto del servidor.
	 * @return true Si la conexi�n fue exitosa.
	 * @return false Si fall� la conexi�n.
	 */
	bool
	ConnectToServer(const std::string& ip, int port);

	// Enviar y recibir datos

	/**
	 * @brief Env�a una cadena de texto por el socket.
	 */
	bool
	SendData(SOCKET socket, const std::string& data);


	/**
	 * @brief Env�a datos binarios (ej. AES, RSA) por el socket.
	 */
	bool
	SendData(SOCKET socket, const std::vector<unsigned char>& data);

	/**
	 * @brief Recibe una cadena de texto del socket.
	 */
	std::string
	ReceiveData(SOCKET socket);


	/**
	 * @brief Recibe datos binarios desde el socket.
	 */
	std::vector <unsigned char>
	ReceiveDataBinary(SOCKET socket, int size = 0);

	/**
	 * @brief Cierra un socket.
	 */
	void
	close(SOCKET socket);


	/**
	 * @brief Env�a todos los bytes especificados, incluso si el env�o parcial ocurre en varias llamadas.
	 */
	bool
	SendAll(SOCKET s, const unsigned char* data, int len);

	/**
	 * @brief Recibe exactamente la cantidad de bytes especificada.
	 */
	bool
	ReceiveExact(SOCKET s, unsigned char* out, int len);

public:
	SOCKET m_serverSocket = -1;  // Socket principal del servidor.
	

private:
	bool m_initialized = 0; // Indica si Winsock ha sido inicializado.
};