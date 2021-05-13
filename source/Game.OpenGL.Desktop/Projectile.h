#pragma once

#include "Weapon.h"
#include "Event.h"
#include "PhysicsEntity.h"

using namespace FieaGameEngine;

class Projectile final : public PhysicsEntity
{
	RTTI_DECLARATIONS(Projectile, PhysicsEntity)

public:
	Projectile();
	~Projectile() = default;

	inline float CurrentDamage() { return m_CurrentDamage; }
	inline void SetCurrentDamage(float damage) { m_CurrentDamage = damage; }

	virtual void OnCollisionStart(PhysicsEntity* other) override;

private:
	
	struct DestroyProjectileEvent {
		Projectile* projectile;
	};

	glm::vec4 m_Velocity;
	float m_CurrentDamage = 0.f;
};

ConcreteFactory(Projectile, Scope)
