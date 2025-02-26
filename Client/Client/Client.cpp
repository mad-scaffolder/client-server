#include "Client.h"

void CheckIP(int& erStat) {
	if (erStat <= 0) {
		std::cout << "Error in IP translation to special numeric format" << std::endl;
	}
	else { std::cout << "IP translation to special numeric format is successfull" << std::endl; }
}

void CheckWinSock(int& erStat) {
	if (erStat != 0) {
		std::cout << "Error in WinSock initialization" << std::endl;
	}
	else { std::cout << "WinSock initialization is successfull" << std::endl; }
}

void CheckSocket(SOCKET& clientSock) {
	if (clientSock == INVALID_SOCKET) {
		std::cout << "Error in socket creation" << std::endl;
		CLOSESOCK
	}
	else { std::cout << "Socket creation is successfull" << std::endl; }
}

void ServSockAddr(sockaddr_in& servAddr, in_addr& ip) {
	ZeroMemory(&servAddr, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(SERVER_PORT);
	servAddr.sin_addr = ip;
}

void CheckConnection(int& erStat, SOCKET& clientSock) {
	if (erStat != 0) {
		std::cout << "Error in connection to server" << std::endl;
		CLOSESOCK
	}
	else { std::cout << "Connection to server is successfull" << std::endl; }
}

void CheckExit(SOCKET& clientSock, std::vector<char>& clientBuff) {
	if (clientBuff[0] == 'e' && clientBuff[1] == 'x' &&
		clientBuff[2] == 'i' && clientBuff[3] == 't') {
		shutdown(clientSock, SD_BOTH);
		CLOSESOCK
	}
}

void SendAndRecieve(SOCKET& clientSock, int& packetSize,
					std::vector<char>& clientBuff,
					std::vector<char>& servBuff) {
	while (true) {
		std::cout << "Enter message: ";
		std::cin.getline(clientBuff.data(), BUFFER_SIZE);
		packetSize = send(clientSock, clientBuff.data(), BUFFER_SIZE, 0);
		if (packetSize == SOCKET_ERROR) {
			std::cout << "Error in sending data" << std::endl;
			CLOSESOCK
			break;
		}

		//Check the 'exit' sent from client
		CheckExit(clientSock, clientBuff);

		packetSize = recv(clientSock, servBuff.data(), BUFFER_SIZE, 0);
		if (packetSize == SOCKET_ERROR) {
			std::cout << "Error in receiving data" << std::endl;
			CLOSESOCK
			break;
		}
		std::cout << "Server: " << servBuff.data() << std::endl;
	}
}