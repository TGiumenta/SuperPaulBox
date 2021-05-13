#pragma once

#include "Camera.h"

namespace FieaGameEngine
{
    class GameTime;

    class OrthographicCamera : public Camera
    {
        RTTI_DECLARATIONS(OrthographicCamera, Camera)

    public:
        OrthographicCamera(float viewWidth = DefaultViewWidth, float viewHeight = DefaultViewHeight, float nearPlaneDistance = DefaultNearPlaneDistance, float farPlaneDistance = DefaultFarPlaneDistance);
        OrthographicCamera(const OrthographicCamera&) = delete;
        OrthographicCamera(OrthographicCamera&&) = delete;
        OrthographicCamera& operator=(const OrthographicCamera&) = delete;
        OrthographicCamera& operator=(OrthographicCamera&&) = default;
        virtual ~OrthographicCamera() = default;

        float ViewWidth() const;
        void SetViewWidth(float viewWidth);

        float ViewHeight() const;
        void SetViewHeight(float viewHeight);

        virtual void UpdateProjectionMatrix() override;

        static const float DefaultViewWidth;
        static const float DefaultViewHeight;

    protected:
        float mViewWidth;
        float mViewHeight;
    };
}
