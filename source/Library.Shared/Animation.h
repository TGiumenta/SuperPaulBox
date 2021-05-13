#pragma once

#include "Attributed.h"
#include "Factory.h"
#include "Vector.h"
#include "TypeManager.h"
#include "RenderUtil.h"

namespace FieaGameEngine
{
	struct TextureCoords
	{
		float m_ULeft = 0;
		float m_VBottom = 0;
		float m_URight = 0;
		float m_VTop = 1;

		TextureCoords() = default;

		TextureCoords(float uLeft, float vBottom, float uRight, float vTop) :
			m_ULeft(uLeft), m_VBottom(vBottom), m_URight(uRight), m_VTop(vTop) {};
	};

	class Animation final : public Attributed
	{
		RTTI_DECLARATIONS(Animation, Attributed)

	public:
		Animation();

		const static Vector<Signature> Signatures();

		void Initialize();
		void Update();

		inline void SetTextureInfo(const TextureInfo& info) { textureInfo = info; }
		const TextureInfo& GetTextureInfo() const { return textureInfo; }

		inline const std::string& GetStateName() { return m_StateName; }
		const TextureCoords& GetTextureCoords() const;

	private:
		size_t m_NumSprites;
		float m_TimeAccumulated = 0.f;
		float m_TimeBetween;
		int m_SpriteWidth;
		std::string m_StateName;
		std::string m_SpriteSheet;

		TextureInfo textureInfo;
		size_t currentSprite = 0;

		Vector<TextureCoords> textureCoordinates;
	};

	ConcreteFactory(Animation, Scope)
}
