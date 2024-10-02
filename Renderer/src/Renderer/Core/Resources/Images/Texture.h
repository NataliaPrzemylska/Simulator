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
	class TextureManager;
	class Texture
	{
	public:
		void cleanUp();
		VkImage m_TextureImage;
		VkDeviceMemory m_TextureImageMemory;
	public:
		void LoadTexture(TextureManager* textureManager, const char* path);
		uint32_t width;
		uint32_t height;
	private:
	};
}