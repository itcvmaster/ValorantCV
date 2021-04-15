#include "pch.h"
#include "EventBuilder.h"
#include "AbilityCastEvent.h"
#include "AmmoReloadEvent.h"

CEventBuilder::CEventBuilder()
{
}

CEventBuilder::~CEventBuilder()
{
}

CTimelineData CEventBuilder::build(CTimelineData* pCurrSlice, std::vector<CTimelineData>& aryTimelineData)
{
	CTimelineData* pResult = new CTimelineData();
	
	*pResult = *pCurrSlice;

	if (aryTimelineData.empty())
	{
		aryTimelineData.push_back(*pCurrSlice);

		return *pResult;
	}

	CTimelineData prevSlice = aryTimelineData.back();

	// ShootEvent
	// KillEvent
	// DeathEvent
	// AmmoReloadEvent
	buildAmmoReloadEvent(pCurrSlice, &prevSlice);
	// EnemyAppearedEvent
	// AbilityCastEvent
	buildAbiltyCastEvent(pCurrSlice, &prevSlice);

	return *pResult;
}

void CEventBuilder::buildAbiltyCastEvent(CTimelineData* pCurrSlice, CTimelineData* pPrevSlice)
{
	if (pCurrSlice->m_clsPlayerState.m_health > 0 && pPrevSlice->m_clsPlayerState.m_ammo == INVALID)
	{
		if (pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[0] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[0] + 1 &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[1] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[1] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[2] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[2] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[3] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[3])
		{
			// used C skill
			CAbilityCastEvent* pEvent = new CAbilityCastEvent();

			pEvent->m_ability = 1;

			pCurrSlice->m_aryEvents.push_back(pEvent);
		}

		if (pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[0] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[0] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[1] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[1] + 1 &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[2] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[2] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[3] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[3])
		{
			// used Q skill
			CAbilityCastEvent* pEvent = new CAbilityCastEvent();

			pEvent->m_ability = 0;

			pCurrSlice->m_aryEvents.push_back(pEvent);
		}

		if (pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[0] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[0] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[1] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[1] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[2] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[2] + 1 &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[3] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[3])
		{
			// used E skill
			CAbilityCastEvent* pEvent = new CAbilityCastEvent();

			pEvent->m_ability = 2;

			pCurrSlice->m_aryEvents.push_back(pEvent);
		}

		if (pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[0] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[0] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[1] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[1] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[2] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[2] &&
			pPrevSlice->m_clsPlayerState.m_aryAbilityLeft[3] == pCurrSlice->m_clsPlayerState.m_aryAbilityLeft[3] + 1)
		{
			// used X skill
			CAbilityCastEvent* pEvent = new CAbilityCastEvent();

			pEvent->m_ability = 3;

			pCurrSlice->m_aryEvents.push_back(pEvent);
		}
	}
}

void CEventBuilder::buildAmmoReloadEvent(CTimelineData* pCurrSlice, CTimelineData* pPrevSlice)
{
	if (pPrevSlice->m_clsPlayerState.m_ammo >= 0 && pPrevSlice->m_clsPlayerState.m_ammoLeft >= 0 &&
		pCurrSlice->m_clsPlayerState.m_ammo >= 0 && pCurrSlice->m_clsPlayerState.m_ammoLeft >= 0 &&
		pPrevSlice->m_clsPlayerState.m_ammoLeft > pCurrSlice->m_clsPlayerState.m_ammoLeft &&
		pPrevSlice->m_clsPlayerState.m_ammo + pPrevSlice->m_clsPlayerState.m_ammoLeft == pCurrSlice->m_clsPlayerState.m_ammo + pCurrSlice->m_clsPlayerState.m_ammoLeft)
	{
		CAmmoReloadEvent* pEvent = new CAmmoReloadEvent();

		pEvent->m_oldAmmo = pPrevSlice->m_clsPlayerState.m_ammo;
		pEvent->m_newAmmo = pCurrSlice->m_clsPlayerState.m_ammo;
		
		pCurrSlice->m_aryEvents.push_back(pEvent);
	}
}
