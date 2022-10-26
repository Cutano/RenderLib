#pragma once

#include <memory>

#include "dexode/EventBus.hpp"

namespace RL
{
    class EventBus;
    
    class EventListener
    {
    public:
        EventListener();

        template <class Event, typename _ = void>
        void SubscribeEvent(std::function<void(const Event&)>&& callback);

    private:
        std::shared_ptr<dexode::EventBus> m_EventBus;
        std::unique_ptr<dexode::EventBus::Listener> m_Listener;
    };
}
