#pragma once
#include "Entity.h"
#include "Vector.h"
#include "TypeManager.h"

using namespace FieaGameEngine;

class Weapon : public Entity
{
	RTTI_DECLARATIONS(Weapon, Entity)

public:
	/// <summary>
	/// Deleted default constructor to prevent default initialization
	/// </summary>
	Weapon() = delete;

	/// <summary>
	/// Deleted copy constructor 
	/// </summary>
	/// <param name="other"> The weapon to copy </param>
	Weapon(const Weapon& other) = delete;

	/// <summary>
	/// Deleted move constructor
	/// </summary>
	/// <param name="other"> The weapon to move </param>
	Weapon(Weapon&& other) noexcept = delete;

	/// <summary>
	/// Deleted copy assignment operator
	/// </summary>
	/// <param name="other"> The weapon to copy to "this" weapon </param>
	/// <returns> The LHS of the = operator </returns>
	Weapon& operator=(const Weapon& other) = delete;

	/// <summary>
	/// Deleted move assignment operator
	/// </summary>
	/// <param name="other"> The weapon to move to "this" weapon </param>
	/// <returns> The LHS of the = operator </returns>
	Weapon& operator=(Weapon&& other) noexcept = delete;
	
	/// <summary>
	/// Pure virtual destructor to create abstract base class
	/// </summary>
	virtual ~Weapon() = 0 {};

	/// <summary>
	/// The method called on every game loop dependent on the rendered weapon
	/// </summary>
	/// <param name="worldState"> Contains reference to the delta time and the event queue if needed </param>
	virtual void Update(WorldState* worldState) override;

	/// <summary>
	/// The pure virtual method that spawns projectiles - intended to be derived by children
	/// </summary>
	virtual void SpawnProjectile() = 0 {};

	/// <summary>
	/// Queries the prescribed attributes of the weapon
	/// </summary>
	/// <returns> The prescribed attributes of the weapon </returns>
	static const Vector<Signature> Signatures();

	/// <summary>
	/// The amount of time between bullets on the weapon
	/// </summary>
	/// <returns> The data member float that's a prescribed attribute </returns>
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