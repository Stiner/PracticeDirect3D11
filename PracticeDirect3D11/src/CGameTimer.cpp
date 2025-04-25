// PracticeD3D11

#include "PCH.h"
#include "CGameTimer.h"

CGameTimer::CGameTimer()
    : _SecondsPerCount(0.0)
    , _DeltaTime(-1.0)
    , _BaseTime(0)
    , _PausedTime(0)
    , _StopTime(0)
    , _PrevTime(0)
    , _CurrTime(0)
    , _IsStopped(false)
{
    int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    _SecondsPerCount = 1.0 / (double)countsPerSec;
}

float CGameTimer::DeltaTime() const
{
    return (float)_DeltaTime;
}

float CGameTimer::TotalTime() const
{
    if (_IsStopped)
    {
        return (float)(((_StopTime - _PausedTime) - _BaseTime) * _SecondsPerCount);
    }
    else
    {
        return (float)(((_CurrTime - _PausedTime) - _BaseTime) * _SecondsPerCount);
    }
}

bool CGameTimer::IsStopped() const
{
    return _IsStopped;
}

void CGameTimer::Reset()
{
    int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    _BaseTime  = currTime;
    _PrevTime  = currTime;
    _StopTime  = 0;
    _IsStopped = false;
}

void CGameTimer::Start()
{
    if (_IsStopped)
    {
        int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

        _PausedTime += (currTime - _StopTime);

        _PrevTime  = currTime;
        _StopTime  = 0;
        _IsStopped = false;
    }
}

void CGameTimer::Stop()
{
    if (!_IsStopped)
    {
        int64 currTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

        _StopTime = currTime;
        _IsStopped  = true;
    }
}

void CGameTimer::Tick()
{
    if (_IsStopped)
    {
        _DeltaTime = 0.0;
        return;
    }

    int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    _CurrTime  = currTime;
    _DeltaTime = (currTime - _PrevTime) * _SecondsPerCount;
    _PrevTime  = currTime;

    // SDK(2010 June)의 CDXUTTimer항목에 따르면,
    // 프로세서가 절전 모드로 들어가거나 다른 프로세서와 엉키는 경우
    // _DeltaTime이 음수가 될 수 있다고 한다.
    if (_DeltaTime < 0.0)
    {
        _DeltaTime = 0;
    }
}
