#pragma once
#include "Weapon.h"
#include "TypeManager.h"
#include "PhysicsEntity.h"

using namespace FieaGameEngine;

class MachineGun final : public Weapon
{
	RTTI_DECLARATIONS(MachineGun, Weapon);

public:
	/// <summary>
	/// Constructor for object initialization
	/// </summary>
	MachineGun();

	/// <summary>
	/// Defaulted copy constructor 
	/// </summary>
	/// <param name="other"> The machinegun to copy </param>
	MachineGun(const MachineGun& other) = default;

	/// <summary>
	/// Defaulted move constructor
	/// </summary>
	/// <param name="other"> The machinegun to move </param>
	MachineGun(MachineGun&& other) noexcept = default;

	/// <summary>
	/// Defaulted copy assignment operator
	/// </summary>
	/// <param name="other"> The machinegun to copy to "this" machinegun </param>
	/// <returns> The LHS of the = operator </returns>
	MachineGun& operator=(const MachineGun& other) = default;

	/// <summary>
	/// Defaulted move assignment operator
	/// </summary>
	/// <param name="other"> The machinegun to move to "this" machinegun </param>
	/// <returns> The LHS of the = operator </returns>
	MachineGun& operator=(MachineGun&& other) noexcept = default;

	/// <summary>
	/// Defaulted destructor
	/// </summary>
	~MachineGun() = default;

	/// <summary>
	/// Queries the prescribed attributes of the machinegun
	/// </summary>
	/// <returns> The prescribed attributes of the machinegun </returns>
	static const Vector<Signature> Signatures();

	/// <summary>
	/// The derived spawn projectile method which plays sound and calls the base method with the json file
	/// </summary>
	virtual void SpawnProjectile() override;

private:
	const std::string& m_machingGunSound = "ak.mp3";
};

ConcreteFactory(MachineGun, Scope);