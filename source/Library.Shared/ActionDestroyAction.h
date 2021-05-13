#pragma once
#include "Action.h"


namespace FieaGameEngine
{
	class ActionDestroyAction final : public Action
	{
		RTTI_DECLARATIONS(ActionDestroyAction, Action);

	public:
        /// <summary>
        /// Default constructor that passes the ActionDestroyAction typeID to Action's constructor
        /// </summary>
		ActionDestroyAction();

        /// <summary>
        /// Default copy constructor
        /// </summary>
        /// <param name="other"> The ActionDestroyAction to copy </param>
        ActionDestroyAction(const ActionDestroyAction& other) = default;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name="other"> The ActionDestroyAction to move </param>
        ActionDestroyAction(ActionDestroyAction&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name="other"> The ActionDestroyAction to copy </param>
        ActionDestroyAction& operator=(const ActionDestroyAction& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name="other"> The ActionDestroyAction to move </param>
        ActionDestroyAction& operator=(ActionDestroyAction&& other) = default;

        /// <summary>
        /// Default destructor
        /// </summary>
		~ActionDestroyAction() = default;

        /// <summary>
        /// Gets the name of the target of this Action's destruction
        /// </summary>
        /// <returns> The name of the target of this Action's destruction </returns>
        inline std::string GetTarget() const { return m_actionName; }

        /// <summary>
        /// Sets the name of the target of this Action's destruction
        /// </summary>
        /// <returns> The name to make the target of this Action's destruction </returns>
		inline void SetTarget(const std::string& targetName) { m_actionName = targetName; }

        /// <summary>
        /// Creates a clone of this ActionDestroyAction
        /// </summary>
        /// <returns> The newly heap-allocated ActionDestroyAction </returns>
        gsl::owner<ActionDestroyAction*> Clone() const override;

        /// <summary>
        /// Update method that queues this deletion with the passed in world state
        /// </summary>
        /// <param name="worldState"> The WorldState to queue this delete in </param>
        void Update(WorldState* worldState) override;

        /// <summary>
        /// Retrieves the Signatures for this class, which describe its prescribed 
        /// attributes
        /// </summary>
        /// <returns> A Vector of Signatures representing the prescribed attributes
        /// of this class </returns>
		static const Vector<Signature> Signatures();

	private:
        /// <summary>
        /// The name of the action to delete
        /// </summary>
		std::string m_actionName;

	};

	ConcreteFactory(ActionDestroyAction, Scope);
}

