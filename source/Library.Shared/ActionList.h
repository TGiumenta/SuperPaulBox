#pragma once
#include "Action.h"
namespace FieaGameEngine
{
	class ActionList : public Action
	{
		RTTI_DECLARATIONS(ActionList, Action);

	public:
		/// <summary>
		/// ActionList constructor that passes the typeID up to the Action constructor
		/// </summary>
		/// <param name="type"> The typeId to pass to the Action constructor </param>
		ActionList(RTTI::IdType type);

		/// <summary>
		/// Default constructor that passes this typeID to the Action constructor
		/// </summary>
		ActionList();

		/// <summary>
		/// Default copy constructor
		/// </summary>
		/// <param name="other"> The ActionList to copy </param>
		ActionList(const ActionList& other) = default;

        /// <summary>
        /// Default move constructor
        /// </summary>
        /// <param name="other"> The ActionList to move </param>
		ActionList(ActionList&& other) = default;

        /// <summary>
        /// Default copy assignment
        /// </summary>
        /// <param name="other"> The ActionList to copy </param>
		ActionList& operator=(const ActionList& other) = default;

        /// <summary>
        /// Default move assignment
        /// </summary>
        /// <param name="other"> The ActionList to move </param>
		ActionList& operator=(ActionList&& other) = default;

        /// <summary>
        /// Default destructor
        /// </summary>
        virtual ~ActionList() = default;

		/// <summary>
		/// Update method that calls update on all nested Actions
		/// </summary>
		/// <param name="worldState"> The current WorldState </param>
		virtual void Update(WorldState* worldState) override;

		/// <summary>
		/// Creates a new action and adopts into this Actions datum
		/// </summary>
		/// <param name="className"> Class name of the action to create </param>
		/// <param name="instanceName"> Name of the action </param>
		Action* CreateAction(const std::string& className, const std::string& instanceName);

        /// <summary>
        /// Retrieves the Signatures for this class, which describe its prescribed 
        /// attributes
        /// </summary>
        /// <returns> A Vector of Signatures representing the prescribed attributes
        /// of this class </returns>
		static const Vector<Signature> Signatures();

	private:
		/// <summary>
		/// The index of the child actions datum in our order vector
		/// </summary>
		static const size_t m_childrenIndex = 2;
	};

}
