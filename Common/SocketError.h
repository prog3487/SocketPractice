#pragma once

#include <stdio.h>
#include "Windows.h"

void Log(const WCHAR* inFormat, ...)
{
	//not thread safe...
	static WCHAR temp[4096];

	va_list args;
	va_start(args, inFormat);

#if _WIN32
	_vsnwprintf_s(temp, 4096, 4096, inFormat, args);
#else
	vsnprintf(temp, 4096, inFormat, args);
#endif

	OutputDebugString(temp);
	OutputDebugString(L"\n");
}

#define LOG( ... ) Log( __VA_ARGS__ );

class SocketError
{
public:
	static void			ReportError(const WCHAR* inOperationDesc);
	static int			GetLastError();
};

//!
void SocketError::ReportError(const WCHAR* inOperationDesc)
{
#if _WIN32
	LPVOID lpMsgBuf;
	DWORD errorNum = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		errorNum,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);


	LOG(L"Error %s: %d- %s", inOperationDesc, errorNum, lpMsgBuf);
#else
	LOG(L"Error: %hs", inOperationDesc);
#endif
}

//!
int SocketError::GetLastError()
{
#if _WIN32
	return WSAGetLastError();
#else
	return errno;
#endif

}
