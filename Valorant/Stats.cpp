#include "stdafx.h"
#include "Stats.h"
#include <iostream>
#include <Windows.h>

static WHITE_PATTERN m_PtnScore[10];
static WHITE_PATTERN m_PtnTime[10];
static WHITE_PATTERN m_PtnAmmo[10];
static WHITE_PATTERN m_PtnArmor[10];
static WHITE_PATTERN m_PtnCredits[10];
static COLOR_PATTERN m_PtnAgents[N_AGENTS];
static WHITE_PATTERN m_PtnWeaponChange[1];
static WHITE_PATTERN m_PtnSpike[1];
static COLOR_PATTERN m_PtnHitHeadshot[2];
static WHITE_PATTERN m_PtnOtherplayer[1];
static COLOR_PATTERN m_PtnMapAgents[N_MAPAGENTS];
static WHITE_PATTERN m_PtnMapRedCircle[1];
static WHITE_PATTERN m_PtnWeapon[18];

static unsigned char *m_ImgPtr;

static int allyScoreY, allyScoreX1, allyScoreX10, allyScoreX01;
static int enemyScoreY, enemyScoreX1, enemyScoreX10, enemyScoreX01;
static int timeY, timeX100, timeX010, timeX001;
static int ammoY, ammoX100, ammoX010, ammoX001;
static int healthY, healthX100, healthX010, healthX001, healthX10, healthX01, healthX1;
static int armorY, armorX10, armorX01, armorX1;
static int ammoleftY, ammoleftX100, ammoleftX010, ammoleftX001;
static int creditsY, creditsX1, creditsX10, creditsX100, creditsX1000, creditsX10000, creditsX100000;
static int agentsY, agentsXAlly, agentsXEnemy, agentsXStep;
static int weaponchangeY, weaponchangeX, weaponchangeY1, weaponchangeX1, weaponchangeY2, weaponchangeX2, weaponchangeY3, weaponchangeX3;
static int spikeY, spikeX, hitheadshotY, hitheadshotX, otherY, otherX;
static int skillY, skillStep, skillWidth;
static int weaponX, weaponY1, weaponY2, weaponY3;

CStats::CStats()
{
	m_ImgHeight = 0;
}

CStats::~CStats()
{

}

