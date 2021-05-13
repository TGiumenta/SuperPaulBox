#include "pch.h"
#include "Action.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(Action);

    Action::Action(RTTI::IdType type)
        : Attributed(type)
    {
    }

    const Vector<Signature> Action::Signatures()
    {
        return Vector<Signature>
        {
            { "Name", Datum::DatumType::String, 1, offsetof(Action, m_name) }
        };
    }
}