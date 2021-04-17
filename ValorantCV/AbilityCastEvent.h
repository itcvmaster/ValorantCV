#pragma once
#include "IEvent.h"

#include <string>

class CAbilityCastEvent : public IEvent
{
public:
	CAbilityCastEvent();

public:
	int8_t m_ability;	// 0 - Q, 1 - C, 2 - E, 3 - X
};

