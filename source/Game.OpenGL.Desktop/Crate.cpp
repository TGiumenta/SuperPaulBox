#include "pch.h"
#include "Crate.h"
#include "Game.h"
#include "EventMessageAttributed.h"

RTTI_DEFINITIONS(Crate)

Crate::Crate() : 
	PhysicsEntity(Crate::TypeIdClass(), ECollisionChannel::CRATES)
{
	SetTextureFile("Crate.png");
}

void Crate::Init(WorldState* state)
{
	PhysicsEntity::Init(state);
	SetOriginalPosition(GetPosition());
}

void Crate::OnCollisionStart(PhysicsEntity* other)
{
	if (other->GetTag() == "Player" && !IsPendingDestruction())
	{
		DestroyCrateEvent destroyCratePayload{ this };
		Event<DestroyCrateEvent> destroyCreateEvent(destroyCratePayload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
			(std::make_shared<Event<DestroyCrateEvent>>(destroyCreateEvent));

		Game::GetGame()->GetWorldState().DestroyEntity(this);

		EventMessageAttributed incrementScorePayload;
		incrementScorePayload.SetWorldState(Game::GetGame()->GetWorldState());
		incrementScorePayload.SetSubtype("IncrementScoreReaction");
		incrementScorePayload.AppendAuxiliaryAttribute("Target") = "Score";
		incrementScorePayload.AppendAuxiliaryAttribute("Step") = 1.f;
		Event<EventMessageAttributed> incrementScoreEvent(incrementScorePayload);
		Game::GetGame()->GetWorldState().GetEventQueue().Enqueue
		(std::make_shared<Event<EventMessageAttributed>>(incrementScoreEvent));
	}
}