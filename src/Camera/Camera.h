#pragma once
#include "Renderer/Core/Camera/Camera.h"
namespace Scene {
	class Camera : public Renderer::Camera
	{
	public:
		Camera();
		~Camera();
		glm::mat4& getViewMatrix() { return m_View; };
		glm::mat4& getProjMatrix() { return m_Proj; };
		void OnUpdate();
		glm::mat4 m_View;
		glm::mat4 m_Proj;
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Orientation;
	};
}