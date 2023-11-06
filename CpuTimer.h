//***************************************************************************************
// CpuTimer.h by Frank Luna (C) 2011 All Rights Reserved.
// Modify name from GameTimer.cpp
// CPU��ʱ��
//***************************************************************************************

#pragma once

#ifndef CPU_TIMER_H
#define CPU_TIMER_H

class CpuTimer
{
public:
    CpuTimer();

    float TotalTime()const;     // ���ش�Reset()����֮�󾭹���ʱ�䣬����������ͣ�ڼ��
    float DeltaTime()const;     // ����֡���ʱ��

    void Reset();               // ��ʱ��ʼǰ������Ҫ����ʱ����
    void Start();               // �ڿ�ʼ��ʱ��ȡ����ͣ��ʱ�����
    void Stop();                // ����Ҫ��ͣ��ʱ�����
    void Tick();                // ��ÿһ֡��ʼ��ʱ�����
    bool IsStopped() const;     // ��ʱ���Ƿ���ͣ/����

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