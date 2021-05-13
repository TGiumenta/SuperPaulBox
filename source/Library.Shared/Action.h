#pragma once
#include "Attributed.h"
#include "TypeManager.h"

namespace FieaGameEngine
{
    class WorldState;

    class Action : public Attributed
    {
        RTTI_DECLARATIONS(Action, Attributed);

    public:
        /// <summary>
        /// Constructor that passes the passed in typeID up to the attributed constructor
        /// </summary>
        /// <param name="type"> The typeID to pass to attributed </param>
        Action(RTTI::IdType type);

        /// <summary>
        /// Default copy constructor 
        /// </summary>
        /// <param name="other"> The Action to copy </param>
        Action(const Action& other) = default;

        /// <summary>
        /// Default move constructor 
        /// </summary>
        /// <param name="other"> The Action to move </param>
        Action(Action&& other) = default;

        /// <summary>
        /// Default copy assignment 
        /// </summary>
        /// <param name="other"> The Action to copy </param>
        Action& operator=(const Action& other) = default;

        /// <summary>
        /// Default move assignment 
        /// </summary>
        /// <param name="other"> The Action to move </param>
        Action& operator=(Action&& other) = default;

        /// <summary>
        /// Default destructor
        /// </summary>
        virtual ~Action() = default;

        /// <summary>
        /// Pure virtual Update method, all inherited classes must define this
        /// </summary>
        /// <param name="worldState"> The current WorldState </param>
        virtual void Update(WorldState* worldState) = 0;

        /// <summary>
        /// Gets the name of this Action
        /// </summary>
        /// <returns> The name of this Action </returns>
        inline std::string GetName() const { return m_name; };

        /// <summary>
        /// Sets the name of this Action
        /// </summary>
        /// <returns> The name to set this Action to </returns>
        inline void SetName(const std::string& name) { m_name = name; };

        /// <summary>
        /// Retrieves the Signatures for this class, which describe its prescribed 
        /// attributes
        /// </summary>
        /// <returns> A Vector of Signatures representing the prescribed attributes
        /// of this class </returns>
        static const Vector<Signature> Signatures();

    private:
        /// <summary>
        /// The name of this Action
        /// </summary>
        std::string m_name;
    };
}

