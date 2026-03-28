#include "KlientTCP.h"
#include <iostream>
#include <string>

void KlientTCP::sprawdzPort(int port)
{
	if (port < 0 || port >= 65535) m_port = 54321;
	else m_port = port;
}

void KlientTCP::setWSAStart()
{
	if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
		std::cerr << "blad wsa";
	} return;
}

void KlientTCP::setSocket()
{
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET) {
		std::cerr << "blad socketu";
		WSACleanup();
		return;
	}
}

void KlientTCP::connectServer()
{
	m_serverAddr.sin_family = AF_INET;
	m_serverAddr.sin_port = htons(m_port);
	if (inet_pton(AF_INET, m_ip, &m_serverAddr.sin_addr) <= 0) {
		std::cerr << "niepoprawny adres ip";
		return;
	}
	if (connect(m_socket, (SOCKADDR*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR) {
		std::cerr << "blad laczenia z serwerem";
		return;
	}
	std::cout << "\npolaczono z serwerem";
}

void KlientTCP::clean()
{
	closesocket(m_socket);
	m_socket = INVALID_SOCKET;
	WSACleanup();
}

void KlientTCP::polaczZSerwerem(const char* ip, int port)
{
	m_ip = ip;
	sprawdzPort(port);
	setWSAStart();
	setSocket();
	connectServer();
}

void KlientTCP::wyslijDane(const std::vector<std::byte>& dane)
{
	if (dane.empty() || m_socket == INVALID_SOCKET) return;
	int iResult = send(m_socket, reinterpret_cast<const char*>(dane.data()), dane.size(), 0);

	if (iResult == SOCKET_ERROR) {
		std::cerr << "blad wysylania danych";
		return;
	}
	std::cout << "wyslano " << iResult << " bajtow\n";
}

void KlientTCP::odbierzOdpowiedz()
{
	char recvbuf[512];
	int iResult = recv(m_socket, recvbuf, 512, 0);
	if (iResult > 0) {
		std::cout << "\nOdpowiedz serwera: " << std::string(recvbuf, iResult) << "\n";
	}
	else if (iResult == 0) {
		std::cout << "\nserwer zamknal polaczenie";
	}
	else {
		std::cerr << "blad odbierania odpowiedzi";
	}
}
