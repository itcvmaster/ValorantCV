#include "pch.h"
#include "IEvent.h"

IEvent::IEvent()
{
}

IEvent* IEvent::createInstance(EventType eventType)
{
	return nullptr;
}

IEvent::~IEvent()
{
	exitInstance();
}

void IEvent::exitInstance()
{
}
