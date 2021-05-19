#pragma once

#include "Entity.h"
#include "Vector.h"
#include "TypeManager.h"
#include "Event.h"

using namespace FieaGameEngine;

class Crate;

struct SwapWeaponEvent : public RTTI
{
	RTTI_DECLARATIONS(SwapWeaponEvent, RTTI)
};

class CrateManager final : public Entity, public EventSubscriber
{
	RTTI_DECLARATIONS(CrateManager, Entity)

public:
	/// <summary>
	/// Constructor for object initialization and subscribes from the events
	/// </summary>
	CrateManager();

	/// <summary>
	/// Deleted copy constructor 
	/// </summary>
	/// <param name="other"> The cratemanager to copy </param>
	CrateManager(const CrateManager& other) = delete;

	/// <summary>
	/// Deleted move constructor
	/// </summary>
	/// <param name="other"> The cratemanager to move </param>
	CrateManager(CrateManager&& other) noexcept = delete;

	/// <summary>
	/// Deleted copy assignment operator
	/// </summary>
	/// <param name="other"> The cratemanager to copy to "this" cratemanager </param>
	/// <returns> The LHS of the = operator </returns>
	CrateManager& operator=(const CrateManager& other) = delete;

	/// <summary>
	/// Deleted move assignment operator
	/// </summary>
	/// <param name="other"> The cratemanager to move to "this" cratemanager </param>
	/// <returns> The LHS of the = operator </returns>
	CrateManager& operator=(CrateManager&& other) noexcept = delete;

	/// <summary>
	/// Destroys the crate manager and unsubscribes from the events
	/// </summary>
	~CrateManager();

	/// <summary>
	/// Initializes the manager by spawning the first crate
	/// </summary>
	/// <param name="state"> An unused parameter but required because it is virtual </param>
	virtual void Init(WorldState* state) override;

	/// <summary>
	/// Queries the prescribed attributes of the cratemanager
	/// </summary>
	/// <returns> The prescribed attributes of the cratemanager </returns>
	static const Vector<Signature> Signatures();

	/// <summary>
	/// The implementation of the events that cratemanager subscribes to
	/// </summary>
	/// <param name="publisher"> The event to publish and enact </param>
	virtual void Notify(const EventPublisher& publisher) override;

private:
	Crate* SpawnCrate();
	Crate* SpawnCrate(const glm::vec4& originalPosition);

	static const size_t size = 8;
	glm::vec4 m_CratePositions[size]{};
};

ConcreteFactory(CrateManager, Scope)

