#include "pch.h"
#include "ParserEngine.h"


CParserEngine::CParserEngine()
	: m_nImageWidth(0)
	, m_nImageHeight (0)
	, m_pImageBufferPtr (nullptr)
{
	ZeroMemory(m_PtnScore, sizeof(WHITE_PATTERN) * 10);
	ZeroMemory(m_PtnTime, sizeof(WHITE_PATTERN) * 10);
	ZeroMemory(m_PtnAmmo, sizeof(WHITE_PATTERN) * 10);
	ZeroMemory(m_PtnArmor, sizeof(WHITE_PATTERN) * 10);
	ZeroMemory(m_PtnCredits, sizeof(WHITE_PATTERN) * 10);
	ZeroMemory(m_PtnAgents, sizeof(COLOR_PATTERN) * N_AGENTS);
	ZeroMemory(m_PtnWeaponChange, sizeof(WHITE_PATTERN) * 1);
	ZeroMemory(m_PtnSpike, sizeof(WHITE_PATTERN) * 1);
	ZeroMemory(m_PtnHitHeadShot, sizeof(COLOR_PATTERN) * 2);
	ZeroMemory(m_PtnOtherPlayer, sizeof(WHITE_PATTERN) * 1);
	ZeroMemory(m_PtnMapAgents, sizeof(COLOR_PATTERN) * N_MAPAGENTS);
	ZeroMemory(m_PtnMapRedCircle, sizeof(WHITE_PATTERN) * 1);
	ZeroMemory(m_PtnWeapon, sizeof(WHITE_PATTERN) * 18);

	m_allyScoreY = 0; 
	m_allyScoreX1 = 0; 
	m_allyScoreX10 = 0; 
	m_allyScoreX01 = 0;
	
	m_enemyScoreY = 0; 
	m_enemyScoreX1 = 0; 
	m_enemyScoreX10 = 0; 
	m_enemyScoreX01 = 0;

	m_timeY = 0; 
	m_timeX100 = 0; 
	m_timeX010 = 0; 
	m_timeX001 = 0;

	m_ammoY = 0; 
	m_ammoX100 = 0; 
	m_ammoX010 = 0; 
	m_ammoX001 = 0;

	m_healthY = 0; 
	m_healthX100 = 0; 
	m_healthX010 = 0; 
	m_healthX001 = 0; 
	m_healthX10 = 0; 
	m_healthX01 = 0; 
	m_healthX1 = 0;

	m_armorY = 0; 
	m_armorX10 = 0; 
	m_armorX01 = 0; 
	m_armorX1 = 0;

	m_ammoleftY = 0; 
	m_ammoleftX100 = 0; 
	m_ammoleftX010 = 0; 
	m_ammoleftX001 = 0;

	m_creditsY = 0; 
	m_creditsX1 = 0; 
	m_creditsX10 = 0; 
	m_creditsX100 = 0; 
	m_creditsX1000 = 0; 
	m_creditsX10000 = 0; 
	m_creditsX100000 = 0;

	m_agentsY = 0; 
	m_agentsXAlly = 0; 
	m_agentsXEnemy = 0; 
	m_agentsXStep = 0;

	m_weaponchangeY = 0; 
	m_weaponchangeX = 0; 
	m_weaponchangeY1 = 0; 
	m_weaponchangeX1 = 0; 
	m_weaponchangeY2 = 0; 
	m_weaponchangeX2 = 0; 
	m_weaponchangeY3 = 0; 
	m_weaponchangeX3 = 0;

	m_spikeY = 0; 
	m_spikeX = 0; 
	m_hitheadshotY = 0; 
	m_hitheadshotX = 0; 
	m_otherY = 0; 
	m_otherX = 0;

	m_skillY = 0; 
	m_skillStep = 0; 
	m_skillWidth = 0;

	m_weaponX = 0; 
	m_weaponY1 = 0; 
	m_weaponY2 = 0; 
	m_weaponY3 = 0;
}

CParserEngine::~CParserEngine()
{
}

