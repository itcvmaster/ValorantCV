#include "stdafx.h"
#include "CheckEvent.h"
#include <iostream>
#include <windows.h>


static int m_first = 0;
static int m_tTickCount = 0;
static int m_tStartTickCount = 0;
static int m_tChangeTargetShootDelay = 1500;
static char m_sKilledEnemy[20] = { 0 };

CCheckEvent::CCheckEvent()
{
	m_YouAlive = true;
	HIS_LEN = 1;
	m_tEnemyAppear = 0;
	m_tShoot = 0;
	m_Stats.m_WeaponChange = 2;
	m_Stats.m_WeaponId = 0;
	m_tStartTickCount = GetTickCount();

	m_tChangeTargetShootDelay = GetPrivateProfileInt("Set", _T("changeTargetShootDelay"), 1500, POS_INI);
}


CCheckEvent::~CCheckEvent()
{
}

void CCheckEvent::Check(CStats *stats)
{
	m_strState[0] = 0;
	m_strEvent[0] = 0;

	int i;
	
	if (m_first < HIS_LEN - 1)
	{
		m_StatsHis[m_first + 1] = *stats;
		m_first++;
		m_Stats = *stats;
		return;
	}

	m_tTickCount = GetTickCount()- m_tStartTickCount;

	for (i = 0; i < HIS_LEN - 1; i++)
		m_StatsHis[i] = m_StatsHis[i + 1];

	m_StatsHis[HIS_LEN - 1] = *stats;
	
	Etcs();
	WeaponChange();

	Score();
	RoundTime();
	Enemy();
	Ammo();
	Health();
	Armor();
	Credits();
	Agents();
	Skills();


	if (m_Stats.m_Otherplayer == 0)
		m_YouAlive = false;

	if ((m_Stats.m_Otherplayer != 0) && (m_StatsHis[0].m_Health>0))
		m_YouAlive = true;

	if (m_StatsHis[0].m_Health == INVALID)
	{
		int cntAlly = 0;

		for (i = 0; i < 5; i++)
		{
			if (strlen(m_Stats.m_YourAliveAgents[i]) > 0)
				cntAlly++;
		}

		if (cntAlly == 0)
			m_YouAlive = false;
	}

	PrintValue();
}
void CCheckEvent::PrintValue()
{
	int i, f;

	sprintf_s(m_strState, "%s\n{\"gameStats\":{\"round\":%d,\"roundTime\":%d,\"yourTeamScore\":%d,\"enemyTeamScore\":%d,\"youAlive\":%d",
		m_strState, m_Stats.m_Round, m_Stats.m_RoundTime, m_Stats.m_YourTeamScore, m_Stats.m_EnemyTeamScore, m_YouAlive);

	if ((m_StatsHis[0].m_Health == INVALID) && (m_StatsHis[0].m_Ammo == INVALID))
		strcat_s(m_strState, ",\"flash\":1");
	else 
		strcat_s(m_strState, ",\"flash\":0");

	strcat_s(m_strState, ",\"yourAliveAgents\":[");
	f = 0;

	for (i = 0; i < 5; i++)
	{
		if (strlen(m_Stats.m_YourAliveAgents[i]))
		{
			if (f == 0)
				f = 1;
			else 
				strcat_s(m_strState, ",");

			sprintf_s(m_strState, "%s\"%s\"", m_strState, m_Stats.m_YourAliveAgents[i]);
		}
	}

	strcat_s(m_strState, "],\"enemyAliveAgents\":[");
	f = 0;

	for (i = 0; i < 5; i++)
	{
		if (strlen(m_Stats.m_EnemyAliveAgents[i]))
		{
			if (f == 0)
				f = 1;
			else 
				strcat_s(m_strState, ",");

			sprintf_s(m_strState, "%s\"%s\"", m_strState, m_Stats.m_EnemyAliveAgents[i]);
		}
	}
	
	strcat_s(m_strState, "]},");

	sprintf_s(m_strState, "%s\"playerStats\":{\"health\":%d,\"hasSpike\":%d,\"armor\":%d,\"ammo\":%d,\"ammoLeft\":%d,\"credits\":%d,\"weaponId\":%d},\"abilityStats\":[%d,%d,%d,%d],\"enemyRect\":[",
		m_strState, m_Stats.m_Health, ((m_Stats.m_Spike == 0) ? 1 : 0), ((m_Stats.m_Armor < 0) ? 0 : m_Stats.m_Armor), m_Stats.m_Ammo, m_Stats.m_AmmoLeft, m_Stats.m_Credits, m_Stats.m_WeaponId, m_Stats.m_Skills[0], m_Stats.m_Skills[1], m_Stats.m_Skills[2], m_Stats.m_Skills[3]);

	for (i = 0; i < m_Stats.m_nEnemy; i++)
	{
		if (i > 0)
			strcat_s(m_strState, ",");

		sprintf_s(m_strState, "%s{\"head\":[%d,%d,%d,%d],\"body\":[%d,%d,%d,%d],\"leg\":[%d,%d,%d,%d],\"agent\":\"%s\"}", m_strState,
			m_Stats.m_Enemy[i].head_left, m_Stats.m_Enemy[i].head_top, m_Stats.m_Enemy[i].head_right, m_Stats.m_Enemy[i].head_bottom,
			m_Stats.m_Enemy[i].body_left, m_Stats.m_Enemy[i].body_top, m_Stats.m_Enemy[i].body_right, m_Stats.m_Enemy[i].body_bottom,
			m_Stats.m_Enemy[i].leg_left, m_Stats.m_Enemy[i].leg_top, m_Stats.m_Enemy[i].leg_right, m_Stats.m_Enemy[i].leg_bottom,
			m_Stats.m_Enemy[i].agents);
	}

	strcat_s(m_strState, "]}");
}

