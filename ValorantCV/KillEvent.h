#pragma once
#include "IEvent.h"
#include <string>

class CKillEvent : public IEvent
{
public:
    CKillEvent();

public:
    std::string m_killerAgent;
    std::string m_targetAgent;
};

