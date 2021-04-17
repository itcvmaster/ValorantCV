#pragma once
#include "IEvent.h"
#include <string>

class CYouKilledEvent : public IEvent
{
public:
	CYouKilledEvent();

public:
	std::string m_killerAgent;
};