bool CStats::Init(int imgWidth, int imgHeight)
{
	FILE *fp;
	char fn[MAX_PATH];

	m_ImgWidth = imgWidth;
	OutputDebugStringA("CStats::Init - 1");
	if (m_ImgHeight == imgHeight)
		return true;
	
	m_ImgHeight = imgHeight;
	
	sprintf_s(fn, "ptn%d.dat", m_ImgHeight);
	OutputDebugStringA("CStats::Init - 2");
	if (fopen_s(&fp, fn, "rb"))
	{
		m_ImgHeight = 0; 
		return false;
	}

	OutputDebugStringA("CStats::Init - 3");
	fread(m_PtnScore, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnTime, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnAmmo, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnArmor, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnCredits, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnAgents, N_AGENTS, sizeof(COLOR_PATTERN), fp);
	fread(m_PtnWeaponChange, 1, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnSpike, 1, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnHitHeadshot, 2, sizeof(COLOR_PATTERN), fp);
	fread(m_PtnOtherplayer, 1, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnMapAgents, N_MAPAGENTS, sizeof(COLOR_PATTERN), fp);
	fread(m_PtnMapRedCircle, 1, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnWeapon, 18, sizeof(WHITE_PATTERN), fp);
	
	fclose(fp);
	OutputDebugStringA("CStats::Init - 4");
	char strApp[MAX_PATH];
	
	sprintf_s(strApp, "%d", m_ImgHeight);

	allyScoreY = GetPrivateProfileInt(strApp, _T("allyScoreY"), 0, POS_INI);
	allyScoreX1 = GetPrivateProfileInt(strApp, _T("allyScoreX1"), 0, POS_INI);
	allyScoreX10 = GetPrivateProfileInt(strApp, _T("allyScoreX10"), 0, POS_INI);
	allyScoreX01 = GetPrivateProfileInt(strApp, _T("allyScoreX01"), 0, POS_INI);
	enemyScoreY = GetPrivateProfileInt(strApp, _T("enemyScoreY"), 0, POS_INI);
	enemyScoreX1 = GetPrivateProfileInt(strApp, _T("enemyScoreX1"), 0, POS_INI);
	enemyScoreX10 = GetPrivateProfileInt(strApp, _T("enemyScoreX10"), 0, POS_INI);
	enemyScoreX01 = GetPrivateProfileInt(strApp, _T("enemyScoreX01"), 0, POS_INI);

	timeY = GetPrivateProfileInt(strApp, _T("timeY"), 0, POS_INI);
	timeX100 = GetPrivateProfileInt(strApp, _T("timeX100"), 0, POS_INI);
	timeX010 = GetPrivateProfileInt(strApp, _T("timeX010"), 0, POS_INI);
	timeX001 = GetPrivateProfileInt(strApp, _T("timeX001"), 0, POS_INI);

	ammoY = GetPrivateProfileInt(strApp, _T("ammoY"), 0, POS_INI);
	ammoX100 = GetPrivateProfileInt(strApp, _T("ammoX100"), 0, POS_INI);
	ammoX010 = GetPrivateProfileInt(strApp, _T("ammoX010"), 0, POS_INI);
	ammoX001 = GetPrivateProfileInt(strApp, _T("ammoX001"), 0, POS_INI);

	healthY = GetPrivateProfileInt(strApp, _T("healthY"), 0, POS_INI);
	healthX100 = GetPrivateProfileInt(strApp, _T("healthX100"), 0, POS_INI);
	healthX010 = GetPrivateProfileInt(strApp, _T("healthX010"), 0, POS_INI);
	healthX001 = GetPrivateProfileInt(strApp, _T("healthX001"), 0, POS_INI);
	healthX10 = GetPrivateProfileInt(strApp, _T("healthX10"), 0, POS_INI);
	healthX01 = GetPrivateProfileInt(strApp, _T("healthX01"), 0, POS_INI);
	healthX1 = GetPrivateProfileInt(strApp, _T("healthX1"), 0, POS_INI);

	armorY = GetPrivateProfileInt(strApp, _T("armorY"), 0, POS_INI);
	armorX10 = GetPrivateProfileInt(strApp, _T("armorX10"), 0, POS_INI);
	armorX01 = GetPrivateProfileInt(strApp, _T("armorX01"), 0, POS_INI);
	armorX1 = GetPrivateProfileInt(strApp, _T("armorX1"), 0, POS_INI);

	ammoleftY = GetPrivateProfileInt(strApp, _T("ammoleftY"), 0, POS_INI);
	ammoleftX100 = GetPrivateProfileInt(strApp, _T("ammoleftX100"), 0, POS_INI);
	ammoleftX010 = GetPrivateProfileInt(strApp, _T("ammoleftX010"), 0, POS_INI);
	ammoleftX001 = GetPrivateProfileInt(strApp, _T("ammoleftX001"), 0, POS_INI);

	creditsY = GetPrivateProfileInt(strApp, _T("creditsY"), 0, POS_INI);
	creditsX1 = GetPrivateProfileInt(strApp, _T("creditsX1"), 0, POS_INI);
	creditsX10 = GetPrivateProfileInt(strApp, _T("creditsX10"), 0, POS_INI);
	creditsX100 = GetPrivateProfileInt(strApp, _T("creditsX100"), 0, POS_INI);
	creditsX1000 = GetPrivateProfileInt(strApp, _T("creditsX1000"), 0, POS_INI);
	creditsX10000 = GetPrivateProfileInt(strApp, _T("creditsX10000"), 0, POS_INI);

	agentsY = GetPrivateProfileInt(strApp, _T("agentsY"), 0, POS_INI);
	agentsXAlly = GetPrivateProfileInt(strApp, _T("agentsXAlly"), 0, POS_INI);
	agentsXEnemy = GetPrivateProfileInt(strApp, _T("agentsXEnemy"), 0, POS_INI);
	agentsXStep = GetPrivateProfileInt(strApp, _T("agentsXStep"), 0, POS_INI);

	weaponchangeY = GetPrivateProfileInt(strApp, _T("weaponchangeY"), 0, POS_INI);
	weaponchangeX = GetPrivateProfileInt(strApp, _T("weaponchangeX"), 0, POS_INI);
	weaponchangeY1 = GetPrivateProfileInt(strApp, _T("weaponchangeY1"), 0, POS_INI);
	weaponchangeX1 = GetPrivateProfileInt(strApp, _T("weaponchangeX1"), 0, POS_INI);
	weaponchangeY2 = GetPrivateProfileInt(strApp, _T("weaponchangeY2"), 0, POS_INI);
	weaponchangeX2 = GetPrivateProfileInt(strApp, _T("weaponchangeX2"), 0, POS_INI);
	weaponchangeY3 = GetPrivateProfileInt(strApp, _T("weaponchangeY3"), 0, POS_INI);
	weaponchangeX3 = GetPrivateProfileInt(strApp, _T("weaponchangeX3"), 0, POS_INI);
	weaponX = GetPrivateProfileInt(strApp, _T("weaponX"), 0, POS_INI);
	weaponY1 = GetPrivateProfileInt(strApp, _T("weaponY1"), 0, POS_INI);
	weaponY2 = GetPrivateProfileInt(strApp, _T("weaponY2"), 0, POS_INI);
	weaponY3 = GetPrivateProfileInt(strApp, _T("weaponY3"), 0, POS_INI);

	spikeY = GetPrivateProfileInt(strApp, _T("spikeY"), 0, POS_INI);
	spikeX = GetPrivateProfileInt(strApp, _T("spikeX"), 0, POS_INI);
	hitheadshotY = GetPrivateProfileInt(strApp, _T("hitheadshotY"), 0, POS_INI);
	hitheadshotX = GetPrivateProfileInt(strApp, _T("hitheadshotX"), 0, POS_INI);
	otherY = GetPrivateProfileInt(strApp, _T("otherY"), 0, POS_INI);
	otherX = GetPrivateProfileInt(strApp, _T("otherX"), 0, POS_INI);

	skillY = GetPrivateProfileInt(strApp, _T("skillY"), 0, POS_INI);
	skillStep = GetPrivateProfileInt(strApp, _T("skillStep"), 0, POS_INI);
	skillWidth = GetPrivateProfileInt(strApp, _T("skillWidth"), 0, POS_INI);
	OutputDebugStringA("CStats::Init - 5");
	return true;
}

void CStats::GetStats(unsigned char *imgPtr, int imgWidth, int imgHeight)
{
	if (!Init(imgWidth, imgHeight))
		return;

	m_ImgPtr = imgPtr;
	
	GetScore();// cout << "  round:" << m_Round << "," << m_YourTeamScore << ":" << m_EnemyTeamScore;
	GetRoundTime();// cout << "  RoundTime:" << m_RoundTime;
	GetAmmo();//cout << "  Ammo:" << m_Ammo;
	GetHealth();// cout << "  Health:" << m_Health;
	GetArmor();// cout << "  Armor:" << m_Armor;
	GetAmmoLeft();// cout << "  AmmoLeft:" << m_AmmoLeft;
	GetCredits();// cout << "  Credits:" << m_Credits;
	GetAllyAgents();// cout << endl << m_YourAliveAgents[0] << m_YourAliveAgents[1] << m_YourAliveAgents[2] << m_YourAliveAgents[3] << m_YourAliveAgents[4];
	GetEnemyAgents();// cout << endl << m_EnemyAliveAgents[0] << m_EnemyAliveAgents[1] << m_EnemyAliveAgents[2] << m_EnemyAliveAgents[3] << m_EnemyAliveAgents[4];
	GetSkills();// cout << "  Skills:" << m_Skills[0] << ":" << m_Skills[1] << ":" << m_Skills[2] << ":" << m_Skills[3];
	GetWeaponchange();//cout << "   weaponchange:" << m_WeaponChange;
	GetETC();// cout << "  Spike:" << m_Spike << "  Hit:" << m_HitHeadshot << "  Other:" << m_Otherplayer;
	
	GetEnemy();
}

