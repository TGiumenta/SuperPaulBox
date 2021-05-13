#include "pch.h"
#include "EventMessageAttributed.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(EventMessageAttributed);

    EventMessageAttributed::EventMessageAttributed()
        : Attributed(EventMessageAttributed::TypeIdClass())
    {
    }

    const Vector<Signature> EventMessageAttributed::Signatures()
    {
        return Vector<Signature>
        {
            { "Subtype", Datum::DatumType::String, 1, offsetof(EventMessageAttributed, m_subtype) }
        };
    }
}