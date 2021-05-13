#include "pch.h"
#include "ActionListIf.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(ActionListIf);

    ActionListIf::ActionListIf()
        : Action(ActionListIf::TypeIdClass())
    {
    }

    gsl::owner<ActionListIf*> ActionListIf::Clone() const
    {
        return new ActionListIf(*this);
    }

    void ActionListIf::Update(WorldState* worldState)
    {
        if (m_condition)
        {
            Datum& thenActions = m_order[m_thenIndex]->second;

            for (size_t i = 0; i < thenActions.Size(); ++i)
            {
                Scope* scopeChild = &thenActions.Get<Scope>(i);
                assert(scopeChild->Is(Action::TypeIdClass()));
                Action& child = static_cast<Action&>(*scopeChild);
                child.Update(worldState);
            }
        }
        else
        {
            Datum& elseActions = m_order[m_elseIndex]->second;

            for (size_t i = 0; i < elseActions.Size(); ++i)
            {
                Scope* scopeChild = &elseActions.Get<Scope>(i);
                assert(scopeChild->Is(Action::TypeIdClass()));
                Action& child = static_cast<Action&>(*scopeChild);
                child.Update(worldState);
            }
        }
    }


    const Vector<Signature> ActionListIf::Signatures()
    {
        return Vector<Signature>
        {
            { "Then", Datum::DatumType::Table, 0, 0 },
            { "Else", Datum::DatumType::Table, 0, 0 }
        };
    }
}