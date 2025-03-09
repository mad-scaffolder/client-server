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

void ClientSockAddr(SOCKETLIST, sockaddr_in& clientAddr) {
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

//Checks if data recieved is an array or a text message
int DataType(std::vector<char>& clientBuff) {
	int result = 0;
	for (char buff : clientBuff) {
		if (buff == '\0') { break; }
		if (buff == ' ') continue;
		if (!isdigit(buff)) {
			result = 1;
		}
	}
	return result;
}

//Send message to client
void SendData(SOCKETLIST, int& packetSize, std::vector<char>& servBuff) {
	packetSize = send(clientSock, servBuff.data(), servBuff.size(), 0);
	if (packetSize == SOCKET_ERROR) {
		std::cout << "Error in sending data" << std::endl;
		CLOSEBOTHSOCKS
	}
	else { std::cout << "Data sent successfully." << std::endl; }
}

//Check if Client wants to exit
bool CheckExit(std::vector<char>& clientBuff) {
	if (clientBuff[0] == 'e' && clientBuff[1] == 'x' &&
		clientBuff[2] == 'i' && clientBuff[3] == 't') {
		return true;
	}
	else { return false; }
}

//Check if Server must shut down
bool CheckEnd(std::vector<char>& clientBuff) {
	if (clientBuff[0] == 'e' && clientBuff[1] == 'n' &&
		clientBuff[2] == 'd') {
		return true;
	}
	else { return false; }
}

//Prepare message to send
void MsgToSend(std::vector<char>& servBuff, std::string msg) {
	servBuff.clear();
	for (char m : msg) { servBuff.push_back(m); }
	servBuff.push_back('\n');
}

//Check if character is a vowel
bool IsVowel(char& c) {
	char vowels[] = { 'a', 'e', 'i', 'o', 'u', 'A', 'E', 'I', 'O', 'U' };
	bool result = false;
	for (char vowel : vowels) {
		if (vowel == c) { result = true; break; }
	}
	return result;
}

std::string CountVowels(std::vector<char>& clientBuff) {
	int counter = 0;
	std::string result = "Number of vowels in message is ";
	for (char buff : clientBuff) {
		if (buff == '\0') { break; }
		if (IsVowel(buff)) { counter++; }
	}
	result.append(std::to_string(counter));
	result.append(".\n");
	return result;
}

bool Compare(int a, int b) {
	return a > b;
}

std::string SortArray(std::vector<char>& clientBuff) {
	std::vector<int> arr;
	std::string result = "Sorted array: ";
	std::string temp;

	//Make array of integers from clientBuff
	for (char buff : clientBuff) {
		if (buff == '\0') {
			if (!temp.empty()) {
				arr.push_back(stoi(temp));
			}
			break;
		}
		if (isdigit(buff)) {
			temp.insert(temp.end(), 1, buff);
		}
		if (buff == ' ') {
			if (!temp.empty()) {
				arr.push_back(stoi(temp));
				temp.clear();
			}
			else { continue; }
		}
		else { continue; }
	}

	//Sort and return string value
	std::sort(arr.begin(), arr.end(), Compare);
	for (int a : arr) {
		result.append(std::to_string(a));
		result.append(" ");
	}
	result.append("\n");
	return result;
}

void RecieveAndSend(SOCKETLIST, bool& endFlag) {
	std::vector <char> servBuff(BUFF_SIZE), clientBuff(BUFF_SIZE);
	int packetSize = 0;
	SOCKET trueClientSock = clientSock;
	while (true) {	
		packetSize = recv(trueClientSock, clientBuff.data(), clientBuff.size(), 0);

		if (packetSize == SOCKET_ERROR) {
			std::cout << "Error in receiving data" << std::endl;
			break;
		}
		else if (packetSize == 0) {
			std::cout << "Client " << trueClientSock << " disconnected." << std::endl;
			break;
		}
		else {
			std::cout << "Received data: " << clientBuff.data() << std::endl;
		}

		int dataType = DataType(clientBuff);

		if (dataType == 1) {
			if (CheckExit(clientBuff)) {
				std::cout << "Exit accepted." << std::endl;
				shutdown(trueClientSock, SD_BOTH);
				closesocket(trueClientSock);
				std::cout << "Client " << trueClientSock << " disconnected." << std::endl;
				break;
			}
			if (CheckEnd(clientBuff)) {
				std::cout << "End accepted." << std::endl;
				endFlag = true;
				shutdown(trueClientSock, SD_BOTH);
				closesocket(trueClientSock);
				CLOSESOCK
				break;
			}
			MsgToSend(servBuff, CountVowels(clientBuff));
			SendData(trueClientSock, servSock, packetSize, servBuff);
		}
		else {
			MsgToSend(servBuff, SortArray(clientBuff));
			SendData(trueClientSock, servSock, packetSize, servBuff);
		}
		std::this_thread::yield();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}