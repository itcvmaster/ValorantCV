#pragma once
#include "IEvent.h"
#include "Common.h"
#include <string>
#include <vector>

class CShootEvent : public IEvent
{
public:
    CShootEvent();

public:
    int8_t m_isFirstShot;
    int32_t m_reactionTimeMs;
    int8_t m_weaponId;
    int8_t m_isTargetChanged;
    std::string m_targetAgent;
    int16_t m_crossHairX;
    int16_t m_crossHairY;
    int16_t m_crossHairZ;
    int8_t m_hitType;
    int8_t m_hasKilledEnemy;

    std::vector<EnemyRect> m_aryEnemyRects;
};

