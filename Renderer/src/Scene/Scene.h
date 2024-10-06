/*#pragma once
#include <vector>
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/glm.hpp>

namespace Renderer {
	struct TimeStep {
		float deltaTime;
	};
	enum ImageUsage
	{
		Image_Texture = 0,
		Image_GUI
	};
	enum SceneState {
		SceneState_Undefined = 0,
		SceneState_DontCare,
		SceneState_WaitingForLoading,
		SceneState_Loading,
		SceneState_Loaded,
		SceneState_Playing
	};
	struct SceneImageInfo {
		const char* path;
		ImageUsage usage;
	};
	class Scene
	{
	public:
		void cleanUp();
		std::vector<SceneImageInfo> m_Images;
		void onUpdate(const TimeStep ts);
	private:
		glm::vec3 m_Position;
		glm::vec3 m_Orientation;
	public:
		uint32_t m_State;
	};
}
*/