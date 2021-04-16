#include "pch.h"
#include "ValorantReader.h"

CValorantReader::CValorantReader()
	: m_pImgBuffer (NULL)
{
}

BOOL CValorantReader::InitInstance()
{
	m_pImgBuffer = new BYTE[1920 * 1080 * 5 * 3];

	return FALSE;
}

BOOL CValorantReader::Initialize()
{
	return FALSE;
}

int CValorantReader::Run()
{
	while (true)
	{
		pipeline();
		Sleep(20);
	}

	return 0;
}

BOOL CValorantReader::pipeline()
{
	int nScreenWidth = 0, nScreenHeight = 0;

	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (m_pImgBuffer == NULL)
	{
		return FALSE;
	}

	m_pGameCapturer->captureScreen(m_pImgBuffer, nScreenWidth, nScreenHeight);
	CTimelineData currSlice = m_pEngine->run(m_pImgBuffer, nScreenWidth, nScreenHeight);
	m_currSlice = m_pEventBuilder->build(&currSlice, m_aryTimelineData);

	return FALSE;
}

BOOL CValorantReader::exportCurrent(CTimelineData& out)
{
	return FALSE;
}

BOOL CValorantReader::exportAll(std::vector<CTimelineData>& outArray)
{
	return FALSE;
}
