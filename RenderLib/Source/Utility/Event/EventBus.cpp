#include "EventBus.h"

namespace RL
{
    void EventBus::Init()
    {
        m_EventBus = std::make_unique<dexode::EventBus>();
    }
    
    template <class Event, typename _>
    void EventBus::SubscribeEvent(std::function<void(const Event&)>&& callback)
    {
        
    }

    template <typename Event>
    void EventBus::SpreadEvent(Event event)
    {
        m_EventBus->postpone(event);
    }
}