void CCheckEvent::Score()
{
	int i, j;

	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_YourTeamScore != m_StatsHis[i].m_YourTeamScore)
			break;
	}

	if ((i == HIS_LEN) && (m_StatsHis[0].m_YourTeamScore >= 0))
	{
		m_Stats.m_YourTeamScore = m_StatsHis[0].m_YourTeamScore;
	}

	for (j = 1; j < HIS_LEN; j++)
	{
		if (m_StatsHis[0].m_EnemyTeamScore != m_StatsHis[j].m_EnemyTeamScore)
			break;
	}

	if ((j == HIS_LEN) && (m_StatsHis[0].m_EnemyTeamScore >= 0))
		m_Stats.m_EnemyTeamScore = m_StatsHis[0].m_EnemyTeamScore;

	m_Stats.m_Round = m_Stats.m_YourTeamScore + m_Stats.m_EnemyTeamScore + 1;
}

void CCheckEvent::RoundTime()
{
	int i;

	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_RoundTime != m_StatsHis[i].m_RoundTime)
			break;
	}

	if ((i == HIS_LEN) && (m_StatsHis[0].m_RoundTime != MATCH_ERROR))
		m_Stats.m_RoundTime = m_StatsHis[0].m_RoundTime;
}

void CCheckEvent::Ammo()
{
	int i, j;

	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_Ammo != m_StatsHis[i].m_Ammo)
			break;
	}

	for (j = 1; j < HIS_LEN; j++)
	{
		if (m_StatsHis[0].m_AmmoLeft != m_StatsHis[j].m_AmmoLeft)
			break;
	}

	//ammo reload
	if ((i == HIS_LEN) && (j == HIS_LEN) &&
		(m_Stats.m_Ammo >= 0) && (m_Stats.m_AmmoLeft >= 0) &&
		(m_StatsHis[0].m_Ammo >= 0) && (m_StatsHis[0].m_AmmoLeft >= 0) &&
		(m_Stats.m_AmmoLeft>m_StatsHis[0].m_AmmoLeft) &&
		(m_Stats.m_Ammo + m_Stats.m_AmmoLeft == m_StatsHis[0].m_Ammo + m_StatsHis[0].m_AmmoLeft))
	{
//		if(m_YouAlive)
		sprintf_s(m_strEvent, "%s\n{\"action\":\"ammoReload\",\"time\":%d,\"oldAmmo\":%d,\"newAmmo\":%d}",
			m_strEvent, m_tTickCount, m_Stats.m_Ammo, m_StatsHis[0].m_Ammo);
	}

	//shoot
	if ((i == HIS_LEN) && (m_Stats.m_Ammo >= 0) && (m_StatsHis[0].m_Ammo >= 0) &&
		(m_Stats.m_Ammo > m_StatsHis[0].m_Ammo) && (m_Stats.m_AmmoLeft == m_StatsHis[0].m_AmmoLeft))
	{
//		if (m_YouAlive)
		{
			int tOldShoot = m_tShoot;

			m_tShoot = m_tTickCount;

			sprintf_s(m_strEvent, "%s\n{\"action\":\"shoot\",\"time\":%d,",
				m_strEvent, m_tTickCount);

			if ((m_Stats.m_nEnemy > 0) && (m_tEnemyAppear))//first shoot
			{
				sprintf_s(m_strEvent, "%s\"firstShot\":true,\"reactionTime\":%d,",
					m_strEvent, m_tShoot - m_tEnemyAppear);
				m_tEnemyAppear = 0;
			}
			else
			{
				sprintf_s(m_strEvent, "%s\"firstShot\":false,\"reactionTime\":-1,",
					m_strEvent);
			}
			
			sprintf_s(m_strEvent, "%s\"weaponId\":%d,", m_strEvent, m_Stats.m_WeaponId);//weaponId

			if (m_tShoot - tOldShoot > m_tChangeTargetShootDelay)//change target
				sprintf_s(m_strEvent, "%s\"changeTarget\":true,", m_strEvent);
			else
				sprintf_s(m_strEvent, "%s\"changeTarget\":false,", m_strEvent);

			if (m_Stats.m_nEnemy == 0)
			{
				sprintf_s(m_strEvent, "%s\"agent\":,\"crosshair\":[%d,%d,0],",//crosshair
					m_strEvent, m_StatsHis[0].m_ImgWidth / 2, m_StatsHis[0].m_ImgHeight / 2);
				strcat_s(m_strEvent, "\"enemyRect\":,\"hitEnemy\":-1,");
			}
			else//enemyRect
			{
				int m, n = 0, d, dd;

				d = 100000;
				
				for (m = 0; m < m_Stats.m_nEnemy; m++)
				{
					dd = abs(m_Stats.m_Enemy[m].left + m_Stats.m_Enemy[m].right - m_StatsHis[0].m_ImgWidth)
						+ abs(m_Stats.m_Enemy[m].top + m_Stats.m_Enemy[m].bottom - m_StatsHis[0].m_ImgHeight);
					
					if (dd < d)
					{
						d = dd;
						n = m;
					}
				}

				sprintf_s(m_strEvent, "%s\"agent\":\"%s\",\"crosshair\":[%d,%d,%d],",//crosshair
					m_strEvent, m_Stats.m_Enemy[n].agents,m_StatsHis[0].m_ImgWidth / 2, m_StatsHis[0].m_ImgHeight / 2, m_Stats.m_Enemy[n].mapY);

				sprintf_s(m_strEvent, "%s\"enemyRect\":{\"head\":[%d,%d,%d,%d],\"body\":[%d,%d,%d,%d],\"leg\":[%d,%d,%d,%d]},", m_strEvent,
					m_Stats.m_Enemy[n].head_left, m_Stats.m_Enemy[n].head_top, m_Stats.m_Enemy[n].head_right, m_Stats.m_Enemy[n].head_bottom,
					m_Stats.m_Enemy[n].body_left, m_Stats.m_Enemy[n].body_top, m_Stats.m_Enemy[n].body_right, m_Stats.m_Enemy[n].body_bottom,
					m_Stats.m_Enemy[n].leg_left, m_Stats.m_Enemy[n].leg_top, m_Stats.m_Enemy[n].leg_right, m_Stats.m_Enemy[n].leg_bottom);
				
				if ((m_Stats.m_Enemy[n].body_left <= m_StatsHis[0].m_ImgWidth / 2) &&
					(m_Stats.m_Enemy[n].body_right >= m_StatsHis[0].m_ImgWidth / 2) &&
					(m_Stats.m_Enemy[n].head_top <= m_StatsHis[0].m_ImgHeight / 2) &&
					(m_Stats.m_Enemy[n].body_bottom >= m_StatsHis[0].m_ImgHeight / 2))
				{
					strcat_s(m_strEvent, "\"hitEnemy\":2,");
				}
				else if ((m_Stats.m_Enemy[n].leg_left <= m_StatsHis[0].m_ImgWidth / 2) &&
					(m_Stats.m_Enemy[n].leg_right >= m_StatsHis[0].m_ImgWidth / 2) &&
					(m_Stats.m_Enemy[n].leg_top <= m_StatsHis[0].m_ImgHeight / 2) &&
					(m_Stats.m_Enemy[n].leg_bottom >= m_StatsHis[0].m_ImgHeight / 2))
				{
					strcat_s(m_strEvent, "\"hitEnemy\":3,");
				}
				else 
				{
					strcat_s(m_strEvent, "\"hitEnemy\":0,");
				}
			}
			
			strcat_s(m_strEvent, "\"killEnemy\":false}");
		}
	}

	if ((i == HIS_LEN) && (m_StatsHis[0].m_Ammo >= 0))
		m_Stats.m_Ammo = m_StatsHis[0].m_Ammo;

	if ((j == HIS_LEN) && (m_StatsHis[0].m_AmmoLeft >= 0))
		m_Stats.m_AmmoLeft = m_StatsHis[0].m_AmmoLeft;
}

