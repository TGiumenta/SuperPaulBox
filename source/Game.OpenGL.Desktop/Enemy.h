#pragma once
#include "PhysicsEntity.h"
#include "TypeManager.h"
#include "HashMap.h"
#include "Input.h"
#include "Event.h"
#include <box2d/box2d.h>

using namespace FieaGameEngine;

class Enemy final : public PhysicsEntity, public b2RayCastCallback
{
	RTTI_DECLARATIONS(Enemy, PhysicsEntity);

public:
	Enemy();
	~Enemy();

	void Init(WorldState* state) override;

	virtual void Update(WorldState* worldState) override;

	static const Vector<Signature> Signatures();

	struct CreateEnemyEvent {
		Enemy* crate;
	};


private:
	virtual void CreateFixture() override;

	bool CheckForWall();

	void Respawn();
	void WalkRight();
	void WalkLeft();

	virtual void OnCollisionStart(PhysicsEntity* other) override;
	virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;

	float m_walkSpeed = 5100.0f;
	float m_healthPoints;
	float m_maxHP;
	bool m_reachedGround = false;
	bool m_reachedRespawnZone = false;
	bool m_isAgainstWall = false;
};

ConcreteFactory(Enemy, Scope);