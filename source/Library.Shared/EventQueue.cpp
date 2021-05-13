#include "pch.h"
#include "EventQueue.h"

namespace FieaGameEngine
{
    EventQueue::EventQueue(GameTime& gameTime)
        : m_gameTime(&gameTime)
    {
    }

    void EventQueue::Enqueue(std::shared_ptr<EventPublisher> e, std::chrono::milliseconds delay)
    {
        QueueEntry entry{ std::move(e), delay, m_gameTime->CurrentTime() };
        m_queueBuffer.PushBack(std::move(entry));
    }

    void EventQueue::Clear()
    {
        m_events.Clear();
        m_queueBuffer.Clear();
    }

    bool EventQueue::IsEmpty() const
    {
       return m_events.IsEmpty() && m_queueBuffer.IsEmpty();
    }

    size_t EventQueue::Size() const
    {
        return m_events.Size() + m_queueBuffer.Size();
    }

    void EventQueue::Update()
    {
        // Add all queued events sitting in the buffer to our event queue, then clear the buffer
        for (QueueEntry& entry : m_queueBuffer)
        {
            m_events.PushBack(entry);
        }
        m_queueBuffer.Clear();

        // Partition our events into expired and not expired
        auto it = std::partition(m_events.begin(), m_events.end(), 
            [this](QueueEntry entry) { return !entry.IsExpired(m_gameTime->CurrentTime()); });

        // Iterate through the list of event publishers and deliver the expired ones
        for (auto i = it; i != m_events.end(); ++i)
        {
            i->m_eventPublisher->Deliver();
        }

        // Delete all the delivered events if there were any
        if (it != m_events.end())
        {
            m_events.Remove(it, m_events.end());
        }
    }
}