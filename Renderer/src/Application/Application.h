#pragma once
#include "Renderer/Renderer.h"
#include "Renderer/Core/Resources/ResourceManager.h"
#include "Window/Window.h"
#include <memory>
#include "Bench.h"
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
		ResourceManager& getResourceManager() { return m_Renderer.m_ResourceManager; }
		Renderer& getRenderer() { return m_Renderer; }
		VkDevice& getNativeDevice() { return m_Renderer.GetNativeDevice(); };
		Device& getDevice() { return m_Renderer.GetDevice(); };
	};
};