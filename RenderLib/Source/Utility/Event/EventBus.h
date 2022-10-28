#pragma once

#include <memory>
#include <eventbus/event_bus.hpp>

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
        void SpreadEvent(Event&& event)
        {
            m_EventBus->fire_event<Event>(std::forward<Event>(event));
        }
        
    private:
        EventBus() = default;

        std::shared_ptr<dp::event_bus> m_EventBus;

        friend class EventListener;
    };
}
