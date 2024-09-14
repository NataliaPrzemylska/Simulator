#pragma once
#include "Renderer/Renderer.h"
#include "Window/Window.h"
#include <memory>
namespace Renderer {
	class Application
	{
	public:
		static std::shared_ptr<Application> Get();
		void run();
		GLFWwindow* getGLFWwindow() { return m_Window.getNativeWindow(); };
	private:
		void mainLoop();
	private:
		Window m_Window;
		Renderer m_Renderer;
	private:
		static std::shared_ptr<Application> ApplicationInstance;
	};
};