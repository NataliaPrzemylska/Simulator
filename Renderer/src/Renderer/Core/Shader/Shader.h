#pragma once
#include <filesystem>
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#endif
#include <GLFW/glfw3.h>
namespace Renderer {
	class Device;
	class Shader
	{
	public:
		void LoadShader(const Device& device, std::filesystem::path path);
		void CleanUp(const Device& device);
		VkShaderModule getShaderModule() { return m_NativeShaderModule; }
	private:
		VkShaderModule m_NativeShaderModule;
	};
}