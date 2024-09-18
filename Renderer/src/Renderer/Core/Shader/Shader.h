#pragma once
#include <filesystem>
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif
namespace Renderer {
	class Device;
	class Shader
	{
	public:
		void LoadShader(std::filesystem::path path);
		void CleanUp();
		VkShaderModule getShaderModule() { return m_NativeShaderModule; }
	private:
		VkShaderModule m_NativeShaderModule;
	};
}