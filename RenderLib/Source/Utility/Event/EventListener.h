#pragma once

#include <memory>
#include <vector>
#include <eventbus/event_bus.hpp>

namespace RL
{
    class EventBus;
    
    class EventListener
    {
    public:
        EventListener();

        template <class Event, typename _ = void>
        void SubscribeEvent(std::function<void(const Event&)>&& callback)
        {
            m_Handlers.emplace_back(m_EventBus->register_handler<Event>(std::forward<std::function<void(const Event&)>>(callback)));
        }

    private:
        std::shared_ptr<dp::event_bus> m_EventBus;
        std::vector<dp::handler_registration> m_Handlers;
    };
}
