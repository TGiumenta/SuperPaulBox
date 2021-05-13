#include "pch.h"
#include "WorldContactListener.h"
#include "PhysicsEntity.h"

namespace FieaGameEngine
{
    void WorldContactListener::BeginContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // If one of the bodies is nullptr
        if (!bodyA || !bodyB) { return; }

        // Assuming all bodies in the game are PhysicsEntities
        PhysicsEntity* entityA = reinterpret_cast<PhysicsEntity*>(bodyA->GetUserData().pointer);
        PhysicsEntity* entityB = reinterpret_cast<PhysicsEntity*>(bodyB->GetUserData().pointer);

        // If this is blowing up, there is a b2body not correctly connected to a PhysicsEntity
        entityA->OnCollisionStart(entityB);
        entityB->OnCollisionStart(entityA);

    }

    void WorldContactListener::EndContact(b2Contact* contact)
    {
        b2Body* bodyA = contact->GetFixtureA()->GetBody();
        b2Body* bodyB = contact->GetFixtureB()->GetBody();

        // If one of the bodies is nullptr
        if (!bodyA || !bodyB) { return; }

        // Assuming all bodies in the game are PhysicsEntities
        PhysicsEntity* entityA = reinterpret_cast<PhysicsEntity*>(bodyA->GetUserData().pointer);
        PhysicsEntity* entityB = reinterpret_cast<PhysicsEntity*>(bodyB->GetUserData().pointer);

        // If this is blowing up, there is a b2body not correctly connected to a PhysicsEntity
        entityA->OnCollisionEnd(entityB);
        entityB->OnCollisionEnd(entityA);
    }
}