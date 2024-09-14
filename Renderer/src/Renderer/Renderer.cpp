#include "Renderer.h"
#include <iostream>
#include <exception>
#include "Application/Application.h"
namespace Renderer {
	void Renderer::run() {
		throw std::runtime_error("??");
	}
	void Renderer::Renderer::init()
	{
		m_Device.init(Application::Get()->getGLFWwindow());

	}
	void Renderer::cleanUp()
	{
		m_Device.cleanUp();
	}
}