#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <vector>

#define CLOSESOCK closesocket(clientSock); WSACleanup();

//Constants
const char SERVER_IP[] = "127.0.0.1";
const int SERVER_PORT = 54000;
const int BUFFER_SIZE = 1024;

void CheckIP(int& erStat);

void CheckWinSock(int& erStat);

void CheckSocket(SOCKET& sock);

void ServSockAddr(sockaddr_in& serverAddr, in_addr& ip);

void CheckConnection(int& erStat, SOCKET& sock);

void CheckExit(SOCKET& clientSock, std::vector<char>& clientBuff);

void SendAndRecieve(SOCKET& clientSock, int& packetSize,
					std::vector<char>& clientBuff,
					std::vector<char>& servBuff);
