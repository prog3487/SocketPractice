#pragma once

#include <memory>
#include "SocketAddress.h"
#include "SocketError.h"

class SocketTCP;
using SocketTCPPtr = std::shared_ptr<SocketTCP>;

class SocketTCP
{
public:

	~SocketTCP();
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inToAddress);
	int Listen(int inBackLog = SOMAXCONN);
	SocketTCPPtr Accept(SocketAddress& inFromAddress);
	int Send(const void* inData, int inLen);
	int Receive(void* inBuffer, int inLen);
	int SetNonBlockingMode(bool NonBlocking);


private:

	friend class SocketUtil;
	SocketTCP(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};


//!
SocketTCP::~SocketTCP()
{
	int result = closesocket(mSocket);
	if (result != NO_ERROR)
		SocketError::ReportError(L"SocketTCP::closesocket");
}

//!
int SocketTCP::Connect(const SocketAddress& inAddress)
{
	int err = connect(mSocket, inAddress.SockAddr(), inAddress.GetSize());
	if (err >= 0)
	{
		return NO_ERROR;
	}

	SocketError::ReportError(L"SocketTCP::Connect");
	return -SocketError::GetLastError();
}

//!
int SocketTCP::Bind(const SocketAddress& inBindAddress)
{
	int err = bind(mSocket, inBindAddress.SockAddr(), inBindAddress.GetSize());
	if (err == 0)
	{
		return NO_ERROR;
	}

	SocketError::ReportError(L"SocketTCP::Bind");
	return -SocketError::GetLastError();
}

//!
int SocketTCP::Listen(int inBackLog)
{
	int err = listen(mSocket, inBackLog);
	if (err >= 0)
	{
		return NO_ERROR;
	}

	SocketError::ReportError(L"SocketTCP::Listen");
	return -SocketError::GetLastError();
}

//!
SocketTCPPtr SocketTCP::Accept(SocketAddress& inFromAddress)
{
	int length = inFromAddress.GetSize();
	SOCKET newSocket = accept(mSocket, inFromAddress.SockAddr(), &length);

	if (newSocket != INVALID_SOCKET)
	{
		return SocketTCPPtr(new SocketTCP(newSocket));
	}

	SocketError::ReportError(L"SocketTCP::Accept");
	return nullptr;
}

//!
int SocketTCP::Send(const void* inData, int inLen)
{
	int bytesSentCount = send(mSocket, static_cast<const char*>(inData), inLen, 0);

	if (bytesSentCount >= 0)
	{
		return bytesSentCount;
	}

	SocketError::ReportError(L"SocketTCP::Send");
	return -SocketError::GetLastError();
}

//!
int SocketTCP::Receive(void* inBuffer, int inLen)
{
	int bytesReceivedCount = recv(mSocket, static_cast<char*>(inBuffer), inLen, 0);
	return bytesReceivedCount;
}

//!
int SocketTCP::SetNonBlockingMode(bool NonBlocking)
{
	u_long arg = NonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);

	if (result != NO_ERROR)
	{
		SocketError::ReportError(L"SocketTCP::SetNonBlockingMode");
		return -SocketError::GetLastError();
	}
	
	return NO_ERROR;
}
