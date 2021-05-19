#pragma once

#include "PhysicsEntity.h"
#include "Event.h"

using namespace FieaGameEngine;

class Weapon;

class Crate final : public PhysicsEntity
{
	RTTI_DECLARATIONS(Crate, PhysicsEntity)

public:
	/// <summary>
	/// Constructor for object initialization
	/// </summary>
	Crate();

	/// <summary>
	/// Defaulted copy constructor 
	/// </summary>
	/// <param name="other"> The crate to copy </param>
	Crate(const Crate& other) = default;

	/// <summary>
	/// Defaulted move constructor
	/// </summary>
	/// <param name="other"> The crate to move </param>
	Crate(Crate&& other) noexcept = default;

	/// <summary>
	/// Defaulted copy assignment operator
	/// </summary>
	/// <param name="other"> The crate to copy to "this" crate </param>
	/// <returns> The LHS of the = operator </returns>
	Crate& operator=(const Crate& other) = default;

	/// <summary>
	/// Defaulted move assignment operator
	/// </summary>
	/// <param name="other"> The crate to move to "this" crate </param>
	/// <returns> The LHS of the = operator </returns>
	Crate& operator=(Crate&& other) noexcept = default;

	/// <summary>
	/// Defaulted destructor
	/// </summary>
	~Crate() = default;

	/// <summary>
	/// Queries the original position of the crate
	/// </summary>
	/// <returns> The original position of the crate </returns>
	const glm::vec4& GetOriginalPosition() { return m_OriginalLocation; }

	/// <summary>
	/// Setter for the original position, should rarely be used
	/// </summary>
	/// <param name="location"> The position to set </param>
	void SetOriginalPosition(glm::vec4&& location) { m_OriginalLocation = location; }
	
	/// <summary>
	/// Initializes the crate by setting the data member for position
	/// </summary>
	/// <param name="state"> Contains reference to the delta time and the event queue if needed </param>
	virtual void Init(WorldState* state) override;

	/// <summary>
	/// The collision event that defines when another entity enters this one
	/// </summary>
	/// <param name="other"> The entity entering collision with this one </param>
	virtual void OnCollisionStart(PhysicsEntity* other) override;

	struct DestroyCrateEvent {
		Crate* crate;
	};

private:
	glm::vec4 m_OriginalLocation{};
};

ConcreteFactory(Crate, Scope)