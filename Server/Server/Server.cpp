#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdio.h>
#include <vector>
#include <Windows.h>

#pragma comment(lib, "Ws2_32.lib")



int main()
{
	//Constants
	const char IP_SERV[] = "127.0.0.1";
	const int PORT = 54000;
	const short BUFF_SIZE = 1024;

	//Keep error code in erStat
	//Successfull translation is kept in ipToNum
	int erStat;
	in_addr ipToNum;

	erStat = inet_pton(AF_INET, IP_SERV, &ipToNum);
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
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET) {
		std::cout << "Error in socket creation" << std::endl;
		closesocket(servSock);
		WSACleanup();
		return 1;
	}
	else { std::cout << "Socket creation is successfull" << std::endl; }

	//Bind socket
	sockaddr_in servAddr;
	//Fills a block of memory with zeros.
	ZeroMemory(&servAddr, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(PORT);
	servAddr.sin_addr = ipToNum;

	erStat = bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr));
	if (erStat != 0) {
		std::cout << "Error in socket binding" << std::endl;
		closesocket(servSock);
		WSACleanup();
		return 1;
	}
	else { std::cout << "Socket binding is successfull" << std::endl; }

	//Listen
	erStat = listen(servSock, SOMAXCONN);
	if (erStat != 0) {
		std::cout << "Error in socket listening" << std::endl;
		closesocket(servSock);
		WSACleanup();
		return 1;
	}
	else { std::cout << "Listening..." << std::endl; }

	//Client socket connection
	sockaddr_in clientAddr;
	ZeroMemory(&clientAddr, sizeof(clientAddr));
	int clientAddrSize = sizeof(clientAddr);

	SOCKET clientSock = accept(servSock, (sockaddr*)&clientAddr, &clientAddrSize);
	if (clientSock == INVALID_SOCKET) {
		std::cout << "Client detected but connection failed." << std::endl;
		closesocket(servSock);
		WSACleanup();
		return 1;
	}
	else {
		std::cout << "Client socket connection is successfull" << std::endl;

		char clientIP[INET_ADDRSTRLEN];
		//Convert client's IP address to string
		inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
		std::cout << "Client IP: " << clientIP << std::endl;
	}

	//Receive data
	std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
	int packetSize = 0;
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

		//Send data
		std::string servMsg = "Hello from server!";
		packetSize = send(clientSock, servMsg.c_str(), servMsg.size() + 1, 0);
		if (packetSize == SOCKET_ERROR) {
			std::cout << "Error in sending data" << std::endl;
			closesocket(clientSock);
			closesocket(servSock);
			WSACleanup();
			return 1;
		}
		else { std::cout << "Data sent successfully." << std::endl; }
	}

	

	//Close socket
	closesocket(clientSock);
	closesocket(servSock);
	WSACleanup();
	return 0;
}