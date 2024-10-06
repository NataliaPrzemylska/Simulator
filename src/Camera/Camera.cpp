#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Application/Application.h"

Scene::Camera::Camera() : m_Position(2.0f, 2.0f, 2.0f), m_Orientation(0.0f, 0.0f, 0.0f), m_View(1.f), m_Proj(1.f)
{
	m_View = glm::lookAt(m_Position, m_Orientation, glm::vec3(0, 0, 1.0f));
}

Scene::Camera::~Camera()
{
}

void Scene::Camera::OnUpdate()
{
	m_View = glm::lookAt(m_Position, m_Orientation, glm::vec3(0, 0, 1.0f));
	m_Proj = glm::perspective(glm::radians(45.0f), Renderer::Application::Get()->getRenderer().GetSwapChainExtent().width / (float)Renderer::Application::Get()->getRenderer().GetSwapChainExtent().height, 0.1f, 10.0f);

}
