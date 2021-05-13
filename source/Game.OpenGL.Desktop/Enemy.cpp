#include "pch.h"
#include "Enemy.h"
#include <GLFW/glfw3.h>
#include "VectorHelper.h"
#include "Projectile.h"
#include "Game.h"

RTTI_DEFINITIONS(Enemy);

Enemy::Enemy() : 
    PhysicsEntity(Enemy::TypeIdClass(), ECollisionChannel::ENEMIES)
{
}

Enemy::~Enemy()
{
}

void Enemy::Update(WorldState* worldState)
{
    PhysicsEntity::Update(worldState);

    if (CheckForWall())
    {
        if (GetFacingDirection() == FacingDirection::FACING_LEFT)
        {
            WalkRight();
        }
        else
        {
            WalkLeft();
        }
    }
    
    if (GetFacingDirection() == FacingDirection::FACING_LEFT && m_reachedGround && m_healthPoints > 0)
    {
        WalkLeft();
    }
    else if (GetFacingDirection() == FacingDirection::FACING_RIGHT && m_reachedGround && m_healthPoints > 0)
    {
        WalkRight();
    }

    if (m_reachedRespawnZone)
    {
        m_reachedRespawnZone = false;
        Respawn();
    }

    if (m_healthPoints <= 0 && GetPosition().y < -100)
    {
        Game::GetGame()->GetWorldState().DestroyEntity(this);
    }
}

void Enemy::Init(WorldState* state)
{
    PhysicsEntity::Init(state);

    SetPosition(b2Vec2(0, 39));
    m_healthPoints = m_maxHP;

    // decide initial direction
    if (rand() % 2 == 0)
    {
        SetFacingDirection(FacingDirection::FACING_LEFT);
    }
    else
    {
        SetFacingDirection(FacingDirection::FACING_RIGHT);
    }
}


void Enemy::WalkRight()
{
    float deltaTime = Game::GetGame()->GetWorldState().GetDeltaTime();
    ApplyForce(m_walkSpeed * deltaTime * 400, 0.0f);
    SetFacingDirection(FacingDirection::FACING_RIGHT);
}

void Enemy::WalkLeft()
{
    float deltaTime = Game::GetGame()->GetWorldState().GetDeltaTime();
    ApplyForce(-m_walkSpeed * deltaTime * 400, 0.0f);
    SetFacingDirection(FacingDirection::FACING_LEFT);
}

void Enemy::OnCollisionStart(PhysicsEntity* other)
{
   if (other->Is("Projectile"))
   {
       auto proj = other->As<Projectile>();
       m_healthPoints -= proj->CurrentDamage();
       ApplyForce(proj->GetLinearVelocity().x * 10000, 500000);
       Game::GetGame()->GetWorldState().DestroyEntity(other);
       if (m_healthPoints <= 0)
       {
           SetAllowRotation(true);
           SetAngularVelocity(float(rand() % 180 + 180));
           TurnOffCollisions();
       }
   }

   

   if (other->GetTag() == "LevelGeometry")
   {
       m_reachedGround = true;
   }

   if (other->GetTag() == "RespawnZone")
   {
       m_reachedRespawnZone = true;
   }
}


void Enemy::CreateFixture()
{
    b2CircleShape shape;
    glm::vec4 scale = m_transform.m_scale;

    shape.m_radius = scale.x / 2;
    m_startingFixture.shape = &shape;
    m_body->CreateFixture(&m_startingFixture);
}

bool Enemy::CheckForWall()
{
    b2Vec2 raycastStart(GetPosition().x, GetPosition().y);
    float raycastLength = (abs(GetScale().x) / 2) + 0.2f;
    raycastLength *= (GetFacingDirection() == FacingDirection::FACING_RIGHT) ? 1 : -1;
    b2Vec2 raycastEnd = raycastStart + b2Vec2(raycastLength, 0);

    m_isAgainstWall = false;
    Game::GetGame()->GetPhysicsWorld().RayCast(this, raycastStart, raycastEnd);

    return m_isAgainstWall;
}

void Enemy::Respawn()
{
    m_healthPoints = m_maxHP;
    SetLinearVelocity(b2Vec2(0, 0));
    SetPosition(b2Vec2(0, 39));
    m_walkSpeed += 200;
    m_reachedGround = false;
    SetTextureFile("EnemyFast.png");

    if (rand() % 2 == 0)
    {
        SetFacingDirection(FacingDirection::FACING_LEFT);
    }
    else
    {
        SetFacingDirection(FacingDirection::FACING_RIGHT);
    }
}

float Enemy::ReportFixture(b2Fixture* fixture, const b2Vec2&, const b2Vec2&, float)
{
    b2Body* body = fixture->GetBody();
    PhysicsEntity* entity = reinterpret_cast<PhysicsEntity*>(body->GetUserData().pointer);

    if (entity->GetTag() == "LevelWall")
    {
        m_isAgainstWall = true;
        return 0;
    }

    return 1;
}

const Vector<Signature> Enemy::Signatures()
{
    return Vector<Signature>
    {
        { "WalkSpeed", Datum::DatumType::Float, 1, offsetof(Enemy, m_walkSpeed) },
        { "MaxHealth", Datum::DatumType::Float, 1, offsetof(Enemy, m_maxHP) }
    };
}