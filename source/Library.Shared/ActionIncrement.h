#pragma once
#include "Action.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	class ActionIncrement final : public Action
	{
		RTTI_DECLARATIONS(ActionIncrement, Action);

	public:
		/// <summary>
		/// Default constructor that passes the typeID to the Action constructor
		/// </summary>
		ActionIncrement();

		/// <summary>
		/// Default destructor
		/// </summary>
		~ActionIncrement() = default;

		/// <summary>
		/// Gets the increment target of this Action
		/// </summary>
		/// <returns> The increment target of this Action </returns>
		inline const std::string& GetTarget() const { return m_target; }

		/// <summary>
		/// Sets the increment target of this Action
		/// </summary>
		/// <param name="target"> The new increment target of this Action </param>
		inline void SetTarget(const std::string& target) { m_target = target; }

		/// <summary>
		/// Gets the increment step of this Action
		/// </summary>
		/// <returns> The increment step of this Action </returns>
		inline float GetStep() const { return m_step; }

		/// <summary>
		/// Sets the increment step of this Action
		/// </summary>
		/// <param name="step"> The increment to set the step of this Action to </param>
		inline void SetStep(float step) { m_step = step; }

        /// <summary>
        /// Creates a clone of this ActionIncrement
        /// </summary>
        /// <returns> The newly heap-allocated ActionIncrement </returns>
		gsl::owner<ActionIncrement*> Clone() const override;

		/// <summary>
		/// Updates the target by incrementing it by step amount
		/// </summary>
		/// <param name="worldState"> The current world state </param>
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
		/// The target to increment on update 
		/// </summary>
		std::string m_target;

		/// <summary>
		/// The amount to increment the target by
		/// </summary>
		float m_step = 1.0f;
	};

	ConcreteFactory(ActionIncrement, Scope);
}