CTimelineData CParserEngine::run(unsigned char* imgPtr_, int imgWidth_, int imgHeight_)
{
	CTimelineData *pResult = new CTimelineData();

	if (!Init(imgWidth_, imgHeight_))
		return *pResult;

	m_pImageBufferPtr = imgPtr_;

	// GameState
	pResult->m_clsGameState.m_yourTeamScore = GetYourTeamScore();
	pResult->m_clsGameState.m_enemyTeamScore = GetEnemyTeamScore();
	pResult->m_clsGameState.m_round = pResult->m_clsGameState.m_yourTeamScore + pResult->m_clsGameState.m_enemyTeamScore + 1;
	pResult->m_clsGameState.m_roundTime = GetRoundTime();
	pResult->m_clsGameState.m_aryYourAgents = GetAllyAgents();
	pResult->m_clsGameState.m_aryEnemyAgents = GetEnemyAgents();
	
	// PlayerState
	pResult->m_clsPlayerState.m_health = GetHealth();

	int nOtherPlayer = GetIsOtherPlayer();

	if (nOtherPlayer != 0 && pResult->m_clsPlayerState.m_health > 0)
		pResult->m_clsPlayerState.m_isAlive = true;

	pResult->m_clsPlayerState.m_armor = GetArmor();
	pResult->m_clsPlayerState.m_ammo = GetAmmo();
	pResult->m_clsPlayerState.m_ammoLeft = GetAmmoLeft();
	pResult->m_clsPlayerState.m_hasSpike = GetHasSpike();
	pResult->m_clsPlayerState.m_credits = GetCredits();
	pResult->m_clsPlayerState.m_weaponId = GetWeaponId();
	
	GetAbilityLeft(pResult->m_clsPlayerState.m_aryAbilityLeft);

	return *pResult;
}

int8_t CParserEngine::GetYourTeamScore()
{
	int8_t u8YourTeamScore = 0;
	int ally, ally1, ally2;

	ally = MatchWhitePattern(m_nImageWidth / 2 + m_allyScoreX1, m_allyScoreY, m_PtnScore, 10);
	ally1 = MatchWhitePattern(m_nImageWidth / 2 + m_allyScoreX10, m_allyScoreY, m_PtnScore, 10);
	ally2 = MatchWhitePattern(m_nImageWidth / 2 + m_allyScoreX01, m_allyScoreY, m_PtnScore, 10);

	if ((ally == INVALID) && (ally1 != INVALID) && (ally2 != INVALID))
		u8YourTeamScore  = ally1 * 10 + ally2;
	else if ((ally != INVALID) && (ally1 == INVALID) && (ally2 == INVALID))
		u8YourTeamScore = ally;
	else if ((ally == INVALID) && (ally1 == INVALID) && (ally2 == INVALID))
		u8YourTeamScore = INVALID;
	else
		u8YourTeamScore = MATCH_ERROR;

	return u8YourTeamScore;
}

int8_t CParserEngine::GetEnemyTeamScore()
{
	int8_t u8EnemyTeamScore = 0;
	int enemy, enemy1, enemy2;
	enemy = MatchWhitePattern(m_nImageWidth / 2 + m_enemyScoreX1, m_enemyScoreY, m_PtnScore, 10);
	enemy1 = MatchWhitePattern(m_nImageWidth / 2 + m_enemyScoreX10, m_enemyScoreY, m_PtnScore, 10);
	enemy2 = MatchWhitePattern(m_nImageWidth / 2 + m_enemyScoreX01, m_enemyScoreY, m_PtnScore, 10);

	if ((enemy == INVALID) && (enemy1 != INVALID) && (enemy2 != INVALID))
		u8EnemyTeamScore = enemy1 * 10 + enemy2;
	else if ((enemy != INVALID) && (enemy1 == INVALID) && (enemy2 == INVALID))
		u8EnemyTeamScore = enemy;
	else if ((enemy == INVALID) && (enemy1 == INVALID) && (enemy == INVALID))
		u8EnemyTeamScore = INVALID;
	else
		u8EnemyTeamScore = MATCH_ERROR;

	return u8EnemyTeamScore;
}

int8_t CParserEngine::GetRoundTime()
{
	int8_t u8RoundTime = 0;
	int time1, time2, time3;

	time1 = MatchWhitePattern(m_nImageWidth / 2 + m_timeX100, m_timeY, m_PtnTime, 2);
	time2 = MatchWhitePattern(m_nImageWidth / 2 + m_timeX010, m_timeY, m_PtnTime, 6);
	time3 = MatchWhitePattern(m_nImageWidth / 2 + m_timeX001, m_timeY, m_PtnTime, 10);

	if ((time1 != INVALID) && (time2 != INVALID) && (time3 != INVALID))
		u8RoundTime = time1 * 60 + time2 * 10 + time3;
	else if ((time1 == INVALID) && (time2 == INVALID) && (time3 == INVALID))
		u8RoundTime = INVALID;
	else
		u8RoundTime = MATCH_ERROR;

	return u8RoundTime;
}

