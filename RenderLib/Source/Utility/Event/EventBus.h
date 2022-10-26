#pragma once

#include <memory>
#include <functional>

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

        template <class Event, typename _ = void>
        void SubscribeEvent(std::function<void(const Event&)>&& callback);

        template <typename Event>
        void SpreadEvent(Event event);
        
    private:
        EventBus() = default;

        std::unique_ptr<dexode::EventBus> m_EventBus;
    };
}
