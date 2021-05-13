#include "pch.h"
#include "ActionCreateAction.h"
#include "WorldState.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(ActionCreateAction);

    ActionCreateAction::ActionCreateAction()
        : Action(ActionCreateAction::TypeIdClass())
    {
    }

    gsl::owner<ActionCreateAction*> ActionCreateAction::Clone() const
    {
        return new ActionCreateAction(*this);
    }

    void ActionCreateAction::Update(WorldState* worldState)
    {
        assert(m_parent != nullptr);
        worldState->AddCreateActionRequest(m_className, m_actionName, *m_parent);
    }

    const Vector<Signature> ActionCreateAction::Signatures()
    {
        return Vector<Signature>
        {
            { "ClassName", Datum::DatumType::String, 1, offsetof(ActionCreateAction, m_className) },
            { "ActionName", Datum::DatumType::String, 1, offsetof(ActionCreateAction, m_actionName) }
        };
    }
}