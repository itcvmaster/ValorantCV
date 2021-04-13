#pragma once
#include "Stats.h"


class CCheckEvent
{
public:
	CCheckEvent();
	~CCheckEvent();

	CStats m_StatsHis[9];
	CStats m_Stats;
	bool m_YouAlive;
	char m_strState[2048];
	char m_strEvent[2048];
	int HIS_LEN;

	int m_tEnemyAppear;
	int m_tShoot;

	void Check(CStats *stats);
	void PrintValue();
	void Score();
	void RoundTime();
	void Ammo();
	void Health();
	void Armor();
	void Credits();
	void Agents();
	void Skills();
	void WeaponChange();
	void Etcs();
	void Enemy();
};

