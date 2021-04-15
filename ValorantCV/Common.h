#pragma once

#define SETTING_INI _T(".\\set.ini")
#define POS_INI (".\\pos.ini")

#define MATCH_ERROR -2
#define INVALID -1
#define MASK_LEN 1600

#define MAX_ABILITY 4

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif // !MAX

#ifndef MIN
#define MIN(a,b) (((a) < (b)) ? (a) : (b))
#endif // !MIN

#define N_AGENTS 20
#define N_MAPAGENTS 28

enum EventType
{
	kEventNone = 0,

	kEventShoot,
	kEventKill,
	kEventDeath,
	kEventAmmoReload,
	kEventEnemyAppeared,
	kEventAbilityCast,

	kEventCount
};

template<typename T>
struct rectangle
{
	T width(void) const
	{
		return x2 - x1 > 0 ? x2 - x1 : x1 - x2;
	}

	T height(void) const
	{
		return y2 - y1 > 0 ? y2 - y1 : y1 - y2;
	}

	T x1, y1;
	T x2, y2;
};

typedef rectangle<unsigned short> recti16;

typedef struct WHITE_PATTERN {
	int width;
	int height;
	char mask[MASK_LEN]; //0: no mask, 1: mask, valid length is width*height
} WHITE_PATTERN;

typedef struct COLOR_PATTERN {
	int width;
	int height;
	unsigned char R[MASK_LEN];
	unsigned char G[MASK_LEN];
	unsigned char B[MASK_LEN];
} COLOR_PATTERN;

typedef struct EnemyRect
{
	recti16 m_enemyHead;
	recti16 m_enemyBody;
	recti16 m_enemyLeg;
} EnemyRect;

