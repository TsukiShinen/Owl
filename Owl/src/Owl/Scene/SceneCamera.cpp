#include "opch.h"
#include "SceneCamera.h"

#include "glm/ext/matrix_clip_space.hpp"

Owl::SceneCamera::SceneCamera()
{
    RecalculateProjection();
}

void Owl::SceneCamera::SetOrthographic(const float pSize, const float pNearClip, const float pFarClip)
{
    m_OrthographicSize = pSize;
    m_OrthographicNear = pNearClip;
    m_OrthographicFar = pFarClip;
    RecalculateProjection();
}

void Owl::SceneCamera::SetViewportSize(const uint32_t pWidth, const uint32_t pHeight)
{
    m_AspectRatio = static_cast<float>(pWidth) / static_cast<float>(pHeight);
    RecalculateProjection();
}

void Owl::SceneCamera::RecalculateProjection()
{
    const float orthoLeft = -m_OrthographicSize * 0.5f * m_AspectRatio;
    const float orthoRight = m_OrthographicSize * 0.5f * m_AspectRatio;
    const float orthoBottom = -m_OrthographicSize * 0.5f;
    const float orthoTop = m_OrthographicSize * 0.5f;

    m_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
}
