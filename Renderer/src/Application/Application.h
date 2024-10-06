#pragma once
#include "Renderer/Renderer.h"
#include "Renderer/Core/Resources/ResourceManager.h"
#include "Window/Window.h"
#include <memory>
#include "Bench.h"
#include "Renderer/Core/Camera/Camera.h"

namespace Renderer {
	class Application
	{
	public:
		static std::shared_ptr<Application> Get();
	private:
		static std::shared_ptr<Application> ApplicationInstance;
	public:
		void run();
	private:
		void mainLoop();
	private:
		Window m_Window;
	public:
		Renderer m_Renderer;
		Camera* m_Camera;
	public: // Getters
		GLFWwindow* getGLFWwindow() { return m_Window.getNativeWindow(); };
		ResourceManager& getResourceManager() { return m_Renderer.m_ResourceManager; }
		Renderer& getRenderer() { return m_Renderer; }
		VkDevice& getNativeDevice() { return m_Renderer.GetNativeDevice(); };
		Device& getDevice() { return m_Renderer.GetDevice(); };
	public: // Setters
		void setCamera(Camera* camera) { m_Camera = camera; };
	};
};