int8_t CParserEngine::GetHealth()
{
	int8_t u8Health = 0;
	int health1, health2, health3, health4, health5, health6;

	health1 = MatchWhitePattern(m_nImageWidth / 2 + m_healthX100, m_healthY, m_PtnAmmo, 2);
	health2 = MatchWhitePattern(m_nImageWidth / 2 + m_healthX010, m_healthY, m_PtnAmmo, 10);
	health3 = MatchWhitePattern(m_nImageWidth / 2 + m_healthX001, m_healthY, m_PtnAmmo, 10);

	health4 = MatchWhitePattern(m_nImageWidth / 2 + m_healthX10, m_healthY, m_PtnAmmo, 10);
	health5 = MatchWhitePattern(m_nImageWidth / 2 + m_healthX01, m_healthY, m_PtnAmmo, 10);

	health6 = MatchWhitePattern(m_nImageWidth / 2 + m_healthX1, m_healthY, m_PtnAmmo, 10);

	if ((health1 != INVALID) && (health2 != INVALID) && (health3 != INVALID) && (health4 == INVALID) && (health5 == INVALID) && (health6 == INVALID))
		u8Health = health1 * 100 + health2 * 10 + health3;
	else if ((health1 == INVALID) && (health2 == INVALID) && (health3 == INVALID) && (health4 != INVALID) && (health5 != INVALID) && (health6 == INVALID))
		u8Health = health4 * 10 + health5;
	else if ((health1 == INVALID) && (health2 == INVALID) && (health3 == INVALID) && (health4 == INVALID) && (health5 == INVALID) && (health6 != INVALID))
		u8Health = health6;
	else if ((health1 == INVALID) && (health2 == INVALID) && (health3 == INVALID) && (health4 == INVALID) && (health5 == INVALID) && (health6 == INVALID))
		u8Health = INVALID;
	else
		u8Health = MATCH_ERROR;

	return u8Health;
}

int CParserEngine::GetIsOtherPlayer()
{
	int u8IsOtherPlayer = 0;

	u8IsOtherPlayer = MatchWhitePattern(m_otherX, m_otherY, m_PtnOtherPlayer, 1);

	return u8IsOtherPlayer;
}

int8_t CParserEngine::GetArmor()
{
	int8_t u8Armor = 0;
	int armor1, armor2, armor3;

	armor1 = MatchWhitePattern(m_nImageWidth / 2 + m_armorX10, m_armorY, m_PtnArmor, 10);
	armor2 = MatchWhitePattern(m_nImageWidth / 2 + m_armorX01, m_armorY, m_PtnArmor, 10);
	armor3 = MatchWhitePattern(m_nImageWidth / 2 + m_armorX1, m_armorY, m_PtnArmor, 10);

	if ((armor1 != INVALID) && (armor2 != INVALID) && (armor3 == INVALID))
		u8Armor = armor1 * 10 + armor2;
	else if ((armor1 == INVALID) && (armor2 == INVALID) && (armor3 != INVALID))
		u8Armor = armor3;
	else if ((armor1 == INVALID) && (armor2 == INVALID) && (armor3 == INVALID))
		u8Armor = INVALID;
	else
		u8Armor = MATCH_ERROR;

	return u8Armor;
}

int16_t CParserEngine::GetAmmo()
{
	int16_t u16Ammo = 0;
	int ammo0, ammo1, ammo2;

	ammo0 = MatchWhitePattern(m_nImageWidth / 2 + m_ammoX100, m_ammoY, m_PtnAmmo, 3);
	ammo1 = MatchWhitePattern(m_nImageWidth / 2 + m_ammoX010, m_ammoY, m_PtnAmmo, 10);
	ammo2 = MatchWhitePattern(m_nImageWidth / 2 + m_ammoX001, m_ammoY, m_PtnAmmo, 10);

	if ((ammo0 != INVALID) && (ammo1 != INVALID) && (ammo2 != INVALID))
		u16Ammo = ammo0 * 100 + ammo1 * 10 + ammo2;
	else if ((ammo0 == INVALID) && (ammo1 != INVALID) && (ammo2 != INVALID))
		u16Ammo = ammo1 * 10 + ammo2;
	else if ((ammo0 == INVALID) && (ammo1 == INVALID) && (ammo2 != INVALID))
		u16Ammo = ammo2;
	else if ((ammo0 == INVALID) && (ammo1 == INVALID) && (ammo2 == INVALID))
		u16Ammo = INVALID;
	else
		u16Ammo = MATCH_ERROR;

	return u16Ammo;
}

