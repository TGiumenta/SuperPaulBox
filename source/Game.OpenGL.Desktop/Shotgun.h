#pragma once

#include "Weapon.h"

using namespace FieaGameEngine;

class Shotgun final : public Weapon
{
	RTTI_DECLARATIONS(Shotgun, Weapon);

public:
	/// <summary>
	/// Constructor for object initialization
	/// </summary>
	Shotgun();

	/// <summary>
	/// Defaulted copy constructor 
	/// </summary>
	/// <param name="other"> The shotgun to copy </param>
	Shotgun(const Shotgun& other) = default;

	/// <summary>
	/// Defaulted move constructor
	/// </summary>
	/// <param name="other"> The shotgun to move </param>
	Shotgun(Shotgun&& other) noexcept = default;

	/// <summary>
	/// Defaulted copy assignment operator
	/// </summary>
	/// <param name="other"> The shotgun to copy to "this" shotgun </param>
	/// <returns> The LHS of the = operator </returns>
	Shotgun& operator=(const Shotgun& other) = default;

	/// <summary>
	/// Defaulted move assignment operator
	/// </summary>
	/// <param name="other"> The shotgun to move to "this" shotgun </param>
	/// <returns> The LHS of the = operator </returns>
	Shotgun& operator=(Shotgun&& other) noexcept = default;

	/// <summary>
	/// Defaulted destructor
	/// </summary>
	~Shotgun() = default;

	/// <summary>
	/// Queries the prescribed attributes of the shotgun
	/// </summary>
	/// <returns> The prescribed attributes of the shotgun </returns>
	static const Vector<Signature> Signatures();

	/// <summary>
	/// The derived spawn projectile method which plays sound and calls the base method with the json file
	/// Also defined the random y-component of the velocity for each projectile
	/// </summary>
	virtual void SpawnProjectile() override;

private:
	const std::string& m_shotgunShoot = "shotty.mp3";
	const std::string& m_shotgunReload = "shotty reload.mp3";
};

ConcreteFactory(Shotgun, Scope);
