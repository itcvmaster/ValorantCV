#pragma once

#include "TimelineData.h"

class CEventBuilder
{
public:
	CEventBuilder();
	virtual ~CEventBuilder();

	CTimelineData build(CTimelineData* pCurrSlice, std::vector<CTimelineData>& aryTimelineData);

private:
	void buildAbiltyCastEvent(CTimelineData* pCurrSlice, CTimelineData* pPrevSlice);
	void buildAmmoReloadEvent(CTimelineData* pCurrSlice, CTimelineData* pPrevSlice);

public:

};

