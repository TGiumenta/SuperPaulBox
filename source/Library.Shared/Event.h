#pragma once
#include "RTTI.h"
#include "Vector.h"
#include "SList.h"
#include <chrono>
#include "GameTime.h"
#include <memory>

namespace FieaGameEngine
{
    /// <summary>
    /// Abstract EventSubscriber class that defines the base for any class that
    /// will listen to events from an EventPublisher. All derived classes must
    /// implement the Notify method.
    /// </summary>
    class EventSubscriber
    {


    public:

        /// <summary>
        /// Default destructor
        /// </summary>
        virtual ~EventSubscriber() {};

        /// <summary>
        /// Pure virtual Notify method that describes how this subscriber handles getting
        /// an event
        /// </summary>
        /// <param name=""> The event being receieved </param>
        virtual void Notify(const class EventPublisher&) = 0;
    };

    /// <summary>
    /// Abstract EventPublisher class that defines the base for any class that will
    /// deliver events to listeners
    /// </summary>
    class EventPublisher : public RTTI
    {
        RTTI_DECLARATIONS(EventPublisher, RTTI);
    public:
        /// <summary>
        /// Event publisher constructor that is used to get a pointer to the subscriber
        /// list in the derived class
        /// </summary>
        /// <param name="subscribers"> The subscriber list that belongs to the derived
        /// class </param>
        EventPublisher(const Vector<EventSubscriber*>& subscribers)
            : m_subscribersPtr(&subscribers) {}

        /// <summary>
        /// Default copy constructor
        /// </summary>
        /// <param name="other"> The EventPublisher to copy </param>
        EventPublisher(const EventPublisher& other) = default;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name="other"> The EventPublisher to move </param>
        EventPublisher(EventPublisher&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name="other"> The EventPublisher to copy </param>
        /// <returns> A reference to the newly assigned EventPublisher </returns>
        EventPublisher& operator=(const EventPublisher& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name="other"> The EventPublisher to move </param>
        /// <returns> A reference to the newly assigned EventPublisher </returns>
        EventPublisher& operator=(EventPublisher&& other) = default;

        /// <summary>
        /// Default destructor 
        /// </summary>
        virtual ~EventPublisher() = 0 {};

        /// <summary>
        /// Notifies all subscribers of this event
        /// </summary>
        void Deliver() const; 

    protected:
        /// <summary>
        /// Pointer to the subscriber list that belongs to the derived class (Event)
        /// </summary>
        const Vector<EventSubscriber*>* m_subscribersPtr;
    };

    /// <summary>
    /// Templated event class that defines a specific event with a payload
    /// </summary>
    /// <typeparam name="T"> Defines the class that will represent the payload for
    /// delivering this event </typeparam>
    template <typename T>
    class Event : public EventPublisher
    {
        RTTI_DECLARATIONS(Event<T>, EventPublisher);
    public:

        /// <summary>
        /// Constructor that sets the message that this event will deliver
        /// </summary>
        /// <param name="message"> The message to set this event to </param>
        Event(const T& message)
            : EventPublisher(m_subscribers), m_message(message) {}

        /// <summary>
        /// Constructor that sets the message that this event will deliver
        /// </summary>
        /// <param name="message"> The message to set this event to </param>
        Event(T&& message)
            : EventPublisher(m_subscribers), m_message(std::forward<T>(message)) {}

        /// <summary>
        /// Default copy constructor
        /// </summary>
        /// <param name="other"> The Event to copy </param>
        Event(const Event& other) = default;

        /// <summary>
        /// Default move constructor 
        /// </summary>
        /// <param name="other"> The Event to move </param>
        Event(Event&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name="other"> The Event to copy </param>
        /// <returns> A reference to the newly assigned Event </returns>
        Event& operator=(const Event& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name="other"> The Event to move </param>
        /// <returns> A reference to the newly assigned Event </returns>
        Event& operator=(Event&& other) = default;

        /// <summary>
        /// Default Destructor
        /// </summary>
        virtual ~Event() = default;
        
        /// <summary>
        /// Subscribes the passed in EventSubscriber to this event, so that its Notify 
        /// method is called whenever Deliver is called on this event. If this subscriber
        /// is already subscribed, nothing happens
        /// </summary>
        /// <param name="subscriber"> The subscriber to subscribe </param>
        static void Subscribe(EventSubscriber& subscriber)
        {
            auto it = m_subscribers.Find(&subscriber);
            if (it == m_subscribers.end())
            {
                m_subscribers.PushBack(&subscriber);
            }
        }

        /// <summary>
        /// Unsubscribes the passed in EventSubscriber from this event. If this subscriber
        /// wasn't subscribed, nothing happens.
        /// </summary>
        /// <param name="subscriber"> The subscriber to unsubscribe </param>
        /// <returns> True if the subscriber was unsubscribed, false otherwise </returns>
        static bool Unsubscribe(EventSubscriber& subscriber)
        {
            return m_subscribers.Remove(&subscriber);
        }

        /// <summary>
        /// Unsubscribes all subscribers from this event
        /// </summary>
        static void UnsubscribeAll()
        {
            m_subscribers.Clear();
            m_subscribers.ShrinkToFit();
        }

        /// <summary>
        /// Gets the payload attached to this event
        /// </summary>
        /// <returns> A reference to the payload attached to this event </returns>
        const T& Message() const
        {
            return m_message;
        }

    private:

        /// <summary>
        /// Static subscriber list shared between all events of the same template type
        /// </summary>
        inline static Vector<EventSubscriber*> m_subscribers{};

        /// <summary>
        /// The payload attached to this event
        /// </summary>
        T m_message;
    };

    template <typename T>
    RTTI_DEFINITIONS(Event<T>);

}