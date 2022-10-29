#include "EventBus.h"

namespace RL
{
    void EventBus::Init()
    {
        m_EventBus = std::make_shared<dp::event_bus>();
    }

    void EventBus::Update()
    {
        
    }

    void EventBus::Shutdown()
    {
        m_EventBus->remove_handlers();
    }
}
