#pragma once
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

class SerwerTCP
{
private:
	int m_port;
	void sprawdzPort(int port);

	WSADATA m_wsaData;
	SOCKET m_listenSocket;
	sockaddr_in m_serverAddr;
	SOCKET m_clientSocket;

	void setWSAStart();
	void setListenSocket();
	void setServerStart();
	void setClientSocket();
	void clean();
public:
	void uruchomSerwerTCP(int port);
	SerwerTCP() {}
	~SerwerTCP() { clean(); }
};