int16_t CParserEngine::GetAmmoLeft()
{
	int16_t u16AmmoLeft = 0;
	int ammoleft1, ammoleft2, ammoleft3;

	ammoleft1 = MatchWhitePattern(m_nImageWidth / 2 + m_ammoleftX100, m_ammoleftY, m_PtnArmor, 10);
	ammoleft2 = MatchWhitePattern(m_nImageWidth / 2 + m_ammoleftX010, m_ammoleftY, m_PtnArmor, 10);
	ammoleft3 = MatchWhitePattern(m_nImageWidth / 2 + m_ammoleftX001, m_ammoleftY, m_PtnArmor, 10);

	if ((ammoleft1 != INVALID) && (ammoleft2 == INVALID) && (ammoleft3 == INVALID))
		u16AmmoLeft = ammoleft1;
	else if ((ammoleft1 != INVALID) && (ammoleft2 != INVALID) && (ammoleft3 == INVALID))
		u16AmmoLeft = ammoleft1 * 10 + ammoleft2;
	else if ((ammoleft1 != INVALID) && (ammoleft2 != INVALID) && (ammoleft3 != INVALID))
		u16AmmoLeft = ammoleft1 * 100 + ammoleft2 * 10 + ammoleft3;
	else if ((ammoleft1 == INVALID) && (ammoleft2 == INVALID) && (ammoleft3 == INVALID))
		u16AmmoLeft = INVALID;
	else
		u16AmmoLeft = MATCH_ERROR;

	return u16AmmoLeft;
}

int16_t CParserEngine::GetCredits()
{
	int16_t u16Credits = 0;
	int credit;

	u16Credits = INVALID;

	credit = MatchWhitePattern(m_nImageWidth + m_creditsX1, m_creditsY, m_PtnCredits, 10);

	if (credit == INVALID)
		return u16Credits;

	u16Credits = credit;
	credit = MatchWhitePattern(m_nImageWidth + m_creditsX10, m_creditsY, m_PtnCredits, 10);

	if (credit == INVALID)
		return u16Credits;

	u16Credits += credit * 10;
	credit = MatchWhitePattern(m_nImageWidth + m_creditsX100, m_creditsY, m_PtnCredits, 10);

	if (credit == INVALID)
		return u16Credits;

	u16Credits += credit * 100;
	credit = MatchWhitePattern(m_nImageWidth + m_creditsX1000, m_creditsY, m_PtnCredits, 10);

	if (credit == INVALID)
		return u16Credits;

	u16Credits += credit * 1000;
	credit = MatchWhitePattern(m_nImageWidth + m_creditsX10000, m_creditsY, m_PtnCredits, 10);

	if (credit == INVALID)
		return u16Credits;

	u16Credits += credit * 10000;

	return u16Credits;
}

int8_t CParserEngine::GetHasSpike()
{
	return MatchWhitePattern(m_nImageWidth + m_spikeX, m_spikeY, m_PtnSpike, 1);
}

int8_t CParserEngine::GetWeaponId()
{
	int8_t u8WeaponId = -1;
	int nWeaponChange;

	nWeaponChange = MatchWhitePattern(m_nImageWidth + m_weaponchangeX, m_weaponchangeY, m_PtnWeaponChange, 1);

	if (nWeaponChange != INVALID)
	{
		unsigned char* p;
		int c1, c2, c3;

		p = m_pImageBufferPtr + (m_nImageWidth * m_weaponchangeY1 + m_nImageWidth + m_weaponchangeX1) * 3;
		c1 = (int)(*p) + *(p + 1) + *(p + 2);
		p = m_pImageBufferPtr + (m_nImageWidth * m_weaponchangeY2 + m_nImageWidth + m_weaponchangeX2) * 3;
		c2 = (int)(*p) + *(p + 1) + *(p + 2);
		p = m_pImageBufferPtr + (m_nImageWidth * m_weaponchangeY3 + m_nImageWidth + m_weaponchangeX3) * 3;
		c3 = (int)(*p) + *(p + 1) + *(p + 2);

		if (c1 < 255 * 3 - 150 && c2 < 255 * 3 - 80 && c3 < 255 * 3 - 150)
		{
			//skill
			u8WeaponId = -1;
		}
		else if (c1 >= c2 && c1 >= c3)
		{
			//automatic rifle
			u8WeaponId = MatchWhitePattern(m_nImageWidth + m_weaponX, m_weaponY1, &(m_PtnWeapon[6]), 12);

			if (u8WeaponId != INVALID)
			{
				u8WeaponId += 6;
			}
		}
		else if (c2 >= c1 && c2 >= c3)
		{
			//pistol
			u8WeaponId = MatchWhitePattern(m_nImageWidth + m_weaponX, m_weaponY2, &(m_PtnWeapon[1]), 5);

			if (u8WeaponId != INVALID)
			{
				u8WeaponId += 1;
			}
		}
		else if (c3 >= c1 && c3 >= c2)
		{
			//knife
			u8WeaponId = 0;
		}
	}

	return u8WeaponId;
}