void CStats::GetScore()
{
	int ally, ally1, ally2, enemy, enemy1, enemy2;
	
	ally = MatchWhitePattern(m_ImgWidth / 2 + allyScoreX1, allyScoreY, m_PtnScore, 10);
	ally1 = MatchWhitePattern(m_ImgWidth / 2 + allyScoreX10, allyScoreY, m_PtnScore, 10);
	ally2 = MatchWhitePattern(m_ImgWidth / 2 + allyScoreX01, allyScoreY, m_PtnScore, 10);

	if ((ally == INVALID) && (ally1 != INVALID) && (ally2 != INVALID))
		m_YourTeamScore = ally1 * 10 + ally2;
	else if ((ally != INVALID) && (ally1 == INVALID) && (ally2 == INVALID))
		m_YourTeamScore = ally;
	else if ((ally == INVALID) && (ally1 == INVALID) && (ally2 == INVALID))
		m_YourTeamScore = INVALID;
	else 
		m_YourTeamScore = MATCH_ERROR;

	enemy = MatchWhitePattern(m_ImgWidth / 2 + enemyScoreX1, enemyScoreY, m_PtnScore, 10);
	enemy1 = MatchWhitePattern(m_ImgWidth / 2 + enemyScoreX10, enemyScoreY, m_PtnScore, 10);
	enemy2 = MatchWhitePattern(m_ImgWidth / 2 + enemyScoreX01, enemyScoreY, m_PtnScore, 10);

	if ((enemy == INVALID) && (enemy1 != INVALID) && (enemy2 != INVALID))
		m_EnemyTeamScore = enemy1 * 10 + enemy2;
	else if ((enemy != INVALID) && (enemy1 == INVALID) && (enemy2 == INVALID))
		m_EnemyTeamScore = enemy;
	else if ((enemy == INVALID) && (enemy1 == INVALID) && (enemy2 == INVALID))
		m_EnemyTeamScore = INVALID;
	else 
		m_EnemyTeamScore = MATCH_ERROR;
}

void CStats::GetRoundTime()
{
	int time1, time2, time3;

	time1 = MatchWhitePattern(m_ImgWidth / 2 + timeX100, timeY, m_PtnTime, 2);
	time2 = MatchWhitePattern(m_ImgWidth / 2 + timeX010, timeY, m_PtnTime, 6);
	time3 = MatchWhitePattern(m_ImgWidth / 2 + timeX001, timeY, m_PtnTime, 10);

	if ((time1 != INVALID) && (time2 != INVALID) && (time3 != INVALID)) 
		m_RoundTime=time1 * 60 + time2 * 10 + time3;
	else if ((time1 == INVALID) && (time2 == INVALID) && (time3 == INVALID)) 
		m_RoundTime = INVALID;
	else 
		m_RoundTime = MATCH_ERROR;
}

void CStats::GetAmmo()
{
	int ammo0, ammo1, ammo2;

	ammo0 = MatchWhitePattern(m_ImgWidth / 2 + ammoX100, ammoY, m_PtnAmmo, 3);
	ammo1 = MatchWhitePattern(m_ImgWidth / 2 + ammoX010, ammoY, m_PtnAmmo, 10);
	ammo2 = MatchWhitePattern(m_ImgWidth / 2 + ammoX001, ammoY, m_PtnAmmo, 10);

	if ((ammo0 != INVALID) && (ammo1 != INVALID) && (ammo2 != INVALID))
		m_Ammo = ammo0 * 100 + ammo1 * 10 + ammo2;
	else if ((ammo0 == INVALID) && (ammo1 != INVALID) && (ammo2 != INVALID))
		m_Ammo= ammo1 * 10 + ammo2;
	else if ((ammo0 == INVALID) && (ammo1 == INVALID) && (ammo2 != INVALID))
		m_Ammo= ammo2;
	else if ((ammo0 == INVALID) && (ammo1 == INVALID) && (ammo2 == INVALID))
		m_Ammo = INVALID;
	else 
		m_Ammo = MATCH_ERROR;
}

void CStats::GetHealth()
{
	int health1, health2, health3, health4, health5, health6;

	health1 = MatchWhitePattern(m_ImgWidth / 2 + healthX100, healthY, m_PtnAmmo, 2);
	health2 = MatchWhitePattern(m_ImgWidth / 2 + healthX010, healthY, m_PtnAmmo, 10);
	health3 = MatchWhitePattern(m_ImgWidth / 2 + healthX001, healthY, m_PtnAmmo, 10);

	health4 = MatchWhitePattern(m_ImgWidth / 2 + healthX10, healthY, m_PtnAmmo, 10);
	health5 = MatchWhitePattern(m_ImgWidth / 2 + healthX01, healthY, m_PtnAmmo, 10);
	
	health6 = MatchWhitePattern(m_ImgWidth / 2 + healthX1, healthY, m_PtnAmmo, 10);

	if ((health1 != INVALID) && (health2 != INVALID) && (health3 != INVALID)&& (health4 == INVALID) && (health5 == INVALID) && (health6 == INVALID))
		m_Health = health1 * 100 + health2 * 10 + health3;
	else if ((health1 == INVALID) && (health2 == INVALID) && (health3 == INVALID) && (health4 != INVALID) && (health5 != INVALID) && (health6 == INVALID))
		m_Health = health4 * 10 + health5;
	else if ((health1 == INVALID) && (health2 == INVALID) && (health3 == INVALID) && (health4 == INVALID) && (health5 == INVALID) && (health6 != INVALID))
		m_Health = health6;
	else if ((health1 == INVALID) && (health2 == INVALID) && (health3 == INVALID) && (health4 == INVALID) && (health5 == INVALID) && (health6 == INVALID))
		m_Health = INVALID;
	else 
		m_Health = MATCH_ERROR;
}

