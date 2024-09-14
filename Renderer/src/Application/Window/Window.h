#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
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