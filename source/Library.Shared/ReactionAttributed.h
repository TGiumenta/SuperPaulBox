#pragma once
#include "Reaction.h"
#include "Factory.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Reaction Attributed class that allows creating reactions through JSON
	/// </summary>
	class ReactionAttributed : public Reaction
	{
		RTTI_DECLARATIONS(ReactionAttributed, Reaction);

	public:
		/// <summary>
		/// Default constructor that subscribes this ReactionAttributed to events
		/// of type EventMessageAttributed
		/// </summary>
		/// <param name="subtype"> Optional parameter that defines the subtype of
		/// this ReactionAttributed </param>
		explicit ReactionAttributed(const std::string& subtype = "");

        /// <summary>
        /// Default copy constructor
        /// </summary>
        /// <param name=""> The other ReactionAttributed to copy from </param>
		ReactionAttributed(const ReactionAttributed& other) = default;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name=""> The other ReactionAttributed to move from </param>
		ReactionAttributed(ReactionAttributed&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name=""> The other ReactionAttributed to copy from </param>
        /// <returns> A reference to this ReactionAttributed </returns>
		ReactionAttributed& operator=(const ReactionAttributed& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name=""> The other ReactionAttributed to move from </param>
        /// <returns> A reference to this ReactionAttributed </returns>
		ReactionAttributed& operator=(ReactionAttributed&& other) = default;

		/// <summary>
		/// Default destructor that unsubscribes this ReactionAttributed from
		/// events of type EventMessageAttributed
		/// </summary>
		~ReactionAttributed();

		/// <summary>
		/// Handles the case where an event we are subscribed to is delivered. 
		/// In this case, specifically looks for EventMessageAttributed and checks its
		/// subtype against our own, calling update on our contained ActionList if there
		/// is a match
		/// </summary>
		/// <param name="event"></param>
		void Notify(const EventPublisher& event) override;

		/// <summary>
		/// Gets the subtype of this ReactionAttributed
		/// </summary>
		/// <returns> The subtype of this ReactionAttributed </returns>
		const std::string& GetSubtype() const { return m_subtype; }

        /// <summary>
        /// Retrieves the Signatures for this class, which describe its prescribed 
        /// attributes
        /// </summary>
        /// <returns> A Vector of Signatures representing the prescribed attributes
        /// of this class </returns>
        static const Vector<Signature> Signatures();

	private:
		/// <summary>
		/// The subtype of this ReactionAttributed
		/// </summary>
		std::string m_subtype;
	};

	ConcreteFactory(ReactionAttributed, Scope);
}