void CStats::GetArmor()
{
	int armor1, armor2, armor3;

	armor1 = MatchWhitePattern(m_ImgWidth / 2 + armorX10, armorY, m_PtnArmor, 10);
	armor2 = MatchWhitePattern(m_ImgWidth / 2 + armorX01, armorY, m_PtnArmor, 10);
	armor3 = MatchWhitePattern(m_ImgWidth / 2 + armorX1, armorY, m_PtnArmor, 10);
	
	if ((armor1 != INVALID) && (armor2 != INVALID)&& (armor3 == INVALID))
		m_Armor = armor1 * 10 + armor2;
	else if ((armor1 == INVALID) && (armor2 == INVALID) && (armor3 != INVALID))
		m_Armor = armor3;
	else if ((armor1 == INVALID) && (armor2 == INVALID) && (armor3 == INVALID))
		m_Armor = INVALID;
	else
		m_Armor = MATCH_ERROR;

}
void CStats::GetAmmoLeft()
{
	int ammoleft1, ammoleft2, ammoleft3;

	ammoleft1 = MatchWhitePattern(m_ImgWidth / 2 + ammoleftX100, ammoleftY, m_PtnArmor, 10);
	ammoleft2 = MatchWhitePattern(m_ImgWidth / 2 + ammoleftX010, ammoleftY, m_PtnArmor, 10);
	ammoleft3 = MatchWhitePattern(m_ImgWidth / 2 + ammoleftX001, ammoleftY, m_PtnArmor, 10);

	if ((ammoleft1 != INVALID) && (ammoleft2 == INVALID) && (ammoleft3 == INVALID))
		m_AmmoLeft = ammoleft1;
	else if ((ammoleft1 != INVALID) && (ammoleft2 != INVALID) && (ammoleft3 == INVALID))
		m_AmmoLeft = ammoleft1 * 10 + ammoleft2;
	else if ((ammoleft1 != INVALID) && (ammoleft2 != INVALID) && (ammoleft3 != INVALID))
		m_AmmoLeft = ammoleft1 * 100 + ammoleft2 * 10 + ammoleft3;
	else if ((ammoleft1 == INVALID) && (ammoleft2 == INVALID) && (ammoleft3 == INVALID))
		m_AmmoLeft = INVALID;
	else 
		m_AmmoLeft = MATCH_ERROR;
}

void CStats::GetCredits()
{
	int credit;

	m_Credits = INVALID;
	
	credit = MatchWhitePattern(m_ImgWidth+ creditsX1, creditsY, m_PtnCredits, 10);
	
	if (credit == INVALID)
		return;

	m_Credits = credit;
	credit = MatchWhitePattern(m_ImgWidth + creditsX10, creditsY, m_PtnCredits, 10);
	
	if (credit == INVALID)
		return;
	
	m_Credits += credit * 10; 
	credit = MatchWhitePattern(m_ImgWidth + creditsX100, creditsY, m_PtnCredits, 10); 
	
	if (credit == INVALID)
		return;
	
	m_Credits += credit * 100; 
	credit = MatchWhitePattern(m_ImgWidth + creditsX1000, creditsY, m_PtnCredits, 10); 
	
	if (credit == INVALID)
		return;
	
	m_Credits += credit * 1000; 
	credit = MatchWhitePattern(m_ImgWidth + creditsX10000, creditsY, m_PtnCredits, 10); 
	
	if (credit == INVALID)
		return;
	
	m_Credits += credit * 10000;
}

void CStats::GetAllyAgents()
{
	int i, n;

	for (i = 0; i < 5; i++)
	{
		m_YourAliveAgents[i][0] = 0;
		n = MatchColorPattern(m_ImgWidth / 2 + agentsXAlly + i * agentsXStep, agentsY, m_PtnAgents, N_AGENTS);
		
		if (n == 19)
			n = 8;
		
		if (n != INVALID)
			sprintf_s(m_YourAliveAgents[i], 16, "agent%d", n);
	}
}

void CStats::GetEnemyAgents()
{
	int i, n;

	for (i = 0; i < 5; i++)
	{
		m_EnemyAliveAgents[i][0] = 0;
		n = MatchColorPattern(m_ImgWidth / 2 + agentsXEnemy + i * agentsXStep, agentsY, m_PtnAgents, N_AGENTS, true);
		
		if (n == 19)
			n = 8;
		
		if (n != INVALID)
			sprintf_s(m_EnemyAliveAgents[i], 16, "agent%d", n);
	}

}

void CStats::GetSkills()
{
	int x = m_ImgWidth / 2 - 1 - skillStep - (skillStep + skillWidth) / 2;
	int R = 95, G = 238, B = 184, diff = 35;
	unsigned char *p;
	
	for (int j = 0; j < 4; j++)
	{
		m_Skills[j] = 0;
		
		int f = 0;
		
		for (int i = 0; i < skillWidth; i++)
		{
			p = m_ImgPtr + (skillY*m_ImgWidth + x + i) * 3;
			
			if ((abs(B - *p) < diff) && (abs(G - *(++p)) < diff) && (abs(R - *(++p)) < diff))
			{
				if (f == 0)
				{
					m_Skills[j]++;
				}

				f = 1;
			}
			else
			{
				f = 0;
			}
		}

		x += skillStep;
	}
}

void CStats::GetWeaponchange()
{
	m_WeaponChange = MatchWhitePattern(m_ImgWidth + weaponchangeX, weaponchangeY, m_PtnWeaponChange, 1);

	if (m_WeaponChange != INVALID)
	{
		unsigned char *p;
		int c1, c2, c3;

		p = m_ImgPtr + (m_ImgWidth * weaponchangeY1 + m_ImgWidth + weaponchangeX1) * 3;
		c1 = (int)(*p) + *(p + 1) + *(p + 2);
		p = m_ImgPtr + (m_ImgWidth * weaponchangeY2 + m_ImgWidth + weaponchangeX2) * 3;
		c2 = (int)(*p) + *(p + 1) + *(p + 2);
		p = m_ImgPtr + (m_ImgWidth * weaponchangeY3 + m_ImgWidth + weaponchangeX3) * 3;
		c3 = (int)(*p) + *(p + 1) + *(p + 2);

		if (c1 < 255 * 3 - 150 && c2 < 255 * 3 - 80 && c3 < 255 * 3 - 150)
		{
			m_WeaponChange = 0;//skill
			m_WeaponId = -1;
		}
		else if (c1 >= c2 && c1 >= c3)
		{
			m_WeaponChange = 1;//automatic rifle
			m_WeaponId = MatchWhitePattern(m_ImgWidth + weaponX, weaponY1, &(m_PtnWeapon[6]), 12);
			
			if (m_WeaponId != INVALID) 
			{
				m_WeaponId += 6;
			}
		}
		else if (c2 >= c1 && c2 >= c3)
		{
			m_WeaponChange = 2;//pistol
			m_WeaponId = MatchWhitePattern(m_ImgWidth + weaponX, weaponY2, &(m_PtnWeapon[1]), 5);
			
			if (m_WeaponId != INVALID)
			{
				m_WeaponId += 1;
			}
		}
		else if (c3 >= c1 && c3 >= c2)
		{
			m_WeaponChange = 3;//knife
			m_WeaponId = 0;
		}

	}
}

