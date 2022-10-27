﻿#include "EventListener.h"
#include "EventBus.h"

namespace RL
{
    EventListener::EventListener()
    {
        m_EventBus = EventBus::Get().m_EventBus;
        m_Listener = std::make_unique<dexode::EventBus::Listener>(m_EventBus);
    }
    
}
