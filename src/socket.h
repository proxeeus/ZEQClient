
#ifndef _ZEQ_SOCKET_H
#define _ZEQ_SOCKET_H

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#else
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <net/if.h>
#define INVALID_SOCKET -1
#endif

#include "types.h"
#include "exception.h"

#define toNetworkLong htonl
#define toNetworkShort htons
#define toHostLong ntohl
#define toHostShort ntohs

class Socket
{
private:
	static const uint32 RECV_BUF_SIZE = 8192;
#ifdef _WIN32
	SOCKET mSocket;
#else
	int mSocket;
#endif
	byte mRecvBuf[RECV_BUF_SIZE];

public:
	static void loadLibrary();
	static void closeLibrary();

public:
	Socket(const char* ip, uint16 port);
	virtual ~Socket();

	byte* getBuffer() { return mRecvBuf; }
	int recvPacket();
	int recvWithTimeout(uint32 milliseconds);
	void sendPacket(void* data, int len);
};

#endif
