#pragma once
#include "s_arx.h"
#include "s_pid.h"
#include <WinSock2.h>

class serialDeserial
{
	PID deserialPID;
	ARX deserialARX;
public:
	std::vector<std::byte> serializePID(const PID& pid);
	std::vector<std::byte> serializeARX(const ARX& arx);
	template<typename T> std::byte* dodajDoBufora(std::byte* ptr, const T& wart);
};


