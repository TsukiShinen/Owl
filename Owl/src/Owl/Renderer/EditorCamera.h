#pragma once

#include "Camera.h"
#include "Owl/Core/DeltaTime.h"
#include "Owl/Events/Event.h"
#include "Owl/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Owl
{
	class EditorCamera : public Camera
	{
	public:
		EditorCamera() = default;
		EditorCamera(float pFov, float pAspectRatio, float pNearClip, float pFarClip);

		void OnUpdate(DeltaTime pDeltaTime);
		void OnEvent(Event& pEvent);

		[[nodiscard]] float GetDistance() const { return m_Distance; }
		void SetDistance(const float pDistance) { m_Distance = pDistance; }

		void SetViewportSize(const float pWidth, const float pHeight)
		{
			m_ViewportWidth = pWidth;
			m_ViewportHeight = pHeight;
			UpdateProjection();
		}

		[[nodiscard]] const auto& GetViewMatrix() const { return m_ViewMatrix; }
		[[nodiscard]] glm::mat4 GetViewProjection() const { return m_Projection * m_ViewMatrix; }

		[[nodiscard]] glm::vec3 GetUpDirection() const;
		[[nodiscard]] glm::vec3 GetRightDirection() const;
		[[nodiscard]] glm::vec3 GetForwardDirection() const;
		[[nodiscard]] const glm::vec3& GetPosition() const { return m_Position; }
		[[nodiscard]] glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

	private:
		void UpdateProjection();
		void UpdateView();

		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const glm::vec2& pDelta);
		void MouseRotate(const glm::vec2& pDelta);
		void MouseZoom(float pDelta);

		[[nodiscard]] glm::vec3 CalculatePosition() const;

		[[nodiscard]] std::pair<float, float> PanSpeed() const;
		[[nodiscard]] float RotationSpeed() const;
		[[nodiscard]] float ZoomSpeed() const;
		float m_FOV = 45.0f, m_AspectRatio = 1.778f, m_NearClip = 0.1f, m_FarClip = 1000.0f;

		glm::mat4 m_ViewMatrix;
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		glm::vec3 m_FocalPoint = {0.0f, 0.0f, 0.0f};

		glm::vec2 m_InitialMousePosition = {0.0f, 0.0f};

		float m_Distance = 10.0f;
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280, m_ViewportHeight = 720;
	};
}
