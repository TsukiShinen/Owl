#pragma once
#include "glm/glm.hpp"

namespace Owl
{
	class OrthographicCamera
	{
	public:
		OrthographicCamera(float pLeft, float pRight, float pBottom, float pTop);

		void SetProjection(float pLeft, float pRight, float pBottom, float pTop);

		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPosition(const glm::vec3& pPosition) { m_Position = pPosition; RecalculateViewMatrix(); }

		float GetRotation() const { return m_Rotation; }
		void SetRotation(const float pRotation) { m_Rotation = pRotation; RecalculateViewMatrix(); }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();
	private:
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;
	};
}
