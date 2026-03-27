#include <iostream>
#include <WinSock2.h>
#include "serialDeserial.h"
#pragma comment(lib, "ws2_32.lib")
#define PORT 54321

void main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "blad wsastartup\n";
        return;
    }
    SOCKET ListenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (ListenSocket == INVALID_SOCKET) {
        std::cerr << "blad tworzenia socketu";
        WSACleanup();
        return;
    }
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    if (bind(ListenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "blad bind";
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    listen(ListenSocket, SOMAXCONN);
    std::cout << "wlacz hercules\n";

    SOCKET ClientSocket = accept(ListenSocket, NULL, NULL);
    if (ClientSocket == INVALID_SOCKET) {
        std::cerr << "blad accept";
        closesocket(ListenSocket);
        WSACleanup();
        return;
    }

    std::cout << "polaczono\n";

    serialDeserial dane;

    char recvbuf[512];
    int recvbuflen = 512;
    int iResult;
    do {
        iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0) {
            std::cout << "odebrano " << iResult << " bajtow\n";
            std::vector<std::byte> odebraneDane(iResult);
            std::memcpy(odebraneDane.data(), recvbuf, iResult);
            dane.sprawdzTypRamki(odebraneDane);

            PID nowyPID = dane.gotowyPID();
            
            std::cout << "odebrany pid: k = " << nowyPID.getNastawaK() << ", ti = " << nowyPID.getNastawaTI() << ", tD = " << nowyPID.getNastawaTD() << "\n";
            const char* odp = "odebrano";
            send(ClientSocket, odp, strlen(odp), 0);

        }
        else if (iResult == 0) {
            std::cout << "polaczenie zamkniete";
        }
        else {
            std::cerr << "blad " << WSAGetLastError() << "\n";
        }
    } while (iResult > 0);

    closesocket(ClientSocket);
    closesocket(ListenSocket);
    WSACleanup();
    return;
}
