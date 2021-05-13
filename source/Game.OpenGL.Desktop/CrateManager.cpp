#include "pch.h"
#include "CrateManager.h"
#include "Crate.h"
#include "Game.h"

RTTI_DEFINITIONS(CrateManager);
RTTI_DEFINITIONS(SwapWeaponEvent);

CrateManager::CrateManager() :
	Entity(CrateManager::TypeIdClass())
{
	Event<Crate::DestroyCrateEvent>::Subscribe(*this);
}

CrateManager::~CrateManager()
{
	Event<Crate::DestroyCrateEvent>::Unsubscribe(*this);
}

void CrateManager::Init(WorldState*)
{
	SpawnCrate();
}

const Vector<Signature> CrateManager::Signatures()
{
	return Vector<Signature>
	{
		{ "PossiblePositions", Datum::DatumType::Vector, size, offsetof(CrateManager, m_CratePositions)},
	};
}

void CrateManager::Notify(const EventPublisher& publisher)
{
	const Event<Crate::DestroyCrateEvent>* event = publisher.As<Event<Crate::DestroyCrateEvent>>();
	if (event)
	{
		SpawnCrate(event->Message().crate->GetOriginalPosition());
	}
}

Crate* CrateManager::SpawnCrate()
{
	Entity* entity = Game::GetGame()->GetWorldState().Instantiate("Crate", "Crate.json");
	return entity->As<Crate>();
}

Crate* CrateManager::SpawnCrate(const glm::vec4& originalPosition)
{
	size_t randomIndex = rand() % size;
	while (m_CratePositions[randomIndex] == originalPosition)
	{
		randomIndex = rand() % size;
	}

	Entity* entity = Game::GetGame()->GetWorldState().Instantiate("Crate", "Crate.json", m_CratePositions[randomIndex]);
	SwapWeaponEvent payload{};
	Event<SwapWeaponEvent> event(payload);
	//event.Deliver();
    Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
        (std::make_shared<Event<SwapWeaponEvent>>(event));

	Crate* crate = entity->As<Crate>();
	assert(crate != nullptr);

	return crate;
}
