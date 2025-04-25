// PracticeD3D11

#pragma once

class CGameTimer
{
public:
    CGameTimer();

    /// <summary>
    /// 게임시간 반환
    /// </summary>
    /// <returns>초 단위 게임시간.</returns>
    float TotalTime() const;

    /// <summary>
    /// 경과시간 반환
    /// </summary>
    /// <returns>초 단위 경과시간.</returns>
    float DeltaTime() const;

    /// <summary>
    /// 타이머 일시정지 여부 반환
    /// </summary>
    /// <returns>true:정지됨 / false:돌아감</returns>
    bool IsStopped() const;

    /// <summary>
    /// 타이머를 초기화
    /// - 게임 루프 시작 시 호출
    /// </summary>
    void Reset();

    /// <summary>
    /// 타이머를 시작
    /// - 타이머 시작/재개 시 호출
    /// </summary>
    void Start();

    /// <summary>
    /// 타이머 일시정지
    /// - 일시 정지시 호출해야 함
    /// </summary>
    void Stop();

    /// <summary>
    /// 타이머 업데이트
    /// - 매 프레임마다 호출 해야 함
    /// </summary>
    void Tick();

private:
    double _SecondsPerCount;
    double _DeltaTime;

    int64 _BaseTime;
    int64 _PausedTime;
    int64 _StopTime;
    int64 _PrevTime;
    int64 _CurrTime;

    bool _IsStopped;
};

