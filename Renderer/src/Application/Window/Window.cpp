#include "Window.h"
#include <iostream>
namespace Renderer {
	void Window::init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
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