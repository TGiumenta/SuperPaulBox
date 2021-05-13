#include "pch.h"
#include "ActionList.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(ActionList);

    ActionList::ActionList(RTTI::IdType type)
        : Action(type)
    {
    }

    ActionList::ActionList()
        : Action(ActionList::TypeIdClass())
    {
    }

    void ActionList::Update(WorldState* worldState)
    {
        Datum& children = m_order[m_childrenIndex]->second;

        for (size_t i = 0; i < children.Size(); ++i)
        {
            Scope* scopeChild = &children.Get<Scope>(i);
            assert(scopeChild->Is(Action::TypeIdClass()));
            Action& child = static_cast<Action&>(*scopeChild);
            child.Update(worldState);
        }
    }

	Action* ActionList::CreateAction(const std::string& className, const std::string& instanceName)
	{
		Scope* createdScope = Factory<Scope>::Create(className);
		if (createdScope == nullptr)
		{
			throw std::exception("Trying to create an action whose class doesn't exist!");
		}

		Action* actionPtr = createdScope->As<Action>();
		if (actionPtr == nullptr)
		{
			delete createdScope;
			throw std::exception("Trying to create an action with a non-action class name!");
		}

		Adopt(*actionPtr, "Actions");
		actionPtr->SetName(instanceName);

		return actionPtr;
	}

    const Vector<Signature> ActionList::Signatures()
    {
        return Vector<Signature>
        {
            { "Actions", Datum::DatumType::Table, 0, 0 }
        };
    }

}