void CParserEngine::GetAbilityLeft(int8_t* pAbilityInfo)
{
	int x = m_nImageWidth / 2 - 1 - m_skillStep - (m_skillStep + m_skillWidth) / 2;
	int R = 95, G = 238, B = 184, diff = 35;
	unsigned char* p;

	for (int j = 0; j < MAX_ABILITY; j++)
	{
		pAbilityInfo[j] = 0;

		int f = 0;

		for (int i = 0; i < m_skillWidth; i++)
		{
			p = m_pImageBufferPtr + (m_skillY * m_nImageWidth + x + i) * 3;

			if ((abs(B - *p) < diff) && (abs(G - *(++p)) < diff) && (abs(R - *(++p)) < diff))
			{
				if (f == 0)
				{
					pAbilityInfo[j]++;
				}

				f = 1;
			}
			else
			{
				f = 0;
			}
		}

		x += m_skillStep;
	}
}

std::vector<std::string> CParserEngine::GetAllyAgents()
{
	std::vector<std::string> agentList;

	for (int i = 0; i < 5; i++)
	{
		int n = MatchColorPattern(m_nImageWidth / 2 + m_agentsXAlly + i * m_agentsXStep, m_agentsY, m_PtnAgents, N_AGENTS);

		if (n == 19)
			n = 8;

		if (n != INVALID)
		{
			char szAliveAgent[MAX_PATH] = { 0 };

			sprintf_s(szAliveAgent, "agent%d", n);
			agentList.push_back(szAliveAgent);
		}
	}

	return agentList;
}

std::vector<std::string> CParserEngine::GetEnemyAgents()
{
	std::vector<std::string> agentList;

	for (int i = 0; i < 5; i++)
	{
		int n = MatchColorPattern(m_nImageWidth / 2 + m_agentsXEnemy + i * m_agentsXStep, m_agentsY, m_PtnAgents, N_AGENTS, true);

		if (n == 19)
			n = 8;

		if (n != INVALID)
		{
			char szAliveAgent[MAX_PATH] = { 0 };

			sprintf_s(szAliveAgent, "agent%d", n);
			agentList.push_back(szAliveAgent);
		}
	}

	return agentList;
}

