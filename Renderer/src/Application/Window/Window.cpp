#include "Window.h"
#include <iostream>
#include "Application/Application.h"
namespace Renderer {
	void drop_callback(GLFWwindow* window, int count, const char** paths) {
		Application::Get()->getResourceManager().LoadModel(paths[0]);
	}
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
		glfwSetDropCallback(m_NativeWindow, drop_callback);
	}
	void Window::cleanUp()
	{
		glfwDestroyWindow(this->getNativeWindow());
		glfwTerminate();
	}
}