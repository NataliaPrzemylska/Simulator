#pragma once
#include <filesystem>
#include "Renderer/Core/Resources/Buffer.h"
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN

namespace Renderer {
	class Texture
	{
	public:
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
	public:
		void LoadTexture(std::string& path);
		void* loadTextureImage(std::string& path);
	private:

	};
}