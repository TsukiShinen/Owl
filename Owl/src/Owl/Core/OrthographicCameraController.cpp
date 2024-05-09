#include "opch.h"
#include "OrthographicCameraController.h"

#include "Owl/Core/Input.h"

namespace Owl
{
	OrthographicCameraController::OrthographicCameraController(const float pRatioAspect, const bool pEnableRotation)
		: m_AspectRatio(pRatioAspect), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_IsRotationEnabled(pEnableRotation)
	{
	}

	void OrthographicCameraController::OnUpdate(const DeltaTime pDeltaTime)
	{
		if (Input::IsKeyPressed(Key::A))
			m_CameraPosition.x -= m_CameraTranslationSpeed * pDeltaTime;
		if (Input::IsKeyPressed(Key::D))
			m_CameraPosition.x += m_CameraTranslationSpeed * pDeltaTime;
		if (Input::IsKeyPressed(Key::W))
			m_CameraPosition.y += m_CameraTranslationSpeed * pDeltaTime;
		if (Input::IsKeyPressed(Key::S))
			m_CameraPosition.y -= m_CameraTranslationSpeed * pDeltaTime;

		m_Camera.SetPosition(m_CameraPosition);
		if (m_IsRotationEnabled)
		{
			if (Input::IsKeyPressed(Key::Q))
				m_CameraRotation += m_CameraRotationSpeed * pDeltaTime;
			if (Input::IsKeyPressed(Key::E))
				m_CameraRotation -= m_CameraRotationSpeed * pDeltaTime;
			m_Camera.SetRotation(m_CameraRotation);
		}
	}

	void OrthographicCameraController::OnEvent(Event& pEvent)
	{
		EventDispatcher dispatcher(pEvent);
		dispatcher.Dispatch<MouseScrolledEvent>(OWL_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(OWL_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(const MouseScrolledEvent& pEvent)
	{
		m_ZoomLevel -= pEvent.GetYOffset() * 0.25f;
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(const WindowResizeEvent& pEvent)
	{
		m_AspectRatio = static_cast<float>(pEvent.GetWidth()) / static_cast<float>(pEvent.GetHeight());
		m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
		
		return false;
	}
}
