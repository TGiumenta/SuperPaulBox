#pragma once
#include "Action.h"
namespace FieaGameEngine
{
	class ActionCreateAction final : public Action
	{
		RTTI_DECLARATIONS(ActionCreateAction, Action);

	public:
		/// <summary>
		/// Default constructor that passes the ActionCreateAction typeID to Action's constructor
		/// </summary>
		ActionCreateAction();

		/// <summary>
		/// Default copy constructor
		/// </summary>
		/// <param name="other"> The ActionCreateAction to copy </param>
		ActionCreateAction(const ActionCreateAction& other) = default;

		/// <summary>
		/// Default move constructor
		/// </summary>
		/// <param name="other"> The ActionCreateAction to move </param>
		ActionCreateAction(ActionCreateAction&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name="other"> The ActionCreateAction to copy </param>
		ActionCreateAction& operator=(const ActionCreateAction& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name="other"> The ActionCreateAction to move </param>
		ActionCreateAction& operator=(ActionCreateAction&& other) = default;

		/// <summary>
		/// Default destructor
		/// </summary>
		~ActionCreateAction() = default;

		/// <summary>
		/// Gets the name of the class that this action will create
		/// </summary>
		/// <returns> The name of the class that this action will create </returns>
		inline std::string GetClassName() const { return m_className; }

		/// <summary>
		/// Sets the name of the class that this action will create
		/// </summary>
		/// <param name="className"> The name of the class for this action to create </param>
		inline void SetClassName(const std::string& className) { m_className = className; }

		/// <summary>
		/// Gets the name that the created Action will have
		/// </summary>
		/// <returns> The name that the created Action will have </returns>
		inline std::string GetActionName() const { return m_actionName; }

		/// <summary>
		/// Sets the name that the created Action will have
		/// </summary>
		/// <param name="actionName"> The name for the created Action to have </param>
		inline void SetActionName(const std::string& actionName) { m_actionName = actionName; }

		/// <summary>
		/// Creates a clone of this ActionCreateAction
		/// </summary>
		/// <returns> The newly heap-allocated ActionCreateAction </returns>
        gsl::owner<ActionCreateAction*> Clone() const override;

		/// <summary>
		/// Update method that queues this creation with the passed in world state
		/// </summary>
		/// <param name="worldState"> The WorldState to queue this create in </param>
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
		/// The name of the Action class to create
		/// </summary>
		std::string m_className;

		/// <summary>
		/// The name to give the Action once it is created
		/// </summary>
		std::string m_actionName;
	};

	ConcreteFactory(ActionCreateAction, Scope);
}