void CCheckEvent::Health()
{
	int i;

	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_Health != m_StatsHis[i].m_Health)
			break;
	}

	if ((i==HIS_LEN)&&(m_StatsHis[0].m_Health >= 0))
		m_Stats.m_Health = m_StatsHis[0].m_Health;
}

void CCheckEvent::Armor()
{
	int i;

	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_Armor != m_StatsHis[i].m_Armor)
			break;
	}

	if ((i == HIS_LEN) && (m_StatsHis[0].m_Armor != MATCH_ERROR))
		m_Stats.m_Armor = m_StatsHis[0].m_Armor;
}

void CCheckEvent::Credits()
{
	int i;

	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_Credits != m_StatsHis[i].m_Credits)
			break;
	}

	if ((i == HIS_LEN) && (m_StatsHis[0].m_Credits >= 0))
		m_Stats.m_Credits = m_StatsHis[0].m_Credits;
}

void CCheckEvent::Agents()
{
	int cntAlly0 = 0, cntEnemy0 = 0, cntAlly = 0, cntEnemy = 0;
	int i, j;

	for (i = 0; i < 5; i++)
	{
		if (strlen(m_Stats.m_YourAliveAgents[i])>0)
			cntAlly++;

		if (strlen(m_Stats.m_EnemyAliveAgents[i])>0)
			cntEnemy++;

		if (strlen(m_StatsHis[0].m_YourAliveAgents[i])>0)
			cntAlly0++;

		if (strlen(m_StatsHis[0].m_EnemyAliveAgents[i])>0)
			cntEnemy0++;
	}

	if ((cntAlly > cntAlly0) && (!((cntAlly0 == 0) && (cntEnemy0 == 0))))
	{
		for (i = 0; i < 5; i++)
		{
			if (strlen(m_Stats.m_YourAliveAgents[i]) == 0)
				continue;

			for (j = 0; j < 5; j++)
			{
				if (strcmp(m_Stats.m_YourAliveAgents[i], m_StatsHis[0].m_YourAliveAgents[j]) == 0)
					break;
			}

			if (j == 5)
			{
				if ((m_YouAlive) && (m_StatsHis[0].m_Health == INVALID) && (m_StatsHis[0].m_Ammo == INVALID))
				{
					m_YouAlive = false; 
					m_Stats.m_Health = 0; 
					m_Stats.m_Armor = 0;
					sprintf_s(m_strEvent, "%s\n{\"action\":\"youKilled\",\"time\":%d,\"agentName\":\"%s\"}"
						, m_strEvent, m_tTickCount, m_Stats.m_YourAliveAgents[i]);
				}
				else
				{
					sprintf_s(m_strEvent, "%s\n{\"action\":\"allyKilled\",\"time\":%d,\"agentName\":\"%s\"}"
						, m_strEvent, m_tTickCount, m_Stats.m_YourAliveAgents[i]);
				}
			}
		}
	}

	if ((cntEnemy > cntEnemy0) && (!((cntAlly0 == 0) && (cntEnemy0 == 0))))
	{
		for (i = 0; i < 5; i++)
		{
			if (strlen(m_Stats.m_EnemyAliveAgents[i]) == 0)
				continue;

			for (j = 0; j < 5; j++)
			{
				if (strcmp(m_Stats.m_EnemyAliveAgents[i], m_StatsHis[0].m_EnemyAliveAgents[j]) == 0)
					break;
			}

			if (j == 5)
			{
				sprintf_s(m_strEvent, "%s\n{\"action\":\"enemyKilled\",\"time\":%d,\"agentName\":\"%s\"}"
					, m_strEvent, m_tTickCount, m_Stats.m_EnemyAliveAgents[i]);
				strcpy_s(m_sKilledEnemy, m_Stats.m_EnemyAliveAgents[i]);
			}
		}
	}

	memcpy(m_Stats.m_YourAliveAgents, m_StatsHis[0].m_YourAliveAgents, sizeof(m_Stats.m_YourAliveAgents));
	memcpy(m_Stats.m_EnemyAliveAgents, m_StatsHis[0].m_EnemyAliveAgents, sizeof(m_Stats.m_EnemyAliveAgents));
}