void CStats::GetETC()
{
	m_Spike = MatchWhitePattern(m_ImgWidth + spikeX, spikeY, m_PtnSpike, 1);
	m_HitHeadshot = MatchColorPattern(m_ImgWidth / 2 + hitheadshotX, hitheadshotY, m_PtnHitHeadshot, 2);
	m_Otherplayer = MatchWhitePattern(otherX, otherY, m_PtnOtherplayer, 1);
}

void CStats::GetEnemy()
{
	int i, j;

	m_nEnemy = 0;
	memset(m_Enemy, 0, 5 * sizeof(ENEMYRECT));
	m_nMapEnemy = 0;
	memset(m_MapEnemy, 0, sizeof(ENEMYRECT) * 5);
	
	GetEnemyOnScreen();

	if ((m_Otherplayer != 0) && (m_Health != INVALID))
	{
		GetEnemyOnMap();

		//combine two result
		for (i = 0; i < m_nMapEnemy; i++)
		{
			if (m_MapEnemy[i].right == 0)
				continue;

			for (j = 0; j < m_nEnemy; j++)
			{
				if (m_Enemy[j].right == 0)
					continue;

				if ((m_MapEnemy[i].top <= m_Enemy[j].bottom) &&
					(m_MapEnemy[i].bottom >= m_Enemy[j].top) &&
					(m_MapEnemy[i].left <= m_Enemy[j].right) &&
					(m_MapEnemy[i].right >= m_Enemy[j].left))
				{
					break;
				}
			}

			if (j < m_nEnemy)
			{
				if ((m_Enemy[j].right - m_Enemy[j].left + 1)*(m_Enemy[j].bottom - m_Enemy[j].top + 1)<
					(m_MapEnemy[i].right - m_MapEnemy[i].left + 1)*(m_MapEnemy[i].bottom - m_MapEnemy[i].top + 1) * 2)
				{
					m_MapEnemy[i].right = (m_MapEnemy[i].right + m_Enemy[j].right) / 2;
					m_MapEnemy[i].left = (m_MapEnemy[i].left + m_Enemy[j].left) / 2;
					m_MapEnemy[i].top = MIN(m_MapEnemy[i].top, m_Enemy[j].top);
					m_MapEnemy[i].bottom = (m_MapEnemy[i].bottom + m_Enemy[j].bottom) / 2;
				}

				m_Enemy[j].right = 0;
			}
		}

		for (j = 0; j < m_nEnemy; j++)
		{
			if (m_Enemy[j].right == 0)
			{
				m_nEnemy--;
				m_Enemy[j] = m_Enemy[m_nEnemy];
				j--;
			}
		}

		for (i = 0; i < m_nMapEnemy; i++)
		{
			if (m_MapEnemy[i].right > 0)
			{
				m_Enemy[m_nEnemy] = m_MapEnemy[i];
				m_nEnemy++;
			}
		}

	}

	//set head, body, leg
	for (i = 0; i < m_nEnemy; i++)
	{
		m_Enemy[i].head_left = (m_Enemy[i].left * 65 + m_Enemy[i].right * 35) / 100;
		m_Enemy[i].head_right = (m_Enemy[i].left * 35 + m_Enemy[i].right * 65) / 100;
		m_Enemy[i].body_left = m_Enemy[i].leg_left = m_Enemy[i].left;
		m_Enemy[i].body_right = m_Enemy[i].leg_right = m_Enemy[i].right;
		m_Enemy[i].head_top = m_Enemy[i].top;
		m_Enemy[i].head_bottom = m_Enemy[i].body_top = (m_Enemy[i].top * 90 + m_Enemy[i].bottom * 10) / 100;
		m_Enemy[i].body_bottom = m_Enemy[i].leg_top = (m_Enemy[i].top * 50 + m_Enemy[i].bottom * 50) / 100;
		m_Enemy[i].leg_bottom = m_Enemy[i].bottom;

		m_Enemy[i].left -= 3; m_Enemy[i].top -= 3;
		m_Enemy[i].right += 3; m_Enemy[i].bottom += 3;
	}
}

