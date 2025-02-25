#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <stdio.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    //Constants
	const char SERVER_IP[] = "127.0.0.1";
	const int SERVER_PORT = 54000;
	const int BUFFER_SIZE = 1024;

	//Keep error code in erStat
	//Successfull translation is kept in ipToNum
	int erStat;
	in_addr ipToNum;

	erStat = inet_pton(AF_INET, SERVER_IP, &ipToNum);
	if (erStat <= 0) {
		std::cout << "Error in IP translation to special numeric format" << std::endl;
		return 1;
	}
	else { std::cout << "IP translation to special numeric format is successfull" << std::endl; }
	
	//WinSock initialization
	WSADATA wsData;
	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
	if (erStat != 0) {
		std::cout << "Error in WinSock initialization" << std::endl;
		return 1;
	}
	else { std::cout << "WinSock initialization is successfull" << std::endl; }
	
	//Create socket
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Error in socket creation" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	else { std::cout << "Socket creation is successfull" << std::endl; }

	//Connect to server
	sockaddr_in serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVER_PORT);
	serverAddr.sin_addr = ipToNum;

	erStat = connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (erStat != 0) {
		std::cout << "Error in connection to server" << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}
	else { std::cout << "Connection to server is successfull" << std::endl; }

	//Send and receive data
	std::vector<char> servBuff(BUFFER_SIZE), clientBuff(BUFFER_SIZE);
	int packetSize = 0;
	while (true) {
		std::cout << "Enter message: ";
		std::cin.getline(clientBuff.data(), BUFFER_SIZE);
		packetSize = send(clientSocket, clientBuff.data(), BUFFER_SIZE, 0);
		if (packetSize == SOCKET_ERROR) {
			std::cout << "Error in sending data" << std::endl;
			closesocket(clientSocket);
			WSACleanup();
			break;
		}
		packetSize = recv(clientSocket, servBuff.data(), BUFFER_SIZE, 0);
		if (packetSize == SOCKET_ERROR) {
			std::cout << "Error in receiving data" << std::endl;
			closesocket(clientSocket);
			WSACleanup();
			break;
		}
		std::cout << "Server: " << servBuff.data() << std::endl;
	}
}