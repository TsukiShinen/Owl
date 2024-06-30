#pragma once
#include "Owl/Renderer/Camera.h"

namespace Owl
{
    class SceneCamera : public Camera
    {
    public:
        enum class ProjectionType { Perspective = 0, Orthographic = 1 };
    public:
        SceneCamera();
        virtual ~SceneCamera() = default;

        void SetOrthographic(float pSize, float pNearClip, float pFarClip);
        void SetPerspective(float pVerticalFov, float pNearClip, float pFarClip);
        
        void SetViewportSize(uint32_t pWidth, uint32_t pHeight);

        [[nodiscard]] float GetPerspectiveVerticalFov() const { return m_PerspectiveVerticalFov; }
        void SetPerspectiveVerticalFov(const float pVerticalFov) { m_PerspectiveVerticalFov = pVerticalFov; RecalculateProjection(); }
        [[nodiscard]] float GetPerspectiveNearClip() const { return m_PerspectiveNear; }
        void SetPerspectiveNearClip(const float pNear) { m_PerspectiveNear = pNear; RecalculateProjection(); }
        [[nodiscard]] float GetPerspectiveFarClip() const { return m_PerspectiveFar; }
        void SetPerspectiveFarClip(const float pFar) { m_PerspectiveFar = pFar; RecalculateProjection(); }

        [[nodiscard]] float GetOrthographicSize() const { return m_OrthographicSize; }
        void SetOrthographicSize(const float pSize) { m_OrthographicSize = pSize; RecalculateProjection(); }
        [[nodiscard]] float GetOrthographicNearClip() const { return m_OrthographicNear; }
        void SetOrthographicNearClip(const float pNear) { m_OrthographicNear = pNear; RecalculateProjection(); }
        [[nodiscard]] float GetOrthographicFarClip() const { return m_OrthographicFar; }
        void SetOrthographicFarClip(const float pFar) { m_OrthographicFar = pFar; RecalculateProjection(); }
        
        [[nodiscard]] ProjectionType GetProjectionType() const { return m_ProjectionType; }
        void SetProjectionType(const ProjectionType pType) { m_ProjectionType = pType; RecalculateProjection(); }
    private:
        void RecalculateProjection();
    private:
        ProjectionType m_ProjectionType = ProjectionType::Orthographic;
        
        float m_PerspectiveVerticalFov= glm::radians(45.0f);
        float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;
        
        float m_OrthographicSize = 10.f;
        float m_OrthographicNear = -1.0f, m_OrthographicFar = 1.0f;
        
        float m_AspectRatio = 0.0f;
    };
}
