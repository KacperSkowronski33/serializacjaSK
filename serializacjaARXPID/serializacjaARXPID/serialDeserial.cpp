#include "serialDeserial.h"
#include <iostream>

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

void serialDeserial::sprawdzTypRamki(std::vector<std::byte> &dane)
{
	if (dane.empty()) return;
	std::byte* ptr = dane.data();
	char typRamki;
	ptr = pobierzZBufora(ptr, typRamki);
	
	switch (typRamki)
	{
	case 0x01:
		this->deserializedPID(ptr);
		break;
	case 0x02:
		this->deserialiedARX(ptr);
		break;
	default:
		break;
	};
}

void serialDeserial::deserializedPID(std::byte* ptr)
{
	double k, ti, td;
	ptr = pobierzZBufora(ptr, k);
	ptr = pobierzZBufora(ptr, ti);
	ptr = pobierzZBufora(ptr, td);
	m_deserialPID.setK(k);
	m_deserialPID.setTI(ti);
	m_deserialPID.setTD(td);
}

void serialDeserial::deserialiedARX(std::byte* ptr)
{
	int k;
	ptr = pobierzZBufora(ptr, k);
	
	double uMin, uMax, yMin, yMax, szum;
	double tabParametrow[5];
	ptr = pobierzZBufora(ptr, tabParametrow);
	uMin = tabParametrow[0]; uMax = tabParametrow[1]; yMin = tabParametrow[2]; yMax = tabParametrow[3]; szum = tabParametrow[4];
	
	char czySzum, czyLimity;
	ptr = pobierzZBufora(ptr, czySzum);
	ptr = pobierzZBufora(ptr, czyLimity);
	
	size_t countA, countB;
	ptr = pobierzZBufora(ptr, countA);
	std::vector<double> wspA(countA);
	std::memcpy(wspA.data(), ptr, countA * sizeof(double));
	ptr += countA * sizeof(double);

	ptr = pobierzZBufora(ptr, countB);
	std::vector<double> wspB(countB);
	std::memcpy(wspB.data(), ptr, countB * sizeof(double));

	m_deserialARX.ustawParametry(wspA, wspB, k);
	m_deserialARX.ustawLimitWejscia(uMin, uMax);
	m_deserialARX.ustawLimitWyjscia(yMin, yMax);
	m_deserialARX.ustawAmplitudeSzumu(szum);
	m_deserialARX.przelaczSzum(static_cast<bool>(czySzum));
	m_deserialARX.przelaczLimity(static_cast<bool>(czyLimity));
}

template<typename T>
std::byte* serialDeserial::dodajDoBufora(std::byte* ptr, const T& wart)
{
	std::memcpy(ptr, &wart, sizeof(T));
	return ptr + sizeof(T);
}

template<typename T>
std::byte* serialDeserial::pobierzZBufora(std::byte* ptr, T& dest)
{
	std::memcpy(&dest, ptr, sizeof(T));
	return ptr + sizeof(T);
}
