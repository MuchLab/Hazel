#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace Hazel {
	class OrthographicCamera {
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

		void SetProjectionMatrix(float left, float right, float bottom, float top);

		inline glm::vec3& GetPosition() { return m_Position; }
		inline void SetPosition(const glm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }

		inline float GetRotation() { return m_Rotation; }
		inline void SetRotation(const float rotation) { m_Rotation = rotation; RecalculateViewMatrix(); }

		inline glm::mat4 GetProjectionMatrix() { return m_ProjectionMatrix; }
		inline glm::mat4 GetViewMatrix() { return m_ViewMatrix; }
		inline glm::mat4 GetProjectionViewMatrix() { return m_ViewProjectionMatrix; }
	private:
		void RecalculateViewMatrix();

	private:
		glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
		float m_Rotation = 0.0f;

		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ViewProjectionMatrix;
	};

}

