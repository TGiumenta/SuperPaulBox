#include "pch.h"
#include "Weapon.h"
#include "Player.h"
#include "Game.h"
#include "Projectile.h"

RTTI_DEFINITIONS(Weapon)

using namespace FieaGameEngine;

Weapon::Weapon(RTTI::IdType type) :
	Entity(type)
{

}

void Weapon::Update(WorldState* worldState)
{
	Entity::Update(worldState);
	if (!m_CanFire)
	{
		float deltaTime = Game::GetGame()->GetWorldState().GetDeltaTime();

		m_TimeAccumulated += deltaTime;
		if (m_TimeAccumulated >= FireRate())
		{
			m_TimeAccumulated = 0.f;
			m_CanFire = true;
		}
	}
}

const Vector<Signature> Weapon::Signatures()
{
	return Vector<Signature>
	{
		{ "FireRate", Datum::DatumType::Float, 1, offsetof(Weapon, m_FireRate) },
		{ "ProjectileDamage", Datum::DatumType::Float, 1, offsetof(Weapon, m_ProjectileDamage) },
		{ "ProjectileXSpeed", Datum::DatumType::Float, 1, offsetof(Weapon, m_ProjectileXSpeed) }
	};
}

Projectile* Weapon::SpawnProjectile(std::string&& fileName)
{
	Entity* entity = Game::GetGame()->GetWorldState().Instantiate("Projectile", fileName, GetPosition());
	Projectile* projectile = entity->As<Projectile>();
	assert(projectile != nullptr);
	projectile->SetCurrentDamage(m_ProjectileDamage);

	PhysicsEntity* projectilePhysics = entity->As<PhysicsEntity>();

	float facingModifier = (GetFacingDirection() == FacingDirection::FACING_RIGHT) ? 1.0f : -1.0f;

	projectilePhysics->SetLinearVelocity(b2Vec2(m_ProjectileXSpeed * facingModifier, 0));

	for (size_t index = 0; index < 5; ++index)
	{
		Player::CameraShake payload{ glm::vec3(0, (rand() % 18) - 9, 70) };
		Event<Player::CameraShake> event(payload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
		(std::make_shared<Event<Player::CameraShake>>(event), std::chrono::milliseconds(50*index));
	}

	Player::CameraShake payload{ RenderUtil::GetDefaultCameraPosition() };
	Event<Player::CameraShake> event(payload);
	Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
	(std::make_shared<Event<Player::CameraShake>>(event), std::chrono::milliseconds(250));

	return projectile;
}