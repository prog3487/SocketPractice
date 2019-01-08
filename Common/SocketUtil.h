#pragma once

#include "WinSock2.h"
#include "Ws2tcpip.h"
#include "SocketError.h"
#include "SocketTCP.h"

enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};

//---------------------------------------------------------
class SocketUtil
{
public:
	static SocketTCPPtr CreateTCPSocket(SocketAddressFamily inFamily);
};

//!
SocketTCPPtr SocketUtil::CreateTCPSocket(SocketAddressFamily inFamily)
{
	SOCKET s = socket(inFamily, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		SocketError::ReportError(L"SocketUtil::CreateTCPSocket::socket");
		return nullptr;
	}

	return SocketTCPPtr(new SocketTCP(s));
}
