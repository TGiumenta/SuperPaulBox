#include "pch.h"
#include "Shotgun.h"
#include "Projectile.h"
#include "AudioManager.h"
#include "Game.h"

RTTI_DEFINITIONS(Shotgun)

Shotgun::Shotgun() :
	Weapon(Shotgun::TypeIdClass())
{

}

const Vector<Signature> Shotgun::Signatures()
{
	return Vector<Signature> {};
}

void Shotgun::SpawnProjectile()
{
	if (Weapon::m_CanFire)
	{
		for (size_t index = 0; index < 8; ++index)
		{
			Projectile* projectile = Weapon::SpawnProjectile("Projectile.json");
			if (projectile != nullptr)
			{
				float startingXVelocity = projectile->GetLinearVelocity().x;
				startingXVelocity = startingXVelocity + (rand() % 5);
				float yVelocity = (rand() % 20) - 10.0f;
				projectile->SetLinearVelocity(b2Vec2(startingXVelocity, yVelocity));
			}
		}

		AudioManager::PlaySoundEvent playSoundPayload{ m_shotgunShoot, false, false };
		Event<AudioManager::PlaySoundEvent> playSoundEvent(playSoundPayload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
		(std::make_shared<Event<AudioManager::PlaySoundEvent>>(playSoundEvent));


		AudioManager::PlaySoundEvent playSoundPayloadReload{ m_shotgunReload, false, false };
		Event<AudioManager::PlaySoundEvent> playSoundEventReload(playSoundPayloadReload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
		(std::make_shared<Event<AudioManager::PlaySoundEvent>>(playSoundEventReload), std::chrono::milliseconds(750));

		Weapon::m_CanFire = false;
	}
}