#include "serialDeserial.h"

std::vector<std::byte> serialDeserial::serializePID(const PID& pid)
{
	const int liczbaParametrow = 3;
	char typRamki = 0x01; //naglowek dla pid

	size_t rozmiar = liczbaParametrow * sizeof(double);
	std::vector<std::byte> bufor(rozmiar + sizeof(typRamki));

	std::byte* ptr = bufor.data();
	std::memcpy(ptr, &typRamki, sizeof(typRamki));
	ptr += sizeof(typRamki);

	double parametry[liczbaParametrow] = {
		pid.getP(),
		pid.getI(),
		pid.getD()
	};
	std::memcpy(ptr, parametry, rozmiar);
	return bufor;
}

std::vector<std::byte> serialDeserial::serializeARX(const ARX& arx)
{
	std::vector<double> wspA = arx.getA();
	std::vector<double> wspB = arx.getB();
	
	size_t sizeA = wspA.size() * sizeof(double);
	size_t sizeB = wspB.size() * sizeof(double);

	const size_t rozmiarStalychParametrow = sizeof(int) + (5 * sizeof(double)) + (2 * sizeof(char));
	const int liczbaWsp = 2;
	char typRamki = 0x02; //naglowek dla arx
	size_t rozmiar = sizeA + sizeB + sizeof(size_t) * liczbaWsp;
	
	std::vector<std::byte> bufor(rozmiar + sizeof(typRamki) + rozmiarStalychParametrow);
	std::byte* ptr = bufor.data();

	ptr = dodajDoBufora(ptr, typRamki);
	ptr = dodajDoBufora(ptr, arx.getK());

	double tabParametrow[] = {
		arx.getUMin(),
		arx.getUMax(),
		arx.getYMin(),
		arx.getYMax(),
		arx.getSzum()
	};
	ptr = dodajDoBufora(ptr, tabParametrow);
	ptr = dodajDoBufora(ptr, static_cast<char>(arx.getSzumAktywny()));
	ptr = dodajDoBufora(ptr, static_cast<char>(arx.getLimityAktywne()));

	size_t countA = wspA.size();
	ptr = dodajDoBufora(ptr, countA);
	std::memcpy(ptr, wspA.data(), sizeA);
	ptr += sizeA;

	size_t countB = wspB.size();
	ptr = dodajDoBufora(ptr, countB);
	std::memcpy(ptr, wspB.data(), sizeB);
	ptr += sizeB;
	
	return bufor;
}

template<typename T>
std::byte* serialDeserial::dodajDoBufora(std::byte* ptr, const T& wart)
{
	std::memcpy(ptr, &wart, sizeof(T));
	return ptr + sizeof(T);
}