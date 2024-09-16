#pragma once
#ifndef RENDERER_INCLUDE_GLFW_VULKAN
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif // !RENDERER_INCLUDE_GLFW_VULKAN
#include <memory>
namespace Renderer {
	class Window
	{
	public:
		Window() : m_NativeWindow() {};
		void init();
		void cleanUp();
		GLFWwindow* getNativeWindow() { return m_NativeWindow; };
	private:
		GLFWwindow* m_NativeWindow;
	};
}