#include "SerwerTCP.h"
#include <iostream>
#include "serialDeserial.h"

void SerwerTCP::sprawdzPort(int port)
{
    if (port < 0 || port >= 65535) m_port = 54321;
    else m_port = port;
}

void SerwerTCP::setWSAStart()
{
    if (WSAStartup(MAKEWORD(2, 2), &m_wsaData) != 0) {
        std::cerr << "blad wsastartup\n";
        return;
    }
}

void SerwerTCP::setListenSocket()
{
    m_listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_listenSocket == INVALID_SOCKET) {
        std::cerr << "blad tworzenia socketu";
        WSACleanup();
        return;
    }
}

void SerwerTCP::setServerStart()
{
    m_serverAddr.sin_family = AF_INET;
    m_serverAddr.sin_addr.s_addr = INADDR_ANY;
    m_serverAddr.sin_port = htons(m_port);

    if (bind(m_listenSocket, (SOCKADDR*)&m_serverAddr, sizeof(m_serverAddr)) == SOCKET_ERROR) {
        std::cerr << "blad bind";
        closesocket(m_listenSocket);
        WSACleanup();
        return;
    }

    listen(m_listenSocket, SOMAXCONN);
    //std::cout << "wlacz hercules\n";
}

void SerwerTCP::setClientSocket()
{
    m_clientSocket = accept(m_listenSocket, NULL, NULL);
    if (m_clientSocket == INVALID_SOCKET) {
        std::cerr << "blad accept";
        closesocket(m_listenSocket);
        WSACleanup();
        return;
    }
    std::cout << "polaczono\n";
}

void SerwerTCP::clean()
{
    closesocket(m_clientSocket);
    closesocket(m_listenSocket);
    WSACleanup();
}

void SerwerTCP::uruchomSerwerTCP(int port)
{
    sprawdzPort(port);
    setWSAStart();
    setListenSocket();
    setServerStart();
    setClientSocket();

    serialDeserial dane;

    char recvbuf[512];
    int recvbuflen = 512;
    int iResult;
    do {
        iResult = recv(m_clientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            std::cout << "odebrano " << iResult << " bajtow\n";
            std::vector<std::byte> odebraneDane(iResult);
            std::memcpy(odebraneDane.data(), recvbuf, iResult);
            dane.sprawdzTypRamki(odebraneDane);

            PID nowyPID = dane.gotowyPID();
            ARX nowyARX = dane.gotowyARX();

            std::cout << "odebrany arx: k = " << nowyARX.getK() << ", uMin = " << nowyARX.getUMin() << ", uMax = " << nowyARX.getUMax() << ", szum = " << nowyARX.getSzum() << "\n";
            std::vector<double> wektorA = nowyARX.getA();
            std::cout << "Wektor A (" << wektorA.size() << " elementow): ";
            for (double wartosc : wektorA) {
                std::cout << wartosc << "  ";
            }
            std::cout << "\n\n";
            std::vector<double> wektorB = nowyARX.getB();
            std::cout << "Wektor B (" << wektorB.size() << " elementow): ";
            for (double wartosc : wektorB) {
                std::cout << wartosc << "  ";
            }
            std::cout << "\n\n";

            std::cout << "odebrany pid: k = " << nowyPID.getNastawaK() << ", ti = " << nowyPID.getNastawaTI() << ", tD = " << nowyPID.getNastawaTD() << "\n";
            const char* odp = "odebrano";
            send(m_clientSocket, odp, strlen(odp), 0);
        }
        else if (iResult == 0) {
            std::cout << "polaczenie zamkniete";
        }
        else {
            std::cerr << "blad " << WSAGetLastError() << "\n";
        }
    } while (iResult > 0);
}

