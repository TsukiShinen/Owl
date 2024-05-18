#include "opch.h"
#include "OrthographicCameraController.h"

#include "Owl/Core/Input.h"

namespace Owl
{
	OrthographicCameraController::OrthographicCameraController(const float pRatioAspect, const bool pEnableRotation)
		: m_AspectRatio(pRatioAspect),
		  m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel),
		  m_IsRotationEnabled(pEnableRotation)
	{
	}

	void OrthographicCameraController::OnUpdate(const DeltaTime pDeltaTime)
	{
		OWL_PROFILE_FUNCTION();
		
		if (Input::IsKeyPressed(Key::A))
		{
			m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * pDeltaTime;
			m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * pDeltaTime;
		}
		if (Input::IsKeyPressed(Key::D))
		{
			m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * pDeltaTime;
			m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * pDeltaTime;
		}
		if (Input::IsKeyPressed(Key::W))
		{
			m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * pDeltaTime;
			m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * pDeltaTime;
		}
		if (Input::IsKeyPressed(Key::S))
		{
			m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * pDeltaTime;
			m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * pDeltaTime;
		}

		m_Camera.SetPosition(m_CameraPosition);
		if (m_IsRotationEnabled)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * pDeltaTime;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * pDeltaTime;

			if (m_CameraRotation > 180.0f)
				m_CameraRotation -= 360.0f;
			else if (m_CameraRotation <= -180.0f)
				m_CameraRotation += 360.0f;

			m_Camera.SetRotation(m_CameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& pEvent)
	{
		OWL_PROFILE_FUNCTION();
		
		EventDispatcher dispatcher(pEvent);
		dispatcher.Dispatch<MouseScrolledEvent>(OWL_BIND_EVENT_FN(OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(OWL_BIND_EVENT_FN(OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(const MouseScrolledEvent& pEvent)
	{
		OWL_PROFILE_FUNCTION();
		
		m_ZoomLevel -= pEvent.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}

	bool OrthographicCameraController::OnWindowResized(const WindowResizeEvent& pEvent)
	{
		OWL_PROFILE_FUNCTION();
		
		m_AspectRatio = static_cast<float>(pEvent.GetWidth()) / static_cast<float>(pEvent.GetHeight());
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);

		return false;
	}
}
