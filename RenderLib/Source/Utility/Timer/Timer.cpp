#include "Timer.h"

#include <chrono>

namespace RL
{
    void Timer::Init()
    {
        m_StartTime = std::chrono::high_resolution_clock::now();
    }

    void Timer::Update()
    {
        m_LastFrameTime = m_ThisFrameTime;
        m_ThisFrameTime = std::chrono::high_resolution_clock::now();
    }

    double Timer::GetDeltaTime()
    {
        const auto duration = m_ThisFrameTime - m_LastFrameTime;
        return static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(duration).count()) / 1000.0;
    }

    double Timer::GetFPS()
    {
        const double dt = GetDeltaTime();
        return 1000.0 / dt;
    }
}
