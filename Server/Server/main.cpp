#include "Server.h"




int main()
{
	//Keep error code in erStat
	//Successfull translation is kept in ipToNum
	int erStat;
	in_addr ipToNum;
	erStat = inet_pton(AF_INET, IP_SERV, &ipToNum);
	CheckIP(erStat);

	//WinSock initialization
	WSADATA wsData;
	erStat = WSAStartup(MAKEWORD(2, 2), &wsData);
	CheckWinSock(erStat);

	//Create socket
	SOCKET servSock = socket(AF_INET, SOCK_STREAM, 0);
	CheckSocket(servSock);

	//Bind socket
	sockaddr_in servAddr;
	ServSockAddr(servAddr, ipToNum);
	erStat = bind(servSock, (sockaddr*)&servAddr, sizeof(servAddr));
	CheckBind(erStat, servSock);

	//Initialize shutdown flag
	bool endFlag = false;
	//Store Client sockets
	std::vector<SOCKET> clientSocks;
	std::mutex mute;
	while (!endFlag) {
		
		//Listen
		erStat = listen(servSock, SOMAXCONN);
		CheckListen(erStat, servSock);
		//Client socket connection
		sockaddr_in clientAddr;
		ZeroMemory(&clientAddr, sizeof(clientAddr));
		int clientAddrSize = sizeof(clientAddr);
		clientSocks.push_back(accept(servSock, (sockaddr*)&clientAddr, &clientAddrSize));
		SOCKET clientSock = clientSocks.back();
		ClientSockAddr(clientSock, servSock, clientAddr);

		//Receive and send data in detached thread
		std::thread thread = std::thread(RecieveAndSend, std::ref(clientSock), std::ref(servSock), std::ref(endFlag));
		thread.detach();
	}
	std::cout << "Server shut down." << std::endl;
	//CLOSESOCK
		return 0;
}