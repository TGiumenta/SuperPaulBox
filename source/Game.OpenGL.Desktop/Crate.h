#pragma once

#include "PhysicsEntity.h"
#include "Event.h"

using namespace FieaGameEngine;

class Weapon;

class Crate final : public PhysicsEntity
{
	RTTI_DECLARATIONS(Crate, PhysicsEntity)

public:
	Crate();
	~Crate() = default;

	const glm::vec4& GetOriginalPosition() { return m_OriginalLocation; }
	void SetOriginalPosition(glm::vec4&& location) { m_OriginalLocation = location; }
	virtual void Init(WorldState* state) override;

	virtual void OnCollisionStart(PhysicsEntity* other) override;

	struct DestroyCrateEvent {
		Crate* crate;
	};

private:
	glm::vec4 m_OriginalLocation{};
	Weapon* m_HeldWeapon;
};

ConcreteFactory(Crate, Scope)