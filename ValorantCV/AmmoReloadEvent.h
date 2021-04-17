#pragma once
#include "IEvent.h"

class CAmmoReloadEvent : public IEvent
{
public:
	CAmmoReloadEvent();

public:
	int16_t m_oldAmmo;
	int16_t m_newAmmo;
};

