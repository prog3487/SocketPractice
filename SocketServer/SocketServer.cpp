// SocketServer.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "SocketTCP.h"
#include "SocketAddressFactory.h"
#include "SocketUtil.h"

int main(int argc, char *argv[])
{
 	if (argc <= 1)
 		return 1;

	// Initialize Winsock
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return -1;
	}

	std::string myIP(argv[1]);

	SocketAddressPtr socketAddr = SocketAddressFactory::CreateIPv4FromString(myIP);
	if (socketAddr == nullptr)
		return 2;
	
	SocketTCPPtr logSocket = SocketUtil::CreateTCPSocket(INET);
	int result = logSocket->Bind(*socketAddr->SockAddr());
	if (result != NO_ERROR)
		return 3;

	result = logSocket->Listen();
	if (result != NO_ERROR)
		return 4;

	std::cout << "Listening..." << std::endl;

	SocketAddressPtr clientAddr = std::make_shared<SocketAddress>();
	SocketTCPPtr clientSocket = logSocket->Accept(*clientAddr.get());
	if (clientSocket == nullptr)
		return 5;

	std::cout << "Accepted!" << std::endl;

	size_t cnt = 0;
	bool isGameRunning = true;
	while (cnt != SIZE_MAX && isGameRunning)
	{
		std::string data("AI log...");
		data += std::to_string(cnt) + '\0';
		
		clientSocket->Send((void*)data.data(), data.size());

		Sleep(1000);
		++cnt;

		// check for exit
		SHORT keyState = ::GetAsyncKeyState(VK_RETURN);
		if (keyState)
			isGameRunning = false;
	}

	logSocket.reset();
	clientSocket.reset();

	WSACleanup();
	return 0;
}