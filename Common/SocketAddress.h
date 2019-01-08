#pragma once

#include <WinSock2.h>
#include <memory>

class SocketAddress
{
public:
	//!
	SocketAddress(uint32_t inAddress, uint16_t inPort)
	{
		GetAsSockAddrIn()->sin_family = AF_INET;
		GetAsSockAddrIn()->sin_addr.S_un.S_addr = htonl(inAddress);
		GetAsSockAddrIn()->sin_port = htons(inPort);
	}

	//!
	SocketAddress(const sockaddr& inSockAddr)
	{
		memcpy(&mSockAddr, &inSockAddr, sizeof(sockaddr));
	}

	//!
	SocketAddress()
	{
		memset(&mSockAddr, 0, sizeof(sockaddr));
	}

	//!
	size_t GetSize() const { return sizeof(sockaddr); }

	//!
	sockaddr* SockAddr() { return &mSockAddr; }
	const sockaddr* SockAddr() const { return &mSockAddr; }

private:
	sockaddr mSockAddr;
	
	sockaddr_in* GetAsSockAddrIn()
	{
		return reinterpret_cast<sockaddr_in*>(&mSockAddr);
	}
};

using SocketAddressPtr = std::shared_ptr<SocketAddress>;