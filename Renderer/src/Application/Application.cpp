#include "Application.h"
#include "Bench.h"
#define CUBE_PATH TEXTURES_FOLDER_PATH "/cb.png"
namespace Renderer {
	std::shared_ptr<Application> Application::ApplicationInstance = std::make_shared<Application>();
	

	std::shared_ptr<Application> Application::Get()
	{
		return ApplicationInstance; 
	}

	void Application::run()
	{
		m_Window.init();
		m_Renderer.init();
		BEGIN_SESSION("main loop", "mainloop.json");
		this->mainLoop();
		END_SESSION();
		m_Renderer.cleanUp();
		m_Window.cleanUp();
	}

	void Application::mainLoop()
	{
		while (!glfwWindowShouldClose(m_Window.getNativeWindow())) {
			glfwPollEvents();
			m_Renderer.onImGuiRender();
			m_Renderer.onRender();
		}
	}

}