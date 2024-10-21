#include "opch.h"
#include "SceneCamera.h"

#include "glm/ext/matrix_clip_space.hpp"

Owl::SceneCamera::SceneCamera()
{
    RecalculateProjection();
}

void Owl::SceneCamera::SetOrthographic(const float pSize, const float pNearClip, const float pFarClip)
{
    m_ProjectionType = ProjectionType::Orthographic;
    m_OrthographicSize = pSize;
    m_OrthographicNear = pNearClip;
    m_OrthographicFar = pFarClip;
    RecalculateProjection();
}

void Owl::SceneCamera::SetPerspective(const float pVerticalFov, const float pNearClip, const float pFarClip)
{
    m_ProjectionType = ProjectionType::Perspective;
    m_PerspectiveVerticalFov = pVerticalFov;
    m_PerspectiveNear = pNearClip;
    m_PerspectiveFar = pFarClip;
    RecalculateProjection();
}

void Owl::SceneCamera::SetViewportSize(const uint32_t pWidth, const uint32_t pHeight)
{
    OWL_CORE_ASSERT(pWidth > 0 && pHeight > 0);
    m_AspectRatio = static_cast<float>(pWidth) / static_cast<float>(pHeight);
    RecalculateProjection();
}

void Owl::SceneCamera::RecalculateProjection()
{
    if (m_ProjectionType == ProjectionType::Perspective)
    {
        m_Projection = glm::perspective(m_PerspectiveVerticalFov, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
    }
    if (m_ProjectionType == ProjectionType::Orthographic)
    {
        const float orthoLeft = -m_OrthographicSize * 0.5f * m_AspectRatio;
        const float orthoRight = m_OrthographicSize * 0.5f * m_AspectRatio;
        const float orthoBottom = -m_OrthographicSize * 0.5f;
        const float orthoTop = m_OrthographicSize * 0.5f;

        m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
    }
}
