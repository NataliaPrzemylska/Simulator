#include "Window.h"
#include <iostream>
#include "Application/Application.h"
namespace Renderer {
	void frameBufferResizeCallback(GLFWwindow* window, int width, int height) {
		Application::Get()->m_Renderer.m_FrameBufferResize = true;
	}
	void Window::init()
	{
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		//glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
		m_NativeWindow = glfwCreateWindow(600, 600, "Simulator", nullptr, nullptr);
		glfwSetWindowUserPointer(getNativeWindow(), this);
		glfwSetFramebufferSizeCallback(m_NativeWindow, frameBufferResizeCallback);
	}
	void Window::cleanUp()
	{
		glfwDestroyWindow(this->getNativeWindow());
		glfwTerminate();
	}
}