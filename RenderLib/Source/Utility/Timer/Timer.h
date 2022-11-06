#pragma once

#include <chrono>

namespace RL
{
    class Timer
    {
    public:
        static Timer& Get()
        {
            static Timer s_Instance;
            return s_Instance;
        }

        ~Timer() = default;
        Timer(const Timer& other) = delete;
        Timer(Timer&& other) noexcept = delete;
        Timer& operator=(const Timer& other) = delete;
        Timer& operator=(Timer&& other) noexcept = delete;

        void Init();
        void Update();

        double GetDeltaTime();
        double GetFPS();

    private:
        Timer() = default;

        std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
        std::chrono::time_point<std::chrono::steady_clock> m_LastFrameTime;
        std::chrono::time_point<std::chrono::steady_clock> m_ThisFrameTime;
    };
}
