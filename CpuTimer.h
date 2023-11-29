//***************************************************************************************
// CpuTimer.h by Frank Luna (C) 2011 All Rights Reserved.
// Modified name from GameTimer.cpp
// CPU Timer
//***************************************************************************************

#pragma once

#ifndef CPU_TIMER_H
#define CPU_TIMER_H

class CpuTimer
{
public:
    CpuTimer();

    float TotalTime()const;     // Returns the total time since Reset() was called, unless it's currently stopped.
    float DeltaTime()const;     // Returns the time between frames.

    void Reset();               // Call before starting the timer to set the initial time.
    void Start();               // Starts the timer from a stopped state.
    void Stop();                // Stops the timer.
    void Tick();                // Call at the start of each frame to update the timer.
    bool IsStopped() const;     // Checks if the timer is stopped/paused.

private:
    double m_SecondsPerCount = 0.0;
    double m_DeltaTime = -1.0;

    __int64 m_BaseTime = 0;
    __int64 m_PausedTime = 0;
    __int64 m_StopTime = 0;
    __int64 m_PrevTime = 0;
    __int64 m_CurrTime = 0;

    bool m_Stopped = false;
};

#endif // GAMETIMER_H
