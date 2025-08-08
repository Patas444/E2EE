#include "Prerequisites.h"
#include "Server.h"
#include "Client.h"

int main(int argc,char** argv) {
	std::string mode, ip;
	int port = 0;

	if (argc < 2) {
		std::cout << "Modo (Server / Client): ";
		std::cin >> mode;
		if (mode == "server") {
			std::cout << "Puerto: ";
			std::cin >> port;
		}
		else if (mode == "client") {
			std::cout << "IP: ";
			std::cin >> ip;
			std::cout << "Puerto: ";
			std::cin >> port;
		}
		else {
			std::cerr << "Modo no reconocido. \n";
			return 0;
		}
	}
	else {
		mode = argv[1];
		if (mode == "server") {
			port = (argc >= 3) ? std::stoi(argv[2]) : 12345;
		}
		else if (mode == "client") {
			if (argc < 4) {
				std::cerr << "Faltan <ip> <port>.\n"; return 1; }
			ip = argv[2];
			port = std::stoi(argv[3]);
		}
	}

	if (mode == "server") {
		Server s(port);
		if (!s.Start()) return 2;
		s.WaitForClient();
		s.ReceiveEncryptedMessage();
	}
	else {
		Client c(ip, port);
		if (!c.Connect()) return 1;
		c.ExchangeKeys();
		c.SendAESKeyEncrypted();
		c.SendEncryptedMessage("Hola servidor, cifrado AES; ");
	}

	std::cout << "Listo. Presiona [Enter] para salir...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cin.get();
	return 0;
}