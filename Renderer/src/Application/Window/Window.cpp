#include "Window.h"
#include <iostream>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
namespace Renderer {
	void Window::init()
	{
		//throw std::runtime_error("window couldn't be initalized");
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_NativeWindow = glfwCreateWindow(600, 600, "Simulator", nullptr, nullptr);
		glfwSetWindowUserPointer(getNativeWindow(), this);
		//glfwSetFramebufferSizeCallback(m_Window, frameBufferResizeCallback);
	}
	void Window::cleanUp()
	{
		glfwDestroyWindow(this->getNativeWindow());
		glfwTerminate();
	}
}