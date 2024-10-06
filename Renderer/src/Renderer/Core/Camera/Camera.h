#pragma once
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
namespace Renderer {
	class Camera
	{
	public:
		virtual glm::mat4& getViewMatrix() = 0;
		virtual glm::mat4& getProjMatrix() = 0;
		virtual void OnUpdate() = 0;
	private:

	};
}