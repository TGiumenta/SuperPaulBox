#include "pch.h"
#include "Avatar.h"

namespace UnitTests
{
    RTTI_DEFINITIONS(Avatar);

    Avatar::Avatar()
        : FieaGameEngine::Entity(Avatar::TypeIdClass())
    {
    }

    const FieaGameEngine::Vector<FieaGameEngine::Signature> Avatar::Signatures()
    {
        return FieaGameEngine::Vector<FieaGameEngine::Signature>
        {
            { "Health", FieaGameEngine::Datum::DatumType::Integer, 1, offsetof(Avatar, m_health) },
            { "Dps", FieaGameEngine::Datum::DatumType::Float, 1, offsetof(Avatar, m_dps) }
        };
    }

    gsl::owner<Scope*> Avatar::Clone() const
    {
        return new Avatar(*this);
    }
}


