#pragma once

#include <any>
#include <memory>
#include <functional>

#include "Events.h"

namespace dexode
{
    class EventBus;
}

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

        template <typename Event>
        void SpreadEvent(Event event);
        
    private:
        EventBus() = default;

        std::shared_ptr<dexode::EventBus> m_EventBus;

        friend class EventListener;
    };
}
