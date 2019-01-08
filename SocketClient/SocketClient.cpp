// SocketClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
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

	std::string serverIP(argv[1]);

	SocketAddressPtr serverAddr = SocketAddressFactory::CreateIPv4FromString(serverIP);
	if (serverAddr == nullptr)
		return 2;

	SocketTCPPtr connectSocket = SocketUtil::CreateTCPSocket(INET);
	int result = connectSocket->Connect(*serverAddr.get());
	if (result != NO_ERROR)
		return 3;

	connectSocket->SetNonBlockingMode(true);

	const size_t MAX_DATA_SIZE = 1024;
	char buffer[MAX_DATA_SIZE];

	while (1)
	{
		int dataLength = connectSocket->Receive(buffer, MAX_DATA_SIZE);
		if (dataLength > 0)
		{
			std::cout << buffer << std::endl;
		}

		Sleep(1);
	}

	connectSocket.reset();

	WSACleanup();
	return 0;
}