void CStats::GetEnemyOnMap()
{
	int dxy = 50 * m_ImgHeight / 1080, oxy = 227* m_ImgHeight / 1080;
	int i, j, m;
	int ii, jj;
	int wid = oxy * 2 - dxy * 2;
	int hei = oxy - dxy + 15;
	unsigned char *pMap = new unsigned char[wid*hei],*p;

	//detect red color on map
	for (j = 0; j < hei; j++)
	{
		for (i = 0; i < wid; i++)
		{
			p = m_ImgPtr + ((dxy + j)*m_ImgWidth + dxy + i) * 3;

			if ((p[2] == 255) && (p[1] == 0) && (p[0] == 0))
				pMap[j*wid + i] = 0;
			else if ((abs((int)(p[0]) - p[1]) < 5) && ((int)(p[2]) - p[1] > 70))
				pMap[j*wid+i] = 255;
			else if ((abs((int)(p[0]) - 80) < 6) &&(abs((int)(p[1]) - 94) < 6) &&(p[2] > 200))
				pMap[j*wid + i] = 255;
			else if ((abs((int)(p[0]) - 68) < 6) &&(abs((int)(p[1]) - 82) < 6) &&(p[2] > 180))
				pMap[j*wid + i] = 255;
			else
				pMap[j*wid + i] = 0;
		}
	}

	//set detect area on map
	for (j = 0; j < hei; j++)
	{
		for (i = 0; i < wid; i++)
		{
			if (pMap[j*wid + i] == 0)
				continue;

			for (jj = 0; jj < m_PtnMapRedCircle->height; jj++)
			{
				if (j - jj < 0)
					break;

				for (ii = 0; ii < m_PtnMapRedCircle->width; ii++)
				{
					if (i - ii < 0)
						break;

					pMap[(j - jj)*wid + (i - ii)] |= 1;
				}
			}

		}
	}

	//detect red circle on map
	for (j = 0; j <= hei - m_PtnMapRedCircle->height; j++)
	{
		for (i = 0; i <= wid - m_PtnMapRedCircle->width; i++)
		{
			if (pMap[j*wid + i] == 0)
				continue;

			int cc = 0;

			for (jj = 0; jj < m_PtnMapRedCircle->height; jj++)
			{
				for (ii = 0; ii < m_PtnMapRedCircle->width; ii++)
				{
					if ((pMap[(j + jj)*wid + i + ii]>1) && (m_PtnMapRedCircle->mask[jj * m_PtnMapRedCircle->width + ii]))
						cc++;
				}
			}


			if (cc >= m_PtnMapRedCircle->width*2/3)//detect agent icon on map
			{
				ii= dxy + i + (m_PtnMapRedCircle->width - m_PtnMapAgents[0].width) / 2;
				jj= dxy + j + (m_PtnMapRedCircle->height - m_PtnMapAgents[0].height) / 2;
				
				m_MapEnemy[m_nMapEnemy].mapX = ii + (m_PtnMapAgents[0].width + 1) / 2 - oxy;
				m_MapEnemy[m_nMapEnemy].mapY = oxy - jj - (m_PtnMapAgents[0].height + 1) / 2;

				if (m_MapEnemy[m_nMapEnemy].mapY < 1)
					break;

				for (m = 0; m < m_nMapEnemy; m++)
				{
					if (abs(m_MapEnemy[m].mapX - m_MapEnemy[m_nMapEnemy].mapX) +
						abs(m_MapEnemy[m].mapY - m_MapEnemy[m_nMapEnemy].mapY) <
						(m_PtnMapRedCircle->width + m_PtnMapAgents[0].width) / 2 - 1)
					{
						break;
					}
				}

				if (m < m_nMapEnemy)
					continue;

				m = MatchMapPattern(ii, jj, m_PtnMapAgents, N_MAPAGENTS);

				if (m != INVALID)
				{
					m /= 2;
					sprintf_s(m_MapEnemy[m_nMapEnemy].agents, "agent%d", m);
					m_nMapEnemy++;
					
					if (m_nMapEnemy == 5)
						break;
				}

			}
		}

		if (m_nMapEnemy == 5) 
			break;
	}

	delete[] pMap;

	//sort by mapY
	ENEMYRECT er;
	for (i = 0; i < m_nMapEnemy - 1; i++)
	{
		for (j = i + 1; j < m_nMapEnemy; j++)
		{
			if (m_MapEnemy[i].mapY > m_MapEnemy[j].mapY)
			{
				er = m_MapEnemy[i]; m_MapEnemy[i] = m_MapEnemy[j]; m_MapEnemy[j] = er;
			}
		}
	}

	//detect enemy on screen according mapEnemy
	int k, siz;

	for (k = 0; k < m_nMapEnemy; k++)
	{
		int dx = m_MapEnemy[k].mapX;
		int dy = m_MapEnemy[k].mapY;
		int x;

		siz = m_ImgWidth/2 / dy;

		for (j = m_ImgHeight / 4; j < m_ImgHeight * 3 / 4; j++)
		{
//			x = m_ImgWidth / 2 + sqrt((m_ImgWidth * 2 / 5)*(m_ImgWidth * 2 / 5) + (j - m_ImgHeight / 2)*(j - m_ImgHeight / 2)) * (dx * 2) / 2 / dy;
			x = m_ImgWidth / 2 + (m_ImgWidth * 2 / 5) * dx / dy;
			p = m_ImgPtr + (j * m_ImgWidth) * 3;
			
			int i1, i2;
			
			i1 = MAX(0, x - siz * 3 / 2); i2 = MIN(x + siz * 3 / 2, m_ImgWidth);
			
			for (i = i1; i < i2; i++)
			{
				//avoid past enemy rect
				int kk;
				
				for (kk = 0; kk < k; kk++)
				{
					if ((i >= m_MapEnemy[kk].left) && (i <= m_MapEnemy[kk].right) &&
						(j >= m_MapEnemy[kk].top) && (j <= m_MapEnemy[kk].bottom))
					{
						break;
					}
				}
				
				if (kk < k)
					continue;

				if (((*(p + i * 3) < 80) && (*(p + i * 3 + 1) < 100) && (*(p + i * 3 + 2) > 252)) ||
					((*(p + i * 3) < 120) && (*(p + i * 3 + 1) < 120) && (*(p + i * 3 + 2) > 252) && (abs((int)(*(p + i * 3)) - *(p + i * 3 + 1)) < 4)) ||
					((abs((int)(*(p + i * 3)) - *(p + i * 3 + 1)) < 4) && (*(p + i * 3 + 2) > *(p + i * 3 + 1) * 3) && (*(p + i * 3 + 2) > 150)))
				{
					m_MapEnemy[k].top = j;
					m_MapEnemy[k].left = i - siz * 3 / 4;
					m_MapEnemy[k].right = i + siz * 3 / 4;
					m_MapEnemy[k].bottom = MIN(j + 4 * siz, m_ImgHeight - 1);
					break;
				}
			}

			if (i < i2)
			{
				break;
			}
		}
	}
}

