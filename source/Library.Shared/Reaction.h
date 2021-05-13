#pragma once
#include "Event.h"
#include "ActionList.h"

namespace FieaGameEngine
{
	/// <summary>
	/// Reaction class that serves as a base class for all Reactions
	/// </summary>
	class Reaction : public EventSubscriber, public ActionList
	{
		RTTI_DECLARATIONS(Reaction, ActionList)
	public:
		/// <summary>
		/// Constructor that passes the given type ID up to ActionList
		/// </summary>
		Reaction(RTTI::IdType type);

		/// <summary>
		/// Empty update method so that derived classes can easily do 
		/// nothing by just inheriting from this class
		/// </summary>
		virtual void Update(WorldState*) override {};

	};
}