BOOL CParserEngine::Init(int imgWidth_, int imgHeight_)
{
	FILE* fp;
	char fn[MAX_PATH];

	m_nImageWidth = imgWidth_;

	if (m_nImageHeight == imgHeight_)
		return TRUE;

	m_nImageHeight = imgHeight_;

	sprintf_s(fn, "ptn%d.dat", m_nImageHeight);

	if (fopen_s(&fp, fn, "rb"))
	{
		m_nImageHeight = 0;
		return FALSE;
	}

	fread(m_PtnScore, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnTime, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnAmmo, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnArmor, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnCredits, 10, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnAgents, N_AGENTS, sizeof(COLOR_PATTERN), fp);
	fread(m_PtnWeaponChange, 1, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnSpike, 1, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnHitHeadShot, 2, sizeof(COLOR_PATTERN), fp);
	fread(m_PtnOtherPlayer, 1, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnMapAgents, N_MAPAGENTS, sizeof(COLOR_PATTERN), fp);
	fread(m_PtnMapRedCircle, 1, sizeof(WHITE_PATTERN), fp);
	fread(m_PtnWeapon, 18, sizeof(WHITE_PATTERN), fp);

	fclose(fp);

	char strApp[MAX_PATH];

	sprintf_s(strApp, "%d", m_nImageHeight);

	m_allyScoreY = GetPrivateProfileIntA(strApp, "allyScoreY", 0, POS_INI);
	m_allyScoreX1 = GetPrivateProfileIntA(strApp, "allyScoreX1", 0, POS_INI);
	m_allyScoreX10 = GetPrivateProfileIntA(strApp, "allyScoreX10", 0, POS_INI);
	m_allyScoreX01 = GetPrivateProfileIntA(strApp, "allyScoreX01", 0, POS_INI);
	m_enemyScoreY = GetPrivateProfileIntA(strApp, "enemyScoreY", 0, POS_INI);
	m_enemyScoreX1 = GetPrivateProfileIntA(strApp, "enemyScoreX1", 0, POS_INI);
	m_enemyScoreX10 = GetPrivateProfileIntA(strApp, "enemyScoreX10", 0, POS_INI);
	m_enemyScoreX01 = GetPrivateProfileIntA(strApp, "enemyScoreX01", 0, POS_INI);

	m_timeY = GetPrivateProfileIntA(strApp, "timeY", 0, POS_INI);
	m_timeX100 = GetPrivateProfileIntA(strApp, "timeX100", 0, POS_INI);
	m_timeX010 = GetPrivateProfileIntA(strApp, "timeX010", 0, POS_INI);
	m_timeX001 = GetPrivateProfileIntA(strApp, "timeX001", 0, POS_INI);

	m_ammoY = GetPrivateProfileIntA(strApp, "ammoY", 0, POS_INI);
	m_ammoX100 = GetPrivateProfileIntA(strApp, "ammoX100", 0, POS_INI);
	m_ammoX010 = GetPrivateProfileIntA(strApp, "ammoX010", 0, POS_INI);
	m_ammoX001 = GetPrivateProfileIntA(strApp, "ammoX001", 0, POS_INI);

	m_healthY = GetPrivateProfileIntA(strApp, "healthY", 0, POS_INI);
	m_healthX100 = GetPrivateProfileIntA(strApp, "healthX100", 0, POS_INI);
	m_healthX010 = GetPrivateProfileIntA(strApp, "healthX010", 0, POS_INI);
	m_healthX001 = GetPrivateProfileIntA(strApp, "healthX001", 0, POS_INI);
	m_healthX10 = GetPrivateProfileIntA(strApp, "healthX10", 0, POS_INI);
	m_healthX01 = GetPrivateProfileIntA(strApp, "healthX01", 0, POS_INI);
	m_healthX1 = GetPrivateProfileIntA(strApp, "healthX1", 0, POS_INI);

	m_armorY = GetPrivateProfileIntA(strApp, "armorY", 0, POS_INI);
	m_armorX10 = GetPrivateProfileIntA(strApp, "armorX10", 0, POS_INI);
	m_armorX01 = GetPrivateProfileIntA(strApp, "armorX01", 0, POS_INI);
	m_armorX1 = GetPrivateProfileIntA(strApp, "armorX1", 0, POS_INI);

	m_ammoleftY = GetPrivateProfileIntA(strApp, "ammoleftY", 0, POS_INI);
	m_ammoleftX100 = GetPrivateProfileIntA(strApp, "ammoleftX100", 0, POS_INI);
	m_ammoleftX010 = GetPrivateProfileIntA(strApp, "ammoleftX010", 0, POS_INI);
	m_ammoleftX001 = GetPrivateProfileIntA(strApp, "ammoleftX001", 0, POS_INI);

	m_creditsY = GetPrivateProfileIntA(strApp, "creditsY", 0, POS_INI);
	m_creditsX1 = GetPrivateProfileIntA(strApp, "creditsX1", 0, POS_INI);
	m_creditsX10 = GetPrivateProfileIntA(strApp, "creditsX10", 0, POS_INI);
	m_creditsX100 = GetPrivateProfileIntA(strApp, "creditsX100", 0, POS_INI);
	m_creditsX1000 = GetPrivateProfileIntA(strApp, "creditsX1000", 0, POS_INI);
	m_creditsX10000 = GetPrivateProfileIntA(strApp, "creditsX10000", 0, POS_INI);

	m_agentsY = GetPrivateProfileIntA(strApp, "agentsY", 0, POS_INI);
	m_agentsXAlly = GetPrivateProfileIntA(strApp, "agentsXAlly", 0, POS_INI);
	m_agentsXEnemy = GetPrivateProfileIntA(strApp, "agentsXEnemy", 0, POS_INI);
	m_agentsXStep = GetPrivateProfileIntA(strApp, "agentsXStep", 0, POS_INI);

	m_weaponchangeY = GetPrivateProfileIntA(strApp, "weaponchangeY", 0, POS_INI);
	m_weaponchangeX = GetPrivateProfileIntA(strApp, "weaponchangeX", 0, POS_INI);
	m_weaponchangeY1 = GetPrivateProfileIntA(strApp, "weaponchangeY1", 0, POS_INI);
	m_weaponchangeX1 = GetPrivateProfileIntA(strApp, "weaponchangeX1", 0, POS_INI);
	m_weaponchangeY2 = GetPrivateProfileIntA(strApp, "weaponchangeY2", 0, POS_INI);
	m_weaponchangeX2 = GetPrivateProfileIntA(strApp, "weaponchangeX2", 0, POS_INI);
	m_weaponchangeY3 = GetPrivateProfileIntA(strApp, "weaponchangeY3", 0, POS_INI);
	m_weaponchangeX3 = GetPrivateProfileIntA(strApp, "weaponchangeX3", 0, POS_INI);
	m_weaponX = GetPrivateProfileIntA(strApp, "weaponX", 0, POS_INI);
	m_weaponY1 = GetPrivateProfileIntA(strApp, "weaponY1", 0, POS_INI);
	m_weaponY2 = GetPrivateProfileIntA(strApp, "weaponY2", 0, POS_INI);
	m_weaponY3 = GetPrivateProfileIntA(strApp, "weaponY3", 0, POS_INI);

	m_spikeY = GetPrivateProfileIntA(strApp, "spikeY", 0, POS_INI);
	m_spikeX = GetPrivateProfileIntA(strApp, "spikeX", 0, POS_INI);
	m_hitheadshotY = GetPrivateProfileIntA(strApp, "hitheadshotY", 0, POS_INI);
	m_hitheadshotX = GetPrivateProfileIntA(strApp, "hitheadshotX", 0, POS_INI);
	m_otherY = GetPrivateProfileIntA(strApp, "otherY", 0, POS_INI);
	m_otherX = GetPrivateProfileIntA(strApp, "otherX", 0, POS_INI);

	m_skillY = GetPrivateProfileIntA(strApp, "skillY", 0, POS_INI);
	m_skillStep = GetPrivateProfileIntA(strApp, "skillStep", 0, POS_INI);
	m_skillWidth = GetPrivateProfileIntA(strApp, "skillWidth", 0, POS_INI);

	return TRUE;
}

