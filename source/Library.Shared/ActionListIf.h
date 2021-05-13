#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	class ActionListIf final : public Action
	{
        RTTI_DECLARATIONS(ActionListIf, Action);

    public:
        /// <summary>
        /// Default constructor that passes this typeId to the Action constructor
        /// </summary>
        ActionListIf();

        /// <summary>
        /// Default copy constructor
        /// </summary>
        /// <param name="other"> The ActionListIf to copy </param>
        ActionListIf(const ActionListIf& other) = default;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name="other"> The ActionListIf to move </param>
        ActionListIf(ActionListIf&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name="other"> The ActionListIf to copy </param>
        ActionListIf& operator=(const ActionListIf& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name="other"> The ActionListIf to move </param>
        ActionListIf& operator=(ActionListIf&& other) = default;

        /// <summary>
        /// Default destructor
        /// </summary>
        ~ActionListIf() = default;

        /// <summary>
        /// Gets the condition value of this Action
        /// </summary>
        /// <returns> The condition value of this Action </returns>
        inline int GetCondition() const { return m_condition; }

        /// <summary>
        /// Sets the condition for this Action's If/Else decision. When set to 
        /// 0 it will take the else branch, when set to anything else it will
        /// take the Then branch
        /// </summary>
        /// <returns> The value to set the condition to </returns>
        inline void SetCondition(int condition) { m_condition = condition; }

        /// <summary>
        /// Creates a clone of this ActionListIf
        /// </summary>
        /// <returns> The newly heap-allocated ActionListIf </returns>
        gsl::owner<ActionListIf*> Clone() const override;

        /// <summary>
        /// Update function that either calls all of the nested actions in the 
        /// Then datum or the Else datum based on the current condition
        /// </summary>
        /// <param name="worldState"></param>
        void Update(WorldState * worldState) override;

        /// <summary>
        /// Update method that queues this creation with the passed in world state
        /// </summary>
        /// <param name="worldState"> The WorldState to queue this create in </param>
        static const Vector<Signature> Signatures();

    private:
        /// <summary>
        /// The condition that determines if this Action runs the Then or Else actions
        /// </summary>
        int m_condition;

        /// <summary>
        /// The index of the Then datum
        /// </summary>
        static const size_t m_thenIndex = 2;

        /// <summary>
        /// The index of the Else datum
        /// </summary>
        static const size_t m_elseIndex = 3;
	};

    ConcreteFactory(ActionListIf, Scope);
}