void CStats::GetEnemyOnScreen()
{
	//enemy detect
	unsigned char *p, *pt = m_ImgPtr;
	unsigned char *pp, *pptr = new unsigned char[m_ImgWidth * m_ImgHeight * 3];
	
	memset(pptr, 0, m_ImgWidth * m_ImgHeight * 3);

	int i, j, ii, jj, tp = m_ImgHeight, bt = -1, rt = -1, lt = m_ImgWidth, cnt = 0;
	int mapSize = 440 * m_ImgHeight / 1080;
	
	for (j = 5; j < m_ImgHeight - 5; j++)
	{
		p = pt + j * m_ImgWidth * 3; p += 3 * 5;
		pp = pptr + j * m_ImgWidth * 3; pp += 3 * 5;
		
		for (i = 5; i < m_ImgWidth - 5; i++)
		{
			if ((i < mapSize) && (j < mapSize))
			{
				p += 3;
				pp += 3;
				continue; 
			}

			if ((*(p + 2) >= 254) && (*(p + 1) != 0) && (*p != 0) && (

				((*(p + 1) < 50) && (*p < 50) && (*(p + 1) < *p*1.5))
				|| ((*(p + 1) < 70) && (*p < 70) && (((*(p + 1) < *p) ? *p - *(p + 1) : *(p + 1) - *p) < 4))

				)

				)
			{
				int cc = 0, st = 2; byte d = 150; byte *zz;

				for (jj = -st; jj <= st; jj++)
				{
					for (ii = -st; ii <= st; ii++)
					{
						zz = pt + ((j + jj) * m_ImgWidth + i + ii) * 3;
						if (zz[2] < d)cc++;
						if (zz[2] / 3 * 2 < zz[0] / 2 + zz[1] / 2)cc++;
					}
				}

				if (cc >= 23)
				{
					*pp = 255;
					
					if (i > rt) rt = i;
					if (i < lt) lt = i;
					if (j < tp) tp = j;
					if (j > bt) bt = j;

					cnt++;
				}
			}

			p += 3;
			pp += 3;
		}
	}

	int r, l, t, b;
	int r1, l1, t1, b1;
	double screenWidthRate = ((double)(1080)) * m_ImgWidth / 1920 / m_ImgHeight;

	if (cnt > 1)
	{
		double rate = (double(rt - lt)) / (bt - tp) / screenWidthRate;
//			cout << "  " << rate << "  " << rt - lt << "  " << bt - tp;

		if ((rate < 0.9) || ((bt - tp < 27) && (rate < 3)))//one enemy  
		{
			m_nEnemy = 1;
			m_Enemy[0].left = lt;
			m_Enemy[0].top = tp;
			m_Enemy[0].right = rt;
			m_Enemy[0].bottom = bt;
		}
		else//two enemy
		{
			//first enemy
			r = lt; l = rt; t = bt; b = tp;
			
			for (j = tp; j <= bt; j++)
			{
				pp = pptr + (j * m_ImgWidth + lt) * 3;
				
				for (i = lt; i < (lt + rt) / 2; i++)
				{
					if (*pp)
					{
						if (i > r) r = i;
						if (i < l) l = i;
						if (j < t) t = j;
						if (j > b) b = j;
					}
					
					pp += 3;
				}
			}

			r1 = r; l1 = l; t1 = t; b1 = b;

			//second enemy
			r = lt; l = rt; t = bt; b = tp;

			for (j = tp; j <= bt; j++)
			{
				pp = pptr + (j * m_ImgWidth + (lt + rt) / 2) * 3;

				for (i = (lt + rt) / 2; i <= rt; i++)
				{
					if (*pp)
					{
						if (i > r) r = i;
						if (i < l) l = i;
						if (j < t) t = j;
						if (j > b) b = j;
					}

					pp += 3;
				}
			}

			if (l - r1 < (rt - lt)*0.1)//one enemy
			{
				if (((rt - lt)>(bt - tp) * 4) && (bt - tp < 10))
				{
					m_nEnemy = 0;
				}
				else
				{
					m_nEnemy = 1;
					m_Enemy[0].left = lt;
					m_Enemy[0].top = tp;
					m_Enemy[0].right = rt;
					m_Enemy[0].bottom = bt;
				}
			}
			else//two enemy
			{
				if ((b1 == t1) || (((r1 - l1) > (b1 - t1) * 4) && (b1 - t1 < 15)))
				{
					m_nEnemy = 0;
				}
				else
				{
					m_nEnemy = 1;
					m_Enemy[0].left = l1;
					m_Enemy[0].top = t1;
					m_Enemy[0].right = r1;
					m_Enemy[0].bottom = b1;
				}

				if (!((b == t) || (((r - l) > (b - t) * 4) && (b - t < 15))))
				{
					m_Enemy[m_nEnemy].left = l;
					m_Enemy[m_nEnemy].top = t;
					m_Enemy[m_nEnemy].right = r;
					m_Enemy[m_nEnemy].bottom = b;
					m_nEnemy++;
				}
			}
		}
	}

	//recheck rect
	for (i = 0; i < m_nEnemy; i++)
	{
		r = (int)MIN(m_ImgWidth, m_Enemy[i].right + MAX((m_Enemy[i].right - m_Enemy[i].left)*0.2, 5));
		l = (int)MAX(0, m_Enemy[i].left - MAX((m_Enemy[i].right - m_Enemy[i].left)*0.2, 5));
		t = (int)MAX(0, tp - MAX((m_Enemy[i].bottom - m_Enemy[i].top)*0.2, 30));
		b = (int)MIN(m_ImgHeight, m_Enemy[i].bottom + MAX((m_Enemy[i].bottom - m_Enemy[i].top)*0.2, 30));

		for (j = t; j < b; j++)
		{
			p = pt + (j * m_ImgWidth + l) * 3;
			pp = pptr + (j * m_ImgWidth + l) * 3;

			for (int k = l; k < r; k++)
			{
				if ((*(p + 2) >= 254) && (*(p + 1) != 0) && (*p != 0) && (
					((*(p + 1) < 50) && (*p < 50))
					|| ((*(p + 1) < 120) && (*p < 120) && (((*(p + 1) < *p) ? *p - *(p + 1) : *(p + 1) - *p) < 4))

					)
					)
				{
					*pp = 255;

					if (k > m_Enemy[i].right) m_Enemy[i].right = k;
					if (k < m_Enemy[i].left) m_Enemy[i].left = k;
					if (j < m_Enemy[i].top) m_Enemy[i].top = j;
					if (j > m_Enemy[i].bottom) m_Enemy[i].bottom = j;
				}

				p += 3;
				pp += 3;
			}
		}

		m_Enemy[i].bottom = (int)MIN(m_ImgHeight, m_Enemy[i].bottom + (m_Enemy[i].bottom - m_Enemy[i].top)*0.3);
	}

	delete[]pptr;
}

int CStats::MatchWhitePattern(int x, int y, WHITE_PATTERN *ptn, int nPtn)
{
	int rtn;
	
	rtn = MatchWhitePattern_(x, y, ptn, nPtn);
	
	if (rtn == INVALID)rtn = MatchWhitePattern_(x - 1, y, ptn, nPtn);
	if (rtn == INVALID)rtn = MatchWhitePattern_(x + 1, y, ptn, nPtn);

	return rtn;
}

