#include "pch.h"
#include "Player.h"
#include <GLFW/glfw3.h>
#include "Weapon.h"
#include "VectorHelper.h"
#include "Game.h"
#include "CrateManager.h"
#include "CustomGame.h"
#include "AudioManager.h"

RTTI_DEFINITIONS(Player);

Player::Player()
    : PhysicsEntity(Player::TypeIdClass(), ECollisionChannel::PLAYER)
{
    Event<KeyboardEvent>::Subscribe(*this);
    Event<SwapWeaponEvent>::Subscribe(*this);
    Event<CameraShake>::Subscribe(*this);

    BindInput(GLFW_KEY_D, KEY_REPEAT, &Player::WalkRight);
    BindInput(GLFW_KEY_A, KEY_REPEAT, &Player::WalkLeft);
    BindInput(GLFW_KEY_SPACE, KEY_PRESS, &Player::Jump);
    BindInput(GLFW_MOUSE_BUTTON_LEFT, KEY_REPEAT, &Player::ShootWeapon);
}

Player::~Player()
{
    Event<KeyboardEvent>::Unsubscribe(*this);
    Event<SwapWeaponEvent>::Unsubscribe(*this);
    Event<CameraShake>::Unsubscribe(*this);
}

void Player::CreateFixture()
{
    // Create the square fixture near top of player
    b2PolygonShape shape;
    glm::vec4 scale = m_transform.m_scale;
    float xScale = scale.x;
    float yScale = scale.y;

    b2Vec2 bottomLeftCorner(-xScale / 2, -yScale / 2);
    b2Vec2 bottomRightCorner(xScale / 2, -yScale / 2);
    b2Vec2 TopLeftCorner(-xScale / 2, yScale / 2);
    b2Vec2 TopRightCorner(xScale / 2, yScale / 2);

    shape.SetAsBox(abs(scale.x) / 2, abs(scale.y) / 2);
    m_startingFixture.shape = &shape;
    m_body->CreateFixture(&m_startingFixture);

    // Create the circle fixture at player feet
    m_startingFixture.density = 0.1f;

    b2CircleShape circleShape;
    circleShape.m_radius = abs(scale.x) / 50;
    circleShape.m_p = bottomLeftCorner;
    m_startingFixture.shape = &circleShape;
    m_body->CreateFixture(&m_startingFixture);

    circleShape.m_p = bottomRightCorner;
    m_body->CreateFixture(&m_startingFixture);
}

void Player::Update(WorldState* worldState)
{
    PhysicsEntity::Update(worldState);

    UpdateIsOnGround();
    UpdateLinearDamping();
    UpdateAnimationStates();
}

void Player::Render(WorldState* worldState)
{
    PhysicsEntity::Render(worldState);
}

void Player::Init(WorldState* state)
{
    // Find all the weapons from JSON
    Datum* weapons = Find("Weapons");
    if (weapons != nullptr)
    {
        size_t size = weapons->Size();
        for (size_t index = 0; index < size; ++index)
        {
            // Add weapons to the list and initialize them
            Scope& scope = weapons->Get<Scope>(index);
            Weapon* weapon = static_cast<Weapon*>(&scope);
            m_WeaponList.PushBack(weapon);
            weapon->Init(state);
        }
    }

    // Set the current weapon to the Pistol, it's always index 0
    SetCurrentWeaponByIndex(0);
    Adopt(*m_CurrentWeapon, "Children");

    PhysicsEntity::Init(state);
}

void Player::BindInput(int key, int action, Player::InputFunction func)
{
    KeyboardEvent event{ key, action };
    m_InputActions[event] = func;
}

void Player::Notify(const EventPublisher& publisher)
{
    const Event<KeyboardEvent>* keyboardEvent = publisher.As<Event<KeyboardEvent>>();
#ifdef _DEBUG
    if (keyboardEvent)
    {
        auto it = m_InputActions.Find(keyboardEvent->Message());
        if (it != m_InputActions.end())
        {
            Player::InputFunction func = it->second;
            (this->*func)();
        }
    }
    else
    {
        const Event<SwapWeaponEvent>* swapWeaponEvent = publisher.As<Event<SwapWeaponEvent>>();
        if (swapWeaponEvent)
        {
            // Remove the current weapon, and grab the new random weapon to equip it
            m_CurrentWeapon->Orphan();
            size_t randomIndex = rand() % m_WeaponList.Size();
            SetCurrentWeaponByIndex(randomIndex);
            Adopt(*m_CurrentWeapon, "Children");
        }

        const Event<CameraShake>* cameraShakeEvent = publisher.As<Event<CameraShake>>();
        if (cameraShakeEvent)
        {
            RenderUtil::GetCamera().SetPosition(cameraShakeEvent->Message().position);
        }
    }
#endif // _DEBUG

#ifndef _DEBUG
    bool isKeyboardEvent = keyboardEvent->Message().Is(KeyboardEvent::TypeIdClass());
    if (isKeyboardEvent)
    {
        auto it = m_InputActions.Find(keyboardEvent->Message());
        if (it != m_InputActions.end())
        {
            Player::InputFunction func = it->second;
            (this->*func)();
        }
    }
    else
    {
        const Event<SwapWeaponEvent>* swapWeaponEvent = publisher.As<Event<SwapWeaponEvent>>();
        bool isWeaponSwapEvent = swapWeaponEvent->Message().Is(SwapWeaponEvent::TypeIdClass());
        if (isWeaponSwapEvent)
        {
            size_t randomIndex = rand() % m_WeaponList.Size();
            m_CurrentWeapon->Orphan();
            SetCurrentWeaponByIndex(randomIndex);
            Adopt(*m_CurrentWeapon, "Children");
        }
    }
#endif // _RELEASE

}

