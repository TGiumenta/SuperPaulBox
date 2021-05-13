#include "pch.h"
#include "OrthographicCamera.h"
#include "VectorHelper.h"

namespace FieaGameEngine
{
    RTTI_DEFINITIONS(OrthographicCamera)

    const float OrthographicCamera::DefaultViewWidth = 100.0f;
    const float OrthographicCamera::DefaultViewHeight = 100.0f;

    OrthographicCamera::OrthographicCamera(float viewWidth, float viewHeight, float nearPlaneDistance, float farPlaneDistance) :
        Camera(nearPlaneDistance, farPlaneDistance),
        mViewWidth(viewWidth), mViewHeight(viewHeight)
    {
    }

    float OrthographicCamera::ViewWidth() const
    {
        return mViewWidth;
    }

    void OrthographicCamera::SetViewWidth(float viewWidth)
    {
        if (viewWidth > 0.0f)
        {
            mViewWidth = viewWidth;
        }
    }

    float OrthographicCamera::ViewHeight() const
    {
        return mViewHeight;
    }

    void OrthographicCamera::SetViewHeight(float viewHeight)
    {
        if (viewHeight > 0.0f)
        {
            mViewHeight = viewHeight;
        }
    }

    void OrthographicCamera::UpdateProjectionMatrix()
    {
        /*XMMATRIX projectionMatrix = XMMatrixOrthographicRH(mViewWidth, mViewHeight, mNearPlaneDistance, mFarPlaneDistance);
        XMStoreFloat4x4(&mProjectionMatrix, projectionMatrix);*/

        // glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
        mProjectionMatrix = glm::ortho(-80.0f, 80.0f, -45.0f, 45.0f, 0.1f, 100.0f);
        mProjectionMatrix *= glm::lookAt(mPosition, glm::vec3(0, 0, 0), Vector3Helper::Up);
    }
}
