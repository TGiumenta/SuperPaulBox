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

	int i = 0;
};

class CrateManager final : public Entity, public EventSubscriber
{
	RTTI_DECLARATIONS(CrateManager, Entity)

public:
	CrateManager();
	~CrateManager();

	virtual void Init(WorldState* state) override;

	static const Vector<Signature> Signatures();

	virtual void Notify(const EventPublisher& publisher) override;

	

private:
	Crate* SpawnCrate();
	Crate* SpawnCrate(const glm::vec4& originalPosition);

	static const size_t size = 8;
	glm::vec4 m_CratePositions[size]{};
};

ConcreteFactory(CrateManager, Scope)

