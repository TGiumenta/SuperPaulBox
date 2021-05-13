#include "pch.h"
#include "ActionEvent.h"
#include "EventMessageAttributed.h"
#include "WorldState.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(ActionEvent);

    ActionEvent::ActionEvent()
        : Action(ActionEvent::TypeIdClass())
    {
    }

    void ActionEvent::Update(WorldState* worldState)
    {
        EventMessageAttributed e;
        
        auto attributes = GetAuxiliaryAttributes();
        for (PairType* ptr : attributes)
        {
            Datum& newDatum = e.Append(ptr->first);

            if (ptr->second.Type() == Datum::DatumType::Table)
            {
                newDatum.SetType(Datum::DatumType::Table);
                for (size_t i = 0; i < ptr->second.Size(); ++i)
                {
                    Scope* newScope = ptr->second.Get<Scope>(i).Clone();
                    e.Adopt(*newScope, ptr->first);
                }
            }
            else
            {
                newDatum = ptr->second;
            }
        }

        e.SetSubtype(m_subtype);
        e.SetWorldState(*worldState);
        auto event = std::make_shared<Event<EventMessageAttributed>>(e);
        worldState->GetEventQueue().Enqueue(event, std::chrono::milliseconds(m_delay));
    }

    const Vector<Signature> ActionEvent::Signatures()
    {
        return Vector<Signature>
        {
            { "Subtype", Datum::DatumType::String, 1, offsetof(ActionEvent, m_subtype) },
            { "Delay", Datum::DatumType::Integer, 1, offsetof(ActionEvent, m_delay) }
        };
    }
}