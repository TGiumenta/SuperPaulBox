#pragma once
#include "Weapon.h"
#include "TypeManager.h"
#include "PhysicsEntity.h"

using namespace FieaGameEngine;

class Pistol final : public Weapon
{
	RTTI_DECLARATIONS(Pistol, Weapon);

public:
	/// <summary>
	/// Constructor for object initialization
	/// </summary>
	Pistol();

	/// <summary>
	/// Defaulted copy constructor 
	/// </summary>
	/// <param name="other"> The pistol to copy </param>
	Pistol(const Pistol& other) = default;

	/// <summary>
	/// Defaulted move constructor
	/// </summary>
	/// <param name="other"> The pistol to move </param>
	Pistol(Pistol&& other) noexcept = default;

	/// <summary>
	/// Defaulted copy assignment operator
	/// </summary>
	/// <param name="other"> The pistol to copy to "this" pistol </param>
	/// <returns> The LHS of the = operator </returns>
	Pistol& operator=(const Pistol& other) = default;

	/// <summary>
	/// Defaulted move assignment operator
	/// </summary>
	/// <param name="other"> The pistol to move to "this" pistol </param>
	/// <returns> The LHS of the = operator </returns>
	Pistol& operator=(Pistol&& other) noexcept = default;

	/// <summary>
	/// Defaulted destructor
	/// </summary>
	~Pistol() = default;

	/// <summary>
	/// Queries the prescribed attributes of the pistol
	/// </summary>
	/// <returns> The prescribed attributes of the pistol </returns>
	static const Vector<Signature> Signatures();

	/// <summary>
	/// The derived spawn projectile method which plays sound and calls the base method with the json file
	/// </summary>
	virtual void SpawnProjectile() override;

private:
	const std::string& m_pistolSound = "generic_shot.mp3";
};

ConcreteFactory(Pistol, Scope);