void CCheckEvent::Skills()
{
	if (/*(m_YouAlive) && */(m_Stats.m_Health > 0) && (m_StatsHis[0].m_Ammo == INVALID))
	{

		if ((m_Stats.m_Skills[0] == m_StatsHis[0].m_Skills[0] + 1) &&
			(m_Stats.m_Skills[1] == m_StatsHis[0].m_Skills[1]) &&
			(m_Stats.m_Skills[2] == m_StatsHis[0].m_Skills[2]) &&
			(m_Stats.m_Skills[3] == m_StatsHis[0].m_Skills[3]))
		{
			sprintf_s(m_strEvent, "%s\n{\"action\":\"abilityUsed\",\"time\":%d,\"skill\":\"C\"}"
				, m_strEvent, m_tTickCount);
		}

		if ((m_Stats.m_Skills[0] == m_StatsHis[0].m_Skills[0]) &&
			(m_Stats.m_Skills[1] == m_StatsHis[0].m_Skills[1] + 1) &&
			(m_Stats.m_Skills[2] == m_StatsHis[0].m_Skills[2]) &&
			(m_Stats.m_Skills[3] == m_StatsHis[0].m_Skills[3]))
		{
			sprintf_s(m_strEvent, "%s\n{\"action\":\"abilityUsed\",\"time\":%d,\"skill\":\"Q\"}"
				, m_strEvent, m_tTickCount);
		}
		
		if ((m_Stats.m_Skills[0] == m_StatsHis[0].m_Skills[0]) &&
			(m_Stats.m_Skills[1] == m_StatsHis[0].m_Skills[1]) &&
			(m_Stats.m_Skills[2] == m_StatsHis[0].m_Skills[2] + 1) &&
			(m_Stats.m_Skills[3] == m_StatsHis[0].m_Skills[3]))
		{
			sprintf_s(m_strEvent, "%s\n{\"action\":\"abilityUsed\",\"time\":%d,\"skill\":\"E\"}"
				, m_strEvent, m_tTickCount);
		}
		
		if ((m_Stats.m_Skills[0] == m_StatsHis[0].m_Skills[0]) &&
			(m_Stats.m_Skills[1] == m_StatsHis[0].m_Skills[1]) &&
			(m_Stats.m_Skills[2] == m_StatsHis[0].m_Skills[2]) &&
			(m_Stats.m_Skills[3] == m_StatsHis[0].m_Skills[3] + 1))
		{
			sprintf_s(m_strEvent, "%s\n{\"action\":\"abilityUsed\",\"time\":%d,\"skill\":\"X\"}"
				, m_strEvent, m_tTickCount);
		}
	}

	memcpy(m_Stats.m_Skills, m_StatsHis[0].m_Skills, sizeof(m_Stats.m_Skills));
}

