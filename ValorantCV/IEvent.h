#pragma once

#include "Common.h"

class IEvent
{
public:
	IEvent();
	virtual ~IEvent();
	virtual void exitInstance();

public:
	IEvent* createInstance(EventType eventType);

public:
	uint8_t m_eventType;
	uint64_t m_timestamp;
};