int CParserEngine::MatchWhitePattern(int x, int y, WHITE_PATTERN* ptn, int nPtn)
{
	int ret = INVALID;

	ret = MatchWhitePattern_(x, y, ptn, nPtn);

	if (ret == INVALID)
		ret = MatchWhitePattern_(x - 1, y, ptn, nPtn);

	if (ret == INVALID)
		ret = MatchWhitePattern_(x + 1, y, ptn, nPtn);

	return ret;
}

int CParserEngine::MatchWhitePattern_(int x, int y, WHITE_PATTERN* ptn, int nPtn)
{
	int w = ptn[0].width, h = ptn[0].height;
	unsigned int* blackImg = new unsigned int[w * h];
	unsigned char* p;
	unsigned int* pi = blackImg;
	unsigned int avgBlackColor = 0;
	unsigned int maxRGB = 0;
	int i, j, n;

	for (j = 0; j < h; j++)
	{
		if (y + j >= m_nImageHeight)
		{
			delete[] blackImg;
			return INVALID;
		}

		for (i = 0; i < w; i++)
		{
			if (x + i >= m_nImageWidth)
			{
				delete[] blackImg;
				return INVALID;
			}

			p = m_pImageBufferPtr + (m_nImageWidth * (y + j) + (x + i)) * 3;
			
			avgBlackColor += *p;
			*pi = *p;
			p++;

			avgBlackColor += *p;
			*pi += *p;
			p++;

			avgBlackColor += *p;
			*pi += *p;

			if (maxRGB < *pi)
			{
				maxRGB = *pi;
			}

			pi++;
		}
	}

	avgBlackColor /= (w * h);
	avgBlackColor += 9;

	if (avgBlackColor >= 253 * 3)
	{
		avgBlackColor = 253 * 3 - 1;
	}

	for (i = 0; i < w*h; i++)
	{
		if (blackImg[i] > avgBlackColor)
			blackImg[i] = 1;
		else
			blackImg[i] = 0;
	}

	for (n = nPtn-1; n >= 0; n--)
	{
		for (i = 0; i < w * h; i++)
		{
			if (ptn[i].mask[i] == 0)
				continue;

			if (blackImg[i] == 0)
				break;
		}

		if (i == w * h)
		{
			delete[] blackImg;
			return n;
		}
	}

	delete[] blackImg;

	return INVALID;
}

int CParserEngine::MatchColorPattern(int x, int y, COLOR_PATTERN* ptn, int nPtn, bool flip)
{
	int ret = INVALID;

	ret = MatchColorPattern_(x, y, ptn, nPtn, flip);

	if (ret == INVALID)
		ret = MatchColorPattern_(x - 1, y, ptn, nPtn, flip);

	if (ret == INVALID)
		ret = MatchColorPattern_(x + 1, y, ptn, nPtn, flip);

	return ret;
}

