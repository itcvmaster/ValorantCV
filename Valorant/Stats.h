#pragma once
#include "../inc/common.h"

class CStats
{
public:
	CStats();
	~CStats();

	bool Init(int imgWidth, int imgHeight);

	void GetStats(unsigned char *imgPtr, int imgWidth, int imgHeight);
	void GetScore();
	void GetRoundTime();
	void GetAmmo();
	void GetHealth();
	void GetArmor();
	void GetAmmoLeft();
	void GetCredits();
	void GetAllyAgents();
	void GetEnemyAgents();
	void GetSkills();
	void GetWeaponchange();
	void GetETC();

	void GetEnemy();
	void GetEnemyOnScreen();
	void GetEnemyOnMap();

	int m_ImgWidth, m_ImgHeight;

	int m_Round, m_YourTeamScore, m_EnemyTeamScore;
	int m_RoundTime, m_Ammo, m_Health;
	int m_Armor, m_AmmoLeft;
	int m_Credits;
	char m_YourAliveAgents[5][16], m_EnemyAliveAgents[5][16];
	int m_Skills[4];//c,q,e,x
	int m_Spike;//0:has,//-1:no
	int m_HitHeadshot;//0:hit,1:heashot,-1:no
	int m_Otherplayer;//0:otherplayer,-1:self
	int m_WeaponChange;//0:skill,1:automatic rifle,2:pistol,3:knife,
	int m_WeaponId;//-1:skill,0:Knife,...

	int m_nEnemy;
	ENEMYRECT m_Enemy[5];
	int m_nMapEnemy;
	ENEMYRECT m_MapEnemy[5];

private:
	int MatchWhitePattern(int x, int y, WHITE_PATTERN *ptn, int nPtn);
	int MatchWhitePattern_(int x, int y, WHITE_PATTERN *ptn, int nPtn);
	int MatchColorPattern(int x, int y, COLOR_PATTERN *ptn, int nPtn, bool flip=false);
	int MatchColorPattern_(int x, int y, COLOR_PATTERN *ptn, int nPtn, bool flip = false);
	int MatchMapPattern(int x, int y, COLOR_PATTERN *ptn, int nPtn);
};

