#include "EventListener.h"
#include "EventBus.h"

namespace RL
{
    EventListener::EventListener()
    {
        m_EventBus = EventBus::Get().m_EventBus;
    }
    
}
