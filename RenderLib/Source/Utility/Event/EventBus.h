#pragma once

#include <memory>
#include <dexode/EventBus.hpp>

namespace RL
{
    class EventBus
    {
    public:
        static EventBus& Get()
        {
            static EventBus s_Instance;
            return s_Instance;
        }

        ~EventBus() = default;
        EventBus(const EventBus& other) = delete;
        EventBus(EventBus&& other) noexcept = delete;
        EventBus& operator=(const EventBus& other) = delete;
        EventBus& operator=(EventBus&& other) noexcept = delete;

        void Init();
        void Update();

        template <typename Event>
        void SpreadEvent(Event event)
        {
            m_EventBus->postpone<Event>(event);
            // Should I move process to main loop?
            m_EventBus->process();
        }
        
    private:
        EventBus() = default;

        std::shared_ptr<dexode::EventBus> m_EventBus;

        friend class EventListener;
    };
}
