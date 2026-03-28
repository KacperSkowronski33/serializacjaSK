#pragma once
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <vector>
#include <cstddef>
#pragma comment(lib, "ws2_32.lib")

class KlientTCP
{
private:
	const char* m_ip;
	int m_port;
	void sprawdzPort(int port);

	WSADATA m_wsaData;
	SOCKET m_socket;
	sockaddr_in m_serverAddr;
	
	void setWSAStart();
	void setSocket();
	void connectServer();
	void clean();
public:
	void polaczZSerwerem(const char* ip, int port);
	void wyslijDane(const std::vector<std::byte>& dane);
	void odbierzOdpowiedz();

	KlientTCP() {}
	~KlientTCP() { clean(); }
};