void CCheckEvent::WeaponChange()
{
	int i;

	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_WeaponChange != m_StatsHis[i].m_WeaponChange)
			break;
	}

	if ((i == HIS_LEN) && (m_StatsHis[0].m_WeaponChange >= 0)) 
	{
		if ((m_StatsHis[0].m_WeaponChange == 0) && (m_StatsHis[0].m_Ammo == INVALID))
		{
			m_Stats.m_WeaponChange = m_StatsHis[0].m_WeaponChange;
			m_Stats.m_WeaponId = -1;
		}
		else if (m_StatsHis[0].m_WeaponChange > 0)
		{
			m_Stats.m_WeaponChange = m_StatsHis[0].m_WeaponChange;
			m_Stats.m_WeaponId = m_StatsHis[0].m_WeaponId;
		}
	}
}

void CCheckEvent::Etcs()
{
	int i;
	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_Spike != m_StatsHis[i].m_Spike)
			break;
	}

	if (i == HIS_LEN)
		m_Stats.m_Spike = m_StatsHis[0].m_Spike;

	for (i = 1; i < HIS_LEN; i++)
		if (m_StatsHis[0].m_Otherplayer != m_StatsHis[i].m_Otherplayer)
			break;

	if (i == HIS_LEN)
		m_Stats.m_Otherplayer = m_StatsHis[0].m_Otherplayer;

	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_HitHeadshot != m_StatsHis[i].m_HitHeadshot)
			break;
	}

	if (i == HIS_LEN)
	{
//		if (m_YouAlive)
		{
			if ((m_Stats.m_HitHeadshot == INVALID) && (m_StatsHis[0].m_HitHeadshot == 0))//kill enemy(no headshot)
			{
				sprintf_s(m_strEvent, "%s\n{\"action\":\"shoot\",\"time\":%d,\"firstShot\":false,\"reactionTime\":-1,\"weaponId\":%d,\"changeTarget\":false,}"
					, m_strEvent, m_tTickCount,m_Stats.m_WeaponId);

				if (m_Stats.m_nEnemy == 0)
				{
					sprintf_s(m_strEvent, "%s\"agent\":\"%s\",\"crosshair\":[%d,%d,0],\"enemyRect\":,\"hitEnemy\":2,",
						m_strEvent, m_sKilledEnemy, m_StatsHis[0].m_ImgWidth / 2, m_StatsHis[0].m_ImgHeight / 2);
				}
				else//enemyRect
				{
					int m, n = 0, d, dd;

					d = 100000;
					
					for (m = 0; m < m_Stats.m_nEnemy; m++)
					{
						dd = abs(m_Stats.m_Enemy[m].left + m_Stats.m_Enemy[m].right - m_StatsHis[0].m_ImgWidth)
							+ abs(m_Stats.m_Enemy[m].top + m_Stats.m_Enemy[m].bottom - m_StatsHis[0].m_ImgHeight);
						if (dd < d) {
							d = dd; n = m;
						}
					}
					
					sprintf_s(m_strEvent, "%s\"agent\":\"%s\",\"crosshair\":[%d,%d,%d],",//crosshair
						m_strEvent, m_sKilledEnemy, m_StatsHis[0].m_ImgWidth / 2, m_StatsHis[0].m_ImgHeight / 2, m_Stats.m_Enemy[n].mapY);

					sprintf_s(m_strEvent, "%s\"enemyRect\":{\"head\":[%d,%d,%d,%d],\"body\":[%d,%d,%d,%d],\"leg\":[%d,%d,%d,%d]},", m_strEvent,
						m_Stats.m_Enemy[n].head_left, m_Stats.m_Enemy[n].head_top, m_Stats.m_Enemy[n].head_right, m_Stats.m_Enemy[n].head_bottom,
						m_Stats.m_Enemy[n].body_left, m_Stats.m_Enemy[n].body_top, m_Stats.m_Enemy[n].body_right, m_Stats.m_Enemy[n].body_bottom,
						m_Stats.m_Enemy[n].leg_left, m_Stats.m_Enemy[n].leg_top, m_Stats.m_Enemy[n].leg_right, m_Stats.m_Enemy[n].leg_bottom);

					if ((m_Stats.m_Enemy[n].leg_left <= m_StatsHis[0].m_ImgWidth / 2) &&
						(m_Stats.m_Enemy[n].leg_right >= m_StatsHis[0].m_ImgWidth / 2) &&
						(m_Stats.m_Enemy[n].leg_top <= m_StatsHis[0].m_ImgHeight / 2) &&
						(m_Stats.m_Enemy[n].leg_bottom >= m_StatsHis[0].m_ImgHeight / 2))
					{
						strcat_s(m_strEvent, "\"hitEnemy\":3,");
					}
					else
					{
						strcat_s(m_strEvent, "\"hitEnemy\":2,");
					}
				}

				strcat_s(m_strEvent, "\"killEnemy\":true}");
			}
			else if ((m_Stats.m_HitHeadshot != 1) && (m_StatsHis[0].m_HitHeadshot == 1))//kill enemy(headshot)
			{
				sprintf_s(m_strEvent, "%s\n{\"action\":\"shoot\",\"time\":%d,\"firstShot\":false,\"reactionTime\":-1,\"weaponId\":%d,\"changeTarget\":false,}"
					, m_strEvent, m_tTickCount, m_Stats.m_WeaponId);

				if (m_Stats.m_nEnemy == 0)
				{
					sprintf_s(m_strEvent, "%s\"agent\":\"%s\",\"crosshair\":[%d,%d,0],\"enemyRect\":,\"hitEnemy\":1,",
						m_strEvent, m_sKilledEnemy, m_StatsHis[0].m_ImgWidth / 2, m_StatsHis[0].m_ImgHeight / 2);
				}
				else//enemyRect
				{
					int m, n = 0, d, dd;

					d = 100000;

					for (m = 0; m < m_Stats.m_nEnemy; m++)
					{
						dd = abs(m_Stats.m_Enemy[m].left + m_Stats.m_Enemy[m].right - m_StatsHis[0].m_ImgWidth)
							+ abs(m_Stats.m_Enemy[m].top + m_Stats.m_Enemy[m].bottom - m_StatsHis[0].m_ImgHeight);
						
						if (dd < d)
						{
							d = dd; n = m;
						}
					}

					sprintf_s(m_strEvent, "%s\"agent\":\"%s\",\"crosshair\":[%d,%d,%d],",//crosshair
						m_strEvent, m_sKilledEnemy, m_StatsHis[0].m_ImgWidth / 2, m_StatsHis[0].m_ImgHeight / 2, m_Stats.m_Enemy[n].mapY);
					
					sprintf_s(m_strEvent, "%s\"enemyRect\":{\"head\":[%d,%d,%d,%d],\"body\":[%d,%d,%d,%d],\"leg\":[%d,%d,%d,%d]},", m_strEvent,
						m_Stats.m_Enemy[n].head_left, m_Stats.m_Enemy[n].head_top, m_Stats.m_Enemy[n].head_right, m_Stats.m_Enemy[n].head_bottom,
						m_Stats.m_Enemy[n].body_left, m_Stats.m_Enemy[n].body_top, m_Stats.m_Enemy[n].body_right, m_Stats.m_Enemy[n].body_bottom,
						m_Stats.m_Enemy[n].leg_left, m_Stats.m_Enemy[n].leg_top, m_Stats.m_Enemy[n].leg_right, m_Stats.m_Enemy[n].leg_bottom);

					strcat_s(m_strEvent, "\"hitEnemy\":1,");
				}

				strcat_s(m_strEvent, "\"killEnemy\":true}");
			}
		}

		m_Stats.m_HitHeadshot = m_StatsHis[0].m_HitHeadshot;
	}
}

