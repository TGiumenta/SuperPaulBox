#include "pch.h"
#include "PhysicsEntity.h"
#include "GameTime.h"
#include "WorldState.h"
#include "Game.h"
#include <math.h>
#include "CollisionDefines.h"

using namespace std::chrono;

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(PhysicsEntity);

    PhysicsEntity::PhysicsEntity()
        : Entity(PhysicsEntity::TypeIdClass())
    {
        InitStartingFixtures();
    }

    PhysicsEntity::PhysicsEntity(RTTI::IdType type, ECollisionChannel collisionChannel)
        : Entity(type), m_collisionChannel(collisionChannel)
    {
        InitStartingFixtures();
    }

    PhysicsEntity::~PhysicsEntity()
    {
        if (m_body)
        {
            Game::GetGame()->GetPhysicsWorld().DestroyBody(m_body);
        }
    }

    void PhysicsEntity::Init(WorldState* worldState)
    {
        Entity::Init(worldState);

        m_startingDef.type = m_stringToBodyType.at(m_bodyType);
        m_startingDef.position = b2Vec2(m_transform.m_position.x, m_transform.m_position.y);
        m_startingDef.fixedRotation = true;
        m_body = Game::GetGame()->GetPhysicsWorld().CreateBody(&m_startingDef);
        m_bodyInitialized = true;

        CreateFixture();
        SetAsTrigger(m_isTrigger);
        SetCollisionFilter();
    }

    void PhysicsEntity::ApplyForce(float xForce, float yForce)
    {
        m_body->ApplyForceToCenter(b2Vec2(xForce, yForce), true);
    }

    void PhysicsEntity::ApplyLinearImpulse(float xImpulse, float yImpulse)
    {
        m_body->ApplyLinearImpulseToCenter(b2Vec2(xImpulse, yImpulse), true);
    }

    void PhysicsEntity::SetPosition(b2Vec2 newPosition)
    {
        Entity::SetPosition(newPosition);

        if (m_bodyInitialized && m_body)
        {
            m_body->SetTransform(newPosition, m_body->GetAngle());
        }
    }

    void PhysicsEntity::SetAsTrigger(bool IsTrigger)
    {
        for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
        {
            f->SetSensor(IsTrigger);
        }
    }

    void PhysicsEntity::TurnOffCollisions()
    {
        for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
        {
            b2Filter filter = f->GetFilterData();

            //change whatever you need to, eg.
            filter.maskBits = 0;

            //and set it back
            f->SetFilterData(filter);
        }
    }

    void PhysicsEntity::CreateFixture()
    {
        b2PolygonShape shape;
        glm::vec4 scale = m_transform.m_scale;

        shape.SetAsBox(abs(scale.x) / 2, abs(scale.y) / 2);
        m_startingFixture.shape = &shape;
        m_body->CreateFixture(&m_startingFixture);
    }

    void PhysicsEntity::InitStartingFixtures()
    {
        // Default physics body
        m_startingDef.gravityScale = 1.0f;
        m_startingDef.linearDamping = 0.1f;
        m_startingDef.angularDamping = 0.1f;

        // Default physics fixture
        m_startingFixture.density = 1.0f;
        m_startingFixture.friction = 0.1f;

        // Store this pointer in the userdata
        m_startingDef.userData.pointer = reinterpret_cast<uintptr_t>(this);
    }

    void PhysicsEntity::SetCollisionFilter()
    {
        uint16 categoryBits = CollisionUtiltiy::GetCategoryBits(m_collisionChannel); 
        uint16 maskBits = CollisionUtiltiy::GetMaskBits(m_collisionChannel); 

        for (b2Fixture* f = m_body->GetFixtureList(); f; f = f->GetNext())
        {
            b2Filter filter = f->GetFilterData();

            //change whatever you need to, eg.
            filter.categoryBits = categoryBits;
            filter.maskBits = maskBits; //0b11101;

            //and set it back
            f->SetFilterData(filter);
        }
    }

    void PhysicsEntity::Update(WorldState* worldState)
    {
        assert(m_body != nullptr);
        b2Vec2 physicsPos = m_body->GetPosition();
        float rotation = m_body->GetAngle();
        m_transform.m_position = glm::vec4(physicsPos.x, physicsPos.y, 0.0f, 0.0f);
        m_transform.m_rotation = glm::vec4(0.0f, 0.0f, rotation, 0.0f);

        Entity::Update(worldState);
    }

    const Vector<Signature> PhysicsEntity::Signatures()
    {
        return Vector<Signature>
        {
            { "BodyType", Datum::DatumType::String, 1, offsetof(PhysicsEntity, m_bodyType) },
            { "GravityScale", Datum::DatumType::Float, 1, offsetof(PhysicsEntity, m_startingDef.gravityScale) },
            { "LinearDamping", Datum::DatumType::Float, 1, offsetof(PhysicsEntity, m_startingDef.linearDamping) },
            { "AngularDamping", Datum::DatumType::Float, 1, offsetof(PhysicsEntity, m_startingDef.angularDamping) },
            { "Density", Datum::DatumType::Float, 1, offsetof(PhysicsEntity, m_startingFixture.density) },
            { "Friction", Datum::DatumType::Float, 1, offsetof(PhysicsEntity, m_startingFixture.friction) },
            { "IsTrigger", Datum::DatumType::Integer, 1, offsetof(PhysicsEntity, m_isTrigger) }
        };
    }
}