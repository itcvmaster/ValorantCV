#pragma once
#include "IEvent.h"
#include <string>

class CEnemyAppearedEvent : public IEvent
{
public:
	CEnemyAppearedEvent();

public:
	std::string m_enemyAgent;
	EnemyRect m_enemyRect;
};

