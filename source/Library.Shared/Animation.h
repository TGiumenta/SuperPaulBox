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
		/// <summary>
		/// Constructor for object initialization
		/// </summary>
		Animation();

		/// <summary>
		/// Defaulted copy constructor 
		/// </summary>
		/// <param name="other"> The animation to copy </param>
		Animation(const Animation& other) = default;

		/// <summary>
		/// Defaulted move constructor
		/// </summary>
		/// <param name="other"> The animation to move </param>
		Animation(Animation&& other) noexcept = default;

		/// <summary>
		/// Defaulted copy assignment operator
		/// </summary>
		/// <param name="other"> The animation to copy to "this" animation </param>
		/// <returns> The LHS of the = operator </returns>
		Animation& operator=(const Animation& other) = default;

		/// <summary>
		/// Defaulted move assignment operator
		/// </summary>
		/// <param name="other"> The crate to move to "this" crate </param>
		/// <returns> The LHS of the = operator </returns>
		Animation& operator=(Animation&& other) noexcept = default;

		/// <summary>
		/// Defaulted destructor
		/// </summary>
		~Animation() = default;

		/// <summary>
		/// Queries the prescribed attributes of the animation
		/// </summary>
		/// <returns> The prescribed attributes of the animation </returns>
		const static Vector<Signature> Signatures();

		/// <summary>
		/// Creates the texture coordinates for the animation
		/// </summary>
		void Initialize();
		
		/// <summary>
		/// Updates the animation by determining if enough of the elapsed time has passed
		/// </summary>
		void Update();

		/// <summary>
		/// Sets the information on the texture which contains the width/height
		/// </summary>
		/// <param name="info"> The info to set </param>
		void SetTextureInfo(const TextureInfo& info) { textureInfo = info; }

		/// <summary>
		/// Queries the current texture information on the animation
		/// </summary>
		/// <returns> The current texture information </returns>
		const TextureInfo& GetTextureInfo() const { return textureInfo; }

		/// <summary>
		/// Queries the state name on this animation
		/// </summary>
		/// <returns> The state name on the animation </returns>
		const std::string& GetStateName() { return m_StateName; }

		/// <summary>
		/// Retrieves the current texture coordinates on the animations
		/// </summary>
		/// <returns> The current texture coordinates </returns>
		const TextureCoords& GetTextureCoords() const { return textureCoordinates[currentSprite]; }

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
