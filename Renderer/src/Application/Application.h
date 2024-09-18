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
		Renderer m_Renderer;
	private:
		void mainLoop();
	private:
		Window m_Window;
	private:
		static std::shared_ptr<Application> ApplicationInstance;
	public: // Getters
		Renderer& getRenderer() { return m_Renderer; }
		VkDevice& getNativeDevice() { return m_Renderer.GetNativeDevice(); };
		Device& getDevice() { return m_Renderer.GetDevice(); };
	};
};