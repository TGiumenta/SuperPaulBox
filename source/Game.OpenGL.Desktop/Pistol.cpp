#include "pch.h"
#include "Pistol.h"
#include "AudioManager.h"
#include "Game.h"

RTTI_DEFINITIONS(Pistol)

Pistol::Pistol() :
	Weapon(Pistol::TypeIdClass())
{

}

const Vector<Signature> Pistol::Signatures()
{
	return Vector<Signature> {};
}

void Pistol::SpawnProjectile()
{
	if (Weapon::m_CanFire)
	{
		Weapon::SpawnProjectile("Projectile.json");

		AudioManager::PlaySoundEvent playSoundPayload{ m_pistolSound, false, false };
		Event<AudioManager::PlaySoundEvent> playSoundEvent(playSoundPayload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
		(std::make_shared<Event<AudioManager::PlaySoundEvent>>(playSoundEvent));

		Weapon::m_CanFire = false;
	}
}