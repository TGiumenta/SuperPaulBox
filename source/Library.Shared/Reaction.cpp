#include "pch.h"
#include "Reaction.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(Reaction);

    Reaction::Reaction(RTTI::IdType type)
        : ActionList(type)
    {
    }
}