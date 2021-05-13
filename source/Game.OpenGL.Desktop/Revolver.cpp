#include "pch.h"
#include "Revolver.h"
#include "AudioManager.h"
#include "Game.h"

RTTI_DEFINITIONS(Revolver)

Revolver::Revolver() :
	Weapon(Revolver::TypeIdClass())
{

}

const Vector<Signature> Revolver::Signatures()
{
	return Vector<Signature> {};
}

void Revolver::SpawnProjectile()
{
	if (Weapon::m_CanFire)
	{
		Weapon::SpawnProjectile("Projectile.json");

		AudioManager::PlaySoundEvent playSoundPayload{ m_revolverSound, false, false };
		Event<AudioManager::PlaySoundEvent> playSoundEvent(playSoundPayload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
		(std::make_shared<Event<AudioManager::PlaySoundEvent>>(playSoundEvent));

		Weapon::m_CanFire = false;
	}
}