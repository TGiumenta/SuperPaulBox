#include "pch.h"
#include "ReactionAttributed.h"
#include "Event.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(ReactionAttributed);

    ReactionAttributed::ReactionAttributed(const std::string& subtype)
        : Reaction(ReactionAttributed::TypeIdClass()), m_subtype(subtype)
    {
        Event<EventMessageAttributed>::Subscribe(*this);
    }

    ReactionAttributed::~ReactionAttributed()
    {
        Event<EventMessageAttributed>::Unsubscribe(*this);
    }

    void ReactionAttributed::Notify(const EventPublisher& event)
    {
        // assume the event publisher is event message attributed
        const Event<EventMessageAttributed>* castedEvent = event.As<Event<EventMessageAttributed>>();
        assert(castedEvent != nullptr);
        const EventMessageAttributed& message = castedEvent->Message();

        // if it matches the subtype of this reaction attributed
        if (message.GetSubtype() == m_subtype)
        {

            // Make a copy of auxiliary attributes
            Scope scope;
            auto attributes = message.GetAuxiliaryAttributes();
            for (PairType* ptr : attributes)
            {
                Datum& newDatum = scope.Append(ptr->first);

                if (ptr->second.Type() == Datum::DatumType::Table)
                {
                    newDatum.SetType(Datum::DatumType::Table);
                    for (size_t i = 0; i < ptr->second.Size(); ++i)
                    {
                        Scope* newScope = ptr->second.Get<Scope>(i).Clone();
                        scope.Adopt(*newScope, ptr->first);
                    }
                }
                else
                {
                    newDatum = ptr->second;
                }
            }

            // Push the copied attributes onto the stack and call update
            WorldState* worldState = message.GetWorldState();
            worldState->m_scopeStack.Push(&scope);
            ActionList::Update(worldState);
            worldState->m_scopeStack.Pop();
        }
    }

    const Vector<Signature> ReactionAttributed::Signatures()
    {
        return Vector<Signature>
        {
            { "Subtype", Datum::DatumType::String, 1, offsetof(ReactionAttributed, m_subtype) }
        };
    }
}