#include "Server.h"

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

void CheckSocket(SOCKET& servSock) {
	if (servSock == INVALID_SOCKET) {
		std::cout << "Error in socket creation" << std::endl;
		CLOSESOCK
	}
	else { std::cout << "Socket creation is successfull" << std::endl; }
}

void ServSockAddr(sockaddr_in& servAddr, in_addr& ip) {
	ZeroMemory(&servAddr, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr = ip;
}

void CheckBind(int& erStat, SOCKET& servSock) {
	if (erStat != 0) {
		std::cout << "Error in socket binding" << std::endl;
		CLOSESOCK
	}
	else { std::cout << "Socket binding is successfull" << std::endl; }
}

void CheckListen(int& erStat, SOCKET& servSock) {
	if (erStat != 0) {
		std::cout << "Error in socket listening" << std::endl;
		CLOSESOCK
	}
	else { std::cout << "Listening..." << std::endl; }
}

void ClientSockAddr(SOCKET& clientSock, SOCKET& servSock, sockaddr_in& clientAddr) {
	if (clientSock == INVALID_SOCKET) {
		std::cout << "Client detected but connection failed." << std::endl;
		CLOSESOCK
	}
	else {
		std::cout << "Client socket connection is successfull" << std::endl;

		char clientIP[INET_ADDRSTRLEN];
		//Convert client's IP address to string
		inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
		std::cout << "Client IP: " << clientIP << std::endl;
	}
}

void RecieveAndSend(SOCKET& clientSock,
	SOCKET& servSock, int& packetSize,
	std::vector<char>& clientBuff,
	std::vector<char>& servBuff) {
	while (true) {
		packetSize = recv(clientSock, clientBuff.data(), clientBuff.size(), 0);
		if (packetSize == SOCKET_ERROR) {
			std::cout << "Error in receiving data" << std::endl;
			break;
		}
		else if (packetSize == 0) {
			std::cout << "Client disconnected" << std::endl;
			break;
		}
		else {
			std::cout << "Received data: " << clientBuff.data() << std::endl;
		}

		//Check the 'exit' sent from client
		if (clientBuff[0] == 'e' && clientBuff[1] == 'x' &&
			clientBuff[2] == 'i' && clientBuff[3] == 't') {
			shutdown(clientSock, SD_BOTH);
			CLOSEBOTHSOCKS
		}

		//Send data
		std::string servMsg = "Hello from server!";
		packetSize = send(clientSock, servMsg.c_str(), servMsg.size() + 1, 0);
		if (packetSize == SOCKET_ERROR) {
			std::cout << "Error in sending data" << std::endl;
			CLOSEBOTHSOCKS
		}
		else { std::cout << "Data sent successfully." << std::endl; }
	}
}