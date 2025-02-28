#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <inaddr.h>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

#define CLOSESOCK closesocket(servSock); WSACleanup();
#define CLOSEBOTHSOCKS closesocket(clientSock); closesocket(servSock); WSACleanup();
#define SOCKETLIST SOCKET& clientSock, SOCKET& servSock

//Constants
const char IP_SERV[] = "127.0.0.1";
const int PORT = 54000;
const short BUFF_SIZE = 1024;

void CheckIP(int& erStat);

void CheckWinSock(int& erStat);

void CheckSocket(SOCKET& sock);

void ServSockAddr(sockaddr_in& serverAddr, in_addr& ip);

void CheckBind(int& erStat, SOCKET& servSock);

void CheckListen(int& erStat, SOCKET& servSock);

void ClientSockAddr(SOCKETLIST, sockaddr_in& clientAddr);

bool RecieveAndSend(SOCKETLIST, int& packetSize, std::vector<char>& clientBuff, std::vector<char>& servBuff);