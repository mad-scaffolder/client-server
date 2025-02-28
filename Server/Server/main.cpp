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

	bool endFlag = false;

	while (true) {
		//Listen
		erStat = listen(servSock, SOMAXCONN);
		CheckListen(erStat, servSock);
		//Client socket connection
		sockaddr_in clientAddr;
		ZeroMemory(&clientAddr, sizeof(clientAddr));
		int clientAddrSize = sizeof(clientAddr);
		SOCKET clientSock = accept(servSock, (sockaddr*)&clientAddr, &clientAddrSize);
		ClientSockAddr(clientSock, servSock, clientAddr);
		//Receive and send data
		std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
		int packetSize = 0;
		std::thread thread = std::thread(RecieveAndSend, std::ref(clientSock), std::ref(servSock), 
			std::ref(packetSize), std::ref(clientBuff), std::ref(servBuff), std::ref(endFlag));
		thread.join();
		if (endFlag == true) {
			break;
		}
	}
	std::cout << "Server shut down." << std::endl;
	CLOSESOCK
	return 0;
}