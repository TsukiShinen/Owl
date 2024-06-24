#pragma once
#include "Owl/Core/DeltaTime.h"
#include "Owl/Events/ApplicationEvent.h"
#include "Owl/Events/MouseEvent.h"
#include "Owl/Renderer/OrthographicCamera.h"

namespace Owl
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float pRatioAspect, bool pEnableRotation = false);

		void OnUpdate(DeltaTime pDeltaTime);
		void OnEvent(Event& pEvent);
		
		void OnResize(float pWidth, float pHeight);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }

		void SetZoomLevel(const float pLevel) { m_ZoomLevel = pLevel; }
		float GetZoomLevel() const { return m_ZoomLevel; }

	private:
		bool OnMouseScrolled(const MouseScrolledEvent& pEvent);
		bool OnWindowResized(const WindowResizeEvent& pEvent);
		float m_AspectRatio;
		float m_ZoomLevel = 1.0f;
		OrthographicCamera m_Camera;

		bool m_IsRotationEnabled;

		glm::vec3 m_CameraPosition = {0.f, 0.0f, 0.0f};
		float m_CameraRotation = 0.0f;
		float m_CameraTranslationSpeed = 1;
		float m_CameraRotationSpeed = 1;
	};
}
