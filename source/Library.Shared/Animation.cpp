#include "pch.h"
#include "Animation.h"
#include "Game.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Animation)

	Animation::Animation() :
		Attributed(Animation::TypeIdClass())
	{
	}

	const Vector<Signature> Animation::Signatures()
	{
		return Vector<Signature>
		{
			{ "TimeBetween", Datum::DatumType::Float, 1, offsetof(Animation, m_TimeBetween) },
			{ "SpriteWidth", Datum::DatumType::Integer, 1, offsetof(Animation, m_SpriteWidth) },
			{ "StateName", Datum::DatumType::String, 1, offsetof(Animation, m_StateName) },
			{ "SpriteSheet", Datum::DatumType::String, 1, offsetof(Animation, m_SpriteSheet) },
		};
	}

	void Animation::Initialize()
	{
		textureInfo = RenderUtil::LoadTexture(m_SpriteSheet);
		m_NumSprites = (textureInfo.m_width) / m_SpriteWidth;

		for (size_t tempCurrentSprite = 0; tempCurrentSprite < m_NumSprites; ++tempCurrentSprite)
		{
			// Build UV rectangle
			float uLeft = (1.0f / m_NumSprites) * tempCurrentSprite;
			float vBottom = 0;
			float uRight = (1.0f / m_NumSprites) * (tempCurrentSprite + 1);
			float vTop = 1;

			TextureCoords entry(uLeft, vBottom, uRight, vTop);
			textureCoordinates.PushBack(entry);
		}
	}

	void Animation::Update()
	{
		float deltaTime = Game::GetGame()->GetWorldState().GetDeltaTime();

		m_TimeAccumulated += deltaTime;

		if (m_TimeAccumulated >= m_TimeBetween)
		{
			currentSprite = ((++currentSprite) % m_NumSprites);
			m_TimeAccumulated = 0;
		}
	}

	const TextureCoords& Animation::GetTextureCoords() const
	{
		return textureCoordinates[currentSprite];
	}
}