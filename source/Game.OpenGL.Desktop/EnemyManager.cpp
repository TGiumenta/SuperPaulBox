#include "pch.h"
#include "EnemyManager.h"
#include "pch.h"
#include "Enemy.h"
#include "Game.h"

RTTI_DEFINITIONS(EnemyManager)

EnemyManager::EnemyManager() :
	Entity(EnemyManager::TypeIdClass())
{
	Event<Enemy::CreateEnemyEvent>::Subscribe(*this);
}

EnemyManager::~EnemyManager()
{
	Event<Enemy::CreateEnemyEvent>::Unsubscribe(*this);
}

void EnemyManager::Init(WorldState*)
{
	SpawnEnemy();
}

void EnemyManager::Update(WorldState*)
{
	m_enemySpawnTimer -= Game::GetGame()->GetWorldState().GetDeltaTime();

	Game::GetGame()->GetWorldState().GetGameTime().ElapsedGameTimeSeconds();

	if (Game::GetGame()->m_totalGameTime >= m_enemySpawnTimerIncreaseThreshold && m_maxEnemySpawnTimer > 2.0f)
	{
		m_enemySpawnTimerIncreaseThreshold += 30;
		m_maxEnemySpawnTimer -= 0.25;
	}

	if (m_enemySpawnTimer <= 0)
	{
		SpawnEnemy();
		m_enemySpawnTimer = m_maxEnemySpawnTimer;
	}
}

const Vector<Signature> EnemyManager::Signatures()
{
	return Vector<Signature>
	{
	};
}

void EnemyManager::Notify(const EventPublisher& publisher)
{
	const Event<Enemy::CreateEnemyEvent>* event = publisher.As<Event<Enemy::CreateEnemyEvent>>();
	if (event)
	{
		SpawnEnemy();
	}
}

Enemy* EnemyManager::SpawnEnemy()
{
	Entity* entity = Game::GetGame()->GetWorldState().Instantiate("Enemy", "Enemy.json");
	entity->SetPosition(b2Vec2(0, 39));
	return entity->As<Enemy>();
}