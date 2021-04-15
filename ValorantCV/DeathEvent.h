#pragma once
#include "IEvent.h"
#include <string>

class CDeathEvent : public IEvent
{
public:
	CDeathEvent();

public:
	std::string m_killerAgent;
};

