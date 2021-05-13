#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include "GameComponent.h"

namespace FieaGameEngine
{
	class Game;
	class GameTime;

	class Camera : public GameComponent
	{
		RTTI_DECLARATIONS(Camera, GameComponent)

	public:
		Camera();
		Camera(float nearPlaneDistance, float farPlaneDistance);
		Camera(const Camera&) = default;
		Camera& operator=(const Camera&) = default;
		Camera(Camera&&) = default;
		Camera& operator=(Camera&&) = default;
		virtual ~Camera() = default;

		const glm::vec3& Position() const;
		const glm::vec3& Direction() const;
		const glm::vec3& Up() const;
		const glm::vec3& Right() const;

		float NearPlaneDistance() const;
		float FarPlaneDistance() const;

		const glm::mat4& ViewMatrix() const;
		const glm::mat4& ProjectionMatrix() const;
		glm::mat4 ViewProjectionMatrix() const;

		virtual void SetPosition(float x, float y, float z);
		virtual void SetPosition(const glm::vec3& position);

		virtual void Reset();
		virtual void Initialize() override;
		virtual void Update(const GameTime& gameTime) override;
		virtual void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;
		virtual void ApplyRotation(const glm::mat4& transform);

		inline static const float DefaultNearPlaneDistance{ 0.01f };
		inline static const float DefaultFarPlaneDistance{ 1000.0f };

	protected:		
		float mNearPlaneDistance;
		float mFarPlaneDistance;

		glm::vec3 mPosition{ 0 };
		glm::vec3 mDirection{ 0 };
		glm::vec3 mUp{ 0 };
		glm::vec3 mRight{ 0 };

		glm::mat4 mViewMatrix{ 1 };
		glm::mat4 mProjectionMatrix{ 1 };

		bool mViewMatrixDataDirty{ true };
		bool mProjectionMatrixDataDirty{ true };
	};
}

