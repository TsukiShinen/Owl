#pragma once
#include "Owl/Renderer/Camera.h"

namespace Owl
{
    class SceneCamera : public Camera
    {
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float pSize, float pNearClip, float pFarClip);
        void SetViewportSize(uint32_t pWidth, uint32_t pHeight);

        [[nodiscard]] float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(const float pOrthographicSize) { m_OrthographicSize = pOrthographicSize; RecalculateProjection(); }
    private:
        void RecalculateProjection();
    private:
        float m_OrthographicSize = 10.f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
        
        float m_AspectRatio = 0.0f;
    };
}
