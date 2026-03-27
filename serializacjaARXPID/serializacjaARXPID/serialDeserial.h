#pragma once
#include "s_arx.h"
#include "s_pid.h"

class serialDeserial
{
	PID m_deserialPID;
	ARX m_deserialARX;
public:
	serialDeserial();

	//serializacja
	std::vector<std::byte> serializePID(const PID& pid);
	std::vector<std::byte> serializeARX(const ARX& arx);
	template<typename T> std::byte* dodajDoBufora(std::byte* ptr, const T& wart);

	//deserializacja
	template<typename T> std::byte* pobierzZBufora(std::byte* ptr, T& dest);
	void sprawdzTypRamki(std::vector<std::byte> &dane);
	void deserializedPID(std::byte* ptr);
	void deserialiedARX(std::byte* ptr);

	//pid arx
	PID gotowyPID() const { return m_deserialPID; };
	ARX gotowyARX() const { return m_deserialARX; };
};


