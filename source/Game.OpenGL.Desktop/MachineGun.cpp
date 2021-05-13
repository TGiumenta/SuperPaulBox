#include "pch.h"
#include "MachineGun.h"
#include "AudioManager.h"
#include "Game.h"

RTTI_DEFINITIONS(MachineGun)

MachineGun::MachineGun() :
	Weapon(MachineGun::TypeIdClass())
{

}

const Vector<Signature> MachineGun::Signatures()
{
	return Vector<Signature> {};
}

void MachineGun::SpawnProjectile()
{
	if (Weapon::m_CanFire)
	{
		Weapon::SpawnProjectile("Projectile.json");

		AudioManager::PlaySoundEvent playSoundPayload{ m_machingGunSound, false, false };
		Event<AudioManager::PlaySoundEvent> playSoundEvent(playSoundPayload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
		(std::make_shared<Event<AudioManager::PlaySoundEvent>>(playSoundEvent));

		Weapon::m_CanFire = false;
	}
}