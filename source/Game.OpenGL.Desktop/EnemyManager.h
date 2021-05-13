#pragma once

#include "Entity.h"
#include "Vector.h"
#include "TypeManager.h"
#include "Event.h"

using namespace FieaGameEngine;

class Enemy;

class EnemyManager final : public Entity, public EventSubscriber
{
	RTTI_DECLARATIONS(EnemyManager, Entity)

public:
	EnemyManager();
	~EnemyManager();

	virtual void Init(WorldState* state) override;

	virtual void Update(WorldState* worldState) override;

	static const Vector<Signature> Signatures();

	virtual void Notify(const EventPublisher& publisher) override;

private:
	Enemy* SpawnEnemy();
	float m_maxEnemySpawnTimer = 5;
	float m_enemySpawnTimer = m_maxEnemySpawnTimer;
	float m_enemySpawnTimerIncreaseThreshold = 30;
};

ConcreteFactory(EnemyManager, Scope)

