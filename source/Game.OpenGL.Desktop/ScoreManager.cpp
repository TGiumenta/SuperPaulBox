#include "pch.h"
#include "ScoreManager.h"
#include <GLFW/glfw3.h>
#include "Game.h"
#include "EventMessageAttributed.h"
#include "ActionIncrement.h"


RTTI_DEFINITIONS(ScoreManager)

using namespace FieaGameEngine;

ScoreManager::ScoreManager() :
	Entity(ScoreManager::TypeIdClass())
{
	//Event<KeyboardEvent>::Subscribe(*this);
	Event<EventMessageAttributed>::Subscribe(*this);
	//BindInput(GLFW_KEY_M, KEY_REPEAT, &ScoreManager::AddScore);
}

ScoreManager::~ScoreManager()
{
	Event<KeyboardEvent>::Unsubscribe(*this);
	Event<EventMessageAttributed>::Unsubscribe(*this);
}

void ScoreManager::Init(WorldState* worldState)
{
	// Find the prescribed attribute and store it off.
	m_ScoreDatum = Find("Score");
	assert(m_ScoreDatum != nullptr);

	// Create a new reaction and attach an action increment to it
	m_ReactionIncrementScore = new ReactionAttributed("IncrementScoreReaction");
	auto action = m_ReactionIncrementScore->CreateAction("ActionIncrement", "ScoreActionIncrement");
	assert(action->Is(ActionIncrement::TypeIdClass()));
	ActionIncrement& actionIncrement = static_cast<ActionIncrement&>(*action);
	actionIncrement.SetTarget("Score");
	actionIncrement.SetStep(1.f);
	// Adopt the reaction into this scope so it can find the score datum
	Adopt(*m_ReactionIncrementScore, "IncrementScoreReaction");

	Entity::Init(worldState);

	// Find the number entities and store them off in a vector
	// Set visibilities to false and turn on the first number
	Datum* numbers = Find("Children");
	if (numbers != nullptr)
	{
		size_t size = numbers->Size();
		for (size_t index = 0; index < size; ++index)
		{
			Scope& scope = numbers->Get<Scope>(index);
			Number* number = static_cast<Number*>(&scope);
			number->SetVisibility(false);
			m_NumbersList.PushBack(number);
		}
		EnableNextVisibleNumber(m_VisibleNumberIndex);
	}

	// Create and store off all the texture infos to read from later
	for (size_t i = 0; i < m_NumberAmount; ++i)
	{
		const std::string numberName = NumberStringLiteral + std::to_string(i);
		TextureInfo textureInfo = RenderUtil::LoadTexture(numberName + ".png");
		m_NumberTextures.Insert(std::make_pair(numberName, RenderUtil::LoadTexture(numberName + ".png")));
	}
}

void ScoreManager::Update(WorldState* worldState)
{
	if (GetScore() <= m_MaxScore)
	{
		UpdateScoreSprites();
	}

	Entity::Update(worldState);
}

//void ScoreManager::BindInput(int key, int action, ScoreManager::InputFunction func)
//{
//	KeyboardEvent event{ key, action };
//	m_InputActions[event] = func;
//}

void ScoreManager::Notify(const EventPublisher& publisher)
{
	UNREFERENCED_PARAMETER(publisher);
	//const Event<KeyboardEvent>* keyboardEvent = publisher.As<Event<KeyboardEvent>>();
	//if (keyboardEvent)
	//{
	//	auto it = m_InputActions.Find(keyboardEvent->Message());
	//	if (it != m_InputActions.end())
	//	{
	//		ScoreManager::InputFunction func = it->second;
	//		(this->*func)();
	//	}
	//}
}

const Vector<Signature> ScoreManager::Signatures()
{
	return Vector<Signature>
	{

	};
}

void ScoreManager::UpdateScoreSprites()
{
	int32_t scoreToDivide = GetScore(), numberValue = 0, numberIndex = 0;

	// Get each digit individually and set each number sprite
	while (scoreToDivide > 0)
	{
		numberValue = scoreToDivide % 10;
		scoreToDivide /= 10;
		m_NumbersList[numberIndex++]->SetTextureInfo(m_NumberTextures.At(NumberStringLiteral + std::to_string(numberValue)));
	}

	if (numberIndex > (m_VisibleNumberIndex))
	{
		EnableNextVisibleNumber(m_VisibleNumberIndex++);
	}
}

void ScoreManager::AddScore()
{
	if (GetScore() <= m_MaxScore)
	{
		SetScore(GetScore() + m_Increment);
	}
}

int32_t ScoreManager::GetScore()
{
	return static_cast<int32_t>(m_ScoreDatum->Get<float>());
}

void ScoreManager::SetScore(int32_t score)
{
	m_ScoreDatum->Set(static_cast<float>(score));
}

void ScoreManager::EnableNextVisibleNumber(int32_t visibleNumberIndex)
{
	m_NumbersList[visibleNumberIndex]->SetVisibility(true);
}