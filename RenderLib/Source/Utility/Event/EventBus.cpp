#include "EventBus.h"

#include <dexode/EventBus.hpp>

namespace RL
{
    void EventBus::Init()
    {
        m_EventBus = std::make_shared<dexode::EventBus>();
    }

    template <typename Event>
    void EventBus::SpreadEvent(Event event)
    {
        m_EventBus->postpone<Event>(event);
    }
}