int CParserEngine::MatchColorPattern_(int x, int y, COLOR_PATTERN* ptn, int nPtn, bool flip)
{
	int w = ptn[0].width, h = ptn[0].height;
	int diff = h * w * 3 * 300;
	unsigned char* p;
	int i, j, n, ii, jj;
	int idx;
	int total, t, tt;

	for (n = 0; n < nPtn; n++)
	{
		if ((ptn[n].B[0] == 255) && (ptn[n].G[0] == 255) && (ptn[n].R[0] == 255))
		{
			continue;
		}

		total = 0;

		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				if (flip)
					idx = (j + 1) * w - 1 - i;
				else
					idx = j * w + i;

				t = 0x0FFFFFFFF;

				for (jj = -1; jj <= 1; jj++)
				{
					for (ii = -1; ii <= 1; ii++)
					{
						if (y + j + jj >= m_nImageHeight)
							return INVALID;

						if (y + j + jj < 0)
							return INVALID;

						if (x + i + ii >= m_nImageWidth)
							return INVALID;

						if (x + i + ii < 0)
							return INVALID;

						p = m_pImageBufferPtr + (m_nImageWidth * (y + j + jj) + (x + i + ii)) * 3;
						tt = ((ptn[n].B[idx] > (*p)) ? ptn[n].B[idx] - (*p) : (*p) - ptn[n].B[idx]) *
							((ptn[n].B[idx] > (*p)) ? ptn[n].B[idx] - (*p) : (*p) - ptn[n].B[idx]);
						p++;
						tt += ((ptn[n].G[idx] > (*p)) ? ptn[n].G[idx] - (*p) : (*p) - ptn[n].G[idx]) *
							((ptn[n].G[idx] > (*p)) ? ptn[n].G[idx] - (*p) : (*p) - ptn[n].G[idx]);
						p++;
						tt += ((ptn[n].R[idx] > (*p)) ? ptn[n].R[idx] - (*p) : (*p) - ptn[n].R[idx]) *
							((ptn[n].R[idx] > (*p)) ? ptn[n].R[idx] - (*p) : (*p) - ptn[n].R[idx]);

						if (tt < t)
							t = tt;
					}
				}

				total += t;

				if (total >= diff)
				{
					break;
				}
			}

			if (i < w)
			{
				break;
			}
		}

		if (j == h)
		{
			return n;
		}
	}

	return INVALID;
}

int CParserEngine::MatchMapPattern(int x, int y, COLOR_PATTERN* ptn, int nPtn)
{
	int w = ptn[0].width, h = ptn[0].height;
	unsigned char* p;
	int i, j, n, d, dd, diff = 350 * h * w * 3;
	int idx;

	for (n = 0; n < nPtn; n++)
	{
		if ((ptn[n].B[0] == 255) && (ptn[n].G[0] == 255) && (ptn[n].R[0] == 255))
		{
			continue;
		}

		d = 0;
		for (j = 0; j < h; j++)
		{
			for (i = 0; i < w; i++)
			{
				p = m_pImageBufferPtr + (m_nImageWidth * (y + j) + (x + i)) * 3;
				idx = j * w + i;

				dd = abs((int)(ptn[n].B[idx]) - (*p));
				dd = min(dd, abs((int)(ptn[n].B[idx]) - (*(p + 3))));
				dd = min(dd, abs((int)(ptn[n].B[idx]) - (*(p + m_nImageWidth* 3))));
				dd = min(dd, abs((int)(ptn[n].B[idx]) - (*(p + m_nImageWidth * 3 + 3))));
				d += dd * dd;

				if (d > diff)
					break;

				p++;
				dd = abs((int)(ptn[n].G[idx]) - (*p));
				dd = min(dd, abs((int)(ptn[n].G[idx]) - (*(p + 3))));
				dd = min(dd, abs((int)(ptn[n].G[idx]) - (*(p + m_nImageWidth * 3))));
				dd = min(dd, abs((int)(ptn[n].G[idx]) - (*(p + m_nImageWidth * 3 + 3))));
				d += dd * dd;

				if (d > diff)
					break;

				p++;
				dd = abs((int)(ptn[n].R[idx]) - (*p));
				dd = min(dd, abs((int)(ptn[n].R[idx]) - (*(p + 3))));
				dd = min(dd, abs((int)(ptn[n].R[idx]) - (*(p + m_nImageWidth * 3))));
				dd = min(dd, abs((int)(ptn[n].R[idx]) - (*(p + m_nImageWidth * 3 + 3))));
				d += dd * dd;

				if (d > diff)
				{
					break;
				}
			}

			if (i < w)
			{
				break;
			}
		}

		if (j == h)
		{
			return n;
		}
	}

	return INVALID;
}
