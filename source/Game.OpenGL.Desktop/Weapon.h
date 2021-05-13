#pragma once
#include "Entity.h"
#include "Vector.h"
#include "TypeManager.h"

using namespace FieaGameEngine;

class Weapon : public Entity
{
	RTTI_DECLARATIONS(Weapon, Entity)

public:
	Weapon() = delete;
	virtual ~Weapon() = 0 {};

	virtual void Update(WorldState* worldState) override;

	virtual void SpawnProjectile() = 0 {};

	static const Vector<Signature> Signatures();
	inline float FireRate() { return m_FireRate; }

protected:
	Weapon(RTTI::IdType type);

	class Projectile* SpawnProjectile(std::string&& fileName);

	bool m_CanFire = true;
	float m_ProjectileDamage = 0.f;
	float m_ProjectileXSpeed = 0.f;

private:
	float m_FireRate = 0.f;
	float m_TimeAccumulated = 0.f;
};