void Player::WalkRight()
{
    float deltaTime = Game::GetGame()->GetWorldState().GetDeltaTime();

    if (IsOnGround())
    {
        ApplyForce(m_walkSpeed * deltaTime * 400, 0.0f);
    }
    else
    {
        ApplyForce(m_airControlForce * deltaTime * 400, 0.0f);
    }

    SetFacingDirection(FacingDirection::FACING_RIGHT);
}

void Player::WalkLeft()
{
    float deltaTime = Game::GetGame()->GetWorldState().GetDeltaTime();
    if (IsOnGround())
    {
        ApplyForce(-m_walkSpeed * deltaTime * 400, 0.0f);
    }
    else
    {
        ApplyForce(-m_airControlForce * deltaTime * 400, 0.0f);
    }

    SetFacingDirection(FacingDirection::FACING_LEFT);
}

void Player::Jump()
{
    if (IsOnGround())
    {
        ApplyLinearImpulse(0.0f, m_jumpForce);
        m_CurrentAnimation = m_Animations["Jumping"];
    }
}

void Player::ShootWeapon()
{
    m_CurrentWeapon->SpawnProjectile();
}

void Player::OnCollisionStart(PhysicsEntity* other)
{
    if (other->GetTag() == "Enemy")
    {
        KillPlayer();
    }
}

void Player::UpdateAnimationStates()
{
    if (IsOnGround())
    {
        if (GetCurrentAnimationState() == "Jumping")
        {
            m_CurrentAnimation = m_Animations["Walking"];
        }

        else if (GetCurrentAnimationState() == "Walking" && GetLinearVelocity().Length() < 11.0f)
        {
            m_CurrentAnimation = m_Animations["Idling"];
        }

        else if (GetCurrentAnimationState() == "Idling" && GetLinearVelocity().Length() > 11.0f)
        {
            m_CurrentAnimation = m_Animations["Walking"];
        }
    }
    else
    {
        if (GetCurrentAnimationState() != "Dying")
        {
            m_CurrentAnimation = m_Animations["Jumping"];
        }
    }

}

bool Player::UpdateIsOnGround()
{
    b2Vec2 raycastStart(GetPosition().x, GetPosition().y);
    float raycastLength = (GetScale().y / 2) + 0.2f;
    b2Vec2 raycastEnd = raycastStart + b2Vec2(0, -1 * raycastLength);

    m_IsOnGround = false;
    Game::GetGame()->GetPhysicsWorld().RayCast(this, raycastStart, raycastEnd);

    return m_IsOnGround;
}

void Player::KillPlayer()
{
    Event<KeyboardEvent>::Unsubscribe(*this);
    Event<SwapWeaponEvent>::Unsubscribe(*this);
    m_CurrentAnimation = m_Animations["Dying"];
    m_CurrentWeapon->Orphan();
    CustomGame::GetCustomGame()->ChangeState(GameState::GAME_OVER);
}

void Player::UpdateLinearDamping()
{
    float linearDampingToUse = (m_IsOnGround) ? m_linearDampingGround : m_linearDampingAir;
    SetLinearDamping(linearDampingToUse);
}

float Player::ReportFixture(b2Fixture* fixture, const b2Vec2&, const b2Vec2&, float)
{
    b2Body* body = fixture->GetBody();
    PhysicsEntity* entity = reinterpret_cast<PhysicsEntity*>(body->GetUserData().pointer);

    if (entity->GetTag() == "LevelGeometry" || entity->GetTag() == "LevelWall" || entity->GetTag() == "RespawnZone")
    {
        m_IsOnGround = true;
        return 0;
    }

    return 1;
}

const Vector<Signature> Player::Signatures()
{
    return Vector<Signature>
    {
        { "WalkSpeed", Datum::DatumType::Float, 1, offsetof(Player, m_walkSpeed) },
        { "JumpForce", Datum::DatumType::Float, 1, offsetof(Player, m_jumpForce) },
        { "Weapons", Datum::DatumType::Table, 0, 0 },
        { "AirControl", Datum::DatumType::Float, 1, offsetof(Player, m_airControlForce) },
        { "AirDamping", Datum::DatumType::Float, 1, offsetof(Player, m_linearDampingAir) },
        { "GroundDamping", Datum::DatumType::Float, 1, offsetof(Player, m_linearDampingGround) }
    };
}