void CCheckEvent::Enemy()
{
	int i, j, k;
	
	for (i = 1; i < HIS_LEN; i++)
	{
		if (m_StatsHis[0].m_nEnemy != m_StatsHis[i].m_nEnemy)
			break;
	}
	
	//check new enemy
	int idx, fOldj[5] = { 0 }, fOldk[5] = { 0 }, d, dd;

	if ((i == HIS_LEN) && (m_Stats.m_nEnemy < m_StatsHis[0].m_nEnemy))
	{
		for (j = 0; j < m_Stats.m_nEnemy; j++)
		{
			if (strlen(m_Stats.m_Enemy[j].agents)>0)
			{
				for (k = 0; k < m_StatsHis[0].m_nEnemy; k++)
				{
					if (fOldk[k])
						continue;

					if (strcmp(m_Stats.m_Enemy[j].agents, m_StatsHis[HIS_LEN - 1].m_Enemy[k].agents) == 0)
					{
						fOldj[j] = 1;
						fOldk[k] = 1;
						break;
					}
				}

				if (k < m_StatsHis[0].m_nEnemy)
					continue;
			}
		}

		for (j = 0; j < m_Stats.m_nEnemy; j++)
		{
			if (fOldj[j])
				continue;

			d = 100000;

			for (k = 0; k < m_StatsHis[0].m_nEnemy; k++)
			{
				if (fOldk[k])
					continue;

				dd = abs(m_StatsHis[HIS_LEN - 1].m_Enemy[k].left + m_StatsHis[HIS_LEN - 1].m_Enemy[k].right - m_Stats.m_Enemy[j].left - m_Stats.m_Enemy[j].right)
					+ abs(m_StatsHis[HIS_LEN - 1].m_Enemy[k].top + m_StatsHis[HIS_LEN - 1].m_Enemy[k].bottom - m_Stats.m_Enemy[j].top - m_Stats.m_Enemy[j].bottom);

				if (dd < d)
				{
					d = dd;
					idx = k;
				}
			}

			fOldk[idx] = 1;
		}

		for (k = 0; k < m_StatsHis[0].m_nEnemy; k++)
		{
			if (fOldk[k])
				continue;

			sprintf_s(m_strEvent, "%s\n{\"action\":\"enemyAppeared\",\"time\":%d,\"agent\":\"%s\",\"enemyRect\":{\"head\":[%d,%d,%d,%d],\"body\":[%d,%d,%d,%d],\"leg\":[%d,%d,%d,%d]}}", m_strEvent,
				m_tTickCount, m_StatsHis[HIS_LEN - 1].m_Enemy[k].agents,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].head_left,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].head_top,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].head_right,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].head_bottom,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].body_left,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].body_top,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].body_right,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].body_bottom,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].leg_left,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].leg_top,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].leg_right,
				m_StatsHis[HIS_LEN - 1].m_Enemy[k].leg_bottom
				);
		}

		if (m_Stats.m_nEnemy == 0)
			m_tEnemyAppear = m_tTickCount;
	}

	if (i == HIS_LEN) 
	{
		m_Stats.m_nEnemy = m_StatsHis[0].m_nEnemy;

		for (j = 0; j < m_Stats.m_nEnemy; j++)
			m_Stats.m_Enemy[j] = m_StatsHis[HIS_LEN - 1].m_Enemy[j];
	}
}
