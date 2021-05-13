#pragma once
#include "Attributed.h"
#include "WorldState.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
	/// <summary>
	/// EventMessageAttributed class used as a payload to send information from 
	/// ActionEvents to ReactionAttributed
	/// </summary>
	class EventMessageAttributed : public Attributed
	{
		RTTI_DECLARATIONS(EventMessageAttributed, Attributed);

	public:
		/// <summary>
		/// Default constructor that passes the type ID to Attributed
		/// </summary>
		EventMessageAttributed();

        /// <summary>
        /// Default copy constructor
        /// </summary>
        /// <param name=""> The other EventMessageAttributed to copy from </param>
		EventMessageAttributed(const EventMessageAttributed& other) = default;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name=""> The other EventMessageAttributed to move from </param>
		EventMessageAttributed(EventMessageAttributed&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name=""> The other EventMessageAttributed to copy from </param>
        /// <returns> A reference to this EventMessageAttributed </returns>
		EventMessageAttributed& operator=(const EventMessageAttributed& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name=""> The other EventMessageAttributed to move from </param>
        /// <returns> A reference to this EventMessageAttributed </returns>
		EventMessageAttributed& operator=(EventMessageAttributed&& other) = default;

        /// <summary>
        /// Default destructor
        /// </summary>
        virtual ~EventMessageAttributed() = default;

		/// <summary>
		/// Gets the subtype of this message
		/// </summary>
		/// <returns> The subtype of this message </returns>
		inline const std::string& GetSubtype() const { return m_subtype; }

		/// <summary>
		/// Sets the subtype of this message
		/// </summary>
		/// <param name="subtype"> The subtype to set this message to </param>
		inline void SetSubtype(const std::string& subtype) { m_subtype = subtype; }

		/// <summary>
		/// Gets the WorldState associated with this message
		/// </summary>
		/// <returns> The WorldState associated with this message </returns>
		inline WorldState* GetWorldState() const { return m_worldState; }

		/// <summary>
		/// Sets the WorldState associated with this message
		/// </summary>
		/// <param name="worldState"> The WorldState to associate with this message </param>
		inline void SetWorldState(WorldState& worldState) { m_worldState = &worldState; }

        /// <summary>
        /// Retrieves the Signatures for this class, which describe its prescribed 
        /// attributes
        /// </summary>
        /// <returns> A Vector of Signatures representing the prescribed attributes
        /// of this class </returns>
		static const Vector<Signature> Signatures();

	private:
		/// <summary>
		/// The subtype of this message
		/// </summary>
		std::string m_subtype;

		/// <summary>
		/// The WorldState associated with this message
		/// </summary>
		WorldState* m_worldState;
	};
}