int CStats::MatchWhitePattern_(int x, int y, WHITE_PATTERN *ptn, int nPtn)
{
	int wid = ptn[0].width, hei = ptn[0].height;
	unsigned int *blackImg = new unsigned int[wid*hei];
	unsigned char *p;
	unsigned int *pi = blackImg;
	unsigned int avgBlackColor=0;
	unsigned int maxRGB=0;
	int i, j, n;

	for (j = 0; j < hei; j++)
	{
		if (y + j >= m_ImgHeight)
		{
			delete[]blackImg;
			return INVALID; 
		}
		
		for ( i = 0; i < wid; i++)
		{
			if (x + i >= m_ImgWidth) 
			{ 
				delete[]blackImg; 
				return INVALID; 
			}

			p = m_ImgPtr + (m_ImgWidth*(y + j) + (x + i)) * 3;
			avgBlackColor += *p; *pi = *p; p++;
			avgBlackColor += *p; *pi += *p; p++;
			avgBlackColor += *p; *pi += *p; 

			if (maxRGB < *pi)
			{
				maxRGB = *pi;
			}

			pi++;
		}
	}

	avgBlackColor /= (wid*hei); avgBlackColor += 9;

	if (avgBlackColor >= 253 * 3)
	{
		avgBlackColor = 253 * 3 - 1;
	}
//	if (avgBlackColor < maxRGB-15)avgBlackColor = maxRGB - 15;//game

	for (i = 0; i < wid*hei; i++)
	{
		if (blackImg[i] > avgBlackColor)
			blackImg[i] = 1;
		else
			blackImg[i] = 0;
	}

	for (n = nPtn-1; n >=0; n--)
	{
		for (i = 0; i < wid*hei; i++)
		{
			if (ptn[n].mask[i] == 0)
				continue;

			if (blackImg[i] == 0)
				break;
		}

		if (i == wid*hei) 
		{ 
			delete[]blackImg; 
			return n; 
		}
	}

	delete[]blackImg;

	return INVALID;
}

int CStats::MatchColorPattern(int x, int y, COLOR_PATTERN *ptn, int nPtn, bool flip)
{
	int rtn;

	rtn = MatchColorPattern_(x, y, ptn, nPtn, flip);

	if (rtn == INVALID)
		rtn = MatchColorPattern_(x - 1, y, ptn, nPtn, flip);

	if (rtn == INVALID)
		rtn = MatchColorPattern_(x + 1, y, ptn, nPtn, flip);

	return rtn;
}

int CStats::MatchColorPattern_(int x, int y, COLOR_PATTERN *ptn, int nPtn, bool flip)
{
	int wid = ptn[0].width, hei = ptn[0].height;
	int diff = hei*wid * 3 * 300;
	unsigned char *p;
	int i, j, n, ii, jj;
	int idx;
	int total, t, tt;

	for (n = 0; n <nPtn; n++)
	{
		if ((ptn[n].B[0] == 255) && (ptn[n].G[0] == 255) && (ptn[n].R[0] == 255))
		{
			continue;
		}

		total = 0;
		
		for (j = 0; j < hei; j++)
		{
			for (i = 0; i < wid; i++)
			{
				if (flip)
					idx = (j + 1)*wid - 1 - i;
				else
					idx = j*wid + i;

				t = 0xfffffff;

				for (jj = -1; jj <= 1; jj++)
				{
					for (ii = -1; ii <= 1; ii++)
					{
						if (y + j + jj >= m_ImgHeight) return INVALID;
						if (y + j + jj < 0) return INVALID;
						if (x + i + ii >= m_ImgWidth) return INVALID;
						if (x + i + ii < 0) return INVALID;

						p = m_ImgPtr + (m_ImgWidth * (y + j + jj) + (x + i + ii)) * 3;
						tt = ((ptn[n].B[idx] > (*p)) ? ptn[n].B[idx] - (*p) : (*p) - ptn[n].B[idx])*
							((ptn[n].B[idx] > (*p)) ? ptn[n].B[idx] - (*p) : (*p) - ptn[n].B[idx]);
						p++;
						tt += ((ptn[n].G[idx] > (*p)) ? ptn[n].G[idx] - (*p) : (*p) - ptn[n].G[idx]) *
							((ptn[n].G[idx] > (*p)) ? ptn[n].G[idx] - (*p) : (*p) - ptn[n].G[idx]);
						p++;
						tt += ((ptn[n].R[idx] > (*p)) ? ptn[n].R[idx] - (*p) : (*p) - ptn[n].R[idx])*
							((ptn[n].R[idx] > (*p)) ? ptn[n].R[idx] - (*p) : (*p) - ptn[n].R[idx]);

						if (tt < t)
							t = tt;
					}
				}

				total += t;
				
				if (total >= diff)
					break;
			}

			if (i < wid)
				break;
		}

		if (j == hei)
			return n;
	}

	return INVALID;
}

int CStats::MatchMapPattern(int x, int y, COLOR_PATTERN *ptn, int nPtn)//detect enemy on map
{
	int wid = ptn[0].width, hei = ptn[0].height;
	unsigned char *p;
	int i, j, n, d, dd, diff = 350 * hei*wid * 3;
	int idx;

	for (n = 0; n < nPtn; n++)
	{
		if ((ptn[n].B[0] == 255) && (ptn[n].G[0] == 255) && (ptn[n].R[0] == 255))
		{
			continue;
		}

		d = 0;
		for (j = 0; j < hei; j++)
		{
			for (i = 0; i < wid; i++)
			{
				p = m_ImgPtr + (m_ImgWidth * (y + j) + (x + i)) * 3;
				idx = j*wid + i;

				dd = abs((int)(ptn[n].B[idx])-(*p));
				dd = MIN(dd, abs((int)(ptn[n].B[idx]) - (*(p + 3))));
				dd = MIN(dd, abs((int)(ptn[n].B[idx]) - (*(p + m_ImgWidth * 3))));
				dd = MIN(dd, abs((int)(ptn[n].B[idx]) - (*(p + m_ImgWidth * 3 + 3))));
				d += dd*dd;

				if (d > diff)
					break;

				p++;
				dd = abs((int)(ptn[n].G[idx]) - (*p));
				dd = MIN(dd, abs((int)(ptn[n].G[idx]) - (*(p + 3))));
				dd = MIN(dd, abs((int)(ptn[n].G[idx]) - (*(p + m_ImgWidth * 3))));
				dd = MIN(dd, abs((int)(ptn[n].G[idx]) - (*(p + m_ImgWidth * 3 + 3))));
				d += dd*dd;

				if (d > diff)
					break;

				p++;
				dd = abs((int)(ptn[n].R[idx]) - (*p));
				dd = MIN(dd, abs((int)(ptn[n].R[idx]) - (*(p + 3))));
				dd = MIN(dd, abs((int)(ptn[n].R[idx]) - (*(p + m_ImgWidth * 3))));
				dd = MIN(dd, abs((int)(ptn[n].R[idx]) - (*(p + m_ImgWidth * 3 + 3))));
				d += dd*dd;

				if (d > diff)
					break;
			}

			if (i < wid)
				break;
		}

		if (j == hei)
			return n;
	}

	return INVALID;
}
