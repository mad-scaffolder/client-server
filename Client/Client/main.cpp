#include "Client.h"

#pragma comment(lib, "Ws2_32.lib")

int main()
{
	//Keep error code in erStat
	//Successfull translation is kept in ipToNum
	int erStat;
	in_addr ipToNum;
	erStat = inet_pton(AF_INET, SERVER_IP, &ipToNum);
	CheckIP(erStat);
	
	//WinSock initialization
	WSADATA wsData;
	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
	CheckWinSock(erStat);
	
	//Create socket
	SOCKET clientSock = socket(AF_INET, SOCK_STREAM, 0);
	CheckSocket(clientSock);

	//Connect to server
	sockaddr_in serverAddr;
	ServSockAddr(serverAddr, ipToNum);
	erStat = connect(clientSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	CheckConnection(erStat, clientSock);

	//Send and receive data
	std::vector<char> servBuff(BUFFER_SIZE), clientBuff(BUFFER_SIZE);
	int packetSize = 0;
	SendAndRecieve(clientSock, packetSize, clientBuff, servBuff);

	//Close socket
	CLOSESOCK
	return 0;
}