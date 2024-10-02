#pragma once
#include <vector>
namespace Renderer {
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
		std::vector<SceneImageInfo> m_Images;
		SceneState m_State = SceneState_DontCare;
	private:

	};
}