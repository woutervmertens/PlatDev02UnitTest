#include "stdafx.h"
#include "GameTimer.h"



GameTimer::GameTimer(void):
		m_ElapsedSeconds(0.0f),
		m_TotalSeconds(0.0f),
		m_SecondsPerCount(0.0f),
		m_BaseTime(0),
		m_PausedTime(0),
		m_StopTime(0),
		m_PrevTime(0),
		m_CurrTime(0),
		m_IsStopped(true),
		m_FPS(0),
		m_FpsTimer(0.0f),
		m_FpsCount(0)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_SecondsPerCount = 1.0f / (float) countsPerSec;
}


GameTimer::~GameTimer(void)
{
}

void GameTimer::Update()
{
	if(m_IsStopped)
	{
		m_FPS = 0;
		m_ElapsedSeconds = 0.0f;
		m_TotalSeconds = (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_BaseTime);
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	m_CurrTime = currTime;

	m_ElapsedSeconds = (float)((m_CurrTime - m_PrevTime) * m_SecondsPerCount);
	m_PrevTime = m_CurrTime;

	if(m_ElapsedSeconds < 0.0f)
		m_ElapsedSeconds = 0.0f;

	m_TotalSeconds = (float)(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);

	//FPS LOGIC
	m_FpsTimer += m_ElapsedSeconds;
	++m_FpsCount;
	if(m_FpsTimer >= 1.0f)
	{
		m_FPS = m_FpsCount;
		m_FpsCount = 0;
		m_FpsTimer -= 1.0f;
	}
}

void GameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTime = currTime;
	m_StopTime = 0;
	m_FpsTimer = 0.0f;
	m_FpsCount = 0;
	m_IsStopped = false;
}

void GameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if(m_IsStopped)
	{
		m_PausedTime += (startTime - m_StopTime);

		m_PrevTime = startTime;
		m_StopTime = 0;
		m_IsStopped = false;
	}
}

void GameTimer::Stop()
{
	if(!m_IsStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

		m_StopTime = currTime;
		m_IsStopped = true;
	}
}