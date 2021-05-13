#pragma once
#include <chrono>
#include <memory>
#include "GameTime.h"
#include "RTTI.h"
#include "Vector.h"
#include "SList.h"
#include "Event.h"

namespace FieaGameEngine
{
    /// <summary>
    /// EventQueue class that handles queueing events with various delays and managing
    /// when to deliver them.
    /// </summary>
	class EventQueue final
	{
    public:
        /// <summary>
        /// Constructor that creates a copy of the passed in GameTime
        /// </summary>
        /// <param name="gameTime"> The GameTime to make a copy of to use
        /// for this class's timekeeping </param>
        EventQueue(GameTime& gameTime);

        EventQueue(const EventQueue& other) = delete;
        EventQueue& operator=(const EventQueue& other) = delete;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name="other"> The EventQueue to move </param>
        EventQueue(EventQueue&& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name="other"> The EventQueue to move </param>
        /// <returns> A reference to the newly assigned EventQueue </returns>
        EventQueue& operator=(EventQueue&& other) = default;

        /// <summary>
        /// Default destructor 
        /// </summary>
        ~EventQueue() = default;

        /// <summary>
        /// Updates the current GameTime and calls Deliver on any queued events that
        /// have expired
        /// </summary>
        void Update();

        /// <summary>
        /// Enqueues the passed in event with an optional delay before it is delivered. If this
        /// event is being enqueued as a result of a Notify from another event, it won't be delivered
        /// until at least 1 tick later.
        /// </summary>
        /// <param name="e"> The event to enqueue </param>
        /// <param name="delay"> Optional delay before this event is delivered </param>
        void Enqueue(std::shared_ptr<EventPublisher> e, std::chrono::milliseconds delay = std::chrono::milliseconds(0));

        /// <summary>
        /// Clears all events out of the queue (they will not be delivered)
        /// </summary>
        void Clear();

        /// <summary>
        /// Determines whether the event queue is empty
        /// </summary>
        /// <returns> True if there are no events in the queue, false otherwise </returns>
        bool IsEmpty() const;

        /// <summary>
        /// Returns how many events are in the queue
        /// </summary>
        /// <returns> How many events are in the queue </returns>
        size_t Size() const;

    private:
        /// <summary>
        /// Private QueueEntry struct used to keep track of events in the queue
        /// </summary>
        struct QueueEntry
        {
            std::shared_ptr<EventPublisher> m_eventPublisher;
            std::chrono::milliseconds m_delay;
            std::chrono::high_resolution_clock::time_point m_timeEnqueued;

            inline bool IsExpired(std::chrono::high_resolution_clock::time_point currentTime) const
            {
                return (m_timeEnqueued + m_delay) <= currentTime;
            }
        };

        /// <summary>
        /// Internal GameTime used to keep track of elapsed time
        /// </summary>
        GameTime* m_gameTime;

        /// <summary>
        /// Internal event queue used to keep track of all events that have been added
        /// </summary>
        Vector<QueueEntry> m_events;

        /// <summary>
        /// Queue buffer used just in case events are added to the queue during the iteration
        /// through events. All entries will be transferred to the event queue at the start of
        /// each update call.
        /// </summary>
        Vector<QueueEntry> m_queueBuffer;
	};
}
