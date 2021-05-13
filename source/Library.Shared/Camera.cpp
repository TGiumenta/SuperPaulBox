#include "pch.h"
#include "Camera.h"
#include "VectorHelper.h"

using namespace glm;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Camera)

	Camera::Camera() :
		mNearPlaneDistance(DefaultNearPlaneDistance), mFarPlaneDistance(DefaultFarPlaneDistance)
	{
	}

	Camera::Camera(float nearPlaneDistance, float farPlaneDistance) :
		mNearPlaneDistance(nearPlaneDistance), mFarPlaneDistance(farPlaneDistance)
	{
	}

	const glm::vec3& Camera::Position() const
	{
		return mPosition;
	}

	const glm::vec3& Camera::Direction() const
	{
		return mDirection;
	}

	const glm::vec3& Camera::Up() const
	{
		return mUp;
	}

	const glm::vec3& Camera::Right() const
	{
		return mRight;
	}

	float Camera::NearPlaneDistance() const
	{
		return mNearPlaneDistance;
	}

	float Camera::FarPlaneDistance() const
	{
		return mFarPlaneDistance;
	}

	const glm::mat4& Camera::ViewMatrix() const
	{
		return mViewMatrix;
	}

	const glm::mat4&Camera::ProjectionMatrix() const
	{
		return mProjectionMatrix;
	}

	glm::mat4 Camera::ViewProjectionMatrix() const
	{
		return mProjectionMatrix * mViewMatrix;
	}

	void Camera::SetPosition(float x, float y, float z)
	{
		mPosition = vec3(x, y, z);
		mViewMatrixDataDirty = true;
	}

	void Camera::SetPosition(const glm::vec3& position)
	{
		mPosition = position;
		mViewMatrixDataDirty = true;
	}

	void Camera::Reset()
	{
		mPosition = Vector3Helper::Zero;
		mDirection = Vector3Helper::Forward;
		mUp = Vector3Helper::Up;
		mRight = Vector3Helper::Right;
		mViewMatrixDataDirty = true;

		UpdateViewMatrix();
	}

	void Camera::Initialize()
	{
		UpdateProjectionMatrix();
		Reset();
	}

	void Camera::Update(const GameTime& /*gameTime*/)
	{
		if (mViewMatrixDataDirty)
		{
			UpdateViewMatrix();
		}
	}

	void Camera::UpdateViewMatrix()
	{
		vec3 target = mPosition + mDirection;
		mViewMatrix = lookAt(mPosition, target, mUp);
		mViewMatrixDataDirty = false;
	}

	void Camera::ApplyRotation(const glm::mat4& transform)
	{
		vec4 direction = transform * vec4(mDirection, 0.0f);
		mDirection = (vec3)normalize(direction);

		vec4 up	= transform * vec4(mUp, 0.0f);
		mUp = (vec3)normalize(up);

		mRight = cross(mDirection, mUp);
		mUp = cross(mRight, mDirection);
		mViewMatrixDataDirty = true;
	}
}