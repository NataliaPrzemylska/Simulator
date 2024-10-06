/*#include "Scene.h"
#include "Application/Application.h"
#include <glm/gtc/matrix_transform.hpp>

void Renderer::Scene::cleanUp()
{
	m_Images.clear();
}

void Renderer::Scene::onUpdate(const TimeStep ts)
{
	m_Position = glm::vec3(2.0f, 2.0f, 2.0f);
	m_Orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	*(Application::Get()->getResourceManager().getProjViewMatrices().second) = glm::lookAt(m_Position, m_Orientation, glm::vec3(0,0,1.0f));
	glm::mat4& view = *(Application::Get()->getResourceManager().getProjViewMatrices().second);
	*(Application::Get()->getResourceManager().getProjViewMatrices().first) = glm::perspective(glm::radians(45.0f), Application::Get()->getRenderer().GetSwapChainExtent().width / (float)Application::Get()->getRenderer().GetSwapChainExtent().height, 0.1f, 10.0f);
	glm::mat4& proj = *(Application::Get()->getResourceManager().getProjViewMatrices().first);
}
*/