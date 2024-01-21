#include "WinMin.h"
#include "CpuTimer.h"

CpuTimer::CpuTimer()
{
    __int64 countsPerSec{};
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    m_SecondsPerCount = 1.0 / (double)countsPerSec;
}

float CpuTimer::TotalTime()const
{
    // If Stop() is called and it's stopped, we don't need to include the time after stopping.
    // (m_StopTime - m_BaseTime) may include the time before it was previously stopped.
    // Thus, we can subtract the previously accumulated paused time from m_StopTime.
    //
    //                     |<-- Paused Time -->|
    // ----*---------------*-----------------*------------*------------*------> time
    //  m_BaseTime       m_StopTime        startTime     m_StopTime    m_CurrTime

    if (m_Stopped)
    {
        return (float)(((m_StopTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
    }

    // (m_CurrTime - m_BaseTime) is the time excluding the paused time, which we don't want to include.
    // Thus, we can subtract the previously accumulated paused time from m_CurrTime.
    //
    //  (m_CurrTime - m_PausedTime) - m_BaseTime 
    //
    //                     |<-- Paused Time -->|
    // ----*---------------*-----------------*------------*------> time
    //  m_BaseTime       m_StopTime        startTime     m_CurrTime

    else
    {
        return (float)(((m_CurrTime - m_PausedTime) - m_BaseTime) * m_SecondsPerCount);
    }
}

float CpuTimer::DeltaTime()const
{
    return (float)m_DeltaTime;
}

void CpuTimer::Reset()
{
    __int64 currTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    m_BaseTime = currTime;
    m_PrevTime = currTime;
    m_StopTime = 0;
    m_PausedTime = 0;   // Resetting the paused time to 0 as it's related to Reset
    m_Stopped = false;
}

void CpuTimer::Start()
{
    __int64 startTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

    // Resumes from the previous stop, thus we add the paused duration.
    //
    //                     |<-------d------->|
    // ----*---------------*-----------------*------------> time
    //  m_BaseTime       m_StopTime        startTime     

    if (m_Stopped)
    {
        m_PausedTime += (startTime - m_StopTime);

        m_PrevTime = startTime;
        m_StopTime = 0;
        m_Stopped = false;
    }
}

void CpuTimer::Stop()
{
    if (!m_Stopped)
    {
        __int64 currTime{};
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

        m_StopTime = currTime;
        m_Stopped = true;
    }
}

void CpuTimer::Tick()
{
    if (m_Stopped)
    {
        m_DeltaTime = 0.0;
        return;
    }

    __int64 currTime{};
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
    m_CurrTime = currTime;

    // Frame time between the current Tick and the previous Tick
    m_DeltaTime = (m_CurrTime - m_PrevTime) * m_SecondsPerCount;

    m_PrevTime = m_CurrTime;

    if (m_DeltaTime < 0.0)
    {
        m_DeltaTime = 0.0;
    }
}

bool CpuTimer::IsStopped() const
{
    return m_Stopped;
}
