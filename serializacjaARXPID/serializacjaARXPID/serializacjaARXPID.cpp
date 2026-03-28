#include <iostream>
#include <WinSock2.h>
#include <iomanip>
#include "serialDeserial.h"
#include "SerwerTCP.h"
#include "KlientTCP.h"
#pragma comment(lib, "ws2_32.lib")
#define PORT 54321

void wypiszRamkeHercules(const std::vector<std::byte>& bufor) {
    std::cout << "skopiuj to do herculesa: \n";
    for (auto b : bufor) {
        std::cout << "$" << std::hex << std::uppercase << std::setw(2) << std::setfill('0') << static_cast<int>(b);
    }
    std::cout << std::dec << "\n\n";
}

void main()
{
    std::cout << "Wybierz role [ k / s ] ";
    char rola;
    std::cin >> rola;
    std::cout << "\nPodaj port ";
    int port;
    std::cin >> port;

    if (rola == 's') {
        std::cout << "Serwer\n\n";
        SerwerTCP serwer;
        serwer.uruchomSerwerTCP(port);
    }
    else if (rola == 'k') {
        std::cout << "Klient\n\n";
        std::cout << "\nPodaj adres ip ";
        char adres[15];
        std::cin >> adres;
        
        KlientTCP klient;

        klient.polaczZSerwerem(adres, port);
        
        serialDeserial przedSerialem;
        PID testPID(1.0, 2.0, 3.0);
        testPID.setK(1.5);
        testPID.oblicz(10.0);
        std::vector<std::byte> ramkaPID = przedSerialem.serializePID(testPID);

        ARX testARX({ 1.1,2.2,3.3 }, { 4.4,5.5,6.6 }, 2);
        testARX.ustawLimitWejscia(-5.0, 5.0);
        testARX.ustawAmplitudeSzumu(0.5);
        std::vector<std::byte> ramkaARX = przedSerialem.serializeARX(testARX);

        char petla;
        do {
            std::cout << "wysylam dane\n";
            klient.wyslijDane(ramkaARX);
            klient.odbierzOdpowiedz();
            klient.wyslijDane(ramkaPID);
            klient.odbierzOdpowiedz();
            std::cout << "Czy kontynuowac? [ s - stop / c - kontynuuj ] ";
            std::cin >> petla;
        } while (petla != 's');
        
    }
    return;
}
