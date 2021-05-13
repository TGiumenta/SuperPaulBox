#pragma once
#include "Action.h"

namespace FieaGameEngine
{
	/// <summary>
	/// ActionEvent class responsible for creating EventMessageAttributed payloads
	/// and delivering them to any subscribers
	/// </summary>
	class ActionEvent : public Action
	{
		RTTI_DECLARATIONS(ActionEvent, Action);

	public:
		/// <summary>
		/// Default constructor that passes the type ID to Attributed
		/// </summary>
		ActionEvent();

        /// <summary>
        /// Default copy constructor
        /// </summary>
        /// <param name=""> The other ActionEvent to copy from </param>
		ActionEvent(const ActionEvent& other) = default;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name=""> The other ActionEvent to move from </param>
		ActionEvent(ActionEvent&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name=""> The other ActionEvent to copy from </param>
        /// <returns> A reference to this ActionEvent </returns>
		ActionEvent& operator=(const ActionEvent& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name=""> The other ActionEvent to move from </param>
        /// <returns> A reference to this ActionEvent </returns>
		ActionEvent& operator=(ActionEvent&& other) = default;

		/// <summary>
		/// Default destructor
		/// </summary>
        virtual ~ActionEvent() = default;

		/// <summary>
		/// Update method that creates an EventMessageAttributed, sets its subtype to the 
		/// subtype of this ActionEvent and then delivers it
		/// </summary>
		/// <param name="worldState"> The world state to use </param>
		virtual void Update(WorldState* worldState) override;

        /// <summary>
        /// Retrieves the Signatures for this class, which describe its prescribed 
        /// attributes
        /// </summary>
        /// <returns> A Vector of Signatures representing the prescribed attributes
        /// of this class </returns>
		static const Vector<Signature> Signatures();

		/// <summary>
		/// Sets the subtype of this ActionEvent
		/// </summary>
		/// <param name="subtype"> The subtype to set this ActionEvent to </param>
		void SetSubtype(const std::string& subtype) { m_subtype = subtype; }

		/// <summary>
		/// Gets the subtype of this ActionEvent
		/// </summary>
		/// <returns> The subtype of this ActionEvent </returns>
		const std::string& GetSubtype() const { return m_subtype; }

	private:

		/// <summary>
		/// The subtype of this action event
		/// </summary>
		std::string m_subtype;

		/// <summary>
		/// The delay for events that this ActionEvent enqueues
		/// </summary>
		int m_delay = 0;
	};

	ConcreteFactory(ActionEvent, Action);
}

