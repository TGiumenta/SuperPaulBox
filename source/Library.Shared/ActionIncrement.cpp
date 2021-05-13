#include "pch.h"
#include "ActionIncrement.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(ActionIncrement);


    ActionIncrement::ActionIncrement()
        : Action(ActionIncrement::TypeIdClass())
    {
    }

    gsl::owner<ActionIncrement*> ActionIncrement::Clone() const
    {
        return new ActionIncrement(*this);
    }

    void ActionIncrement::Update(WorldState*)
    {
        auto [foundDatum, foundScope] = Search(m_target);

        if (foundDatum == nullptr)
        {
            throw std::exception("Trying to use ActionIncrement on a target that doesn't exist!");
        }

        float currentVal = foundDatum->Front<float>();
        foundDatum->Set(currentVal + m_step);
    }

    const Vector<Signature> ActionIncrement::Signatures()
    {
        return Vector<Signature>
        {
            { "Target", Datum::DatumType::String, 1, offsetof(ActionIncrement, m_target) },
            { "Step", Datum::DatumType::Integer, 1, offsetof(ActionIncrement, m_step) }
        };
    }
}