#pragma once
#include "PhysicsEntity.h"
#include "TypeManager.h"
#include "HashMap.h"
#include "Input.h"
#include "Event.h"
#include <box2d/box2d.h>

using namespace FieaGameEngine;

class Weapon;

class Player final: public PhysicsEntity, public EventSubscriber, public b2RayCastCallback
{
	RTTI_DECLARATIONS(Player, PhysicsEntity);

public:

	Player();
	~Player();

	void Init(WorldState* state) override;

	virtual void Update(WorldState* worldState) override;

	virtual void Render(WorldState* worldState) override;

	virtual void Notify(const class EventPublisher&) override;

	static const Vector<Signature> Signatures();

	const Vector<Weapon*>& Weapons() { return m_WeaponList; }

	Weapon* CurrentWeapon() { return m_CurrentWeapon; }

	void SetCurrentWeaponByIndex(size_t index) { m_CurrentWeapon = m_WeaponList[index]; }

	bool IsOnGround() const { return m_IsOnGround; }

	void KillPlayer();

	struct CameraShake {
		glm::vec3 position;
	};

private: 

	virtual void CreateFixture() override;

	bool UpdateIsOnGround();
	void UpdateLinearDamping();

	void WalkRight();
	void WalkLeft();
	void Jump();
	void ShootWeapon();

	virtual void OnCollisionStart(PhysicsEntity* other) override;

	virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	void UpdateAnimationStates();

    using InputFunction = void(Player::*)();
    void BindInput(int key, int action, InputFunction func);
	
	HashMap<KeyboardEvent, InputFunction> m_InputActions;

	float m_linearDampingGround = 1.8f;
	float m_linearDampingAir = 0.8f;
	float m_walkSpeed = 5100.0f;
	float m_airControlForce = 3100.0f;
	float m_jumpForce = 10000.0f;

	// Don't use this directly, it only gets updated by the IsOnGround function!
	bool m_IsOnGround = false;

	const std::string SpawnSoundString = "ThatsMyFace.wav";

	Weapon* m_CurrentWeapon = nullptr;
	Vector<Weapon*> m_WeaponList;
	size_t audioFileIndex = 0;
	size_t numAudioFiles = 7;
	std::string audioFileStrings[7] = {"BRR.wav", "ContrivedExample.wav", "DUDE.wav", "LiterallyPaulDied.wav", "PapaSmurfLostHisShit.wav", "ShutUp.wav", "ThisIsRecording.wav"};
};

ConcreteFactory(Player, Scope);