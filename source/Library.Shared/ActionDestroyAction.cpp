#include "pch.h"
#include "ActionDestroyAction.h"
#include "WorldState.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(ActionDestroyAction);

    ActionDestroyAction::ActionDestroyAction()
        : Action(ActionDestroyAction::TypeIdClass())
    {
    }

    gsl::owner<ActionDestroyAction*> ActionDestroyAction::Clone() const
    {
        return new ActionDestroyAction(*this);
    }

    void ActionDestroyAction::Update(WorldState* worldState)
    {
        assert(m_parent != nullptr);
        worldState->AddDestroyActionRequest(m_actionName, *m_parent);
    }

    const Vector<Signature> ActionDestroyAction::Signatures()
    {
        return Vector<Signature>
        {
            { "Action", Datum::DatumType::String, 1, offsetof(ActionDestroyAction, m_actionName) }
        };
    }

}