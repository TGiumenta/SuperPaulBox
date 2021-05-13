#include "pch.h"
#include "Event.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(EventPublisher);

    void EventPublisher::Deliver() const
    {
        // for each subscriber in the m_subscribers list, call notify
        for (EventSubscriber* subscriber : *m_subscribersPtr)
        {
            subscriber->Notify(*this);
        }
    }

}