#include "pch.h"
#include "AbilityCastEvent.h"

CAbilityCastEvent::CAbilityCastEvent()
{
	m_eventType = EventType::kEventAbilityCast;
	m_ability = INVALID;
}
