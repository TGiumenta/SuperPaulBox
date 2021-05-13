#pragma once
#include "Entity.h"
#include "Number.h"
#include "HashMap.h"
#include "Input.h"
#include "ReactionAttributed.h"

using namespace FieaGameEngine;

class ScoreManager final : public Entity, public EventSubscriber
{
	RTTI_DECLARATIONS(ScoreManager, Entity)

public:
	ScoreManager();
	~ScoreManager();

	void Update(WorldState* worldState);

	void Init(WorldState* worldState);

	virtual void Notify(const class EventPublisher&) override;

	static const Vector<Signature> Signatures();

	void UpdateScoreSprites();

	void AddScore();

	int32_t GetScore();

	void SetScore(int32_t score);

	void EnableNextVisibleNumber(int32_t visibleNumbers);

	//using InputFunction = void(ScoreManager::*)();
	//void BindInput(int key, int action, InputFunction func);

private:
	Vector<Number*> m_NumbersList;
	HashMap<const std::string, TextureInfo> m_NumberTextures;

	std::string NumberStringLiteral = "Number";
	// Number of digits possible
	size_t m_NumberAmount = 10;
	// Maximum score possible
	int32_t m_MaxScore = 999;
	// Amount to increment the score by
	int32_t m_Increment = 1;
	// The index of visible number in m_NumbersList
	int32_t m_VisibleNumberIndex = 0;
	// The datum used to hold the score as a float
	Datum* m_ScoreDatum = nullptr;
	// The reaction used to react to an Event<EventMessageAttributed>
	ReactionAttributed* m_ReactionIncrementScore = nullptr;

	// Used for debug key
	// HashMap<KeyboardEvent, InputFunction> m_InputActions;
};

ConcreteFactory(ScoreManager, Scope);