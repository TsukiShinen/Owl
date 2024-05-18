#include "opch.h"
#include "OrthographicCamera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Owl
{
	OrthographicCamera::OrthographicCamera(const float pLeft, const float pRight, const float pBottom, const float pTop)
		: m_ProjectionMatrix(glm::ortho(pLeft, pRight, pBottom, pTop, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		OWL_PROFILE_FUNCTION();
		
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::SetProjection(const float pLeft, const float pRight, const float pBottom, const float pTop)
	{
		OWL_PROFILE_FUNCTION();
		
		m_ProjectionMatrix = glm::ortho(pLeft, pRight, pBottom, pTop, -1.0f, 1.0f);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void OrthographicCamera::RecalculateViewMatrix()
	{
		OWL_PROFILE_FUNCTION();
		
		const glm::mat4 transform = translate(glm::mat4(1.0f), m_Position)
			* rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1));

		m_ViewMatrix = inverse(transform);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}
}
