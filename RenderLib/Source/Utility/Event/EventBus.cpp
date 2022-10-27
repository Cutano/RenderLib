#include "EventBus.h"

namespace RL
{
    void EventBus::Init()
    {
        m_EventBus = std::make_shared<dexode::EventBus>();
    }

    void EventBus::Update()
    {
        m_EventBus->process();
    }
}
